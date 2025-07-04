#include "AdvanceTIM.h"

void Advance_TIM_NVIC_Config(void)
{
	NVIC_InitTypeDef Advance_TIM_NVIC_Structure;
	
	//����
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_0);
	
	//����NVIC�ṹ��
	Advance_TIM_NVIC_Structure.NVIC_IRQChannel=ADVANCE_TIM_IRQ;
	Advance_TIM_NVIC_Structure.NVIC_IRQChannelCmd=ENABLE;
	Advance_TIM_NVIC_Structure.NVIC_IRQChannelPreemptionPriority=0;
	Advance_TIM_NVIC_Structure.NVIC_IRQChannelSubPriority=3;
	NVIC_Init(&Advance_TIM_NVIC_Structure);
}

void Advance_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef Advance_TIM_GPIO_Structure;
	
	//����ʱ��
	ADVANCE_TIM_CH1_GPIO_CLK_FUN(ADVANCE_TIM_CH1_GPIO_CLK,ENABLE);
	
	//����GPIO�ṹ��
	Advance_TIM_GPIO_Structure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	Advance_TIM_GPIO_Structure.GPIO_Pin=ADVANCE_TIM_CH1_PIN;
	Advance_TIM_GPIO_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	
	//��ʼ���ṹ��
	GPIO_Init(ADVANCE_TIM_CH1_PORT,&Advance_TIM_GPIO_Structure);
}

void Advance_TIM_Config(void)
{
	TIM_TimeBaseInitTypeDef Advance_TIM_Base_Structure;
	TIM_ICInitTypeDef Advance_TIM_IC_Structure;
	
	//����ʱ��
	ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM_CLK,ENABLE);
	
	//����ʱ���ṹ��
	Advance_TIM_Base_Structure.TIM_ClockDivision=TIM_CKD_DIV1;
	Advance_TIM_Base_Structure.TIM_CounterMode=TIM_CounterMode_Up;
	//ARR
	Advance_TIM_Base_Structure.TIM_Period=ADVANCE_TIM_PERIOD;
	Advance_TIM_Base_Structure.TIM_Prescaler=ADVANCE_TIM_PSC;
	Advance_TIM_Base_Structure.TIM_RepetitionCounter=0;
	
	//��ʼ��
	TIM_TimeBaseInit(ADVANCE_TIM,&Advance_TIM_Base_Structure);
	
	//�������벶��ṹ��
	Advance_TIM_IC_Structure.TIM_Channel=ADVANCE_TIM_IC1PWM_CHANNEL;
	Advance_TIM_IC_Structure.TIM_ICFilter=0;
	Advance_TIM_IC_Structure.TIM_ICPolarity=TIM_ICPolarity_Rising;
	Advance_TIM_IC_Structure.TIM_ICPrescaler=TIM_ICPSC_DIV1;
	Advance_TIM_IC_Structure.TIM_ICSelection=TIM_ICSelection_DirectTI;
	
	//��ʼ��ΪPWM����ģʽ
	TIM_PWMIConfig(ADVANCE_TIM,&Advance_TIM_IC_Structure);
	
	// ��������PWM����ģʽʱ,ֻ��Ҫ���ô����źŵ���һ·���ɣ����ڲ������ڣ�
	// ����һ·�����ڲ���ռ�ձȣ�����Ӳ���Դ����ã�����Ҫ������
	
	//ѡ�������źŵĴ����ź�
	TIM_SelectInputTrigger(ADVANCE_TIM,TIM_TS_TI1FP1);
	
	//ѡ��Ϊ��ģʽ
	TIM_SelectSlaveMode(ADVANCE_TIM,TIM_SlaveMode_Reset);
	
	//ʹ�ܴ�ģʽ
	TIM_SelectMasterSlaveMode(ADVANCE_TIM,TIM_MasterSlaveMode_Enable);
	
	// ����жϱ�־λ
	TIM_ClearITPendingBit(ADVANCE_TIM, TIM_IT_CC1);
	
	// ʹ�ܲ����ж�,����ж���Ե���������ͨ�������������Ǹ���
  TIM_ITConfig(ADVANCE_TIM, TIM_IT_CC1, ENABLE);	
	
	// ʹ�ܸ߼����ƶ�ʱ������������ʼ����
  TIM_Cmd(ADVANCE_TIM, ENABLE);
}

void ADVANCE_TIM_Init(void)
{
	Advance_TIM_GPIO_Config();
	Advance_TIM_NVIC_Config();
	Advance_TIM_Config();
}
