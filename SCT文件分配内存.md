  学习本节之前，请先回到main学习MDK文件类型全解.md

  在上面的学习中，我们只是提到了sct文件的作用，但是没有提到如何分配内存，这节来详细讲讲

  首先，我们需要在魔法棒的Linker部分取消勾选 use memory layout from target dialog选项，这样再次点击烧录，就可以出现sct文件。或者自己创建一个也可以，但是得和project的名字一样

  <img width="677" height="524" alt="image" src="https://github.com/user-attachments/assets/09c1a650-987f-453b-ad12-0dbd13fe494f" />

  我们在分区的时候，不能超过其ROM和RAM的地址范围。由于这里没有外部SRAM，所以只能用内部RAM代替，但其实就是地址不同而已，用起来没有多大的区别

  这里分别把STACK和HEAP放在不同的区域。然后RW和ZI是系统自动选择放的，系统会优先把RW和ZI数据放在内容更大的RAM区域

  这个实验的逻辑是，对RAM分区，然后分别把栈区和堆区放在不同的区域，最后打印出来看看是否配置sct文件成功

  主函数就打印即可

  

  
