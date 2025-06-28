#include "TIMbase.h"
#include "GCB.h"
#include "USART.h"

volatile u32 time=0;
int main(void)
{
	LED_GPIO_Init();
	USART_Configuration();
	TIM_Base_Init();
	printf("***");
//	BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK,ENABLE);
	while(1)
	{
		if(time==1000)
		{
			time = 0;
			/* LED1 È¡·´ */      
			LED2_TOGGLE;
		}
	}
}


