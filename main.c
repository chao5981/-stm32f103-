#include "stm32f10x.h"   
#include "bsp_ili9341_lcd.h"
#include "USART.h"

static void LCD_Show(void);
static void Delay(__IO u32 nCount);


int main(void)
{
	ILI9341_Init();
	USART_Configuration();
	printf("这是一个液晶屏显示中英文和常见图像的移植实验\n");
	printf("实验开始\n");
	ILI9341_GramScan(6)	;
	LCD_SetFont(&Font8x16);
	LCD_SetColors(BLUE,WHITE);
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
	
	while(1)
	{
		LCD_Show();
	}
}

static void LCD_Show(void)
{
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
	LCD_SetTextColor(BLUE);
	ILI9341_DrawCircle(115,90,80,1);
	LCD_SetTextColor(WHITE);
	ILI9341_DrawCircle(115,90,75,1);
	LCD_SetTextColor(BLUE);
	ILI9341_DrawCircle(115,90,70,1);
	LCD_SetTextColor(WHITE);
	ILI9341_DrawRectangle(90,50,50,8,1);
	ILI9341_DrawRectangle(90,80,50,8,1);
	ILI9341_DrawRectangle(90,110,50,8,1);
	ILI9341_DrawRectangle(140,50,10,68,1);
	ILI9341_DrawRectangle(100,60,40,3,1);
	ILI9341_DrawRectangle(100,68,25,6,1);
	ILI9341_DrawRectangle(100,97,25,6,1);
	
	LCD_SetColors(WHITE,BLUE);
	ILI9341_DispString_EN(100,130,"1960");
	LCD_SetColors(BLUE,WHITE);
	ILI9341_DispString_CH(50,200,"爱北京");
	ILI9341_DispString_CH(130,200,"来工大");
	ILI9341_DispString_CH(66,225,"北京工业大学");
	ILI9341_DispString_EN(100,250,"BJUT");
	Delay(180000000);
}


static void Delay (__IO u32 nCount)
{
  for ( ; nCount != 0; nCount -- );
	
}
