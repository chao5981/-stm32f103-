再stm32f103中，中断可以被分为俩种，一种是内部中断，一种是外部中断。

这一节我们只讲外部中断。

外部中断的实现需要俩个控制器共同实现，一个是NVIC(嵌套向量中断控制器);一个是EXTI(外部中断/事件控制器)。

这俩个是如何工作的呢，简单来说，NVIC是用来配置中断的优先级，并确定哪个中断先执行的。就像是51单片机中调用中断时的"interrupt x"中的"x";而EXTI是负责检测外部引脚的电平变化（如按键、GPIO 边沿触发），并生成中断请求信号给NVIC。

后期补充：正如上面所说，EXTI是用于外部中断的，而内部中断(如TIM、USART、ADC等)是不需要配置EXTI的，只需要配置NVIC结构体即可。

但是NVIC可绝没有C51中的"interrupt x"那么简单。在C51中，我们是根据"interrupt x"中的'x'去判断哪个中断优先;但是在NVIC中，他是把整个优先级分成了"抢占优先级"和"子优先级"俩个优先级。那么如何看哪个中断优先呢？抢占优先级高的就会抢占抢占优先级低的优先得到执行，如果抢占优先级相同，就比较子优先级。如果抢占优先级和子优先级都相同的话，就比较他们的硬件中断编号，编号越小，优先级越高。

