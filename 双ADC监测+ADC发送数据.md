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

    6.混合的规则/注入同步模式：也就是一个ADC是扫描规则组的通道，另外一个ADC扫描注入组，当发生
                    
        
