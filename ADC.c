#include "DMA.h"
#include "ADC.h"

void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_ADC_Structure;
	
	//初始化时钟
	ADCx_1_GPIO_APBxClock_FUN(ADCx_1_GPIO_CLK,ENABLE);
	ADCx_2_GPIO_APBxClock_FUN(ADCx_2_GPIO_CLK,ENABLE);
	
	//配置GPIO结构体
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
	
	//初始化时钟
	ADCx_1_APBxClock_FUN(ADCx_1_CLK,ENABLE);
	ADCx_2_APBxClock_FUN(ADCx_2_CLK,ENABLE);
	
	//配置ADC1结构体
	
	// 模式设置为同步规则模式
	ADC_Structure.ADC_Mode=ADC_Mode_RegSimult;
	
	// 连续转换模式
	ADC_Structure.ADC_ContinuousConvMode=ENABLE;
	
	// 转换结果右对齐
	ADC_Structure.ADC_DataAlign=ADC_DataAlign_Right;
	
	// 不用外部触发转换，软件开启即可
	ADC_Structure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	
	// 转换通道1个
	ADC_Structure.ADC_NbrOfChannel=NOFCHANEL;
	
	// 禁止扫描模式，多通道才要，单通道不需要
	ADC_Structure.ADC_ScanConvMode=ENABLE;
	
	//初始化ADC
	ADC_Init(ADCx_1,&ADC_Structure);
	
	//设置ADC时钟分频，最大支持14HZ，时钟72HZ，设置为8分频，即9HZ
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	
	//配置规则组顺序
	ADC_RegularChannelConfig(ADCx_1,ADCx_1_CHANNEL,1,ADC_SampleTime_239Cycles5);
	
	
	//配置ADC2结构体
	
	// 模式设置为同步规则模式
	ADC_Structure.ADC_Mode=ADC_Mode_RegSimult;
	
	// 连续转换模式
	ADC_Structure.ADC_ContinuousConvMode=ENABLE;
	
	// 转换结果右对齐
	ADC_Structure.ADC_DataAlign=ADC_DataAlign_Right;
	
	// 不用外部触发转换，软件开启即可
	ADC_Structure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	
	// 转换通道1个
	ADC_Structure.ADC_NbrOfChannel=NOFCHANEL;
	
	// 禁止扫描模式，多通道才要，单通道不需要
	ADC_Structure.ADC_ScanConvMode=ENABLE;
	
	//初始化ADC
	ADC_Init(ADCx_2,&ADC_Structure);
	
	//设置ADC时钟分频，最大支持14HZ，时钟72HZ，设置为8分频，即9HZ
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	
	//配置规则组顺序
	ADC_RegularChannelConfig(ADCx_2,ADCx_2_CHANNEL,1,ADC_SampleTime_239Cycles5);
	
	
	//使能ADC1
	ADC_Cmd(ADCx_1,ENABLE);
	
	//使能ADC2
	ADC_Cmd(ADCx_2,ENABLE);
	
	//初始化ADC1，校准寄存器
	ADC_ResetCalibration(ADCx_1);
	while(ADC_GetResetCalibrationStatus(ADCx_1));
	
	//ADC1校准
	ADC_StartCalibration(ADCx_1);
	while(ADC_GetCalibrationStatus(ADCx_1));
	
	//初始化ADC2，校准寄存器
	ADC_ResetCalibration(ADCx_2);
	while(ADC_GetResetCalibrationStatus(ADCx_2));
	
	//ADC2校准
	ADC_StartCalibration(ADCx_2);
	while(ADC_GetCalibrationStatus(ADCx_2));
	
	
	/*必须在ADC校准后，启动转换前启用DMA*/
	
	//开启ADC1和DMA通道。
	ADC_DMACmd(ADCx_1, ENABLE);
	
	//开启ADC2和DMA通道。
	ADC_DMACmd(ADCx_2, ENABLE);
	
	
	//ADC2一定要在ADC1之前开启
	
	//开启硬件触发ADC2转换
	ADC_ExternalTrigConvCmd(ADCx_2,ENABLE);
	
	// 开启软件触发ADC1转换
	ADC_SoftwareStartConvCmd(ADCx_1, ENABLE);
	
}


void ADCx_Init()
{
	ADCx_GPIO_Config();
	ADCx_Config();
	ADCx_DMA_Configuration();
}
