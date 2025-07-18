#include "key.h"
/**
 * @brief 按键初始化
 * @param 无
 * @retval 无
 */
 void KEY_GPIO_Config(void)
 {
	 //初始化一个结构体
	  GPIO_InitTypeDef KeyInit;
	 //设置时钟
	  RCC_APB2PeriphClockCmd(/*RCC_APB2Periph_GPIOA|*/ RCC_APB2Periph_GPIOC,ENABLE);
//	 //配置KEY1
//	  KeyInit.GPIO_Pin=GPIO_Pin_0;
//	 //选择浮空输入
//	  KeyInit.GPIO_Mode=GPIO_Mode_IN_FLOATING;
//	 //选择速度
//	  KeyInit.GPIO_Speed=GPIO_Speed_50MHz;
//	 //初始化IO口
//	  GPIO_Init(KEY1_GPIO_PORT,&KeyInit);
	 //配置KEY2
	  KeyInit.GPIO_Pin=GPIO_Pin_13;
	 //选择浮空输入
	  KeyInit.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	 //选择速度
	  KeyInit.GPIO_Speed=GPIO_Speed_50MHz;
	 //初始化IO口
	  GPIO_Init(KEY2_GPIO_PORT,&KeyInit);
 }
 /**
 * @brief 检测是否有摁键按下
 * @param GPIO的类型，GPIO的引脚
 * @retval 返回是否摁下(KEY_ON/KEY_OFF)
 */
 uint8_t KEY_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
 {
	 if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==KEY_ON)
	 {
		 while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==KEY_ON);
		 return KEY_ON;
	 }
	 else
	 {
			return KEY_OFF;
	 }
}

