#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "USART.h"
#include "Flash.h"

//读取的ID存储位置
__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;

// 函数原型声明
void Delay(__IO uint32_t nCount);

int main(void)
{
	u8 cal_flag=0;
	u8 i=0;
	double DoubleBuffer[4]={1.11,2.22,3.33,5.44};
	u16 U16Buffer[4]={257,258,259,260};
	u32 DoubleAddr=SPI_FLASH_PageSize*2;
	u32 U16Addr=SPI_FLASH_PageSize*1;
	u32 Double_temp_Addr=DoubleAddr,U16_temp_Addr=U16Addr;
	double Double_temp_Data=0;
	u16 U16_temp_Data=0;
	USART_Configuration();
	Flash_SPI_Init();
	USART_SendString(DEBUG_USARTx,"这是一个读写Flash的实验\n");
	printf("实验开始\n");
	DeviceID=Flash_SPI_ReadDeviceID();
	Delay(200);
	FlashID=Flash_SPI_ReadID();
	printf("\r\nFlashID is 0x%X,  Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
	if(sFLASH_ID==FlashID)
	{
		printf("检测到Flash\n");
		Flash_SPI_BufferRead(&cal_flag,SPI_FLASH_PageSize*0, 1);
		if(cal_flag==0xCD)
		{
			printf("检测到Flash含有数据,准备读取数据\n");
			for(i=0;i<4;i++)
			{
				Double_temp_Data=Flash_SPI_ReadDouble(DoubleAddr);
				DoubleAddr+=sizeof(double);
				printf("第%d位double类型数据:%f\n",i,Double_temp_Data);
			}
			for(i=0;i<4;i++)
			{
				U16_temp_Data=Flash_SPI_ReadU16(U16_temp_Addr);
				U16_temp_Addr+=sizeof(u16);
				printf("第%d位U16类型数据:%d\n",i,U16_temp_Data);
			}
		}
		else
		{
			printf("\r\n没有检测到数据标志，FLASH没有存储数据，即将进行小数和u16类型数据写入实验\r\n");
			cal_flag=0xCD;
			Flash_SPI_SectorErase(0);
			Flash_SPI_BufferWrite(&cal_flag,SPI_FLASH_PageSize*0,1);
			DoubleAddr=Double_temp_Addr;
			for(i=0;i<4;i++)
			{
				Flash_SPI_WriteDouble(DoubleBuffer[i],DoubleAddr);
				DoubleAddr+=sizeof(double);
			}
			U16Addr=U16_temp_Addr;
			for(i=0;i<4;i++)
			{
				Flash_SPI_WriteU16(U16Buffer[i],U16Addr);
				U16Addr+=sizeof(u16);
			}
			printf("\r\n请复位开发板，以读取数据进行检验\r\n");
		}
	}
	else
	{
		printf("检测失败！\n");
	}
	Flash_SPI_PowerDown();  
	while(1);  
}


void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
