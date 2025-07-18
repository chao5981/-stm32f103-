#include "exti.h"

void PWR_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  
  /* ����NVICΪ���ȼ���1 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* �����ж�Դ������1 */
  NVIC_InitStructure.NVIC_IRQChannel = KEY1_INT_EXTI_IRQ;
  /* ������ռ���ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* ���������ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж�ͨ�� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
  NVIC_Init(&NVIC_InitStructure);
	
	
	/* �����ж�Դ������2������ʹ������������� */  
  NVIC_InitStructure.NVIC_IRQChannel = KEY2_INT_EXTI_IRQ;
  NVIC_Init(&NVIC_InitStructure);
}

void PWR_EXTI_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;
	
	//����ʱ��
	RCC_APB2PeriphClockCmd(KEY1_INT_GPIO_CLK,ENABLE);
	
	//����NVIC
	PWR_NVIC_Config();

	//����key1��GPIO��EXTI�ṹ��

  GPIO_InitStructure.GPIO_Pin = KEY1_INT_GPIO_PIN;

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure);
	
	EXTI_InitStructure.EXTI_Line=KEY1_INT_EXTI_LINE;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
	
	//ȷ���жϻ���ĸ����Ź���
	GPIO_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE,KEY1_INT_EXTI_PINSOURCE);
	
	EXTI_Init(&EXTI_InitStructure);
	
	
	//����key2��GPIO��EXTI�ṹ��
	GPIO_InitStructure.GPIO_Pin = KEY2_INT_GPIO_PIN;

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);
	
	EXTI_InitStructure.EXTI_Line=KEY2_INT_EXTI_LINE;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	
	//ȷ���жϻ���ĸ����Ź���
	GPIO_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE,KEY2_INT_EXTI_PINSOURCE);
	EXTI_Init(&EXTI_InitStructure);
	
}

