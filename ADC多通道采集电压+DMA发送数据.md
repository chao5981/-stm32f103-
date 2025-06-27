  这一期其实没有什么新鲜的东西，不过就是把原本测电压的引脚扩展多了几个而已(即单通道->多通道)。主要也是移植上一章单通道的代码，这里将移植时的注意事项。

  对于宏定义：

      1.main函数和DMA.c文件中的ADC_ConvertedValue和ADC_ConvertedValueLocal变成数组
      2.DMA.h的宏定义中ADCx_DMA_SIZE设置为转化通道个数NOFCHANEL

  对于DMA文件：

      1.目标端(寄存器)的地址改成数组
      2.目标端(寄存器)的地址开启"每次传输一次数据就地址自增"

  对于ADC文件:

      1.ADC转换通道的GPIO口全部初始化
      2.转换通道个数设置成NOFCHANEL
      3.开启扫描模式
      4.配置规则组顺序

实验验证：用杜邦线连接开发板的GND引脚和PCx引脚，会发现对应的ADC通道电压为0


由于本实验并没有将注入组进行配置，这里我只给个例子，例子为：在一个ADC中，开启一个规则通道，由定时器触发；俩个注入组通道，由中断触发。

    代码如下：
          #include "stm32f10x.h"

      void ADC_Configuration(void) 
      {
          ADC_InitTypeDef ADC_InitStruct;
          
          // 1. 基础ADC配置
          ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;       // 独立模式
          ADC_InitStruct.ADC_ScanConvMode = ENABLE;            // 启用扫描（多通道）
          ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;     // 单次转换（由触发信号控制）
          ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO; // TIM2触发规则组
          ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;  // 数据右对齐
          ADC_InitStruct.ADC_NbrOfChannel = 1;                 // 规则组1个通道
          ADC_Init(ADC1, &ADC_InitStruct);
      
          // 2. 配置规则组通道（例如通道10，PC0）
          ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);
      
          // 3. 配置注入组（2个通道）
          ADC_InjectedChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_55Cycles5); // 注入通道1（PC1）
          ADC_InjectedChannelConfig(ADC1, ADC_Channel_12, 2, ADC_SampleTime_55Cycles5); // 注入通道2（PC2）
          ADC_InjectedSequencerLengthConfig(ADC1, 2);          // 设置注入组长度为2
      
          // 4. 配置注入组触发源为外部中断线（EXTI）
          ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_Ext_IT15); // EXTI15触发
          ADC_ExternalTrigInjectedConvCmd(ADC1, ENABLE);       // 使能注入组外部触发
      
          // 5. 使能ADC中断（用于注入组）
          ADC_ITConfig(ADC1, ADC_IT_JEOC, ENABLE);            // 注入组转换完成中断
          NVIC_EnableIRQ(ADC1_2_IRQn);                        // 使能ADC全局中断
      
          // 6. 校准ADC
          ADC_ResetCalibration(ADC1);
          while(ADC_GetResetCalibrationStatus(ADC1));
          ADC_StartCalibration(ADC1);
          while(ADC_GetCalibrationStatus(ADC1));
      
          // 7. 启动ADC
          ADC_Cmd(ADC1, ENABLE);
      }
      
      // 外部中断配置（触发注入组）
      void EXTI_Configuration(void)
      {
          EXTI_InitTypeDef EXTI_InitStruct;
          
          // 配置EXTI15（例如PC15引脚）触发注入组
          EXTI_InitStruct.EXTI_Line = EXTI_Line15;
          EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
          EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising; // 上升沿触发
          EXTI_InitStruct.EXTI_LineCmd = ENABLE;
          EXTI_Init(&EXTI_InitStruct);
      
          // 配置NVIC
          NVIC_InitTypeDef NVIC_InitStruct;
          NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
          NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
          NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
          NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
          NVIC_Init(&NVIC_InitStruct);
      }
      
      // ADC中断服务函数(直接读取注入组寄存器即可)

可以看到的是，注入组的配置主要是由 ADC_InjectedChannelConfig(), ADC_ExternalTrigInjectedConvConfig(),ADC_ExternalTrigInjectedConvCmd()和 ADC_ITConfig()函数配置，而不是由结构体配置。
