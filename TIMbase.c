#include "TIMbase.h"


/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 *typedef struct
 *{ TIM_Prescaler            ����
 *	TIM_CounterMode			     TIMx,x[6,7]û�У���������
 *  TIM_Period               ����
 *  TIM_ClockDivision        TIMx,x[6,7]û�У���������
 *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]����
 *}TIM_TimeBaseInitTypeDef; 
 *-----------------------------------------------------------------------------
 */


void TIM_base_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_Base_Structure;
	
	//��ʼ��ʱ��
	BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK,ENABLE);
	
	//ʱ�ӷ�Ƶ���ӣ�1Ϊ����Ƶ��2ΪƵ�ʳ���2
	//TIM_Base_Structure.TIM_ClockDivision=TIM_CKD_DIV2;
	
	//����������ģʽ
	//TIM_Base_Structure.TIM_CounterMode=TIM_CounterMode_Up;
	
	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_Base_Structure.TIM_Period=BASIC_TIM_Period;
	
	//// ʱ��Ԥ��Ƶ��
	TIM_Base_Structure.TIM_Prescaler=BASIC_TIM_Prescaler;
	
	//�ظ���������ֵ
	//TIM_Base_Structure.TIM_RepetitionCounter=0;
	
	//��ʼ����ʱ��
	TIM_TimeBaseInit(BASIC_TIM,&TIM_Base_Structure);
	
		
	// ʹ�ܼ�����
  TIM_Cmd(BASIC_TIM, ENABLE);	
	 

}

