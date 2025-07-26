#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "USART.h"
#include "key.h"

void WriteProtect_Toggle(void);
void ReadProtect_Toggle(void);


int main(void)
{
	USART_Configuration();
	KEY_GPIO_Config();
	
	
	if(FLASH_GetWriteProtectionOptionByte() ==0 )
	{
		printf("\r\n目前芯片处于写保护状态，按Key1键解除保护\r\n");
		printf("写保护寄存器的值：WRPR=0x%x\r\n",FLASH_GetWriteProtectionOptionByte());
	}
	else //无写保护
	{
		printf("\r\n目前芯片无 写 保护，按 Key1 键可设置成 写 保护\r\n");
		printf("写保护寄存器的值：WRPR=0x%x\r\n",FLASH_GetWriteProtectionOptionByte());
	}
	
	
	/*  若等于SET，说明处于读保护状态 */
	if(FLASH_GetReadOutProtectionStatus () == SET )
	{
		printf("\r\n目前芯片处于读保护状态，按Key2键解除保护\r\n");
	}
	else
	{
		printf("\r\n目前芯片无 读 保护，按 Key2 键可设置成 读 保护\r\n");
	}
	
	
	while(1)                            
	{	   
		if( KEY_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON  )
		{
			WriteProtect_Toggle();
		} 

		if( KEY_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON  )
		{
			ReadProtect_Toggle();			
		}		
	}	
}

/************************写保护反转************************/
void WriteProtect_Toggle(void)
{
	//
	if(FLASH_GetWriteProtectionOptionByte()==0)
	{
		printf("\r\n芯片处于写保护状态，即将执行解保护过程...\r\n");
		
		//允许对flash寄存器进行操作
		FLASH_Unlock();
		
		//擦除选项字节内容
		FLASH_EraseOptionBytes();
		
		//解除写保护
		FLASH_EnableWriteProtection(0);
		printf("\r\n配置完成，芯片将自动复位加载新配置，复位后芯片会解除写保护状态\r\n");
		
		//复位芯片，定义在core_cm3.h中，标准库查不到
		NVIC_SystemReset();
	}
	else
	{
		printf("\r\n芯片处于写保护状态，即将执行保护过程...\r\n");
		
		//允许对flash寄存器进行操作
		FLASH_Unlock();
		
		//擦除选项字节内容
		FLASH_EraseOptionBytes();
		
		//解除写保护
		FLASH_EnableWriteProtection(FLASH_WRProt_AllPages);
		printf("\r\n配置完成，芯片将自动复位加载新配置，复位后芯片会写保护状态\r\n");
		
		//复位芯片，定义在core_cm3.h中，标准库查不到
		NVIC_SystemReset();
	}
	
}

/************************读保护反转************************/
void ReadProtect_Toggle(void)
{
	if(FLASH_GetReadOutProtectionStatus()==SET)
	{
		printf("\r\n芯片处于读保护状态,即将解除读保护\r\n");
		
		//允许对flash寄存器进行操作
		FLASH_Unlock();
		
		//擦除选项字节内容
		FLASH_EraseOptionBytes();
		
		printf("即将解除读保护，解除读保护会把FLASH的所有内容清空");
		printf("由于解除后程序被清空，所以后面不会有任何提示输出");
		printf("等待20秒后即可给芯片下载新的程序...\r\n");
		
		FLASH_ReadOutProtection(DISABLE);
		
	}
	else
	{
		printf("\r\n芯片未处于读保护状态,即将执行读保护\r\n");
		
		//允许对flash寄存器进行操作
		FLASH_Unlock();
		
		//擦除选项字节内容
		FLASH_EraseOptionBytes();
		
		
		FLASH_ReadOutProtection(ENABLE);
		
		printf("芯片已被设置为读保护，上电复位后生效（必须重新给开发板上电，只按复位键无效）\r\n");
		printf("处于保护状态下无法正常下载新程序，必须要先解除保护状态再下载\r\n");
	}
}
