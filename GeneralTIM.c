#include "GeneralTIM.h"

void General_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef General_TIM_GPIO_Structure; 
	//开启时钟
	GENERAL_MASTER_GPIO_CLK_FUN(GENERAL_MASTER_GPIO_CLK,ENABLE);
	GENERAL_SLAVE1_GPIO_CLK_FUN(GENERAL_SLAVE1_GPIO_CLK,ENABLE);
	GENERAL_SLAVE11_GPIO_CLK_FUN(GENERAL_SLAVE11_GPIO_CLK,ENABLE);
	
	//由于用到了重映射，在开启时钟后还需要使能AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	//设置重映射
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2,ENABLE);//完全重映射TIM2_CH3->PB10
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);//完全重映射TIM3_CH1->PC6
	
	//初始化GPIO结构体
	
	//初始化TIM2_CH3(PB10)
	General_TIM_GPIO_Structure.GPIO_Mode=GPIO_Mode_AF_PP;
	General_TIM_GPIO_Structure.GPIO_Pin=GENERAL_MASTER_PIN;
	General_TIM_GPIO_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GENERAL_MASTER_GPIO_PORT,&General_TIM_GPIO_Structure);
	
	//初始化TIM3_CH1(PC6)
	General_TIM_GPIO_Structure.GPIO_Pin=GENERAL_SLAVE1_PIN;
	GPIO_Init(GENERAL_SLAVE1_GPIO_PORT,&General_TIM_GPIO_Structure);
	
	//初始化TIM4_CH1(PB6)
	General_TIM_GPIO_Structure.GPIO_Pin=GENERAL_SLAVE11_PIN;
	GPIO_Init(GENERAL_SLAVE11_GPIO_PORT,&General_TIM_GPIO_Structure);
}




void General_TIMx_Config(void)
{
	TIM_TimeBaseInitTypeDef General_TIMx_Base_Structure;
	TIM_OCInitTypeDef General_TIMx_OC_Structure;
	
	//开启时钟
	GENERAL_TIM_MASTER_CLK_FUN(GENERAL_TIM_MASTER_CLK,ENABLE);
	GENERAL_TIM_SLAVE1_CLK_FUN(GENERAL_TIM_SLAVE1_CLK,ENABLE);
	GENERAL_TIM_SLAVE11_CLK_FUN(GENERAL_TIM_SLAVE11_CLK,ENABLE);
	
	//配置时基结构体
	General_TIMx_Base_Structure.TIM_ClockDivision=TIM_CKD_DIV1;
	General_TIMx_Base_Structure.TIM_CounterMode=TIM_CounterMode_Up;
	General_TIMx_Base_Structure.TIM_Period=GENERAL_TIM_MASTER_PERIOD;
	General_TIMx_Base_Structure.TIM_Prescaler=GENERAL_TIM_MASTER_PSC;
	General_TIMx_Base_Structure.TIM_RepetitionCounter=0;
	
	//初始化TIM2时钟
	TIM_TimeBaseInit(GENERAL_TIM_MASTER,&General_TIMx_Base_Structure);
	
	//初始化TIM3时钟
	General_TIMx_Base_Structure.TIM_Period=GENERAL_TIM_SLAVE1_PERIOD;
	TIM_TimeBaseInit(GENERAL_TIM_SLAVE1,&General_TIMx_Base_Structure);

	//初始化TIM4时钟
	General_TIMx_Base_Structure.TIM_Period=GENERAL_TIM_SLAVE11_PERIOD;
	TIM_TimeBaseInit(GENERAL_TIM_SLAVE11,&General_TIMx_Base_Structure);
	
	//配置比较输出结构体(主)
	General_TIMx_OC_Structure.TIM_OCMode=TIM_OCMode_PWM1;
	General_TIMx_OC_Structure.TIM_OCPolarity=TIM_OCPolarity_High;
	General_TIMx_OC_Structure.TIM_OutputState=TIM_OutputState_Enable;
	General_TIMx_OC_Structure.TIM_Pulse=GENERAL_TIM_MASTER_PULSE;

	//初始化TIM2_CH3通道
	TIM_OC3Init(GENERAL_TIM_MASTER,&General_TIMx_OC_Structure);
	
	//使能主模式
	TIM_SelectMasterSlaveMode(GENERAL_TIM_MASTER,TIM_MasterSlaveMode_Enable);
	
	//设置主模式output触发源
	TIM_SelectOutputTrigger(GENERAL_TIM_MASTER,TIM_TRGOSource_Update);
	
	//配置比较输出结构体(从)
	General_TIMx_OC_Structure.TIM_OCMode=TIM_OCMode_PWM1;
	General_TIMx_OC_Structure.TIM_OCPolarity=TIM_OCPolarity_High;
	General_TIMx_OC_Structure.TIM_OutputState=TIM_OutputState_Enable;
	General_TIMx_OC_Structure.TIM_Pulse=GENERAL_TIM_SLAVE1_PULSE;
	
	//初始化TIM3_CH1通道
	TIM_OC1Init(GENERAL_TIM_SLAVE1,&General_TIMx_OC_Structure);
	
	/****由于TIM3作为TIM2的从机又最为TIM4的主机，所以主模式和从模式都需要设置****/	
	
	//使能主模式
	TIM_SelectMasterSlaveMode(GENERAL_TIM_SLAVE1,TIM_MasterSlaveMode_Enable);
	
	//设置主模式output触发源
	TIM_SelectOutputTrigger(GENERAL_TIM_SLAVE1,TIM_TRGOSource_Update);
	
	//使能从模式
	TIM_SelectSlaveMode(GENERAL_TIM_SLAVE1,TIM_SlaveMode_Gated);
	
	//设置从模式input触发源
	TIM_SelectInputTrigger(GENERAL_TIM_SLAVE1,TIM_TS_ITR1);
	
	
	//配置比较输出结构体(从从)
	General_TIMx_OC_Structure.TIM_OCMode=TIM_OCMode_PWM1;
	General_TIMx_OC_Structure.TIM_OCPolarity=TIM_OCPolarity_High;
	General_TIMx_OC_Structure.TIM_OutputState=TIM_OutputState_Enable;
	General_TIMx_OC_Structure.TIM_Pulse=GENERAL_TIM_SLAVE11_PULSE;
	
	//初始化TIM4_CH1通道
	TIM_OC1Init(GENERAL_TIM_SLAVE11,&General_TIMx_OC_Structure);
	
	//使能从模式
	TIM_SelectSlaveMode(GENERAL_TIM_SLAVE11,TIM_SlaveMode_Gated);
	
	//设置从模式input触发源
	TIM_SelectInputTrigger(GENERAL_TIM_SLAVE11,TIM_TS_ITR2);
	
	//使能TIM
	TIM_Cmd(GENERAL_TIM_MASTER,ENABLE);
	TIM_Cmd(GENERAL_TIM_SLAVE1,ENABLE);
	TIM_Cmd(GENERAL_TIM_SLAVE11,ENABLE);

}

void General_TIMx_Init(void)
{
	General_TIM_GPIO_Config();
	General_TIMx_Config();
}