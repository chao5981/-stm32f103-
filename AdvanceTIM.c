#include "AdvanceTIM.h"

void GPIO_Advance_TIM_Config(void)
{
	GPIO_InitTypeDef GPIO_Advance_TIM_Structure;
	
	//*********��ʼ���Ƚ�ͨ��*********//
	ADVANCE_TIM_CH1_GPIO_CLK_FUN(ADVANCE_TIM_CH1_GPIO_CLK,ENABLE);
	GPIO_Advance_TIM_Structure.GPIO_Pin=ADVANCE_TIM_CH1_PIN;
	GPIO_Advance_TIM_Structure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Advance_TIM_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(ADVANCE_TIM_CH1_PORT,&GPIO_Advance_TIM_Structure);
	
	
	//*********��ʼ���������ͨ��***********//
	ADVANCE_TIM_CH1N_GPIO_CLK_FUN(ADVANCE_TIM_CH1N_GPIO_CLK,ENABLE);
	GPIO_Advance_TIM_Structure.GPIO_Pin=ADVANCE_TIM_CH1N_PIN;
	GPIO_Advance_TIM_Structure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Advance_TIM_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(ADVANCE_TIM_CH1N_PORT,&GPIO_Advance_TIM_Structure);
	
	//*********��ʼ��ɲ��ͨ��*************//
	ADVANCE_TIM_BKIN_GPIO_CLK_FUN(ADVANCE_TIM_BKIN_GPIO_CLK,ENABLE);
	GPIO_Advance_TIM_Structure.GPIO_Pin=ADVANCE_TIM_BKIN_PIN;
	GPIO_Advance_TIM_Structure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Advance_TIM_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(ADVANCE_TIM_BKIN_PORT,&GPIO_Advance_TIM_Structure);
	
	//��ɲ��ͨ��Ϊ�͵�ƽ����ֹ��ɲ������PWM�޷����
	GPIO_ResetBits(ADVANCE_TIM_BKIN_PORT,ADVANCE_TIM_BKIN_PIN);
}


///*
// * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
// * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
// * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            ����
// *	TIM_CounterMode			     TIMx,x[6,7]û�У���������
// *  TIM_Period               ����
// *  TIM_ClockDivision        TIMx,x[6,7]û�У���������
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]����
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM�ź� ���ں�ռ�ձȵļ���--------------- */
// ARR ���Զ���װ�ؼĴ�����ֵ
// CLK_cnt����������ʱ�ӣ����� Fck_int / (psc+1) = 72M/(psc+1)
// PWM �źŵ����� T = (ARR+1) * (1/CLK_cnt) = (ARR+1)*(PSC+1) / 72M
// ռ�ձ�P=CCR/(ARR+1)

void Advance_TIM_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
	ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM_CLK,ENABLE);
	
	//********����TIM��ʱ���ṹ��********//
	
	//ʱ�ӷ�Ƶ���ӣ�����������ʱ��
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	
	//������ģʽ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	
	//�Զ���װ��ֵ��ARR
	TIM_TimeBaseStructure.TIM_Period=ADVANCE_TIM_PERIOD;
	
	//Ԥ��Ƶ��(��PSC��ֵ)
	TIM_TimeBaseStructure.TIM_Prescaler=ADVANCE_TIM_PSC;
	
	//�ظ�������:ÿ���TIM_RepetitionCounter + 1 ��PWM���ں󴥷�һ�θ����жϺ�DMA����PWM�ź����һ������Ϊ0
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(ADVANCE_TIM,&TIM_TimeBaseStructure);
	
	
	//*********����TIM�Ļ�������ṹ��***********//
	
	//���ͨ�����е�ƽ
	TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;
	
	//�������ͨ�����е�ƽ
	TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCIdleState_Reset;
	
	//����Ƚ�ģʽ
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	
	//���������(�����豸�ĸ���ƽ��Ч)
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	
	//�����������(�����豸�ĸ���ƽ��Ч)
	TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCPolarity_High;
	
	//���ñȽϼĴ���(CRR)��ֵ
	TIM_OCInitStructure.TIM_Pulse=ADVANCE_TIM_PULSE;
	
	//�������ʹ��
	TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable;
	
	//�����ʹ��
	TIM_OCInitStructure.TIM_OutputState= TIM_OutputState_Enable;
	
	TIM_OC1Init(ADVANCE_TIM,&TIM_OCInitStructure);
	
	//ʹ�ܱȽϼĴ���(CCR)
	TIM_OC1PreloadConfig(ADVANCE_TIM,TIM_OCPreload_Enable);
	
	
	/**************��ʼ��������ɲ���ṹ��****************/
	
	//�Ƿ�����Ӳ���Զ��ָ�PWM���(ɲ�������)
	TIM_BDTRInitStructure.TIM_AutomaticOutput=TIM_AutomaticOutput_Enable;
	
	//ɲ��ʹ��
	TIM_BDTRInitStructure.TIM_Break=TIM_Break_Enable;
	
	//����ɲ�������ź�(BKIN)����Ч����(����ʱ����ɲ��)
	TIM_BDTRInitStructure.TIM_BreakPolarity=TIM_BreakPolarity_High;
	
	//��������ʱ��
	TIM_BDTRInitStructure.TIM_DeadTime=11;
	
	//���üĴ���д�������𣬷�ֹ���޸�����
	TIM_BDTRInitStructure.TIM_LOCKLevel=TIM_LOCKLevel_1;
	
	//����ģʽ(ɲ��ʱ)�µ����״̬(��TIM_OCIdleState���)
	TIM_BDTRInitStructure.TIM_OSSIState=TIM_OSSIState_Enable;
	
	//����ģʽ(��ɲ��ʱ)�µ����״̬(��TIM_OCIdleState���)
	TIM_BDTRInitStructure.TIM_OSSRState=TIM_OSSRState_Enable;
	
	//��ʼ���ṹ��
	TIM_BDTRConfig(ADVANCE_TIM, &TIM_BDTRInitStructure);
	
	//ʹ�ܼ�ʱ��
	TIM_Cmd(ADVANCE_TIM, ENABLE);	
	
	//���PWM
	TIM_CtrlPWMOutputs(ADVANCE_TIM, ENABLE);
}

void ADVANCE_TIM_Init(void)
{
	GPIO_Advance_TIM_Config();
	Advance_TIM_Config();
}

