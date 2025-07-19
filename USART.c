#include "USART.h"
/**
 * @brief ����USART��NVIC�����ȼ�
 * @param ��
 * @retval ��
 */
static void USART_NVIC_Configuration(void)
{
	NVIC_InitTypeDef USART_NVIC_structure;
	//һ��أ�������USART��ѡ��2����
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2);
	//������ռ���ȼ�
	USART_NVIC_structure.NVIC_IRQChannelPreemptionPriority=1;
	//���������ȼ�
	USART_NVIC_structure.NVIC_IRQChannelSubPriority=1;
	//ѡ���ж�����
	USART_NVIC_structure.NVIC_IRQChannel=DEBUG_USART_IRQ;
	//ʹ���ж�
	USART_NVIC_structure.NVIC_IRQChannelCmd=ENABLE;
	//��ʼ��NVIC�ṹ��
	NVIC_Init(&USART_NVIC_structure);
}

/**
 * @brief ����USART��GPIO���������
 * @param ��
 * @retval ��
 */
void USART_Configuration(void)
{
	USART_InitTypeDef DEBUG_USART_structure;
	GPIO_InitTypeDef  DEBUG_USART_TX_GPIO_structure,DEBUG_USART_RX_GPIO_structure;
	//����USART��GPIO��ʱ��
	DEBUG_USART_APBxClkCmd(RCC_APB2Periph_USART1,ENABLE);
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK,ENABLE);
	//����USART��GPIO��TX������
	//ѡ�����츴�����
	DEBUG_USART_TX_GPIO_structure.GPIO_Mode=GPIO_Mode_AF_PP;
	DEBUG_USART_TX_GPIO_structure.GPIO_Pin= DEBUG_USART_TX_GPIO_PIN;
	DEBUG_USART_TX_GPIO_structure.GPIO_Speed=GPIO_Speed_50MHz;
	//��ʼ��TX����
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT,&DEBUG_USART_TX_GPIO_structure);
	//����USART��GPIO��RX������
	//ѡ�񸡿�����ģʽ
	DEBUG_USART_RX_GPIO_structure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	DEBUG_USART_RX_GPIO_structure.GPIO_Pin= DEBUG_USART_RX_GPIO_PIN;
	//��ʼ��RX����
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT,&DEBUG_USART_RX_GPIO_structure);
	//����USART�ṹ��
	//USART������ѡ�����115200
	DEBUG_USART_structure.USART_BaudRate=DEBUG_USART_BAUDRATE;
	//USARTӲ�������ƣ���Ϊ�������ﲻ�Ǹ��ٴ������ݴ��䣬�������Ӳ��������
	DEBUG_USART_structure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	//USART����ģʽ��ѡ��ȫ˫��ģʽ���շ�һ��
	DEBUG_USART_structure.USART_Mode=USART_Mode_Rx| USART_Mode_Tx;
	//USARTУ��������ﲻ��Ҫ��ѡ����
	DEBUG_USART_structure.USART_Parity=USART_Parity_No;
	//USARTֹͣ����ѡ��1
	DEBUG_USART_structure.USART_StopBits=USART_StopBits_1;
	//USART���������ֳ���ѡ��8b
	DEBUG_USART_structure.USART_WordLength=USART_WordLength_8b;
	//��ʼ��USART�ṹ��
	USART_Init(DEBUG_USARTx,&DEBUG_USART_structure);
//	//�������õ��ж�
//	USART_NVIC_Configuration();
//	//��ʼUSART�ж�
//	USART_ITConfig(DEBUG_USARTx,USART_IT_RXNE,ENABLE);
	//ʹ��USART
	USART_Cmd(DEBUG_USARTx,ENABLE);
}

/**
 * @brief ����һ���ֽ�
 * @param ����һ��ָ���Ҫ���������
 * @retval ��
 */
void USART_SendByte(USART_TypeDef* pUSARTx,uint16_t Data)
{
	 USART_SendData(pUSARTx,Data);
	 while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);
}

/**
 * @brief ����һ��8λ���ֵ�����
 * @param ����һ��ָ�룬һ��8λ���ֵ����飬Ҫ����������еĸ���
 * @retval ��
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
 * @brief ����һ���ַ���
 * @param ����һ��ָ�룬�ַ�����ָ��
 * @retval ��
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
 * @brief ����һ��16λ������
 * @param ����һ��ָ�룬16λ������
 * @retval ��
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

//�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(DEBUG_USARTx, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTx);
}
