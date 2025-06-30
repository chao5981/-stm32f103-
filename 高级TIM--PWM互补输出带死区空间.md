  高级TIM所谓高级，就是它除了能实现基本的定时功能以外，还能测量脉宽和频率，以及生成PWM信号。

  这一节我先讲解高级TIM生成PWM信号的基本原理。

  先附上一张高级控制定时器框图
  ![image](https://github.com/user-attachments/assets/88909f36-0a20-449c-979c-4c4cdebf05fc)

  严重怀疑这张图就是防止初学者看懂......

  我按我自己的方式讲出来。

  首先，51单片机玩过吧，还记得51单片机是如何控制引脚输出PWM信号实现呼吸灯的吗？

  不记得了的话我附上我当时写的51单片机的PWM程序

      #include<REGX52.H>
      #include"time.h"
      sbit Motor=P1^0;
      unsigned char Compare;
      void main()
      {
      	unsigned char Speed=0;
      	unsigned char Key=0;
      	time0_Init();
      	while(1)
      	{
      		Key=Keyboard();
      		if(Key)
      		{
      			Speed++;
      			Speed%=4;
      			if(Speed==0)
      			{
      				Compare=0;
      			}
      			if(Speed==1)
      			{
      				Compare=50;
      			}
      			if(Speed==2)
      			{
      				Compare=75;
      			}
      			if(Speed==3)
      			{
      				Compare=100;
      			}
      			
      		}
      		Nixie_Set(1,Speed);
      	}
      }
      
      void Time0()  interrupt 3
      {
      	static unsigned int count1=0,count2=0,count3=0;
      	TL0 = 0x9C;		//设置定时初值
      	TH0 = 0xFF;
      	count1++;
      	count2++;
      	count3++;
      	count3%=100;
      	if(count3<Compare)
      	{
      		Motor=1;
      	}
      	else
      	{
      		Motor=0 ;
      	}
      }


  你看，我是令了P1^0引脚为Motor，作为输出PWM信号的引脚。然后我设定了一个定时器，让定时器每隔一段时间就进入一次中断，让count3++;然后当count3满足一定条件后将P1^0翻转以模拟PWM信号。
  
  所以，51单片机生成PWM的信号的方式是软件手动调整的！而stm32的高级TIM就高级在这里，我完全可以不需要手动，我直接通过硬件就可以生成出PWM信号，而且还更加精准！

  
