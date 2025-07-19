![image](https://github.com/user-attachments/assets/4a2734a6-7e2b-458e-8665-a5a384bc4d95)  

这一章的程序不需要自己写一遍以加深记忆，实际上，这种显示屏网上开源的很多，我们只要拿到一个相同型号的显示屏芯片的开源代码，学会移植并且能大致地看懂器代码含义就可以了。

  移植的时候大家只需要根据自己的实际情况修改引脚，修改NORSRAMx模块等即可.要修改需要到bsp_ili9341_lcd.h头文件中修改

  现在我讲俩个移植的时候大家大概率会碰到的情况
  
  首先是FSMC模拟8080时序的时候ADDSET和DATAST如何确定的问题。

  首先，通过对比我们可以发现，8080时序和FSMC的B模式非常相似，见下图：
  ![image](https://github.com/user-attachments/assets/4923ef0c-2094-4439-a0d8-3c3f570f5fb9)
  ![image](https://github.com/user-attachments/assets/5198a5e6-6173-44ef-a607-3a84a1c67d95)
  ![image](https://github.com/user-attachments/assets/4378708f-2275-4be5-9a7c-478dfabfcc88)


  A[25:0](地址线，在这里只需要任选一条地址线作为D/CX线即可)对应D/CX线(数据/命令选择线，低电平表示命令，高电平表示数据)
  
  NEx(片选信号)对应CSX(片选信号)
  
  NOE(读使能)对应RDX(读使能)
  
  NWE(写使能)对应WRX(写使能)
  
  D[15:0](数据线，这里都需要接上)对应D[17:0](数据线)，可以发现这是并行传输
  
  模式B的NADV的变化不用担心，直接不让他连着ILI9431的引脚就完事，这样就不会影响到8080时序。让它自个玩去吧
  
  现在我们需要配置ADDSET和DATAST这俩个值，根据我上一节说的，我们要找类似于t_SU(data)和t_SU(addr),但由于这里是模拟，所以找不到t_SU(data)和t_SU(addr)这样的字眼
  
  这时候我们就要对时序了，让我们看8080和FSMC的NOE和RDX;NWE和WRX。
  
  先看NOE和RDX，我们发现在FSMC模式B中，NOE是低电平，所以那应该是低电平有效;我们看8080时序，RDX也有一段低电平的时候，我们根据表中的时序，得到根据严格的时序要求的话，t_SU(addr)=trdlfm=355ns,计算出来ADDSET应该为25
  
  同理，再看NWE和WRX,我们发现在FSMC模式B中，NWE是低电平然后突然拉高电平延后一个HCLK周期，所以那应该是低电平加上一段高电平有效;我们看8080时序，WRX低电平对应的时间是t_SU(data)=twc=66ns,计算得出DATAST应该为4
  
  这样配置一定是没问题的，但是你的直觉告诉你，这个ADDSET是不是有问题，怎么能这么大一个？
  
  你的直觉是对的，确实太大了，实际上，读时序只需要65ns即可，这个有可能是芯片厂商的问题。毕竟前后延时 大约会有30ns的误差，但也不可能这么大。
  
  所以，如果实在不会看，或者遇到这种纯纯恶心人的怎么办？怕啥，直接枚举法试一试呗！反正它有不会损坏硬件，试就完了

  若这俩个值有变化，则到bsp_ili9341_lcd.c文件中的ILI9341_FSMC_Config函数中修改
  
  接下来讲移植的第二个要点，我们是用A[25:0]中的其中一根地址线去模拟D/CX线，但是这个引脚我们调成了复用模式，决定权已经给了FSMC，A16的拉高和拉低已经不能用GPIO寄存器进行控制了，这时候我们就只能通过操作地址的方式进行"硬拉"
  
  首先，我们用的是BANK1-BORSRAM1，有效地址范围是0X6000 0000 到 0X63FF FFFF，然后理论上，我们只要使得第16位设为1即可，于是做如下运算：
  
  0X6000 0000 |= (1«16) = 0x6001 0000位高电平;0X6000 0000 &= ~ (1«16) = 0x6000 0000为低电平
  
  但是不出意外的出意外了，这样子是有问题的。查阅参考手册我们可以知道，STM32 内部访问地址时使用的是内部 HADDR 总线，它是需要转换到外部存储器的内部 AHB 地址线，它是字节地址 (8 位)，而存储器访问不都是按字节访问，因此接到存储器的地址线依存储器的数据宽度有所不同。见下图:
![image](https://github.com/user-attachments/assets/4daa696e-29c1-4daf-b580-873c2fdf96dd)

  我们配置FSMC时用的是16位的数据访问方式，所以HADDR[0]未接，也就是说，所以 HADDR 与 FSMC_A 的地址线连接关系会左移一位，所以应该做如下运算:
  
  0X6000 0000 |= (1«(16+1)) = 0x6002 0000为高电平;0X6000 0000 &= ~ (1«(16+1)) = 0x6000 0000为低电平

  如果引脚用的不同，那仅仅是位次不同而已，例如你用的是A15地址线，那就是15位左移1位(16)位置1即可。

  移植就这俩点需要注意，其余的就自由发挥了，我在main函数中有一个示范，大家若不是很熟可以参考参考

  下面我介绍一下常用函数和宏定义，这样大家在移植我的代码的时候就可以不用去辛辛苦苦地翻.c文件了。

  关于宏定义：
  
  1.颜色的宏定义:在bsp_ili9341_lcd.h头文件
  ![image](https://github.com/user-attachments/assets/bbdc3070-1d0c-4430-9869-9b1b463cbe46)

  2.字体的大小设置:在fonts.c头文件
  ![image](https://github.com/user-attachments/assets/b790175d-c1be-475d-bbac-12985c55f175)

  3.显示屏的像素宽度
  ![image](https://github.com/user-attachments/assets/afb6ef45-4f02-4add-a903-a38ace6d510e)


  常用的配置函数:
  1.ILI9341_GramScan ( uint8_t ucOption )
  ![image](https://github.com/user-attachments/assets/f1a48911-28bc-4893-a0e6-ddcd05552d9c)

  2.LCD_SetFont(sFONT *fonts)
  ![image](https://github.com/user-attachments/assets/99db5224-e135-48d2-b0d5-c36dc717423c)
  其中如果包含中文字体，则必须选择Font8x16

  3.LCD_SetTextColor(uint16_t Color)
  ![image](https://github.com/user-attachments/assets/8d18f171-2581-4572-9559-2672d0d9b460)

  4.LCD_SetTextColor(uint16_t Color)
  ![image](https://github.com/user-attachments/assets/96b8f163-9a58-4366-9451-e811222be5d4)

  5.LCD_SetColors(uint16_t TextColor, uint16_t BackColor) 
  ![image](https://github.com/user-attachments/assets/d2a0b8a0-9f5e-40d0-901e-599112a49c5e)

  关于常用的绘图函数：
  1.ILI9341_SetCursor ( uint16_t usX, uint16_t usY )	
  ![image](https://github.com/user-attachments/assets/1c0b74a0-6d4a-496f-be0b-1a91a6a5f8f9)

  2.ILI9341_Clear ( uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight )
  ![image](https://github.com/user-attachments/assets/7729d961-fbd2-4238-acde-2a029980b499)

  3.ILI9341_DrawLine ( uint16_t usX1, uint16_t usY1, uint16_t usX2, uint16_t usY2 )
  ![image](https://github.com/user-attachments/assets/71224ce6-ba80-4282-a3e4-c9b236c23e1d)

  4.ILI9341_DrawRectangle ( uint16_t usX_Start, uint16_t usY_Start, uint16_t usWidth, uint16_t usHeight, uint8_t ucFilled )
  ![image](https://github.com/user-attachments/assets/c9979505-e622-4a1d-8e69-c160c7f065b1)

  5.ILI9341_DrawCircle ( uint16_t usX_Center, uint16_t usY_Center, uint16_t usRadius, uint8_t ucFilled )
  ![image](https://github.com/user-attachments/assets/16f1339d-804d-497a-92ed-102f9d5bd173)

  6.ILI9341_DispStringLine_EN_CH (  uint16_t line, char * pStr )
  ![image](https://github.com/user-attachments/assets/87c6c67b-b473-49ae-8f8c-19f1e08878f1)
  其中Line的宽度和汉字的一致

  7.ILI9341_DispString_EN_CH(	uint16_t usX, uint16_t usY,  char * pStr )
  ![image](https://github.com/user-attachments/assets/7c28ebf9-a5e8-43b0-84f5-be602600b601)



  若不熟悉如何调用这些函数，可以看一下我的main.c文件参考一下。当然，能不烧录就不烧录，因为画的太丑了......

   若要使用点阵屏，则需要调用ILI9341_Init初始化，然后设置字体格式即可。若要修改背景板和字体颜色，则须自行调用上述的函数修改

  如果要显示中文字体，则需要在font.c文件中使用flash的字模，这时候需要编写相应的函数和宏定义，定义如下：


    #if GBKCODE_FLASH
    
    #include "Flash.h"
    
    #else
    
    #include "./ff.h"
    
    #endif


    //选择使用FLASH字模还是SD卡的字模
    #if GBKCODE_FLASH
    
    //选择 1：GB2312字模
    //选择 0：HZLIB字模(旧版，不建议使用)
    #if 1
    
    /*使用FLASH字模*/
    //字模GB2312_H1616配套的函数
    
    //中文字库存储在FLASH的起始地址 ：
    //GBKCODE_START_ADDRESS 在fonts.h文件定义
    /**
      * @brief  获取FLASH中文显示字库数据
    	* @param  pBuffer:存储字库矩阵的缓冲区
    	* @param  c ： 要获取的文字
      * @retval None.
      */
    int GetGBKCode_from_EXFlash( uint8_t * pBuffer, uint16_t c)
    { 
        unsigned char High8bit,Low8bit;
        unsigned int pos;
    	
    		static uint8_t everRead=0;
    		
    		/*第一次使用，初始化FLASH*/
    		if(everRead == 0)
    		{
    			Flash_SPI_Init();
    			everRead = 1;
    		}
    	
    	  High8bit= c >> 8;     /* 取高8位数据 */
        Low8bit= c & 0x00FF;  /* 取低8位数据 */		
    	  	
    		/*GB2312 公式*/
        pos = ((High8bit-0xa1)*94+Low8bit-0xa1)*WIDTH_CH_CHAR*HEIGHT_CH_CHAR/8; 
    		Flash_SPI_BufferRead(pBuffer,GBKCODE_START_ADDRESS+pos,WIDTH_CH_CHAR*HEIGHT_CH_CHAR/8); //读取字库数据  
    //	  printf ( "%02x %02x %02x %02x\n", pBuffer[0],pBuffer[1],pBuffer[2],pBuffer[3]);
    	
    		return 0;  
         
    }
    
    #else //SD卡字模
    
    /*使用SD字模*/
    
    static FIL fnew;													/* file objects */
    static FATFS fs;													/* Work area (file system object) for logical drives */
    static FRESULT res_sd; 
    static UINT br;            					/* File R/W count */
    
    //字库文件存储位置，fonts.h中的宏：
    //#define GBKCODE_FILE_NAME			"0:/Font/GB2312_H2424.FON"
    
    /**
      * @brief  获取SD卡中文显示字库数据
    	* @param  pBuffer:存储字库矩阵的缓冲区
    	* @param  c ： 要获取的文字
      * @retval None.
      */
    int GetGBKCode_from_sd ( uint8_t * pBuffer, uint16_t c)
    { 
        unsigned char High8bit,Low8bit;
        unsigned int pos;
    		
    		static uint8_t everRead = 0;
    	
        High8bit= c >> 8;     /* 取高8位数据 */
        Low8bit= c & 0x00FF;  /* 取低8位数据 */
    		
        pos = ((High8bit-0xa1)*94+Low8bit-0xa1)*WIDTH_CH_CHAR*HEIGHT_CH_CHAR/8;
    	
    		/*第一次使用，挂载文件系统，初始化sd*/
    		if(everRead == 0)
    		{
    			res_sd = f_mount(&fs,"0:",1);
    			everRead = 1;
    
    		}
    		
        res_sd = f_open(&fnew , GBKCODE_FILE_NAME, FA_OPEN_EXISTING | FA_READ);
        
        if ( res_sd == FR_OK ) 
        {
            f_lseek (&fnew, pos);		//指针偏移
    			
    				//16*16大小的汉字 其字模 占用16*16/8个字节
            res_sd = f_read( &fnew, pBuffer, WIDTH_CH_CHAR*HEIGHT_CH_CHAR/8, &br );		 
            
            f_close(&fnew);
            
            return 0;  
        }    
        else
            return -1;    
    }
    
    #endif
