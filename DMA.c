#include "DMA.h"


void DMA_GeneralTIM_Init(void)
{
	DMA_InitTypeDef DMA_GeneralTIM_Structure;
	
	DMA_General_TIMx_CLK_FUN(DMA_General_TIMx_CLK,ENABLE);
	
	DMA_GeneralTIM_Structure.DMA_BufferSize=DMA_General_TIMx_Capture_Data_Num;
	
	DMA_GeneralTIM_Structure.DMA_DIR=DMA_DIR_PeripheralSRC;
	
	DMA_GeneralTIM_Structure.DMA_M2M=DMA_M2M_Disable;
	
	DMA_GeneralTIM_Structure.DMA_MemoryBaseAddr=(u32)DMA_GeneralTIM_Capture_Buffer;
	
	DMA_GeneralTIM_Structure.DMA_MemoryDataSize=DMA_MemoryDataSize_Word;
	
	DMA_GeneralTIM_Structure.DMA_MemoryInc= DMA_MemoryInc_Enable;
	
	DMA_GeneralTIM_Structure.DMA_Mode= DMA_Mode_Circular ;
	
	DMA_GeneralTIM_Structure.DMA_PeripheralBaseAddr=(u32)&DMA_General_TIMx->CCR1;
	
	DMA_GeneralTIM_Structure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Word;
	
	DMA_GeneralTIM_Structure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	
	DMA_GeneralTIM_Structure.DMA_Priority=DMA_Priority_High;
	
	DMA_Init(DMA_General_TIMx_Channely,&DMA_GeneralTIM_Structure);
	
	TIM_DMACmd(DMA_General_TIMx,DMA_General_TIMx_Capture_Register,ENABLE);
	
	DMA_Cmd(DMA_General_TIMx_Channely,ENABLE);
}
	
