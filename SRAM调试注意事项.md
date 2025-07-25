  一般情况下，我们在 MDK 中编写工程应用后，调试时都是把程序下载到芯片的内部 FLASH 运行测试的，代码的 CODE 及 RW-data 的内容被写入到内部 FLASH 中存储。但在某些应用场合下却不希望或不能修改内部 FLASH 的内容，
这时就可以使用 RAM 调试功能了，它的本质是把原来存储在内部 FLASH 的代码 (CODE 及 RW-data 的内容) 改为存储到 SRAM 中 (内部 SRAM 或外部 SDRAM 均可)，芯片复位后从 SRAM 中加载代码并运行。

  把代码下载到 RAM 中调试有如下优点：

    下载程序非常快。
    不改写内部 FLASH 的原有程序。
    对于内部 FLASH 被锁定的芯片，可以把解锁程序下载到 RAM 上，进行解锁。

  相对地，把代码下载到 RAM 中调试有如下缺点：

      存储在 RAM 上的程序掉电后会丢失，不能像 FLASH 那样保存
      若使用 STM32 的内部 SRAM 存储程序，程序的执行速度与在 FLASH 上执行速度无异，但SRAM 空间较小
      若使用外部扩展的 SRAM 存储程序，程序空间非常大，但 STM32 读取外部 SRAM 的速度比读取内部 FLASH 慢
      外部 SRAM 中调试的程序无法完美仿真在内部 FLASH 运行时的环境

  了解如何把程序烧录到SRAM之前，我们需要了解STM32的启动方式

  第一步：从地址 0x00000000 处取出栈指针 MSP 的初始值，该值就是栈顶的地址

  第二步:从地址 0x00000004 处取出程序指针 PC 的初始值，该值指向复位后应执行的第一条指令

  图示如下：

  <img width="897" height="149" alt="image" src="https://github.com/user-attachments/assets/e562e0dc-6744-4071-b91d-0fe0c4be604b" />

  上述过程由内核自动设置运行环境并执行主体程序，因此它被称为自举过程

  虽然内核是固定访问 0x00000000 和 0x00000004 地址的，但实际上这两个地址可以被重映射到其它地址空间，根据BOOT0和BOOT1引脚的电平情况，这两个地址可以被映射到内部 FLASH、内部 SRAM 以及系统存储器中

  <img width="875" height="219" alt="image" src="https://github.com/user-attachments/assets/04447a8a-e091-451c-9e49-d6827ff2b250" />

  内部FLASH和内部SRAM我们都很熟悉，唯独不懂的是系统存储器，系统存储器是一段特殊的空间，用户不能访问，ST 公司在芯片出厂前就在系统存储器中固化了一段代码。因而使用系统存储器启动方式时，内核会执行该代码，该
代码运行时，会为 ISP 提供支持 (In System Program)，如检测USART1/2、CAN2 及 USB 通讯接口传输过来的信息，并根据这些信息更新自己内部 FLASH 的内容，达到升级产品应用程序的目的，因此这种启动方式也称为 ISP 
启动方式。

  虽然MSP和PC地址被重映射到了SRAM区域，但是在启动文件中把设置栈顶及首条指令地址到了最前面的地址空间，但这并没有指定绝对地址，各种内容的绝对地址是由链接器根据分散加载文件 (*.sct) 分配的。而默认设置的sct
文件的起始地址是FLASH的地址，也就是说，即便你重映射到了SRAM区域，程序仍然不会烧录到SRAM中，仍然需要修改sct文件，将地址改成SRAM的地址

  现在来讲解一下具体配置步骤

  1.在原工程的基础上创建一个调试版本

  <img width="911" height="687" alt="image" src="https://github.com/user-attachments/assets/89d49fdd-4179-4dee-8634-c5def9893337" />

  当需要切换工程版本时，点击 MDK 工程名的下拉菜单可选择目标工程，在不同的工程中，所有配置都是独立的，例如你在SRAM调试工程中修改的魔法棒配置，那只会影响你这一个SRAM的工程，而不会影响你原来的工程，点击Rebuild
