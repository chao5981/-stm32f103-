#include "stm32f10x.h"
#include "ADC.h"
#include "DMA.h"
#include "USART.h"

void Delay(__IO u32 nCount);

// ADC1转换的电压值通过MDA方式传到SRAM
extern __IO uint16_t ADC_ConvertedValue;

// 局部变量，用于保存转换计算后的电压值 
float ADC_ConvertedValueLocal;


int main(void)
{
	ADCx_Init();
	USART_Configuration();
	printf("这是一个单通道ADC和DMA发送数据的实验\n");
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
