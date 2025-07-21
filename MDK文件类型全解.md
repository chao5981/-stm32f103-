  此篇不要刚开始学stm32就打开看，可能看不懂，等你学完了stm32所有外设后，再回头来看这篇。
  
  在按照网上的教程设置完一个通用的文件夹，并熟练地用MDK创建应用程序，想必应该对MDK已经挺熟悉的了。

  但是MDK到底是怎么工作的呢？魔法棒的各种天花乱坠的配置是用来干嘛的呢？再打开output文件夹后，那么多各种后缀的文件是干什么用的呢？

  看完以下这章，你将学习到：

    MDK的工作原理
    编译过程中的各个操作的含义
    各个类型文件是什么意思
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

  RO-data：全称为Read Only data，即只读数据域，它指程序中用到的只读数据。换句话说，就是不可以修改的数据。例如你定义的const变量，这就属于RO-data。你在工程文件中定义一个const变量并且调用它，你再次编译会发现RO-data的值会
发生改变。

  未加入const前
  <img width="1915" height="996" alt="image" src="https://github.com/user-attachments/assets/ca71020a-b7fe-4fad-85cf-f7d0a32c0d88" />

  加入后
  <img width="1919" height="1078" alt="image" src="https://github.com/user-attachments/assets/e235cd0e-0fd0-4959-b65b-872b55b3e35b" />

  



  
