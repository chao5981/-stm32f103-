#include "flash.h"

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;    
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

void Flash_SPI_Init(void)
{
	GPIO_InitTypeDef FLASH_SPI_NSS_Sturcture,FLASH_SPI_SCK_Sturcture,FLASH_SPI_MISO_Sturcture,FLASH_SPI_MOSI_Sturcture;
	SPI_InitTypeDef FLASH_SPI_Structure;
	//配置SPI_NSS引脚
		FLASH_SPI_CS_APBxClock_FUN(FLASH_SPI_CS_CLK,ENABLE);
	FLASH_SPI_NSS_Sturcture.GPIO_Pin=FLASH_SPI_CS_PIN;
	FLASH_SPI_NSS_Sturcture.GPIO_Mode=GPIO_Mode_Out_PP;
	FLASH_SPI_NSS_Sturcture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(FLASH_SPI_CS_PORT,&FLASH_SPI_NSS_Sturcture);
	//配置SPI_SCK引脚
	FLASH_SPI_SCK_APBxClock_FUN(FLASH_SPI_SCK_CLK,ENABLE);
	FLASH_SPI_SCK_Sturcture.GPIO_Pin=FLASH_SPI_SCK_PIN;
	FLASH_SPI_SCK_Sturcture.GPIO_Mode=GPIO_Mode_AF_PP;
	FLASH_SPI_SCK_Sturcture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(FLASH_SPI_SCK_PORT,&FLASH_SPI_SCK_Sturcture);
	//配置SPI_MISO引脚
	FLASH_SPI_MISO_APBxClock_FUN(FLASH_SPI_MISO_CLK,ENABLE);
	FLASH_SPI_MISO_Sturcture.GPIO_Pin=FLASH_SPI_MISO_PIN;
	FLASH_SPI_MISO_Sturcture.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	FLASH_SPI_MISO_Sturcture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(FLASH_SPI_MISO_PORT,&FLASH_SPI_MISO_Sturcture);
	//配置SPI_MOSI引脚
	FLASH_SPI_MOSI_APBxClock_FUN(FLASH_SPI_MOSI_CLK,ENABLE);
	FLASH_SPI_MOSI_Sturcture.GPIO_Pin=FLASH_SPI_MOSI_PIN;
	FLASH_SPI_MOSI_Sturcture.GPIO_Mode=GPIO_Mode_AF_PP;
	FLASH_SPI_MOSI_Sturcture.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(FLASH_SPI_MOSI_PORT,&FLASH_SPI_MOSI_Sturcture);
	SPI_FLASH_CS_HIGH();
	//配置SPI
	FLASH_SPI_APBxClock_FUN(FLASH_SPI_CLK,ENABLE);
	FLASH_SPI_Structure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_4;
	FLASH_SPI_Structure.SPI_CPHA=SPI_CPHA_2Edge;
	FLASH_SPI_Structure.SPI_CPOL=SPI_CPOL_High;
	FLASH_SPI_Structure.SPI_CRCPolynomial=7;
	FLASH_SPI_Structure.SPI_DataSize=SPI_DataSize_8b;
	FLASH_SPI_Structure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
	FLASH_SPI_Structure.SPI_FirstBit=SPI_FirstBit_MSB;
	FLASH_SPI_Structure.SPI_Mode=SPI_Mode_Master;
	FLASH_SPI_Structure.SPI_NSS=SPI_NSS_Soft;
	SPI_Init(FLASH_SPIx,&FLASH_SPI_Structure);
	SPI_Cmd(FLASH_SPIx,ENABLE);
}

u8 Flash_SPI_SendByte(u8 Byte)
{
	SPITimeout=SPIT_FLAG_TIMEOUT;
	while(SPI_I2S_GetFlagStatus(FLASH_SPIx,SPI_I2S_FLAG_TXE)==RESET)
	{
		if((SPITimeout--) == 0) 
		{
			return SPI_TIMEOUT_UserCallback(0);
		}
	}
	SPI_I2S_SendData(FLASH_SPIx,Byte);
	SPITimeout=SPIT_FLAG_TIMEOUT;
	while(SPI_I2S_GetFlagStatus(FLASH_SPIx,SPI_I2S_FLAG_RXNE)==RESET)
	{
		if((SPITimeout--) == 0) 
		{
			return SPI_TIMEOUT_UserCallback(1);
		}
	}
	return SPI_I2S_ReceiveData(FLASH_SPIx);
}

u8 Flash_SPI_ReadByte(void)
{
	return (Flash_SPI_SendByte(Dummy_Byte));
}

u32 Flash_SPI_ReadID(void)
{
	u8 temp1=0,temp2=0,temp3=0;
	u32 ID_data;
	SPI_FLASH_CS_LOW();
	Flash_SPI_SendByte(W25X_JedecDeviceID);
	temp1=Flash_SPI_ReadByte();
	temp2=Flash_SPI_ReadByte();
	temp3=Flash_SPI_ReadByte();
	SPI_FLASH_CS_HIGH();
	ID_data=(temp1<<16)|(temp2<<8)|temp3;
	return ID_data;
}

