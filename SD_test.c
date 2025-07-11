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
		printf("SD卡初始化失败，请确保SD卡已正确接入开发板，或换一张SD卡测试！\n");
	}
	else
	{
		 printf("SD卡初始化成功！\n");	
	}
	
	if(Status==SD_OK)
	{
		 /*擦除测试*/
		SD_EraseTest();
		/*single block 读写测试*/
    SD_SingleBlockTest();
		
		 /*muti block 读写测试*/
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
		 //如果擦除成功，那么他会返回0xFF,我们读出来然后进行对比即可
		 Status=SD_ReadMultiBlocks(Buffer_Block_Rx,0,BLOCK_SIZE,NUMBER_OF_BLOCKS);
		 //等待读操作忙完
		 Status=SD_WaitReadOperation();
		 //等待DMA传输完毕
		 while(SD_GetStatus() != SD_TRANSFER_OK);
	 }
	 
	 if(Status==SD_OK)
	 {
		 EraseStatus=eBuffercmp(Buffer_Block_Rx,MULTI_BUFFER_SIZE);
	 }
	 if(EraseStatus==PASSED)
	 {
		  printf("SD卡擦除测试成功！\n");
	 }
	 else
  {
    printf("SD卡擦除测试失败！\n");
    printf("温馨提示：部分SD卡不支持擦除测试，若SD卡能通过下面的single读写测试，即表示SD卡能够正常使用。\n");
  }
 }
 
 
 void SD_SingleBlockTest(void)
 {
	 //将数据进行批量改动，然后写进去再读出来进行对比，如果一样则读写操作成功
	 Fill_Buffer(Buffer_Block_Tx,BLOCK_SIZE,0x320F);
	 
	 if(Status==SD_OK)
	 {
		 //写入数据
		 Status=SD_WriteBlock(Buffer_Block_Tx,0x00,BLOCK_SIZE);
		 
		 //等待写操作忙完
		 Status=SD_WaitWriteOperation();
		 
		//等待DMA传输完毕
		 while(SD_GetStatus()!=SD_TRANSFER_OK);
		 
	 }
	 
	 if(Status==SD_OK)
	 {
		 //读出数据
		Status=SD_ReadBlock(Buffer_Block_Rx,0x00,BLOCK_SIZE);
		 
		 //等待读操作忙完
		 Status=SD_WaitReadOperation();
		 
		 //等待DMA传输完毕
		 while(SD_GetStatus()!=SD_TRANSFER_OK);
		 
	 }
	 
	 if(Status==SD_OK)
	 {
		  TransferStatus1 = Buffercmp(Buffer_Block_Tx, Buffer_Block_Rx, BLOCK_SIZE);
	 }
	 
	  if(TransferStatus1 == PASSED)
  {
    printf("Single block 测试成功！\n");

  }
  else
  {
    printf("Single block 测试失败，请确保SD卡正确接入开发板，或换一张SD卡测试！\n");
  }
	
 }
 
 //这个类似于Flash的连续写
 void SD_MultiBlockTest(void)
 {
	//将数据进行批量改动，然后写进去再读出来进行对比，如果一样则读写操作成功
	 Fill_Buffer(Buffer_Block_Tx,MULTI_BUFFER_SIZE,0x0);
	 
	 if(Status==SD_OK)
	 {
		 //写入数据
		Status=SD_WriteMultiBlocks(Buffer_Block_Tx,0x00,BLOCK_SIZE,NUMBER_OF_BLOCKS);
		 
		 //等待写操作忙完
		Status=SD_WaitWriteOperation();
		 
		//等待DMA传输完毕
		 while(SD_GetStatus()!=SD_TRANSFER_OK);
		 
	 }
	 
	 if(Status==SD_OK)
	 {
		 //读出数据
		 Status=SD_ReadMultiBlocks(Buffer_Block_Rx,0x00,BLOCK_SIZE,NUMBER_OF_BLOCKS);
		 
		 //等待读操作忙完
		 Status=SD_WaitReadOperation();
		 
		 //等待DMA传输完毕
		 while(SD_GetStatus()!=SD_TRANSFER_OK);
		 
	 }
	 
	 if(Status==SD_OK)
	 {
		  TransferStatus2 = Buffercmp(Buffer_Block_Tx, Buffer_Block_Rx, MULTI_BUFFER_SIZE);
	 }
	 
	  if(TransferStatus2 == PASSED)
  {
    printf("Multi block 测试成功！\n");

  }
  else
  {
    printf("Multi block 测试失败，请确保SD卡正确接入开发板，或换一张SD卡测试！\n");
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
