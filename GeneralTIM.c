#include "GeneralTIM.h"
#include "DMA.h"
#include "USART.h"

void General_TIM_NVIC_Config(void)
{
	NVIC_InitTypeDef General_TIM_NVIC_Structure;
	//分组
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	
	
	General_TIM_NVIC_Structure.NVIC_IRQChannel=GENERAL_TIM_IRQ;
	
	General_TIM_NVIC_Structure.NVIC_IRQChannelCmd=ENABLE;
	
	General_TIM_NVIC_Structure.NVIC_IRQChannelPreemptionPriority=0;
	
	General_TIM_NVIC_Structure.NVIC_IRQChannelSubPriority=3;
	
	 NVIC_Init(&General_TIM_NVIC_Structure);
}

void General_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef General_TIM_GPIO_Structure;
	GENERAL_TIM_CH1_GPIO_CLK_FUN(GENERAL_TIM_CH1_GPIO_CLK,ENABLE);
	
	General_TIM_GPIO_Structure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	General_TIM_GPIO_Structure.GPIO_Pin=GENERAL_TIM_CH1_PIN;
	GPIO_Init(GENERAL_TIM_CH1_PORT,&General_TIM_GPIO_Structure);
}


void General_TIM_Config(void)
{
	TIM_ICInitTypeDef General_TIM_IC_Structure;
	TIM_TimeBaseInitTypeDef General_TIM_OC_Structure;
	GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK,ENABLE);
	
	/*******时基结构体配置*********/
	General_TIM_OC_Structure.TIM_ClockDivision=TIM_CKD_DIV1;
	
	General_TIM_OC_Structure.TIM_CounterMode=TIM_CounterMode_Up;
	
	General_TIM_OC_Structure.TIM_Period=GENERAL_TIM_PERIOD;
	
	General_TIM_OC_Structure.TIM_Prescaler=GENERAL_TIM_PSC;
	
	General_TIM_OC_Structure.TIM_RepetitionCounter=0;
	
	TIM_TimeBaseInit(GENERAL_TIM,&General_TIM_OC_Structure);
	
	/******捕获结构体配置*********/
	General_TIM_IC_Structure.TIM_Channel=GENERAL_TIM_CHANNEL_x;
	
	General_TIM_IC_Structure.TIM_ICFilter=0x04;
	
	General_TIM_IC_Structure.TIM_ICPolarity=GENERAL_TIM_START_ICPolarity;
	
	General_TIM_IC_Structure.TIM_ICPrescaler=TIM_ICPSC_DIV1;
	
	General_TIM_IC_Structure.TIM_ICSelection=TIM_ICSelection_DirectTI;
	
	TIM_ICInit(GENERAL_TIM,&General_TIM_IC_Structure);
	
}

void GENERAL_TIM_Init(void)
{
	General_TIM_GPIO_Config();
	General_TIM_NVIC_Config();
	DMA_GeneralTIM_Init();
	General_TIM_Config();
	// 清除更新和捕获中断标志位
  TIM_ClearFlag(GENERAL_TIM, TIM_FLAG_Update|GENERAL_TIM_IT_CCx);	
  // 开启更新和捕获中断  
	TIM_ITConfig (GENERAL_TIM, TIM_IT_Update | GENERAL_TIM_IT_CCx, ENABLE );
	// 使能计数器
	TIM_Cmd(GENERAL_TIM, ENABLE);
	
}
