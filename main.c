#include "USART.h"
#include "tpad.h"
#include "systick.h"
#include "GCB.h"
#include "beep.h"
#include "TIMbase.h"

#define LONG_PRESS_THRESHOLD 200

//��¼ʱ��
extern u16 tpad_span_stage;

//�Ƿ񱻰��µ�һ����ֵ���������̧�ֵ������
u8 isPressed;


/**
 * @brief ���ݰ���
 * @param ��
 * @retval ��
 */
int main(void)
{
		//���ݰ���״̬
		uint8_t tpadStatus=0;
		//��¼�������µ�ʱ��
		u16 pressStartTime=0,currentTime=0;
	
		LED_GPIO_Init();
		TIM_Base_Init();
	  USART_Configuration();
		Beep_Init();
		TPAD_Init();
		printf("\r\n����һ�����ݰ�����ʵ��\r\n");

		while(1)
    {
        tpadStatus = TPAD_Scan();
			
			//���������
        if (tpadStatus == TPAD_ON)
        {
					//isPressed��1������ʼ��¼ʱ��
            if (!isPressed)
            {
               isPressed = 1;
							 pressStartTime = tpad_span_stage;
            }
            else
            {
                currentTime = tpad_span_stage;
							//��ѭ�������ʱ������Ԥ��ֵ������Ϊ�ǳ���
                if (currentTime - pressStartTime >= LONG_PRESS_THRESHOLD)
                {
                    LED1_ON;
                    SysTick_NO_EXTI_Delay_1ms(25);
                    LED1_OFF;
                    SysTick_NO_EXTI_Delay_1ms(25);
                }
            }
        }
				//���������
        else
        {
					//��֮ǰ���¹�������Ϊ��̧����
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



