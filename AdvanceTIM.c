#include "AdvanceTIM.h"

void Advance_TIM_NVIC_Config(void)
{
	NVIC_InitTypeDef Advance_TIM_NVIC_Structure;
	
	//分组
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_0);
	
	//配置NVIC结构体
	Advance_TIM_NVIC_Structure.NVIC_IRQChannel=ADVANCE_TIM_IRQ;
	Advance_TIM_NVIC_Structure.NVIC_IRQChannelCmd=ENABLE;
	Advance_TIM_NVIC_Structure.NVIC_IRQChannelPreemptionPriority=0;
	Advance_TIM_NVIC_Structure.NVIC_IRQChannelSubPriority=3;
	NVIC_Init(&Advance_TIM_NVIC_Structure);
}

void Advance_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef Advance_TIM_GPIO_Structure;
	
	//开启时钟
	ADVANCE_TIM_CH1_GPIO_CLK_FUN(ADVANCE_TIM_CH1_GPIO_CLK,ENABLE);
	
	//配置GPIO结构体
	Advance_TIM_GPIO_Structure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	Advance_TIM_GPIO_Structure.GPIO_Pin=ADVANCE_TIM_CH1_PIN;
	Advance_TIM_GPIO_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	
	//初始化结构体
	GPIO_Init(ADVANCE_TIM_CH1_PORT,&Advance_TIM_GPIO_Structure);
}

void Advance_TIM_Config(void)
{
	TIM_TimeBaseInitTypeDef Advance_TIM_Base_Structure;
	TIM_ICInitTypeDef Advance_TIM_IC_Structure;
	
	//开启时钟
	ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM_CLK,ENABLE);
	
	//配置时基结构体
	Advance_TIM_Base_Structure.TIM_ClockDivision=TIM_CKD_DIV1;
	Advance_TIM_Base_Structure.TIM_CounterMode=TIM_CounterMode_Up;
	//ARR
	Advance_TIM_Base_Structure.TIM_Period=ADVANCE_TIM_PERIOD;
	Advance_TIM_Base_Structure.TIM_Prescaler=ADVANCE_TIM_PSC;
	Advance_TIM_Base_Structure.TIM_RepetitionCounter=0;
	
	//初始化
	TIM_TimeBaseInit(ADVANCE_TIM,&Advance_TIM_Base_Structure);
	
	//配置输入捕获结构体
	Advance_TIM_IC_Structure.TIM_Channel=ADVANCE_TIM_IC1PWM_CHANNEL;
	Advance_TIM_IC_Structure.TIM_ICFilter=0;
	Advance_TIM_IC_Structure.TIM_ICPolarity=TIM_ICPolarity_Rising;
	Advance_TIM_IC_Structure.TIM_ICPrescaler=TIM_ICPSC_DIV1;
	Advance_TIM_IC_Structure.TIM_ICSelection=TIM_ICSelection_DirectTI;
	
	//初始化为PWM输入模式
	TIM_PWMIConfig(ADVANCE_TIM,&Advance_TIM_IC_Structure);
	
	// 当工作做PWM输入模式时,只需要设置触发信号的那一路即可（用于测量周期）
	// 另外一路（用于测量占空比）会由硬件自带设置，不需要再配置
	
	//选择输入信号的触发信号
	TIM_SelectInputTrigger(ADVANCE_TIM,TIM_TS_TI1FP1);
	
	//选择为从模式
	TIM_SelectSlaveMode(ADVANCE_TIM,TIM_SlaveMode_Reset);
	
	//使能从模式
	TIM_SelectMasterSlaveMode(ADVANCE_TIM,TIM_MasterSlaveMode_Enable);
	
	// 清除中断标志位
	TIM_ClearITPendingBit(ADVANCE_TIM, TIM_IT_CC1);
	
	// 使能捕获中断,这个中断针对的是主捕获通道（测量周期那个）
  TIM_ITConfig(ADVANCE_TIM, TIM_IT_CC1, ENABLE);	
	
	// 使能高级控制定时器，计数器开始计数
  TIM_Cmd(ADVANCE_TIM, ENABLE);
}

void ADVANCE_TIM_Init(void)
{
	Advance_TIM_GPIO_Config();
	Advance_TIM_NVIC_Config();
	Advance_TIM_Config();
}
