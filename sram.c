#include "sram.h"

/**
 * @brief 配置FMSC的GPIO引脚
 * @param 无
 * @retval 无
 */
static void SRAM_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
//使能GPIO的时钟
	RCC_APB2PeriphClockCmd(FSMC_A16_GPIO_CLK|/*数据信号线*/
                         FSMC_D0_GPIO_CLK | FSMC_D1_GPIO_CLK | FSMC_D2_GPIO_CLK | 
                         FSMC_D3_GPIO_CLK | FSMC_D4_GPIO_CLK | FSMC_D5_GPIO_CLK |
                         FSMC_D6_GPIO_CLK | FSMC_D7_GPIO_CLK | FSMC_D8_GPIO_CLK |
                         FSMC_D9_GPIO_CLK | FSMC_D10_GPIO_CLK| FSMC_D11_GPIO_CLK|
                         FSMC_D12_GPIO_CLK| FSMC_D13_GPIO_CLK| FSMC_D14_GPIO_CLK|
                         FSMC_D15_GPIO_CLK|  
                         /*控制信号线*/
                         FSMC_CS_GPIO_CLK  | FSMC_WE_GPIO_CLK | FSMC_OE_GPIO_CLK |
                         FSMC_UDQM_GPIO_CLK|FSMC_LDQM_GPIO_CLK, ENABLE);
	
	//GPIO配置
	
	//通用GPIO设置
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;       //配置为复用功能
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     
	
	//A地址信号线的引脚配置
	GPIO_InitStructure.GPIO_Pin = FSMC_A16_GPIO_PIN; 
  GPIO_Init(FSMC_A16_GPIO_PORT, &GPIO_InitStructure);
	
	
	//D数据信号线的引脚配置
	 GPIO_InitStructure.GPIO_Pin = FSMC_D0_GPIO_PIN; 
  GPIO_Init(FSMC_D0_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_D1_GPIO_PIN; 
  GPIO_Init(FSMC_D1_GPIO_PORT, &GPIO_InitStructure);
    
  GPIO_InitStructure.GPIO_Pin = FSMC_D2_GPIO_PIN; 
  GPIO_Init(FSMC_D2_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_D3_GPIO_PIN; 
  GPIO_Init(FSMC_D3_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_D4_GPIO_PIN; 
  GPIO_Init(FSMC_D4_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_D5_GPIO_PIN; 
  GPIO_Init(FSMC_D5_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_D6_GPIO_PIN; 
  GPIO_Init(FSMC_D6_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_D7_GPIO_PIN; 
  GPIO_Init(FSMC_D7_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_D8_GPIO_PIN; 
  GPIO_Init(FSMC_D8_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_D9_GPIO_PIN; 
  GPIO_Init(FSMC_D9_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_D10_GPIO_PIN; 
  GPIO_Init(FSMC_D10_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_D11_GPIO_PIN; 
  GPIO_Init(FSMC_D11_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_D12_GPIO_PIN; 
  GPIO_Init(FSMC_D12_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_D13_GPIO_PIN; 
  GPIO_Init(FSMC_D13_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_D14_GPIO_PIN; 
  GPIO_Init(FSMC_D14_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_D15_GPIO_PIN; 
  GPIO_Init(FSMC_D15_GPIO_PORT, &GPIO_InitStructure);
	
	
	/*控制信号线*/
  GPIO_InitStructure.GPIO_Pin = FSMC_CS_GPIO_PIN; 
  GPIO_Init(FSMC_CS_GPIO_PORT, &GPIO_InitStructure);
    
  GPIO_InitStructure.GPIO_Pin = FSMC_WE_GPIO_PIN; 
  GPIO_Init(FSMC_WE_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_OE_GPIO_PIN; 
  GPIO_Init(FSMC_OE_GPIO_PORT, &GPIO_InitStructure);    
}


/**
 * @brief 配置FSMC并使能
 * @param 无
 * @retval 无
 */
void FSMC_SRAM_Init(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  readWriteTimingStructure;
	
	//先配置"FSMC_NORSRAMTimingInitTypeDef"结构体，后面要用到
	
	//选择模式，在这里我用的是模式A
	readWriteTimingStructure.FSMC_AccessMode=FSMC_AccessMode_A;
	//地址保持时间，由于现代储存器速度够快，一般为0
	readWriteTimingStructure.FSMC_AddressHoldTime=0x00;
	//地址建立时间(时序图中的ADDSET)，根据SRAM芯片的t_AH参数进行配置(这里只做示例)
	readWriteTimingStructure.FSMC_AddressSetupTime=0x00;
	//总线周转时间,模式A是异步所以为0
	readWriteTimingStructure.FSMC_BusTurnAroundDuration=0x00;
	//时钟分频，模式A是异步所以为0
	readWriteTimingStructure.FSMC_CLKDivision=0x00;
	//数据延迟，模式A是异步所以为0
	readWriteTimingStructure.FSMC_DataLatency=0x00;
	//数据建立时间(DATAST),，根据SRAM芯片的t_DH参数进行配置(这里只做示例)
	readWriteTimingStructure.FSMC_DataSetupTime=0x02;
	
	
	//配置"FSMC_NORSRAMInitTypeDef"结构体
	
	//异步等待，一般不需要
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable;
	//选择存储区域，这里我用的是bank1(stm32f10x只有bank1区域)
	FSMC_NORSRAMInitStructure.FSMC_Bank=FSMC_Bank1_NORSRAM1;
	//是否启用"连发"模式，即突发访问模式，这里不需要
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode=FSMC_BurstAccessMode_Disable;
	//数据/地址复用(即是否让数据线和地址线共用一个引脚)，一般禁掉
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux=FSMC_DataAddressMux_Disable;
	//是否开启扩展模式，这里不需要
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode=FSMC_ExtendedMode_Disable;
	//选择数据宽度，这里选择16b
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth=FSMC_MemoryDataWidth_16b;
	//选择存储器类型，接的是NOR Flash还是SRAM
	FSMC_NORSRAMInitStructure.FSMC_MemoryType=FSMC_MemoryType_SRAM;
	//读写数据，绑定一个时序配置结构体(FSMC_NORSRAMTimingInitTypeDef)
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct=&readWriteTimingStructure;
	//等待信号使能，这里不需要等待
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal=FSMC_WaitSignal_Disable;
	//等待信号阶段(在突发传输的哪个阶段插入"等待信号")，一般选择在等待前插入
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive=FSMC_WaitSignalActive_BeforeWaitState;
	//等待信号极性(规定"等待信号"是低电平有效还是高电平有效)
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity=FSMC_WaitSignalPolarity_Low;
	//回绕模式，突发传输时是否让地址自动"绕回来"，常用值时Disable
	FSMC_NORSRAMInitStructure.FSMC_WrapMode=FSMC_WrapMode_Disable;
	//突发写使能(是否启用"连续写入"模式)，普通的SRAM用不到
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst=FSMC_WriteBurst_Disable;
	//写使能，是否允许写入存储器
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation=FSMC_WriteOperation_Enable;
	//写时序，如果启用扩展时序，可以单独配置，不启用的画设置为NULL
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct=NULL;
	
	
	//初始化结构体
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
	//使能结构体和GPIO
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1,ENABLE);
	SRAM_GPIO_Config();
}

/**
 * @brief 以U8类型向SRAM写入数据
 * @param pBuffer：指向存储数据的指针
 * @param WriteAddress：存储数据的地址
 * @param WriteBufferNum 写入数据的个数
 * @retval 无
 */
void SRAM_WriteU8Buffer(u8 *pBuffer,u32 WriteAddress,u8 WriteBufferNum)
{
	__IO uint32_t write_pointer = (uint32_t)WriteAddress;
	for(;WriteBufferNum!=0;WriteBufferNum--)
	{
		//将地址强转为u8类型的指针，然后解引用等于pBuffer指针的内容
		*(u8*)(Bank1_SRAM1_ADDR + write_pointer)=*pBuffer++;
		write_pointer++;
	}
}


/**
 * @brief 以U16类型向SRAM写入数据
 * @param pBuffer：指向存储数据的指针
 * @param WriteAddress：存储数据的地址
 * @param WriteBufferNum 写入数据的个数
 * @retval 无
 */
void SRAM_WriteU16Buffer(u16 *pBuffer,u32 WriteAddress,u16 WriteBufferNum)
{
	__IO uint32_t write_pointer = (uint32_t)WriteAddress;
	for(;WriteBufferNum!=0;WriteBufferNum--)
	{
		//将地址强转为u16类型的指针，然后解引用等于pBuffer指针的内容
		*(u16*)(Bank1_SRAM1_ADDR + write_pointer)=*pBuffer++;
		write_pointer+=2;
	}
}


/**
 * @brief 以U32类型向SRAM写入数据
 * @param pBuffer：指向存储数据的指针
 * @param WriteAddress：存储数据的地址
 * @param WriteBufferNum 写入数据的个数
 * @retval 无
 */
void SRAM_WriteU32Buffer(u32 *pBuffer,u32 WriteAddress,u32 WriteBufferNum)
{
	__IO uint32_t write_pointer = (uint32_t)WriteAddress;
	for(;WriteBufferNum!=0;WriteBufferNum--)
	{
		//将地址强转为u32类型的指针，然后解应用等于pBuffer指针的内容
		*(u32*)(Bank1_SRAM1_ADDR + write_pointer)=*pBuffer++;
		write_pointer+=4;
	}
}

/**
 * @brief 以U8类型向SRAM读取数据
 * @param pBuffer：指向取出的存储数据的指针
 * @param ReadAddress：读取数据的地址
 * @param WriteBufferNum 读取数据的个数
 * @retval 无
 */
void SRAM_ReadU8Buffer(u8 *pBuffer,u32 ReadAddress,u8 ReadBufferNum)
{
	__IO uint32_t read_pointer = (uint32_t)ReadAddress;
	for(;ReadBufferNum!=0;ReadBufferNum--)
	{
		*pBuffer++=*(__IO uint8_t*)(Bank1_SRAM1_ADDR + read_pointer);
		read_pointer++;
	}
}


/**
 * @brief 以U16类型向SRAM读取数据
 * @param pBuffer：指向取出的存储数据的指针
 * @param ReadAddress：读取数据的地址
 * @param WriteBufferNum 读取数据的个数
 * @retval 无
 */
void SRAM_ReadU16Buffer(u16 *pBuffer,u32 ReadAddress,u16 ReadBufferNum)
{
	__IO uint32_t read_pointer = (uint32_t)ReadAddress;
	for(;ReadBufferNum!=0;ReadBufferNum--)
	{
		*pBuffer++=*(__IO uint16_t*)(Bank1_SRAM1_ADDR + read_pointer);
		read_pointer+=2;
	}
}


/**
 * @brief 以U32类型向SRAM读取数据
 * @param pBuffer：指向取出的存储数据的指针
 * @param ReadAddress：读取数据的地址
 * @param WriteBufferNum 读取数据的个数
 * @retval 无
 */
void SRAM_ReadU32Buffer(u32 *pBuffer,u32 ReadAddress,u32 ReadBufferNum)
{
	__IO uint32_t read_pointer = (uint32_t)ReadAddress;
	for(;ReadBufferNum!=0;ReadBufferNum--)
	{
		*pBuffer++=*(__IO uint32_t*)(Bank1_SRAM1_ADDR + read_pointer);
		read_pointer+=4;
	}
}


/**
 * @brief 测试SRAM是否正常
 * @param 无
 * @retval 正常返回1，不正常返回0
 */
u8 SRAM_Test(void)
{
	/*写入数据计数器*/
  uint32_t counter=0;
  
  /* 8位的数据 */
  uint8_t ubWritedata_8b = 0, ubReaddata_8b = 0;  
  
  /* 16位的数据 */
  uint16_t uhWritedata_16b = 0, uhReaddata_16b = 0; 
  
  SRAM_INFO("正在检测SRAM，以8位、16位的方式读写sram...");
	
/*按8位格式读写数据，并校验*/
  
  /* 把SRAM数据全部重置为0 ，IS62WV51216_SIZE是以8位为单位的 */
  for (counter = 0x00; counter < IS62WV51216_SIZE; counter++)
  {
    *(__IO uint8_t*) (Bank1_SRAM1_ADDR + counter) = (uint8_t)0x0;
  }
  
  /* 向整个SRAM写入数据  8位 */
  for (counter = 0; counter < IS62WV51216_SIZE; counter++)
  {
    *(__IO uint8_t*) (Bank1_SRAM1_ADDR + counter) = (uint8_t)(ubWritedata_8b + counter);
  }
  
  /* 读取 SRAM 数据并检测*/
  for(counter = 0; counter<IS62WV51216_SIZE;counter++ )
  {
    ubReaddata_8b = *(__IO uint8_t*)(Bank1_SRAM1_ADDR + counter);  //从该地址读出数据
    
    if(ubReaddata_8b != (uint8_t)(ubWritedata_8b + counter))      //检测数据，若不相等，跳出函数,返回检测失败结果。
    {
      SRAM_ERROR("8位数据读写错误！");
      return 0;
    }
  }
	
  
  /*按16位格式读写数据，并检测*/
  
  /* 把SRAM数据全部重置为0 */
  for (counter = 0x00; counter < IS62WV51216_SIZE/2; counter++)
  {
    *(__IO uint16_t*) (Bank1_SRAM1_ADDR + 2*counter) = (uint16_t)0x00;
  }
  
  /* 向整个SRAM写入数据  16位 */
  for (counter = 0; counter < IS62WV51216_SIZE/2; counter++)
  {
    *(__IO uint16_t*) (Bank1_SRAM1_ADDR + 2*counter) = (uint16_t)(uhWritedata_16b + counter);
  }
  
    /* 读取 SRAM 数据并检测*/
  for(counter = 0; counter<IS62WV51216_SIZE/2;counter++ )
  {
    uhReaddata_16b = *(__IO uint16_t*)(Bank1_SRAM1_ADDR + 2*counter);  //从该地址读出数据
    
    if(uhReaddata_16b != (uint16_t)(uhWritedata_16b + counter))      //检测数据，若不相等，跳出函数,返回检测失败结果。
    {
      SRAM_ERROR("16位数据读写错误！");

      return 0;
    }
  }
  SRAM_INFO("SRAM读写测试正常！"); 
  /*检测正常，return 1 */
  return 1;

}

