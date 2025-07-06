#include "systick.h"
#include "core_cm3.h"
#include "misc.h"

static __IO u32 TimingDelay;
/**
 * @brief 初始化利用中断的系统计时器--10us级
 * @param 无
 * @retval 无
 */
void SysTickInit_10us(void)
{
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
	//其中SystemCoreClock=72000000
	//判断定时器是否配置正确，若失败则返回1
	if(SysTick_Config(SystemCoreClock / 100000))
	{
		while(1);
	}
}
/**
 * @brief 初始化利用中断的系统计时器--ms级
 * @param 无
 * @retval 无
 */
void SysTickInit_1ms(void)
{
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
	//其中SystemCoreClock=72000000
	//判断定时器是否配置正确，若失败则返回1
	if(SysTick_Config(SystemCoreClock / 1000))
	{
		while(1);
	}
}
/**
 * @brief 低CPU暂用的10us计时器
 * @param 延迟us时间
 * @retval 无
 */
void SysTick_EXTI_Delay_10us(__IO uint32_t time)
{
	TimingDelay=time;
	//使能计时器
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
	//让中断函数调用后自减，直到减到0才退出程序，这样理论上就是10us
	while(TimingDelay!=0);
}
/**
 * @brief 低CPU暂用的1ms计时器
 * @param 延迟us时间
 * @retval 无
 */
void SysTick_EXTI_Delay_1ms(__IO uint32_t time)
{
	TimingDelay=time;
		//使能计时器
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
	//让中断函数调用后自减，直到减到0才退出程序，这样理论上就是1ms
	while(TimingDelay!=0);
}
/**
 * @brief 给中断函数进行调用，达到每隔一段时间就让TimingDelay递减的目的
 * @param 无
 * @retval 无
 */
void TimingDelay_Reduction(void)
{
	if(TimingDelay!=0x00)
	{
		TimingDelay--;
	}
}

/**
 * @brief 无中断类型高CPU占用高精度10us计时器(内部已经初始化)
 * @param 传入要延时的10us
 * @retval 无
 */
void SysTick_NO_EXTI_Delay_10us(__IO uint32_t us)
{
	uint32_t i;
	//配置计时器
	SysTick_Config(SystemCoreClock / 100000);
	for(i=0;i<us;i++)
	{
		// 当计数器的值减小到0的时候，CRTL寄存器的位16会置1	
		while( !((SysTick->CTRL)&(1<<16)) );
	}
	// 关闭SysTick定时器
	SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;
}
/**
 * @brief 无中断类型高CPU占用高精度1ms计时器(内部已经初始化)
 * @param 传入要延时的10us
 * @retval 无
 */
void SysTick_NO_EXTI_Delay_1ms(__IO uint32_t ms)
{
	uint32_t i;
	//配置计时器
	SysTick_Config(SystemCoreClock /1000);
	for(i=0;i<ms;i++)
	{
		// 当计数器的值减小到0的时候，CRTL寄存器的位16会置1	
		while( !((SysTick->CTRL)&(1<<16)) );
	}
	// 关闭SysTick定时器
	SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;
}
