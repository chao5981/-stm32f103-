  在stm32中，有上电复位和掉电复位位 (POR 与 PDR)。当检测到 VDD 的电压低于阈值 VPOR 及 VPDR 时，无需外部电路辅助，STM32 芯片会自动保持在复位状态，防止因电压不足强行工作而带来严重的后果。见图 POR 与 PDR ，在刚开始电压低
于 VPOR 时 (约 1.92V)，STM32 保持在上电复位状态 (POR，Power On Reset)，当 VDD 电压持续上升至大于 VPOR 时，芯片开始正常运行，而在芯片正常运行的时候，当检测到 VDD 电压下降至低于 VPDR 阈值 (约 1.88V)，会进入掉电复位状
态 (PDR，Power Down Reset)。

  具体如图所示：
  
  <img width="696" height="385" alt="image" src="https://github.com/user-attachments/assets/ec9cae33-ae69-438d-8f98-eee8f99f3a2e" />

  这是保护stm32芯片的重要操作，对于stm32来说，这并不算突兀，毕竟都电压上升和下降的过程它还是能监测到的，但对于操作者而言，这未免太突然了。怎么突然掉电了？我还没保存数据呢，我还没监测最后的数据呢

  正因为有这样的需求，stm32存在一个PVD电源监控，当电源电压上升/下降到一定阈值时，则会进入PVD中断，进行紧急处理。

  能选择的电压阈值如下：

  <img width="967" height="576" alt="image" src="https://github.com/user-attachments/assets/ac572f85-3c47-4d2a-96b0-9510dd80d8cd" />

  如何开启PVD？

  配置NVIC和EXTI，具体是上升沿触发，下降沿触发，还是上升沿和下降沿都触发PVD，这个在EXTI结构体中的EXTI_Trigger选择。然后调用PWR_PVDLevelConfig函数设置预定阈值，最后使能PVD即可。(开启中断前不要忘记提前清除一次PVD标志位)
