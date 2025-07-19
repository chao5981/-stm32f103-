#include "stm32f10x.h"   // �൱��51��Ƭ���е�  #include <reg51.h>
#include "bsp_ili9341_lcd.h"
#include "USART.h"
#include "bsp_rtc.h"
#include "Key.h"


struct rtc_time systmtime=
{
0,0,0,1,1,2000,0
};

extern __IO uint32_t TimeDisplay ;



//��*��ע�����
//��bsp_rtc.h�ļ��У�

//1.�����ú�USE_LCD_DISPLAY�����Ƿ�ʹ��LCD��ʾ
//2.�����ú�RTC_CLOCK_SOURCE_LSI��RTC_CLOCK_SOURCE_LSE����ʹ��LSE������LSI����

//3.STM32��LSE����Ҫ��ǳ��ϸ�ͬ���ĵ�·������������Ʒʱ����Щ��������⡣
//  ��ʵ����Ĭ��ʹ��LSI����
//  
//4.��������ϣ��RTC������Դ�������Ȼ���У���Ҫ��������ĵ�ز۰�װť�۵�أ�
//  �������Ҹĳ�ʹ���ⲿ����ģʽRTC_CLOCK_SOURCE_LSE
//  ť�۵���ͺţ�CR1220

int main(void)
{
	struct rtc_time User_Set_Time;
	
//��ʹ�øú������Ƿ�ʹ��Һ����ʾ
	
//#ifdef��#ifndef��һ��
#ifdef USE_LCD_DISPLAY
	
		ILI9341_Init();
		LCD_SetFont(&Font8x16);
		LCD_SetColors(WHITE,BLACK);
		ILI9341_Clear(0,0,ILI9341_LESS_PIXEL,ILI9341_MORE_PIXEL);
		ILI9341_DispStringLine_EN(LINE(0),"        BH RTC demo");
	
#endif
	
	USART_Configuration();
	KEY_GPIO_Config();
	RTC_NVIC_Config();
	RTC_CheckAndConfig(&systmtime);

	while(1)
	{
		/* ÿ��1s ����һ��ʱ��*/
	    if (TimeDisplay == 1)
	    {
				/* ��ǰʱ�� */
	      Time_Display( RTC_GetCounter(),&systmtime); 		  
	      TimeDisplay = 0;
	    }
		
			if(KEY_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN)==KEY_ON)
			{
				
				Time_Regulate_Get(&User_Set_Time);
				Time_Adjust(&User_Set_Time);
				
				//�򱸷ݼĴ���д���־.�´��ٴ�����ʱ����ȷ�ж��Ѿ�RTC�Ѿ��������
				BKP_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
			}
	}
	
}


