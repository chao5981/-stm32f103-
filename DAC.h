#ifndef __DAC_H
#define __DAC_H

#include "stm32f10x.h"


//���Ҳ��������ڵĵ���
#define POINT_NUM 32

//DAC DHR12RD�Ĵ�����12λ���Ҷ��롢˫ͨ��
#define DAC_DHR12RD_ADDRESS      (DAC_BASE+0x20) 


//GPIO���ź궨��
#define 	DAC_GPIO_CLK_FUN		RCC_APB2PeriphClockCmd
#define		DAC_GPIO_CLK				RCC_APB2Periph_GPIOA
#define		DAC_GPIO_PONT				GPIOA
#define		DAC1_GPIO_Pin				GPIO_Pin_4
#define		DAC2_GPIO_Pin				GPIO_Pin_5


//DAC�궨��
#define 	DAC_CLK_FUN					RCC_APB1PeriphClockCmd
#define 	DAC_CLK							RCC_APB1Periph_DAC


//����

void DAC_Mode_Config(void);


#endif		/*__DAC_H*/

