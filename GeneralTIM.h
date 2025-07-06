#ifndef __GENERALTIM_H
#define	__GENERALTIM_H

#include "stm32f10x.h"

/* 定时器 */
#define GENERAL_TIM_MASTER_CLK_FUN    RCC_APB1PeriphClockCmd
#define GENERAL_TIM_MASTER     		    TIM2
#define GENERAL_TIM_MASTER_CLK        RCC_APB1Periph_TIM2
#define GENERAL_TIM_MASTER_PERIOD     (72-1)
#define GENERAL_TIM_MASTER_PSC        0
#define GENERAL_TIM_MASTER_PULSE      36

#define GENERAL_TIM_SLAVE1_CLK_FUN    RCC_APB1PeriphClockCmd
#define GENERAL_TIM_SLAVE1     		    TIM3
#define GENERAL_TIM_SLAVE1_CLK        RCC_APB1Periph_TIM3
#define GENERAL_TIM_SLAVE1_PERIOD     (5-1)
#define GENERAL_TIM_SLAVE1_PSC        0
#define GENERAL_TIM_SLAVE1_PULSE      1

#define GENERAL_TIM_SLAVE11_CLK_FUN    RCC_APB1PeriphClockCmd
#define GENERAL_TIM_SLAVE11     		  TIM4
#define GENERAL_TIM_SLAVE11_CLK       RCC_APB1Periph_TIM4
#define GENERAL_TIM_SLAVE11_PERIOD    (5-1)
#define GENERAL_TIM_SLAVE11_PSC       0
#define GENERAL_TIM_SLAVE11_PULSE     1

/* TIM主模式PWM输出引脚 */
#define GENERAL_MASTER_GPIO_CLK_FUN   RCC_APB2PeriphClockCmd
#define GENERAL_MASTER_PIN            GPIO_Pin_10             
#define GENERAL_MASTER_GPIO_PORT      GPIOB                      
#define GENERAL_MASTER_GPIO_CLK       RCC_APB2Periph_GPIOB
#define GENERAL_MASTER_PINSOURCE			GPIO_PinSource5
#define GENERAL_MASTER_AF							GPIO_AF_TIM2

/* TIM从模式PWM输出引脚 */
#define GENERAL_SLAVE1_GPIO_CLK_FUN   RCC_APB2PeriphClockCmd
#define GENERAL_SLAVE1_PIN            GPIO_Pin_6             
#define GENERAL_SLAVE1_GPIO_PORT      GPIOC                      
#define GENERAL_SLAVE1_GPIO_CLK       RCC_APB2Periph_GPIOC
#define GENERAL_SLAVE1_PINSOURCE			GPIO_PinSource6
#define GENERAL_SLAVE1_AF							GPIO_AF_TIM3

/* TIM从模式的从模式PWM输出引脚 */
#define GENERAL_SLAVE11_GPIO_CLK_FUN  RCC_APB2PeriphClockCmd
#define GENERAL_SLAVE11_PIN           GPIO_Pin_6             
#define GENERAL_SLAVE11_GPIO_PORT     GPIOB                      
#define GENERAL_SLAVE11_GPIO_CLK      RCC_APB2Periph_GPIOB
#define GENERAL_SLAVE11_PINSOURCE			GPIO_PinSource6
#define GENERAL_SLAVE11_AF						GPIO_AF_TIM4

void General_TIMx_Init(void);

#endif /* __GENERALTIM_H */

