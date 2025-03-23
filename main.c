#include "GCB.h"
#include "stm32f10x.h"
#define SOFT_DELAY Delay(0x0FFFFF);
// 来到这里的时候，系统的时钟已经被配置成72M。  
/**
 * @brief 主函数
 * @param 无
 * @retval 无
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




