#include "USART.h"
#include "WWDG.h"
#include "GCB.h"
#include "key.h"
#include "systick.h"

int main(void)
{
	uint8_t wwdg_tr, wwdg_wr; 
	u32 time=0;
	USART_Configuration();
	LED_GPIO_Init();
	KEY_GPIO_Config();
	SysTick_EXTI_Init_1ms();
	if (RCC->CSR & RCC_CSR_WWDGRSTF)
	{
		printf("\r\n程序复位！\r\n");
		LED_RED;
		SysTick_EXTI_Delay_1ms(1000);
	}
	else
	{
		LED_BLUE;
		SysTick_EXTI_Delay_1ms(1000);
	}
	
	/* WWDG 配置函数
 * ctv ：递减计时器的值， 取值范围为：0x7f~0x40
 * wdr ：窗口值，取值范围为：0x7f~0x40
 * prv：预分频器值，取值可以是
 *      @arg WWDG_Prescaler_1: WWDG counter clock = (PCLK1(36MHZ)/4096)/1
 *      @arg WWDG_Prescaler_2: WWDG counter clock = (PCLK1(36mhz)/4096)/2
 *      @arg WWDG_Prescaler_4: WWDG counter clock = (PCLK1(36mhz)/4096)/4
 *      @arg WWDG_Prescaler_8: WWDG counter clock = (PCLK1(36mhz)/4096)/8
 */
	WWDG_Config(0x7f,WWDG_Prescaler_8,0x5f);
	
	//读取窗口上限值
	wwdg_wr = WWDG->CFR & 0X7F;
	
	printf("\r\nWWDG实验\r\n");
	while(1)
	{
		LED_GREEN;
		time++;
		if(time>=1000000)
		{
			while(1);
		}
		
		//读取当前计数器的值
		wwdg_tr = WWDG->CR & 0X7F;
		if( wwdg_tr < wwdg_wr )
		{
			// 喂狗，重新设置计数器的值为最大0X7F
			WWDG_Feed();
		}
	}
}


