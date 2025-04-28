#include "I2C.h"

uint16_t EEPROM_ADDRESS;

static __IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT;   


static uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode);

void I2C_EE_WaitEepromStandbyState(void);
/**
 * @brief ��ʼ��I2C��SDA��SCL����
 * @param ��
 * @retval ��
 */
static void I2C_EEPROM_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_SDA_Structure,GPIO_SCL_Structure;
	EEPROM_I2Cx_GPIO_CLK_FUN(EEPROM_I2Cx_GPIO_CLK,ENABLE);
	GPIO_SDA_Structure.GPIO_Mode=GPIO_Mode_AF_OD;
	GPIO_SDA_Structure.GPIO_Pin=EEPROM_I2Cx_SDA_GPIO_PIN;
	GPIO_SDA_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(EEPROM_I2Cx_SDA_GPIO_PORT,&GPIO_SDA_Structure);
	
	GPIO_SCL_Structure.GPIO_Mode=GPIO_Mode_AF_OD;
	GPIO_SCL_Structure.GPIO_Pin=EEPROM_I2Cx_SCL_GPIO_PIN;
	GPIO_SCL_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(EEPROM_I2Cx_SCL_GPIO_PORT,&GPIO_SCL_Structure);
}
/**
 * @brief ��ʼ��I2C
 * @param ��
 * @retval ��
 */
static void I2C_EEPROM_Configuration(void)
{
	I2C_InitTypeDef I2C_Structure;
	//����I2Cʱ��
	EEPROM_I2Cx_CLK_FUN(EEPROM_I2Cx_CLK,ENABLE);
	//ʹ��ACK��Ӧ
	I2C_Structure.I2C_Ack=I2C_Ack_Enable;
	//ָ����ַ�ĳ���
	I2C_Structure.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;
	//ָ��SCL��ʱ��Ƶ�ʣ����ֵҪ����40000
	I2C_Structure.I2C_ClockSpeed=I2C_Speed;
	//����ʱ��ռ�ձȣ�һ�������Ҫ���ϸ�
	I2C_Structure.I2C_DutyCycle=I2C_DutyCycle_2;
	//����I2C��ʹ��ģʽ
	I2C_Structure.I2C_Mode=I2C_Mode_I2C;
	//���������i2c��ַ
	I2C_Structure.I2C_OwnAddress1=I2Cx_OWN_ADDRESS7;
	I2C_Init(EEPROM_I2Cx,&I2C_Structure);
	I2C_Cmd(EEPROM_I2Cx,ENABLE);
}

/**
 * @brief ��ʼ��I2C
 * @param ��
 * @retval ��
 */
void I2C_EEPROM_Init(void)
{
	I2C_EEPROM_Configuration();
	I2C_EEPROM_GPIO_Configuration();
	
	/* ����ͷ�ļ�i2c_ee.h�еĶ�����ѡ��EEPROMҪд��ĵ�ַ */
#ifdef EEPROM_Block0_ADDRESS
  /* ѡ�� EEPROM Block0 ��д�� */
  EEPROM_ADDRESS = EEPROM_Block0_ADDRESS;
#endif

#ifdef EEPROM_Block1_ADDRESS  
	/* ѡ�� EEPROM Block1 ��д�� */
  EEPROM_ADDRESS = EEPROM_Block1_ADDRESS;
#endif

#ifdef EEPROM_Block2_ADDRESS  
	/* ѡ�� EEPROM Block2 ��д�� */
  EEPROM_ADDRESS = EEPROM_Block2_ADDRESS;
#endif

#ifdef EEPROM_Block3_ADDRESS  
	/* ѡ�� EEPROM Block3 ��д�� */
  EEPROM_ADDRESS = EEPROM_Block3_ADDRESS;
#endif

}
/**
 * @brief ����дһ���ֽ�
 * @param д�������(������������ʡջ�Ŀռ�);д��ĵ�ַ
 * @retval ������Ϣ
 */
