  在单个ADC中，ADC 采集需要在一个通道采集并且转换完成后才会进行下一个通道的采集，而双重ADC机制可以使用两个 ADC 同时采样一个或者多个通道，大大提高了采样率。

  下面我讲介绍双ADC模式的各类模式，配置步骤和关键的代码展示(关键代码由AI生成，AI对于ADC这一部分代码的细节处理有时候存在问题，请谨慎甄别)：

      1.同步规则模式（ADC_Mode_RegSimult）：ADC1和ADC2同时测量同一组“常规信号”（规则组），结果合并到ADC1_DR寄存器（ADC1数据在低16位，ADC2在高16位）。ADC1为主，ADC2为从。
          步骤：
              1.GPIO初始化：配置ADC通道对应的引脚为模拟输入。
              2.时钟使能：开启ADC1、ADC2和DMA时钟
              3.DMA配置：绑定到ADC1_DR，存储32位数据（高低16位分离）。
              4.ADC模式配置：ADC1和ADC2均设置为同步规则模式。ADC1：主设备，外部触发（实际证明软件触发和硬件触发均可）;ADC2：从设备，软件触发（实际由ADC1同步触发）;
              5.校准与启动：校准后启动硬件触发/软件触发。ADC1可以选择调用俩种形式ADC_SoftwareStartConvCmd/ADC_ExternalTrigConvCmd，ADC2必须选择调用硬件触发转换ADC_ExternalTrigConvCmd。
                  关键代码：
                      // 1. ADC1配置（主）
                      ADC_InitTypeDef ADC_InitStruct;
                      ADC_InitStruct.ADC_Mode = ADC_Mode_RegSimult;      // 同步规则模式
                      ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO; // TIM2触发
                      ADC_Init(ADC1, &ADC_InitStruct);
                      
                      // 2. ADC2配置（从）
                      ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 软件触发（实际由ADC1同步）
                      ADC_Init(ADC2, &ADC_InitStruct);
                      
                      // 3. DMA配置（32位传输）
                      DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; // ADC寄存器数据为32位
                      DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;            // 内存存储32位
                      DMA_Init(DMA1_Channel1, &DMA_InitStruct);
                      
                      // 4. 启动定时器触发
                      TIM_Cmd(TIM2, ENABLE);


      2.同步注入模式（ADC_Mode_InjSimult）：ADC1和ADC2同步转换同一注入通道组，结果分别存到ADC1_JDRx和ADC2_JDRx寄存器中。ADC1为主，ADC2为从。
          步骤：
              1.GPIO初始化：配置注入通道引脚。
              2.ADC配置：ADC1和ADC2均设置为同步注入模式；触发源配置为外部触发（如EXTI）。
              3.校准与启动：校准后启动外部触发。都选择硬件触发转换ADC_ExternalTrigConvCmd。
              备注：由于注入族每一个注入通道有自己的寄存器，所以我们不需要用到DMA，直接读取寄存器即可。
                  关键代码：
                      // 1. ADC1和ADC2配置
                      ADC_InitStruct.ADC_Mode = ADC_Mode_InjSimult;       // 同步注入模式
                      ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigInjecConv_Ext_IT15; // EXTI15触发
                      ADC_Init(ADC1, &ADC_InitStruct);
                      ADC_Init(ADC2, &ADC_InitStruct);
                      
                      // 2. 配置注入通道
                      ADC_InjectedChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_28Cycles5);
                      ADC_InjectedChannelConfig(ADC2, ADC_Channel_1, 1, ADC_SampleTime_28Cycles5);
                      
                      // 3. 启动外部触发（如EXTI）
                      EXTI_InitStruct.EXTI_Line = EXTI_Line15;
                      EXTI_Init(&EXTI_InitStruct);

                        //读取数据
                    uint16_t adc1_data = ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_1);
                    uint16_t adc2_data = ADC_GetInjectedConversionValue(ADC2, ADC_InjectedChannel_1);
                    
                  
      3.快速交叉模式（ADC_Mode_FastInterl）：ADC1和ADC2交替转换同一规则组（只能单通道）。
          步骤：
              1.GPIO初始化：配置同一通道的引脚。
              2.ADC配置：ADC1和ADC2均设置为快速交叉模式；触发源为定时器（间隔7个ADCCLK）。
              3.DMA配置，基本上和同步相同
              4.启动定时器：确保触发间隔匹配。
                  关键代码：
                      // 1. ADC配置
                      ADC_InitStruct.ADC_Mode = ADC_Mode_FastInterl;      // 快速交叉模式
                      ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO; // TIM3触发
                      ADC_Init(ADC1, &ADC_InitStruct);
                      ADC_Init(ADC2, &ADC_InitStruct);
                      
                      // 2. 定时器配置（触发间隔 = 7 ADCCLK）
                      TIM_TimeBaseInit(TIM3, &TIM_InitStruct);
                      TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
    
                      // DMA配置（存储交替采样数据）
                      DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
                      DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)adc_buffer;  // uint16_t数组
                      DMA_InitStruct.DMA_BufferSize = 128;                       // 数据个数
                      DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
                      DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
                      // ...其余配置同同步规则模式

      4.慢速交替模式（ADC_Mode_SlowInterl）：ADC1和ADC2交替转换同一个规则组（只能单通道）。
          步骤：
              1.GPIO初始化：配置ADC通道引脚为模拟输入。
              2.ADC模式设置：设置为慢速交替模式
              3.DMA配置，基本上和同步相同
              4.启动定时器：确保触发间隔匹配。
                  关键代码：
                      //ADC配置
                      ADC_InitStruct.ADC_Mode = ADC_Mode_SlowInterl;  // 慢速交替模式
                      ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO; // 定时器触发
                      ADC_Init(ADC1, &ADC_InitStruct);
                      ADC_Init(ADC2, &ADC_InitStruct);
                      
                      //定时器配置
                      TIM_TimeBaseInit(TIM3, &TIM_InitStruct);
                      TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update); // 定时器触发间隔 ≥14 ADCCLK
    
                      //DMA配置
                      // DMA配置（存储交替采样数据）
                      DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
                      DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)adc_buffer;  // uint16_t数组
                      DMA_InitStruct.DMA_BufferSize = 128;                       // 数据个数
                      DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
                      DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
                      // ...其余配置同同步规则模式

                  
    5.交替触发模式（ADC_Mode_AlterTrig）：ADC1和ADC2轮流转换各自的注入组（可多通道）。
        关键步骤：
            1.GPIO初始化：配置注入通道引脚。
            2.ADC模式设置
            3.手动触发转换
            4.读取数据（无需DMA），直接读取寄存器
                关键代码：
                    //ADC配置
                    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent; // 独立模式
                    ADC_InitStruct.ADC_ExternalTrigInjecConv = ADC_ExternalTrigInjecConv_None; // 软件触发
                    ADC_Init(ADC1, &ADC_InitStruct);
                    ADC_Init(ADC2, &ADC_InitStruct);
                    
                    //手动触发
                    ADC_SoftwareStartInjectedConvCmd(ADC1, ENABLE); // 启动ADC1注入组
                    Delay(10); // 自定义间隔
                    ADC_SoftwareStartInjectedConvCmd(ADC2, ENABLE); // 启动ADC2注入组
                    
                    //读取数据
                    uint16_t adc1_data = ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_1);
                    uint16_t adc2_data = ADC_GetInjectedConversionValue(ADC2, ADC_InjectedChannel_1);

    6.混合的规则/注入同步模式：俩个ADC同时开启了规则组通道和注入组通道。一开始都是在测量规则组通道的数据；触发外接条件后，测量注入组通道的数据。ADC1的配置和ADC2的配置一模一样。

    7.混合的同步规则模式+交替触发模式：俩个ADC同步测规则组信号时，紧急信号来了就切换成交替触发模式测量注入组通道。
        关键步骤：
            1.GPIO初始化：配置注入通道引脚。
            2.ADC模式设置：其中一个ADC为规则组同步模式，另外一个为独立模式
            3.手动触发转换
            4.规则组正常配置DMA，到交替触发时手动读取
                关键代码：
                    // ========== ADC1配置（主设备，规则组同步） ==========
                    ADC_InitTypeDef ADC_InitStruct;
                    ADC_InitStruct.ADC_Mode = ADC_Mode_RegSimult;          // 规则组同步模式
                    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO; // TIM2触发规则组
                    ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;        // 连续转换
                    ADC_Init(ADC1, &ADC_InitStruct);
                    
                    // 配置规则组通道（例如通道10）
                    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);
                    
                    // ========== ADC2配置（从设备，注入组交替触发） ==========
                    ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;        // 独立模式（注入组单独控制）
                    ADC_InitStruct.ADC_ExternalTrigInjecConv = ADC_ExternalTrigInjecConv_None; // 软件触发
                    ADC_Init(ADC2, &ADC_InitStruct);
                    
                    // 配置注入组通道（2个通道交替）
                    ADC_InjectedChannelConfig(ADC2, ADC_Channel_11, 1, ADC_SampleTime_55Cycles5); // 注入通道1
                    ADC_InjectedChannelConfig(ADC2, ADC_Channel_12, 2, ADC_SampleTime_55Cycles5); // 注入通道2
                    ADC_InjectedSequencerLengthConfig(ADC2, 2);           // 注入组长度=2
                    
                    // 启动定时器触发规则组
                    TIM_Cmd(TIM2, ENABLE);
                    
                    // 手动交替触发注入组（在中断中调用）
                    void Trigger_InjectedGroup() 
                    {
                        static uint8_t toggle = 0;
                        if (toggle == 0) {
                            ADC_SoftwareStartInjectedConvCmd(ADC1, ENABLE); // 触发ADC1注入组
                        } else {
                            ADC_SoftwareStartInjectedConvCmd(ADC2, ENABLE); // 触发ADC2注入组
                        }
                        toggle ^= 1;
                    }


    8.交叉模式+同步注入模式：一开始俩个ADC在交替采集一个规则通道组，紧急信号来临后各自测自己的注入通道组
        关键步骤：
            1.GPIO初始化：配置注入通道引脚。
            2.ADC模式设置：其中一个ADC为注入组同步模式，另外一个为交叉模式
            3.定时器启动
            4.DMA正常配置，到中断处取注入组寄存器的数据
                关键代码：
                    // ========== ADC1配置（主设备，注入组同步） ==========
                    ADC_InitStruct.ADC_Mode = ADC_Mode_InjSimult;          // 注入组同步模式
                    ADC_InitStruct.ADC_ExternalTrigInjecConv = ADC_ExternalTrigInjecConv_Ext_IT15; // EXTI触发注入组
                    ADC_Init(ADC1, &ADC_InitStruct);
                    
                    // 配置注入组通道（同步转换）
                    ADC_InjectedChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_55Cycles5);
                    ADC_InjectedChannelConfig(ADC2, ADC_Channel_11, 1, ADC_SampleTime_55Cycles5);
                    
                    // ========== ADC2配置（从设备，规则组交叉） ==========
                    ADC_InitStruct.ADC_Mode = ADC_Mode_FastInterl;         // 快速交叉模式
                    ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO; // TIM3触发交叉采样
                    ADC_Init(ADC2, &ADC_InitStruct);
                    
                    // 配置规则组交叉通道（同一通道交替采样）
                    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);
                    ADC_RegularChannelConfig(ADC2, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);
                    
                    // 启动外部中断和定时器
                    EXTI_Cmd(EXTI_Line15, ENABLE);  // 使能EXTI15触发注入组
                    TIM_Cmd(TIM3, ENABLE);          // 启动TIM3触发交叉采样
                                        
                                                            
