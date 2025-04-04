什么是位带操作？就是你可以直接对内存中的某一位进行赋值
就像是51单片机中的sbit，例如在51中，我们可以直接用sbit对一个引脚进行操作，例如sbit PB0=1.
位带操作其实很简单，只需要定义这三个函数即可
1.通过固定公式，把位带地址和为序号转化为地址转化为地址的宏，这样可以直接操作某个位的地址
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x02000000+((addr & 0x00FFFFFF)<<5)+(bitnum<<2)) 
2.把地址转化为指针，这样我们可以直接对地址所在的内存进行赋值
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
3.将上述俩行直接封装为一个函数，方便操作
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))
完成上述操作，只需要你按着要求填入参数，即可对你想要的引脚进行单独操作。
现在的问题是，在这个参数中要求填入这个寄存器的地址。怎么获取这个GPIOx和寄存器对应的地址呢？
首先，在stm32f10x.h(还是.c)头文件中，已经定义了GPIOx(x为从A~G)的基本地址，命名为GPIOx_BASE.
让我们打开STM32F10x—中文参考手册，找到属于GPIO的寄存器，你会发现一个叫做“地址偏移”的东西
![image](https://github.com/user-attachments/assets/70df9661-ad9e-4f7a-8f25-5e33a06b0fa9)
以GPIOB_BSRR为例，他的地址就是GPIOB的地址加上BSRR的地址偏移。(即GPIOB_BASE+0x10).
好，正常输入对应能写入寄存器的地址及其引脚，你就实现了单独控制一个引脚的功能。
