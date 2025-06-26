#include "DMA.h"
#include "ADC.h"

__IO uint16_t ADC_ConvertedValue;

/**
 * @brief ����DMA�����USART��ɽ��ܺͷ������ݣ�DMA�����ط����ݸ�����
 * @param  ��
 * @retval  ��
 */
void ADCx_DMA_Configuration(void)
{
	DMA_InitTypeDef ADCx_DMA_Sturcture;
	//����ʱ��
	RCC_AHBPeriphClockCmd(ADCx_DMA_CLK,ENABLE);
	
	//��λDMA
	DMA_DeInit(ADCx_DMA_Channel);
	
	//Ҫ�������������
	ADCx_DMA_Sturcture.DMA_BufferSize=ADCx_DMA_SIZE;
	
	//���ݵĴ��䷽��SRC�Ǵ����赽�ڴ棬DST�Ǵ��ڴ浽����
	ADCx_DMA_Sturcture.DMA_DIR=DMA_DIR_PeripheralSRC;
	
	//�رմӼĴ������Ĵ���ģʽ(ֻ�е����ĴӼĴ������Ĵ�����û���κ����������²ſ�������)
	ADCx_DMA_Sturcture.DMA_M2M=DMA_M2M_Disable;
	
	//Ŀ��˵ĵ�ַ
	ADCx_DMA_Sturcture.DMA_MemoryBaseAddr=(uint32_t)&ADC_ConvertedValue;
	
	//Ŀ��˵�ÿ�����ݵĴ�С
	ADCx_DMA_Sturcture.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
	
	//Ŀ����Ƿ�ÿ����һ���Զ�����"�洢����ַ"
	ADCx_DMA_Sturcture.DMA_MemoryInc=DMA_MemoryInc_Disable;
	
	//DMAģʽѡ��_Normal:һ�δ����ֹͣ;Circular:�Զ�����
	ADCx_DMA_Sturcture.DMA_Mode=DMA_Mode_Circular;
	
	//Դͷ�˵ĵ�ַ��ע�⣬DMA_PeripheralBaseAdd��������uint32_t����&ADCx->DR��ָ��,������Ҫǿת
	ADCx_DMA_Sturcture.DMA_PeripheralBaseAddr=( uint32_t ) ( & ( ADCx->DR ) );
	
	
	//Դͷ�˵�ÿ�����ݵĴ�С
	ADCx_DMA_Sturcture.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
	
	//Դͷ���Ƿ�ÿ����һ���Զ�����"�洢����ַ"
	ADCx_DMA_Sturcture.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	
	//����ͨ�����ȼ��������ͨ��ͬʱ��ռ����ʱ������
	ADCx_DMA_Sturcture.DMA_Priority=DMA_Priority_High;
	
	//��ʼ���ṹ��
	DMA_Init(ADCx_DMA_Channel, &ADCx_DMA_Sturcture);
	
	/// ʹ�� DMA ͨ��
	DMA_Cmd(ADCx_DMA_Channel,ENABLE);
}
