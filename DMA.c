#include "DMA.h"
#include "USART.h"
uint8_t TX_Buffer[TOP_SIZE];
/**
 * @brief ����DMA�����USART��ɽ��ܺͷ������ݣ�DMA�����ط����ݸ�����
 * @param  ��
 * @retval  ��
 */
void USARTx_DMA_Configuration(void)
{
	DMA_InitTypeDef DEBUG_USART_DMA_Sturcture;
	//����ʱ��
	RCC_AHBPeriphClockCmd(USART_DMA_CLK,ENABLE);
	//Ҫ�������������
	DEBUG_USART_DMA_Sturcture.DMA_BufferSize=TOP_SIZE;
	//���ݵĴ��䷽��SRC�Ǵ����赽�ڴ棬DST�Ǵ��ڴ浽����
	DEBUG_USART_DMA_Sturcture.DMA_DIR=DMA_DIR_PeripheralDST;
	//�رմӼĴ������Ĵ���ģʽ(ֻ�е����ĴӼĴ������Ĵ�����û���κ����������²ſ�������)
	DEBUG_USART_DMA_Sturcture.DMA_M2M=DMA_M2M_Disable;
	//Ŀ��˵ĵ�ַ��ע�⣬DMA_MemoryBaseAddr��������uint32_t����TX-Buffer��ָ��,������Ҫǿת
	DEBUG_USART_DMA_Sturcture.DMA_MemoryBaseAddr=(uint32_t)TX_Buffer;
	//Ŀ��˵�ÿ�����ݵĴ�С
	DEBUG_USART_DMA_Sturcture.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	//Ŀ����Ƿ�ÿ����һ���Զ�����"�洢����ַ"
	DEBUG_USART_DMA_Sturcture.DMA_MemoryInc=DMA_MemoryInc_Enable;
	//DMAģʽѡ��_Normal:һ�δ����ֹͣ;Circular:�Զ�����
	DEBUG_USART_DMA_Sturcture.DMA_Mode=DMA_Mode_Normal;
	//Դͷ�˵ĵ�ַ��ע�⣬DMA_PeripheralBaseAdd��������uint32_t����&USART1->DR��ָ��,������Ҫǿת
	DEBUG_USART_DMA_Sturcture.DMA_PeripheralBaseAddr=(uint32_t)&USART1->DR;
	//Դͷ�˵�ÿ�����ݵĴ�С
	DEBUG_USART_DMA_Sturcture.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	//Դͷ���Ƿ�ÿ����һ���Զ�����"�洢����ַ"
	DEBUG_USART_DMA_Sturcture.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	//����ͨ�����ȼ��������ͨ��ͬʱ��ռ����ʱ������
	DEBUG_USART_DMA_Sturcture.DMA_Priority=DMA_Priority_High;
	//��ʼ���ṹ��
	DMA_Init(USART_TX_DMA_CHANNEL, &DEBUG_USART_DMA_Sturcture);
	//����USARTx��DMA��ͨ����Ҳ�����ֲ�˵�ġ�����USART_CR3�Ĵ����ϵ�DMATλ���
	USART_DMACmd(DEBUG_USARTx,USART_DMAReq_Tx, ENABLE);
}
