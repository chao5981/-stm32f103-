#include "stm32f10x.h"  
#include "USART.h"
#include "application.h"
#include "GCB.h"


int main(void)
{
	USART_Configuration();
	XPT2046_Init();
	ILI9341_Init();
	LED_GPIO_Init();
	Calibrate_or_Get_TouchParaWithFlash(6,0);
	ILI9341_GramScan(6);
	printf("这是一个液晶显示屏触摸板的实验\n");
	printf("实验开始\n");
	Application_Init();
	
	while(1)
	{
		XPT2046_TouchEvenHandler();
	}
}


