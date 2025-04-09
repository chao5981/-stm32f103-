#include "systick.h"// 相当于51单片机中的  #include <reg51.h>
#include "GCB.h"
int main(void)
{
	SysTickInit_1ms();
	LED_GPIO_Init();
	while(1)
	{
		LEDG(ON);
		SysTick_EXTI_Delay_1ms(1000);
		LEDG(OFF);
		
		LEDR(ON);
		SysTick_EXTI_Delay_1ms(1000);
		LEDR(OFF);
		
		LEDB(ON);
		SysTick_EXTI_Delay_1ms(1000);
		LEDB(OFF);
	}
}


