#include "PVD.h"

void PVD_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
	
	//开启PWR时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	//分组
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	//配置NVIC结构体
	NVIC_InitStructure.NVIC_IRQChannel = PVD_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	//配置EXTI结构体
	EXTI_InitStructure.EXTI_Line = EXTI_Line16;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
	//清除中断标志位
	EXTI_ClearITPendingBit(EXTI_Line16);
	
	//设定PVD的预定阈值,根据自己的实际情况选择
	PWR_PVDLevelConfig(PWR_PVDLevel_2V7);
	
	//使能PVD
	PWR_PVDCmd(ENABLE);
}

