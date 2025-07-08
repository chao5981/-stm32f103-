#include "USART.h"
#include "tpad.h"
#include "systick.h"
#include "GCB.h"
#include "beep.h"
#include "TIMbase.h"

#define LONG_PRESS_THRESHOLD 200

//记录时长
extern u16 tpad_span_stage;

//是否被按下的一个标值，方便完成抬手单点操作
u8 isPressed;


/**
 * @brief 电容按键
 * @param 无
 * @retval 无
 */
int main(void)
{
		//电容按键状态
		uint8_t tpadStatus=0;
		//记录按键按下的时长
		u16 pressStartTime=0,currentTime=0;
	
		LED_GPIO_Init();
		TIM_Base_Init();
	  USART_Configuration();
		Beep_Init();
		TPAD_Init();
		printf("\r\n这是一个电容按键的实验\r\n");

		while(1)
    {
        tpadStatus = TPAD_Scan();
			
			//如果被按下
        if (tpadStatus == TPAD_ON)
        {
					//isPressed置1，并开始记录时长
            if (!isPressed)
            {
               isPressed = 1;
							 pressStartTime = tpad_span_stage;
            }
            else
            {
                currentTime = tpad_span_stage;
							//在循环中如果时长超过预定值，则认为是长按
                if (currentTime - pressStartTime >= LONG_PRESS_THRESHOLD)
                {
                    LED1_ON;
                    SysTick_NO_EXTI_Delay_1ms(25);
                    LED1_OFF;
                    SysTick_NO_EXTI_Delay_1ms(25);
                }
            }
        }
				//如果不按了
        else
        {
					//且之前按下过，则认为是抬手了
            if (isPressed)
            {
                isPressed = 0;
                currentTime = tpad_span_stage;
                if (currentTime - pressStartTime < LONG_PRESS_THRESHOLD)
                {
                    BEEP_ON();
                    SysTick_NO_EXTI_Delay_1ms(25);
                    BEEP_OFF();
										tpad_span_stage=0;
                }
            }
        }
    }
}



