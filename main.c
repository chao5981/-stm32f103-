#include "GeneralTIM.h"
#include "DMA.h"
#include "USART.h"

extern u8 General_TIM_Capture_Flag;
extern u32 General_TIM_Capture_Period;
extern u8 General_TIM_New_Data_Ready;
__IO u32 DMA_GeneralTIM_Capture_Buffer[2];

int main(void)
{
	u32 time=0;
	
	// TIM 计数器的驱动时钟
	uint32_t TIM_PscCLK = 72000000 / (GENERAL_TIM_PSC+1);
	USART_Configuration();
	GENERAL_TIM_Init();
	printf("\r\n这是一个用通用TIM捕获测量脉宽实验\r\n");
	printf("\r\n请按下K1键\r\n");
	while(1)
	{
		if(General_TIM_New_Data_Ready==1)
		{
			if (DMA_GeneralTIM_Capture_Buffer[1] >= DMA_GeneralTIM_Capture_Buffer[0])
			{
				time=DMA_GeneralTIM_Capture_Buffer[1]-DMA_GeneralTIM_Capture_Buffer[0]+General_TIM_Capture_Period*(GENERAL_TIM_PERIOD+1);
			}
			else
			{
				time=DMA_GeneralTIM_Capture_Buffer[0]-DMA_GeneralTIM_Capture_Buffer[1]+General_TIM_Capture_Period*(GENERAL_TIM_PERIOD+1);
			}
			printf("%d   %d",DMA_GeneralTIM_Capture_Buffer[0],DMA_GeneralTIM_Capture_Buffer[1]);
			printf("\r\nPA0的脉宽:%d.%d s\r\n",time/TIM_PscCLK,time%TIM_PscCLK);
			General_TIM_New_Data_Ready=0;
		}
	}
}


