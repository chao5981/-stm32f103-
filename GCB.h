#ifndef GCB_H
#define GCB_H

#include "stm32f10x.h"
/**
 * @brief 定义LED绿灯的端口
 * @param 无
 * @retval无
 */
#define LED_G_GPIO_PORT  GPIOB
#define LED_G_GPIO_RCC   RCC_APB2Periph_GPIOB
#define LED_G_GPIO_PIN   GPIO_Pin_0

/**
 * @brief 定义LED红灯的端口
 * @param 无
 * @retval无
 */
#define LED_R_GPIO_PORT  GPIOB
#define LED_R_GPIO_RCC   RCC_APB2Periph_GPIOB
#define LED_R_GPIO_PIN   GPIO_Pin_5

/**
 * @brief 定义LED蓝灯的端口
 * @param 无
 * @retval无
 */
#define LED_B_GPIO_PORT  GPIOB
#define LED_B_GPIO_RCC   RCC_APB2Periph_GPIOB
#define LED_B_GPIO_PIN   GPIO_Pin_1


//设置开关模式
#define ON  0
#define OFF 1

//用HAL固件库控制IO端口
#define LEDG(a) if(a) \
				GPIO_SetBits(LED_G_GPIO_PORT,LED_G_GPIO_PIN); \
				else  \
				GPIO_ResetBits(LED_G_GPIO_PORT,LED_G_GPIO_PIN)

#define LEDR(a) if(a) \
				GPIO_SetBits(LED_R_GPIO_PORT,LED_R_GPIO_PIN); \
				else  \
				GPIO_ResetBits(LED_R_GPIO_PORT,LED_R_GPIO_PIN)

#define LEDB(a) if(a) \
				GPIO_SetBits(LED_B_GPIO_PORT,LED_B_GPIO_PIN); \
				else  \
				GPIO_ResetBits(LED_B_GPIO_PORT,LED_B_GPIO_PIN)				
				
void LED_GPIO_Init(void);

#endif