u32 Flash_SPI_ReadDeviceID(void)
{
  u32 Temp = 0;

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "RDID " instruction */
  Flash_SPI_SendByte(W25X_DeviceID);
  Flash_SPI_SendByte(Dummy_Byte);
  Flash_SPI_SendByte(Dummy_Byte);
  Flash_SPI_SendByte(Dummy_Byte);
  
  /* Read a byte from the FLASH */
  Temp = Flash_SPI_SendByte(Dummy_Byte);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  return Temp;
}

void Flash_SPI_WriteEnable(void)
{
	SPI_FLASH_CS_LOW();
	Flash_SPI_SendByte(W25X_WriteEnable);
	SPI_FLASH_CS_HIGH();
}

void Flash_SPI_WaitForWriteOrEraseEnd(void)
{
	u8 Flash_SPI_status=0;
	SPI_FLASH_CS_LOW();
	Flash_SPI_SendByte(W25X_ReadStatusReg);
	do
	{
		Flash_SPI_status=Flash_SPI_ReadByte();
	}while(Flash_SPI_status&WIP_Flag);
	SPI_FLASH_CS_HIGH();
}

void Flash_SPI_SectorErase(u32 SectorAddr)
{
	Flash_SPI_WriteEnable();
	Flash_SPI_WaitForWriteOrEraseEnd();
	SPI_FLASH_CS_LOW();
	Flash_SPI_SendByte(W25X_SectorErase);
	Flash_SPI_SendByte((SectorAddr&0xFF0000)>>16);
	Flash_SPI_SendByte((SectorAddr&0xFF00)>>8);
	Flash_SPI_SendByte(SectorAddr&0xFF);
	SPI_FLASH_CS_HIGH();
	Flash_SPI_WaitForWriteOrEraseEnd();
}

void Flash_SPI_BulkErase(void)
{
  /* 发送FLASH写使能命令 */
  Flash_SPI_WriteEnable();

  /* 整块 Erase */
  /* 选择FLASH: CS低电平 */
  SPI_FLASH_CS_LOW();
  /* 发送整块擦除指令*/
   Flash_SPI_SendByte(W25X_ChipErase);
  /* 停止信号 FLASH: CS 高电平 */
  SPI_FLASH_CS_HIGH();

  /* 等待擦除完毕*/
  Flash_SPI_WaitForWriteOrEraseEnd();
}


void Flash_SPI_PageWrite(u8 *pBuffer,u32 WriteAddr,u16 NumberByteToWrite)
{
	if(NumberByteToWrite>SPI_FLASH_PerWritePageSize)
	{
		FLASH_ERROR("超出页写范围!\n");
		return;
	}
	Flash_SPI_WriteEnable();
	Flash_SPI_WaitForWriteOrEraseEnd();
	SPI_FLASH_CS_LOW();
	Flash_SPI_SendByte(W25X_PageProgram);
	Flash_SPI_SendByte((WriteAddr&0xFF0000)>>16);
	Flash_SPI_SendByte((WriteAddr&0xFF00)>>8);
	Flash_SPI_SendByte(WriteAddr&0xFF);
	while(NumberByteToWrite--)
	{
		Flash_SPI_SendByte(*pBuffer);
		pBuffer++;
	}
	SPI_FLASH_CS_HIGH();
	Flash_SPI_WaitForWriteOrEraseEnd();
}

