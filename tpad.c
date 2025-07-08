#include "tpad.h"
#include "systick.h"
#include "USART.h"


// ���ݰ������ص�ʱ����ʱ��
uint16_t tpad_default_val;

/**
 * @brief ��ʼ��GPIO�ṹ��
 * @param ��
 * @retval ��
 */
void TPAD_General_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef TPAD_General_TIM_GPIO_Structure;
	
	//����ʱ��
	TPAD_TIM_CH_GPIO_CLK_FUN(TPAD_TIM_CH_GPIO_CLK,ENABLE);
	
	//����GPIO�ṹ�壬ģʽ��ѡ��Ϊ���������ŵ�
	TPAD_General_TIM_GPIO_Structure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	TPAD_General_TIM_GPIO_Structure.GPIO_Pin=TPAD_TIM_CH_PIN;
	TPAD_General_TIM_GPIO_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(TPAD_TIM_CH_PORT,&TPAD_General_TIM_GPIO_Structure);
}


/**
 * @brief ͨ��TIM��������
 * @param ��
 * @retval ��
 */
void TPAD_General_TIM_Config(void)
{
	TIM_TimeBaseInitTypeDef TPAD_General_TIM_BASE_Structure;
	TIM_ICInitTypeDef TPAD_General_TIM_IC_Structure;
	
	//����ʱ��
	TPAD_TIM_APBxClock_FUN(TPAD_TIM_CLK,ENABLE);
	
	//����ʱ���ṹ��
	TPAD_General_TIM_BASE_Structure.TIM_ClockDivision=TIM_CKD_DIV1;
	TPAD_General_TIM_BASE_Structure.TIM_CounterMode=TIM_CounterMode_Up;
	TPAD_General_TIM_BASE_Structure.TIM_Period=TPAD_TIM_Period;
	TPAD_General_TIM_BASE_Structure.TIM_Prescaler=TPAD_TIM_Prescaler;
	TPAD_General_TIM_BASE_Structure.TIM_RepetitionCounter=0;
	
	TIM_TimeBaseInit(TPAD_TIM,&TPAD_General_TIM_BASE_Structure);
	
	//�������벶��ṹ��
	TPAD_General_TIM_IC_Structure.TIM_Channel=TPAD_TIM_CHANNEL_x;
	TPAD_General_TIM_IC_Structure.TIM_ICFilter=0x0;
	TPAD_General_TIM_IC_Structure.TIM_ICPolarity=TIM_ICPolarity_Rising;
	TPAD_General_TIM_IC_Structure.TIM_ICPrescaler=TIM_ICPSC_DIV1;
	TPAD_General_TIM_IC_Structure.TIM_ICSelection=TIM_ICSelection_DirectTI;
	
	TIM_ICInit(TPAD_TIM,&TPAD_General_TIM_IC_Structure);
	
	//ʹ��ͨ��TIM
	TIM_Cmd(TPAD_TIM,ENABLE);
	
}


/**
 * @brief TPADʱ�ӳ�ʼ��
 * @param ��
 * @retval ��
 */
void TPAD_TIM_Init(void)
{
	TPAD_General_TIM_GPIO_Config();
	TPAD_General_TIM_Config();
}


/**
 * @brief �õ����ȷŵ�Ȼ�󻻳ɳ��ģʽ
 * @param ��
 * @retval ��
 */
void TPAD_Reset(void)
{
	GPIO_InitTypeDef TPAD_General_TIM_GPIO_Structure;
	
	//����ʱ��
	TPAD_TIM_CH_GPIO_CLK_FUN(TPAD_TIM_CH_GPIO_CLK,ENABLE);
	
	//����GPIO�ṹ�壬ģʽ��ѡ��Ϊ���������ŵ�
	TPAD_General_TIM_GPIO_Structure.GPIO_Mode=GPIO_Mode_Out_PP;
	TPAD_General_TIM_GPIO_Structure.GPIO_Pin=TPAD_TIM_CH_PIN;
	TPAD_General_TIM_GPIO_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(TPAD_TIM_CH_PORT,&TPAD_General_TIM_GPIO_Structure);
	
	//�ŵ�
	GPIO_ResetBits(TPAD_TIM_CH_PORT,TPAD_TIM_CH_PIN);
	
	//��ʱһ��ȷ���ŵ����
	SysTick_NO_EXTI_Delay_1ms(5);
	
	//���³�ʼ��GPIOģʽ
	TPAD_General_TIM_GPIO_Structure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	
	GPIO_Init(TPAD_TIM_CH_PORT,&TPAD_General_TIM_GPIO_Structure);
}


