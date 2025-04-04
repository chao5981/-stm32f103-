#include "clock.h"
#include "stm32f10x_rcc.h"

void HSE_SetSysClock(uint32_t pllmul)
{
	__IO uint32_t HSEStartUpStatus = 0;
	//��RCC��ʼ��Ϊ��λ״̬
	RCC_DeInit();
	//ʹ��HSE
	RCC_HSEConfig(RCC_HSE_ON);
	HSEStartUpStatus=RCC_WaitForHSEStartUp();
	//�ȴ�HSE�ȶ����ȶ���ż�������
	if( HSEStartUpStatus==SUCCESS)
	{
		//----------------------------------------------------------------------//
		// �������ǲ���FLASH�����õ��ģ����������FLASH�Ļ���������ע�͵�ҲûӰ��
    // ʹ��FLASH Ԥ��ȡ������
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    // SYSCLK�������������ʱ��ı������ã�����ͳһ���ó�2
		// ���ó�2��ʱ��SYSCLK����48MҲ���Թ�����������ó�0����1��ʱ��
		// ������õ�SYSCLK�����˷�Χ�Ļ���������Ӳ�����󣬳��������
		// 0��0 < SYSCLK <= 24M
		// 1��24< SYSCLK <= 48M
		// 2��48< SYSCLK <= 72M
    FLASH_SetLatency(FLASH_Latency_2);
		//----------------------------------------------------------------------//
		//*******Ԥ�������÷Ŵ������Сϵ��********//
		//��HCLK����Ϊ1��Ƶ
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		//��APB2����Ϊ1��Ƶ
		RCC_PCLK2Config (RCC_HCLK_Div1);
		//��PLLMUL����Ϊ1��Ƶ���Ŵ�����������
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,pllmul);
		
		//ʹ��RLL
		RCC_PLLCmd(ENABLE);
		//�ȴ�RCC�ȶ�
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET)
		{
		}
		//�ȶ���SW����PLLCLK����Ϊϵͳʱ��SYSCLK
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		//��ȡʱ���л�λ��ȷ��PLLCLK������Ϊϵͳʱ��
		//��ע��SW���ܵ�ֵ��0x00��HSI���趨Ϊϵͳʱ�ӣ�0x04��HSE������Ϊϵͳʱ�ӣ�0x08��PLL������Ϊϵͳʱ�ӣ�0x13δʹ��
		while (RCC_GetSYSCLKSource() != 0x08)
    {
    }
	}
	else
	{
		//���HSE���ȶ���	������������ɽ��е���
		//���HSE�������ϻ���ʧ�ܣ���ϵͳ���HSI����Ϊϵͳʱ��
		while (1)
    {
    }
	}
}

void HSI_SYSClock(uint32_t pllmul)
{
	//��RCC��ʼ��Ϊ��λ״̬
	RCC_DeInit();
	//ʹ��HSI
	RCC_HSICmd(ENABLE);
	if(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) != RESET)
	{
		//----------------------------------------------------------------------//
		// �������ǲ���FLASH�����õ��ģ����������FLASH�Ļ���������ע�͵�ҲûӰ��
    // ʹ��FLASH Ԥ��ȡ������
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    // SYSCLK�������������ʱ��ı������ã�����ͳһ���ó�2
		// ���ó�2��ʱ��SYSCLK����48MҲ���Թ�����������ó�0����1��ʱ��
		// ������õ�SYSCLK�����˷�Χ�Ļ���������Ӳ�����󣬳��������
		// 0��0 < SYSCLK <= 24M
		// 1��24< SYSCLK <= 48M
		// 2��48< SYSCLK <= 72M
    FLASH_SetLatency(FLASH_Latency_2);
		//----------------------------------------------------------------------//
		//*******Ԥ�������÷Ŵ������Сϵ��********//
		//��HCLK����Ϊ1��Ƶ
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		//��APB2����Ϊ1��Ƶ
		RCC_PCLK2Config (RCC_HCLK_Div1);
		//��PLLMUL����Ϊ1��Ƶ���Ŵ�����������
		RCC_PLLConfig(RCC_PLLSource_HSI_Div2,pllmul);
		//ʹ��PLL
	  RCC_PLLCmd(ENABLE);
		//�ȴ�PLL�ȶ�
	  while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET)
		{
		}
		//�ȶ���SWϵͳʱ��ΪPLLCLK
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		//��ȡʱ��λ״̬��ȷ��ѡ���λPLLCLK
		while(RCC_GetSYSCLKSource()!=0x08)
		{
		}
	}
	else
	{
		// ���HSI����ʧ�ܣ���ô����ͻ���������û�����������ӳ���Ĵ��봦��
		// ��HSE����ʧ�ܻ��߹��ϵ�ʱ�򣬵�Ƭ�����Զ���HSI����Ϊϵͳʱ�ӣ�HSI���ڲ��ĸ���ʱ�ӣ�8MHZ
		while(1)
		{
		}
	}
}
