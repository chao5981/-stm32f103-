  在stm32中，有上电复位和掉电复位位 (POR 与 PDR)。当检测到 VDD 的电压低于阈值 VPOR 及 VPDR 时，无需外部电路辅助，STM32 芯片会自动保持在复位状态，防止因电压不足强行工作而带来严重的后果。见图 POR 与 PDR ，在刚开始电压低
于 VPOR 时 (约 1.92V)，STM32 保持在上电复位状态 (POR，Power On Reset)，当 VDD 电压持续上升至大于 VPOR 时，芯片开始正常运行，而在芯片正常运行的时候，当检测到 VDD 电压下降至低于 VPDR 阈值 (约 1.88V)，会进入掉电复位状
态 (PDR，Power Down Reset)。

  具体如图所示：
  
  <img width="696" height="385" alt="image" src="https://github.com/user-attachments/assets/ec9cae33-ae69-438d-8f98-eee8f99f3a2e" />

  这是保护stm32芯片的重要操作，但是 
