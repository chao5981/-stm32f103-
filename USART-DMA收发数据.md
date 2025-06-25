DMA(Direct Memory Access)—直接存储器存取，是单片机的一个外设，它的主要功能是用来搬数据，但是不需要占用 CPU。因此在高频率、重复性强、数据量大的数据数据搬运任务中非常常见。

下面是DMA的功能框图:

![image](https://github.com/user-attachments/assets/bad481a1-4fe8-4483-a91c-e2af89017c98)

注意:这张图中的AHB1和AHB2总线接的东西有问题,实际上应该是这样的
![image](https://github.com/user-attachments/assets/ec841840-151e-4df6-9f7f-2ad4ffc649c9)
其余部分并没有问题

简单介绍一下这张图，首先，DMA分成俩部分:DMA1和DMA2;DMA与所有AHB从设备都有映射;也就是说，所有挂在AHB时钟线上的外设都可以与DMA联系起来;从AHB映射到DMA的通道之间有一个"仲裁器",其实就是优先级，仲裁器管理 DMA 通道请求分为两个阶段。第一阶段属于软件阶段，可以在 DMA_CCRx 寄存器中设置，有 4 个等级：非常高、高、中和低四个优先级。第二阶段属于硬件阶段，如果两个或以上的 DMA 通道请求设置的优先级一样，则他们优先级取决于通道编号，编号越低优先权越高，比如通道 0 高于通道 1。在大容量产品和互联型产品中，DMA1 控制器拥有高于 DMA2 控制器的优先级。

DMA和AHB从设备之间的映射如下图:

![image](https://github.com/user-attachments/assets/30ba250f-be9a-4de0-849c-968996034e1f)
![image](https://github.com/user-attachments/assets/e1d52870-9102-4d97-bcb1-cf7620b168ce)

DMA也可以设置中断，有三种中断类型，见下图。
![image](https://github.com/user-attachments/assets/e4437200-12e6-455f-8c99-1664da0bf9c9)

若要配置中断，我们只需要调用下图的函数，并配置NVIC函数即可
![image](https://github.com/user-attachments/assets/ef1bccb3-1f75-43dc-817b-7797c552c6ce)

下面我将实现一个实验,即利用USART进行接收数据，然后用DMA实现数据的回传。

调用DMA，我们需要配置DMA的结构体的成员
![image](https://github.com/user-attachments/assets/ea908503-e91d-499a-88e7-3ffc77b887d0)

我挨个进行介绍:

1.DMA_BufferSize：要传输的数据量，就是你一次发送你想发多少字节，这个由你自己决定。

2.DMA_DIR:数据的传输方向，有俩个选择，一个是从内存到外设(DST);一个是从外设到内存(STC),如果确认是着俩个一个的话，那么下面的DMA_M2M必须选择DISABLE。

3.DMA_M2M：数据只从寄存器到寄存器传输，如果开启了这个，那么就会自动忽略DMA_DIR的成员。

4.DMA_MemoryBaseAddr:内存的地址(翻译为:源头端地址)，DMA从中拿取数据或者是存储数据的地址。

5.DMA_PeripheralBaseAddr:外设的地址(翻译为:目标端地址)，DMA从中拿取数据或者是存储数据的地址。

注意注意：如果按照直白的翻译，4和5应该分别被翻译成源头端地址和目标端地址，其实这样的说法是不准确的，在M2M模式中，按照翻译填入相关的内存地址是正确的，但是若不是M2M模式，数据传输的方向应该不由翻译所定，应该看DMA_DIR的方向。

还值得一提的是，这俩个地址的参数需要传入uint32_t类型的指针，如果不是uint32_t类型的话，需要强转一下。

6.DMA_MemoryDataSize:内存的每项的数据大小(翻译为:源头端的每项的数据大小),也就是DMA每次搬运的数据大小

7.DMA_PeripheralDataSize:外设的每项的数据大小(翻译为:目标端的每项的数据大小),也就是DMA每次搬运的数据大小

8.DMA_MemoryInc:内存(翻译为:源头端)是否每传输一次数据就自动增加"储存器地址"。

9.DMA_PeripheralInc:外设(翻译为:目标端)是否每传输一次数据就自动增加"储存器地址"。

注意:内存(源头端)，也就是DMA_MemoryInc一般地一定选择自增的，因为你要不这样的话，写入的数据会不断地被覆盖，读出的数据自会读取第一个;而如果DMA_PeripheralInc填的是外设，那就绝对不可以自增，因为你要这样做的话，传输一项数据的时候外设的地址就飞了。但是如果是M2M模式，DMA_PeripheralInc填的是目标端内存，那仍然需要自增。

10.DMA_Priority:设置DMA通信的优先级。

11.DMA_Mode:DMA的模式选择，Circular是循环模式，也就是传输一次过后会自动重启，不用手动调用;Normal是一次传输，传输过后需要手动重启。

配置完这些结构体的成员后，我们调用相关的函数初始化即可。


接下来我们自需要编写中断函数即可。

先通过USART的中断接收到数据，将数据存到一个数组(缓冲区)中，当接收到换行符或者缓存区满的时候，启动DMA发送数据即可。

  这里我是使用软件控制DMA发送数据，而没有采用硬件硬拉，其实硬件硬拉也不是不行。在与有些外设的交互中是由硬件完成的而不需要我们手动操作，例如：
  
  1.在ADC中，一旦 ADC 转换完成，DMA 会自动响应这个请求，将数据从 ADC 的数据寄存器传输到内存中，配置如下：

  ADC_DMACmd(ADCx, ENABLE);
  
  2.USART发送缓冲区/接收缓冲非区为空过后才发送DMA请求，配置如下：

  // 使能USART的DMA发送请求（发送缓冲区为空时触发）
  USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);

  // 使能USART的DMA接收请求（接收缓冲区非空时触发）
  USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
  
  3. SPI 的发送缓冲区/接收缓冲区非空为空时，硬件会触发 DMA 请求，配置如下：

  // 使能SPI的DMA发送请求（发送缓冲区为空时触发）
  SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);

  // 使能SPI的DMA接收请求（接收缓冲区非空时触发）
  SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);

  2和3其实就是Tx还是Rx接DMA通道的区别而已。
