#include "exit_led.h"
/**
 * @brief 配置NVIC
 * @param 无
 * @retval 无
 */
static void NVIC_Configuration()
{
	//创建一个NVIC的结构体
	NVIC_InitTypeDef NVIC_InitStructure;
	//先配置子优先级和抢占优先级各个给多少位
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	//配置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	//配置子优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	//使能NVIC_IQN通道
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	//启动或者禁止该中断通道
	NVIC_InitStructure.NVIC_IRQChannel=KEY1_NVIC_IRQN;
	//初始化结构体
	NVIC_Init(&NVIC_InitStructure);
}
/**
 * @brief 初始化EXTI和GPIO
 * @param 无
 * @retval 无
 */
void Key_EXTI_Configuration()
{
	GPIO_InitTypeDef Keystructure;
	EXTI_InitTypeDef EXTIstructure;
	//嵌套调用NVIC_Configuration()，注意，它只需要配置一次
	NVIC_Configuration();
	//********GPIO的配置*********//
	//使能时钟
	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK,ENABLE);
	//确定引脚
	Keystructure.GPIO_Pin=GPIO_Pin_0;
	//确定输入模式
	Keystructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	//初始化GPIO结构体
	GPIO_Init(KEY1_GPIO_PORT,&Keystructure);
	//*******EXTI的配置*********//
	//确定中断的外设的来源
	GPIO_EXTILineConfig(KEY1_EXTI_PortSource,KEY1_EXTI_PinSource);
	//配置哪个外设的那个引脚导致中断(指定EXTI线)
	EXTIstructure.EXTI_Line=KEY1_EXTI_LINE;
	//配置是产生事件线还是中断线
	EXTIstructure.EXTI_Mode=EXTI_Mode_Interrupt;
	//配置边沿检测电路，如何才能才是中断
	EXTIstructure.EXTI_Trigger=EXTI_Trigger_Rising;
	//启动EXTI线(配置软件中断事件寄存器)
	EXTIstructure.EXTI_LineCmd=ENABLE;
	//初始化EXTI结构体
	EXTI_Init(&EXTIstructure);
	//手动清除一次中断标志位
	EXTI_ClearITPendingBit(KEY1_EXTI_LINE);
}
