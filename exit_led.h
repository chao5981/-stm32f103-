#ifndef __EXIT_LED_H
#define __EXIT_LED_H


#include "stm32f10x.h"

#define KEY1_GPIO_PORT  			GPIOA
#define KEY1_GPIO_CLK   			(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)
#define KEY1_GPIO_PIN   			GPIO_Pin_0
#define KEY1_EXTI_LINE  			EXTI_Line0
#define KEY1_EXTI_MODE  			EXTI_Mode_Interrupt
#define KEY1_EXTI_TRIGGER   	EXTI_Trigger_Rising
#define KEY1_NVIC_IRQN				EXTI0_IRQn
#define KEY1_EXTI_PortSource  GPIO_PortSourceGPIOA
#define KEY1_EXTI_PinSource   GPIO_PinSource0 
#define KEY1_EXTI_Handler     EXTI0_IRQHandler

void Key_EXTI_Configuration();


#endif