#include "tpad.h"
#include "systick.h"
#include "USART.h"


// 电容按键空载的时候充电时间
uint16_t tpad_default_val;

/**
 * @brief 初始化GPIO结构体
 * @param 无
 * @retval 无
 */
void TPAD_General_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef TPAD_General_TIM_GPIO_Structure;
	
	//开启时钟
	TPAD_TIM_CH_GPIO_CLK_FUN(TPAD_TIM_CH_GPIO_CLK,ENABLE);
	
	//配置GPIO结构体，模式先选择为输出，让其放电
	TPAD_General_TIM_GPIO_Structure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	TPAD_General_TIM_GPIO_Structure.GPIO_Pin=TPAD_TIM_CH_PIN;
	TPAD_General_TIM_GPIO_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(TPAD_TIM_CH_PORT,&TPAD_General_TIM_GPIO_Structure);
}


/**
 * @brief 通用TIM捕获配置
 * @param 无
 * @retval 无
 */
void TPAD_General_TIM_Config(void)
{
	TIM_TimeBaseInitTypeDef TPAD_General_TIM_BASE_Structure;
	TIM_ICInitTypeDef TPAD_General_TIM_IC_Structure;
	
	//开启时钟
	TPAD_TIM_APBxClock_FUN(TPAD_TIM_CLK,ENABLE);
	
	//配置时基结构体
	TPAD_General_TIM_BASE_Structure.TIM_ClockDivision=TIM_CKD_DIV1;
	TPAD_General_TIM_BASE_Structure.TIM_CounterMode=TIM_CounterMode_Up;
	TPAD_General_TIM_BASE_Structure.TIM_Period=TPAD_TIM_Period;
	TPAD_General_TIM_BASE_Structure.TIM_Prescaler=TPAD_TIM_Prescaler;
	TPAD_General_TIM_BASE_Structure.TIM_RepetitionCounter=0;
	
	TIM_TimeBaseInit(TPAD_TIM,&TPAD_General_TIM_BASE_Structure);
	
	//配置输入捕获结构体
	TPAD_General_TIM_IC_Structure.TIM_Channel=TPAD_TIM_CHANNEL_x;
	TPAD_General_TIM_IC_Structure.TIM_ICFilter=0x0;
	TPAD_General_TIM_IC_Structure.TIM_ICPolarity=TIM_ICPolarity_Rising;
	TPAD_General_TIM_IC_Structure.TIM_ICPrescaler=TIM_ICPSC_DIV1;
	TPAD_General_TIM_IC_Structure.TIM_ICSelection=TIM_ICSelection_DirectTI;
	
	TIM_ICInit(TPAD_TIM,&TPAD_General_TIM_IC_Structure);
	
	//使能通用TIM
	TIM_Cmd(TPAD_TIM,ENABLE);
	
}


/**
 * @brief TPAD时钟初始化
 * @param 无
 * @retval 无
 */
void TPAD_TIM_Init(void)
{
	TPAD_General_TIM_GPIO_Config();
	TPAD_General_TIM_Config();
}


/**
 * @brief 让电容先放电然后换成充电模式
 * @param 无
 * @retval 无
 */
void TPAD_Reset(void)
{
	GPIO_InitTypeDef TPAD_General_TIM_GPIO_Structure;
	
	//开启时钟
	TPAD_TIM_CH_GPIO_CLK_FUN(TPAD_TIM_CH_GPIO_CLK,ENABLE);
	
	//配置GPIO结构体，模式先选择为输出，让其放电
	TPAD_General_TIM_GPIO_Structure.GPIO_Mode=GPIO_Mode_Out_PP;
	TPAD_General_TIM_GPIO_Structure.GPIO_Pin=TPAD_TIM_CH_PIN;
	TPAD_General_TIM_GPIO_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(TPAD_TIM_CH_PORT,&TPAD_General_TIM_GPIO_Structure);
	
	//放电
	GPIO_ResetBits(TPAD_TIM_CH_PORT,TPAD_TIM_CH_PIN);
	
	//延时一下确保放电完成
	SysTick_NO_EXTI_Delay_1ms(5);
	
	//重新初始化GPIO模式
	TPAD_General_TIM_GPIO_Structure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	
	GPIO_Init(TPAD_TIM_CH_PORT,&TPAD_General_TIM_GPIO_Structure);
}


/**
 * @brief 得到充电时长
 * @param 无
 * @retval 返回充电时长
 */
u16 TPAD_Get_Val(void)
{
	
	//放电，马上自动充电
	TPAD_Reset();
	
	//计数器清0
	TIM_SetCounter(TPAD_TIM,0);
	
	//清除所有可能的中断标志位
	TIM_ClearITPendingBit(TPAD_TIM,TIM_IT_Update|TPAD_TIM_IT_CCx);
	
	//死等,测量电容充电时间
	/*注意是状态检测而不是中断检测，这俩个不一样*/
	while(TIM_GetFlagStatus(TPAD_TIM,TPAD_TIM_IT_CCx)==RESET)
	{
		//如果捕获寄存器在计时器快计数完了还没捕捉到高电平,则充电失败
		if(TIM_GetCounter(TPAD_TIM)>=TPAD_TIM_Period-100)
		{
			printf("\r\n电容充电测量时间失败！\r\n");
			//直接返回计数器的值
			return TIM_GetCounter(TPAD_TIM);
		}
	}
	//如果成功，则返回捕获寄存器捕捉的值
	return TPAD_TIM_GetCapturex_FUN(TPAD_TIM);
}


/**
 * @brief 多次测量取最大值
 * @param 无
 * @retval 返回充电时长最大值
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
 * @brief TPAD初始化
 * @param 无
 * @retval 返回电容默认充电时长
 */
u8 TPAD_Init(void)
{
	u16 temp=0;
	TPAD_TIM_Init();
	temp=TPAD_Get_Val();
	if((temp>=TPAD_DEFAULT_VAL_MIN)&&(temp<=TPAD_DEFAULT_VAL_MAX))
	{
		printf("\r\n电容初始化成功,充电时间为%d\r\n",temp);
		//成功则存入默认充电时长的全局变量
		tpad_default_val=temp;
		return 0;
	}
	else
	{
		printf("\r\n电容初始化失败！\r\n");
		printf("temp=%d",temp);
		return 1;
	}
}


/**
 * @brief 电容按键扫描
 * @param 无
 * @retval 1表示电容按下，0表示没有按下
 */
uint8_t TPAD_Scan(void)
{
	u8 res=0;
	u16 scan_val=0;
	//测量电容充电时长
	scan_val=TPAD_Get_Max_Val(3);
	//如果充电时长大于默认值加上门槛值则认为按下
	if(scan_val>(tpad_default_val+TPAD_GATE_VAL))
	{
		//再次测量，类似于消抖处理
			scan_val=TPAD_Get_Max_Val(3);
		//若任然满足，则认为电容真的按下
			if(scan_val>(tpad_default_val+TPAD_GATE_VAL))
			{
				res=1;
			}
	}
	return res;
}
