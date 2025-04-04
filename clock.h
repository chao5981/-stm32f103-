#define _CLOCK_H__
#ifndef _CLOKC_H__

#include "stm32f10x.h"
 //RCC_PLLMul_x, x:[2,3,...16]
void HSE_SetSysClock(uint32_t pllmul);
void HSI_SYSClock(uint32_t pllmul);

#endif
