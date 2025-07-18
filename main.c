#include "USART.h"
#include "GCB.h"
#include "exti.h"


static void Delay(__IO u32 nCount);


int main(void)
{
	//����PWRʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	USART_Configuration();
	LED_GPIO_Init();
	PWR_EXTI_Config();
	
	printf("\r\n ******************* \r\n");
	
	printf("\r\n ʵ��˵����\r\n");

	printf("\r\n 1.�������У��̵Ʊ�ʾSTM32�������У���Ʊ�ʾ˯��״̬�����Ʊ�ʾ�մ�˯��״̬������\r\n");
	printf("\r\n 2.��������һ��ʱ����Զ�����˯��״̬����˯��״̬�£���ʹ��KEY1��KEY2����\r\n");
	printf("\r\n 3.��ʵ��ִ������һ��ѭ����\r\n ------�����̵�(��������)->�����(˯��ģʽ)->��KEY1��KEY2����->������(�ձ�����)-----��\r\n");
	printf("\r\n 4.��˯��״̬�£�DAP�������޷���STM32���س���\r\n �ɰ�KEY1��KEY2���Ѻ����أ�\r\n �򰴸�λ��ʹоƬ���ڸ�λ״̬��Ȼ���ڵ����ϵ�����ذ�ť�����ͷŸ�λ��������������\r\n");
	
	while(1)
	{
		/*********ִ������***************************/
		printf("\r\n STM32�������У����̵�\r\n");
	
		LED_GREEN;	
		Delay(0x3FFFFF);
		
		/*****����ִ����ϣ�����˯�߽��͹���***********/
		
		
		printf("\r\n ����˯��ģʽ����KEY1��KEY2�����ɻ���\r\n");

		//ʹ�ú��ָʾ������˯��״̬
		LED_RED;
		//����˯��ģʽ
		__WFI();	//WFIָ�����˯��
		
		//�ȴ��жϻ���  K1��K2�����ж�	
		
		/***�����ѣ�������ָʾ***/
		LED_BLUE;	
		Delay(0x1FFFFF);		
			
		printf("\r\n ���˳�˯��ģʽ\r\n");
		//����ִ��whileѭ��

	}
}


static void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}