uint32_t I2C_EEPROM_ByteWrite(uint8_t *pBuffer,uint8_t WriteAddr)
{
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	I2C_GenerateSTART(EEPROM_I2Cx,ENABLE);
	//EV5
	while(!I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((I2CTimeout--)==0)
		{
			return I2C_TIMEOUT_UserCallback(0);
		}
	}
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	I2C_Send7bitAddress(EEPROM_I2Cx,EEPROM_ADDRESS,I2C_Direction_Transmitter);
	//EV6
	while(!I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((I2CTimeout--)==0)
		{
			return I2C_TIMEOUT_UserCallback(1);
		}
	}
	I2C_SendData(EEPROM_I2Cx,WriteAddr);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//EV8_2
	while(!I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((I2CTimeout--)==0)
		{
			return I2C_TIMEOUT_UserCallback(2);
		}
	}
	I2C_SendData(EEPROM_I2Cx,*pBuffer);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//EV8_2
	while(!I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((I2CTimeout--)==0)
		{
			return I2C_TIMEOUT_UserCallback(3);
		}
	}
	I2C_GenerateSTOP(EEPROM_I2Cx,ENABLE);
	return 1;
}
/**
 * @brief д��һҳ
 * @param д�������(������������ʡջ�Ŀռ�);д��ĵ�ַ;д���ٸ�����
 * @retval ������Ϣ
 */
uint32_t I2C_EEPROM_PageWrite(uint8_t *pBuffer,uint8_t WriteAddr,uint8_t NumByteToWrite)
{
	I2CTimeout = I2CT_LONG_TIMEOUT;
	//״̬��⣬��ֹ�������״̬
	while(I2C_GetFlagStatus(EEPROM_I2Cx, I2C_FLAG_BUSY))   
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(4);
  } 
	
	I2C_GenerateSTART(EEPROM_I2Cx,ENABLE);
	
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//EV5
	while(!I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((I2CTimeout--)==0)
		{
			return I2C_TIMEOUT_UserCallback(5);
		}
	}
	
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	I2C_Send7bitAddress(EEPROM_I2Cx,EEPROM_ADDRESS,I2C_Direction_Transmitter);
	//EV6
	while(!I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((I2CTimeout--)==0)
		{
			return I2C_TIMEOUT_UserCallback(6);
		}
	}
	I2C_SendData(EEPROM_I2Cx,WriteAddr);
	
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//EV8_2
	while(!I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((I2CTimeout--)==0)
		{
			return I2C_TIMEOUT_UserCallback(7);
		}
	}
	while(NumByteToWrite--)
	{
		I2C_SendData(EEPROM_I2Cx,*pBuffer);
		I2CTimeout = I2CT_FLAG_TIMEOUT;
		//EV8_2
		while(!I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			if((I2CTimeout--)==0)
			{
				return I2C_TIMEOUT_UserCallback(8);
			}
		}
		pBuffer++;
	}
	I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE);
	return 1;
}
/**
 * @brief ���Է�ҳд
 * @param д�������(������������ʡջ�Ŀռ�);д��ĵ�ַ;д���ٸ�����
 * @retval ��
 */
