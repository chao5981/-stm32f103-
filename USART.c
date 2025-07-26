#include "USART.h"
/**
 * @brief 配置USART在NVIC的优先级
 * @param 无
 * @retval 无
 */
static void USART_NVIC_Configuration(void)
{
	NVIC_InitTypeDef USART_NVIC_structure;
	//一般地，我们在USART中选择2分组
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2);
	//配置抢占优先级
	USART_NVIC_structure.NVIC_IRQChannelPreemptionPriority=1;
	//配置子优先级
	USART_NVIC_structure.NVIC_IRQChannelSubPriority=1;
	//选择中断渠道
	USART_NVIC_structure.NVIC_IRQChannel=DEBUG_USART_IRQ;
	//使能中断
	USART_NVIC_structure.NVIC_IRQChannelCmd=ENABLE;
	//初始化NVIC结构体
	NVIC_Init(&USART_NVIC_structure);
}

/**
 * @brief 配置USART和GPIO的相关引脚
 * @param 无
 * @retval 无
 */
void USART_Configuration(void)
{
	USART_InitTypeDef DEBUG_USART_structure;
	GPIO_InitTypeDef  DEBUG_USART_TX_GPIO_structure,DEBUG_USART_RX_GPIO_structure;
	//配置USART和GPIO的时钟
	DEBUG_USART_APBxClkCmd(RCC_APB2Periph_USART1,ENABLE);
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK,ENABLE);
	//配置USART在GPIO的TX的引脚
	//选择推挽复用输出
	DEBUG_USART_TX_GPIO_structure.GPIO_Mode=GPIO_Mode_AF_PP;
	DEBUG_USART_TX_GPIO_structure.GPIO_Pin= DEBUG_USART_TX_GPIO_PIN;
	DEBUG_USART_TX_GPIO_structure.GPIO_Speed=GPIO_Speed_50MHz;
	//初始化TX引脚
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT,&DEBUG_USART_TX_GPIO_structure);
	//配置USART在GPIO的RX的引脚
	//选择浮空输入模式
	DEBUG_USART_RX_GPIO_structure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	DEBUG_USART_RX_GPIO_structure.GPIO_Pin= DEBUG_USART_RX_GPIO_PIN;
	//初始化RX引脚
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT,&DEBUG_USART_RX_GPIO_structure);
	//配置USART结构体
	//USART波特率选择最高115200
	DEBUG_USART_structure.USART_BaudRate=DEBUG_USART_BAUDRATE;
	//USART硬件流控制，因为我们这里不是高速大量数据传输，因此无需硬件控制流
	DEBUG_USART_structure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	//USART传输模式，选择全双工模式，收发一起
	DEBUG_USART_structure.USART_Mode=USART_Mode_Rx| USART_Mode_Tx;
	//USART校验符，这里不需要，选择无
	DEBUG_USART_structure.USART_Parity=USART_Parity_No;
	//USART停止符，选择1
	DEBUG_USART_structure.USART_StopBits=USART_StopBits_1;
	//USART传输数据字长，选择8b
	DEBUG_USART_structure.USART_WordLength=USART_WordLength_8b;
	//初始化USART结构体
	USART_Init(DEBUG_USARTx,&DEBUG_USART_structure);
//	//开启配置的中断
//	USART_NVIC_Configuration();
//	//开始USART中断
//	USART_ITConfig(DEBUG_USARTx,USART_IT_RXNE,ENABLE);
	//使能USART
	USART_Cmd(DEBUG_USARTx,ENABLE);
}

/**
 * @brief 传输一个字节
 * @param 传入一个指针和要传输的数据
 * @retval 无
 */
void USART_SendByte(USART_TypeDef* pUSARTx,uint16_t Data)
{
	 USART_SendData(pUSARTx,Data);
	 while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);
}

/**
 * @brief 传输一个8位数字的数组
 * @param 传入一个指针，一个8位数字的数组，要传入的数组中的个数
 * @retval 无
 */
void USART_SendArray(USART_TypeDef* pUSARTx,uint8_t *array,uint16_t num)
{
	uint8_t i;
	for(i=0;i<num;i++)
	{
		USART_SendByte(pUSARTx,array[i]);	
	}
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);
}

/**
 * @brief 传输一个字符串
 * @param 传入一个指针，字符串的指针
 * @retval 无
 */
void USART_SendString(USART_TypeDef* pUSARTx,char *str)
{
	unsigned int i=0;
	while(*(str+i)!='\0')
	{
		USART_SendByte(pUSARTx,*(str+i));
		i++;
	}
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);
}

/**
 * @brief 传输一个16位的数字
 * @param 传入一个指针，16位的数字
 * @retval 无
 */
void USART_SendHalfWord(USART_TypeDef* pUSARTx,uint16_t Data)
{
	uint8_t temp_h,temp_l;
	temp_h=(Data&0xFF00)>>8;
	temp_l=Data&0xFF;
	USART_SendData(pUSARTx,temp_h);
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);
	USART_SendData(pUSARTx,temp_l);
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);
}

//重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		USART_SendData(DEBUG_USARTx, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTx);
}
