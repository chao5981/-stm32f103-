#include "USART.h"
#include "key.h"
#include "GCB.h"

static void Delay(__IO u32 nCount);
static uint8_t KEY2_LongPress(void);

int main(void)
{
	
	/* 使能电源管理单元的时钟,必须要使能时钟才能进入待机模式 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);
	
	
	KEY_GPIO_Config();
	LED_GPIO_Init();
	USART_Configuration();
	
	printf("\r\n ***************** \r\n");
  printf("\r\n 待机模式例程\r\n");
	
	printf("\r\n 实验说明：\r\n");

	printf("\r\n 1.本程序中，绿灯表示本次复位是上电或引脚复位，红灯表示即将进入待机状态，蓝灯表示本次是待机唤醒的复位\r\n");
	printf("\r\n 2.长按KEY2按键后，会进入待机模式\r\n");
	printf("\r\n 3.在待机模式下，按KEY1按键可唤醒，唤醒后系统会进行复位，程序从头开始执行\r\n");
	printf("\r\n 4.可通过检测WU标志位确定复位来源\r\n");
	
	printf("\r\n 5.在待机状态下，DAP下载器无法给STM32下载程序，需要唤醒后才能下载");
	
	if(PWR_GetFlagStatus(PWR_FLAG_WU)!=RESET)
	{
		LED_BLUE;
		printf("\r\n 待机唤醒复位 \r\n");
	}
	else
	{
		LED_GREEN;
		printf("\r\n 非待机唤醒复位 \r\n");
	}
	
	while(1)
	{
		// K2 按键长按进入待机模式
		if(KEY2_LongPress())
		{
			
			printf("\r\n 即将进入待机模式，进入待机模式后可按KEY1唤醒，唤醒后会进行复位，程序从头开始执行\r\n");
			LED_RED;	
			Delay(0xFFFFFF);
			
			/*清除WU状态位*/
			PWR_ClearFlag (PWR_FLAG_WU);
			
			//开启WKUP唤醒引脚
			 PWR_WakeUpPinCmd(ENABLE);
			
			//进入待机模式
			PWR_EnterSTANDBYMode();
			
		}
	}
	
	
}

static void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}



 /**
  * @brief  用于检测按键是否被长时间按下
  * @param  无
  * @retval 1 ：按键被长时间按下  0 ：按键没有被长时间按下
  */
static uint8_t KEY2_LongPress(void)
{			
	uint8_t downCnt =0;																				//记录按下的次数
	uint8_t upCnt =0;																					//记录松开的次数			

	while(1)																										//死循环，由return结束
	{	
		Delay(0x2FFFF);																					//延迟一段时间再检测

		if(GPIO_ReadInputDataBit ( KEY2_GPIO_PORT, KEY2_GPIO_PIN ) == SET)	//检测到按下按键
		{
			downCnt++;																					//记录按下次数
			upCnt=0;																						//清除按键释放记录

			if(downCnt>=100)																		//按下时间足够
			{
				return 1; 																				//检测到按键被时间长按下
			}
		}
		else 
		{
			upCnt++; 																						//记录释放次数
			if(upCnt>5)																					//连续检测到释放超过5次
			{
				return 0;																				//按下时间太短，不是按键长按操作
			}
		}//	if(GPIO_ReadInputDataBit 
	}//while
}
/*********************************************END OF FILE**********************/


