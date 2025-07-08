#ifndef __IWDG_H
#define __IWDG_H

#include "stm32f10x.h"

#define IWDG_RESET_FLAG_FUN 		    RCC_GetFlagStatus
#define IWDG_RESET_FLAG_CLEAR_FUN   RCC_ClearFlag

void IWDG_Config(u8 prv, u16 rlv);
void IWDG_Feed(void);


#endif
