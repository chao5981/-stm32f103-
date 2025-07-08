#include "IWDG.h"

/*
 * 设置 IWDG 的超时时间
 * Tout = prv/40 * rlv (s)
 *      prv可以是[4,8,16,32,64,128,256]
 * prv:预分频器值，取值如下：
 *     @arg IWDG_Prescaler_4: IWDG prescaler set to 4
 *     @arg IWDG_Prescaler_8: IWDG prescaler set to 8
 *     @arg IWDG_Prescaler_16: IWDG prescaler set to 16
 *     @arg IWDG_Prescaler_32: IWDG prescaler set to 32
 *     @arg IWDG_Prescaler_64: IWDG prescaler set to 64
 *     @arg IWDG_Prescaler_128: IWDG prescaler set to 128
 *     @arg IWDG_Prescaler_256: IWDG prescaler set to 256
 *
 * rlv:重装载寄存器的值，取值范围为：0-0XFFF
 * 函数调用举例：
 * IWDG_Config(IWDG_Prescaler_64 ,625);  // IWDG 1s 超时溢出
 * 溢出时间 Tout = prv/40 * rlv(s) Tout=64/40*625=1s
 */



/**
 * @brief 初始化IWDG
 * @param prv:prescaler value：预分频器的值 ；rlv:reload value:重载寄存器的值
 * @retval 无
 */
void IWDG_Config(u8 prv, u16 rlv)
{
	//使能 预分频寄存器PR和重装载寄存器RLR可写
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	//设置预分频器的值
	IWDG_SetPrescaler(prv);
	
	//设置重装载寄存器的值
	IWDG_SetReload(rlv);
	
	//把重装载寄存器的值放到计数器中
	IWDG_ReloadCounter();
	
	//使能IWDG
	IWDG_Enable();
	
}


void IWDG_Feed(void)
{
	IWDG_ReloadCounter();
}