让我们先看看NVIC的相关寄存器。
![image](https://github.com/user-attachments/assets/2248c2b6-d642-4fdb-aadc-da50abd4c28f)
你现在所看到的就是配置优先级的寄存器，正常的应该有16位的，但是绝大多数CM3芯片都会精简设计，以致实际上支持的优先级数减少，在F103中，只使用了高4bit。

那可是这个寄存器可不会告诉计算机哪个部分是抢占优先级哪个部分是子优先级。所以这里的分组需要我们自己调用库函数去告诉计算机

那么我们配置之后，优先级和子优先级的范围是多少？请见下图：
![image](https://github.com/user-attachments/assets/82b5ed57-5988-446d-816f-d338e4cdfc9d)


EXTI控制器的原理图如图所示。
![image](https://github.com/user-attachments/assets/2ca12bfc-abe3-4ee4-9ca7-9b3723444e02)
红色的是中断线，我先讲解中断线。


1.输入线连接上一个外设(例如按键等)，外设电平的变化会被输入线检测到并且传递给边沿检测电路

2.如图所示，边沿检测电路中连接着俩个寄存器上升沿触发选择寄存器 (EXTI_RTSR) 和下降沿触发选择寄存器 (EXTI_FTSR),你可以配置这个边沿检测电路选择其中一个寄存器或者俩个寄存器都选。只有来自输入线上的电平变化和你选的寄存器的电平变化一致时，才会输出有效信号。例如：你要求当按键的电平从0变化到1才能触发中断，当输入线传来的引脚信号是从1变成了0，和边沿检测电路的不一致，那么就发出无效信号，若传来的信号是从0到1，和边沿检测电路的信号一致，它才会向后面发出有效信号。

3.一个或门电路，连接着软件中断事件寄存器(EXTI_SWIER)，你可以配置使得中断的是否开启，就和C51中的中断开关类似

4.一个与门电路，连接着中断屏蔽寄存器(EXTI_IMR),和软件中断事件寄存器的作用类似，只是反了过来而已

5.将EXTI_PR寄存器内容输出到NVIC内，从而实现系统中断事件控制。

接下来我们来看事件线。

事件线的前三步和中断线一致，我们这里就不做赘述

4.也就是编号6：一个与门电路，连接着事件屏蔽寄存器(EXTI_EMR)，类似于中断屏蔽计时器。

5.也就是编号7：一个脉冲发生器，当传来有效信号1时，就会产生一个脉冲信号，这个脉冲信号可以给其他外设电路使用，比如定时器TIM、模拟数字转换器ADC等等。

至此，EXTI和NVIC的工作原理我们全部介绍完毕。我们整个队。

GPIO的输入引脚检测到电平的变化，并和EXTI的边沿检测电路进行对比，当检测到你设置的触发条件，EXTI会认为这是一次有效的中断触发，并置位对应的挂起标志（EXTI_PR寄存器中的位会被置1），也就是“记录”中断发生了，还没有真正执行中断服务程序(NVIC).然后EXTI 把中断“申请”交给了NVIC。NVIC再检查你配置的这个中断线（比如 EXTI0_IRQn）是否已启用？是否结构体初始化NVIC_Init()？是否NVIC_IQNchannelcmd是否ENABLE? NVIC再根据当前是否有别的中断在执行、优先级是否更高，来决定马上进入中断服务程序（ISR），还是等其他更高优先级中断执行完再处理。
![image](https://github.com/user-attachments/assets/ef1b8579-4398-4b93-aa09-016d1ab699f6)


下面我们开始利用中断编写一个程序--按下按键即为中断，在中断函数中使得LED灯的颜发生变化。

首先，配置NVIC及其相关寄存器

1.确定抢占优先级和子优先级的分组
![image](https://github.com/user-attachments/assets/eed9ed0b-595f-48f4-bb11-04d7eda5f0ea)

2.初始化结构体对里面的成员进行配置
![image](https://github.com/user-attachments/assets/9aaa30cb-3652-4343-8368-9a134bc95ba4)
这里我相信大家NVIC_IRQChannelCmd(启用或者禁止中断通道)，NVIC_IRQChannelPreemptionPriority(配置抢占优先级大小)和NVIC_IRQChannelSubPriority(配置子优先级的大小)大家应该都没有问题，最大的问题可能是NVIC_IRQChannel(配置哪个要中断通道)，这里我着重讲一下这个。

NVIC_IRQChannel：指定配置的中断源，但是我们打开枚举值，发现根本没有对应的GPIO接口啊
![image](https://github.com/user-attachments/assets/af1004fc-ba91-4598-8aee-10dfd2e55bee)

后期添加：这里值得一说的是，中断源没有这么少。我们打开stm32f10x.h头文件翻到167行，下面又很多的if宏定义，stm32f103VET6说用的芯片属于STM32F10X_HD系列，因此还有如下的中断源

![image](https://github.com/user-attachments/assets/01427af7-3c22-419b-b62e-24b581338304)


那是因为GPIO的接口和EXTIx_IQN有个映射(即把一个具体的硬件资源（比如某个GPIO引脚）关联到某个内部功能（比如EXTI线路、某个定时器通道、ADC输入通道等）上)，映射关系如下。
![image](https://github.com/user-attachments/assets/c4508888-32d3-417f-8d27-795c5a184b6c)
![image](https://github.com/user-attachments/assets/03326240-4d49-4262-a49c-ac282fbcc769)

例如我要配置的中断线再在PA0中，那么我们只需要选择EXTI0_IQn这个参数即可。

所以NVIC_channel的作用是什么，正是我上面整队的时候所说，NVIC再检查你配置的这个中断线（比如 EXTI0_IRQn）是否已启用，以判断NVIC是否对你的中断进行响应。

3.然后我们将这个结构体初始化即可
![image](https://github.com/user-attachments/assets/d3bff902-6ed2-4ab5-a176-df2917c53b66)


我们配置NVIC的函数就算是写完了，这里需要注意的是，这个配置NVIC的函数只需要在主函数中设置一次。当你后面写复杂程序时，若你发现自己的中断无法实现或者优先级不对，那需要考虑是不是几个模块的初始化函数都包含了NVIC的配置函数。

配置完毕后，我们还需要配置按键和EXTI

按键的配置就是正常的GPIO的配置，这里我们直接略过

1.EXTI的配置我们需要先定义一个结构体，并按照里面含有的成员进行配置
![image](https://github.com/user-attachments/assets/9c6d49be-6ded-4f55-a54e-069e63612758)

2.配置EXTI的时钟，EXTI的时钟虽然也是挂在APB2上，要求传入的参数时"RCC_APB2Periph_AFIO"。这一步我和GPIO的时钟或了一下一起使能了。

3.调用函数GPIO_EXTILineConfig()，确定中断的外设和引脚来源，注意这里的外设和引脚来源的参数还和GPIOx和引脚还不一样。
![image](https://github.com/user-attachments/assets/da265c82-6704-4637-ae25-99fa748037f1)
如何找到请见main函数中的操作手则。

4.配置里面含有的成员，这里我只和大家讲解一下这几个成员的意思。
①.EXTI_Line：选择外部中断线，例如EXTI0_Line就是表示EXTI0
②.EXTI_Mode：选择中断或事件模式
③.EXTI_Trigger：触发模式，选择上升沿触发，下降沿触发或者双边沿触发
④.EXTI——Linecmd:启动/禁用EXTI线

5.最后初始化这个结构体即可
![image](https://github.com/user-attachments/assets/03c5ecf0-096b-4682-91c6-6250de2c87b0)

后期补充：防御性编程：在初始化EXTI结构体后手动清除一次中断标志位EXTI_ClearITPendingBit()，防止初始化阶段的意外中断触发，提高系统稳定性.

最后，我们需要在中断的头文件"stm32f10x_it.c"中编写中断函数，每一个EXTIx都有相对应的中断函数，例如EXTI0就对应着中断函数EXTI0_IRQHandler(),依次类推。按着自己的需要，编写中断函数即可。
