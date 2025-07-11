#include "stm32_eval_sdio_sd.h"
#include "SD_test.h"
#include "USART.h"


/* Private define ------------------------------------------------------------*/
#define BLOCK_SIZE            512 /* Block Size in Bytes */

#define NUMBER_OF_BLOCKS      10  /* For Multi Blocks operation (Read/Write) */
#define MULTI_BUFFER_SIZE    (BLOCK_SIZE * NUMBER_OF_BLOCKS)



/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t Buffer_Block_Tx[BLOCK_SIZE], Buffer_Block_Rx[BLOCK_SIZE];
uint8_t Buffer_MultiBlock_Tx[MULTI_BUFFER_SIZE], Buffer_MultiBlock_Rx[MULTI_BUFFER_SIZE];
volatile TestStatus EraseStatus = FAILED, TransferStatus1 = FAILED, TransferStatus2 = FAILED;
SD_Error Status = SD_OK;


static void SD_EraseTest(void);
static void SD_SingleBlockTest(void);
void SD_MultiBlockTest(void);
static void Fill_Buffer(uint8_t *pBuffer, uint32_t BufferLength, uint32_t Offset);
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);
static TestStatus eBuffercmp(uint8_t* pBuffer, uint32_t BufferLength);

void SD_Test(void)
{
	if((Status=SD_Init())!=SD_OK)
	{
		printf("SD����ʼ��ʧ�ܣ���ȷ��SD������ȷ���뿪���壬��һ��SD�����ԣ�\n");
	}
	else
	{
		 printf("SD����ʼ���ɹ���\n");	
	}
	
	if(Status==SD_OK)
	{
		 /*��������*/
		SD_EraseTest();
		/*single block ��д����*/
    SD_SingleBlockTest();
		
		 /*muti block ��д����*/
    SD_MultiBlockTest();
	}
	
}

 void SD_EraseTest(void)
 {
	 if(Status==SD_OK)
	 {
		 Status=SD_Erase(0X00,MULTI_BUFFER_SIZE);
		
	 }
	 if(Status==SD_OK)
	 {
		 //��������ɹ�����ô���᷵��0xFF,���Ƕ�����Ȼ����жԱȼ���
		 Status=SD_ReadMultiBlocks(Buffer_Block_Rx,0,BLOCK_SIZE,NUMBER_OF_BLOCKS);
		 //�ȴ�������æ��
		 Status=SD_WaitReadOperation();
		 //�ȴ�DMA�������
		 while(SD_GetStatus() != SD_TRANSFER_OK);
	 }
	 
	 if(Status==SD_OK)
	 {
		 EraseStatus=eBuffercmp(Buffer_Block_Rx,MULTI_BUFFER_SIZE);
	 }
	 if(EraseStatus==PASSED)
	 {
		  printf("SD���������Գɹ���\n");
	 }
	 else
  {
    printf("SD����������ʧ�ܣ�\n");
    printf("��ܰ��ʾ������SD����֧�ֲ������ԣ���SD����ͨ�������single��д���ԣ�����ʾSD���ܹ�����ʹ�á�\n");
  }
 }
 
 
 void SD_SingleBlockTest(void)
 {
	 //�����ݽ��������Ķ���Ȼ��д��ȥ�ٶ��������жԱȣ����һ�����д�����ɹ�
	 Fill_Buffer(Buffer_Block_Tx,BLOCK_SIZE,0x320F);
	 
	 if(Status==SD_OK)
	 {
		 //д������
		 Status=SD_WriteBlock(Buffer_Block_Tx,0x00,BLOCK_SIZE);
		 
		 //�ȴ�д����æ��
		 Status=SD_WaitWriteOperation();
		 
		//�ȴ�DMA�������
		 while(SD_GetStatus()!=SD_TRANSFER_OK);
		 
	 }
	 
	 if(Status==SD_OK)
	 {
		 //��������
		Status=SD_ReadBlock(Buffer_Block_Rx,0x00,BLOCK_SIZE);
		 
		 //�ȴ�������æ��
		 Status=SD_WaitReadOperation();
		 
		 //�ȴ�DMA�������
		 while(SD_GetStatus()!=SD_TRANSFER_OK);
		 
	 }
	 
	 if(Status==SD_OK)
	 {
		  TransferStatus1 = Buffercmp(Buffer_Block_Tx, Buffer_Block_Rx, BLOCK_SIZE);
	 }
	 
	  if(TransferStatus1 == PASSED)
  {
    printf("Single block ���Գɹ���\n");

  }
  else
  {
    printf("Single block ����ʧ�ܣ���ȷ��SD����ȷ���뿪���壬��һ��SD�����ԣ�\n");
  }
	
 }
 
 //���������Flash������д
 void SD_MultiBlockTest(void)
 {
	//�����ݽ��������Ķ���Ȼ��д��ȥ�ٶ��������жԱȣ����һ�����д�����ɹ�
	 Fill_Buffer(Buffer_Block_Tx,MULTI_BUFFER_SIZE,0x0);
	 
	 if(Status==SD_OK)
	 {
		 //д������
		Status=SD_WriteMultiBlocks(Buffer_Block_Tx,0x00,BLOCK_SIZE,NUMBER_OF_BLOCKS);
		 
		 //�ȴ�д����æ��
		Status=SD_WaitWriteOperation();
		 
		//�ȴ�DMA�������
		 while(SD_GetStatus()!=SD_TRANSFER_OK);
		 
	 }
	 
	 if(Status==SD_OK)
	 {
		 //��������
		 Status=SD_ReadMultiBlocks(Buffer_Block_Rx,0x00,BLOCK_SIZE,NUMBER_OF_BLOCKS);
		 
		 //�ȴ�������æ��
		 Status=SD_WaitReadOperation();
		 
		 //�ȴ�DMA�������
		 while(SD_GetStatus()!=SD_TRANSFER_OK);
		 
	 }
	 
	 if(Status==SD_OK)
	 {
		  TransferStatus2 = Buffercmp(Buffer_Block_Tx, Buffer_Block_Rx, MULTI_BUFFER_SIZE);
	 }
	 
	  if(TransferStatus2 == PASSED)
  {
    printf("Multi block ���Գɹ���\n");

  }
  else
  {
    printf("Multi block ����ʧ�ܣ���ȷ��SD����ȷ���뿪���壬��һ��SD�����ԣ�\n");
  }
	 
 }
 
 TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;
}

/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the Buffer to fill
  * @param  BufferLength: size of the buffer to fill
  * @param  Offset: first value to fill on the Buffer
  * @retval None
  */
void Fill_Buffer(uint8_t *pBuffer, uint32_t BufferLength, uint32_t Offset)
{
  uint16_t index = 0;

  /* Put in global buffer same values */
  for (index = 0; index < BufferLength; index++)
  {
    pBuffer[index] = index + Offset;
  }
}

/**
  * @brief  Checks if a buffer has all its values are equal to zero.
  * @param  pBuffer: buffer to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer values are zero
  *         FAILED: At least one value from pBuffer buffer is different from zero.
  */
TestStatus eBuffercmp(uint8_t* pBuffer, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    /* In some SD Cards the erased state is 0xFF, in others it's 0x00 */
    if ((*pBuffer != 0xFF) && (*pBuffer != 0x00))
    {
      return FAILED;
    }

    pBuffer++;
  }

  return PASSED;
}

/*********************************************END OF FILE**********************/
