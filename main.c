#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "GCB.h"
// 把“位带地址+位序号”转换成别名地址的宏(即可直接访问某个位的地址)
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x02000000+((addr & 0x00FFFFFF)<<5)+(bitnum<<2)) 
// 把一个地址转换成一个指针，让你可以直接访问变量一样直接操作内存的值
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
// 把位带别名区地址转换成指针，把某个寄存器的某一位转换成可直接访问的地址，达到操作某个位的目的
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))

// GPIO ODR 和 IDR 寄存器地址映射 
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C   
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C   
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C   
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C   
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C   
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C      
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C      
  
#define GPIOA_IDR_Addr    (GPIOA_BASE+8)  //0x40010808   
#define GPIOB_IDR_Addr    (GPIOB_BASE+8)  //0x40010C08   
#define GPIOC_IDR_Addr    (GPIOC_BASE+8)  //0x40011008   
#define GPIOD_IDR_Addr    (GPIOD_BASE+8)  //0x40011408   
#define GPIOE_IDR_Addr    (GPIOE_BASE+8)  //0x40011808   
#define GPIOF_IDR_Addr    (GPIOF_BASE+8)  //0x40011A08   
#define GPIOG_IDR_Addr    (GPIOG_BASE+8)  //0x40011E08 

//GPIO BSRR寄存器地址映射
#define GPIO_A_BSRR_Addr  (GPIOA_BASE+10)
#define GPIO_B_BSRR_Addr  (GPIOB_BASE+10)
#define GPIO_C_BSRR_Addr  (GPIOC_BASE+10)
#define GPIO_D_BSRR_Addr  (GPIOD_BASE+10)
#define GPIO_E_BSRR_Addr  (GPIOE_BASE+10)
#define GPIO_F_BSRR_Addr  (GPIOF_BASE+10)
#define GPIO_G_BSRR_Addr  (GPIOG_BASE+10)


#define PBout(n)  BIT_ADDR(GPIOB_ODR_Addr,n)
#define PBin(n)  BIT_ADDR(GPIOB_ODR_Addr,n)

void Delay(__IO uint32_t ncount);

int main(void)
{
	LED_GPIO_Init();
		while(1)
		{
			PBout(0)=0;
			Delay(0x0FFFFF);
			PBin(0)=1;
			Delay(0x0FFFFF);
			
			PBout(5)=0;
			Delay(0x0FFFFF);
			PBin(5)=1;
			Delay(0x0FFFFF);
			
			PBout(1)=0;
			Delay(0x0FFFFF);
			PBin(1)=1;
			Delay(0x0FFFFF);
		}
}
void Delay(__IO uint32_t ncount)
{
	for(;ncount>0;ncount--);
}
// 来到这里的时候，系统的时钟已经被配置成72M。
