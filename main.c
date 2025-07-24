#include "stm32f10x.h"   // �൱��51��Ƭ���е�  #include <reg51.h>
#include "USART.h"
#include <stdlib.h>

u32 RW_test_Value=7;
u32 ZI_test_Value=0;

//�������鵽��ָ���Ĵ洢�ռ䡱
uint8_t ZI_test_Grup[10]  ={0};
//�������鵽��ָ���Ĵ洢�ռ䡱
uint8_t RW_test_Grup[10] ={1,2,3};

int main(void)
{
	u32 RW_inter_test_Value=10;
	uint32_t * pointer = (uint32_t*)malloc(sizeof(uint32_t)*3); 
	USART_Configuration();
	
	printf("\r\nSCT�ļ�Ӧ�á����Զ������������ָ���Ĵ洢�ռ䡱ʵ��\r\n");
  
	printf("\r\nʹ�á�	uint32_t RW_inter_test_Value =10; ����䶨��ľֲ�������\r\n");
	printf("��������ĵ�ַΪ��0x%x,����ֵΪ��%d\r\n",(uint32_t)&RW_inter_test_Value,RW_inter_test_Value);
	
  printf("\r\nʹ�á�uint32_t RW_test_Value  =7 ;����䶨���ȫ�ֱ�����\r\n");
	printf("��������ĵ�ַΪ��0x%x,����ֵΪ��%d\r\n",(uint32_t)&RW_test_Value,RW_test_Value);
	
  printf("\r\nʹ�á�uint32_t ZI_test_Value  =0 ; ����䶨���ȫ�ֱ�����\r\n");
	printf("��������ĵ�ַΪ��0x%x,����ֵΪ��%d\r\n",(uint32_t)&ZI_test_Value,ZI_test_Value);
	
	
	printf("\r\nʹ�á�uint8_t ZI_test_Grup[10]  ={0};����䶨���ȫ�����飺\r\n");
	printf("��������ĵ�ַΪ��0x%x,����ֵΪ��%d,%d,%d\r\n",(uint32_t)&ZI_test_Grup,ZI_test_Grup[0],ZI_test_Grup[1],ZI_test_Grup[2]);
	
  printf("\r\nʹ�á�uint8_t RW_test_Grup[10] ={1,2,3};����䶨���ȫ�����飺\r\n");
	printf("��������ĵ�ַΪ��0x%x,����ֵΪ��%d��%d,%d\r\n",(uint32_t)&RW_test_Grup,RW_test_Grup[0],RW_test_Grup[1],RW_test_Grup[2]);
	
		
/*��ʵ���е�sct���ã���ʹ���ⲿ�洢��ʱ�������������ܲ�������
  ʹ��malloc�޷��õ������ĵ�ַ�����Ƽ���ʵ�ʹ���Ӧ��*/
  /*��һ�������Ƽ���������ο��̳��е�˵��*/
	
	if(pointer != NULL)
	{
		*(pointer)=1;
		*(++pointer)=2;
		*(++pointer)=3;	
		
		printf("\r\nʹ�á�	uint32_t *pointer = (uint32_t*)malloc(sizeof(uint32_t)*3); ����̬����ı���\r\n");
		printf("\r\n�����Ĳ���Ϊ��\r\n*(pointer++)=1;\r\n*(pointer++)=2;\r\n*pointer=3;\r\n\r\n");
		printf("��������������ĵ�ַΪ��0x%x,�鿴����ֵ������\r\n",(uint32_t)pointer); 
		printf("*(pointer--)=%d, \r\n",*(pointer--));
		printf("*(pointer--)=%d, \r\n",*(pointer--));
		printf("*(pointer)=%d, \r\n",*(pointer));
		
		free(pointer);
	}
	else
	{
		printf("\r\nʹ��malloc��̬���������������\r\n");	
	}
}


