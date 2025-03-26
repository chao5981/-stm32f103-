#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"

//引脚定义
#define KEY1_GPIO_CLK    RCC_APB2Periph_GPIOA
#define KEY1_GPIO_PORT   GPIOA
#define KEY1_GPIO_PIN    GPIO_Pin_0

#define KEY2_GPIO_CLK    RCC_APB2Periph_GPIOC
#define KEY2_GPIO_PORT   GPIOC
#define KEY2_GPIO_PIN    GPIO_Pin_13

//此处摁下为高电平，松开为低电平
#define KEY_ON  1
#define KEY_OFF 0

 void KEY_GPIO_Config(void);
 uint8_t KEY_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);

#endif