/**
 * @brief �õ����ʱ��
 * @param ��
 * @retval ���س��ʱ��
 */
u16 TPAD_Get_Val(void)
{
	
	//�ŵ磬�����Զ����
	TPAD_Reset();
	
	//��������0
	TIM_SetCounter(TPAD_TIM,0);
	
	//������п��ܵ��жϱ�־λ
	TIM_ClearITPendingBit(TPAD_TIM,TIM_IT_Update|TPAD_TIM_IT_CCx);
	
	//����,�������ݳ��ʱ��
	/*ע����״̬���������жϼ�⣬��������һ��*/
	while(TIM_GetFlagStatus(TPAD_TIM,TPAD_TIM_IT_CCx)==RESET)
	{
		//�������Ĵ����ڼ�ʱ����������˻�û��׽���ߵ�ƽ,����ʧ��
		if(TIM_GetCounter(TPAD_TIM)>=TPAD_TIM_Period-100)
		{
			printf("\r\n���ݳ�����ʱ��ʧ�ܣ�\r\n");
			//ֱ�ӷ��ؼ�������ֵ
			return TIM_GetCounter(TPAD_TIM);
		}
	}
	//����ɹ����򷵻ز���Ĵ�����׽��ֵ
	return TPAD_TIM_GetCapturex_FUN(TPAD_TIM);
}


/**
 * @brief ��β���ȡ���ֵ
 * @param ��
 * @retval ���س��ʱ�����ֵ
 */
u16 TPAD_Get_Max_Val(u16 num)
{
	u16 temp=0,res=0;
	while(num--)
	{
		temp=TPAD_Get_Val();
		if(temp>res)
		{
			res=temp;
		}
	}
	return res;
}


/**
 * @brief TPAD��ʼ��
 * @param ��
 * @retval ���ص���Ĭ�ϳ��ʱ��
 */
u8 TPAD_Init(void)
{
	u16 temp=0;
	TPAD_TIM_Init();
	temp=TPAD_Get_Val();
	if((temp>=TPAD_DEFAULT_VAL_MIN)&&(temp<=TPAD_DEFAULT_VAL_MAX))
	{
		printf("\r\n���ݳ�ʼ���ɹ�,���ʱ��Ϊ%d\r\n",temp);
		//�ɹ������Ĭ�ϳ��ʱ����ȫ�ֱ���
		tpad_default_val=temp;
		return 0;
	}
	else
	{
		printf("\r\n���ݳ�ʼ��ʧ�ܣ�\r\n");
		printf("temp=%d",temp);
		return 1;
	}
}


/**
 * @brief ���ݰ���ɨ��
 * @param ��
 * @retval 1��ʾ���ݰ��£�0��ʾû�а���
 */
uint8_t TPAD_Scan(void)
{
	u8 res=0;
	u16 scan_val=0;
	//�������ݳ��ʱ��
	scan_val=TPAD_Get_Max_Val(3);
	//������ʱ������Ĭ��ֵ�����ż�ֵ����Ϊ����
	if(scan_val>(tpad_default_val+TPAD_GATE_VAL))
	{
		//�ٴβ�������������������
			scan_val=TPAD_Get_Max_Val(3);
		//����Ȼ���㣬����Ϊ������İ���
			if(scan_val>(tpad_default_val+TPAD_GATE_VAL))
			{
				res=1;
			}
	}
	return res;
}
