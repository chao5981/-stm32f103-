#include "stm32f10x.h"   // �൱��51��Ƭ���е�  #include <reg51.h>
#include "USART.h"
#include "Flash.h"

//��ȡ��ID�洢λ��
__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;

// ����ԭ������
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
	USART_SendString(DEBUG_USARTx,"����һ����дFlash��ʵ��\n");
	printf("ʵ�鿪ʼ\n");
	DeviceID=Flash_SPI_ReadDeviceID();
	Delay(200);
	FlashID=Flash_SPI_ReadID();
	printf("\r\nFlashID is 0x%X,  Manufacturer Device ID is 0x%X\r\n", FlashID, DeviceID);
	if(sFLASH_ID==FlashID)
	{
		printf("��⵽Flash\n");
		Flash_SPI_BufferRead(&cal_flag,SPI_FLASH_PageSize*0, 1);
		if(cal_flag==0xCD)
		{
			printf("��⵽Flash��������,׼����ȡ����\n");
			for(i=0;i<4;i++)
			{
				Double_temp_Data=Flash_SPI_ReadDouble(DoubleAddr);
				DoubleAddr+=sizeof(double);
				printf("��%dλdouble��������:%f\n",i,Double_temp_Data);
			}
			for(i=0;i<4;i++)
			{
				U16_temp_Data=Flash_SPI_ReadU16(U16_temp_Addr);
				U16_temp_Addr+=sizeof(u16);
				printf("��%dλU16��������:%d\n",i,U16_temp_Data);
			}
		}
		else
		{
			printf("\r\nû�м�⵽���ݱ�־��FLASHû�д洢���ݣ���������С����u16��������д��ʵ��\r\n");
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
			printf("\r\n�븴λ�����壬�Զ�ȡ���ݽ��м���\r\n");
		}
	}
	else
	{
		printf("���ʧ�ܣ�\n");
	}
	Flash_SPI_PowerDown();  
	while(1);  
}


void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
