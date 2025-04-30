I2C的基本通信原理，我想必大家在C51系列已经有了初步的了解

但是在C51中，我们主要是学习了I2C协议发送数据的时序是如何编写的，而在固件库中，这个时序已经被封装在了I2C_SendData()和I2C_ReceiveData()里面了。

在实际工程中，我们通常需要连续写；连续读；写入小数；读出小数等处理，光靠调用I2C_SendData()和I2C_ReceiveData()函数很明显是有点麻烦了，因此，我们还需要把它封装一下。

在编写之前，我们先看看在stm32中I2C的框架图
![image](https://github.com/user-attachments/assets/bf95f9d9-487c-430e-8dc4-a1abcbdbef12)

引脚还是和C51的一样，一个是SDA用于数据的传输和读取;一个是SCL用于控制时间。而且不难发现在SCL下面连着中断，在实际工程运用当中，I2C一般是写在中断中进行收发数据的，这样可以极大的减少CPU的占用(因为等待事件while循环会一直卡在哪里，后面你将会认识到),在这里我就直接写在主函数里了，如果大家有时间可以自己尝试一下。

I2C有主模式和从模式，主模式就是stm32作为主机，掌握着数据收发大权，它说收就收，说发就发，大家都得听他的(你可以理解为S);从机模式就是stm32为被接受者，别人要求你收数据你就收，要求你发你就得发，它得听别人的(你可以理解为M)。在这个实验中，我们是以stm32为主机，EEPROM外设为从机。

这里我附上I2C主模式的时序图，由于这个是简单的时序图，遗漏了一些关键的细节，接下来我会进行讲解
![image](https://github.com/user-attachments/assets/00a27496-ce2c-48a1-835b-c06ae82b6fd5)
1.调用I2C_GenerateSTART()，发送起始信号

2.设置一个变量I2CTimeout，令其等于I2CT_FLAG_TIMEOUT(宏定义为((uint32_t)0x1000))，并用while循环调用I2C_CheckEvent()函数等待EV5事件，如果响应了则跳出while循环，如果没有直接退出，防止死等。

3.接着发送"大地址"，因为一个固件可能含有多个EEPROM，你得告诉编译器你得存在哪里，这时候需要调用I2C_Send7bitAddress()

4.设置一个变量I2CTimeout，令其等于I2CT_FLAG_TIMEOUT(宏定义为((uint32_t)0x1000))，并用while循环调用I2C_CheckEvent()函数等待EV6事件，如果响应了则跳出while循环，如果没有直接退出，防止死等。

5.注意，这就是和时序图不一样的地方，在这里还需要发送"内部地址",也就是告诉编译器你存地址存在EEPROM里面的哪个位置。你或许会疑惑，难道不是在流程图中的"地址"那一块就直接发送了"大地址"和"内部地址"不可以吗。实际上是不行的。

6.设置一个变量I2CTimeout，令其等于I2CT_FLAG_TIMEOUT(宏定义为((uint32_t)0x1000))，并用while循环调用I2C_CheckEvent()函数等待EV8事件，如果响应了则跳出while循环，如果没有直接退出，防止死等。




![image](https://github.com/user-attachments/assets/178743d4-8b5d-42ec-83ce-5e44775f55f5)
  
