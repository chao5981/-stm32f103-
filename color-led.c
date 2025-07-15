#include "color-led.h"

void Color_LED_GPIO_Config(void)
{
	GPIO_InitTypeDef Color_LED_GPIO_Structure;
	
	//开启时钟并且开启重映射时钟
	COLOR_GPIO_CLK_FUN(COLOR_TIM_GPIO_CLK,ENABLE);
	
	COLOR_GPIO_CLK_FUN(RCC_APB2Periph_AFIO,ENABLE);
	
	
	//重映射IO口设置
	COLOR_GPIO_REMAP_FUN();
	
	//配置GPIO结构体
	
	//红灯
	Color_LED_GPIO_Structure.GPIO_Mode=GPIO_Mode_AF_PP;
	Color_LED_GPIO_Structure.GPIO_Pin=COLOR_RED_TIM_LED_PIN;
	Color_LED_GPIO_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(COLOR_RED_TIM_LED_PORT,&Color_LED_GPIO_Structure);
	
	//绿灯
	Color_LED_GPIO_Structure.GPIO_Pin=COLOR_GREEN_TIM_LED_PIN;
	
	GPIO_Init(COLOR_GREEN_TIM_LED_PORT,&Color_LED_GPIO_Structure);

	//蓝灯
	Color_LED_GPIO_Structure.GPIO_Pin=COLOR_BLUE_TIM_LED_PIN;
	
	GPIO_Init(COLOR_BLUE_TIM_LED_PORT,&Color_LED_GPIO_Structure);
	
	
}


void Color_LED_TIMx_Config(void)
{
	TIM_TimeBaseInitTypeDef  Color_LED_TIMx_Base_Structure;
	TIM_OCInitTypeDef  Color_LED_TIMx_OC_Structure;

	//开启时钟
	COLOR_TIM_APBxClock_FUN(COLOR_TIM_CLK,ENABLE);
	
	//配置时基结构体
	Color_LED_TIMx_Base_Structure.TIM_ClockDivision= TIM_CKD_DIV1;
	Color_LED_TIMx_Base_Structure.TIM_CounterMode=TIM_CounterMode_Up;
	Color_LED_TIMx_Base_Structure.TIM_Period=COLOR_TIM_PERIOD;
	Color_LED_TIMx_Base_Structure.TIM_Prescaler=COLOR_TIM_PSC;
	Color_LED_TIMx_Base_Structure.TIM_RepetitionCounter=0;
	
	TIM_TimeBaseInit(COLOR_TIMx,&Color_LED_TIMx_Base_Structure);
	
	
	//配置输出结构体
	Color_LED_TIMx_OC_Structure.TIM_OCMode=TIM_OCMode_PWM1;
	Color_LED_TIMx_OC_Structure.TIM_OCPolarity=TIM_OCPolarity_Low;
	Color_LED_TIMx_OC_Structure.TIM_OutputState=TIM_OutputState_Enable;
	Color_LED_TIMx_OC_Structure.TIM_Pulse=0;
	
		//使能通道和预装载
  COLOR_RED_TIM_OCxInit(COLOR_TIMx, &Color_LED_TIMx_OC_Structure);	 							
  COLOR_RED_TIM_OCxPreloadConfig(COLOR_TIMx, TIM_OCPreload_Enable);						
  
	//使能通道和预装载
  COLOR_GREEN_TIM_OCxInit(COLOR_TIMx, &Color_LED_TIMx_OC_Structure);	 									
  COLOR_GREEN_TIM_OCxPreloadConfig(COLOR_TIMx, TIM_OCPreload_Enable);						
  
	//使能通道和预装载
  COLOR_BLUE_TIM_OCxInit(COLOR_TIMx, &Color_LED_TIMx_OC_Structure);	 									
  COLOR_BLUE_TIM_OCxPreloadConfig(COLOR_TIMx, TIM_OCPreload_Enable);
	
	//使能ARR预装载寄存器
	TIM_ARRPreloadConfig(COLOR_TIMx, ENABLE);	
	
	//使能定时器
	TIM_Cmd(COLOR_TIMx, ENABLE);
}


void COLOR_TIMx_LED_Init(void)
{
	Color_LED_GPIO_Config();
	Color_LED_TIMx_Config();
}


void COLOR_TIMx_LED_Close(void)
{
	SetColorValue(0,0,0);
	TIM_Cmd(COLOR_TIMx, DISABLE);                   							//失能定时器						
	COLOR_TIM_APBxClock_FUN(COLOR_TIM_CLK, DISABLE); 	//失能定时器时钟
	
	//关闭LED灯
	GPIO_SetBits(COLOR_RED_TIM_LED_PORT,COLOR_RED_TIM_LED_PIN);
	GPIO_SetBits(COLOR_GREEN_TIM_LED_PORT,COLOR_GREEN_TIM_LED_PIN);
	GPIO_SetBits(COLOR_BLUE_TIM_LED_PORT,COLOR_BLUE_TIM_LED_PIN);
}


/**
  * @brief  设置RGB LED的颜色
	* @param  rgb:要设置LED显示的颜色值格式RGB888
  * @retval 无
  */
void SetRGBColor(uint32_t rgb)
{
	//根据颜色值修改定时器的比较寄存器值
	COLOR_TIMx->COLOR_RED_CCRx = (uint8_t)(rgb>>16);			//R
	COLOR_TIMx->COLOR_GREEN_CCRx = (uint8_t)(rgb>>8);	  //G     
	COLOR_TIMx->COLOR_BLUE_CCRx = (uint8_t)rgb;						//B
}


 /**
  * @brief  设置RGB LED的颜色
	* @param  r\g\b:要设置LED显示的颜色值
  * @retval 无
  */
void SetColorValue(uint8_t r,uint8_t g,uint8_t b)
{
	//根据颜色值修改定时器的比较寄存器值
	COLOR_TIMx->COLOR_RED_CCRx = r;	
	COLOR_TIMx->COLOR_GREEN_CCRx = g;	        
	COLOR_TIMx->COLOR_BLUE_CCRx = b;	
}