void I2C_EEPROM_Buffer_Write(uint8_t *pBuffer,uint8_t WriteAddr,uint8_t NumByteToWrite)
{
	uint8_t count=0,Addr=0,NumOfPage=0,NumOfSingle=0,temp=0;
	Addr=WriteAddr%I2C_PageSize;
	count=I2C_PageSize-Addr;
	NumOfPage=NumByteToWrite/I2C_PageSize;
	NumOfSingle=NumByteToWrite%I2C_PageSize;
	if(Addr==0)
	{
		if(NumOfPage==0)
		{
			I2C_EEPROM_PageWrite(pBuffer,WriteAddr,NumOfSingle);
			I2C_EE_WaitEepromStandbyState();
		}
		else
		{
			while(NumOfPage--)
			{
				I2C_EEPROM_PageWrite(pBuffer,WriteAddr,I2C_PageSize);
				I2C_EE_WaitEepromStandbyState();
				pBuffer+=I2C_PageSize;
				WriteAddr+=I2C_PageSize;
			}
			if(NumOfSingle!=0)
			{
				I2C_EEPROM_PageWrite(pBuffer,WriteAddr,NumOfSingle);
				I2C_EE_WaitEepromStandbyState();
			}
		}
	}
	else
	{
		if(NumOfPage==0)
		{
			if(NumOfSingle<=count)
			{
				I2C_EEPROM_PageWrite(pBuffer,WriteAddr,NumByteToWrite);
				I2C_EE_WaitEepromStandbyState();
			}
			else
			{
				temp=NumOfSingle-count;
				I2C_EEPROM_PageWrite(pBuffer,WriteAddr,count);
				I2C_EE_WaitEepromStandbyState();
				WriteAddr+=count;
				pBuffer+=count;
				I2C_EEPROM_PageWrite(pBuffer,WriteAddr,temp);
				I2C_EE_WaitEepromStandbyState();
			}
		}
		else
		{
			NumByteToWrite-=count;
			NumOfPage=NumByteToWrite/I2C_PageSize;
			NumOfSingle=NumByteToWrite%I2C_PageSize;
			if(count!=0)
			{
				I2C_EEPROM_PageWrite(pBuffer,WriteAddr,count);
				I2C_EE_WaitEepromStandbyState();
				WriteAddr+=count;
				pBuffer+=count;
			}
			while(NumOfPage--)
			{
				I2C_EEPROM_PageWrite(pBuffer,WriteAddr,I2C_PageSize);
				I2C_EE_WaitEepromStandbyState();
				pBuffer+=I2C_PageSize;
				WriteAddr+=I2C_PageSize;
			}
			if(NumOfSingle!=0)
			{
				I2C_EEPROM_PageWrite(pBuffer,WriteAddr,NumOfSingle);
				I2C_EE_WaitEepromStandbyState();
			}
		}
	}
}
/**
 * @brief ������
 * @param ��ȡ������(������������ʡջ�Ŀռ�);��ȡ�ĵ�ַ;��ȡ���ٸ�����
 * @retval ������Ϣ
 */
