#include "SRAM.h"
#include "malloc.h"
#include "stm32f10x.h"


/**
 * @brief SRAM���ڴ���Ͷ�дʵ��
 * @param ��
 * @retval ��
 */
int main(void)
{
	u8 *temp;
	u8 sramx=SRAMIN;   //�������ڲ�SRAM����ʾ��
	u8 SRAM_Test_an=2;
	u8 U8data[1]={12};
	u16 U16data[1]={65535};
	u32 U32data[1]={0xCCCCCCCC};
	u8 *U8temp;
	u16 *U16temp;
	u32 *U32temp;
	FSMC_SRAM_Init();
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);		//��ʼ���ⲿ�ڴ��
	
	printf("���Կ�ʼ\n");
	printf("����SRAM�Ƿ�����\n");
	SRAM_Test_an=SRAM_Test();
	if(SRAM_Test_an!=1)
	{
		printf("SRAM_Test��д�������⣡������USART������");
		return 0;
	}
	else
	{
		printf("SRAM����\n");
		printf("��ʼ�����ڴ�\n");
		temp = mymalloc ( sramx, 1024 *2 );//����2K�ֽ�
	
		sprintf((char*)temp,"Memory Malloc");//ʹ������ռ�洢����
		
		printf ( "SRAMIN USED:%d%%\r\n", my_mem_perused(SRAMIN) );//��ʾ�ڲ��ڴ�ʹ����
		printf ( "SRAMEX USED:%d%%\r\n", my_mem_perused(SRAMEX) );//��ʾ�ⲿ�ڴ�ʹ����

		myfree(sramx,temp);//�ͷ��ڴ�
		temp=0;		      	//ָ��յ�ַ

		printf ( "\r\n�ͷ��ڴ�\r\n" );
		printf ( "SRAMIN USED:%d%%\r\n", my_mem_perused(SRAMIN) );//��ʾ�ڲ��ڴ�ʹ����
		printf ( "SRAMEX USED:%d%%\r\n", my_mem_perused(SRAMEX) );//��ʾ�ⲿ�ڴ�ʹ����
	
	}
	printf("���ж�д����\n");
	SRAM_WriteU8Buffer(U8data,Bank1_SRAM1_ADDR,1);
	SRAM_WriteU16Buffer(U16data,Bank1_SRAM1_ADDR+10,1);
	SRAM_WriteU32Buffer(U32data,Bank1_SRAM1_ADDR+30,1);
	
	SRAM_ReadU8Buffer(U8temp,Bank1_SRAM1_ADDR,1);
	SRAM_ReadU16Buffer(U16temp,Bank1_SRAM1_ADDR+10,1);
	SRAM_ReadU32Buffer(U32temp,Bank1_SRAM1_ADDR+30,1);
	
	while(1);
}
