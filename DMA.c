#include "DMA.h"
#include "USART.h"
uint8_t TX_Buffer[TOP_SIZE];
/**
 * @brief 配置DMA，配合USART完成接受和发送数据，DMA用于重发数据给电脑
 * @param  无
 * @retval  无
 */
void USARTx_DMA_Configuration(void)
{
	DMA_InitTypeDef DEBUG_USART_DMA_Sturcture;
	//开启时钟
	RCC_AHBPeriphClockCmd(USART_DMA_CLK,ENABLE);
	//要传输的数据数量
	DEBUG_USART_DMA_Sturcture.DMA_BufferSize=TOP_SIZE;
	//数据的传输方向，SRC是从外设到内存，DST是从内存到外设
	DEBUG_USART_DMA_Sturcture.DMA_DIR=DMA_DIR_PeripheralDST;
	//关闭从寄存器到寄存器模式(只有单纯的从寄存器到寄存器且没有任何外设的情况下才可以启用)
	DEBUG_USART_DMA_Sturcture.DMA_M2M=DMA_M2M_Disable;
	//目标端的地址，注意，DMA_MemoryBaseAddr的类型是uint32_t，而TX-Buffer是指针,所以需要强转
	DEBUG_USART_DMA_Sturcture.DMA_MemoryBaseAddr=(uint32_t)TX_Buffer;
	//目标端的每项数据的大小
	DEBUG_USART_DMA_Sturcture.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	//目标端是否每传输一项自动增加"存储器地址"
	DEBUG_USART_DMA_Sturcture.DMA_MemoryInc=DMA_MemoryInc_Enable;
	//DMA模式选择，_Normal:一次传输就停止;Circular:自动重启
	DEBUG_USART_DMA_Sturcture.DMA_Mode=DMA_Mode_Normal;
	//源头端的地址，注意，DMA_PeripheralBaseAdd的类型是uint32_t，而&USART1->DR是指针,所以需要强转
	DEBUG_USART_DMA_Sturcture.DMA_PeripheralBaseAddr=(uint32_t)&USART1->DR;
	//源头端的每项数据的大小
	DEBUG_USART_DMA_Sturcture.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	//源头端是否每传输一项自动增加"存储器地址"
	DEBUG_USART_DMA_Sturcture.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	//设置通道优先级，当多个通道同时抢占总线时起到作用
	DEBUG_USART_DMA_Sturcture.DMA_Priority=DMA_Priority_High;
	//初始化结构体
	DMA_Init(USART_TX_DMA_CHANNEL, &DEBUG_USART_DMA_Sturcture);
	//开启USARTx与DMA的通道，也就是手册说的“设置USART_CR3寄存器上的DMAT位激活”
	USART_DMACmd(DEBUG_USARTx,USART_DMAReq_Tx, ENABLE);
}
