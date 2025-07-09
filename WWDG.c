#include "WWDG.h"

void WWDG_NVIC_Config(void)
{
	NVIC_InitTypeDef WWDG_NVIC_Structure;
	
	//1���飬��Ϊ�����ܷɱ��������һ�̽����жϱ�����Ҫ����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	WWDG_NVIC_Structure.NVIC_IRQChannel = WWDG_IRQn;
  WWDG_NVIC_Structure.NVIC_IRQChannelPreemptionPriority = 0;
  WWDG_NVIC_Structure.NVIC_IRQChannelSubPriority = 0;
  WWDG_NVIC_Structure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&WWDG_NVIC_Structure);
}


/**
 * @brief ����WWDG
 * @param ctv:���¼��ؼ�������ֵ(ι��)��prv:����Ԥ��Ƶ����ֵ;wdv:WWDG��������ֵ
 * @retval ��
 */
void WWDG_Config(u8 ctv,u32 prv,u8 wdv)
{
	//����ʱ��
	WWDG_CLK_FUN(WWDG_CLK,ENABLE);
	
	//���¼��ؼ�������ֵ(ι��)
	WWDG_SetCounter(ctv);
	
	//����Ԥ��Ƶ����ֵ
	WWDG_SetPrescaler(prv);
	
	//���ô�������ֵ
	WWDG_SetWindowValue(wdv);
	
	//ʹ��WWDG
	WWDG_Enable(WWDG_CNT);
	
	// �����ǰ�����жϱ�־λ
	WWDG_ClearFlag();	
	
	//����NVIC
	WWDG_NVIC_Config();
	
	//����EWI�ж�
	WWDG_EnableIT();
	
	
}

void WWDG_Feed(void)
{
	WWDG_SetCounter(WWDG_CNT);
}
