<img width="1913" height="824" alt="image" src="https://github.com/user-attachments/assets/17c60156-d6c1-46c7-ba7c-c00bd0e1cff8" />  此篇不要刚开始学stm32就打开看，可能看不懂，等你学完了stm32所有外设后，再回头来看这篇。
  
  在按照网上的教程设置完一个通用的文件夹，并熟练地用MDK创建应用程序，想必应该对MDK已经挺熟悉的了。

  但是MDK到底是怎么工作的呢？魔法棒的各种天花乱坠的配置是用来干嘛的呢？再打开output文件夹后，那么多各种后缀的文件是干什么用的呢？

  看完以下这章，你将学习到：

    MDK的工作原理
    各个类型文件是干嘛的
    编译过程中的各个操作的含义
    程序是如何存储和运行
    各个编译器能独立地做些什么
    MDK魔法棒各个配置的含义
    若用其他的烧录器，我们要如何转换为相相对应的格式方便其下载？
    如何保护自己的代码不被轻易的窃取？


  首先，我们先看MDK的工作原理。

  在编译(如果已经编译过了，请点击旁边的Rebuild)的时候，我们会发现第一行有这么一句话。

  <img width="802" height="51" alt="image" src="https://github.com/user-attachments/assets/9f7b73fa-fd0e-41cf-bbb4-0959555f736e" />

  这段话有一个地址，我们顺着地址打开它。

  <img width="1285" height="521" alt="image" src="https://github.com/user-attachments/assets/8376b136-a687-4d98-b858-d8f6106f9d69" />

  发现有这么几个脚本

  其实，MDK能够工作的原因就是靠这几个编译器。见MDK的编译过程图

  <img width="949" height="420" alt="image" src="https://github.com/user-attachments/assets/3c1b394f-af59-423e-88cd-92dd1beeedf1" />

  想必大致的意思大家能理解吧。就是.c和.s文件分别经过armcc编译器和armasm编译器生成对应的.o文件；每个.o文件分别由各自的code,data,debug数据;然后这些.o文件通过armlink链接器分门别类的放一块，并生成在相应的.axf映像文件;
最后再经过formelf格式转换器倒腾一手变成.bin或者.hex文件的格式。

  在MDK中，烧录要的是.hex文件的格式，但有些烧录软件，要的是.bin文件的格式。

  看完上述部分，你对MDK的工作原理和各个类型文件的作用应该就有一个大概的了解了。

  我们来看编译时build output打印出来的信息有什么用

  <img width="1182" height="622" alt="image" src="https://github.com/user-attachments/assets/cd14447c-ba41-4b5a-9912-62d8988bb0a8" />

  *** Using Compiler 'V5.06 update 7 (build 960)', folder: 'D:\keil 5.36\Keil_v5\ARM\ARMCC\Bin' ----- 告诉你它用的编译器在哪个文件夹，注意，V5.06 update 7(build 960) 不是keil的型号，而是编译器的名字

  assembling startup_stm32f10x_hd.s...  ----- 用armasm编译器把.s文件生成为对应的.o文件

  compiling stm32f10x_adc.c...   ------ 用armcc编译器把.c文件生成对应的.o文件

  这里说不定就会有疑问了，在project视图的FWLB目录下，有这么多的文件名，为什么build output没有全部打印出来，去看output文件夹，发现好多文件的.o文件都没有，也就说明它们有一部分根本没经过armcc编译，为什么？

  <img width="313" height="474" alt="image" src="https://github.com/user-attachments/assets/2c5e8d39-612e-434d-a5c5-2912adeacc20" />

  这不得不提我们MDK非常智能的功能，它会把要用到的头文件编译，例如你用到了GPIO_Init(),那么MDK就会让stm32f10x_gpio.c通过armcc生成对应的.o文件，如果没用到，那么就直接把你移除掉。这个我们可以打开.map文件看得到，这个后面再提

  <img width="763" height="546" alt="image" src="https://github.com/user-attachments/assets/2d7e210e-94d0-484a-9007-814eb3da4a37" />

  这样的举动能很好地帮助我们节省本身就不大的内部FLASH空间，是个非常好的一步

  再往下看

  linking... ----- 用armlink链接器把.o的各个数据放好，并生成了映像文件.axf

  Program Size: Code=1176 RO-data=320 RW-data=0 ZI-data=1024  -----附带了程序各个域大小的说明，包括 Code、RO-data、RW-data 及 ZI-data 的大小。

  这里又需要展开讲了，Code,RO-data,PW-data,ZI-data分别是什么东西？

  Code：叫做代码域，是编译器生成的机器指令。换句话说，就是Code不是人话，是给机器看的，你要是能看明白，那你可真是人工智能了。

  RO-data：全称为Read Only data，即只读数据域，它指程序中用到的只读数据。换句话说，就是不可以修改的数据。你定义的const变量(包括变量，数组，结构体等，只要被const修饰的)，这就属于RO-data。你在工程文件中定义一个const变量并且调用它，你再次编译会发现RO-data的值会
