#ifndef __FLASH_H
#define __FLASH_H

#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>

//#define  sFLASH_ID              0xEF3015   //W25X16
//#define  sFLASH_ID              0xEF4015	 //W25Q16
//#define  sFLASH_ID              0XEF4018   //W25Q128
#define  sFLASH_ID              0XEF4017    //W25Q64

#define SPI_FLASH_PageSize              256
#define SPI_FLASH_PerWritePageSize      256
#define FLASH_USER_SECTOR_START  				0x000000
#define FLASH_USER_SECTOR_SIZE   				4096		

/*命令定义-开头*******************************/
#define W25X_WriteEnable		      0x06 
#define W25X_WriteDisable		      0x04 
#define W25X_ReadStatusReg		    0x05 
#define W25X_WriteStatusReg		    0x01 
#define W25X_ReadData			        0x03 
#define W25X_FastReadData		      0x0B 
#define W25X_FastReadDual		      0x3B 
#define W25X_PageProgram		      0x02 
#define W25X_BlockErase			      0xD8 
#define W25X_SectorErase		      0x20 
#define W25X_ChipErase			      0xC7 
#define W25X_PowerDown			      0xB9 
#define W25X_ReleasePowerDown	    0xAB 
#define W25X_DeviceID			        0xAB 
#define W25X_ManufactDeviceID   	0x90 
#define W25X_JedecDeviceID		    0x9F

/* WIP(busy)标志，FLASH内部正在写入 */
#define WIP_Flag                  0x01
#define Dummy_Byte                0xFF
/*命令定义-结尾*******************************/


/*SPI接口定义-开头****************************/
#define      FLASH_SPIx                        SPI1
#define      FLASH_SPI_APBxClock_FUN          RCC_APB2PeriphClockCmd
#define      FLASH_SPI_CLK                     RCC_APB2Periph_SPI1

//CS(NSS)引脚 片选选普通GPIO即可
#define      FLASH_SPI_CS_APBxClock_FUN       	RCC_APB2PeriphClockCmd
#define      FLASH_SPI_CS_CLK                  RCC_APB2Periph_GPIOC    
#define      FLASH_SPI_CS_PORT                 GPIOC
#define      FLASH_SPI_CS_PIN                  GPIO_Pin_0

//SCK引脚
#define      FLASH_SPI_SCK_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define      FLASH_SPI_SCK_CLK                 RCC_APB2Periph_GPIOA   
#define      FLASH_SPI_SCK_PORT                GPIOA   
#define      FLASH_SPI_SCK_PIN                 GPIO_Pin_5
//MISO引脚
#define      FLASH_SPI_MISO_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define      FLASH_SPI_MISO_CLK                RCC_APB2Periph_GPIOA    
#define      FLASH_SPI_MISO_PORT               GPIOA 
#define      FLASH_SPI_MISO_PIN                GPIO_Pin_6
//MOSI引脚
#define      FLASH_SPI_MOSI_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define      FLASH_SPI_MOSI_CLK                RCC_APB2Periph_GPIOA    
#define      FLASH_SPI_MOSI_PORT               GPIOA 
#define      FLASH_SPI_MOSI_PIN                GPIO_Pin_7

#define  		SPI_FLASH_CS_LOW()     						GPIO_ResetBits( FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN )
#define  		SPI_FLASH_CS_HIGH()    						GPIO_SetBits( FLASH_SPI_CS_PORT, FLASH_SPI_CS_PIN )

/*SPI接口定义-结尾****************************/


/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

/*信息输出*/
#define FLASH_DEBUG_ON         1

#define FLASH_INFO(fmt,arg...)           printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define FLASH_ERROR(fmt,arg...)          printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
#define FLASH_DEBUG(fmt,arg...)          do{\
                                          if(FLASH_DEBUG_ON)\
                                          printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

//**********外部调用函数************																		
void Flash_SPI_Init(void);
u32 Flash_SPI_ReadDeviceID(void);
u32 Flash_SPI_ReadID(void);
void Flash_SPI_SectorErase(u32 SectorAddr);
void Flash_SPI_BulkErase(void);
void Flash_SPI_BufferWrite(u8 *pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void Flash_SPI_BufferRead(u8 *pBuffer,u32 ReadAddr,u16 NumByteToRead);
void Flash_SPI_PowerDown(void);																				
void Flash_SPI_WAKEUP(void);
void Flash_SPI_WriteDouble(double DoubleData,u32 WriteDoubleAddr);
double Flash_SPI_ReadDouble(u32 ReadDoubleAddr);
void Flash_SPI_WriteU16(u16 U16Data,u32 WriteU16Addr);
u16 Flash_SPI_ReadU16(u32 ReadU16Addr);
void Flash_SPI_WriteAny(u32 addr, void* data, size_t len);																					
void Flash_SPI_ReadAny(u32 addr, void* data, size_t len);


//*********内部调用函数*************
u8 Flash_SPI_SendByte(u8 Byte);
u8 Flash_SPI_ReadByte(void);
void Flash_SPI_WriteEnable(void);
void Flash_SPI_WaitForWriteOrEraseEnd(void);																					
																					
#endif

