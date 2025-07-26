#include "stm32f10x.h"   // �൱��51��Ƭ���е�  #include <reg51.h>
#include "USART.h"
#include "key.h"

void WriteProtect_Toggle(void);
void ReadProtect_Toggle(void);


int main(void)
{
	USART_Configuration();
	KEY_GPIO_Config();
	
	
	if(FLASH_GetWriteProtectionOptionByte() ==0 )
	{
		printf("\r\nĿǰоƬ����д����״̬����Key1���������\r\n");
		printf("д�����Ĵ�����ֵ��WRPR=0x%x\r\n",FLASH_GetWriteProtectionOptionByte());
	}
	else //��д����
	{
		printf("\r\nĿǰоƬ�� д �������� Key1 �������ó� д ����\r\n");
		printf("д�����Ĵ�����ֵ��WRPR=0x%x\r\n",FLASH_GetWriteProtectionOptionByte());
	}
	
	
	/*  ������SET��˵�����ڶ�����״̬ */
	if(FLASH_GetReadOutProtectionStatus () == SET )
	{
		printf("\r\nĿǰоƬ���ڶ�����״̬����Key2���������\r\n");
	}
	else
	{
		printf("\r\nĿǰоƬ�� �� �������� Key2 �������ó� �� ����\r\n");
	}
	
	
	while(1)                            
	{	   
		if( KEY_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON  )
		{
			WriteProtect_Toggle();
		} 

		if( KEY_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON  )
		{
			ReadProtect_Toggle();			
		}		
	}	
}

/************************д������ת************************/
void WriteProtect_Toggle(void)
{
	//
	if(FLASH_GetWriteProtectionOptionByte()==0)
	{
		printf("\r\nоƬ����д����״̬������ִ�нⱣ������...\r\n");
		
		//�����flash�Ĵ������в���
		FLASH_Unlock();
		
		//����ѡ���ֽ�����
		FLASH_EraseOptionBytes();
		
		//���д����
		FLASH_EnableWriteProtection(0);
		printf("\r\n������ɣ�оƬ���Զ���λ���������ã���λ��оƬ����д����״̬\r\n");
		
		//��λоƬ��������core_cm3.h�У���׼��鲻��
		NVIC_SystemReset();
	}
	else
	{
		printf("\r\nоƬ����д����״̬������ִ�б�������...\r\n");
		
		//�����flash�Ĵ������в���
		FLASH_Unlock();
		
		//����ѡ���ֽ�����
		FLASH_EraseOptionBytes();
		
		//���д����
		FLASH_EnableWriteProtection(FLASH_WRProt_AllPages);
		printf("\r\n������ɣ�оƬ���Զ���λ���������ã���λ��оƬ��д����״̬\r\n");
		
		//��λоƬ��������core_cm3.h�У���׼��鲻��
		NVIC_SystemReset();
	}
	
}

/************************��������ת************************/
void ReadProtect_Toggle(void)
{
	if(FLASH_GetReadOutProtectionStatus()==SET)
	{
		printf("\r\nоƬ���ڶ�����״̬,�������������\r\n");
		
		//�����flash�Ĵ������в���
		FLASH_Unlock();
		
		//����ѡ���ֽ�����
		FLASH_EraseOptionBytes();
		
		printf("���������������������������FLASH�������������");
		printf("���ڽ���������գ����Ժ��治�����κ���ʾ���");
		printf("�ȴ�20��󼴿ɸ�оƬ�����µĳ���...\r\n");
		
		FLASH_ReadOutProtection(DISABLE);
		
	}
	else
	{
		printf("\r\nоƬδ���ڶ�����״̬,����ִ�ж�����\r\n");
		
		//�����flash�Ĵ������в���
		FLASH_Unlock();
		
		//����ѡ���ֽ�����
		FLASH_EraseOptionBytes();
		
		
		FLASH_ReadOutProtection(ENABLE);
		
		printf("оƬ�ѱ�����Ϊ���������ϵ縴λ����Ч���������¸��������ϵ磬ֻ����λ����Ч��\r\n");
		printf("���ڱ���״̬���޷����������³��򣬱���Ҫ�Ƚ������״̬������\r\n");
	}
}
