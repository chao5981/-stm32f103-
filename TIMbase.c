#include "TIMbase.h"


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
	
		
	// 使能计数器
  TIM_Cmd(BASIC_TIM, ENABLE);	
	 

}

