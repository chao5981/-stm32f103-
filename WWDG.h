#ifndef __WWDG_H
#define __WWDG_H

#include "stm32f10x.h"

/***********º¯Êý************/
#define WWDG_CLK_FUN   RCC_APB1PeriphClockCmd
#define WWDG_CLK			 RCC_APB1Periph_WWDG
#define WWDG_Handler   WWDG_IRQHandler


/**********²ÎÊý**********/
#define WWDG_CNT 0x7f

void WWDG_Config(u8 ctv,u32 prv,u8 wdv);
void WWDG_Feed(void);

#endif /*__WWDG_H*/
