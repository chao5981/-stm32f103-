#include "application.h"

#define Rectangle_Width  30
#define Rectangle_Height 16

#define NO_Filled    0
#define Filled       1

#define A_start_X   30
#define A_start_Y   50

#define B_start_X   30
#define B_start_Y   80

#define C_start_X   30
#define C_start_Y   110

#define D_start_X   30
#define D_start_Y   140

void Application_Init(void)
{
	Touch_Button_Init();
}

void Touch_Button_Init(void)
{
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
	LCD_SetColors(BLUE,WHITE);
	ILI9341_DispString_EN_CH(50,0,"请回答以下问题:");
	ILI9341_DispString_EN_CH(50,20,"BJUT的湖的名字叫做？");
	ILI9341_DispString_EN_CH(70,50,"A.月明湖");
	LCD_SetTextColor(BLUE);
	ILI9341_DrawRectangle(A_start_X,A_start_Y,Rectangle_Width,Rectangle_Height,NO_Filled);
	ILI9341_DrawRectangle(B_start_X,B_start_Y,Rectangle_Width,Rectangle_Height,NO_Filled);
	ILI9341_DrawRectangle(C_start_X,C_start_Y,Rectangle_Width,Rectangle_Height,NO_Filled);
	ILI9341_DrawRectangle(D_start_X,D_start_Y,Rectangle_Width,Rectangle_Height,NO_Filled);
	LCD_SetColors(BLUE,WHITE);
	ILI9341_DispString_EN_CH(70,80,"B.月亮湖");
	ILI9341_DispString_EN_CH(70,110,"C.太阳湖");
	ILI9341_DispString_EN_CH(70,140,"D.半月湖");
}

void Touch_Button_Up(uint16_t x,uint16_t y)
{
	if(x>=A_start_X&&x<=A_start_X+Rectangle_Width&&y>=A_start_Y+Rectangle_Height&&y<=A_start_Y+2*Rectangle_Height)
	{
		LCD_SetColors(BLUE,WHITE);
		ILI9341_DispString_EN_CH(50,200,"回答正确!");
	}
	else if(x>=B_start_X&&x<=B_start_X+Rectangle_Width&&y>=B_start_Y+Rectangle_Height&&y<=B_start_Y+2*Rectangle_Height)
	{
		LCD_SetColors(BLUE,WHITE);
		ILI9341_DispString_EN_CH(50,200,"回答错误!");
		Application_Delay(3000000);
		Application_Init();
	}
	else if(x>=C_start_X&&x<=C_start_X+Rectangle_Width&&y>=C_start_Y+Rectangle_Height&&y<=C_start_Y+2*Rectangle_Height)
	{
		LCD_SetColors(BLUE,WHITE);
		ILI9341_DispString_EN_CH(50,200,"回答错误!");
		Application_Delay(3000000);
		Application_Init();
	}
	else if(x>=D_start_X&&x<=D_start_X+Rectangle_Width&&y>=D_start_Y+Rectangle_Height&&y<=D_start_Y+2*Rectangle_Height)
	{
		LCD_SetColors(BLUE,WHITE);
		ILI9341_DispString_EN_CH(50,200,"回答错误!");
		Application_Delay(3000000);
		Application_Init();
	}
	else
	{
		LCD_SetColors(WHITE,BLUE);
		ILI9341_DispString_EN_CH(50,200,"无效区域!");
	}
}

void Application_Delay(__IO u32 ncount)
{
	for(;ncount>0;ncount--)
	{
	}
}
