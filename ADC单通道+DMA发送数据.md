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

    1.规则通道：顾名思义，规则通道就是很规矩的意思，你可以理解为就是就是一群普通人在医院挂号排队，依次挂号。调用ADC_RegularChannelConfig()函数进行配置
    2.注入通道：注入，可以理解为插入，插队的意思，是一种不安分的通道。你还是可以理解为医院排队挂号的情景，来了个重伤的患者，再不医治小命不保，所以挂号就得给他挂号看病，其他人都得等一会他。调用ADC_InjectedChannelConfig()函数进行配置。
 
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

    1.规则通道转换结束中断：当规则组的所有通道转换完成时触发中断。
       启用规则组中断
       配置NVIC（嵌套向量中断控制器）
       在中断服务函数中处理数据
         关键代码如下：
            ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE); // 规则组单通道转换完成中断
            // 或 ADC_ITConfig(ADC1, ADC_IT_EOS, ENABLE); // 规则组所有通道序列完成中断（多通道扫描时）

            NVIC_InitTypeDef NVIC_InitStruct;
            NVIC_InitStruct.NVIC_IRQChannel = ADC1_IRQn; // ADC1全局中断
            NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
            NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
            NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStruct);
            
            void ADC1_IRQHandler(void) {
                if (ADC_GetITStatus(ADC1, ADC_IT_EOC) { // 检查规则组中断标志
                    uint16_t adc_value = ADC_GetConversionValue(ADC1); // 读取数据
                    ADC_ClearITPendingBit(ADC1, ADC_IT_EOC); // 清除标志
                }
            }
    
    2.注入通道转换结束中断：当注入组的通道转换完成时触发中断（优先级高于规则组）。
        启用注入组中断
        配置NVIC(与规则组共用同一中断向量，无需重复配置）
        在中断服务函数中处理数据
          关键代码如下：
            ADC_ITConfig(ADC1, ADC_IT_JEOC, ENABLE); // 注入组转换完成中断
            
            NVIC_InitTypeDef NVIC_InitStruct;
            NVIC_InitStruct.NVIC_IRQChannel = ADC1_IRQn; // ADC1全局中断
            NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
            NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
            NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStruct);
            
           void ADC1_IRQHandler(void)
           {
              if (ADC_GetITStatus(ADC1, ADC_IT_JEOC)) { // 检查注入组中断标志
                  uint16_t adc_value = ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_1); // 读取JDR1数据
                  ADC_ClearITPendingBit(ADC1, ADC_IT_JEOC); // 清除标志
              }
           }

      3.模拟看门狗中断:当ADC转换值 低于低阈值 或 高于高阈值 时，触发中断。
           设置阈值范围（12位ADC，范围0-4095）
           选择监控的通道
           启用模拟看门狗中断
           编写中断服务函数
           启动ADC并测试
              关键代码如下：
                  ADC_AnalogWatchdogThresholdsConfig(ADC1, LowThreshold, HighThreshold);  
                  // 示例：ADC_AnalogWatchdogThresholdsConfig(ADC1, 1000, 3000); // 监控1.0V~3.0V（假设参考电压3.3V）
                  
                  // 监控单个通道（如通道4）：
                  ADC_AnalogWatchdogSingleChannelConfig(ADC1, ADC_Channel_4);  
                  
                  // 或监控所有规则组通道：
                  ADC_AnalogWatchdogCmd(ADC1, ADC_AnalogWatchdog_AllRegEnable, ENABLE);  
                  
                  // 启用AWD中断
                  ADC_ITConfig(ADC1, ADC_IT_AWD, ENABLE);  
                  
                  // 配置NVIC（嵌套向量中断控制器）
                  NVIC_InitTypeDef NVIC_InitStruct;
                  NVIC_InitStruct.NVIC_IRQChannel = ADC1_IRQn;       // ADC1中断向量
                  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1; // 抢占优先级
                  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;     // 子优先级
                  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
                  NVIC_Init(&NVIC_InitStruct);
                  
                  void ADC1_IRQHandler(void) {
                      if (ADC_GetITStatus(ADC1, ADC_IT_AWD)) {  // 检查AWD中断标志
                          // 获取触发看门狗的ADC值（需根据实际通道读取）
                          uint16_t adc_value = ADC_GetConversionValue(ADC1);  
                          
                          // 处理异常（例如关闭电源、发送警报）
                          printf("AWD Alert! ADC Value: %d\n", adc_value);  
                          
                          ADC_ClearITPendingBit(ADC1, ADC_IT_AWD);  // 必须清除中断标志！
                      }
                  }
                  
                  // 初始化ADC后启动ADC
                  ADC_Cmd(ADC1, ENABLE);  
                  
                  // 启动看门狗
                  ADC_AnalogWatchdogCmd(ADC1, ENABLE);  
                  
                  // 开始转换（若使用软件触发）
                  ADC_SoftwareStartConvCmd(ADC1, ENABLE);  


  8.电压转换的计算公式：模拟电压经过 ADC 转换后，是一个 12 位的数字值，直接打印的可读性比较差，换算为公式为：如果转换后的数值为 X ，X 对应的模拟电压为 Y，2^12 / 3.3 = X/Y，=> Y = (3.3 * X ) / 2^12。


  现在已经大致了解了ADC的关键工作原理，现在开始讲解ADC的结构体成员

     1.ADC_Mode
        作用：选择ADC是“单干”还是“合作”。例如：独立模式（ADC_Mode_Independent）：1个ADC自己工作;双ADC模式（如ADC_Mode_RegSimult）：2个ADC一起干活，比如一个测电压，另一个测温度。
        
     2.ADC_ScanConvMode
        作用：要不要“扫描多个通道”。例如：禁用（DISABLE）：只测1个通道（比如只测PA1的电压）；启用（ENABLE）：按顺序测多个通道（比如PA1、PA2、PA3…）。
        
     3.ADC_ContinuousConvMode
        作用：ADC是“一次测量”还是“连续测量”。例如：单次（DISABLE）：测一次就停下，需要手动触发下一次；连续（ENABLE）：像心电图一样不停测量（适合实时监控）。

     4.ADC_ExternalTrigConv
        作用：ADC是“一次测量”还是“连续测量”。例如：单次（DISABLE）：测一次就停下，需要手动触发下一次；连续（ENABLE）：像心电图一样不停测量（适合实时监控）。

     5.ADC_NbrOfChannel
        作用：作用：告诉ADC要扫描几个通道（如果启用了扫描模式）。例如：设为3，就是扫描通道1、2、3。

     6.ADC_DataAlign
        作用：测量结果“左对齐”还是“右对齐”。一般选择右对齐

  如果你要开启DMA接受数据：DMA需要按照如下配置：
  
     1.复位DMA
     2.传输的数据数量调为1
     3.数据传输方向设为SRC
     4.关闭寄存器到寄存器
     5.寄存器地址填入你设置的缓冲区(缓冲区类型要和数据大小一致)
     6.寄存器和外设的每项数据大小为半字
     7.寄存器和外设地址不自动递增
     8.DMA选择Circular模式
     8.优先级根据实际情况设置

  下面提一嘴主要步骤：

    1.初始化GPIO引脚和时钟
    2.初始化ADC结构体和时钟
    3.初始化DMA结构体和时钟
    4.复位(校准)ADC
    5.外设DMA请求启动
    6.外设使能
    7.在需要的时候开启DMA

 需要注意的是ADC和DMA函数调用顺序，第4-6步必须严格按照步骤来调用函数，否则程序将没有任何现象。这在DMA+USART中提到过，若遗忘可以翻阅看看。
    

