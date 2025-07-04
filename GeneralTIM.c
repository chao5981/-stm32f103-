#include "GeneralTIM.h"

void General_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef General_TIM_GPIO_Structure;
	
	//��ʱ��
	GENERAL_TIM_CH1_GPIO_CLK_FUN(GENERAL_TIM_CH1_GPIO_CLK,ENABLE);
	
	//����GPIO�ṹ��
	General_TIM_GPIO_Structure.GPIO_Mode=GPIO_Mode_AF_PP;
	General_TIM_GPIO_Structure.GPIO_Pin=GENERAL_TIM_CH1_PIN;
	General_TIM_GPIO_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GENERAL_TIM_CH1_PORT,&General_TIM_GPIO_Structure);
}



void General_TIM_Config(void)
{
	TIM_TimeBaseInitTypeDef General_TIM_BASE_Structure;
	TIM_OCInitTypeDef General_TIM_OC_Structure;
	
	//��ʱ��
	GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK,ENABLE);
	
	//����ʱ���ṹ��
	
	General_TIM_BASE_Structure.TIM_ClockDivision=TIM_CKD_DIV1;
	
	General_TIM_BASE_Structure.TIM_CounterMode=TIM_CounterMode_Up;
	//ARR
	General_TIM_BASE_Structure.TIM_Period=GENERAL_TIM_PERIOD;
	
	General_TIM_BASE_Structure.TIM_Prescaler=GENERAL_TIM_PSC;
	
	General_TIM_BASE_Structure.TIM_RepetitionCounter=0;
	
	TIM_TimeBaseInit(GENERAL_TIM,&General_TIM_BASE_Structure);
	
	//��������ṹ��
	
	General_TIM_OC_Structure.TIM_OCMode=TIM_OCMode_PWM1;
	
	General_TIM_OC_Structure.TIM_OCPolarity=TIM_OCPolarity_High;
	
	General_TIM_OC_Structure.TIM_OutputNState=TIM_OutputState_Disable;
	
	General_TIM_OC_Structure.TIM_OutputState=TIM_OutputState_Enable;
	
	General_TIM_OC_Structure.TIM_Pulse=GENERAL_TIM_CCR1;
	//��ʼ���ṹ��
	TIM_OC1Init(GENERAL_TIM,&General_TIM_OC_Structure);
	//�����ȽϼĴ���
	TIM_OC1PreloadConfig(GENERAL_TIM,TIM_OCPreload_Enable);
	//ʹ��TIM
	TIM_Cmd(GENERAL_TIM,ENABLE);
	
}

void GENERAL_TIM_Init(void)
{
	General_TIM_GPIO_Config();
	General_TIM_Config();
}
