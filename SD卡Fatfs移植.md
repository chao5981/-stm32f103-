  其实在flash读写程序中，也能用到Fatfs，但是由于未知的原因，导致fatfs移植到flash读写程序上并不成功，会出现显示写入成功但实际上没有成功的情况。不知道是我代码的问题导致的芯片损坏还是芯片本来就不良，于是我没有放上去。

  这里统一来讲一下fatfs。

  FatFs 是面向小型嵌入式系统的一种通用的 FAT 文件系统。它完全是由 ANSIC 语言编写并且完全独立于底层的 I/O 介质。因此它可以很容易地不加修改地移植到其他的处理器当中，如 8051、PIC、AVR、SH、Z80、H8、ARM 等。

  FatFs 文件系统的源码可以从 fatfs 官网下载:http://elm-chan.org/fsw/ff/00index_e.html

  fatfs文件调用逻辑如图所示：
  <img width="967" height="405" alt="image" src="https://github.com/user-attachments/assets/36d5ccf1-6d9c-485c-bd15-1d1f5098d292" />


  application层是我们最顶端的函数调用，fatfs配件我们需要稍微改动ffconf.h里面的几个宏定义；为了fatfs组件能适配Flash/SD卡，我们需要在diskio.c文件中编写相关的函数，把最底层的Flash/SD卡的读写函数以合理的顺序写入到diskio.c文件
中的读写函数中，以便于上层函数的调用。

  下面讲解移植需要做的事项：

  1.修改ffconf.h文件中的以下的宏定义：

        #define _USE_MKFS 1    //_USE_MKFS：格式化功能选择，为使用 FatFs 格式化功能，需要把它设置为 1。
        #define _CODE_PAGE 936    //_CODE_PAGE：语言功能选择，并要求把相关语言文件添加到工程宏。为支持简体中文文件名需要使用“936”
        #define _USE_LFN 2        //_USE_LFN：长文件名支持，默认不支持长文件名，这里配置为 2，支持长文件名，并指定使用栈空间为缓冲区。
        #define _VOLUMES 2        //_VOLUMES：指定物理设备数量，这里设置为 2，包括预留 SD 卡和 SPI Flash 芯片。
        #define _MIN_SS 512      //MIN_SS 、_MAX_SS：指定扇区大小的最小值和最大值。SD 卡扇区大小一般都为 512 字节
        #define _MAX_SS 4096      //SPI Flash 芯片扇区大小一般设置为 4096 字节，所以需要把 _MAX_SS 改为 4096。


  2.