uint32_t I2C_EEPROM_BufferRead(uint8_t *pBuffer,uint8_t ReadAddr,uint8_t NumByteToRead)
{
	I2CTimeout = I2CT_LONG_TIMEOUT;
	//״̬��⣬��ֹ�������״̬
	while(I2C_GetFlagStatus(EEPROM_I2Cx, I2C_FLAG_BUSY))   
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(9);
  } 
	I2C_GenerateSTART(EEPROM_I2Cx,ENABLE);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//EV5
	while(!I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((I2CTimeout--)==0)
		{
			return I2C_TIMEOUT_UserCallback(10);
		}
	}
	I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_ADDRESS,I2C_Direction_Transmitter);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//EV6
	while(!I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((I2CTimeout--)==0)
		{
			return I2C_TIMEOUT_UserCallback(11);
		}
	}
	
	I2C_Cmd(EEPROM_I2Cx,ENABLE);
	
	I2C_SendData(EEPROM_I2Cx,ReadAddr);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//EV8
	while(!I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((I2CTimeout--)==0)
		{
			return I2C_TIMEOUT_UserCallback(12);
		}
	}
	I2C_GenerateSTART(EEPROM_I2Cx,ENABLE);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//EV5
	while(!I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((I2CTimeout--)==0)
		{
			return I2C_TIMEOUT_UserCallback(13);
		}
	}
	I2C_Send7bitAddress(EEPROM_I2Cx,EEPROM_ADDRESS,I2C_Direction_Receiver);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	//EV6
	while(!I2C_CheckEvent(EEPROM_I2Cx,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{
		if((I2CTimeout--)==0)
		{
			return I2C_TIMEOUT_UserCallback(14);
		}
	}
	while(NumByteToRead)
	{
		if(NumByteToRead==1)
		{
			I2C_AcknowledgeConfig(EEPROM_I2Cx,DISABLE);
			I2C_GenerateSTOP(EEPROM_I2Cx,ENABLE);
		}
		I2CTimeout = I2CT_LONG_TIMEOUT;
	//EV7
	while(I2C_CheckEvent(EEPROM_I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)==0)
	{
		if((I2CTimeout--)==0)
		{
			return I2C_TIMEOUT_UserCallback(15);
		}
	}
	*pBuffer = I2C_ReceiveData(EEPROM_I2Cx);
	 pBuffer++;
	 NumByteToRead--;
	}
	I2C_AcknowledgeConfig(EEPROM_I2Cx,ENABLE);
	return 1;
}
/**
 * @brief д��һ��С��
 * @param д��ĵ�ַ;д���С��
 * @retval ��
 */
void I2C_EEPROM_WriteDouble(u8 WriteAddr, double val)
{
     union {
        double d;
        u8 bytes[8];
    } converter;

    converter.d = val;
    I2C_EEPROM_Buffer_Write(converter.bytes, WriteAddr, 8);
}

/**
 * @brief ��ȡһ��С��
 * @param ��ȡ�ĵ�ַ
 * @retval ����С��
 */
double I2C_EEPROM_ReadDouble(u8 ReadAddr)
{
	 union {
        double d;
        u8 bytes[8];
    } converter;
	 I2C_EEPROM_BufferRead(converter.bytes, ReadAddr, 8);
	 I2C_EE_WaitEepromStandbyState();
	return converter.d;
}

/**
 * @brief д��һ��int���͵�����
 * @param д��ĵ�ַ;д�������
 * @retval ��
 */
void I2C_EEPROM_WriteUInt16(u8 WriteAddr, u16 val)
{
    union {
        u16 value;
        u8 bytes[2];
    } converter;

    converter.value = val;
		I2C_EEPROM_Buffer_Write(converter.bytes, WriteAddr, 2);
}

/**
 * @brief ��ȡһ��int���͵�����
 * @param ��ȡ�ĵ�ַ
 * @retval ����int���͵�����
 */
u16 I2C_EEPROM_ReadUInt16(u8 ReadAddr)
{
	 union {
        u16 value;
        u8 bytes[2];
    } converter;
	I2C_EEPROM_BufferRead(converter.bytes, ReadAddr, 2);
	I2C_EE_WaitEepromStandbyState();
	return converter.value;
}
/**
 * @brief �ȴ��¼�
 * @param ��
 * @retval ��
 */
void I2C_EE_WaitEepromStandbyState(void)      
{
  vu16 SR1_Tmp = 0;

  do
  {
    /* Send START condition */
    I2C_GenerateSTART(EEPROM_I2Cx, ENABLE);
    /* Read I2C1 SR1 register */
    SR1_Tmp = I2C_ReadRegister(EEPROM_I2Cx, I2C_Register_SR1);
    /* Send EEPROM address for write */
    I2C_Send7bitAddress(EEPROM_I2Cx, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  }while(!(I2C_ReadRegister(EEPROM_I2Cx, I2C_Register_SR1) & 0x0002));
  
  /* Clear AF flag */
  I2C_ClearFlag(EEPROM_I2Cx, I2C_FLAG_AF);
    /* STOP condition */    
    I2C_GenerateSTOP(EEPROM_I2Cx, ENABLE); 
}

/**
 * @brief ��ʾ������Ϣ
 * @param ����ص�����ǵ�����
 * @retval ��
 */
static  uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* Block communication and all processes */
  EEPROM_ERROR("I2C �ȴ���ʱ!errorCode = %d",errorCode);
  
  return 0;
}
