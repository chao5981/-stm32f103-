#ifndef __DMA_H
#define __DMA_H

#include "stm32f10x.h"
#define TOP_SIZE  64
extern uint8_t TX_Buffer[TOP_SIZE];


#define USART_TX_DMA_CHANNEL   DMA1_Channel4
#define USART_DMA_CLK          RCC_AHBPeriph_DMA1


//����USART4���Ǿ���Ҫ���������ע�Ϳ���������ǰ���һ��ע�͵�
//#define USART_DMA_CLK          RCC_AHBPeriph_DMA1

void USARTx_DMA_Configuration(void);

#endif
