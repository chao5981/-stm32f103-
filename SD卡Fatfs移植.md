  其实在flash读写程序中，也能用到Fatfs，但是由于未知的原因，导致fatfs移植到flash读写程序上并不成功，会出现显示写入成功但实际上没有成功的情况。不知道是我代码的问题导致的芯片损坏还是芯片本来就不良，于是我没有放上去。

  这里统一来讲一下fatfs。

  FatFs 是面向小型嵌入式系统的一种通用的 FAT 文件系统。它完全是由 ANSIC 语言编写并且完全独立于底层的 I/O 介质。因此它可以很容易地不加修改地移植到其他的处理器当中，如 8051、PIC、AVR、SH、Z80、H8、ARM 等。

  FatFs 文件系统的源码可以从 fatfs 官网下载:http://elm-chan.org/fsw/ff/00index_e.html

  fatfs文件调用逻辑如图所示：
  <img width="967" height="405" alt="image" src="https://github.com/user-attachments/assets/36d5ccf1-6d9c-485c-bd15-1d1f5098d292" />


  application层是我们最顶端的函数调用，fatfs配件我们需要稍微改动ffconf.h里面的几个宏定义；为了fatfs组件能适配Flash/SD卡，我们需要在diskio.c文件中编写相关的函数，把最底层的Flash/SD卡的读写函数以合理的顺序写入到diskio.c文件
中的读写函数中，以便于上层函数的调用。

  下面讲解移植需要做的事项，其实可以直接移植，这个不理解也无妨：

  1.修改ffconf.h文件中的以下的宏定义：

        #define _USE_MKFS 1    //_USE_MKFS：格式化功能选择，为使用 FatFs 格式化功能，需要把它设置为 1。
        #define _CODE_PAGE 936    //_CODE_PAGE：语言功能选择，并要求把相关语言文件添加到工程宏。为支持简体中文文件名需要使用“936”
        #define _USE_LFN 2        //_USE_LFN：长文件名支持，默认不支持长文件名，这里配置为 2，支持长文件名，并指定使用栈空间为缓冲区。
        #define _VOLUMES 2        //_VOLUMES：指定物理设备数量，这里设置为 2，包括预留 SD 卡和 SPI Flash 芯片。
        #define _MIN_SS 512      //MIN_SS 、_MAX_SS：指定扇区大小的最小值和最大值。SD 卡扇区大小一般都为 512 字节
        #define _MAX_SS 4096      //SPI Flash 芯片扇区大小一般设置为 4096 字节，所以需要把 _MAX_SS 改为 4096。


  编写diskio.c文件的各个函数

  2.宏定义设备序号

      /* 为每个设备定义一个物理编号 */
    #define ATA			           0     // SD卡
    #define SPI_FLASH		       1     // 预留外部SPI Flash使用
    
    #define SD_BLOCKSIZE     512 

 3.编写disk_status - 获取设备状态

     默认状态为STA_NOINIT(未初始化)
    
    对于SD卡(ATA)，清除STA_NOINIT标志表示已准备好
    
    对于SPI Flash，保持未初始化状态(当前未实现)
    
    其他设备编号返回未初始化状态
    
          
  4. disk_initialize - 设备初始化

    对于SD卡： 调用SD_Init()进行初始化， 成功则清除STA_NOINIT标志，失败则保持未初始化状态
    
    对于SPI Flash，保持未实现状态
    
    其他设备编号返回未初始化状态

  5.disk_read - 读取扇区

      对于SD卡：检查缓冲区是否4字节对齐，如果不对齐，使用临时对齐缓冲区逐个扇区读取，读取后复制到目标缓冲区，
      对齐的缓冲区直接调用SD_ReadMultiBlocks读取多个块，等待读取操作完成(SD_WaitReadOperation)，检查传输状态直到完成， 根据操作结果返回成功或错误
    
      对于SPI Flash，保持未实现状态

  6.disk_write - 写入扇区

    默认返回参数错误(RES_PARERR)，检查count参数有效性
    
    对于SD卡：检查缓冲区是否4字节对齐，如果不对齐，使用临时对齐缓冲区逐个扇区写入，先将数据复制到临时缓冲区
    对齐的缓冲区直接调用SD_WriteMultiBlocks写入多个块，等待写入操作完成(SD_WaitWriteOperation)，检查传输状态直到完成，根据操作结果返回成功或错误
    
    对于SPI Flash，保持未实现状态

  7.disk_ioctl - 设备控制

    默认返回参数错误(RES_PARERR)
    
    对于SD卡支持以下命令：
    
        GET_SECTOR_SIZE: 获取扇区大小(512字节)
        
        GET_BLOCK_SIZE: 获取擦除块大小(1个扇区)
        
        GET_SECTOR_COUNT: 获取总扇区数(卡容量/块大小)
        
        CTRL_SYNC: 同步命令(空操作)
    
    对于SPI Flash，保持未实现状态

  8.和SD卡读写一样配置中断函数

  这里解释以下什么是4字节对齐。STM32 的 DMA (直接内存访问) 和某些硬件加速操作（如 SDIO）通常要求数据缓冲区是 4 字节对齐（32 位对齐）。否则会出现硬件异常（如 HardFault），性能下降（CPU 可能需要多次访问内存）数据错误（DMA 可能无法正确传输数据）。而在USART，ADC等低速环境下，不需要四字节对齐。

  完成以上配置就算是底层函数都配置完成了，然后就可以调用fatfs里的函数了，具体含义可以直接用浏览器的翻译软件，都是能看懂的。

  这里我留几个疑问，以后再来解决问题

  1.如果按照我的main函数执行，程序在续写实验就会卡住
  
  2.f_stat函数会卡死
  
  3.如果不进行读写测试实验，直接调用f_opendir打开已有的文件夹会出现FR_DISK_ERR报错

  4.直接全部复制sdio文件的全部代码，keil会出现declaration is incompatible报错，指向读写函数

  5.如果全部复制，会出现keil会弹出could not write file警告

  6.如果用的是老版keil，并且下载器的CMSIS-DAP-Cortex-M Targer Driver Setup中的Debug区域的connect是设置没有改成under set，烧录时会出现Erase Failed! Error: Flash Download failed - "Cortex-M3"报错

  一切的问题都很稀奇古怪，我用不清楚是什么原因，网上也查不到好的解决方法。所以这个后面再处理吧。

  附上我的程序的实验效果图：

  实验前：
  <img width="1272" height="494" alt="d0962c6ad9914bf00123ac3292e2ec4" src="https://github.com/user-attachments/assets/a42c9146-89fb-4e58-948e-1240564cbde2" />
  <img width="774" height="408" alt="1c7a2dcec7ff39850c4f6e81338e0f5" src="https://github.com/user-attachments/assets/ba70e93e-324b-48e9-94e1-3b7d0202f520" />
  <img width="1062" height="403" alt="b93e39b97a7c3328f8501357980d10c" src="https://github.com/user-attachments/assets/9e3ea645-8653-48eb-bf28-8ecea99d0f63" />


  实验后：
  <img width="1194" height="595" alt="8671548da1d9bb40c2202f651e59141" src="https://github.com/user-attachments/assets/22168adf-e9e9-4f54-a851-11587b54edf2" />
  <img width="853" height="548" alt="8e7c895aa957151068d446e4ffa6a7b" src="https://github.com/user-attachments/assets/5cdc234c-0ba8-4774-89ae-6bfdb2cd254e" />
  <img width="985" height="395" alt="521320535e8e77c95e67a7f7fc4e33a" src="https://github.com/user-attachments/assets/f7e765aa-0a4e-43a0-9756-20f5a87e3c45" />


