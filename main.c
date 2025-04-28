#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "I2C.h"
#include "DMA.h"
#include "USART.h"

	u8 U8BufferReceive[4]={0,0,0,0};
	u16 tempU16=0;
	double tempDouble;

int main(void)
{
	u8 U8Buffer[4]={2,5,9,10};
	u16 U16Buffer[4]={278,279,280,281};
	double DoubleBuffer[4]={1.11,2.22,3.33,4.44};
	u8 i;
	u8 DoubleBufferAddr=72,DoubleReadAddr = 72;
  u8 u16BufferAddr=40, U16ReadAddr = 40;
  u8 u8BufferAddr=10;
	USART_Configuration();
	USARTx_DMA_Configuration();
	I2C_EEPROM_Init();
	USART_SendString(DEBUG_USARTx,"这是一个写入小数到AT24C02的测试\n");
	printf("开始测试\n");
	printf("开始存入u8类型整数\n");
	I2C_EEPROM_Buffer_Write(U8Buffer,u8BufferAddr,4);
	printf("开始存入写入u16类型整数\n");
	for(i=0;i<4;i++)
	{
		printf("&&%d\n&&",U16Buffer[i]);
		I2C_EEPROM_WriteUInt16(u16BufferAddr,U16Buffer[i]);
		u16BufferAddr+=sizeof(u16);
	}
	printf("开始存入写入double类型小数\n");
	for(i=0;i<4;i++)
	{
		printf("&&%f&&\n",DoubleBuffer[i]);
		I2C_EEPROM_WriteDouble(DoubleBufferAddr,DoubleBuffer[i]);
		DoubleBufferAddr+=sizeof(double);
	}
	printf("数据全部存储完毕，准备读取数据\n");
	printf("读取u8类型整数\n");
	I2C_EEPROM_BufferRead(U8BufferReceive,u8BufferAddr,4);
	for(i=0;i<4;i++)
	{
		printf("**%d**\n",U8BufferReceive[i]);
	}
	printf("读取u16类型的整数\n");
	for(i=0;i<4;i++)
	{
		tempU16=I2C_EEPROM_ReadUInt16(U16ReadAddr);
		U16ReadAddr+=sizeof(u16);
		printf("**%d**\n",tempU16);
	}
	printf("读取double类型的小数\n");
	for(i=0;i<4;i++)
	{
		tempDouble=I2C_EEPROM_ReadDouble(DoubleReadAddr);
		DoubleReadAddr+=sizeof(double);
		printf("**%f**\n",tempDouble);
	}
	printf("实验结束\n");
	 while (1)
  {      
  }
}


