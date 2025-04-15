#ifndef __DMA_H
#define __DMA_H

#include "stm32f10x.h"
#define TOP_SIZE  64
extern uint8_t TX_Buffer[TOP_SIZE];


#define USART_TX_DMA_CHANNEL   DMA1_Channel4
#define USART_DMA_CLK          RCC_AHBPeriph_DMA1


//对于USART4，那就需要把下面这个注释开启，并把前面的一行注释掉
//#define USART_DMA_CLK          RCC_AHBPeriph_DMA1

void USARTx_DMA_Configuration(void);

#endif