发生改变。

  未加入const前
  
  <img width="1915" height="996" alt="image" src="https://github.com/user-attachments/assets/ca71020a-b7fe-4fad-85cf-f7d0a32c0d88" />

  加入后
  
  <img width="1919" height="1078" alt="image" src="https://github.com/user-attachments/assets/e235cd0e-0fd0-4959-b65b-872b55b3e35b" />

  注意：若你想看着RO-data在你加入const变量后发生变化，那你需要在函数中调用它，不然编译器会直接把他优化掉。以及RO-data多了12bit的原因，而不是10bite，大概率也是优化的原因。(反向优化？有点意思哈)

  RW-data：全称为Read Write data，即可读写数据域，它指初始化为“非 0 值”的可读写数据，程序刚运行时，这些数据具有非 0 的初始值。换句话说，就是可以修改的数据，并且初始值不为0.例如已初始化的全局变量(int globalVar = 42; ),已初始化的全局/局部静态变量(static float staticGlobal = 3.14f),包含已初始化数据的结构体 / 联合体成员;就属于RW-data。在工程文件中定义一个静态变量并且调用它，你再次编译会发现RW-data的值会发生改变。

  未加入前：

 <img width="1915" height="961" alt="image" src="https://github.com/user-attachments/assets/b8611747-5b90-452b-b066-615a039a2690" />

  加入后：
  
 <img width="1885" height="979" alt="image" src="https://github.com/user-attachments/assets/c714d309-bc3b-4914-9999-9d0c01144fbe" />

  注意：若你想看着RW-data在你加入全局变量后发生变化，那你需要在函数中调用它，不然编译器会直接把他优化掉。以及RW-data多了4bit的原因，而不是1bit，大概率也是优化的原因。(又反向优化？有点意思哈)

  ZI-data：Zero Initialie data，即 0 初始化数据，它指初始化为“0 值”的可读写数据域。换句话说，就是可修改的数据，并且初始值为0.例如未初始化的全局变量(int globalVar)，显式初始化为 0 的全局变量(int zeroVar = 0),未初始化的静态变量(static int staticGlobal),显式初始化为 0 的静态变量(static int staticZero = 0);那就属于ZI-data。在工程文件中定义一个全局变量并且调用它，你再次编译会发现ZI-data的值会发生改变。

  未加入前：

  <img width="1902" height="977" alt="image" src="https://github.com/user-attachments/assets/db54d285-38c0-4bf1-9f72-f9f248bc28c7" />


  加入后：

  <img width="1908" height="927" alt="image" src="https://github.com/user-attachments/assets/c8caee2e-9bf1-47c3-b2ab-04243f94aa8c" />

  注意：这里除了定义还要调用以外，还有一个非常抽象的是，我这定义了一个全局变量也被直接优化了，除了code以外什么都没有增加;反倒是上面加入一个已经初始化的静态变量，ZI-data却也跟着改变了。见如图：

  <img width="1740" height="924" alt="image" src="https://github.com/user-attachments/assets/89a7de83-8183-4d66-b502-a8f1b39bd0bb" />

  有点抽象，只能说这优化有一定好处，但想要理解那这优化的逻辑做的还是离神太近，离人太远了。

  不知道你看完后有没有一个问题，都是数据，为什么ZI-data能比RO-data和RW-data大这么多？

  因为ZI-data不仅仅存着可修改的初始化为0的数据，还存储着栈空间和堆空间。如函数内部定义的局部变量属于栈空间，进入函数的时候从向栈空间申请内存给局部变量，退出时释放局部变量，归还内存空间。而使用 malloc 动态分配的变量属于堆空间编译器给出的 ZI-data 占用的空间值中包含了堆栈的大小 (经实际测试，若程序中完全没有使用 malloc 动态申请堆空间，编译器会优化，不把堆空间计算在内)。

  这里如果调用malloc，ZI-data会把堆空间也算进来，如图

  未使用malloc前

  <img width="1888" height="959" alt="image" src="https://github.com/user-attachments/assets/5535fa95-39c5-49b7-98d7-8f0c1c3cc9f7" />

  使用后:

  <img width="1735" height="920" alt="image" src="https://github.com/user-attachments/assets/110a17ab-c4f0-4887-b4e5-16cdb91b5fc5" />

  但需要注意的是，栈空间和堆空间都有一定的大小，并且没有计算机那么富裕，如果开的范围太大，那么编译器会报错。大小可以在startup_stm32f10x_hd.s文件中查询

  <img width="624" height="405" alt="image" src="https://github.com/user-attachments/assets/937509bc-7c53-4994-9d5e-278db8cf8d0c" />

  下图展示数组开太大后爆栈时编译器报错
  
  <img width="1892" height="976" alt="image" src="https://github.com/user-attachments/assets/68ba9f10-dbf3-4cd0-bbc8-0b2018dbf8e7" />

  注意：经过我本人的测试，理论上我开malloc(sizeof(u8)*1024)就已经堆溢出了，但是编译器并没有报错，现在不知道是不是自己的问题，大家也可以试一试。大家在使用堆的时候，记住一次开空间不要超过startup_stm32f10x_hd.s文件内的堆空间，并且用完后及时free()掉。

  整理一下队形，我们刚刚讲build output打印出来的信息，谈到了 Program Size: Code=1176 RO-data=320 RW-data=0 ZI-data=1024，展开讲了各个数据的具体含义。

  回到build output打印出来的信息

  FromELF: creating hex file... ----- MDK用了fromelf生成了.hex文件，MDK就可以用.hex文件烧录到我们的stm32里面。

  After Build - User command #1: fromelf --bin --output ..\..\Output\流水灯.bin ..\..\Output\流水灯.axf -----这一步不是MDK自动生成的，而是人为加上去的。这一句我们先放在这，暂时不去讲，后面提到再说，大家会理解更加深刻一点。

  看完上述文字后，你应该对编译过程中的各个操作的含义有了一定的了解。本质上，build output 打印出来的东西流程就是MDK的工作流程。


  不知道你们在学习RW-data和ZI-data是否会有一个疑问，RO-data和ZI-data仅仅是初始值不一样而已，为什么编译器要把他俩分开呢？通过这个现象，我们可以管中窥豹程序的存储和运行。

  在讲解之前，必须先重温一下俩种核心的存储元件:ROM和RAM

  ROM：Read-Only Memory，只读存储器，用于长期存储不可修改的数据，掉电后数据不丢失。常见的类型有：
  
    EEPROM（电可擦除 PROM）：支持多次电擦写，适合存储配置参数。
    Flash ROM：快速电可擦写，如 STM32 内部 Flash（主流嵌入式设备使用）。

  RAM:Random Access Memory,随机存取存储器，用于临时存储程序运行时的数据，支持快速读写。常见类型有：
  
    SRAM（静态 RAM）：速度快，无需刷新，但成本高、集成度低。
    DRAM（动态 RAM）：需定期刷新，成本低、容量大，常见于 PC 内存。
    SDRAM（同步 DRAM）：与系统时钟同步，常用于嵌入式系统扩展内存。

  ROM和RAM的对比如下：

  <img width="594" height="328" alt="image" src="https://github.com/user-attachments/assets/a2a3c83a-d5aa-495c-8060-570052c20f32" />

  现在，请根据ROM和RAM的特性，假设你是设计者，为了不每次开机都要重新烧录一遍，你要把哪些数据存储到哪些地方？

  Code代码域肯定要放在ROM区域吧，不然stm32如何执行呢

  RO-data也得放在ROM区吧，毕竟是恒定的数据，你总不能掉电后它存储的值就丢失了，那下一次上电数据不久发生了变化了吗

  ZI-data那包得放在RAM区的呀，反正你的初始值为0，就算丢不丢失，你上电后的值永远为0，那我还不如放在RAM区，访问数据还快很多

  倒是RW-data倒是犯了难了，RW-data初始值不为0，为了保证上电后数据不丢失，那你只能放在ROM区吧，但是RW-data和ZI-data一样，在程序中需要多次地被调用访问，你要是放在ROM区，那么速度可太慢了，这该怎么办呢

  这就涉及到程序的存储和运行了，前面我们的猜想和设计一样，唯一有区别的就算RW-data的处理。在存储的时候，为了保证掉电不丢失，先是放在了ROM区，等到运行的时候，再把拷贝一份到RAM区，流程图如下：

  <img width="741" height="299" alt="image" src="https://github.com/user-attachments/assets/015c0e91-c992-4585-a494-e7d6e527b5e3" />

  这张图的ROM区还漏了Code，但无伤大雅。

  至于为什么不把RO-data也不一起拷贝过去，这样不是能加快其访问速度吗。我感觉是有俩种考虑。第一：RAM的空间有限，挤不下了;第二个是RO-data的访问频率没RW-data大，因此把他放在ROM区也无妨。

  总结：当程序存储到 STM32 芯片的内部 FLASH 时 (即 ROM 区)，它占用的空间是 Code、RO-data 及RW-data 的总和，所以如果这些内容比 STM32 芯片的 FLASH 空间大，程序就无法被正常保存了。当程序在执行的时候，需要占用内部 SRAM 空间 (即 RAM 区)，占用的空间包括 RW-data 和ZI-data。图示如下：

  <img width="809" height="180" alt="image" src="https://github.com/user-attachments/assets/e729a57a-0067-4b5f-b969-5bc499784648" />

  学习完上述部分，你应该就理解了程序的存储和运行

  下面来讲解各个编译器单独能干什么操作。

  下面来讲解各个编译器单独能干什么操作，对于MDK魔法棒的哪些操作。

  为了方便我们在任意一个文件夹都可以使用各个编译器，我们需要配置路径，这里以window11为例

  在系统中搜索"环境变量"

  <img width="1111" height="827" alt="image" src="https://github.com/user-attachments/assets/ca99b145-403f-44a2-b8e5-082a58b9a902" />

  点击"编辑用户环境变量"

  选择"path"，大小学都可以

  <img width="694" height="332" alt="image" src="https://github.com/user-attachments/assets/a9e77ba1-3c9c-44b4-bab3-f198c9a7b841" />

  点击编辑，弹出窗口后再点击编辑，把  *** Using Compiler 'V5.06 update 7 (build 960)', folder: 'D:\keil 5.36\Keil_v5\ARM\ARMCC\Bin' 里的地址复制进去，点击确定。(添加的时候不要加任何符号，只要地址，和window 7的配置有点区别)

  <img width="742" height="671" alt="image" src="https://github.com/user-attachments/assets/d483f3a4-cfe4-41e7-9750-5c4996c6c720" />

  然后在任意的文件夹中，在搜索地址的地方输入cmd,输入formelf，如果成功弹出，那就说明配置路径成功。

  <img width="1914" height="950" alt="image" src="https://github.com/user-attachments/assets/e7076260-7a02-40ad-b966-0a8f58735e95" />

  <img width="1742" height="898" alt="image" src="https://github.com/user-attachments/assets/0ca3c047-d848-43a5-911c-bed82df70cd0" />

  开始介绍各个编译器

  1.armcc:用于把 c/c++ 文件编译成 ARM 指令代码，编译后会输出 ELF 格式的 .o 文件。

  打开armcc编译器，上面有使用格式usage和一系列的指令说明option
  
  <img width="1492" height="753" alt="image" src="https://github.com/user-attachments/assets/8c699bbf-ede8-4e53-9c00-be5c8ce6f3bc" />

  如果你想试试单独作用armcc，拷贝一个.c文件到一个文件夹，然后输入指令，你大概率会遇到这样的情况。

  <img width="1475" height="773" alt="image" src="https://github.com/user-attachments/assets/3c3e480c-535b-4d09-bea3-8ab7561804b6" />

  为什么呢？因为一堆option你还没选择，包括但不限于:开始时这个文件的类型，你要转化成哪种芯片的机器码，是否生成用于高级调试的表，编译器自动优化等级等等

  所以这个就不是给人操作的，是给MDK操作的

  这个编译器的配置对应的是MDK魔法棒的C/C++模块，如图所示：

  <img width="670" height="495" alt="image" src="https://github.com/user-attachments/assets/3ba3be8a-ec2f-41aa-8d9a-d8deeeae512a" />

  这里简单依次讲解:

  Preprocessor Symbols（预处理符号）:
  
    Define：定义预处理宏，多个宏之间用逗号或空格分隔 
    Undefine：取消定义某些宏。该输入框通常保持为空，除非有特殊需求需要取消某个已定义的宏

  Language / Code Generation（语言 / 代码生成）:只讲可能会用到的，大部分其实都用不到

    Optimization：优化等级选择，有 Level 0 (O0)、Level 1 (O1)、Level 2 (O2)、Level 3 (O3) 等选项。
      Level 0 (O0) 表示不进行优化，编译速度快且调试信息完整，方便调试；
      Level 3 (O3) 则是最高级别的优化，会尽可能优化代码，减少代码体积、提高运行速度，但可能会使调试变得复杂，因为优化后的代码结构和原始代码差异较大。

    Optimize for Time：勾选后，编译器会优先考虑优化代码的执行时间，以提高程序的运行效率，通常配合较高的优化等级使用。

    C99 Mode：勾选后，编译器将支持 C99 标准的部分特性，比如新的关键字、变长数组等。如果项目使用了 C99 的特性，需要勾选此选项。

    One ELF Section per Function：编译器会为每个函数生成独立的 ELF 节，便于链接优化和调试便利。
    如果对代码的优化、调试便利性有较高要求，且对文件大小和编译时间增加的容忍度较高，可以勾选，
    如果项目对可执行文件大小极为敏感，或者函数数量较少，编译时间已经是关键因素，那么不勾选该选项可能更为合适。

  其余可以通过百度自行查询，也可以查询MDK的帮助手册，但是全是英文且废话多，比较难懂

   Include Paths（包含路径）：用于指定编译器查找头文件的路径

   Misc Controls（杂项控制）：一般为无

   Compiler control string：由上述配置自动拼接生成，也就是输入到armcc的命令，我们要按照这样的格式输入，才能得到.o文件。你完全可以把inerwork后面的文件夹换成对应的文件试一试，

   <img width="1475" height="774" alt="image" src="https://github.com/user-attachments/assets/8bcf7d41-19dd-4b6a-a7c3-3179aaa58c38" />
  
  只是不会出现前面的报错，而是出现了文件找不到的情况。

  所以这个编译器不是给人正常操作的...,我们用的也不多

  接下来看armasm，和armcc的功能差不多，把.s文件转为.o文件，对应的是魔法棒的Asm模块

  <img width="708" height="516" alt="image" src="https://github.com/user-attachments/assets/935af966-0f07-47ff-8f4f-2eb3921e29df" />

  这里简单依次讲解:

  Conditional Assembly Control Symbols（条件汇编控制符号）

    Define：用于定义汇编阶段的预处理宏
    Undefine：用于取消汇编阶段的预处理宏

   Language / Code Generation（语言与代码生成）

     Thumb Mode（Thumb 模式）：强制汇编器生成 Thumb 指令集（ARM 指令集的 16 位精简版）
     减小代码体积（Thumb 指令更短），适合 Cortex-M 系列（仅支持 Thumb/Thumb-2）。但实际测试开不开都无所谓

     No Warnings（关闭警告）：启用后，汇编器会忽略所有警告信息（如语法不严谨、符号未使用等）

  Include Paths（头文件搜索路径）：作用：指定汇编器查找汇编头文件（如 .inc、.s 依赖的文件）的路径。 但是我发现即使不添加，按照启动文件的配置也可以正常烧录程序，这个我也不知道为什么，网上也查不到相关信息

  Misc Controls（杂项控制）：手动输入汇编器的额外参数（覆盖默认配置）。

  Assembler control string（汇编器控制字符串）：由上述配置自动拼接生成，也就是输入到armasm的命令，我们要按照这样的格式输入，才能得到.o文件。

  简而言之，这个也不是给人操作的。
  
  接下来介绍armlink，armlink是链接器，它把各个 O 文件链接组合在一起生成 ELF 格式的 AXF 文件，AXF 文件是可执行的，下载器把该文件中的指令代码下载到芯片后，该芯片就能运行程序了；利用 armlink 还可以控制程序存储到指定的 ROM 或 RAM 地址。

  对应的是魔法棒的Linker模块
  
  <img width="723" height="505" alt="image" src="https://github.com/user-attachments/assets/81fb8d72-78be-447d-99e5-4997ba50d230" />

  下面介绍主要配置

   Use Memory Layout from Target Dialog：启用后，链接器优先使用 Target 选项卡中配置的 内存布局（如 IRAM、IROM 的起始地址和大小）

   Report 'might fail' Conditions as Errors：将链接器的 “潜在失败警告” 升级为错误（强制中断编译）。

   如果你开启了 Use Memory Layout from Target Dialog，那么后面的都不用管了，因为RO/RW-data分配到哪里，MDK已经根据之前target配置的值自动分配好，但是如果你想单独把某些数据存储到某些区域，进行更加细致的分配，那你需要不勾选Use Memory Layout from Target Dialog，并且打开.sct文件进行配置。如何配置这个后面再提。

  X/O Base：配置 扩展 / 外部存储区 的基地址（极少用，常规开发依赖 Target 配置）。
  
  R/O Base：配置 只读存储区（RO） 的基地址（如 Flash 起始地址，默认与 Target 同步）。
  
  R/W Base：配置 可读写存储区（RW/ZI） 的基地址（如 RAM 起始地址，默认与 Target 同步）。

  Scatter File（分散加载文件）：用自定义脚本（.sct 文件）替代 Target 配置，实现复杂内存布局。(如果开启了 Use Memory Layout from Target Dialog ，那这个无需配置;如果未开启，那需要手动配置，如何操作后面再提)

  Misc controls（杂项控制）：手动添加链接器参数（覆盖默认配置）

  Linker control string（链接器控制字符串）：由上述配置自动拼接生成，也就是输入到armlink的命令，我们要按照这样的格式输入，才能得到.axf文件。

  总的来说，armlink也不是给人操作的

  那tm到底哪些是给人操作的？至少咱能用到吧

  有的兄弟，接下来fromelf用到的就比较多了

  对于fromelf编译器的配置，MDK反而没有集成多少，只有生成HEX的选项，而对于另外生成.lib的选项，是另外一个编译器armar的。

  我们之前说过，有些烧录软件是不支持.hex文件的，需要.bin类型或者.i32类型的文件，这时候我们可以手动用fromelf帮助我们生成

  <img width="1427" height="728" alt="image" src="https://github.com/user-attachments/assets/beabccd5-b67e-4309-b12d-c39003c772e2" />

  <img width="884" height="269" alt="image" src="https://github.com/user-attachments/assets/e79ea70f-6e8a-4fc5-8033-19e423b08aac" />

  当然了，MDK虽然没有全部集成，但是是可以手动配置的，在魔法棒的User里面配置

  <img width="718" height="519" alt="image" src="https://github.com/user-attachments/assets/7282fac8-4481-4701-934f-f7ff1749cdef" />

  输入的命令和直接输入到fromelf的指令差不多，就是文件地址得改成全局的地址，这就是为什么最开始先配置环境路径的原因，每次使用都得输入绝对路径那还了得。

  路径以keil 5的程序所在地址开始，.\为同一级文件夹，..\为上一级文件夹。

  看完上述，对各个编译器能独立地做些什么和MDK魔法棒各个配置的含义应该有了一个全面的了解

  我们除了output/objects里面的文件类型以外，在project中也有几个类型的文件，这里我直接附上一张表格，方便对应

  <img width="508" height="633" alt="image" src="https://github.com/user-attachments/assets/53397863-9623-4d4a-840a-24d8d4d6cf64" />

  具体哪个文件有什么作用，这个在野火的开发手册里面讲的很清楚，我已经没有能力比他讲地更加优秀，要想了解各个文件具体的作用，请查阅野火的开发指南或者b站上野火官方高级篇

  这里我只提及可能大家未来会用到的

  如果，我想查看各个文件的具体信息，我要怎么查看呢

  这些文件的具体信息是ELF文件头，ELF 文件头用来描述整个文件的组织，例如数据的大小端格式，程序头、节区头在文件中的位置等。所以查看ELF可以很方便地区查看各个文件的关键信息。

  <img width="780" height="483" alt="image" src="https://github.com/user-attachments/assets/de950317-03b7-422a-94df-7d68bb5a7654" />


  这时候我们可以利用fromelf去查看，输入fromelf –text –v bsp_led.o(.axf文件也可以打印出ELF文件头)指令，就可以打印出bsp_led.o的详细信息，但我们一般把信息直接存储到一个记事本里面，方便查看。

  <img width="1468" height="870" alt="image" src="https://github.com/user-attachments/assets/cf69c325-27d0-47a1-bc72-1ecbee76ed1d" />

  这里面数据意思如下：

  <img width="845" height="840" alt="image" src="https://github.com/user-attachments/assets/5a779f6b-971e-4445-9adf-29ac7c83d5a6" />

  <img width="842" height="347" alt="image" src="https://github.com/user-attachments/assets/51403959-96ea-41d3-9c23-07b2d9b5123d" />

  <img width="845" height="154" alt="image" src="https://github.com/user-attachments/assets/9e3c17b3-fe43-4520-8543-b6150dca72c3" />

  要想查看各个函数的具体详细，需要勾选“Options for Target ->C/C++ -> One ELF Section per Function”中的选项，生成的 *.o 文件内部的代码区域才会与 C 文件中定义的函数名一致，否则它会把多个函数合成一个代码段，名字一般跟 C 文件中的函数名不同。

  下面是.axf文件的ELF生成的具体信息。

  <img width="844" height="484" alt="image" src="https://github.com/user-attachments/assets/fa02a101-0853-40a6-a697-fba6417a836b" />

  <img width="666" height="757" alt="image" src="https://github.com/user-attachments/assets/616dd321-4175-4f5a-a286-97472db729b4" />

  <img width="676" height="569" alt="image" src="https://github.com/user-attachments/assets/5440e821-bbe9-4cdb-a1bd-e566082a89ed" />

  <img width="684" height="351" alt="image" src="https://github.com/user-attachments/assets/69b872e8-838f-4ce8-bbe7-c85dbdce6cf1" />

  我们大概率在实践中最有可能就是用到这么多，其他的有需要再了解吧

  所以我们能理解的是，axf文件有大量的信息，且直接使用 fromelf即可反汇编代码，所以更不要随便泄露 axf 文件。lib 文件也能反使用 fromelf 文件反汇编代码，不过它不能还原出 C 代码，由于 lib 文件的主要目的是为了保护 C 源代码，也算是达到了它的要求。

  那可是这样也麻烦，我只想想看看哪些文件是否正确调用，然后Code，RO-data各个文件是多少，没有简单点的方法吗。还是有的，这个需要打开我们的.map文件

  双击"Project"下面的的一个文件夹，就能打开map

  <img width="217" height="81" alt="image" src="https://github.com/user-attachments/assets/49052823-a282-4d46-98cb-0076769783d4" />

  上面有哪些文件被移除，哪些函数被调用;以及还有各个函数的起始地址;最下面还要每个文件的Code等数据的信息;最好还有个汇总信息.

  根据这些，你能发现哪些函数的哪些数据被编译器优化，通过对比就可以发现了

  最后再讲解一个文件，.sct文件

  也就是我之前没填的坑，如果我想自由手动的存储数据到某个地址，就需要在.sct文件进行修改

  若要启用sct文件，需要关闭Linker的Use Memory Layout from Target Dialog

  打开output/objects文件夹，找到.sct文件，拖至keil5里面即可编辑

  其编辑格式如下：
  
  <img width="694" height="688" alt="image" src="https://github.com/user-attachments/assets/03dae55c-4c21-46c4-b459-31b001a6a8c0" />

  例子如下：
  <img width="701" height="349" alt="image" src="https://github.com/user-attachments/assets/1de68f79-7d27-4c70-9027-009b455e2c87" />

  书写格式如下:

  <img width="721" height="196" alt="image" src="https://github.com/user-attachments/assets/450522a6-403d-4b8c-bff2-2593529d0e62" />

  <img width="712" height="132" alt="image" src="https://github.com/user-attachments/assets/fd643268-201a-4202-8971-e0a7d3aaade9" />

  模块选择样式：模块选择样式可用于选择 o 及 lib 目标文件作为输入节区，它可以直接使用目标文件名或“*”通配符，也可以使用“.ANY”。例如，使用语句“bsp_led.o”可以选择bsp_led.o 文件，使用语句“.o”可以选择所有 o 文件，使用“.lib”可以选择所有 lib 文件使用“*”或“.ANY”可以选择所有的 o 文件及 lib 文件。其中“.ANY”选择语句的优先级是最低的，所有其它选择语句选择完剩下的数据才会被“.ANY”语句选中。

  输入节区样式：我们知道在目标文件中会包含多个节区或符号，通过输入节区样式可以选择要控制的节区。示例文件中“(RESET，+First)”语句的 RESET 就是输入节区样式，它选择了名为 RESET 的节区，并使用后面介绍的节区特性控制字“+First”表示它要存储到本区域的第一个地址。示例文件中的“(InRoot$$Sections)”是一个链接器支持的特殊选择符号，它可以选择所有标准库里要求存储到 root 区域的节区，如 __main.o、__scatter.o 等内容。

  输入符号样式：同样地，使用输入符号样式可以选择要控制的符号，符号样式需要使用“:gdef:”来修饰。例如可以使用“*(:gdef:Value_Test)”来控制选择符号“Value_Test”

  输入节区属性：通过在模块选择样式后面加入输入节区属性，可以选择样式中不同的内容，每个节区属性描述符前要写一个“+”号，使用空格或“，”号分隔开，可以使用的节区属性描述符见表属性描述符及其意义 。例如，示例文件中使用“.ANY(+RO)”选择剩余所有节区 RO 属性的内容都分配到执行域ER_IROM1 中，使用“.ANY(+RW +ZI)”选择剩余所有节区 RW 及 ZI 属性的内容都分配到执行域 RW_IRAM1 中。

  节区特性：节区特性可以使用“+FIRST”或“+LAST”选项配置它要存储到的位置，FIRST存储到区域的头部，LAST 存储到尾部。通常重要的节区会放在头部，而 CheckSum(校验和)之类的数据会放在尾部。例如示例文件中使用“(RESET,+First)”选择了 RESET 节区，并要求把它放置到本区域第一个位置，而 RESET 是工程启动代码中定义的向量表，该向量表中定义的堆栈顶和复位向量指针必须要存储在内部 FLASH 的前两个地址，这样 STM32 才能正常启动，所以必须使用 FIRST 控制它们存储到首地址。

  <img width="797" height="262" alt="image" src="https://github.com/user-attachments/assets/a8eb55c8-bcde-490b-8673-e66dad1139d8" />

  以这个为例，讲一下这个sct文件讲的什么内容

  程序的加载域为内部 FLASH 的 0x08000000，最大空间为 0x00080000；程序的执行基地址与加载基地址相同，其中 RESET 节区定义的向量表要存储在内部 FLASH 的首地址，且所有 o 文件及 lib 文件的 RO 属性内容都存储在内部 FLASH 中；程序执行时 RW 及 ZI 区域都存储在以 0x20000000 为基地址，大小为 0x00010000 的空间 (64KB)，这部分正好是 STM32 内部主 SRAM 的大小。

  我觉得要是这样也挺麻烦的，我要找相关的芯片类型，翻阅它们的参考手册，还需要确定ROM区和RAM区的大小，最后还需要手动分配...

  确实，但是这个MDK已经帮助我们集成好了，芯片的选择我们需要配置魔法棒的Device选项，选择好芯片，自动就能确定ROM和RAM的大小

  <img width="630" height="475" alt="image" src="https://github.com/user-attachments/assets/30c3b684-742d-414e-a75a-8dd5de750f94" />

  如果自己懒得开加载域，感到麻烦呢？这个也可以通过MDK配置进行分区，在魔法棒的Target区域，有外部ROM/RAM和内部ROM/RAM的分区，只需要你输入起始地址和大小，MDK会自动生成.sct的加载域的分区

  <img width="663" height="496" alt="image" src="https://github.com/user-attachments/assets/cc52b7d9-f2a8-4f5b-a100-22e4f8be156c" />

  然后你想把哪些数据放在哪里，那就是需要自己配置的事情了。

  除此之外，我们也可以右键特定的文件把他放到相应的区域

  <img width="545" height="646" alt="image" src="https://github.com/user-attachments/assets/e6fb76a7-e01d-49de-b9e2-393d6646068a" />

  修改完后对应的文件会有个小雪花
  
  但是这样单独修改比较麻烦，不常用，我还不如在.sct文件通用修改

  好了，基本上会用到的文件已经全部讲解完毕。


