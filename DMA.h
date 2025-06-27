#ifndef __DMA_H
#define __DMA_H

#include "stm32f10x.h"

#define ADCx_1_DMA_Channel  		DMA1_Channel1
#define ADCx_1_DMA_CLK          RCC_AHBPeriph_DMA1
#define ADCx_1_DMA_SIZE         NOFCHANEL


void ADCx_DMA_Configuration(void);

#endif
