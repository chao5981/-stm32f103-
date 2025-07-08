#include "TIMbase.h"

void TIM_Base_NVIC_Config(void)
{
	NVIC_InitTypeDef TIM_Base_NVIC_Structure;
	//NVIC分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	//配置中断通道
	TIM_Base_NVIC_Structure.NVIC_IRQChannel=BASIC_TIM_IRQ;
	
	//配置抢占优先级大小
	TIM_Base_NVIC_Structure.NVIC_IRQChannelPreemptionPriority=0;
	
	//配置抢占子优先级大小
	TIM_Base_NVIC_Structure.NVIC_IRQChannelSubPriority=3;
	
	//启动中断通道
	TIM_Base_NVIC_Structure.NVIC_IRQChannelCmd=ENABLE;
	
	//初始化结构体
	NVIC_Init(&TIM_Base_NVIC_Structure);
}


/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 *typedef struct
 *{ TIM_Prescaler            都有
 *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
 *  TIM_Period               都有
 *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
 *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
 *}TIM_TimeBaseInitTypeDef; 
 *-----------------------------------------------------------------------------
 */


void TIM_base_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_Base_Structure;
	
	//初始化时钟
	BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK,ENABLE);
	
	//时钟分频因子，1为不分频，2为频率乘以2
	//TIM_Base_Structure.TIM_ClockDivision=TIM_CKD_DIV2;
	
	//计数器计数模式
	//TIM_Base_Structure.TIM_CounterMode=TIM_CounterMode_Up;
	
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_Base_Structure.TIM_Period=BASIC_TIM_Period;
	
	//// 时钟预分频数
	TIM_Base_Structure.TIM_Prescaler=BASIC_TIM_Prescaler;
	
	//重复计数器的值
	//TIM_Base_Structure.TIM_RepetitionCounter=0;
	
	//初始化定时器
	TIM_TimeBaseInit(BASIC_TIM,&TIM_Base_Structure);
	
	// 清除计数器中断标志位
  TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);
	  
	// 开启计数器中断
  TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);
		
	// 使能计数器
  TIM_Cmd(BASIC_TIM, ENABLE);	
	 
//	//暂时关闭定时器，等待使用
//	BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, DISABLE);
}


void TIM_Base_Init(void)
{
	TIM_Base_NVIC_Config();
	TIM_base_Config();
}
