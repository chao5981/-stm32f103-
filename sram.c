#include "sram.h"

/**
 * @brief ����FMSC��GPIO����
 * @param ��
 * @retval ��
 */
static void SRAM_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
//ʹ��GPIO��ʱ��
	RCC_APB2PeriphClockCmd(FSMC_A16_GPIO_CLK|/*�����ź���*/
                         FSMC_D0_GPIO_CLK | FSMC_D1_GPIO_CLK | FSMC_D2_GPIO_CLK | 
                         FSMC_D3_GPIO_CLK | FSMC_D4_GPIO_CLK | FSMC_D5_GPIO_CLK |
                         FSMC_D6_GPIO_CLK | FSMC_D7_GPIO_CLK | FSMC_D8_GPIO_CLK |
                         FSMC_D9_GPIO_CLK | FSMC_D10_GPIO_CLK| FSMC_D11_GPIO_CLK|
                         FSMC_D12_GPIO_CLK| FSMC_D13_GPIO_CLK| FSMC_D14_GPIO_CLK|
                         FSMC_D15_GPIO_CLK|  
                         /*�����ź���*/
                         FSMC_CS_GPIO_CLK  | FSMC_WE_GPIO_CLK | FSMC_OE_GPIO_CLK |
                         FSMC_UDQM_GPIO_CLK|FSMC_LDQM_GPIO_CLK, ENABLE);
	
	//GPIO����
	
	//ͨ��GPIO����
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;       //����Ϊ���ù���
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     
	
	//A��ַ�ź��ߵ���������
	GPIO_InitStructure.GPIO_Pin = FSMC_A16_GPIO_PIN; 
  GPIO_Init(FSMC_A16_GPIO_PORT, &GPIO_InitStructure);
	
	
	//D�����ź��ߵ���������
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
	
	
	/*�����ź���*/
  GPIO_InitStructure.GPIO_Pin = FSMC_CS_GPIO_PIN; 
  GPIO_Init(FSMC_CS_GPIO_PORT, &GPIO_InitStructure);
    
  GPIO_InitStructure.GPIO_Pin = FSMC_WE_GPIO_PIN; 
  GPIO_Init(FSMC_WE_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = FSMC_OE_GPIO_PIN; 
  GPIO_Init(FSMC_OE_GPIO_PORT, &GPIO_InitStructure);    
}


/**
 * @brief ����FSMC��ʹ��
 * @param ��
 * @retval ��
 */
