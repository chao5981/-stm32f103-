#include "clock.h"
#include "stm32f10x_rcc.h"

void HSE_SetSysClock(uint32_t pllmul)
{
	__IO uint32_t HSEStartUpStatus = 0;
	//把RCC初始化为复位状态
	RCC_DeInit();
	//使能HSE
	RCC_HSEConfig(RCC_HSE_ON);
	HSEStartUpStatus=RCC_WaitForHSEStartUp();
	//等待HSE稳定，稳定后才继续进行
	if( HSEStartUpStatus==SUCCESS)
	{
		//----------------------------------------------------------------------//
		// 这两句是操作FLASH闪存用到的，如果不操作FLASH的话，这两个注释掉也没影响
    // 使能FLASH 预存取缓冲区
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    // SYSCLK周期与闪存访问时间的比例设置，这里统一设置成2
		// 设置成2的时候，SYSCLK低于48M也可以工作，如果设置成0或者1的时候，
		// 如果配置的SYSCLK超出了范围的话，则会进入硬件错误，程序就死了
		// 0：0 < SYSCLK <= 24M
		// 1：24< SYSCLK <= 48M
		// 2：48< SYSCLK <= 72M
    FLASH_SetLatency(FLASH_Latency_2);
		//----------------------------------------------------------------------//
		//*******预处理，配置放大或者缩小系数********//
		//将HCLK设置为1分频
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		//将APB2设置为1分频
		RCC_PCLK2Config (RCC_HCLK_Div1);
		//将PLLMUL设置为1分频，放大倍数看主函数
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,pllmul);
		
		//使能RLL
		RCC_PLLCmd(ENABLE);
		//等待RCC稳定
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET)
		{
		}
		//稳定后SW，把PLLCLK设置为系统时钟SYSCLK
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		//读取时钟切换位，确保PLLCLK被设置为系统时钟
		//备注：SW可能的值：0x00：HSI被设定为系统时钟，0x04：HSE被设置为系统时钟，0x08：PLL被设置为系统时钟；0x13未使用
		while (RCC_GetSYSCLKSource() != 0x08)
    {
    }
	}
	else
	{
		//如果HSE不稳定，	程序来到这里，可进行调试
		//如果HSE开启故障或者失败，那系统会把HSI设置为系统时钟
		while (1)
    {
    }
	}
}

void HSI_SYSClock(uint32_t pllmul)
{
	//把RCC初始化为复位状态
	RCC_DeInit();
	//使能HSI
	RCC_HSICmd(ENABLE);
	if(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) != RESET)
	{
		//----------------------------------------------------------------------//
		// 这两句是操作FLASH闪存用到的，如果不操作FLASH的话，这两个注释掉也没影响
    // 使能FLASH 预存取缓冲区
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    // SYSCLK周期与闪存访问时间的比例设置，这里统一设置成2
		// 设置成2的时候，SYSCLK低于48M也可以工作，如果设置成0或者1的时候，
		// 如果配置的SYSCLK超出了范围的话，则会进入硬件错误，程序就死了
		// 0：0 < SYSCLK <= 24M
		// 1：24< SYSCLK <= 48M
		// 2：48< SYSCLK <= 72M
    FLASH_SetLatency(FLASH_Latency_2);
		//----------------------------------------------------------------------//
		//*******预处理，配置放大或者缩小系数********//
		//将HCLK设置为1分频
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		//将APB2设置为1分频
		RCC_PCLK2Config (RCC_HCLK_Div1);
		//将PLLMUL设置为1分频，放大倍数看主函数
		RCC_PLLConfig(RCC_PLLSource_HSI_Div2,pllmul);
		//使能PLL
	  RCC_PLLCmd(ENABLE);
		//等待PLL稳定
	  while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET)
		{
		}
		//稳定后SW系统时钟为PLLCLK
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		//读取时钟位状态，确定选择的位PLLCLK
		while(RCC_GetSYSCLKSource()!=0x08)
		{
		}
	}
	else
	{
		// 如果HSI开启失败，那么程序就会来到这里，用户可在这里添加出错的代码处理
		// 当HSE开启失败或者故障的时候，单片机会自动把HSI设置为系统时钟，HSI是内部的高速时钟，8MHZ
		while(1)
		{
		}
	}
}
