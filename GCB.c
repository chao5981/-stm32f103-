#include "GCB.h"

/**
 * @brief 初始化控制LED灯的IO口
 * @param 无
 * @retval 无
 */

void LED_GPIO_Init(void)
{
	//初始化结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	//初始化时钟
	RCC_APB2PeriphClockCmd(LED_R_GPIO_RCC|LED_B_GPIO_RCC|LED_G_GPIO_RCC,ENABLE);
//	//选择引脚
//	GPIO_InitStructure.GPIO_Pin=LED_G_GPIO_PIN;
//	//选择推挽输出
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	//选择最高速度
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	//GPIO初始化
//	GPIO_Init(LED_G_GPIO_PORT,&GPIO_InitStructure);
//	//换一个引脚，初始化
//	GPIO_InitStructure.GPIO_Pin=LED_R_GPIO_PIN;
//	GPIO_Init(LED_R_GPIO_PORT,&GPIO_InitStructure);
//	//换一个引脚，初始化
//	GPIO_InitStructure.GPIO_Pin=LED_B_GPIO_PIN;
//	GPIO_Init(LED_B_GPIO_PORT,&GPIO_InitStructure);
/************上面这种写法不行***************/	
	
//	//	//选择引脚
//	GPIO_InitStructure.GPIO_Pin=LED_G_GPIO_PIN;
//	//选择推挽输出
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	//选择最高速度
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	//GPIO初始化
//	GPIO_Init(LED_G_GPIO_PORT,&GPIO_InitStructure);
//	//换一个引脚，初始化
//	GPIO_InitStructure.GPIO_Pin=LED_R_GPIO_PIN;
//	//选择推挽输出
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	//选择最高速度
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(LED_R_GPIO_PORT,&GPIO_InitStructure);
//	//换一个引脚，初始化
//	GPIO_InitStructure.GPIO_Pin=LED_B_GPIO_PIN;
//	//选择推挽输出
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
//	//选择最高速度
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(LED_B_GPIO_PORT,&GPIO_InitStructure);
/************这种写法可以，因为可能是mode和speed被意外修改***************/	

	  //选择引脚
		GPIO_InitStructure.GPIO_Pin = LED_G_GPIO_PIN | LED_R_GPIO_PIN | LED_B_GPIO_PIN;
	  //选择推挽输出
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		//选择最高速度
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		//GPIO初始化
		GPIO_Init(GPIOB, &GPIO_InitStructure);
/************这种写法极力推荐，简介***************/		
		
	/* 关闭所有led灯	*/
	GPIO_SetBits(LED_G_GPIO_PORT, LED_G_GPIO_PIN);
		
	/* 关闭所有led灯	*/
	GPIO_SetBits(LED_R_GPIO_PORT, LED_R_GPIO_PIN);	 
    
  /* 关闭所有led灯	*/
	GPIO_SetBits(LED_B_GPIO_PORT, LED_B_GPIO_PIN);
}
