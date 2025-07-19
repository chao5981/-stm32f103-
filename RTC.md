  STM32 的 RTC 外设（Real Time Clock），实质是一个掉电后还继续运行的定时器。从定时器的角度来说，相对于通用定时器 TIM 外设，它十分简单，只有很纯粹的计时和触发中断的功能；但从掉电还继续运行的角度来说，它却是 STM32 中唯一一个
具有如此强大功能的外设。所以 RTC外设的复杂之处并不在于它的定时功能，而在于它掉电还继续运行的特性。

  下面是RTC外设的框架图：

  <img width="866" height="640" alt="image" src="https://github.com/user-attachments/assets/af871dda-dfcc-42e0-824f-1a85f965efe0" />

  看一眼就行，对我们实际帮助不大。

  现在根据这个图讲解RTC配置的主要要点。

  1.电源来源，我们去翻看前面的RCC框图，我们可以看到只有LSE和LSI时钟连接着RTC时钟。也就是说，RTC时钟理论上有俩个时钟源可以选择。但实际情况是，LSI是内部时钟，你stm32断电后RTC就不再计时;LSE是外部时钟，但是特么它经常晶振不起
振，也就是说它基本上只有开启内部时钟源LSI才能正常计数，断电就无法那个继续计数了。所以RTC时钟的用处就被彻底限制死了，它的归属就是在PWR待机模式中能继续计数，充当闹钟可以进行闹钟唤醒stm32.

  2.为了防止后备区域里的数据被意外修改，所以后备区域的寄存器默认写保护。所以要操作后备区域的寄存器时，在开启PWR和Backup时钟后，还需要使能允许访问backup区域。

  3.当允许访问backup区域后，由于时钟频率的差异，在在第一次通过 APB1 接口访问 RTC 时，必须等待 APB1 与 RTC 外设同步，确保被读取出来的 RTC 寄存器值是正确的。若在同步之后，一直没有关闭 APB1 的 RTC 外设接口，就不需要再次同步

  4.还是由于APB1和RTC时钟的差异，在内核发出写指令后，RTC 模块在 3 个 RTCCLK时钟之后，才开始正式的写 RTC 寄存器操作，因此每次操作完成后都需要进行等待。

  5.RTC时钟我们一般设置每一秒计时器计数一次。也就是说在设置频率的时候，由于我们选用的时LSI时钟源，LSI时钟源为40kHz，因此RTC分频为40000-1.

  6.RTC有三种中断方式，一种是秒中断，直接连接着NVIC;一种是闹钟中断，闹钟中断连接着EXTI17line;还有一种是唤醒中断，也就是其计数器计数到头的情况，这时候系统会发生严重错误。

  稍微扩展一下唤醒中断，如果从现在起，把计数器 RTC_CNT 的计数值置 0，然后每秒加 1，RTC_CNT 什么时候会溢出呢？由于 RTC_CNT 是 32 位寄存器，那么就是在2^32秒溢出，也就是136年后，大多数操作系统都是利用时间戳和计时元年来计算
当前时间的，而这个时间戳和计时元年大家都取了同一个标准——UNIX 时间戳和 UNIX 计时元年。在这个计时系统中，使用的是有符号的 32 位整型变量来保存 UNIX 时间戳的，即实际可用计数位数比我们上面例子中的少了一位，少了这一位，UNIX 
计时元年也相对提前了，这个计时方法在 2038 年 1 月 19 日 03 时 14 分 07 秒将会发生溢出。一旦溢出，可能导致系统发生严重错误，届时，很可能会重演一次“千年虫”的问题，所以在设计预期寿命较长的设备需要注意

  现在已经大致讲完了配置要点，接下来详细讲解配置步骤。

  首先，直接移植bsp_calendar和bsp_date文件，这里面都是日期等数据内容。

  自己创建rtc的文件

  如果，你的RTC时钟很久没上电了(RTC时钟依靠纽扣电池供电)，那么你得重新配置时钟LSI。需要开启PWR和Backup时钟，还需要开启允许访问 Backup 区域。使能LSI时钟，等待LSI配置完成;选择RTC时钟源;再使能RTC时钟。接下来要设置频率，
