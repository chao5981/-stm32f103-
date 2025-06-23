  这一章的程序不需要自己写一遍以加深记忆，实际上，这种显示屏网上开源的很多，我们只要拿到一个相同型号的显示屏芯片的开源代码，学会移植并且能大致地看懂器代码含义就可以了。

  现在我讲一个移植的时候大家大概率会碰到的情况，FSMC模拟8080时序的时候ADDSET和DATAST如何确定的问题。

  首先，通过对比我们可以发现，8080时序和FSMC的B模式非常相似，见下图：
  ![image](https://github.com/user-attachments/assets/4923ef0c-2094-4439-a0d8-3c3f570f5fb9)
  ![image](https://github.com/user-attachments/assets/5198a5e6-6173-44ef-a607-3a84a1c67d95)
  ![image](https://github.com/user-attachments/assets/4378708f-2275-4be5-9a7c-478dfabfcc88)


A[25:0](地址线，在这里只需要任选一条地址线作为D/CX线即可)对应D/CX线(数据/命令选择线，低电平表示命令，高电平表示数据)

NEx(片选信号)对应CSX(片选信号)

NOE(读使能)对应RDX(读使能)

NWE(写使能)对应WRX(写使能)

D[15:0](数据线，这里都需要接上)对应D[17:0](数据线)，可以发现这是并行传输

模式B的NADV的变化不用担心，直接不让他连着ILI9431的引脚就完事，这样就不会影响到8080时序。让它自个玩去吧

现在我们需要配置ADDSET和DATAST这俩个值，根据我上一节说的，我们要找类似于t_SU(data)和t_SU(addr),但由于这里是模拟，所以找不到t_SU(data)和t_SU(addr)这样的字眼

这时候我们就要对时序了，让我们看8080和FSMC的NOE和RDX;NWE和WRX。

