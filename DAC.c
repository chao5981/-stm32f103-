#include "DAC.h"
#include "TIMbase.h"

/* �������� ---------------------------------------------------------*/
//��Ϊ��ش洢���ݵļĴ���ֻ��12λ���������ֵֻ����4095������ͻ���ֺ�ʵ�ʲ���������
const uint16_t Sine12bit[POINT_NUM] = {
	2048	, 2460	, 2856	, 3218	, 3532	, 3786	, 3969	, 4072	,
	4093	, 4031	, 3887	, 3668	, 3382	, 3042	,	2661	, 2255	, 
	1841	, 1435	, 1054	, 714		, 428		, 209		, 65		, 3			,
	24		, 127		, 310		, 564		, 878		, 1240	, 1636	, 2048
};

uint32_t DualSine12bit[POINT_NUM];

void DAC_GPIO_Config(void)
{
	GPIO_InitTypeDef DAC_GPIO_Structure;
	
	//����ʱ��
	DAC_GPIO_CLK_FUN(DAC_GPIO_CLK,ENABLE);
	//����GPIO�ṹ�壬������ָ������PA5��SPI-FLASH���ã���ֹ�������������ɴ�ֱ�Ӳ���
	DAC_GPIO_Structure.GPIO_Mode=GPIO_Mode_AIN;
	DAC_GPIO_Structure.GPIO_Pin=DAC1_GPIO_Pin;
	DAC_GPIO_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	
	//��ʼ���ṹ��
	GPIO_Init(DAC_GPIO_PONT,&DAC_GPIO_Structure);
}

void DAC_Config(void)
{
	DAC_InitTypeDef DAC_Structure;
	
	//����ʱ��
	DAC_CLK_FUN(DAC_CLK,ENABLE);
	
	//����ADC�ṹ��
	
	//�����������ɵ������Ҳ��������Ա������Ӳ���������ǲ�������������
	//DAC_Structure.DAC_LFSRUnmask_TriangleAmplitude
	
	//��ʹ��DAC�������
	DAC_Structure.DAC_OutputBuffer=DAC_OutputBuffer_Disable;
	
	//ѡ��TIM6��Ϊ����Դ
	DAC_Structure.DAC_Trigger=DAC_Trigger_T6_TRGO;
	
	//�����ò��η�����
	DAC_Structure.DAC_WaveGeneration=DAC_WaveGeneration_None;
	
	//��ʼ���ṹ��
	DAC_Init(DAC_Channel_1,&DAC_Structure);
	
	//ʹ��DMA����
	DAC_DMACmd(DAC_Channel_1,ENABLE);
	
	//ʹ��DACͨ��1
	DAC_Cmd(DAC_Channel_1,ENABLE);
	
}

void DAC_DMA_Config(void)
{
	DMA_InitTypeDef DAC_DMA_Config;
	
	//����DMAʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
	
	//���ݵĸ���
	DAC_DMA_Config.DMA_BufferSize=POINT_NUM;
	DAC_DMA_Config.DMA_DIR=DMA_DIR_PeripheralDST;
	DAC_DMA_Config.DMA_M2M= DMA_M2M_Disable;
	DAC_DMA_Config.DMA_MemoryBaseAddr=(u32)DualSine12bit;
	DAC_DMA_Config.DMA_MemoryDataSize=DMA_MemoryDataSize_Word;
	DAC_DMA_Config.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DAC_DMA_Config.DMA_Mode=DMA_Mode_Circular;
	DAC_DMA_Config.DMA_PeripheralBaseAddr=DAC_DHR12RD_ADDRESS;
	DAC_DMA_Config.DMA_PeripheralDataSize=DMA_MemoryDataSize_Word;
	DAC_DMA_Config.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	DAC_DMA_Config.DMA_Priority=DMA_Priority_High;
	
	
	DMA_Init(DMA2_Channel4,&DAC_DMA_Config);
	
	DMA_Cmd(DMA2_Channel4,ENABLE);
	
}


void DAC_Mode_Config(void)
{
	uint32_t Idx = 0;
	/* ������Ҳ������ݣ�˫ͨ���Ҷ���*/
  for (Idx = 0; Idx < POINT_NUM; Idx++)
  {
    DualSine12bit[Idx] = (Sine12bit[Idx] << 16) + (Sine12bit[Idx]);
  }
	
	DAC_GPIO_Config();
	TIM_base_Config();
	DAC_Config();
	DAC_DMA_Config();
}
