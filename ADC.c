#include "DMA.h"
#include "ADC.h"

void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_ADC_Structure;
	
	//初始化时钟
	ADC_GPIO_APBxClock_FUN(ADC_GPIO_CLK,ENABLE);
	
	//配置GPIO结构体
	GPIO_ADC_Structure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_ADC_Structure.GPIO_Pin=ADC_PIN1;
	GPIO_Init(ADC_PORT,&GPIO_ADC_Structure);
	
	GPIO_ADC_Structure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_ADC_Structure.GPIO_Pin=ADC_PIN2;
	GPIO_Init(ADC_PORT,&GPIO_ADC_Structure);
	
	GPIO_ADC_Structure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_ADC_Structure.GPIO_Pin=ADC_PIN3;
	GPIO_Init(ADC_PORT,&GPIO_ADC_Structure);
	
	GPIO_ADC_Structure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_ADC_Structure.GPIO_Pin=ADC_PIN4;
	GPIO_Init(ADC_PORT,&GPIO_ADC_Structure);
	
	GPIO_ADC_Structure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_ADC_Structure.GPIO_Pin=ADC_PIN5;
	GPIO_Init(ADC_PORT,&GPIO_ADC_Structure);
	
	GPIO_ADC_Structure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_ADC_Structure.GPIO_Pin=ADC_PIN6;
	GPIO_Init(ADC_PORT,&GPIO_ADC_Structure);
}

void ADCx_Config(void)
{
	ADC_InitTypeDef ADC_Structure;
	
	//初始化时钟
	ADC_APBxClock_FUN(ADC_CLK,ENABLE);
	
	//配置ADC结构体
	
	// 连续转换模式
	ADC_Structure.ADC_ContinuousConvMode=ENABLE;
	
	// 转换结果右对齐
	ADC_Structure.ADC_DataAlign=ADC_DataAlign_Right;
	
	// 不用外部触发转换，软件开启即可
	ADC_Structure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	
	// 只使用一个ADC，属于单模式
	ADC_Structure.ADC_Mode=ADC_Mode_Independent;
	
	// 转换通道个
	ADC_Structure.ADC_NbrOfChannel=NOFCHANEL;
	
	// 开启扫描模式，多通道才要，单通道不需要
	ADC_Structure.ADC_ScanConvMode=ENABLE;
	
	//初始化ADC
	ADC_Init(ADCx,&ADC_Structure);
	
	//设置ADC时钟分频，最大支持14HZ，时钟72HZ，设置为8分频，即9HZ
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	
	//配置规则组顺序
	ADC_RegularChannelConfig(ADCx,ADC_CHANNEL1,1,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADCx,ADC_CHANNEL2,2,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADCx,ADC_CHANNEL3,3,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADCx,ADC_CHANNEL4,4,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADCx,ADC_CHANNEL5,5,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADCx,ADC_CHANNEL6,6,ADC_SampleTime_55Cycles5);
	
	
	//使能ADC
	ADC_Cmd(ADCx,ENABLE);
	
	//初始化ADC，校准寄存器
	ADC_ResetCalibration(ADCx);
	while(ADC_GetResetCalibrationStatus(ADCx));
	
	//ADC校准
	ADC_StartCalibration(ADCx);
	while(ADC_GetCalibrationStatus(ADCx));
	
	
	/*必须在ADC校准后，启动转换前启用DMA*/
	
	//开启DMA通道。
	ADC_DMACmd(ADCx, ENABLE);
	
	// 开启软件触发ADC转换
	ADC_SoftwareStartConvCmd(ADCx, ENABLE);
}


void ADCx_Init()
{
	ADCx_GPIO_Config();
	ADCx_Config();
	ADCx_DMA_Configuration();
}
