#ifndef __DMA_H
#define __DMA_H

#include "stm32f10x.h"

#define ADCx_DMA_Channel  		 DMA1_Channel1
#define ADCx_DMA_CLK          RCC_AHBPeriph_DMA1
#define ADCx_DMA_SIZE         1


void ADCx_DMA_Configuration(void);

#endif
