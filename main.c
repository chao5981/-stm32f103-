#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "GCB.h"
#include "Key.h"
// 来到这里的时候，系统的时钟已经被配置成72M。
int main(void)
{
	LED_GPIO_Init();
	KEY_GPIO_Config();
	LEDB(ON);
	while(1)
	{
		if(KEY_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN))
		{
			/* 关闭所有led灯	*/
			GPIO_SetBits(LED_G_GPIO_PORT, LED_G_GPIO_PIN);
				
			/* 关闭所有led灯	*/
			GPIO_SetBits(LED_R_GPIO_PORT, LED_R_GPIO_PIN);	 
				
			/* 关闭所有led灯	*/
			GPIO_SetBits(LED_B_GPIO_PORT, LED_B_GPIO_PIN);
			LEDG(ON);
		}
		if(KEY_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN))
		{
			/* 关闭所有led灯	*/
			GPIO_SetBits(LED_G_GPIO_PORT, LED_G_GPIO_PIN);
				
			/* 关闭所有led灯	*/
			GPIO_SetBits(LED_R_GPIO_PORT, LED_R_GPIO_PIN);	 
				
			/* 关闭所有led灯	*/
			GPIO_SetBits(LED_B_GPIO_PORT, LED_B_GPIO_PIN);
			LEDR(ON);
		}
	}
}


