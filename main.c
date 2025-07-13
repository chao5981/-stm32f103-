#include "USART.h"
#include "stm32_eval_sdio_sd.h"
#include "ff.h"


/**
  ******************************************************************************
  *                              定义变量
  ******************************************************************************
  */
FATFS fs;													/* FatFs文件系统对象 */
FIL fnew;													/* 文件对象 */
FRESULT res_sd;                /* 文件操作结果 */
UINT fnum;            					  /* 文件成功读写数量 */
BYTE ReadBuffer[1024]={0};        /* 读缓冲区 */
BYTE WriteBuffer[] =              /* 写缓冲区*/
"\r\n你好超超\r\n";  
FILINFO fno;
DIR dir;
extern  SD_CardInfo SDCardInfo;

int main(void)
{
	USART_Configuration();
	printf("\r\n****** 这是一个SD卡 文件系统实验 ******\r\n");
	res_sd=f_mount(&fs,"0:",1);
	printf("****");
	if(res_sd==FR_NO_FILESYSTEM)
	{
		printf("》SD卡还没有文件系统，即将进行格式化...\r\n");
		res_sd=f_mkfs("0:",0,0);
		//格式化文件后需要重新挂载
		if(res_sd==FR_OK)
		{
				printf("》SD卡已成功格式化文件系统。\r\n");
			//取消挂载
				f_mount(NULL,"0:",1);
			//重新挂载
				f_mount(&fs,"0:",1);
				
		}
		else
		{
			printf("！！SD卡挂载文件系统失败。(%d)\r\n",res_sd);
			printf("！！可能原因：SD卡初始化不成功。\r\n");
			while(1);
		}
		
	}
	if(res_sd==FR_OK)
	{
		printf("\r\nSD卡挂载成功！r\n");
	}
	else
	{
		printf("\r\nSD卡挂载失败！\r\n");
		while(1);
	}
	/*----------------------- 文件系统测试：写测试 -----------------------------*/
	/* 打开文件，如果文件不存在则创建它 */
	printf("\r\n****** 即将进行文件写入测试... ******\r\n");	
	res_sd=f_open(&fnew,"0:超超想超超.txt",FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
	if(res_sd==FR_OK)
	{
		printf("》打开/创建超超想超超.txt文件成功，向文件写入数据。\r\n");
		res_sd=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
		if(res_sd==FR_OK)
		{
			printf("》文件写入成功，写入字节数据：%d\n",fnum);
      printf("》向文件写入的数据为：\r\n%s\r\n",WriteBuffer);
		}
		else
		{
			printf("！！文件写入失败：(%d)\n",res_sd);
		}
		//关闭文件
		f_close(&fnew);
	}
	else
	{	
		printf("！！打开/创建文件失败。\r\n");
	}
	
	/*------------------- 文件系统测试：读测试 ------------------------------------*/
	printf("****** 即将进行文件读取测试... ******\r\n");
	res_sd=f_open(&fnew,"0:超超想超超.txt",FA_READ|FA_OPEN_EXISTING);
	if(res_sd==FR_OK)
	{
			printf("》打开文件成功。\r\n");
			res_sd=f_read(&fnew,ReadBuffer,sizeof(ReadBuffer),&fnum);
			if(res_sd==FR_OK)
			{
					 printf("》文件读取成功,读到字节数据：%d\r\n",fnum);
					 printf("》读取得的文件数据为：\r\n%s \r\n", ReadBuffer);	
			}
			else
			{
					 printf("！！文件读取失败：(%d)\n",res_sd);
			}		
			
	}
	else
	{	
		printf("！！打开/创建文件失败。\r\n");
	}
	//关闭文件
	f_close(&fnew);
	
	/*----------------------- 文件系统测试：创建子目录测试 -----------------------------*/
	
	//打开目录，如果没有目录就创建目录
	//经过测试，程序运行到f_stat会直接卡死，所以不建议用
	printf("\r\n准备读取文件夹\r\n");
	res_sd=	f_opendir(&dir,"0:/LC_SDIO_Test002");
	printf("\r\n读取完毕文件夹r\n");
	if(res_sd==FR_OK)
	{
		printf("\r\n0:/LC_SDIO_Test002文件夹存在！\r\n");
	}
	else if(res_sd!=FR_OK)
	{
		res_sd=f_mkdir("0:/LC_SDIO_Test002");
		if(res_sd==FR_OK)
		{
			printf("\r\n创建文件夹0:/LC_SDIO_Test002成功\r\n");
		}
		else
		{
			printf("\r\n!!创建文件夹0:/LC_SDIO_Test002失败\r\n");
			while(1);
		}
	}
	printf("\r\n文件夹准备完毕!\r\n");
		//创建文件
		res_sd=f_open(&fnew,"0:/LC_SDIO_Test002/stm32f103_LC001.txt",FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
		if(res_sd==FR_OK)
		{
			printf("》打开/创建0:/LC_SDIO_Test002/stm32f103_LC001.txt文件成功，向文件写入数据。\r\n");
			res_sd=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
			if(res_sd==FR_OK)
			{
				printf("》文件写入成功，写入字节数据：%d\n",fnum);
				printf("》向文件写入的数据为：\r\n%s\r\n",WriteBuffer);
			}
			else
			{
				printf("！！文件写入失败：(%d)\n",res_sd);
			}
			//关闭文件
			f_close(&fnew);
			f_closedir(&dir);
		}
		else
		{	
			printf("！！打开/创建文件失败。\r\n");
		}
		
		/*******************文件续写/格式化写入测试***********************/
		printf("\r\n准备打开文件进行文件续写实验\r\n");
		//打开文件夹
		res_sd=	f_opendir(&dir,"0:/LC_SDIO_Test002");
		printf("打开目录结果: %d\n", res_sd); 
		if(res_sd==FR_OK)
		{
			printf("\r\n文件夹0:/LC_SDIO_Test002打开成功\r\n");
		}
		else
		{
			printf("\r\n文件夹0:/LC_SDIO_Test002打开失败\r\n");
			while(1);
		}
		res_sd=f_open(&fnew,"0:/LC_SDIO_Test002/stm32f103_LC001.txt",FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
		printf("打开文件结果: %d\n", res_sd); 
		if(res_sd==FR_OK)
		{
			printf("\r\n文件打开成功,准备在文件后追加字体\r\n");
			res_sd=f_lseek(&fnew,f_size(&fnew));
			if(res_sd==FR_OK)
			{
					f_printf(&fnew,"今天是个好日子\n");
					res_sd=f_lseek(&fnew,0);
					res_sd = f_read(&fnew,ReadBuffer,f_size(&fnew),&fnum);
				if(res_sd == FR_OK)
				{
					printf("》0:/LC_SDIO_Test002/stm32f103_LC001.txt文件内容：\n%s\n",ReadBuffer);
				}
			}
			//关闭文件
			f_close(&fnew);
			//关闭文件夹
			f_closedir(&dir);
		}
		else
		{
			printf("\r\n文件打开失败\r\n");
		}
		
		/*******************文件删除测试***********************/
		printf("\r\n准备删除文件\r\n");
		res_sd=f_unlink("0:超超想超超.txt");
		if(res_sd==FR_OK)
		{
			printf("\r\n0:超超想超超.txt文件删除成功\r\n");
		}
		else
		{
			printf("\r\n文件删除失败\r\n");
		}
		
		
		/****************文件改名测试**********************/
		printf("\r\n准备进行文件改名测试\r\n");
		res_sd=f_rename("0:/LC_SDIO_Test001/stm32f103_LC001.txt","0:/LC_SDIO_Test001/stm32f103_LC001_rename_test.txt");
		if(res_sd==FR_OK)
		{
			printf("\r\n0:/LC_SDIO_Test001/stm32f103_LC001.txt改名成功！\r\n");
		}
		else
		{
			printf("\r\n0:/LC_SDIO_Test001/stm32f103_LC001.txt改名失败！\r\n");
		}
		
	//操作完成取消挂载
	f_mount(NULL,"0:",1);
	while(1)
	{
	}
	
}


