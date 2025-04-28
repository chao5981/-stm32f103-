#ifndef __I2C_H
#define __I2C_H

#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>

#define EEPROM_I2Cx        				 		I2C1
#define EEPROM_I2Cx_CLK     			 		RCC_APB1Periph_I2C1
#define EEPROM_I2Cx_CLK_FUN 			 		RCC_APB1PeriphClockCmd
#define EEPROM_I2Cx_GPIO_CLK  		 		RCC_APB2Periph_GPIOB
#define EEPROM_I2Cx_GPIO_CLK_FUN  		RCC_APB2PeriphClockCmd
#define EEPROM_I2Cx_SCL_GPIO_PORT  	  GPIOB
#define EEPROM_I2Cx_SDA_GPIO_PORT  		GPIOB
#define EEPROM_I2Cx_SCL_GPIO_PIN   		GPIO_Pin_6
#define EEPROM_I2Cx_SDA_GPIO_PIN   		GPIO_Pin_7

/*等待超时时间*/
#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))


/*信息输出*/
#define EEPROM_DEBUG_ON         0

#define EEPROM_INFO(fmt,arg...)           printf("<<-EEPROM-INFO->> "fmt"\n",##arg)
#define EEPROM_ERROR(fmt,arg...)          printf("<<-EEPROM-ERROR->> "fmt"\n",##arg)
#define EEPROM_DEBUG(fmt,arg...)          do{\
                                          if(EEPROM_DEBUG_ON)\
                                          printf("<<-EEPROM-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

																					
																					
/* 
 * AT24C02 2kb = 2048bit = 2048/8 B = 256 B
 * 32 pages of 8 bytes each
 *
 * Device Address
 * 1 0 1 0 A2 A1 A0 R/W
 * 1 0 1 0 0  0  0  0 = 0XA0
 * 1 0 1 0 0  0  0  1 = 0XA1 
 */

/* EEPROM Addresses defines */
#define EEPROM_Block0_ADDRESS 0xA0   /* E2 = 0 */
//#define EEPROM_Block1_ADDRESS 0xA2 /* E2 = 0 */
//#define EEPROM_Block2_ADDRESS 0xA4 /* E2 = 0 */
//#define EEPROM_Block3_ADDRESS 0xA6 /* E2 = 0 */																					

/* STM32 I2C 快速模式 */																					
#define I2C_Speed              400000


/* 这个地址只要与STM32外挂的I2C器件地址不一样即可 */
#define I2Cx_OWN_ADDRESS7      0X0A   


/* AT24C01/02每页有8个字节 */
#define I2C_PageSize           8


void I2C_EEPROM_Init(void);
uint32_t I2C_EEPROM_ByteWrite(uint8_t *pBuffer,uint8_t WriteAddr);
uint32_t I2C_EEPROM_PageWrite(uint8_t *pBuffer,uint8_t WriteAddr,uint8_t NumByteToWrite);
void I2C_EEPROM_Buffer_Write(uint8_t *pBuffer,uint8_t WriteAddr,uint8_t NumByteToWrite);
uint32_t I2C_EEPROM_BufferRead(uint8_t *pBuffer,uint8_t ReadAddr,uint8_t NumByteToRead);
void I2C_EE_WaitEepromStandbyState(void);
void I2C_EEPROM_WriteDouble(u8 WriteAddr,double val);
double I2C_EEPROM_ReadDouble(u8 ReadAddr);
void I2C_EEPROM_WriteUInt16(u8 WriteAddr,u16 val);
u16 I2C_EEPROM_ReadUInt16(u8 ReadAddr);

#endif
