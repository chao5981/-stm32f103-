#include "AdvanceTIM.h"
#include "USART.h"
#include "GeneralTIM.h"

int main(void)
{
		USART_Configuration();
		GENERAL_TIM_Init();
		ADVANCE_TIM_Init();
		
		printf("PWM实验，请把A6引脚连接至A8\n");
		while(1)
		{
			
		}
}


