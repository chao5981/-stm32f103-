#include "IWDG.h"

/*
 * ���� IWDG �ĳ�ʱʱ��
 * Tout = prv/40 * rlv (s)
 *      prv������[4,8,16,32,64,128,256]
 * prv:Ԥ��Ƶ��ֵ��ȡֵ���£�
 *     @arg IWDG_Prescaler_4: IWDG prescaler set to 4
 *     @arg IWDG_Prescaler_8: IWDG prescaler set to 8
 *     @arg IWDG_Prescaler_16: IWDG prescaler set to 16
 *     @arg IWDG_Prescaler_32: IWDG prescaler set to 32
 *     @arg IWDG_Prescaler_64: IWDG prescaler set to 64
 *     @arg IWDG_Prescaler_128: IWDG prescaler set to 128
 *     @arg IWDG_Prescaler_256: IWDG prescaler set to 256
 *
 * rlv:��װ�ؼĴ�����ֵ��ȡֵ��ΧΪ��0-0XFFF
 * �������þ�����
 * IWDG_Config(IWDG_Prescaler_64 ,625);  // IWDG 1s ��ʱ���
 * ���ʱ�� Tout = prv/40 * rlv(s) Tout=64/40*625=1s
 */



/**
 * @brief ��ʼ��IWDG
 * @param prv:prescaler value��Ԥ��Ƶ����ֵ ��rlv:reload value:���ؼĴ�����ֵ
 * @retval ��
 */
void IWDG_Config(u8 prv, u16 rlv)
{
	//ʹ�� Ԥ��Ƶ�Ĵ���PR����װ�ؼĴ���RLR��д
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	//����Ԥ��Ƶ����ֵ
	IWDG_SetPrescaler(prv);
	
	//������װ�ؼĴ�����ֵ
	IWDG_SetReload(rlv);
	
	//����װ�ؼĴ�����ֵ�ŵ���������
	IWDG_ReloadCounter();
	
	//ʹ��IWDG
	IWDG_Enable();
	
}


void IWDG_Feed(void)
{
	IWDG_ReloadCounter();
}
