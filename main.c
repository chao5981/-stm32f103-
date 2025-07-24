#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "USART.h"
#include <stdlib.h>

u32 RW_test_Value=7;
u32 ZI_test_Value=0;

//定义数组到“指定的存储空间”
uint8_t ZI_test_Grup[10]  ={0};
//定义数组到“指定的存储空间”
uint8_t RW_test_Grup[10] ={1,2,3};

int main(void)
{
	u32 RW_inter_test_Value=10;
	uint32_t * pointer = (uint32_t*)malloc(sizeof(uint32_t)*3); 
	USART_Configuration();
	
	printf("\r\nSCT文件应用——自动分配变量到“指定的存储空间”实验\r\n");
  
	printf("\r\n使用“	uint32_t RW_inter_test_Value =10; ”语句定义的局部变量：\r\n");
	printf("结果：它的地址为：0x%x,变量值为：%d\r\n",(uint32_t)&RW_inter_test_Value,RW_inter_test_Value);
	
  printf("\r\n使用“uint32_t RW_test_Value  =7 ;”语句定义的全局变量：\r\n");
	printf("结果：它的地址为：0x%x,变量值为：%d\r\n",(uint32_t)&RW_test_Value,RW_test_Value);
	
  printf("\r\n使用“uint32_t ZI_test_Value  =0 ; ”语句定义的全局变量：\r\n");
	printf("结果：它的地址为：0x%x,变量值为：%d\r\n",(uint32_t)&ZI_test_Value,ZI_test_Value);
	
	
	printf("\r\n使用“uint8_t ZI_test_Grup[10]  ={0};”语句定义的全局数组：\r\n");
	printf("结果：它的地址为：0x%x,变量值为：%d,%d,%d\r\n",(uint32_t)&ZI_test_Grup,ZI_test_Grup[0],ZI_test_Grup[1],ZI_test_Grup[2]);
	
  printf("\r\n使用“uint8_t RW_test_Grup[10] ={1,2,3};”语句定义的全局数组：\r\n");
	printf("结果：它的地址为：0x%x,变量值为：%d，%d,%d\r\n",(uint32_t)&RW_test_Grup,RW_test_Grup[0],RW_test_Grup[1],RW_test_Grup[2]);
	
		
/*本实验中的sct配置，若使用外部存储器时，堆区工作可能不正常，
  使用malloc无法得到正常的地址，不推荐在实际工程应用*/
  /*另一种我们推荐的配置请参考教程中的说明*/
	
	if(pointer != NULL)
	{
		*(pointer)=1;
		*(++pointer)=2;
		*(++pointer)=3;	
		
		printf("\r\n使用“	uint32_t *pointer = (uint32_t*)malloc(sizeof(uint32_t)*3); ”动态分配的变量\r\n");
		printf("\r\n定义后的操作为：\r\n*(pointer++)=1;\r\n*(pointer++)=2;\r\n*pointer=3;\r\n\r\n");
		printf("结果：操作后它的地址为：0x%x,查看变量值操作：\r\n",(uint32_t)pointer); 
		printf("*(pointer--)=%d, \r\n",*(pointer--));
		printf("*(pointer--)=%d, \r\n",*(pointer--));
		printf("*(pointer)=%d, \r\n",*(pointer));
		
		free(pointer);
	}
	else
	{
		printf("\r\n使用malloc动态分配变量出错！！！\r\n");	
	}
}


