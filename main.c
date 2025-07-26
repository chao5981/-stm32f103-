#include "stm32f10x.h"   // �൱��51��Ƭ���е�  #include <reg51.h>
#include "USART.h"

#define WRITE_START_ADDR	((uint32_t)0x8000)
#define WRITE_END_ADDR		((uint32_t)0xC000)


/* STM32��������Ʒÿҳ��С2KByte���С�С������Ʒÿҳ��С1KByte */
#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
  #define FLASH_PAGE_SIZE    ((uint16_t)0x800)	//2048
#else
  #define FLASH_PAGE_SIZE    ((uint16_t)0x400)	//1024
#endif


int Inter_Flash_Test(void);

int main(void)
{
	USART_Configuration();
	printf("\r\n ************* \r\n");	
	printf("���ڽ��ж�д�ڲ�FLASHʵ�飬�����ĵȴ�\r\n");
	
	if(Inter_Flash_Test()== 1)
	{
		printf("��д�ڲ�FLASH���Գɹ�\r\n");

	}
	else
	{
		printf("��д�ڲ�FLASH����ʧ��\r\n");
	}
	
	
  while(1)
	{	} 
	
}

int Inter_Flash_Test(void)
{
	uint32_t EraseCounter = 0x00; 	//��¼Ҫ��������ҳ
	uint32_t Address = 0x00;				//��¼д��ĵ�ַ
	uint32_t Data = 0x00;			//��¼д�������
	uint32_t NbrOfPage = 0x00;			//��¼д�����ҳ
	
	
	FLASH_Status FLASHStatus = FLASH_COMPLETE; //��¼ÿ�β����Ľ��
	
	
	NbrOfPage=(WRITE_END_ADDR-WRITE_START_ADDR)/FLASH_PAGE_SIZE;
	
	//����� FLASH �ļĴ�������д����
	FLASH_Unlock();
	
	//������б�־λ
	FLASH_ClearFlag(FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR|FLASH_FLAG_EOP);
	
	
	//��������ҳ��
	for(EraseCounter=0;(EraseCounter<NbrOfPage)&&(FLASHStatus==FLASH_COMPLETE);EraseCounter++)
	{
		FLASHStatus=FLASH_ErasePage((EraseCounter*FLASH_PAGE_SIZE)+WRITE_START_ADDR);

	}
	
	//׼��д������
	Address=WRITE_START_ADDR;
	
	for(;((Address<WRITE_END_ADDR)&&(FLASHStatus==FLASH_COMPLETE));Address+=4)
	{
		FLASHStatus=FLASH_ProgramWord(Address,Data);
		Data++;
	}
	
	
	//��ֹ�� FLASH �ļĴ�������д����
	FLASH_Lock();
	
	//׼����ȡ����
	Address=WRITE_START_ADDR;
	
	//ֱ�Ӷ�ָ��
	Data=0x00;
	for(;(Address<WRITE_END_ADDR);Address+=4)
	{
		if(*(__IO u32 *)Address!=Data)
		{
			return 0;
		}
		Data++;
	}
	return 1;
}
