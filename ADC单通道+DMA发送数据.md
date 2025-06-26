 下图是ADC的功能框架，我将按照这个图的顺序，讲解其要点
  ![image](https://github.com/user-attachments/assets/f26fac1b-7d25-4bb3-8016-e02256128f19)

  1.电压输出范围:
  
    ADC的输入电压范围是0到3.3V,如果外部的电压远高于3.3V/低于3.3V，我们需要在外部加一个电压调理电路，将电压降到ADC可接受范围。其中在设计时一般把 VSSA 和 VREF- 接地，把 VREF+ 和 VDDA 接 3V3。

  2.输入通道：stm32f103的ADC通道汇总如下:
  ![image](https://github.com/user-attachments/assets/2050360d-ec74-4bed-a0da-e8d8ab6fb257)

  名词解释：
  
    1.VSS:VSS 是STM32（以及其他集成电路）的 电源地（Ground）引脚，代表电路的参考零电位（0V），是所有电压测量的基准点。
    2.VREFINT:STM32中的VREFINT（Voltage Reference Internal）是芯片内部提供的一个高精度、稳定的基准电压源.

  3.规则通道和注入通道:

    1.规则通道：顾名思义，规则通道就是很规矩的意思，你可以理解为就是就是一群普通人在医院挂号排队，依次挂号。
    2.注入通道：注入，可以理解为插入，插队的意思，是一种不安分的通道。你还是可以理解为医院排队挂号的情景，来了个重伤的患者，再不医治小命不保，所以挂号就得给他挂号看病，其他人都得等一会他。
 
   总得来说，规则组用于常规轮询（如温度、光照）。注入组用于紧急事件（如故障检测，电机过流）。
  
   在图中我们可以看到，输入通道到注入通道/规则通道有一条线，标注着最多4通道和最多16通道的字眼。这么多通道，但ADC也就几个，有时候甚至只使能一个ADC，那么ADC先转换哪个数据便是问题。因此我们需要排序。
  
   在规则组中，我们可以调用ADC_RegularChannelConfig()函数去配置其ADC转换优先级;在注入组中，我们可以调用ADC_InjectedChannelConfig()函数去配置。
  
   但是如果直接操作寄存器的话，规则组和注入组的配置会有所不同，有兴趣的可以翻阅手册自行了解。

 4.触发源:配置好ADC之后，需要选定触发源(即哪时候开启ADC)，有三种触发形式

    1.软件触发:
      初始化ADC（设置模式、采样时间等）。
      禁用硬件触发，选择软件触发模式。
      手动调用函数启动转换。
        代码如下：
             // 1. ADC基础配置（已省略部分初始化代码）
             ADC_InitTypeDef ADC_InitStruct;
             ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 选择软件触发
             ADC_Init(ADC1, &ADC_InitStruct);
             
             // 2. 校准ADC（略）
             
             // 3. 启动ADC
             ADC_Cmd(ADC1, ENABLE);
             
             // 4. 需要转换时，手动触发
             ADC_SoftwareStartConvCmd(ADC1, ENABLE); // 启动单次转换
             
             // 如果是连续转换模式，只需触发一次
             // ADC_ContinuousConvModeCmd(ADC1, ENABLE);
       
    2.定时器触发
      初始化定时器（TIMx），配置触发事件（如更新事件TIM_TRGO_Update）。
      配置ADC选择定时器触发源。
      启动定时器和ADC。
         代码如下：
             // 1. 配置定时器（TIM2为例）
             TIM_TimeBaseInitTypeDef TIM_InitStruct;
             TIM_InitStruct.TIM_Period = 1000 - 1;       // 自动重装载值
             TIM_InitStruct.TIM_Prescaler = 7200 - 1;    // 分频值（72MHz/7200=10kHz）
             TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
             TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
             
             // 配置TIM2触发ADC的事件（更新事件触发）
             TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
             
             // 启动定时器
             TIM_Cmd(TIM2, ENABLE);
             
             // 2. 配置ADC选择定时器触发
             ADC_InitTypeDef ADC_InitStruct;
             ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO; // TIM2触发
             ADC_Init(ADC1, &ADC_InitStruct);
             
             // 3. 启动ADC
             ADC_Cmd(ADC1, ENABLE);

    3.外部IO触发
       配置GPIO引脚为外部中断模式（如上升沿触发）。
       配置EXTI和NVIC，使能中断。
       在中断服务函数中触发ADC。
          代码如下：
             // 1. 配置GPIO（PA0为例）
             GPIO_InitTypeDef GPIO_InitStruct;
             GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
             GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD; // 下拉输入
             GPIO_Init(GPIOA, &GPIO_InitStruct);
             
             // 2. 配置EXTI（PA0连接到EXTI0）
             EXTI_InitTypeDef EXTI_InitStruct;
             EXTI_InitStruct.EXTI_Line = EXTI_Line0;
             EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
             EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising; // 上升沿触发
             EXTI_InitStruct.EXTI_LineCmd = ENABLE;
             EXTI_Init(&EXTI_InitStruct);
             
             // 3. 配置NVIC
             NVIC_InitTypeDef NVIC_InitStruct;
             NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
             NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
             NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
             NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
             NVIC_Init(&NVIC_InitStruct);
             
             // 4. 在EXTI中断中触发ADC
             void EXTI0_IRQHandler() {
                 if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
                     ADC_SoftwareStartConvCmd(ADC1, ENABLE); // 启动ADC
                     EXTI_ClearITPendingBit(EXTI_Line0);     // 清除中断标志
                 }
             }
            

 5.转换时间：
 
     1.ADC时钟：最大支持14HZ，一般我们以systick为72hz，八分频即可。
     2.采样时间：常用55.5周期

 6.数据放在哪：

    1.规则组：全部数据都放在ADC_DR寄存器中（编译写法：ADC1->DR）;但是规则通道可以有 16 个这么多，可规则数据寄存器只有一个，如果使用多通道转换，那转换的数据就全部都挤在了 DR 里面，所以为了防止数据覆盖，我们一般开启DMA传输
    2.注入组：注入数据寄存器也有 4 个(ADC_JDRx)(编译写法：ADC1->JDR1),每个通道对应着自己的寄存器，不会跟规则组那样产生数据覆盖的问题。

 7.中断：
    
