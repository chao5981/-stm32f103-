#include "GeneralTIM.h"

void General_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef General_TIM_GPIO_Structure; 
	//����ʱ��
	GENERAL_MASTER_GPIO_CLK_FUN(GENERAL_MASTER_GPIO_CLK,ENABLE);
	GENERAL_SLAVE1_GPIO_CLK_FUN(GENERAL_SLAVE1_GPIO_CLK,ENABLE);
	GENERAL_SLAVE11_GPIO_CLK_FUN(GENERAL_SLAVE11_GPIO_CLK,ENABLE);
	
	//�����õ�����ӳ�䣬�ڿ���ʱ�Ӻ���Ҫʹ��AFIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	//������ӳ��
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2,ENABLE);//��ȫ��ӳ��TIM2_CH3->PB10
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);//��ȫ��ӳ��TIM3_CH1->PC6
	
	//��ʼ��GPIO�ṹ��
	
	//��ʼ��TIM2_CH3(PB10)
	General_TIM_GPIO_Structure.GPIO_Mode=GPIO_Mode_AF_PP;
	General_TIM_GPIO_Structure.GPIO_Pin=GENERAL_MASTER_PIN;
	General_TIM_GPIO_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GENERAL_MASTER_GPIO_PORT,&General_TIM_GPIO_Structure);
	
	//��ʼ��TIM3_CH1(PC6)
	General_TIM_GPIO_Structure.GPIO_Pin=GENERAL_SLAVE1_PIN;
	GPIO_Init(GENERAL_SLAVE1_GPIO_PORT,&General_TIM_GPIO_Structure);
	
	//��ʼ��TIM4_CH1(PB6)
	General_TIM_GPIO_Structure.GPIO_Pin=GENERAL_SLAVE11_PIN;
	GPIO_Init(GENERAL_SLAVE11_GPIO_PORT,&General_TIM_GPIO_Structure);
}




void General_TIMx_Config(void)
{
	TIM_TimeBaseInitTypeDef General_TIMx_Base_Structure;
	TIM_OCInitTypeDef General_TIMx_OC_Structure;
	
	//����ʱ��
	GENERAL_TIM_MASTER_CLK_FUN(GENERAL_TIM_MASTER_CLK,ENABLE);
	GENERAL_TIM_SLAVE1_CLK_FUN(GENERAL_TIM_SLAVE1_CLK,ENABLE);
	GENERAL_TIM_SLAVE11_CLK_FUN(GENERAL_TIM_SLAVE11_CLK,ENABLE);
	
	//����ʱ���ṹ��
	General_TIMx_Base_Structure.TIM_ClockDivision=TIM_CKD_DIV1;
	General_TIMx_Base_Structure.TIM_CounterMode=TIM_CounterMode_Up;
	General_TIMx_Base_Structure.TIM_Period=GENERAL_TIM_MASTER_PERIOD;
	General_TIMx_Base_Structure.TIM_Prescaler=GENERAL_TIM_MASTER_PSC;
	General_TIMx_Base_Structure.TIM_RepetitionCounter=0;
	
	//��ʼ��TIM2ʱ��
	TIM_TimeBaseInit(GENERAL_TIM_MASTER,&General_TIMx_Base_Structure);
	
	//��ʼ��TIM3ʱ��
	General_TIMx_Base_Structure.TIM_Period=GENERAL_TIM_SLAVE1_PERIOD;
	TIM_TimeBaseInit(GENERAL_TIM_SLAVE1,&General_TIMx_Base_Structure);

	//��ʼ��TIM4ʱ��
	General_TIMx_Base_Structure.TIM_Period=GENERAL_TIM_SLAVE11_PERIOD;
	TIM_TimeBaseInit(GENERAL_TIM_SLAVE11,&General_TIMx_Base_Structure);
	
	//���ñȽ�����ṹ��(��)
	General_TIMx_OC_Structure.TIM_OCMode=TIM_OCMode_PWM1;
	General_TIMx_OC_Structure.TIM_OCPolarity=TIM_OCPolarity_High;
	General_TIMx_OC_Structure.TIM_OutputState=TIM_OutputState_Enable;
	General_TIMx_OC_Structure.TIM_Pulse=GENERAL_TIM_MASTER_PULSE;

	//��ʼ��TIM2_CH3ͨ��
	TIM_OC3Init(GENERAL_TIM_MASTER,&General_TIMx_OC_Structure);
	
	//ʹ����ģʽ
	TIM_SelectMasterSlaveMode(GENERAL_TIM_MASTER,TIM_MasterSlaveMode_Enable);
	
	//������ģʽoutput����Դ
	TIM_SelectOutputTrigger(GENERAL_TIM_MASTER,TIM_TRGOSource_Update);
	
	//���ñȽ�����ṹ��(��)
	General_TIMx_OC_Structure.TIM_OCMode=TIM_OCMode_PWM1;
	General_TIMx_OC_Structure.TIM_OCPolarity=TIM_OCPolarity_High;
	General_TIMx_OC_Structure.TIM_OutputState=TIM_OutputState_Enable;
	General_TIMx_OC_Structure.TIM_Pulse=GENERAL_TIM_SLAVE1_PULSE;
	
	//��ʼ��TIM3_CH1ͨ��
	TIM_OC1Init(GENERAL_TIM_SLAVE1,&General_TIMx_OC_Structure);
	
	/****����TIM3��ΪTIM2�Ĵӻ�����ΪTIM4��������������ģʽ�ʹ�ģʽ����Ҫ����****/	
	
	//ʹ����ģʽ
	TIM_SelectMasterSlaveMode(GENERAL_TIM_SLAVE1,TIM_MasterSlaveMode_Enable);
	
	//������ģʽoutput����Դ
	TIM_SelectOutputTrigger(GENERAL_TIM_SLAVE1,TIM_TRGOSource_Update);
	
	//ʹ�ܴ�ģʽ
	TIM_SelectSlaveMode(GENERAL_TIM_SLAVE1,TIM_SlaveMode_Gated);
	
	//���ô�ģʽinput����Դ
	TIM_SelectInputTrigger(GENERAL_TIM_SLAVE1,TIM_TS_ITR1);
	
	
	//���ñȽ�����ṹ��(�Ӵ�)
	General_TIMx_OC_Structure.TIM_OCMode=TIM_OCMode_PWM1;
	General_TIMx_OC_Structure.TIM_OCPolarity=TIM_OCPolarity_High;
	General_TIMx_OC_Structure.TIM_OutputState=TIM_OutputState_Enable;
	General_TIMx_OC_Structure.TIM_Pulse=GENERAL_TIM_SLAVE11_PULSE;
	
	//��ʼ��TIM4_CH1ͨ��
	TIM_OC1Init(GENERAL_TIM_SLAVE11,&General_TIMx_OC_Structure);
	
	//ʹ�ܴ�ģʽ
	TIM_SelectSlaveMode(GENERAL_TIM_SLAVE11,TIM_SlaveMode_Gated);
	
	//���ô�ģʽinput����Դ
	TIM_SelectInputTrigger(GENERAL_TIM_SLAVE11,TIM_TS_ITR2);
	
	//ʹ��TIM
	TIM_Cmd(GENERAL_TIM_MASTER,ENABLE);
	TIM_Cmd(GENERAL_TIM_SLAVE1,ENABLE);
	TIM_Cmd(GENERAL_TIM_SLAVE11,ENABLE);

}

void General_TIMx_Init(void)
{
	General_TIM_GPIO_Config();
	General_TIMx_Config();
}