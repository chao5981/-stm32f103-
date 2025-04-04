#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "clock.h"
#include "GCB.h"
void Delay(__IO uint32_t count);
int main(void)
{
	HSI_SYSClock(RCC_PLLMul_8);
	//HSI_SYSClock(RCC_PLLMul_16);
	LED_GPIO_Init();
	while(1)
	{
		LEDG(ON);
		Delay(0x0FFFFF);
		LEDG(OFF);
		Delay(0x0FFFFF);
	}
	// 来到这里的时候，系统的时钟已经被配置成72M。
}

void Delay(__IO uint32_t count)
{	
	for(;count!=0;count--);
}
