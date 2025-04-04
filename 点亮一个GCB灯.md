  有了GPIO的理论知识，让我们开始通过调用HAL去点亮一个GCB灯吧

  
  1.让我们先打开原理图，查看GCB灯连接的引脚
  ![image](https://github.com/user-attachments/assets/ec04224a-4dcd-4008-8dff-c53d7ee674b9)
  知道了其GPIO的引脚和类型，接下来我们要对其寄存器进行配置
  
  2.先配置时钟
  由于GPIO被APB2时钟控制，我们找到对应的启动APB2时钟的库函数
  ![image](https://github.com/user-attachments/assets/f024be46-0deb-40e6-81bd-5103496a9040)
  根据其说明传入相关形参

  3.确定要配置的寄存器
  将GPIO的原理图扔给Deepseek，问需要如何做才能让这个LED灯点亮
  得知，我们需要配置CRL寄存器，在调用库函数里面的位操作函数即可，并且这个灯是低电平点亮

  4.找到配置寄存器相关的函数
  CRL和CRH的库函数都是GPIO_Init()进行配置的，下面是这个库函数的内容
  ![image](https://github.com/user-attachments/assets/87a382f4-479b-42a4-bc1b-452ec686f8b2)
  按着函数简介输入形参配置即可
  配置完过后，其实灯本质上已经可以亮了；那么我们要如何控制灯的点亮和熄灭呢，这时候我们需要用到BSRR寄存器
  ![image](https://github.com/user-attachments/assets/34023f2a-faa8-4192-8af3-f3bf9e266b46)
  按着函数简介输入形参配置即可

  5.编写主函数
  先配置时钟，然后配置CRL寄存器，然后在通过GPIO_SetBits()和GPIO_ResetBits()去控制GCB灯的点亮和熄灭。

  参考代码可以看这个分支的main.c,GCB.h,GCB.c文件，经过了规范化，企业化的编写，仅供大家参考，有问题还望大家多多指点。

  备注：有些参数大家可能看不明白，或者不知道这个参数是什么类型，大家可以打开我的main分支，下载我的stm32f10x_stdperiph_lib_um文件，找到相关函数后点击参数，会有详细的讲解
  
  
