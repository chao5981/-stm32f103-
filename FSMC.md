  FSMC(灵活的静态存储控制器),可以用来管理扩展的寄存器(SRAM,NOR FLASH,NAND FLASH等)。在我们现阶段的学习中，我们大概率用不到他，因为stm32芯片内里面的RAM和Flash内存已经够我们做实验了，但是在实际工程应用中，FMSC有巨大的利用
价值。例如：在图像处理中，我们可以用它外接SRAM或者NOR FLASH去缓存高分辨率的图像数据；或者外界NAND FLASH和SD卡存储大量数据，例如行车记录仪的视频存储；或者用来驱动LCD的显示模块。
  但由于开发板的限制，我现在无法进行实验展示，这里我们只是简要学习一下。
  下面是FSMC的框图：
  ![image](https://github.com/user-attachments/assets/30c4acd3-ab50-4a2a-9df7-af9bddfb8f89)
  
  我们可以看到，引脚分为公共引脚和不同储存器对应的特殊引脚。我来解释一下各个引脚的意思：
  公用信号
  FSMC_A[25:0] ：地址线，用于传输外部存储器的地址信息 ，确定要访问的存储单元位置。
  FSMC_D[15:0] ：数据线，实现 STM32 与外部存储器之间的数据传输，可传输读或写的数据 。
  FSMC_NOE ：输出使能信号，低电平有效。当该信号有效时，允许外部存储器将数据输出到数据总线上，供 STM32 读取 。
  FSMC_NWE ：写使能信号，低电平有效。有效时，STM32 可向外部存储器写入数据 。
  FSMC_NWAIT ：等待信号，由外部存储器反馈给 STM32。若外部存储器来不及响应数据传输，可通过此信号让 STM32 插入等待周期，以协调不同速度设备间的数据传输 。

  
  NOR/PSRAM 信号
  FSMC_NE[4:1] ：NOR/PSRAM 片选信号，低电平有效 。用于选择连接在 FSMC 上的不同 NOR/PSRAM 存储设备或存储区域，不同引脚对应不同地址区域 。
  FSMC_NL (or NADV) ：在 NOR Flash 中，可作为地址有效信号（NADV），指示地址线上的地址是否有效；在 PSRAM 中，可作为低电平有效信号（NL ），具体功能依模式而定 。
  FSMC_NBL[1:0] ：字节选择信号，低电平有效 。用于选择访问 16 位数据中的高字节或低字节，或使能 8 位数据访问 。
  FSMC_CLK ：时钟信号，为 NOR/PSRAM 提供同步时钟，用于同步数据传输和操作时序 。
  FSMC_NCE4_2 ：额外的片选信号，用于进一步细分或选择特定的存储设备或区域 。

  
  NAND 信号
  FSMC_NCE[3:2] ：NAND Flash 片选信号，低电平有效 。用于选择连接的 NAND Flash 设备 。
  FSMC_INTR[3:2] ：NAND Flash 中断信号，用于 NAND Flash 向 STM32 发送中断请求，表明有特定事件发生（如操作完成、错误等） 。

  
  PC 卡信号
  FSMC_INTR ：PC 卡中断信号，PC 卡通过此信号向 STM32 发起中断请求 。
  FSMC_NCE4_1、FSMC_NCE4_2 ：PC 卡相关的片选信号，用于选择 PC 卡设备或特定区域 。
  FSMC_NIORD ：PC 卡读使能信号，低电平有效。有效时允许从 PC 卡读取数据 。
  FSMC_NIOWR ：PC 卡写使能信号，低电平有效。有效时允许向 PC 卡写入数据 。
  FSMC_NIOS16 ：用于选择 PC 卡数据总线宽度为 16 位，当该信号有效时，PC 卡以 16 位数据宽度进行数据传输 。
  FSMC_NREG ：PC 卡寄存器选择信号，用于选择访问 PC 卡内部不同寄存器 。
  FSMC_CD ：卡检测信号，用于检测 PC 卡是否插入 。


  那么,FSMC是如何工作的呢？下面是FSMC的地址映射图：
  ![image](https://github.com/user-attachments/assets/901b8357-f2ae-46df-9004-d65f124bf68d)
  
  我们可以通过访问内核的地址，通过地址映射到FSMC外设，从而访问到外接寄存器。

  从FSMC的角度看，可以把外部存储器划分为固定大小为256M字节的四个存储块
  存储块1用于访问最多4个NOR闪存或PSRAM存储设备。这个存储区被划分为4个NOR/PSRAM区并有4个专用的片选。
  存储块2和3用于访问NAND闪存设备，每个存储块连接一个NAND闪存。
  存储块4用于访问PC卡设备

  在bank区域内，我们还可以进行分区，如图所示：
  ![image](https://github.com/user-attachments/assets/39fb8f2a-5c3c-4405-a9e7-4554412a3fdc)
  
  对于不同的外接寄存器，读写方式也一定有区别，FSMC提供了多种模式。这个可以在参考手册中查阅。

  

