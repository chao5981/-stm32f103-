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

6.写DMA结束传输标志函数，这里直接附上代码

    uint32_t SD_DMAEndOfTransferStatus(void)
    {
      return (uint32_t)DMA_GetFlagStatus(DMA2_FLAG_TC4);   //Channel4 transfer complete flag. 
    
    }
    
7.配置中断，写等待SD_WaitReadOperation()和读等待SD_WaitWriteOperation()函数依赖中断标志位，所有需要加上。代码如下：

    void SDIO_IRQHandler(void) 
    {
      /* Process All SDIO Interrupt Sources */
      SD_ProcessIRQSrc();
    }


完成以上步骤，则SDIO文件移植完成，可以正常使用了。下面先介绍其中的函数：

1.初始化与反初始化函数

    1.void SD_DeInit(void);  功能：将 SD 卡接口相关的硬件和软件配置恢复到初始状态，释放资源，通常用于系统关闭或重新配置  常用于底层
    2.SD_Error SD_Init(void);  功能：初始化 SD 卡接口，包括配置 SDIO 外设、时钟、引脚等，使 SD 卡能够正常通信。

2.状态获取函数

    1.SDTransferState SD_GetStatus(void); 功能：获取 SD 卡当前的数据传输状态，如传输正常、传输繁忙或传输错误。用于DMA数据传输检测
    2.SDCardState SD_GetState(void); 功能：获取 SD 卡当前的工作状态，如就绪、识别、待机、传输等。 用于忙状态检测，但是写忙和读忙都有自己的函数，一般这样用于等待擦除忙完成
    3.uint8_t SD_Detect(void);  功能：检测 SD 卡是否插入卡槽。  常用于底层

3.电源管理函数

    1.SD_Error SD_PowerON(void);  功能：为 SD 卡供电，使 SD 卡进入可操作状态。
    2.SD_Error SD_PowerOFF(void);  功能：关闭 SD 卡的电源，通常在系统低功耗模式或不需要使用 SD 卡时调用。

4.卡片初始化与信息获取函数

    1.SD_Error SD_InitializeCards(void);  功能：对插入的 SD 卡进行初始化操作，使其能够正常通信和读写。
    2.SD_Error SD_GetCardInfo(SD_CardInfo *cardinfo);  功能：获取 SD 卡的详细信息，如容量、块大小、CID、CSD 等，并存储在 SD_CardInfo 结构体中。
    3.SD_Error SD_GetCardStatus(SD_CardStatus *cardstatus);  功能：获取 SD 卡的状态信息，如数据总线宽度、安全模式等，并存储在 SD_CardStatus 结构体中。

5.总线操作与卡片选择函数

    1.SD_Error SD_EnableWideBusOperation(uint32_t WideMode);  功能：启用 SD 卡的宽总线操作模式，如 4 位或 8 位总线。这个常用于底层配置，一般我们用不到
    2.SD_Error SD_SelectDeselect(uint32_t addr);  功能：选择或取消选择指定地址的 SD 卡，用于多卡系统或需要切换操作的卡片。

6.读写操作函数

    1.SD_Error SD_ReadBlock(uint8_t *readbuff, uint64_t ReadAddr, uint16_t BlockSize);  功能：从 SD 卡的指定地址读取一个数据块到指定的缓冲区。
    2.SD_Error SD_ReadMultiBlocks(uint8_t *readbuff, uint64_t ReadAddr, uint16_t BlockSize, uint32_t NumberOfBlocks);  功能：从 SD 卡的指定地址连续读取多个数据块到指定的缓冲区。
    3.SD_Error SD_WriteBlock(uint8_t *writebuff, uint64_t WriteAddr, uint16_t BlockSize);  功能：将指定缓冲区中的一个数据块写入 SD 卡的指定地址。
    4.SD_Error SD_WriteMultiBlocks(uint8_t *writebuff, uint64_t WriteAddr, uint16_t BlockSize, uint32_t NumberOfBlocks);  功能：将指定缓冲区中的多个数据块连续写入 SD 卡的指定地址。

7.传输状态与控制函数

    1.SDTransferState SD_GetTransferState(void);  功能：获取 SD 卡当前的数据传输状态，用于底层
    2.SD_Error SD_StopTransfer(void);  功能：停止当前正在进行的 SD 卡数据传输操作。

8.擦除与状态发送函数

    1.SD_Error SD_Erase(uint32_t startaddr, uint32_t endaddr);  功能：擦除 SD 卡指定地址范围内的数据块。
    2.SD_Error SD_SendStatus(uint32_t *pcardstatus);  功能：向 SD 卡发送请求，获取 SD 卡的状态信息，并存储在 pcardstatus 指向的变量中。  用于底层
    3.SD_Error SD_SendSDStatus(uint32_t *psdstatus);  功能：向 SD 卡发送请求，获取 SD 卡的特定状态信息，并存储在 psdstatus 指向的变量中。  用于底层

9.中断处理与等待函数

    1.SD_Error SD_ProcessIRQSrc(void);  功能：处理 SD 卡接口的中断源，根据中断类型执行相应的操作。
    2.SD_Error SD_WaitReadOperation(void);  功能：等待 SD 卡的读取操作完成。  
    3.SD_Error SD_WaitWriteOperation(void);  功能：等待 SD 卡的写入操作完成。


接下来介绍其定义的返回值类型

上面的函数涉及到的返回类型就是SD_Error和SDTransferState，一般检测就用if语句判断是否OK，其他暂时用不上。


接下来讲解调用是的注意事项：

这个SD读写和Flash的读写有几分相似，就是不要忘记在读，写，擦除等操作后进行等待即可。
