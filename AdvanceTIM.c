#include "AdvanceTIM.h"

void GPIO_Advance_TIM_Config(void)
{
	GPIO_InitTypeDef GPIO_Advance_TIM_Structure;
	
	//*********初始化比较通道*********//
	ADVANCE_TIM_CH1_GPIO_CLK_FUN(ADVANCE_TIM_CH1_GPIO_CLK,ENABLE);
	GPIO_Advance_TIM_Structure.GPIO_Pin=ADVANCE_TIM_CH1_PIN;
	GPIO_Advance_TIM_Structure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Advance_TIM_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(ADVANCE_TIM_CH1_PORT,&GPIO_Advance_TIM_Structure);
	
	
	//*********初始化互补输出通道***********//
	ADVANCE_TIM_CH1N_GPIO_CLK_FUN(ADVANCE_TIM_CH1N_GPIO_CLK,ENABLE);
	GPIO_Advance_TIM_Structure.GPIO_Pin=ADVANCE_TIM_CH1N_PIN;
	GPIO_Advance_TIM_Structure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Advance_TIM_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(ADVANCE_TIM_CH1N_PORT,&GPIO_Advance_TIM_Structure);
	
	//*********初始化刹车通道*************//
	ADVANCE_TIM_BKIN_GPIO_CLK_FUN(ADVANCE_TIM_BKIN_GPIO_CLK,ENABLE);
	GPIO_Advance_TIM_Structure.GPIO_Pin=ADVANCE_TIM_BKIN_PIN;
	GPIO_Advance_TIM_Structure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Advance_TIM_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(ADVANCE_TIM_BKIN_PORT,&GPIO_Advance_TIM_Structure);
	
	//让刹车通道为低电平，防止勿刹车导致PWM无法输出
	GPIO_ResetBits(ADVANCE_TIM_BKIN_PORT,ADVANCE_TIM_BKIN_PIN);
}


///*
// * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
// * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
// * 另外三个成员是通用定时器和高级定时器才有.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            都有
// *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM信号 周期和占空比的计算--------------- */
// ARR ：自动重装载寄存器的值
// CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
// PWM 信号的周期 T = (ARR+1) * (1/CLK_cnt) = (ARR+1)*(PSC+1) / 72M
// 占空比P=CCR/(ARR+1)

void Advance_TIM_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
	ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM_CLK,ENABLE);
	
	//********配置TIM的时基结构体********//
	
	//时钟分频因子，控制死区的时间
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	
	//计数器模式
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	
	//自动重装载值，ARR
	TIM_TimeBaseStructure.TIM_Period=ADVANCE_TIM_PERIOD;
	
	//预分频器(即PSC的值)
	TIM_TimeBaseStructure.TIM_Prescaler=ADVANCE_TIM_PSC;
	
	//重复计数器:每完成TIM_RepetitionCounter + 1 个PWM周期后触发一次更新中断和DMA请求，PWM信号输出一般设置为0
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(ADVANCE_TIM,&TIM_TimeBaseStructure);
	
	
	//*********配置TIM的互补输出结构体***********//
	
	//输出通道空闲电平
	TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;
	
	//互补输出通道空闲电平
	TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCIdleState_Reset;
	
	//输出比较模式
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	
	//主输出极性(告诉设备哪个电平有效)
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	
	//互补输出极性(告诉设备哪个电平有效)
	TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCPolarity_High;
	
	//设置比较寄存器(CRR)的值
	TIM_OCInitStructure.TIM_Pulse=ADVANCE_TIM_PULSE;
	
	//互补输出使能
	TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable;
	
	//主输出使能
	TIM_OCInitStructure.TIM_OutputState= TIM_OutputState_Enable;
	
	TIM_OC1Init(ADVANCE_TIM,&TIM_OCInitStructure);
	
	//使能比较寄存器(CCR)
	TIM_OC1PreloadConfig(ADVANCE_TIM,TIM_OCPreload_Enable);
	
	
	/**************初始化死区和刹车结构体****************/
	
	//是否允许硬件自动恢复PWM输出(刹车解除后)
	TIM_BDTRInitStructure.TIM_AutomaticOutput=TIM_AutomaticOutput_Enable;
	
	//刹车使能
	TIM_BDTRInitStructure.TIM_Break=TIM_Break_Enable;
	
	//设置刹车输入信号(BKIN)的有效极性(即何时触发刹车)
	TIM_BDTRInitStructure.TIM_BreakPolarity=TIM_BreakPolarity_High;
	
	//设置死区时长
	TIM_BDTRInitStructure.TIM_DeadTime=11;
	
	//配置寄存器写保护级别，防止误修改配置
	TIM_BDTRInitStructure.TIM_LOCKLevel=TIM_LOCKLevel_1;
	
	//空闲模式(刹车时)下的输出状态(与TIM_OCIdleState配合)
	TIM_BDTRInitStructure.TIM_OSSIState=TIM_OSSIState_Enable;
	
	//运行模式(非刹车时)下的输出状态(与TIM_OCIdleState配合)
	TIM_BDTRInitStructure.TIM_OSSRState=TIM_OSSRState_Enable;
	
	//初始化结构体
	TIM_BDTRConfig(ADVANCE_TIM, &TIM_BDTRInitStructure);
	
	//使能计时器
	TIM_Cmd(ADVANCE_TIM, ENABLE);	
	
	//输出PWM
	TIM_CtrlPWMOutputs(ADVANCE_TIM, ENABLE);
}

void ADVANCE_TIM_Init(void)
{
	GPIO_Advance_TIM_Config();
	Advance_TIM_Config();
}