void FSMC_SRAM_Init(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  readWriteTimingStructure;
	
	//������"FSMC_NORSRAMTimingInitTypeDef"�ṹ�壬����Ҫ�õ�
	
	//ѡ��ģʽ�����������õ���ģʽA
	readWriteTimingStructure.FSMC_AccessMode=FSMC_AccessMode_A;
	//��ַ����ʱ�䣬�����ִ��������ٶȹ��죬һ��Ϊ0
	readWriteTimingStructure.FSMC_AddressHoldTime=0x00;
	//��ַ����ʱ��(ʱ��ͼ�е�ADDSET)������SRAMоƬ��t_AH������������(����ֻ��ʾ��)
	readWriteTimingStructure.FSMC_AddressSetupTime=0x00;
	//������תʱ��,ģʽA���첽����Ϊ0
	readWriteTimingStructure.FSMC_BusTurnAroundDuration=0x00;
	//ʱ�ӷ�Ƶ��ģʽA���첽����Ϊ0
	readWriteTimingStructure.FSMC_CLKDivision=0x00;
	//�����ӳ٣�ģʽA���첽����Ϊ0
	readWriteTimingStructure.FSMC_DataLatency=0x00;
	//���ݽ���ʱ��(DATAST),������SRAMоƬ��t_DH������������(����ֻ��ʾ��)
	readWriteTimingStructure.FSMC_DataSetupTime=0x02;
	
	
	//����"FSMC_NORSRAMInitTypeDef"�ṹ��
	
	//�첽�ȴ���һ�㲻��Ҫ
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable;
	//ѡ��洢�����������õ���bank1(stm32f10xֻ��bank1����)
	FSMC_NORSRAMInitStructure.FSMC_Bank=FSMC_Bank1_NORSRAM1;
	//�Ƿ�����"����"ģʽ����ͻ������ģʽ�����ﲻ��Ҫ
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode=FSMC_BurstAccessMode_Disable;
	//����/��ַ����(���Ƿ��������ߺ͵�ַ�߹���һ������)��һ�����
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux=FSMC_DataAddressMux_Disable;
	//�Ƿ�����չģʽ�����ﲻ��Ҫ
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode=FSMC_ExtendedMode_Disable;
	//ѡ�����ݿ�ȣ�����ѡ��16b
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth=FSMC_MemoryDataWidth_16b;
	//ѡ��洢�����ͣ��ӵ���NOR Flash����SRAM
	FSMC_NORSRAMInitStructure.FSMC_MemoryType=FSMC_MemoryType_SRAM;
	//��д���ݣ���һ��ʱ�����ýṹ��(FSMC_NORSRAMTimingInitTypeDef)
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct=&readWriteTimingStructure;
	//�ȴ��ź�ʹ�ܣ����ﲻ��Ҫ�ȴ�
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal=FSMC_WaitSignal_Disable;
	//�ȴ��źŽ׶�(��ͻ��������ĸ��׶β���"�ȴ��ź�")��һ��ѡ���ڵȴ�ǰ����
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive=FSMC_WaitSignalActive_BeforeWaitState;
	//�ȴ��źż���(�涨"�ȴ��ź�"�ǵ͵�ƽ��Ч���Ǹߵ�ƽ��Ч)
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity=FSMC_WaitSignalPolarity_Low;
	//����ģʽ��ͻ������ʱ�Ƿ��õ�ַ�Զ�"�ƻ���"������ֵʱDisable
	FSMC_NORSRAMInitStructure.FSMC_WrapMode=FSMC_WrapMode_Disable;
	//ͻ��дʹ��(�Ƿ�����"����д��"ģʽ)����ͨ��SRAM�ò���
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst=FSMC_WriteBurst_Disable;
	//дʹ�ܣ��Ƿ�����д��洢��
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation=FSMC_WriteOperation_Enable;
	//дʱ�����������չʱ�򣬿��Ե������ã������õĻ�����ΪNULL
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct=NULL;
	
	
	//��ʼ���ṹ��
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
	//ʹ�ܽṹ���GPIO
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1,ENABLE);
	SRAM_GPIO_Config();
}

/**
 * @brief ��U8������SRAMд������
 * @param pBuffer��ָ��洢���ݵ�ָ��
 * @param WriteAddress���洢���ݵĵ�ַ
 * @param WriteBufferNum д�����ݵĸ���
 * @retval ��
 */
void SRAM_WriteU8Buffer(u8 *pBuffer,u32 WriteAddress,u8 WriteBufferNum)
{
	__IO uint32_t write_pointer = (uint32_t)WriteAddress;
	for(;WriteBufferNum!=0;WriteBufferNum--)
	{
		//����ַǿתΪu8���͵�ָ�룬Ȼ������õ���pBufferָ�������
		*(u8*)(Bank1_SRAM1_ADDR + write_pointer)=*pBuffer++;
		write_pointer++;
	}
}


/**
 * @brief ��U16������SRAMд������
 * @param pBuffer��ָ��洢���ݵ�ָ��
 * @param WriteAddress���洢���ݵĵ�ַ
 * @param WriteBufferNum д�����ݵĸ���
 * @retval ��
 */
void SRAM_WriteU16Buffer(u16 *pBuffer,u32 WriteAddress,u16 WriteBufferNum)
{
	__IO uint32_t write_pointer = (uint32_t)WriteAddress;
	for(;WriteBufferNum!=0;WriteBufferNum--)
	{
		//����ַǿתΪu16���͵�ָ�룬Ȼ������õ���pBufferָ�������
		*(u16*)(Bank1_SRAM1_ADDR + write_pointer)=*pBuffer++;
		write_pointer+=2;
	}
}


/**
 * @brief ��U32������SRAMд������
 * @param pBuffer��ָ��洢���ݵ�ָ��
 * @param WriteAddress���洢���ݵĵ�ַ
 * @param WriteBufferNum д�����ݵĸ���
 * @retval ��
 */
void SRAM_WriteU32Buffer(u32 *pBuffer,u32 WriteAddress,u32 WriteBufferNum)
{
	__IO uint32_t write_pointer = (uint32_t)WriteAddress;
	for(;WriteBufferNum!=0;WriteBufferNum--)
	{
		//����ַǿתΪu32���͵�ָ�룬Ȼ���Ӧ�õ���pBufferָ�������
		*(u32*)(Bank1_SRAM1_ADDR + write_pointer)=*pBuffer++;
		write_pointer+=4;
	}
}

