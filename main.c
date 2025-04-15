#include "USART.h"   // 相当于51单片机中的  #include <reg51.h>

int main(void)
{
	USART_Configuration();
	USART_SendString(DEBUG_USARTx,"这里是串口数据回收的实验\n");
	printf("欢迎回来！");
	while(1)
	{
		
	}
}


