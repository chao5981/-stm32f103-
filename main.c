#include "GCB.h"
#include "stm32f10x.h"
#define SOFT_DELAY Delay(0x0FFFFF);
// ���������ʱ��ϵͳ��ʱ���Ѿ������ó�72M��  
/**
 * @brief ������
 * @param ��
 * @retval ��
 */
void Delay(__IO uint32_t ncount)
{
	for(;ncount>0;ncount--);
}

int main(void)
{
	LED_GPIO_Init();
	while(1)
	{
		LEDG(ON);
		SOFT_DELAY;
		LEDG(OFF);
		
		LEDR(ON);
		SOFT_DELAY;
		LEDR(OFF);
		
		LEDB(ON);
		SOFT_DELAY;
		LEDB(OFF);
	
	}
}




