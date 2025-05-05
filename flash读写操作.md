外部Flash的读写用到的是SPI协议。SPI协议和I2C有相同之处，我们只需要了解其差异，大致地我们就可以编写外部Flash的读写程序了

第一：SPI和I2C用到的引脚存在差异。

外部Flash主要用到了这四个引脚：NSS;MOSI;MISO;SCK。

NSS：片选信号线，拉低SPI对应的NSS总线，就相当于告诉SPI外设，轮到你工作了。

MOSI:写数据（MCU → Flash）

MISO:读数据（Flash → MCU）

SCK：时钟

而 WP 和 HOLD 通常接上拉电阻或直接拉高（禁用功能），不用 MCU 控制。

第二:SPI和I2C用到的命令有所差别。

在I2C中，我们的步骤是:发送地址->发送数据。这里面的地址其实就相当于命令。而在SPI中，指令比I2C更加丰富，基本步骤为:发送指令->发送地址->发送数据。

下面是SPI外设Flash的常见指令

![image](https://github.com/user-attachments/assets/df40320d-54b1-4869-b932-d129e0a108eb)

第三:EEPROM和Flash写方式的不同。

在EEPROM中，我们可以直接写入数据达到覆盖原有数据的目的。但是在Flash中，我们需要先擦除原有的数据，然后再进行写入，不然SPI的WEL位会强制置位为0，拒绝写入。

第四：SPI和I2C读取数据的不同

在I2C协议中，读取数据是有一个专门的时序，这个时序被封装成了I2C_ReceiveData()函数;但是在SPI中并没有专门的接收数据的函数，虽然有SPI_I2S_ReceiveData()函数，但是被动的接收到返回的数据，而不是主动地去要数据。于是要通过发送伪数据让从设备返回其内部的数据。为什么呢?因为SPI是“全双工”通信：每发送一个字节（MOSI），就会同步接收一个字节（MISO）——无论你想不想接收。而I2C是"半双工"通信，通信双方不能同时“发”和“收”，一次只能是一个方向的数据流。

第五:SPI的事件检测没有I2C那么复杂

在I2C中，我们要查询并等待EV5,EV6,EV7,EV8,EV8_2一系列事件；而在SPI中，我们的事件检测一般就是查询并等待TXE/RXNE是否置位。

上述就是I2C和SPI的差异，现在我们具体了解SPI的结构体成员，对SPI外设--Flash进行读写操作

![image](https://github.com/user-attachments/assets/e6f328d1-99f9-4ab3-a435-53ee9c130537)

下面依次进行解释:
1.SPI_BaudRatePrescaler:波特率预分频系数,设置 SPI 的 SCK 时钟频率(相对于 APB 时钟分频)。

2.SPI_CPHA: 时钟相位（第1还是第2个时钟边沿采样）,设置在哪个边沿采样数据。

3.SPI_CPOL:时钟极性，设置空闲时 SCK 线的电平。

SPI_CPHA和SPI_CPOL共同确定了SPI的模式，如下图

![image](https://github.com/user-attachments/assets/fa781d18-988b-4030-bd37-8dc3138b5b56)

在stm32f10x的W25Q64(Flash)中，支持模式1和模式3.

4.SPI_CRCPolynomial:CRC校验多项式（用于 CRC 检查）,一般默认是7.

5.SPI_DataSize:每帧数据长度（帧宽），可选8位或者16位，W25Q64支持8位。

6.SPI_Direction:设置 SPI 的数据线方向模式,大多数情况下我们选择双线全双工(SPI_Direction_2Lines_FullDuplex)。

7.SPI_FirstBit:选择发送的起始位（高位还是低位先发）,一般我们选择高位先发(SPI_FirstBit_MSB)。

8.SPI_Mode:设定 MCU 是 SPI 的主机还是从机,这个实验中我们选择主机(SPI_Mode_Master)。

9. SPI_NSS:片选信号，设置片选 NSS 引脚的管理方式，分为软件管理，硬件输入和硬件输出。一般主机情况常用软件。

这里我们详细讲一下NSS的作用以及软件和硬件的差别

在SPI外设中，NSS算是一个非常关键的引脚。它控制通过电平的变化告诉从机是否要与主机进行通信。因此，在含有多个SPI外设时，每一个SPI外设要有一个独立的NSS引脚。如果不独立，那么主机将同时和多个SPI外设通信，又因为SPI设置为全双工通信，发送一个数据就会返回一个数据，那主机收到的数据就乱套了，程序就无法运行。

如果是硬件控制的话，那话不用多说，的确做到了每一个SPI外设都有一个独立的NSS引脚。并且我们无需在软件上管理NSS引脚，硬件会帮助我们自动置位。但是硬件的弱点在于不方便，它只是指定了一个引脚，万一其他外设用到了这个关键引脚发送引脚冲突了呢？所以我们更多地使用软件控制。

用软件控制就可以用任意一个GPIO引脚充当某个SPI外设的NSS引脚。SPI外设会完全忽略所有物理NSS引脚，SPI模块内部直接认为NSS信号“永远有效”（相当于强制使能自身），不检测任何GPIO的状态。但是SPI外设又遵循SPI协议，总不能用软件控制就不遵循SPI协议了吧，因此，我们需要手动控制充当NSS信号的GPIO引脚，让其模拟NSS信号以符合SPI协议，这样才能正常工作。

但是这样有一个疑问，这样的话SPI外设的NSS引脚就不"独立"了呀，本质上我就用某个GPIO引脚拉高拉低模拟NSS信号而已。如果几个SPI设备不是同时工作的话，我是不是就可以仅仅用一个GPIO模拟全部SPI设备的NSS的电平变化？

理论上可行，但是实现起来条件非常严苛，你得保证俩个SPI设备永远不会通信；在切换设备时，确保前一个设备的通信完全结束，再操作NSS；没有信号传播延迟或中断干扰；某些SPI从设备（如Flash芯片）在NSS拉低时不会初始化内部状态从而导致设备异常；有非常复杂的同步机制确保无冲突。所以得不偿失，不如就还是一个SPI设备对应着一个GPIO引脚来模拟NSS信号吧。

下面我们开始编写相对应的函数。

一.初始化函数
初始化相应的GPIO引脚，NSS引脚设置为推挽输出，SCK，MOSI设置为复用推挽输出，MISO设置为浮空输入模式；初始化SPI结构体并直接使能SPI。

二.编写写一个字节函数Flash_SPI_SendByte(u8 Byte)
![image](https://github.com/user-attachments/assets/ce6a84c7-d2e3-460a-aa09-7c38ffb1c0d1)
![image](https://github.com/user-attachments/assets/00f09945-6f89-494b-a957-7cab4a2732f7)

这是写一个字节的时序，第2，3，4步固件库已经帮我们完成，封装在SPI_I2S_SendData()函数中。

1.设置SPITimeout=SPIT_FLAG_TIMEOUT( ((uint32_t)0x1000)),等待TXE，如图所示，只有TXE被置1，才可以继续发送数据。

2.调用SPI_I2S_SendData()发送数据

3.设置SPITimeout=SPIT_FLAG_TIMEOUT( ((uint32_t)0x1000)),等待RXNE，如图所示，只有RXNE被置1，才可以接收到数据(全双工通信，发送数据后会接收到数据)。

4.returnSPI_I2S_ReceiveData(FLASH_SPIx)函数得到接收到的数据

三.编写读取一个字节的函数Flash_SPI_ReadByte(void)
直接调用Flash_SPI_SendByte(u8 Byte)即可，参数选择Dummy_Byte(这是一个无效的写入数据，也就是"伪写入")。

四.写使能函数 Flash_SPI_WriteEnable(void)
拉低NSS总线，调用Flash_SPI_SendByte(u8 Byte)发送写指令W25X_WriteEnable，拉高NSS总线。

五.等待擦除/写入完成函数Flash_SPI_WaitForWriteOrEraseEnd();
拉低NSS总线，读取SPI的状态，将这个数据和WIP_Flag进行&操作(判断SPI外设是否在忙)，如果在忙，那就循环进行等待

六.擦除一个扇区的函数
调用写使能函数，拉低NSS总线，接着调用Flash_SPI_SendByte(u8 Byte)发送扇区擦除指令(W25X_SectorErase)，发送地址(地址是u32类型，但是数据的传输为u8类型，因此需要移位操作分三次发送)，然后拉高NSS总线，调用Flash_SPI_WaitForWriteOrEraseEnd();等待擦除完成。

七.页写，翻页写，翻页读函数和I2C的逻辑类似，这里不多介绍

八.
