#include "USART.h"
#include "stm32_eval_sdio_sd.h"
#include "ff.h"


/**
  ******************************************************************************
  *                              �������
  ******************************************************************************
  */
FATFS fs;													/* FatFs�ļ�ϵͳ���� */
FIL fnew;													/* �ļ����� */
FRESULT res_sd;                /* �ļ�������� */
UINT fnum;            					  /* �ļ��ɹ���д���� */
BYTE ReadBuffer[1024]={0};        /* �������� */
BYTE WriteBuffer[] =              /* д������*/
"\r\n��ó���\r\n";  
FILINFO fno;
DIR dir;
extern  SD_CardInfo SDCardInfo;

int main(void)
{
	USART_Configuration();
	printf("\r\n****** ����һ��SD�� �ļ�ϵͳʵ�� ******\r\n");
	res_sd=f_mount(&fs,"0:",1);
	printf("****");
	if(res_sd==FR_NO_FILESYSTEM)
	{
		printf("��SD����û���ļ�ϵͳ���������и�ʽ��...\r\n");
		res_sd=f_mkfs("0:",0,0);
		//��ʽ���ļ�����Ҫ���¹���
		if(res_sd==FR_OK)
		{
				printf("��SD���ѳɹ���ʽ���ļ�ϵͳ��\r\n");
			//ȡ������
				f_mount(NULL,"0:",1);
			//���¹���
				f_mount(&fs,"0:",1);
				
		}
		else
		{
			printf("����SD�������ļ�ϵͳʧ�ܡ�(%d)\r\n",res_sd);
			printf("��������ԭ��SD����ʼ�����ɹ���\r\n");
			while(1);
		}
		
	}
	if(res_sd==FR_OK)
	{
		printf("\r\nSD�����سɹ���r\n");
	}
	else
	{
		printf("\r\nSD������ʧ�ܣ�\r\n");
		while(1);
	}
	/*----------------------- �ļ�ϵͳ���ԣ�д���� -----------------------------*/
	/* ���ļ�������ļ��������򴴽��� */
	printf("\r\n****** ���������ļ�д�����... ******\r\n");	
	res_sd=f_open(&fnew,"0:�����볬��.txt",FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
	if(res_sd==FR_OK)
	{
		printf("����/���������볬��.txt�ļ��ɹ������ļ�д�����ݡ�\r\n");
		res_sd=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
		if(res_sd==FR_OK)
		{
			printf("���ļ�д��ɹ���д���ֽ����ݣ�%d\n",fnum);
      printf("�����ļ�д�������Ϊ��\r\n%s\r\n",WriteBuffer);
		}
		else
		{
			printf("�����ļ�д��ʧ�ܣ�(%d)\n",res_sd);
		}
		//�ر��ļ�
		f_close(&fnew);
	}
	else
	{	
		printf("������/�����ļ�ʧ�ܡ�\r\n");
	}
	
	/*------------------- �ļ�ϵͳ���ԣ������� ------------------------------------*/
	printf("****** ���������ļ���ȡ����... ******\r\n");
	res_sd=f_open(&fnew,"0:�����볬��.txt",FA_READ|FA_OPEN_EXISTING);
	if(res_sd==FR_OK)
	{
			printf("�����ļ��ɹ���\r\n");
			res_sd=f_read(&fnew,ReadBuffer,sizeof(ReadBuffer),&fnum);
			if(res_sd==FR_OK)
			{
					 printf("���ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n",fnum);
					 printf("����ȡ�õ��ļ�����Ϊ��\r\n%s \r\n", ReadBuffer);	
			}
			else
			{
					 printf("�����ļ���ȡʧ�ܣ�(%d)\n",res_sd);
			}		
			
	}
	else
	{	
		printf("������/�����ļ�ʧ�ܡ�\r\n");
	}
	//�ر��ļ�
	f_close(&fnew);
	
	/*----------------------- �ļ�ϵͳ���ԣ�������Ŀ¼���� -----------------------------*/
	
	//��Ŀ¼�����û��Ŀ¼�ʹ���Ŀ¼
	//�������ԣ��������е�f_stat��ֱ�ӿ��������Բ�������
	printf("\r\n׼����ȡ�ļ���\r\n");
	res_sd=	f_opendir(&dir,"0:/LC_SDIO_Test002");
	printf("\r\n��ȡ����ļ���r\n");
	if(res_sd==FR_OK)
	{
		printf("\r\n0:/LC_SDIO_Test002�ļ��д��ڣ�\r\n");
	}
	else if(res_sd!=FR_OK)
	{
		res_sd=f_mkdir("0:/LC_SDIO_Test002");
		if(res_sd==FR_OK)
		{
			printf("\r\n�����ļ���0:/LC_SDIO_Test002�ɹ�\r\n");
		}
		else
		{
			printf("\r\n!!�����ļ���0:/LC_SDIO_Test002ʧ��\r\n");
			while(1);
		}
	}
	printf("\r\n�ļ���׼�����!\r\n");
		//�����ļ�
		res_sd=f_open(&fnew,"0:/LC_SDIO_Test002/stm32f103_LC001.txt",FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
		if(res_sd==FR_OK)
		{
			printf("����/����0:/LC_SDIO_Test002/stm32f103_LC001.txt�ļ��ɹ������ļ�д�����ݡ�\r\n");
			res_sd=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
			if(res_sd==FR_OK)
			{
				printf("���ļ�д��ɹ���д���ֽ����ݣ�%d\n",fnum);
				printf("�����ļ�д�������Ϊ��\r\n%s\r\n",WriteBuffer);
			}
			else
			{
				printf("�����ļ�д��ʧ�ܣ�(%d)\n",res_sd);
			}
			//�ر��ļ�
			f_close(&fnew);
			f_closedir(&dir);
		}
		else
		{	
			printf("������/�����ļ�ʧ�ܡ�\r\n");
		}
		
		/*******************�ļ���д/��ʽ��д�����***********************/
		printf("\r\n׼�����ļ������ļ���дʵ��\r\n");
		//���ļ���
		res_sd=	f_opendir(&dir,"0:/LC_SDIO_Test002");
		printf("��Ŀ¼���: %d\n", res_sd); 
		if(res_sd==FR_OK)
		{
			printf("\r\n�ļ���0:/LC_SDIO_Test002�򿪳ɹ�\r\n");
		}
		else
		{
			printf("\r\n�ļ���0:/LC_SDIO_Test002��ʧ��\r\n");
			while(1);
		}
		res_sd=f_open(&fnew,"0:/LC_SDIO_Test002/stm32f103_LC001.txt",FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
		printf("���ļ����: %d\n", res_sd); 
		if(res_sd==FR_OK)
		{
			printf("\r\n�ļ��򿪳ɹ�,׼�����ļ���׷������\r\n");
			res_sd=f_lseek(&fnew,f_size(&fnew));
			if(res_sd==FR_OK)
			{
					f_printf(&fnew,"�����Ǹ�������\n");
					res_sd=f_lseek(&fnew,0);
					res_sd = f_read(&fnew,ReadBuffer,f_size(&fnew),&fnum);
				if(res_sd == FR_OK)
				{
					printf("��0:/LC_SDIO_Test002/stm32f103_LC001.txt�ļ����ݣ�\n%s\n",ReadBuffer);
				}
			}
			//�ر��ļ�
			f_close(&fnew);
			//�ر��ļ���
			f_closedir(&dir);
		}
		else
		{
			printf("\r\n�ļ���ʧ��\r\n");
		}
		
		/*******************�ļ�ɾ������***********************/
		printf("\r\n׼��ɾ���ļ�\r\n");
		res_sd=f_unlink("0:�����볬��.txt");
		if(res_sd==FR_OK)
		{
			printf("\r\n0:�����볬��.txt�ļ�ɾ���ɹ�\r\n");
		}
		else
		{
			printf("\r\n�ļ�ɾ��ʧ��\r\n");
		}
		
		
		/****************�ļ���������**********************/
		printf("\r\n׼�������ļ���������\r\n");
		res_sd=f_rename("0:/LC_SDIO_Test001/stm32f103_LC001.txt","0:/LC_SDIO_Test001/stm32f103_LC001_rename_test.txt");
		if(res_sd==FR_OK)
		{
			printf("\r\n0:/LC_SDIO_Test001/stm32f103_LC001.txt�����ɹ���\r\n");
		}
		else
		{
			printf("\r\n0:/LC_SDIO_Test001/stm32f103_LC001.txt����ʧ�ܣ�\r\n");
		}
		
	//�������ȡ������
	f_mount(NULL,"0:",1);
	while(1)
	{
	}
	
}


