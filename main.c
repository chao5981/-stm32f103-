#include "stm32f10x.h"   // �൱��51��Ƭ���е�  #include <reg51.h>
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
	// ���������ʱ��ϵͳ��ʱ���Ѿ������ó�72M��
}

void Delay(__IO uint32_t count)
{	
	for(;count!=0;count--);
}
