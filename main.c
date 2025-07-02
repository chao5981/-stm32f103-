#include "stm32f10x.h"
#include "ADC.h"
#include "DMA.h"
#include "USART.h"

void Delay(__IO u32 nCount);

// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint16_t ADC_ConvertedValue;

// �ֲ����������ڱ���ת�������ĵ�ѹֵ 
float ADC_ConvertedValueLocal;


int main(void)
{
	ADCx_Init();
	USART_Configuration();
	printf("����һ����ͨ��ADC��DMA�������ݵ�ʵ��\n");
	while(1)
	{
		ADC_ConvertedValueLocal=(float)ADC_ConvertedValue/4096*3.3;
		printf("\r\n The current AD value = 0x%04X \r\n", ADC_ConvertedValue); 
		printf("\r\n The current AD value = %f V \r\n",ADC_ConvertedValueLocal); 
		Delay(0xffffee);  
	}
}

void Delay(__IO u32 nCount)
{
	for(;nCount>0;nCount--);
}