也会生成不一样的axf,sct文件,所以建议把工程名字给改一下

  <img width="830" height="619" alt="image" src="https://github.com/user-attachments/assets/39952c26-f66b-4127-ab0a-644128d8eb38" />

  但如果两个工程共用了同一个文件，对该文件的修改会同时影响两个工程，例如这两个工程都使用同一个 main 文件，我们在 main 文件修改代码，两个工程都会被修改。

  2.配置sct文件

  <img width="917" height="583" alt="image" src="https://github.com/user-attachments/assets/6f0d06cb-9f48-463d-b918-91d3aceecde8" />

  把原本属于FLASH的地址改成SRAM的地址，并且将一部分区域划分成模拟的ROM存储特定的数据，一部分划分成模拟RAM

  3.配置中断向量表

  在SystemInit函数中，有这样一段话

  <img width="867" height="162" alt="image" src="https://github.com/user-attachments/assets/7fab597c-2f77-4567-9f2c-576e16f10423" />

  我们一般没有宏定义VECT_TAB_SRAM，因此一般指向FLASH，由于要用到SRAM，我们需要把这个宏定义加上，由于直接修改文件会同时影响俩个工程，因此我们把宏定义添加到魔法棒的define中

  <img width="891" height="636" alt="image" src="https://github.com/user-attachments/assets/0268b9ab-9162-45c4-9100-33c69e56902b" />

  不同的宏定义用逗号隔开

  4.修改 FLASH 下载配置

  <img width="865" height="674" alt="image" src="https://github.com/user-attachments/assets/d6f67c1f-568a-43c3-ab62-e493a431e8c5" />

  这个配置对话框原本是用于设置芯片内部 FLASH 信息的，当我们点击 MDK 的下载（LOAD）或（调试）按钮时，它会从此处加载配置然后下载程序到 FLASH 中，而在上图中我们把它的配置修改成下载到内部 SRAM。我们其实是
不希望修改这个的，但是没有下载到SRAM的选项，所以不得已需要手动调节

  各个配置的解释如下：

  把“Download Function”中的擦除选项配置为“Do not Erase”。这是因为数据写入到内部SRAM 中不需要像 FLASH 那样先擦除后写入。在本工程中，如果我们不选择“Do not Erase”的话，会因为擦除过程导致下载出错。

  “RAM for Algorithm”一栏是指“编程算法”(Programming Algorithm) 可使用的 RAM 空间，下载程序到 FLASH 时运行的编程算法需要使用 RAM 空间，在默认配置中它的首地址为0x20000000，即内部 SRAM 的首地址，但由于
我们的分散加载文件配置，0x20000000 地址开始的 32KB 实际为虚拟 ROM 空间，实际的 RAM 空间是从地址 0x20008000 开始的，所以这里把算法 RAM 首地址更改为本工程中实际作为 RAM 使用的地址。若编程算法使用的
RAM 地址与虚拟 ROM 空间地址重合的话，会导致下载出错。

  从这个例子可了解到，这里的配置是跟我们的分散加载文件的实际 RAM 空间和虚拟 ROM 空间信息是一致的，若分散加载文件采用不同的配置，这个下载选项也要作出相应的修改，不能照抄本例子的空间信息。

  理论上，做完这几步就可以正常点击烧录程序，然后就可以了。

  但是复位后 STM32 的程序 PC 指针和 SP 指针却莫名奇妙地指向非预设的 ResetHandler 及栈顶位置，导致程序无法正常运行。也就是说，BOOT1和BOOT0失效了。除了STM32F429以外其他芯片都无一幸免，所以为了程序能正常
在SRAM中运行，我们换了一种简单粗暴的方法，强行把SP和PC指针指向SRAM--使用仿真器

  因为在仿真器中，在复位后会进入到一个.ini文件，我们可以在这个文件强制修改SP和PC指针。

  5.启动仿真器

  先修改配置

  <img width="915" height="749" alt="image" src="https://github.com/user-attachments/assets/e744392b-e6a8-43cc-9b7d-e8fc12de13dc" />

  注意，一定要勾选Use CMSIS-DAP Debugger，不然会出现"*** error 65: access violation at 0xFFFFFFF4 : no 'write' permission"报错

  <img width="924" height="803" alt="image" src="https://github.com/user-attachments/assets/94934304-717a-427d-b974-18c2d2144886" />

  .ini文件直接那我的这个copy一下，这个理论上可以在keil 5 的下载目录中找到，但我试了试，不太好找

  如果找到了，只需要把g,main注释掉即可

  然后点击仿真按钮，程序就可以被烧录到SRAM正常跑了

  <img width="43" height="53" alt="image" src="https://github.com/user-attachments/assets/d913473c-315e-453a-a4ae-08fd6a65da4e" />

  点击全速运行，程序就会正常跑，点击停止，程序停下来。

  如果点击了复位，那么程序就不能运行了，因为仿真的复位并不会进入到ini文件，PC和SP指针又跑偏了

  如果要再次运行，请再次点击仿真按钮即可。

  


  
