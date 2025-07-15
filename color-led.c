#include "color-led.h"

void Color_LED_GPIO_Config(void)
{
	GPIO_InitTypeDef Color_LED_GPIO_Structure;
	
	//����ʱ�Ӳ��ҿ�����ӳ��ʱ��
	COLOR_GPIO_CLK_FUN(COLOR_TIM_GPIO_CLK,ENABLE);
	
	COLOR_GPIO_CLK_FUN(RCC_APB2Periph_AFIO,ENABLE);
	
	
	//��ӳ��IO������
	COLOR_GPIO_REMAP_FUN();
	
	//����GPIO�ṹ��
	
	//���
	Color_LED_GPIO_Structure.GPIO_Mode=GPIO_Mode_AF_PP;
	Color_LED_GPIO_Structure.GPIO_Pin=COLOR_RED_TIM_LED_PIN;
	Color_LED_GPIO_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(COLOR_RED_TIM_LED_PORT,&Color_LED_GPIO_Structure);
	
	//�̵�
	Color_LED_GPIO_Structure.GPIO_Pin=COLOR_GREEN_TIM_LED_PIN;
	
	GPIO_Init(COLOR_GREEN_TIM_LED_PORT,&Color_LED_GPIO_Structure);

	//����
	Color_LED_GPIO_Structure.GPIO_Pin=COLOR_BLUE_TIM_LED_PIN;
	
	GPIO_Init(COLOR_BLUE_TIM_LED_PORT,&Color_LED_GPIO_Structure);
	
	
}


void Color_LED_TIMx_Config(void)
{
	TIM_TimeBaseInitTypeDef  Color_LED_TIMx_Base_Structure;
	TIM_OCInitTypeDef  Color_LED_TIMx_OC_Structure;

	//����ʱ��
	COLOR_TIM_APBxClock_FUN(COLOR_TIM_CLK,ENABLE);
	
	//����ʱ���ṹ��
	Color_LED_TIMx_Base_Structure.TIM_ClockDivision= TIM_CKD_DIV1;
	Color_LED_TIMx_Base_Structure.TIM_CounterMode=TIM_CounterMode_Up;
	Color_LED_TIMx_Base_Structure.TIM_Period=COLOR_TIM_PERIOD;
	Color_LED_TIMx_Base_Structure.TIM_Prescaler=COLOR_TIM_PSC;
	Color_LED_TIMx_Base_Structure.TIM_RepetitionCounter=0;
	
	TIM_TimeBaseInit(COLOR_TIMx,&Color_LED_TIMx_Base_Structure);
	
	
	//��������ṹ��
	Color_LED_TIMx_OC_Structure.TIM_OCMode=TIM_OCMode_PWM1;
	Color_LED_TIMx_OC_Structure.TIM_OCPolarity=TIM_OCPolarity_Low;
	Color_LED_TIMx_OC_Structure.TIM_OutputState=TIM_OutputState_Enable;
	Color_LED_TIMx_OC_Structure.TIM_Pulse=0;
	
		//ʹ��ͨ����Ԥװ��
  COLOR_RED_TIM_OCxInit(COLOR_TIMx, &Color_LED_TIMx_OC_Structure);	 							
  COLOR_RED_TIM_OCxPreloadConfig(COLOR_TIMx, TIM_OCPreload_Enable);						
  
	//ʹ��ͨ����Ԥװ��
  COLOR_GREEN_TIM_OCxInit(COLOR_TIMx, &Color_LED_TIMx_OC_Structure);	 									
  COLOR_GREEN_TIM_OCxPreloadConfig(COLOR_TIMx, TIM_OCPreload_Enable);						
  
	//ʹ��ͨ����Ԥװ��
  COLOR_BLUE_TIM_OCxInit(COLOR_TIMx, &Color_LED_TIMx_OC_Structure);	 									
  COLOR_BLUE_TIM_OCxPreloadConfig(COLOR_TIMx, TIM_OCPreload_Enable);
	
	//ʹ��ARRԤװ�ؼĴ���
	TIM_ARRPreloadConfig(COLOR_TIMx, ENABLE);	
	
	//ʹ�ܶ�ʱ��
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
	TIM_Cmd(COLOR_TIMx, DISABLE);                   							//ʧ�ܶ�ʱ��						
	COLOR_TIM_APBxClock_FUN(COLOR_TIM_CLK, DISABLE); 	//ʧ�ܶ�ʱ��ʱ��
	
	//�ر�LED��
	GPIO_SetBits(COLOR_RED_TIM_LED_PORT,COLOR_RED_TIM_LED_PIN);
	GPIO_SetBits(COLOR_GREEN_TIM_LED_PORT,COLOR_GREEN_TIM_LED_PIN);
	GPIO_SetBits(COLOR_BLUE_TIM_LED_PORT,COLOR_BLUE_TIM_LED_PIN);
}


/**
  * @brief  ����RGB LED����ɫ
	* @param  rgb:Ҫ����LED��ʾ����ɫֵ��ʽRGB888
  * @retval ��
  */
void SetRGBColor(uint32_t rgb)
{
	//������ɫֵ�޸Ķ�ʱ���ıȽϼĴ���ֵ
	COLOR_TIMx->COLOR_RED_CCRx = (uint8_t)(rgb>>16);			//R
	COLOR_TIMx->COLOR_GREEN_CCRx = (uint8_t)(rgb>>8);	  //G     
	COLOR_TIMx->COLOR_BLUE_CCRx = (uint8_t)rgb;						//B
}


 /**
  * @brief  ����RGB LED����ɫ
	* @param  r\g\b:Ҫ����LED��ʾ����ɫֵ
  * @retval ��
  */
void SetColorValue(uint8_t r,uint8_t g,uint8_t b)
{
	//������ɫֵ�޸Ķ�ʱ���ıȽϼĴ���ֵ
	COLOR_TIMx->COLOR_RED_CCRx = r;	
	COLOR_TIMx->COLOR_GREEN_CCRx = g;	        
	COLOR_TIMx->COLOR_BLUE_CCRx = b;	
}
