#include "AdvanceTIM.h"
#include "USART.h"
#include "GeneralTIM.h"

int main(void)
{
		USART_Configuration();
		GENERAL_TIM_Init();
		ADVANCE_TIM_Init();
		
		printf("PWMʵ�飬���A6����������A8\n");
		while(1)
		{
			
		}
}


