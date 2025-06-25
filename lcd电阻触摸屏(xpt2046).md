  这一节也是需要学会移植就可以了，不需要掌握其原理。移植前需要注意，我这是电阻触摸屏，可能不适用于电容触摸屏。

  这里我讲一下LCD显示屏和电阻触摸屏的函数调用逻辑，方便大家理解如何移植。

  主函数逻辑:调用初始化函数，

  接着调用校准函数:Calibrate_or_Get_TouchParaWithFlash(uint8_t LCD_Mode,uint8_t forceCal),一般forceCal我们选择0.
  ![image](https://github.com/user-attachments/assets/c01fb467-ba3e-45c2-affb-503cc24d9ea0)

  然后在循环中调用bsp_xpt2046_lcd.h头文件的XPT2046_TouchEvenHandler()函数进行识别是否有触摸。

  程序就会在反复调用XPT2046_TouchEvenHandler()函数，在这个函数中会有调用俩个关键函数：XPT2046_TouchDown(&cinfo)和XPT2046_TouchUp(&cinfo)，这俩个函数分别是"按下触摸屏的函数"和"松开触摸屏的函数"，我们要在这里编写自己的
程序。

  为了方便移植，我们并不会直接在这俩个函数里面编写按下/松开触摸屏后的程序显示，而是又嵌套一层，我们创建头文件application.c和.h，在这里面我们编写Touch_Button_Down(uint16_t x,uint16_t y)
和Touch_Button_Up(uint16_t x,uint16_t y)函数，编写完成后我们在bsp_xpt2046_lcd.c头文件中引用application.h文件，并且把对应的函数放到XPT2046_TouchDown(&cinfo)和XPT2046_TouchUp(&cinfo)函数中
![image](https://github.com/user-attachments/assets/eddfa1ef-80ba-4311-8153-f5a2a4c04d06)

![image](https://github.com/user-attachments/assets/d9ed8206-b9c4-4947-8747-c5d75c847f9c)


  其中cinfo是一个枚举类型，含有的参数如下:
![image](https://github.com/user-attachments/assets/e7c391e9-d678-4876-90dc-5d3bdbeaadd5)


  这便是整个程序的运行逻辑，所以我们要编写自己的LCD显示屏和触摸程序，我们主要编写的函数为Application_Init(void);Touch_Button_Down(uint16_t x,uint16_t y);Touch_Button_Up(uint16_t x,uint16_t y);当然也可以适当地增加函数
这个由你来决定。

  如果还有疑惑，可以参考以下我的main函数和application文件

  还是那句话，能不烧录就不烧录，做的实在太唐了



