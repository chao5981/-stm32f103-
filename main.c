#include "USART.h"   // �൱��51��Ƭ���е�  #include <reg51.h>

int main(void)
{
	USART_Configuration();
	USART_SendString(DEBUG_USARTx,"�����Ǵ������ݻ��յ�ʵ��\n");
	printf("��ӭ������");
	while(1)
	{
		
	}
}