/**
 * @brief ��U8������SRAM��ȡ����
 * @param pBuffer��ָ��ȡ���Ĵ洢���ݵ�ָ��
 * @param ReadAddress����ȡ���ݵĵ�ַ
 * @param WriteBufferNum ��ȡ���ݵĸ���
 * @retval ��
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
 * @brief ��U16������SRAM��ȡ����
 * @param pBuffer��ָ��ȡ���Ĵ洢���ݵ�ָ��
 * @param ReadAddress����ȡ���ݵĵ�ַ
 * @param WriteBufferNum ��ȡ���ݵĸ���
 * @retval ��
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
 * @brief ��U32������SRAM��ȡ����
 * @param pBuffer��ָ��ȡ���Ĵ洢���ݵ�ָ��
 * @param ReadAddress����ȡ���ݵĵ�ַ
 * @param WriteBufferNum ��ȡ���ݵĸ���
 * @retval ��
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
 * @brief ����SRAM�Ƿ�����
 * @param ��
 * @retval ��������1������������0
 */
u8 SRAM_Test(void)
{
	/*д�����ݼ�����*/
  uint32_t counter=0;
  
  /* 8λ������ */
  uint8_t ubWritedata_8b = 0, ubReaddata_8b = 0;  
  
  /* 16λ������ */
  uint16_t uhWritedata_16b = 0, uhReaddata_16b = 0; 
  
  SRAM_INFO("���ڼ��SRAM����8λ��16λ�ķ�ʽ��дsram...");
	
/*��8λ��ʽ��д���ݣ���У��*/
  
  /* ��SRAM����ȫ������Ϊ0 ��IS62WV51216_SIZE����8λΪ��λ�� */
  for (counter = 0x00; counter < IS62WV51216_SIZE; counter++)
  {
    *(__IO uint8_t*) (Bank1_SRAM1_ADDR + counter) = (uint8_t)0x0;
  }
  
  /* ������SRAMд������  8λ */
  for (counter = 0; counter < IS62WV51216_SIZE; counter++)
  {
    *(__IO uint8_t*) (Bank1_SRAM1_ADDR + counter) = (uint8_t)(ubWritedata_8b + counter);
  }
  
  /* ��ȡ SRAM ���ݲ����*/
  for(counter = 0; counter<IS62WV51216_SIZE;counter++ )
  {
    ubReaddata_8b = *(__IO uint8_t*)(Bank1_SRAM1_ADDR + counter);  //�Ӹõ�ַ��������
    
    if(ubReaddata_8b != (uint8_t)(ubWritedata_8b + counter))      //������ݣ�������ȣ���������,���ؼ��ʧ�ܽ����
    {
      SRAM_ERROR("8λ���ݶ�д����");
      return 0;
    }
  }
	
  
  /*��16λ��ʽ��д���ݣ������*/
  
  /* ��SRAM����ȫ������Ϊ0 */
  for (counter = 0x00; counter < IS62WV51216_SIZE/2; counter++)
  {
    *(__IO uint16_t*) (Bank1_SRAM1_ADDR + 2*counter) = (uint16_t)0x00;
  }
  
  /* ������SRAMд������  16λ */
  for (counter = 0; counter < IS62WV51216_SIZE/2; counter++)
  {
    *(__IO uint16_t*) (Bank1_SRAM1_ADDR + 2*counter) = (uint16_t)(uhWritedata_16b + counter);
  }
  
    /* ��ȡ SRAM ���ݲ����*/
  for(counter = 0; counter<IS62WV51216_SIZE/2;counter++ )
  {
    uhReaddata_16b = *(__IO uint16_t*)(Bank1_SRAM1_ADDR + 2*counter);  //�Ӹõ�ַ��������
    
    if(uhReaddata_16b != (uint16_t)(uhWritedata_16b + counter))      //������ݣ�������ȣ���������,���ؼ��ʧ�ܽ����
    {
      SRAM_ERROR("16λ���ݶ�д����");

      return 0;
    }
  }
  SRAM_INFO("SRAM��д����������"); 
  /*���������return 1 */
  return 1;

}

