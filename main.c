#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "USART.h"

#define WRITE_START_ADDR	((uint32_t)0x8000)
#define WRITE_END_ADDR		((uint32_t)0xC000)


/* STM32大容量产品每页大小2KByte，中、小容量产品每页大小1KByte */
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
	printf("正在进行读写内部FLASH实验，请耐心等待\r\n");
	
	if(Inter_Flash_Test()== 1)
	{
		printf("读写内部FLASH测试成功\r\n");

	}
	else
	{
		printf("读写内部FLASH测试失败\r\n");
	}
	
	
  while(1)
	{	} 
	
}

int Inter_Flash_Test(void)
{
	uint32_t EraseCounter = 0x00; 	//记录要擦除多少页
	uint32_t Address = 0x00;				//记录写入的地址
	uint32_t Data = 0x00;			//记录写入的数据
	uint32_t NbrOfPage = 0x00;			//记录写入多少页
	
	
	FLASH_Status FLASHStatus = FLASH_COMPLETE; //记录每次擦除的结果
	
	
	NbrOfPage=(WRITE_END_ADDR-WRITE_START_ADDR)/FLASH_PAGE_SIZE;
	
	//允许对 FLASH 的寄存器进行写操作
	FLASH_Unlock();
	
	//清除所有标志位
	FLASH_ClearFlag(FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR|FLASH_FLAG_EOP);
	
	
	//擦除所有页数
	for(EraseCounter=0;(EraseCounter<NbrOfPage)&&(FLASHStatus==FLASH_COMPLETE);EraseCounter++)
	{
		FLASHStatus=FLASH_ErasePage((EraseCounter*FLASH_PAGE_SIZE)+WRITE_START_ADDR);

	}
	
	//准备写入数据
	Address=WRITE_START_ADDR;
	
	for(;((Address<WRITE_END_ADDR)&&(FLASHStatus==FLASH_COMPLETE));Address+=4)
	{
		FLASHStatus=FLASH_ProgramWord(Address,Data);
		Data++;
	}
	
	
	//禁止对 FLASH 的寄存器进行写操作
	FLASH_Lock();
	
	//准备读取数据
	Address=WRITE_START_ADDR;
	
	//直接读指针
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
