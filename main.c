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
	printf("����һ��Һ����ʾ���������ʵ��\n");
	printf("ʵ�鿪ʼ\n");
	Application_Init();
	
	while(1)
	{
		XPT2046_TouchEvenHandler();
	}
}