涉及到通过内核操作寄存器，需要进行时钟同步。完成后设置RTC时钟分频，等待设置完成。(如果有开启中断的必要，则还需要使能RTC秒中断，并等待完成)。然后把传入的日期计算时间戳并写入到RTC计数寄存器，并计算星期，再次等待改完成。

 那如果已经上电了呢，我终不能反复初始化吧，因此，我们需要能判断RTC时钟是否初始化的东西。在BKP的寄存器中，有一个备份数据寄存器x，BKP_DRx寄存器不会被系统复位、电源复位、从待机模式唤醒所复位。只会因为纽扣电池用完电或者拔出
被复位，所以你初始化后可以往里面存入数据，表示其已经被初始化，然后下次调用配置时判断一下，以免反复初始化。

 那如果已经上过电了，如果严谨地配置的话，则还需要像没上电一样再写一遍配置，除了不需要然后把传入的日期计算时间戳并写入到RTC计数寄存器，并计算星期，再次等待改完成。但是经过测试，是可以直接开启LSI时钟然后就不用管了.因为配置
RTC的时钟源的寄存器备份域控制寄存器 (RCC_BDCR)是存在写保护的，这些位只能由备份域复位清除。任何内部或外部复位都不会影响这些位。(如果有开启中断的必要，则还需要使能RTC秒中断，并等待完成)

 初始化配置基本上结束了，接下来就是获取时间的，时间显示的函数了，直接移植即可。

     *
     * 函数名：Time_Regulate_Get
     * 描述  ：保存用户使用串口设置的时间，
     *         以便后面转化成时间戳存储到RTC 计数寄存器中。
     * 输入  ：用于读取RTC时间的结构体指针
     * 注意  ：在串口调试助手输入时，输入完数字要加回车
     */
    void Time_Regulate_Get(struct rtc_time *tm)
    {
    	  uint32_t temp_num = 0;
    		uint8_t day_max=0 ;
    
    	  printf("\r\n=========================设置时间==================");
    		
    	  do 
    	  {
    			printf("\r\n  请输入年份(Please Set Years),范围[1970~2038]，输入字符后请加回车:");
    			scanf("%d",&temp_num);
    			if(temp_num <1970 || temp_num >2038)
    			{
    				printf("\r\n 您输入的数字是:%d，不符合要求",temp_num);
    					  
    			}
    			else
    			{	  
    				printf("\n\r  年份被设置为: %d\n\r", temp_num);
    
    				tm->tm_year = temp_num;
    				break;
    			}
    	  }while(1);
    
    
    	 do 
    	  {
    			printf("\r\n  请输入月份(Please Set Months):范围[1~12]，输入字符后请加回车:");
    			scanf("%d",&temp_num);
    			if(temp_num <1 || temp_num >12)
    			{
    				printf("\r\n 您输入的数字是:%d，不符合要求",temp_num);
    					  
    			}
    			else
    			{	  
    				printf("\n\r  月份被设置为: %d\n\r", temp_num);
    
    				tm->tm_mon = temp_num;
    				break;
    			}
    	  }while(1);
    		
    		/*根据月份计算最大日期*/
    		switch(tm->tm_mon)
    			{
    				case 1:
    				case 3:
    				case 5:
    				case 7:
    				case 8:
    				case 10:
    				case 12:					
    						day_max = 31;
    					break;
    				
    				case 4:
    				case 6:
    				case 9:
    				case 11:
    						day_max = 30;
    					break;
    				
    				case 2:					
    				     /*计算闰年*/
    						if((tm->tm_year%4==0) &&
    							 ((tm->tm_year%100!=0) || (tm->tm_year%400==0)) &&
    							 (tm->tm_mon>2)) 
    								{
    									day_max = 29;
    								} else 
    								{
    									day_max = 28;
    								}
    					break;			
    			}
    
    		do 
    	  {				
    			printf("\r\n  请输入日期(Please Set Months),范围[1~%d]，输入字符后请加回车:",day_max);
    			scanf("%d",&temp_num);
    			
    			if(temp_num <1 || temp_num >day_max)
    			{
    				printf("\r\n 您输入的数字是:%d，不符合要求",temp_num);
    			}
    			else
    			{
    				printf("\n\r  日期被设置为: %d\n\r", temp_num);
    
    				tm->tm_mday = temp_num;
    				break;
    			}
    	  }while(1);
    		
    		do 
    	  {				
    			printf("\r\n  请输入时钟(Please Set Hours),范围[0~23]，输入字符后请加回车:");
    			scanf("%d",&temp_num);
    			
    			if( temp_num >23)
    			{
    				printf("\r\n 您输入的数字是:%d，不符合要求",temp_num);
    			}
    			else
    			{
    				printf("\n\r  时钟被设置为: %d\n\r", temp_num);
    
    				tm->tm_hour = temp_num;
    				break;
    			}
    	  }while(1);
    
    		do 
    	  {				
    			printf("\r\n  请输入分钟(Please Set Minutes),范围[0~59]，输入字符后请加回车:");
    			scanf("%d",&temp_num);
    			
    			if( temp_num >59)
    			{
    				printf("\r\n 您输入的数字是:%d，不符合要求",temp_num);
    			}
    			else
    			{
    				printf("\n\r  分钟被设置为: %d\n\r", temp_num);
    
    				tm->tm_min = temp_num;
    				break;
    			}
    	  }while(1);
    
    		do 
    	  {				
    			printf("\r\n  请输入秒钟(Please Set Seconds),范围[0~59]，输入字符后请加回车:");
    			scanf("%d",&temp_num);
    			
    			if( temp_num >59)
    			{
    				printf("\r\n 您输入的数字是:%d，不符合要求",temp_num);
    			}
    			else
    			{
    				printf("\n\r  秒钟被设置为: %d\n\r", temp_num);
    
    				tm->tm_sec = temp_num;
    				break;
    			}
    	  }while(1);
    
    }
    
    /*
     * 函数名：Time_Show
     * 描述  ：显示当前时间值
     * 输入  ：无
     * 输出  ：无
     * 调用  ：外部调用
     */ 
    void Time_Show(struct rtc_time *tm)
    {	 
    	  /* Infinite loop */
    	  while (1)
    	  {
    	    /* 每过1s */
    	    if (TimeDisplay == 1)
    	    {
    				/* Display current time */
    	      Time_Display( RTC_GetCounter(),tm); 		  
    	      TimeDisplay = 0;
    	    }
    	  }
    }
    
    
    /*
     * 函数名：Time_Adjust
     * 描述  ：时间调节
     * 输入  ：用于读取RTC时间的结构体指针（北京时间）
     * 输出  ：无
     * 调用  ：外部调用
     */
    void Time_Adjust(struct rtc_time *tm)
    {
    	
    			/* RTC 配置 */
    		RTC_Configuration();
    
    	  /* 等待确保上一次操作完成 */
    	  RTC_WaitForLastTask();
    		  
    	  /* 计算星期 */
    	  GregorianDay(tm);
    
    	  /* 由日期计算时间戳并写入到RTC计数寄存器 */
    	  RTC_SetCounter(mktimev(tm)-TIME_ZOOM);
    
    	  /* 等待确保上一次操作完成 */
    	  RTC_WaitForLastTask();
    }
    
    /*
     * 函数名：a
     * 描述  ：显示当前时间值
     * 输入  ：-TimeVar RTC计数值，单位为 s
     * 输出  ：无
     * 调用  ：内部调用
     */	
    void Time_Display(uint32_t TimeVar,struct rtc_time *tm)
    {
    	   static uint32_t FirstDisplay = 1;
    	   uint32_t BJ_TimeVar;
    	   uint8_t str[200]; // 字符串暂存  	
    
    	   /*  把标准时间转换为北京时间*/
    	   BJ_TimeVar =TimeVar + TIME_ZOOM;
    
    	   to_tm(BJ_TimeVar, tm);/*把定时器的值转换为北京时间*/	
    	
    	  if((!tm->tm_hour && !tm->tm_min && !tm->tm_sec)  || (FirstDisplay))
    	  {
    	      
    	      GetChinaCalendar((u16)tm->tm_year, (u8)tm->tm_mon, (u8)tm->tm_mday, str);	
    					printf("\r\n 今天新历：%0.2d%0.2d,%0.2d,%0.2d", str[0], str[1], str[2],  str[3]);
    	
    	      GetChinaCalendarStr((u16)tm->tm_year,(u8)tm->tm_mon,(u8)tm->tm_mday,str);
    					printf("\r\n 今天农历：%s\r\n", str);
    	
    	     if(GetJieQiStr((u16)tm->tm_year, (u8)tm->tm_mon, (u8)tm->tm_mday, str))
    					printf("\r\n 今天农历：%s\r\n", str);
    	
    	      FirstDisplay = 0;
    	  }	 	  	
    
    	  /* 输出时间戳，公历时间 */
    	  printf(" UNIX时间戳 = %d 当前时间为: %d年(%s年) %d月 %d日 (星期%s)  %0.2d:%0.2d:%0.2d\r",TimeVar,
    	                    tm->tm_year, zodiac_sign[(tm->tm_year-3)%12], tm->tm_mon, tm->tm_mday, 
    	                    WEEK_STR[tm->tm_wday], tm->tm_hour, 
    	                    tm->tm_min, tm->tm_sec);
    		
    #ifdef  USE_LCD_DISPLAY
    
    		//时间戳
    		sprintf((char *)str," UNIX TimeStamp = %d ",TimeVar);
    
    		ILI9341_DispStringLine_EN(LINE(3),(char*)str);
    		
    		//日期
    		sprintf((char *)str," Date: %d-%d-%d       ",
    																						tm->tm_year, 
    																						tm->tm_mon,
    																						tm->tm_mday);
    		ILI9341_DispStringLine_EN(LINE(5),(char*)str);
    		
    		//生肖
    		sprintf((char *)str," Chinese %s year      ",en_zodiac_sign[(tm->tm_year-3)%12]);
    		
    		ILI9341_DispStringLine_EN(LINE(6),(char*)str);
    		
    		//星期
    		sprintf((char *)str," %s                  ",en_WEEK_STR[tm->tm_wday]);
    		
    		ILI9341_DispStringLine_EN(LINE(7),(char*)str);
    		
    		//时间
    		sprintf((char *)str," Time:  %0.2d:%0.2d:%0.2d",
    																						tm->tm_hour, 									
    																						tm->tm_min, 
    																						tm->tm_sec);
    		
    		ILI9341_DispStringLine_EN(LINE(8),(char*)str);
    #endif
    		
    }


然后如果要显示时间，则需要配置NVIC，并在中断服务中编写RTC_IRQHandler函数，把TimeDisplay置1，然后再清除中断标志位，并且等待完成。

这里显示时间我并不推荐使用Time_Show函数，因为这里面有while(1)死循环，你调用它那么主函数里面的while(1)循环那就是什么事都干不了了，还是用Time_DisPlay吧。就仿造着Time_Show函数编写即可。

如果不想用显示屏也没关系，修改宏定义即可，但是Time_Display里面已经用显示屏的一些函数了，修改起来挺麻烦的，直接在主函数中不调用它就好了。
