#include "IWDG.h"
#include "GCB.h"
#include "systick.h"
#include "key.h"
#include "USART.h"

int main(void)
{
	SysTick_EXTI_Init_1ms();
	LED_GPIO_Init();
	KEY_GPIO_Config();
	USART_Configuration();
	
	if(IWDG_RESET_FLAG_FUN(RCC_FLAG_IWDGRST)!=RESET)
	{
		LED_RED;
		SysTick_EXTI_Delay_1ms(1000);
		printf("复位！\r\n");
		IWDG_RESET_FLAG_CLEAR_FUN();
	}
	else
	{
		LED_BLUE;
		SysTick_EXTI_Delay_1ms(1000);
	}
	
	//一定要在复位函数后初始化IWDG，否则复位状态会被消除
	IWDG_Config(IWDG_Prescaler_64,2500);
	
	printf("***\n");
	
	
//while部分是我们在项目中具体需要写的代码，这部分的程序可以用独立看门狗来监控
	while(1)
	{
		LED_GREEN;
		if(KEY_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN)==KEY_ON)
		{
			IWDG_Feed();
			LED_WHITE;
			SysTick_EXTI_Delay_1ms(500);
			
		}
	}
}


