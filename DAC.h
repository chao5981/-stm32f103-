#ifndef __DAC_H
#define __DAC_H

#include "stm32f10x.h"


//正弦波单个周期的点数
#define POINT_NUM 32

//DAC DHR12RD寄存器，12位、右对齐、双通道
#define DAC_DHR12RD_ADDRESS      (DAC_BASE+0x20) 


//GPIO引脚宏定义
#define 	DAC_GPIO_CLK_FUN		RCC_APB2PeriphClockCmd
#define		DAC_GPIO_CLK				RCC_APB2Periph_GPIOA
#define		DAC_GPIO_PONT				GPIOA
#define		DAC1_GPIO_Pin				GPIO_Pin_4
#define		DAC2_GPIO_Pin				GPIO_Pin_5


//DAC宏定义
#define 	DAC_CLK_FUN					RCC_APB1PeriphClockCmd
#define 	DAC_CLK							RCC_APB1Periph_DAC


//函数

void DAC_Mode_Config(void);


#endif		/*__DAC_H*/

