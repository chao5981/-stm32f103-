#include "SRAM.h"
#include "malloc.h"
#include "stm32f10x.h"


/**
 * @brief SRAM的内存监测和读写实验
 * @param 无
 * @retval 无
 */
int main(void)
{
	u8 *temp;
	u8 sramx=SRAMIN;   //这里以内部SRAM进行示例
	u8 SRAM_Test_an=2;
	u8 U8data[1]={12};
	u16 U16data[1]={65535};
	u32 U32data[1]={0xCCCCCCCC};
	u8 *U8temp;
	u16 *U16temp;
	u32 *U32temp;
	FSMC_SRAM_Init();
	my_mem_init(SRAMIN);		//初始化内部内存池
	my_mem_init(SRAMEX);		//初始化外部内存池
	
	printf("测试开始\n");
	printf("测试SRAM是否正常\n");
	SRAM_Test_an=SRAM_Test();
	if(SRAM_Test_an!=1)
	{
		printf("SRAM_Test读写出现问题！请利用USART检查程序！");
		return 0;
	}
	else
	{
		printf("SRAM正常\n");
		printf("开始分配内存\n");
		temp = mymalloc ( sramx, 1024 *2 );//申请2K字节
	
		sprintf((char*)temp,"Memory Malloc");//使用申请空间存储内容
		
		printf ( "SRAMIN USED:%d%%\r\n", my_mem_perused(SRAMIN) );//显示内部内存使用率
		printf ( "SRAMEX USED:%d%%\r\n", my_mem_perused(SRAMEX) );//显示外部内存使用率

		myfree(sramx,temp);//释放内存
		temp=0;		      	//指向空地址

		printf ( "\r\n释放内存\r\n" );
		printf ( "SRAMIN USED:%d%%\r\n", my_mem_perused(SRAMIN) );//显示内部内存使用率
		printf ( "SRAMEX USED:%d%%\r\n", my_mem_perused(SRAMEX) );//显示外部内存使用率
	
	}
	printf("进行读写操作\n");
	SRAM_WriteU8Buffer(U8data,Bank1_SRAM1_ADDR,1);
	SRAM_WriteU16Buffer(U16data,Bank1_SRAM1_ADDR+10,1);
	SRAM_WriteU32Buffer(U32data,Bank1_SRAM1_ADDR+30,1);
	
	SRAM_ReadU8Buffer(U8temp,Bank1_SRAM1_ADDR,1);
	SRAM_ReadU16Buffer(U16temp,Bank1_SRAM1_ADDR+10,1);
	SRAM_ReadU32Buffer(U32temp,Bank1_SRAM1_ADDR+30,1);
	
	while(1);
}
