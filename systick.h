#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

void SysTick_EXTI_Init_10us(void);
void SysTick_EXTI_Init_1ms(void);
void SysTick_EXTI_Delay_10us(__IO uint32_t time);
void SysTick_EXTI_Delay_1ms(__IO uint32_t time);
void SysTick_NO_EXTI_Delay_10us(__IO uint32_t us);
void SysTick_NO_EXTI_Delay_1ms(__IO uint32_t ms);
void TimingDelay_Reduction(void);

#endif
