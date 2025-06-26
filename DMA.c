#include "DMA.h"
#include "ADC.h"

__IO uint16_t ADC_ConvertedValue;

/**
 * @brief 配置DMA，配合USART完成接受和发送数据，DMA用于重发数据给电脑
 * @param  无
 * @retval  无
 */
void ADCx_DMA_Configuration(void)
{
	DMA_InitTypeDef ADCx_DMA_Sturcture;
	//开启时钟
	RCC_AHBPeriphClockCmd(ADCx_DMA_CLK,ENABLE);
	
	//复位DMA
	DMA_DeInit(ADCx_DMA_Channel);
	
	//要传输的数据数量
	ADCx_DMA_Sturcture.DMA_BufferSize=ADCx_DMA_SIZE;
	
	//数据的传输方向，SRC是从外设到内存，DST是从内存到外设
	ADCx_DMA_Sturcture.DMA_DIR=DMA_DIR_PeripheralSRC;
	
	//关闭从寄存器到寄存器模式(只有单纯的从寄存器到寄存器且没有任何外设的情况下才可以启用)
	ADCx_DMA_Sturcture.DMA_M2M=DMA_M2M_Disable;
	
	//目标端的地址
	ADCx_DMA_Sturcture.DMA_MemoryBaseAddr=(uint32_t)&ADC_ConvertedValue;
	
	//目标端的每项数据的大小
	ADCx_DMA_Sturcture.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
	
	//目标端是否每传输一项自动增加"存储器地址"
	ADCx_DMA_Sturcture.DMA_MemoryInc=DMA_MemoryInc_Disable;
	
	//DMA模式选择，_Normal:一次传输就停止;Circular:自动重启
	ADCx_DMA_Sturcture.DMA_Mode=DMA_Mode_Circular;
	
	//源头端的地址，注意，DMA_PeripheralBaseAdd的类型是uint32_t，而&ADCx->DR是指针,所以需要强转
	ADCx_DMA_Sturcture.DMA_PeripheralBaseAddr=( uint32_t ) ( & ( ADCx->DR ) );
	
	
	//源头端的每项数据的大小
	ADCx_DMA_Sturcture.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
	
	//源头端是否每传输一项自动增加"存储器地址"
	ADCx_DMA_Sturcture.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	
	//设置通道优先级，当多个通道同时抢占总线时起到作用
	ADCx_DMA_Sturcture.DMA_Priority=DMA_Priority_High;
	
	//初始化结构体
	DMA_Init(ADCx_DMA_Channel, &ADCx_DMA_Sturcture);
	
	/// 使能 DMA 通道
	DMA_Cmd(ADCx_DMA_Channel,ENABLE);
}
