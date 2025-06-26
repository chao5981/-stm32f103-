#include "DMA.h"
#include "ADC.h"

void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_ADC_Structure;
	
	//��ʼ��ʱ��
	ADC_GPIO_APBxClock_FUN(ADC_GPIO_CLK,ENABLE);
	
	//����GPIO�ṹ��
	GPIO_ADC_Structure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_ADC_Structure.GPIO_Pin=ADC_PIN;
	GPIO_Init(ADC_PORT,&GPIO_ADC_Structure);
}

void ADCx_Config(void)
{
	ADC_InitTypeDef ADC_Structure;
	
	//��ʼ��ʱ��
	ADC_APBxClock_FUN(ADC_CLK,ENABLE);
	
	//����ADC�ṹ��
	
	// ����ת��ģʽ
	ADC_Structure.ADC_ContinuousConvMode=ENABLE;
	
	// ת������Ҷ���
	ADC_Structure.ADC_DataAlign=ADC_DataAlign_Right;
	
	// �����ⲿ����ת���������������
	ADC_Structure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	
	// ֻʹ��һ��ADC�����ڵ�ģʽ
	ADC_Structure.ADC_Mode=ADC_Mode_Independent;
	
	// ת��ͨ��1��
	ADC_Structure.ADC_NbrOfChannel=1;
	
	// ��ֹɨ��ģʽ����ͨ����Ҫ����ͨ������Ҫ
	ADC_Structure.ADC_ScanConvMode=DISABLE;
	
	//��ʼ��ADC
	ADC_Init(ADCx,&ADC_Structure);
	
	//����ADCʱ�ӷ�Ƶ�����֧��14HZ��ʱ��72HZ������Ϊ8��Ƶ����9HZ
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	
	//���ù�����˳��
	ADC_RegularChannelConfig(ADCx,ADC_CHANNEL,1,ADC_SampleTime_55Cycles5);
	
	
	//ʹ��ADC
	ADC_Cmd(ADCx,ENABLE);
	
	//��ʼ��ADC��У׼�Ĵ���
	ADC_ResetCalibration(ADCx);
	while(ADC_GetResetCalibrationStatus(ADCx));
	
	//ADCУ׼
	ADC_StartCalibration(ADCx);
	while(ADC_GetCalibrationStatus(ADCx));
	
	
	/*������ADCУ׼������ת��ǰ����DMA*/
	
	//����DMAͨ����
	ADC_DMACmd(ADCx, ENABLE);
	
	// �����������ADCת��
	ADC_SoftwareStartConvCmd(ADCx, ENABLE);
}


void ADCx_Init()
{
	ADCx_GPIO_Config();
	ADCx_Config();
	ADCx_DMA_Configuration();
}
