  这一节关于底层的代码一律不讲，因为ST官方已经提供了代码了，我们只需要理解如何移植即可，最底层编写的东西过于复杂，不需要我们自己干。

  在ST的固件库里面的Utilities有他们编写好的代码，我们拷贝一份到我们的工程目录里的user里面

  <img width="1222" height="390" alt="image" src="https://github.com/user-attachments/assets/15bd95c2-aaca-4701-b4cb-33aaad2bdc62" />

  拷贝好后，这一份移植的代码并不可以直接用，下面讲解移植时需要修改的地方。

  1.更改头文件：打开stm32_eval_sdio_sd.h文件中，你会发现一个头文件为stm32_eval.h的，这里我们没有，把他换成stm32f10x.h头文件

  2.增加宏定义：

    1.在stm32_eval_sdio_sd.h文件中，我们需要添加SDIO外设FIFO地址(SDIO 进行传输时，数据会被存储在 FIFO)，FIFO的起始地址可以从可从《STM32 参考手册》的 SDIO 寄存器说明中查询到
    2.定义卡识别模式模式和数据传输模式下下的时钟分频因子:他们俩者共用一个时钟源SDIO_CK,在卡识别模式时要求不超过 400KHz，而在识别后的数据传输模式时则希望有更高的速度（最大不超过 25MHz）。
    因此需要配置俩者的分频因子：SDIO_INIT_CLK_DIV 分频因子用于卡识别模式，SDIO_TRANSFER_CLK_DIV 用于数据传输模式

 3.编写SD_LowLevel_DeInit()函数：

    1.先关闭SDIO时钟使能
    2.关闭SDIO电源
    3.调用SDIO_DeInit()函数
    4.关闭SDIO时钟源
    5.初始化GPIO引脚

4.编写SD_LowLevel_Init()函数：

    1.配置GPIO引脚
    2.配置中断NVIC

5.开启DMA能加快传输速率，蓑衣我还需要编写DMA2的函数，分别是SD_LowLevel_DMA_RxConfig 函数用于配置 DMA 的 SDIO 接收请求参数，并指定接收存储器地址和大小；
SD_LowLevel_DMA_TxConfig 函数用于配置 DMA 的 SDIO 发送请求参数，并指定发送存储器地址和大小。这俩个代码非常类似，只不过是数据的方向和来源不一样而已。代码如下：

    void SD_DMA_TxConfig(uint32_t *BufferSRC, uint32_t BufferSize)
    {
    
      DMA_InitTypeDef DMA_InitStructure;
    
      DMA_ClearFlag(DMA2_FLAG_TC4 | DMA2_FLAG_TE4 | DMA2_FLAG_HT4 | DMA2_FLAG_GL4);
    
      /*!< DMA2 Channel4 disable */
      DMA_Cmd(DMA2_Channel4, DISABLE);
    
      /*!< DMA2 Channel4 Config */
      DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SDIO_FIFO_ADDRESS;
      DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)BufferSRC;
      DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//ÍâÉèÎªÐ´ÈëÄ¿±ê
      DMA_InitStructure.DMA_BufferSize = BufferSize / 4;
      DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//ÍâÉèµØÖ·²»×ÔÔö
      DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
      DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
      DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
      DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
      DMA_InitStructure.DMA_Priority = DMA_Priority_High;
      DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
      DMA_Init(DMA2_Channel4, &DMA_InitStructure);
    
      /*!< DMA2 Channel4 enable */
      DMA_Cmd(DMA2_Channel4, ENABLE);  
    }


    void SD_DMA_RxConfig(uint32_t *BufferDST, uint32_t BufferSize)
    {
      DMA_InitTypeDef DMA_InitStructure;
    
      DMA_ClearFlag(DMA2_FLAG_TC4 | DMA2_FLAG_TE4 | DMA2_FLAG_HT4 | DMA2_FLAG_GL4);//Çå³ýDMA±êÖ¾Î»
    
      /*!< DMA2 Channel4 disable */
      DMA_Cmd(DMA2_Channel4, DISABLE);	//SDIOÎªµÚËÄÍ¨µÀ
    
      /*!< DMA2 Channel4 Config */
      DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SDIO_FIFO_ADDRESS;  //ÍâÉèµØÖ·£¬fifo
      DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)BufferDST; //Ä¿±êµØÖ·
      DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	//ÍâÉèÎªÔ´µØÖ·
      DMA_InitStructure.DMA_BufferSize = BufferSize / 4;  //³ýÒÔ4£¬°Ñ×Ö×ª³É×Ö½Úµ¥Î»
      DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//Ê¹ÄÜÍâÉèµØÖ·²»×ÔÔö
      DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	  //Ê¹ÄÜ´æ´¢Ä¿±êµØÖ·×ÔÔö
      DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;  //ÍâÉèÊý¾Ý´óÐ¡Îª×Ö£¬32Î»
      DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;	//ÍâÉèÊý¾Ý´óÐ¡Îª×Ö£¬32Î»
      DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;			   //²»Ñ­»·£¬Ñ­»·Ä£Ê½Ö÷ÒªÓÃÔÚadcÉÏ
      DMA_InitStructure.DMA_Priority = DMA_Priority_High;	 //Í¨µÀÓÅÏÈ¼¶¸ß
      DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;			 //·Ç ´æ´¢Æ÷ÖÁ´æ´¢Æ÷Ä£Ê½
      DMA_Init(DMA2_Channel4, &DMA_InitStructure);
    
      /*!< DMA2 Channel4 enable */			   //²»ÉèÖÃdmaÖÐ¶Ï£¿
      DMA_Cmd(DMA2_Channel4, ENABLE); 
    }
    

在这里我把函数名改了一下而已，其实没啥区别，不改还好一点，一旦改了下面的调用了这个函数的函数名都得换。

6.配置中断，写等待SD_WaitReadOperation()和读等待SD_WaitWriteOperation()函数依赖中断标志位，所有需要加上。代码如下：

    void SDIO_IRQHandler(void) 
    {
      /* Process All SDIO Interrupt Sources */
      SD_ProcessIRQSrc();
    }


完成以上步骤，则SDIO文件移植完成，可以正常使用了。下面讲解读写过程中需要注意的点。
