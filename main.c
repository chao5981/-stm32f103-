#include "stm32f10x.h"   // �൱��51��Ƭ���е�  #include <reg51.h>
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


