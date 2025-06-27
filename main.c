#include "stm32f10x.h"
#include "ADC.h"
#include "DMA.h"
#include "USART.h"

void Delay(__IO u32 nCount);

// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint32_t ADC_ConvertedValue;

// �ֲ����������ڱ���ת�������ĵ�ѹֵ 
float ADC_ConvertedValueLocal[NOFCHANEL*2];


int main(void)
{
	u16 temp1=0,temp2=0;
	ADCx_Init();
	USART_Configuration();
	printf("����һ����ͨ��ADC��DMA�������ݵ�ʵ��\n");
	while(1)
	{
		temp1=(ADC_ConvertedValue&0xFFFF0000)>>16;
		temp2=(ADC_ConvertedValue&0xFFFF);
		ADC_ConvertedValueLocal[1]=(float)temp1/4096*3.3;
		printf("\r\n The ADC2 value = %f V \r\n",ADC_ConvertedValueLocal[1]); 
		ADC_ConvertedValueLocal[0]=(float)temp2/4096*3.3;
		printf("\r\n The ADC1 value = %f V \r\n",ADC_ConvertedValueLocal[0]); 
		printf("*******\n");
		Delay(0xffffee);  
	}
}

void Delay(__IO u32 nCount)
{
	for(;nCount>0;nCount--);
}
