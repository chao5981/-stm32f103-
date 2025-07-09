#include "WWDG.h"

void WWDG_NVIC_Config(void)
{
	NVIC_InitTypeDef WWDG_NVIC_Structure;
	
	//1分组，因为程序跑飞必须在最后一刻进入中断保存重要数据
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	WWDG_NVIC_Structure.NVIC_IRQChannel = WWDG_IRQn;
  WWDG_NVIC_Structure.NVIC_IRQChannelPreemptionPriority = 0;
  WWDG_NVIC_Structure.NVIC_IRQChannelSubPriority = 0;
  WWDG_NVIC_Structure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&WWDG_NVIC_Structure);
}


/**
 * @brief 配置WWDG
 * @param ctv:重新加载计数器的值(喂狗)；prv:设置预分频器的值;wdv:WWDG窗口上限值
 * @retval 无
 */
void WWDG_Config(u8 ctv,u32 prv,u8 wdv)
{
	//开启时钟
	WWDG_CLK_FUN(WWDG_CLK,ENABLE);
	
	//重新加载计数器的值(喂狗)
	WWDG_SetCounter(ctv);
	
	//设置预分频器的值
	WWDG_SetPrescaler(prv);
	
	//设置窗口上限值
	WWDG_SetWindowValue(wdv);
	
	//使能WWDG
	WWDG_Enable(WWDG_CNT);
	
	// 清除提前唤醒中断标志位
	WWDG_ClearFlag();	
	
	//配置NVIC
	WWDG_NVIC_Config();
	
	//开启EWI中断
	WWDG_EnableIT();
	
	
}

void WWDG_Feed(void)
{
	WWDG_SetCounter(WWDG_CNT);
}
