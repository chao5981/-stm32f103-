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
		printf("��λ��\r\n");
		IWDG_RESET_FLAG_CLEAR_FUN();
	}
	else
	{
		LED_BLUE;
		SysTick_EXTI_Delay_1ms(1000);
	}
	
	//һ��Ҫ�ڸ�λ�������ʼ��IWDG������λ״̬�ᱻ����
	IWDG_Config(IWDG_Prescaler_64,2500);
	
	printf("***\n");
	
	
//while��������������Ŀ�о�����Ҫд�Ĵ��룬�ⲿ�ֵĳ�������ö������Ź������
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


