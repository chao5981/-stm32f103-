#include "DMA.h"
#include "ADC.h"

void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_ADC_Structure;
	
	//��ʼ��ʱ��
	ADCx_1_GPIO_APBxClock_FUN(ADCx_1_GPIO_CLK,ENABLE);
	ADCx_2_GPIO_APBxClock_FUN(ADCx_2_GPIO_CLK,ENABLE);
	
	//����GPIO�ṹ��
	GPIO_ADC_Structure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_ADC_Structure.GPIO_Pin=ADCx_1_PIN;
	GPIO_Init(ADCx_1_PORT,&GPIO_ADC_Structure);
	
	GPIO_ADC_Structure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_ADC_Structure.GPIO_Pin=ADCx_2_PIN;
	GPIO_Init(ADCx_2_PORT,&GPIO_ADC_Structure);
}

void ADCx_Config(void)
{
	ADC_InitTypeDef ADC_Structure;
	
	//��ʼ��ʱ��
	ADCx_1_APBxClock_FUN(ADCx_1_CLK,ENABLE);
	ADCx_2_APBxClock_FUN(ADCx_2_CLK,ENABLE);
	
	//����ADC1�ṹ��
	
	// ģʽ����Ϊͬ������ģʽ
	ADC_Structure.ADC_Mode=ADC_Mode_RegSimult;
	
	// ����ת��ģʽ
	ADC_Structure.ADC_ContinuousConvMode=ENABLE;
	
	// ת������Ҷ���
	ADC_Structure.ADC_DataAlign=ADC_DataAlign_Right;
	
	// �����ⲿ����ת���������������
	ADC_Structure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	
	// ת��ͨ��1��
	ADC_Structure.ADC_NbrOfChannel=NOFCHANEL;
	
	// ��ֹɨ��ģʽ����ͨ����Ҫ����ͨ������Ҫ
	ADC_Structure.ADC_ScanConvMode=ENABLE;
	
	//��ʼ��ADC
	ADC_Init(ADCx_1,&ADC_Structure);
	
	//����ADCʱ�ӷ�Ƶ�����֧��14HZ��ʱ��72HZ������Ϊ8��Ƶ����9HZ
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	
	//���ù�����˳��
	ADC_RegularChannelConfig(ADCx_1,ADCx_1_CHANNEL,1,ADC_SampleTime_239Cycles5);
	
	
	//����ADC2�ṹ��
	
	// ģʽ����Ϊͬ������ģʽ
	ADC_Structure.ADC_Mode=ADC_Mode_RegSimult;
	
	// ����ת��ģʽ
	ADC_Structure.ADC_ContinuousConvMode=ENABLE;
	
	// ת������Ҷ���
	ADC_Structure.ADC_DataAlign=ADC_DataAlign_Right;
	
	// �����ⲿ����ת���������������
	ADC_Structure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	
	// ת��ͨ��1��
	ADC_Structure.ADC_NbrOfChannel=NOFCHANEL;
	
	// ��ֹɨ��ģʽ����ͨ����Ҫ����ͨ������Ҫ
	ADC_Structure.ADC_ScanConvMode=ENABLE;
	
	//��ʼ��ADC
	ADC_Init(ADCx_2,&ADC_Structure);
	
	//����ADCʱ�ӷ�Ƶ�����֧��14HZ��ʱ��72HZ������Ϊ8��Ƶ����9HZ
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	
	//���ù�����˳��
	ADC_RegularChannelConfig(ADCx_2,ADCx_2_CHANNEL,1,ADC_SampleTime_239Cycles5);
	
	
	//ʹ��ADC1
	ADC_Cmd(ADCx_1,ENABLE);
	
	//ʹ��ADC2
	ADC_Cmd(ADCx_2,ENABLE);
	
	//��ʼ��ADC1��У׼�Ĵ���
	ADC_ResetCalibration(ADCx_1);
	while(ADC_GetResetCalibrationStatus(ADCx_1));
	
	//ADC1У׼
	ADC_StartCalibration(ADCx_1);
	while(ADC_GetCalibrationStatus(ADCx_1));
	
	//��ʼ��ADC2��У׼�Ĵ���
	ADC_ResetCalibration(ADCx_2);
	while(ADC_GetResetCalibrationStatus(ADCx_2));
	
	//ADC2У׼
	ADC_StartCalibration(ADCx_2);
	while(ADC_GetCalibrationStatus(ADCx_2));
	
	
	/*������ADCУ׼������ת��ǰ����DMA*/
	
	//����ADC1��DMAͨ����
	ADC_DMACmd(ADCx_1, ENABLE);
	
	//����ADC2��DMAͨ����
	ADC_DMACmd(ADCx_2, ENABLE);
	
	
	//ADC2һ��Ҫ��ADC1֮ǰ����
	
	//����Ӳ������ADC2ת��
	ADC_ExternalTrigConvCmd(ADCx_2,ENABLE);
	
	// �����������ADC1ת��
	ADC_SoftwareStartConvCmd(ADCx_1, ENABLE);
	
}


void ADCx_Init()
{
	ADCx_GPIO_Config();
	ADCx_Config();
	ADCx_DMA_Configuration();
}
