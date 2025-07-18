#include "exti.h"

void PWR_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 配置NVIC为优先级组1 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* 配置中断源：按键1 */
  NVIC_InitStructure.NVIC_IRQChannel = KEY1_INT_EXTI_IRQ;
  /* 配置抢占优先级 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 配置子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
  NVIC_Init(&NVIC_InitStructure);
	
	
	/* 配置中断源：按键2，其他使用上面相关配置 */  
  NVIC_InitStructure.NVIC_IRQChannel = KEY2_INT_EXTI_IRQ;
  NVIC_Init(&NVIC_InitStructure);
}

void PWR_EXTI_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;
	
	//开启时钟
	RCC_APB2PeriphClockCmd(KEY1_INT_GPIO_CLK,ENABLE);
	
	//开启NVIC
	PWR_NVIC_Config();

	//配置key1的GPIO和EXTI结构体

  GPIO_InitStructure.GPIO_Pin = KEY1_INT_GPIO_PIN;

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure);
	
	EXTI_InitStructure.EXTI_Line=KEY1_INT_EXTI_LINE;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
	
	//确定中断会从哪个引脚过来
	GPIO_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE,KEY1_INT_EXTI_PINSOURCE);
	
	EXTI_Init(&EXTI_InitStructure);
	
	
	//配置key2的GPIO和EXTI结构体
	GPIO_InitStructure.GPIO_Pin = KEY2_INT_GPIO_PIN;

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);
	
	EXTI_InitStructure.EXTI_Line=KEY2_INT_EXTI_LINE;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	
	//确定中断会从哪个引脚过来
	GPIO_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE,KEY2_INT_EXTI_PINSOURCE);
	EXTI_Init(&EXTI_InitStructure);
	
}

