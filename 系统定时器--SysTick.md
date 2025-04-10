在程序中，我们时常需要用到Delay函数进行延时处理，但是如果用的单纯是普通的软件延时函数(即含有for循环的)，那么CPU每时每刻都在访问寄存器和轮询判断位，这样CPU就被高度占用。
但是如果我们使用中断来进行计时操作，然后让主函数仅仅是干等，这样能很大的减少CPU的占用

在stm32f103中，有一个系统计时器(SysTick),内嵌在 NVIC 中。系统定时器是一个24bit的向下递减的计数器，计数器每计数一次的时间为1/SYSCLK，一般我们设置系统时钟SYSCLK等于72M
当它减到0时就进入一次中断，并让他再次回到开始的计时值，减到0时就再进入一次中断，如此循环。因此，我们可以用它写一个中断程序，来代替Delay函数。

如何写呢，先配置SysTick,用SysTick_Config()函数配置SysTick的频率，配置其多少时间进入一次中断;在主函数调用这个Delay函数，传入要延时的时间TimingDelay，当TimingDelay不等于0时就让CPU忙等;并且写一个递减函数TimingDelay_Reduction()，在中断函数中调用TimingDelay_Reduction()，进入一次中断就自减一次。这样就能实现延时的效果。

这里有几个注意点：
1.SysTick是嵌入在NVIC中的，可以认为是"内部中断"，所以不需要配置EXTI寄存器。并且，这个SysTick的函数存在core_CM3.h这个头文件中，厂家已经帮你配置好了，只需要调用这个函数即可。

2.在配置SysTick_Config()的频率时见下图
![image](https://github.com/user-attachments/assets/ea1b4b7b-3781-479c-bd14-443f284d2b28)

3.在用SysTick计时时，记得把它开启。代码为：SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk

然后就没有什么需要注意的点了，若有不是很理解的，可以看我的编写的函数。

下面简述可能对大家更有用的。

如果利用中断和SysTick进行计时，的确是低CPU占用了，但是如果是短时间的延时，那么误差会很大，因为每一个函数的进入和出去都会消耗时间。

如果不用中断，只是用SysTick,那么时间将会非常精确，但是CPU占用率很高。

如果你是单纯的只是需要延时，那么这俩个就凑合够用了，但是我是需要在不同时间内控制不同的外设呢？

这时候我们需要用SysTick实现定时回调或时间戳。即不是用它去“等一段时间”，而是用它去“记录时间的流逝”，这样你就可以自己判断“什么时候做某件事”——而不是卡在那等。

这里我用AI跑了一个，大家仅供参考，到后面有需要的话，我再进行特定的编写。

在中断函数：
uint32_t system_tick = 0;
void SysTick_Handler(void)
{
    system_tick++;  // 每 1ms 进入中断一次，就+1，单位是毫秒
}

在自己的头文件中:
void led_blink_logic(void)
{
    static uint32_t last = 0;
    if(system_tick - last >= 500)  // 每500ms执行一次
    {
        Toggle_LED();
        last = system_tick;
    }
}

在头文件中:
int main(void)
{
    SysTick_Config(SystemCoreClock / 1000);  // 每1ms触发一次SysTick_Handler

    while(1)
    {
        led_blink_logic();  // 不是延时，而是判断是否“该闪了”
        //其他的任务照样可以跑
    }
}


