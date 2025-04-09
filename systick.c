#include "systick.h"
#include "core_cm3.h"
#include "misc.h"

static __IO u32 TimingDelay;
/**
 * @brief ��ʼ��ϵͳ��ʱ��--10us��
 * @param ��
 * @retval ��
 */
void SysTickInit_10us(void)
{
	/* SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
	 */
	//����SystemCoreClock=72000000
	//�ж϶�ʱ���Ƿ�������ȷ����ʧ���򷵻�1
	if(SysTick_Config(SystemCoreClock / 100000))
	{
		while(1);
	}
}
/**
 * @brief ��ʼ��ϵͳ��ʱ��--ms��
 * @param ��
 * @retval ��
 */
void SysTickInit_1ms(void)
{
	/* SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
	 */
	//����SystemCoreClock=72000000
	//�ж϶�ʱ���Ƿ�������ȷ����ʧ���򷵻�1
	if(SysTick_Config(SystemCoreClock / 1000))
	{
		while(1);
	}
}
/**
 * @brief ��CPU���õ�10us��ʱ��
 * @param �ӳ�usʱ��
 * @retval ��
 */
void SysTick_EXTI_Delay_10us(__IO uint32_t time)
{
	TimingDelay=time;
	//ʹ�ܼ�ʱ��
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
	//���жϺ������ú��Լ���ֱ������0���˳��������������Ͼ���10us
	while(TimingDelay!=0);
}
/**
 * @brief ��CPU���õ�1ms��ʱ��
 * @param �ӳ�usʱ��
 * @retval ��
 */
void SysTick_EXTI_Delay_1ms(__IO uint32_t time)
{
	TimingDelay=time;
		//ʹ�ܼ�ʱ��
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
	//���жϺ������ú��Լ���ֱ������0���˳��������������Ͼ���1ms
	while(TimingDelay!=0);
}
/**
 * @brief ���жϺ������е��ã��ﵽÿ��һ��ʱ�����TimingDelay�ݼ���Ŀ��
 * @param ��
 * @retval ��
 */
void TimingDelay_Reduction(void)
{
	if(TimingDelay!=0x00)
	{
		TimingDelay--;
	}
}

/**
 * @brief ���ж����͸�CPUռ�ø߾���10us��ʱ��
 * @param ����Ҫ��ʱ��10us
 * @retval ��
 */
void SysTick_NO_EXTI_Delay_10us(__IO uint32_t us)
{
	uint32_t i;
	//���ü�ʱ��
	SysTick_Config(SystemCoreClock / 100000);
	for(i=0;i<us;i++)
	{
		// ����������ֵ��С��0��ʱ��CRTL�Ĵ�����λ16����1	
		while( !((SysTick->CTRL)&(1<<16)) );
	}
	// �ر�SysTick��ʱ��
	SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;
}
/**
 * @brief ���ж����͸�CPUռ�ø߾���1ms��ʱ��
 * @param ����Ҫ��ʱ��10us
 * @retval ��
 */
void SysTick_NO_EXTI_Delay_1ms(__IO uint32_t ms)
{
	uint32_t i;
	//���ü�ʱ��
	SysTick_Config(SystemCoreClock /1000);
	for(i=0;i<ms;i++)
	{
		// ����������ֵ��С��0��ʱ��CRTL�Ĵ�����λ16����1	
		while( !((SysTick->CTRL)&(1<<16)) );
	}
	// �ر�SysTick��ʱ��
	SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;
}
