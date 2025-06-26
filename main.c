#include "stm32f10x.h"
#include "ADC.h"
#include "DMA.h"
#include "USART.h"

void Delay(__IO u32 nCount);

// ADC1转换的电压值通过MDA方式传到SRAM
extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL];

// 局部变量，用于保存转换计算后的电压值 
float ADC_ConvertedValueLocal[NOFCHANEL];


int main(void)
{
	ADCx_Init();
	USART_Configuration();
	printf("这是一个多通道ADC和DMA发送数据的实验\n");
	while(1)
	{
		ADC_ConvertedValueLocal[0]=(float)ADC_ConvertedValue[0]/4096*3.3;
		printf(" \nPC0 value = %f V\n ",ADC_ConvertedValueLocal[0]); 
		ADC_ConvertedValueLocal[1]=(float)ADC_ConvertedValue[1]/4096*3.3;
		printf("PC1 value = %f V \n",ADC_ConvertedValueLocal[1]); 
		ADC_ConvertedValueLocal[2]=(float)ADC_ConvertedValue[2]/4096*3.3;
		printf("PC2 value = %f V \n",ADC_ConvertedValueLocal[2]); 
		ADC_ConvertedValueLocal[3]=(float)ADC_ConvertedValue[3]/4096*3.3;
		printf("PC3 value = %f V \n",ADC_ConvertedValueLocal[3]); 
		ADC_ConvertedValueLocal[4]=(float)ADC_ConvertedValue[4]/4096*3.3; 
		printf("PC4 value = %f V \n",ADC_ConvertedValueLocal[4]); 
		ADC_ConvertedValueLocal[5]=(float)ADC_ConvertedValue[5]/4096*3.3;
		printf("PC5 value = %f V \r\n",ADC_ConvertedValueLocal[5]); 
		Delay(0xffffee);  
	}
}

void Delay(__IO u32 nCount)
{
	for(;nCount>0;nCount--);
}
