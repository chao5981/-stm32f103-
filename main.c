#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "GCB.h"
#include "exit_led.h"
int main(void)
{
	LED_GPIO_Init();
	Key_EXTI_Configuration();
	while(1)
	{
		LEDG(ON);
	}
}


