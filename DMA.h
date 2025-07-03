#ifndef __DMA_H
#define __DMA_H


#include "stm32f10x.h"

#define DMA_General_TIMx_CLK_FUN						RCC_AHBPeriphClockCmd
#define DMA_General_TIMx_CLK								RCC_AHBPeriph_DMA2 
#define DMA_General_TIMx										TIM5
#define DMA_General_TIMx_Capture_Register   TIM_DMA_CC1
#define DMA_General_TIMx_Capture_Data_Num   2
#define DMA_General_TIMx_Channely						DMA2_Channel5


extern __IO u32 DMA_GeneralTIM_Capture_Buffer[2];

void DMA_GeneralTIM_Init(void);

#endif /*__DMA_H*/