void Flash_SPI_BufferWrite(u8 *pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
	u8 count=0,NumOfPage=0,NumOfSingle=0,Addr=0,temp=0;
	if((WriteAddr+NumByteToWrite)>(FLASH_USER_SECTOR_START+FLASH_USER_SECTOR_SIZE))
	{
		FLASH_ERROR("超出用户写取范围，写地址:=0x%06X, 写长度=%d\n",WriteAddr,NumByteToWrite);
		return;
	}
	Addr=WriteAddr%SPI_FLASH_PageSize;
	count=SPI_FLASH_PageSize-Addr;
	NumOfPage=NumByteToWrite/SPI_FLASH_PageSize;
	NumOfSingle=NumByteToWrite%SPI_FLASH_PageSize;
	if(Addr==0)
	{
		if(NumOfPage==0)
		{
			Flash_SPI_PageWrite(pBuffer,WriteAddr,NumByteToWrite);
		}
		else
		{
			while(NumOfPage--)
			{
				Flash_SPI_PageWrite(pBuffer,WriteAddr,SPI_FLASH_PerWritePageSize);
				pBuffer+=SPI_FLASH_PageSize;
				WriteAddr+=SPI_FLASH_PageSize;
			}
			if(NumOfSingle!=0)
			{
				Flash_SPI_PageWrite(pBuffer,WriteAddr,NumOfSingle);
			}
		}
	}
	else
	{
		if(NumOfPage==0)
		{
			if(NumByteToWrite>count)
			{
				temp=NumByteToWrite-count;
				Flash_SPI_PageWrite(pBuffer,WriteAddr,count);
				pBuffer+=count;
				WriteAddr+=count;
				Flash_SPI_PageWrite(pBuffer,WriteAddr,temp);
			}
			else
			{
				Flash_SPI_PageWrite(pBuffer,WriteAddr,NumByteToWrite);
			}
		}
		else
		{
			NumByteToWrite-=count;
			NumOfPage=NumByteToWrite/SPI_FLASH_PageSize;
			NumOfSingle=NumByteToWrite%SPI_FLASH_PageSize;
			Flash_SPI_PageWrite(pBuffer,WriteAddr,count);
			pBuffer+=count;
			WriteAddr+=count;
			while(NumOfPage--)
			{
				Flash_SPI_PageWrite(pBuffer,WriteAddr,SPI_FLASH_PerWritePageSize);
				pBuffer+=SPI_FLASH_PageSize;
				WriteAddr+=SPI_FLASH_PageSize;
			}
			if(NumOfSingle!=0)
			{
				Flash_SPI_PageWrite(pBuffer,WriteAddr,NumOfSingle);
			}
		}
	}
}

void Flash_SPI_BufferRead(u8 *pBuffer,u32 ReadAddr,u16 NumByteToRead)
{
	SPI_FLASH_CS_LOW();
	Flash_SPI_SendByte(W25X_ReadData	);
	Flash_SPI_SendByte((ReadAddr&0xFF0000)>>16);
	Flash_SPI_SendByte((ReadAddr&0xFF00)>>8);
	Flash_SPI_SendByte(ReadAddr&0xFF);
	while(NumByteToRead--)
	{
		*pBuffer=Flash_SPI_ReadByte();
		pBuffer++;
	}
	SPI_FLASH_CS_HIGH();
}


void Flash_SPI_WriteDouble(double DoubleData,u32 WriteDoubleAddr)
{
	union {
        double d;
        u8 bytes[8];
    } converter;

    converter.d =DoubleData;
	if (WriteDoubleAddr + sizeof(double) > FLASH_USER_SECTOR_START+FLASH_USER_SECTOR_SIZE)
	{
			FLASH_ERROR("地址越界！禁止写入\n");
			return;
	}
	Flash_SPI_BufferWrite(converter.bytes, WriteDoubleAddr, sizeof(double));
}

double Flash_SPI_ReadDouble(u32 ReadDoubleAddr)
{
	union {
        double d;
        u8 bytes[8];
    } converter;
	Flash_SPI_BufferRead(converter.bytes,ReadDoubleAddr,sizeof(double));
	return converter.d;
}

void Flash_SPI_WriteU16(u16 U16Data,u32 WriteU16Addr)
{
	 union {
        u16 value;
        u8 bytes[2];
    } converter;
	  converter.value = U16Data;
	if (WriteU16Addr + sizeof(u16) > FLASH_USER_SECTOR_START+FLASH_USER_SECTOR_SIZE)
	{
			FLASH_ERROR("地址越界！禁止写入\n");
			return;
	}
	Flash_SPI_BufferWrite(converter.bytes,WriteU16Addr,sizeof(u16));
}

u16 Flash_SPI_ReadU16(u32 ReadU16Addr)
{
	 union {
        u16 value;
        u8 bytes[2];
    } converter;
	Flash_SPI_BufferRead(converter.bytes,ReadU16Addr,sizeof(u16));
	return converter.value;
}

void Flash_SPI_WriteAny(u32 addr, void* data, size_t len)
{
    Flash_SPI_BufferWrite((u8*)data, addr, len);
}

void Flash_SPI_ReadAny(u32 addr, void* data, size_t len)
{
    Flash_SPI_BufferRead((u8*)data, addr, len);
}

//进入掉电模式
void Flash_SPI_PowerDown(void)   
{ 
  /* 通讯开始：CS低 */
  SPI_FLASH_CS_LOW();

  /* 发送 掉电 命令 */
  Flash_SPI_SendByte(W25X_PowerDown);

  /*通讯结束：CS高 */
  SPI_FLASH_CS_HIGH();
}   


//唤醒
void Flash_SPI_WAKEUP(void)   
{
  /*选择 FLASH: CS 低 */
  SPI_FLASH_CS_LOW();

  /* 发送 上电 命令 */
  Flash_SPI_SendByte(W25X_ReleasePowerDown);

   /* 停止信号 FLASH: CS 高 */
  SPI_FLASH_CS_HIGH();
}
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* 等待超时后的处理,输出错误信息 */
  FLASH_ERROR("SPI 等待超时!errorCode = %d",errorCode);
  return 0;
}


