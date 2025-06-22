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

  
  下面谈谈如何使能FSMC,这里我以Bank1_SRAM1，用模式A为例子。
  ![image](https://github.com/user-attachments/assets/61cbcc9b-7698-40cc-9719-351cdae2e25e)

  1.初始化FSMC的所有GPIO引脚，GPIO引脚模式配置为复用功能(这由于FSMC的硬件要求，必须由FSMC直接驱动，而不是GPIO简单的输入输出模式，复用模式就是把引脚的控制权交给FSMC)，速度为50HZ。
  2.配置FSMC相关的结构体并且使能
    ①.配置NOR Flash/SRAM的时序参数(配置FSMC_NORSRAMTimingInitTypeDef结构体)
    
      a. FSMC_AddressSetupTime（地址建立时间），作用：告诉存储器：“我要给你发地址了，你先准备一下，等我稳定了再干活！”。对应的是时序图的"ADDSRT"的大小。但是ADDSET并不是由FSMC决定的，而是由你外部寄存器决定的。若要确定ADDSET的大小，你需要去查阅外接的寄存器的t_AH/t_SU(addr)参数(单位:us)，计算公式为ADDSET≥t_AH/T(HCLK)-1，HCLK的周期由你的决定(大部分情况下会默认是72HZ)。需要注意的是，ADDSET在stm32的参考手册上明确提到必须≥0。我的参数是瞎选作为示例的.
      
      b.FSMC_AddressHoldTime(地址保持时间)，作用：地址发完后，再多保持一段时间，确保存储器能稳稳地锁存地址。由于现代存储器速度够快，不需要额外保持，通常设为0。
      
      c.FSMC_DataSetupTime（数据建立时间），作用：数据信号（读或写）需要保持稳定的最短时间。对应的是时序图的"DATAST"的大小。但是DATAST并不是由FSMC决定的，而是由你外部寄存器决定的。若要确定DATAST的大小，你需要去查阅外接的寄存器的t_HD(data)参数(单位:us)，计算公式为ADDSET≥t_HD(data)/T(HCLK)-1，HCLK的周期由你的决定(大部分情况下会默认是72HZ)。需要注意的是，DATAST在stm32的参考手册上明确提到必须＞0，也就是说，至少为1。我的参数是瞎选作为示例的，实际上很少见DATAST这么短的。至少都为5/6.
      
      d.FSMC_BusTurnAroundDuration（总线周转时间），作用：从“读”切换到“写”（或反过来）时，总线需要“喘口气”的时间。异步模式通常设为0（直接切换），同步模式可能需要1~2周期。
      
      e.FSMC_CLKDivision（时钟分频），作用：只在同步模式下有用，决定FSMC_CLK时钟的分频系数（比如HCLK/2），异步模式下直接设为0（不需要时钟）。
      
      f.FSMC_DataLatency（数据延迟），作用：同步模式下，存储器可能需要几个时钟周期后才能返回数据（比如Burst读NOR Flash），异步模式下固定为0。
      
      g.FSMC_AccessMode（访问模式），作用：选择四种握手模式（A/B/C/D），决定控制信号（NOE、NWE）的触发时机。这里我选择的是模式A。FMSC相比于传统的GPIO模拟的优势就在这里，你不用去根据时序图用GPIO去模拟，而是你只要把那几个引脚交给FSMC，然后设定好时间，选定好模式，再配置下面我所说的结构体，他就能完成对外寄存器访问的操作，就和操作自己内部的寄存器一样。
      
  ②.配置NOR Flash/SRAM的存储区域、数据宽度、模式等(配置FSMC_NORSRAMInitTypeDef结构体)
  
    a. FSMC_Bank（存储区域选择），作用：选择 FSMC 控制的“银行”（Bank），比如 FSMC_Bank1_NORSRAM1到4。就像选择电脑的 USB 接口（USB1/USB2/USB3），不同接口接不同设备。这里我选择的是bank1_NORSRAM1.
    
    b.FSMC_DataAddressMux（数据/地址复用），作用：是否让数据线和地址线共用同一组引脚（类似“一线两用”）。一般我们禁止复用。
    
    c.FSMC_MemoryType（存储器类型），作用：告诉 FSMC 你接的是 NOR Flash 还是 SRAM。
    
    d.FSMC_MemoryDataWidth（数据宽度），作用：选择数据总线是8位还是16位
    
    e.FSMC_BurstAccessMode（突发访问模式），作用：是否启用“连发”模式（一次发多个地址，连续读写）。由于普通 SRAM 用不到突发模式，一般我们直接disable。
    
    f.FSMC_AsynchronousWait（异步等待），作用：是否让存储器通过信号线告诉 FSMC“我还没准备好，等等！”。由于一般 SRAM 不需要这个功能，一般我们disable。
    
    g.FSMC_WaitSignalPolarity（等待信号极性），作用：规定“等待信号”是高电平有效还是低电平有效（如果启用异步等待），一般我们设置低电平有效
    
    h.FSMC_WrapMode（回绕模式），作用：突发传输时，是否让地址自动“绕回来”（像环形缓冲区）。由于普通 SRAM 用不到突发模式，更何况回绕模式，一般disable
    
    i.FSMC_WaitSignalActive（等待信号阶段）,作用：在突发传输的哪个阶段插入“等待信号”（如果启用）。由于普通 SRAM 用不到突发模式，其实可以随便选，就算用，常用值为BeforeWaitState（在等待状态前插入）。
    
    j.FSMC_WriteOperation（写使能），作用：是否允许写入存储器（必须设为 Enable，否则只能读！）。
    
    k.FSMC_WaitSignal（等待信号使能），作用：是否启用“等待信号”功能（一般 SRAM 不需要），所以常设为disable
    
    l.FSMC_ExtendedMode（扩展模式），作用：是否对“读时序”和“写时序”分开配置（像分开设置上班和下班时间），一般读写用同一套时序，因此我们常设为disable
    
    m.FSMC_WriteBurst（突发写使能），作用：是否启用“连续写入”模式（类似批量发送），由于普通 SRAM 用不到，因此常用disable
    
    n.FSMC_ReadWriteTimingStruct（读写时序），作用：绑定一个时序配置结构体（FSMC_NORSRAMTimingInitTypeDef），设置地址建立时间、数据保持时间，就是把上面你配置的FSMC_NORSRAMTimingInitTypeDef赋值进去，所以FSMC_NORSRAMTimingInitTypeDef结构体要先配置。
    
    o.FSMC_WriteTimingStruct（写时序），作用：如果启用扩展模式（ExtendedMode = Enable），可以单独配置写时序；不启用扩展模式时设为 NULL。
    
3.初始化FSMC_NORSRAMInitTypeDef结构体
4.使能这个结构体
5.编写读写数据的函数，这里我用的是指针直接操作，就和C语言的指针写入差不多。
6.对于查看内存利用率，我用的是类似于C语言的malloc库，这里不讲如何编写，直接移植会用就好。

接下来看我的主函数，写入或者读取确实可以直接用指针操作(例如分配内存然后sprintf)，这样做方便，不用管大小的问题；但像写在哪里/读哪里的数据，用自己编写的读写函数是最方便的。
