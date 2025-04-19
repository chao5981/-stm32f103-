github无法直接上传这个文件，所以只能通过百度网盘传了，好在不大，可以很快的下载完
通过网盘分享的文件：stm32f10x_stdperiph_lib_um.chm
链接: https://pan.baidu.com/s/1uHHZsLyRtJxb9SFH8euz8Q 提取码: 3kmi

下载完毕后，大家看到纯英文的东西和一大堆的文件夹一点会晕，这他喵的什么东西？别急，让我教你如何使用和查找相关的库函数
打开主页，可以不用从左边的一大堆文件开始翻阅，点击data structures
![image](https://github.com/user-attachments/assets/fbf9a7b2-32bf-4c12-be2d-ab9715f6c85f)
这里有全部的初始化的函数，这里我们以GPIO为例子，点击GPIO_InitTypeDef
![image](https://github.com/user-attachments/assets/134849c3-7550-4147-80c8-d7bddf949616)
点进去后，点击GPIO_Exported_Types
![image](https://github.com/user-attachments/assets/57e0db0f-a8b3-4403-bfc3-ee6acb5a6d15)
点进去后，再点击GPIO
![image](https://github.com/user-attachments/assets/7e5634f3-906f-4330-b03d-f1f360230da9)
然后会弹出这个界面，我依次给你翻译一下其中的意思
![image](https://github.com/user-attachments/assets/d4f05c3d-9098-4454-8baa-f7e865e5a4c2)
GPIO Exported Constants：GPIO 导出常量
GPIO Exported Macros：GPIO 导出宏
GPIO Exported Functions：GPIO 导出函数
GPIO Private TypesDefinitions：GPIO 私有类型定义
GPIO Private Defines：GPIO 私有定义
GPIO Private Macros：GPIO 私有宏
GPIO Private Variables：GPIO 私有变量
GPIO Private FunctionPrototypes：GPIO 私有函数原型
GPIO Private Functions：GPIO 私有函数
我们可以根据需要，从这里进入去查找GPIO定义的宏，#define预处理，还有函数

例如我们要查找函数，我们点击GPIO Private Functions
![image](https://github.com/user-attachments/assets/f5201bd3-ec68-4c68-81da-625d74412dfa)
弹出这个，例如我们要看GPIO_Init(),我们就点击他
![image](https://github.com/user-attachments/assets/b13f0193-8db0-466b-b6a1-a5d12b115690)
页面就自动跳转到这个函数的定义，例如，我们一开始不知道传入的参数类型，就看Parameters，
![image](https://github.com/user-attachments/assets/685fbff1-6c4b-4ecc-a310-d04aca2e70a5)
像GPIOx,Parameters就说的很明白，只需要在GPIOA,GPIOB...GPIOG中选就可以了，但 GPIO_InitStruct还是不清楚，那咱就点击参数，以GPIo InitTypeDef为例
![image](https://github.com/user-attachments/assets/30f5aeb2-0ede-4eaa-b886-783e19d8691b)
发现这个参数里面就这几种类型，也就是说，创建相关的结构体，然后选择配置好这些参数就OK了
![image](https://github.com/user-attachments/assets/ef81a7c6-1d17-4308-b83b-f24444b9d9f6)
但是我不知道该怎么给他赋值阿，你别急，以GPIO_Mode为例，点击GPIOMode_TypeDef 
![image](https://github.com/user-attachments/assets/d2cb60c4-bff9-422d-9a56-f96be8d9e2f2)
enum类型的Enumerator就是可以赋的值啦
![image](https://github.com/user-attachments/assets/33589a13-0fff-4359-9260-c19298757350)
主播主播，可是我不知道这说明意思阿，怎么办。
没事，懂这些的AI还有好几个，问他们是什么意思就行了

例如：GPIO_Mode_AF_PP 是片上外设输出; GPIO_Mode_Out_PP 则是GPIO 数据寄存器输出

![image](https://github.com/user-attachments/assets/7b590684-7199-4144-8d59-9aee2127c9c1)
如果你想看具体的头文件，那就点击底下的头文件就好了
![image](https://github.com/user-attachments/assets/1b9a9f7a-65af-40c6-82cb-3c326a5bc137)

好了，现在我能用到的就这么多，后面有需要的话我会再补充，希望对大家有帮助

补充1：一些函数找不到的问题

我们之前翻阅的是外设的私有函数，而遇到几个外设一起作用时，有些函数自然就不在私有函数里面了，我们需要去其外设的导出函数里面查找。
![image](https://github.com/user-attachments/assets/64a4745a-26ec-49e8-b034-1b8228e5df0f)




