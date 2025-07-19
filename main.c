#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "bsp_ili9341_lcd.h"
#include "USART.h"
#include "bsp_rtc.h"
#include "Key.h"


struct rtc_time systmtime=
{
0,0,0,1,1,2000,0
};

extern __IO uint32_t TimeDisplay ;



//【*】注意事项：
//在bsp_rtc.h文件中：

//1.可设置宏USE_LCD_DISPLAY控制是否使用LCD显示
//2.可设置宏RTC_CLOCK_SOURCE_LSI和RTC_CLOCK_SOURCE_LSE控制使用LSE晶振还是LSI晶振

//3.STM32的LSE晶振要求非常严格，同样的电路、板子批量产品时总有些会出现问题。
//  本实验中默认使用LSI晶振。
//  
//4.！！！若希望RTC在主电源掉电后仍然运行，需要给开发板的电池槽安装钮扣电池，
//  ！！！且改成使用外部晶振模式RTC_CLOCK_SOURCE_LSE
//  钮扣电池型号：CR1220

int main(void)
{
	struct rtc_time User_Set_Time;
	
//可使用该宏设置是否使用液晶显示
	
//#ifdef和#ifndef不一样
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
		/* 每过1s 更新一次时间*/
	    if (TimeDisplay == 1)
	    {
				/* 当前时间 */
	      Time_Display( RTC_GetCounter(),&systmtime); 		  
	      TimeDisplay = 0;
	    }
		
			if(KEY_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN)==KEY_ON)
			{
				
				Time_Regulate_Get(&User_Set_Time);
				Time_Adjust(&User_Set_Time);
				
				//向备份寄存器写入标志.下次再次启动时能正确判断已经RTC已经完成配置
				BKP_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
			}
	}
	
}


