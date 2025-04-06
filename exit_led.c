#include "exit_led.h"

static void NVIC_Configuration()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannel=KEY1_NVIC_IRQN;
	NVIC_Init(&NVIC_InitStructure);
}

void Key_EXTI_Configuration()
{
	GPIO_InitTypeDef Keystructure;
	EXTI_InitTypeDef EXTIstructure;
	NVIC_Configuration();
	Keystructure.GPIO_Pin=GPIO_Pin_0;
	Keystructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(KEY1_GPIO_PORT,&Keystructure);
	GPIO_EXTILineConfig(KEY1_EXTI_PortSource,KEY1_EXTI_PinSource);
	EXTIstructure.EXTI_Line=KEY1_EXTI_LINE;
	EXTIstructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTIstructure.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTIstructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTIstructure);
}