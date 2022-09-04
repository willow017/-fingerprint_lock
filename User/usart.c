#include "usart.h"
#include "led.h"


//ָ��ģ�����
//ָ���
/************************************

����� HLK-ZW651ָ��ģ�����߱�
 ��ɫ         ������        stm32F103RCоƬIO��
��ɫ��        VCC3_3V             3V3
��ɫ��        INT_OUT             PB1
��ɫ��        MCU3_3V             3V3
��ɫ��        TTL_TXD             PA3
��ɫ��        TTL_RXD             PA2
��ɫ��          GND               GND

************************************/
//���ָ�ƿ�
uint8_t PS_Empty[12]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x03,0x0d,0x00,0x11};
//�Զ�ע��ģ�� ����Ϊ1~10 ¼�����Ϊ����
uint8_t PS_AutoEnroll1[17]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x08,0x31,0x00,0x01,0x02,0x00,0x73,0x00,0xB0};
uint8_t PS_AutoEnroll2[17]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x08,0x31,0x00,0x02,0x02,0x00,0x73,0x00,0xB1};
uint8_t PS_AutoEnroll3[17]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x08,0x31,0x00,0x03,0x02,0x00,0x73,0x00,0xB2};
uint8_t PS_AutoEnroll4[17]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x08,0x31,0x00,0x04,0x02,0x00,0x73,0x00,0xB3};
uint8_t PS_AutoEnroll5[17]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x08,0x31,0x00,0x05,0x02,0x00,0x73,0x00,0xB4};
uint8_t PS_AutoEnroll6[17]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x08,0x31,0x00,0x06,0x02,0x00,0x73,0x00,0xB5};
uint8_t PS_AutoEnroll7[17]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x08,0x31,0x00,0x07,0x02,0x00,0x73,0x00,0xB6};
uint8_t PS_AutoEnroll8[17]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x08,0x31,0x00,0x08,0x02,0x00,0x73,0x00,0xB7};
uint8_t PS_AutoEnroll9[17]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x08,0x31,0x00,0x09,0x02,0x00,0x73,0x00,0xB8};
uint8_t PS_AutoEnroll10[17]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x08,0x31,0x00,0x10,0x02,0x00,0x73,0x00,0xB9};
//�Զ���ָ֤�� 
uint8_t PS_Autoldentify[17]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x08,0x32,0x00,0xFF,0xFF,0x00,0x06,0x02,0x3F};
//ɾ��ģ�� ��11��12Ϊָ�ƿ�ģ���
uint8_t PS_DeletChar[16]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x07,0x0c,0x00,0x01,0x00,0x01,0x00,0x16};
//����ָ��
uint8_t PS_Sleep[12]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x03,0x33,0x00,0x37};
//����Чģ�����
uint8_t PS_ValidTempleteNum[12]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x03,0x1d,0x00,0x21};




/*
Ӧ���
���ָ�ƿ�Ӧ���
PS_Empty                EF01FFFFFFFF070003 00��ճɹ� 0A 
                                           01�հ��д� 0B
                                           11���ʧ�� 1B

�Զ�ע��ָ��Ӧ���
PS_AutoEnroll           EF01FFFFFFFF070005 00�ɹ�             00ָ�ƺϷ��Լ�� 00ָ�ƺϷ��Լ��       sum ָ�ƺϷ��Լ��
                                           01ʧ��             01��ȡͼ��       F0�ϲ�ģ��                 ��ͼ���
                                           07��������ʧ��     02��������       F1�������ָ�Ƿ���ע��     �����������
                                           0a�ϲ�ģ��ʧ��     03�ж���ָ�뿪   F2�洢ģ��                 ��ָ�뿪����1��¼��ɹ�
                                           0bID�ų�����Χ     04�ϲ�ģ��       n��ǰ¼���n����           �ϲ�ģ��
                                           1fָ�ƿ�����       05ע�����                                  ��ע����
                                           22ָ��ģ��ǿ�     06�洢ģ��                                  ģ��洢���
                                           25¼��������ô���
                                           26��ʱ
																					 27ָ���Ѵ���
												ָ��˵����1 ��ָ��ID����Ч����ȷ���롢����1�Ͳ���2���أ�0b 00 00H���Ϸ��Լ�⣺
                                    ��¼��������ô����򷵻�25 00 00H���ڲ�����ָ��״̬�£���ָ�ƿ������򷵻�1f 00 00H��
                                    ��ָ��ID���Ѵ���ģ���򷵻�22 00 00H��
                                    ָ��Ϸ��Լ��ɹ����򷵻�00 00 00H���������һ��ָ��¼�롣
                                  2 �ȴ���ͼ�ɹ�������00 01 0nH����
                                  3 �ȴ����������ɹ���00 02 0nH�������ʧ�ܣ�07 02 0nH�������µȴ���ͼ�ɹ���
                                  4 �ȴ���ָ�뿪����һ��¼��ɹ���00 03 0nH������ָ�뿪����ת������2��������һ��ѭ����ֱ��nΪ����¼��Ĵ�����ע����¼���
                         ��������Ϊ��ָ����Ҫ�뿪����ôֱ�ӷ��ص�һ��¼��ɹ�������ת������2�����һ�βɼ�ָ�ƣ�û����ָ�뿪¼��ɹ���Ӧ��
                                  5 �ϳ�ģ�壬��֮ǰ��ȡ����ָ������ϳ�һ����ָģ�壬�ɹ�����00 04 F0H��ʧ�ܷ���0A 04 F0H��
                                  6 ָ���ظ���飬ָ����¼�����ָ���Ѿ��洢����ָ����ƥ���飨ͨ�����ò���bit4�������߹رմ˹��ܣ���������ָͬ�ƣ��򷵻�
                         27 05 F1H,�������̣���û����ָͬ�ƣ��򷵻�00 05 F1H��
                                  7 �ǼǸ�ģ�����ݣ��洢ʧ�ܷ���01 06 F2H,�������̣��ɹ�����00 06 F2H��
                                  8 ���յ�PS_Cancelָ�����ֹ��ָ�����Ӧ��		

�Զ���ָ֤��Ӧ���Ŷ
PS_Autoldentify      EF01FFFFFFFF070008 00�ɹ�           00ָ�ƺϷ��Լ�� 0000ID�� 0000�÷� sum ָ�ƺϷ��Լ��
                                        01ʧ��           01��ȡͼ��                             ��ͼ���
																				07��������ʧ��   05��ע��ָ�ƶԱ�                       �������
																				09û������ָ��
																				0bID�ų�����Χ
																				17����ָ��
																				23ָ��ģ��Ϊ��
																				24ָ�ƿ�Ϊ��
																				26��ʱ
																				27��ʾָ���Ѵ���
												ָ��˵����1 ��ָ�ƿ�Ϊ�գ���ȷ����Ͳ������أ�24 00H����ָ��ID����Ч���򷵻�0b 00H�����ѵǼǵ�Template�����ڣ��򷵻�23 00H��
												          2 ָ��Ϸ��Լ��ɹ�������00 00H��������ָ��¼�롣
																	3 ���趨�ĳ�ʱʱ���ڣ���û�����һ��������ָ��¼�룬�򷵻�26 00H���������̡�
																	4 ��������ָ��ͼ�����ȷ�ԡ�������ȷ����ȴ��´βɼ�ͼ��
																	5 ������ָ����ȷ���򷵻�00 01H����¼��ָ�ƻ�ȡͼ��ɹ���
																	6 ����������ʧ�ܣ��򷵻�09 05H���������̡�
																	7 ���������ɹ��󣬰ѵ�ǰ�ɼ�����ָ��ģ�����ѵǼǵ�ָ��ģ��֮����жԱȣ����������������ȶ�ʧ�ܣ��򷵻�09 05H���������̣�
													���Աȳɹ����򷵻�00 05H���Լ���ȷ��ID����͵÷֡�
													        8 ���յ�FpCancelָ�����ֹ��ָ�����Ӧ��

ɾ��ָ��Ӧ���
PS_DeletChar          EF01FFFFFFFF070003 00ɾ��ģ��ɹ� sum
                                         01�հ��д�     sum
																			   10ɾ��ģ��ʧ�� sum

����ָ��Ӧ���
PS_Sleep              EF01FFFFFFFF070003 00�������óɹ� sum
                                         01��������ʧ�� sum

��Чָ�Ƹ���Ӧ���
PS_ValidTempleteNum   EF01FFFFFFFF070005 00��ȡ�ɹ� 0000��Чģ����� sum
                                         01�հ��д�
*/


static void NVIC1_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Ƕ�������жϿ�������ѡ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  /* �������ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

static void NVIC2_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Ƕ�������жϿ�������ѡ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  /* �������ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}


void usart1_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// �򿪴���GPIO��ʱ��
	USART1_GPIO_APBxClkCmd(USART1_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	USART1_APBxClkCmd(USART1_CLK, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = USART1_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART1_TX_GPIO_PORT, &GPIO_InitStructure);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = USART1_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART1_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = USART1_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(USART1, &USART_InitStructure);
	
	// �����ж����ȼ�����
	NVIC1_Configuration();
	
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	
	
	// ʹ�ܴ���
	USART_Cmd(USART1, ENABLE);	    
}

void usart2_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// �򿪴���GPIO��ʱ��
	USART2_GPIO_APBxClkCmd(USART2_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	USART2_APBxClkCmd(USART2_CLK, ENABLE);
	
	// ָ�Ƹ�Ӧ�ϵ��źŽ�

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = USART2_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART2_TX_GPIO_PORT, &GPIO_InitStructure);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = USART2_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART2_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = USART2_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_2;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(USART2, &USART_InitStructure);
	
	// �����ж����ȼ�����
	NVIC2_Configuration();
	
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	
	
	// ʹ�ܴ���
	USART_Cmd(USART2, ENABLE);	    
}

//�����ַ���
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* �ȴ�������� */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

//ָ��ģ�鷢�ͺ���
/*
����1�����ںţ�
����2���������飻
����3�������С��
*/
void Usart_Send8(USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
	uint8_t i;
	
	for(i=0; i<=num; i++)
	{
		Usart_SendByte(pUSARTx, array[i]);
	}
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

/*
���ָ�ƿ⺯��
*/
void Usart_PS_Empty(USART_TypeDef * pUSARTx)
{
	uint8_t i;
	for(i=0;i<=11;i++)
	{
		Usart_SendByte(pUSARTx, PS_Empty[i]);
	}
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

/*
 ָ��������ѯ����
*/
void Usart_PS_ValidTempleteNum(USART_TypeDef * pUSARTx)
{
	uint8_t i;
	for(i=0; i<=11; i++)
	{
		Usart_SendByte(pUSARTx,PS_ValidTempleteNum[i]);
	}
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}
/*
 ָ��ע�ắ��
 ����1�����ں�
 ����2��ָ�ƺ�
*/
void Usart_PS_AutoEnroll(USART_TypeDef * pUSARTx, uint16_t num)
{
	uint8_t i;
	switch(num)
	{
		case 1: 
			for(i=0; i<=16; i++)
			{
				Usart_SendByte(pUSARTx,PS_AutoEnroll1[i]);
			}
			while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
			break;
		case 2:
			for(i=0; i<=16; i++)
			{
				Usart_SendByte(pUSARTx,PS_AutoEnroll2[i]);
			}
			while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
			break;
		case 3: 
			for(i=0; i<=16; i++)
			{
				Usart_SendByte(pUSARTx,PS_AutoEnroll3[i]);
			}
			while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
			break;
		case 4: 
			for(i=0; i<=16; i++)
			{
				Usart_SendByte(pUSARTx,PS_AutoEnroll4[i]);
			}
			while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
			break;
		case 5: 
			for(i=0; i<=16; i++)
			{
				Usart_SendByte(pUSARTx,PS_AutoEnroll5[i]);
			}
			while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
			break;
		case 6: 
			for(i=0; i<=16; i++)
			{
				Usart_SendByte(pUSARTx,PS_AutoEnroll6[i]);
			}
			while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
			break;
		case 7: 
			for(i=0; i<=16; i++)
			{
				Usart_SendByte(pUSARTx,PS_AutoEnroll7[i]);
			}
			while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
			break;
		case 8: 
			for(i=0; i<=16; i++)
			{
				Usart_SendByte(pUSARTx,PS_AutoEnroll8[i]);
			}
			while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
			break;
		case 9: 
			for(i=0; i<=16; i++)
			{
				Usart_SendByte(pUSARTx,PS_AutoEnroll9[i]);
			}
			while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
			break;
		case 10: 
			for(i=0; i<=16; i++)
			{
				Usart_SendByte(pUSARTx,PS_AutoEnroll10[i]);
			}
			while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
			break;
	}
}

/*
 ָ����֤����
*/

void KEY_PS_Autoldentify(USART_TypeDef * pUSARTx)
{
	uint8_t i;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) == 1 )
	{
		for(i=0; i<=16; i++)
		{
			Usart_SendByte(pUSARTx,PS_Autoldentify[i]);
		}
		while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
	}
}


/*
 ָ��ģ�鷵�ش������
*/
// ��ָ֤��
uint8_t RecBuff[17]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t cont=0;

/*
 ����2�жϺ���
*/
void USART2_IRQHandler(void)
{
	uint8_t Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE)!=RESET)
	{
		Res = USART_ReceiveData(USART2);
		RecBuff[cont] = Res;
		if(Res == 0xef || cont > 0){
			cont++;
		}
		else cont=0;
		if(cont == 17){
			cont = 0;
			if(RecBuff[10] == 0x05){  // �ж�ָ���Ƿ�ʶ����ȷ
				// ���ָ��ͨ��ִ�г���
				LED1_TOGGLE;
				USART_SendData(USART1,0x99);
			}
		}
		if(cont == 14){
			cont = 0;
			if(RecBuff[9] ==0x00 && RecBuff[10] == 0x06 && RecBuff[11] == 0xF2){ // �ж�ָ���Ƿ�ע��ɹ�
				//���ָ��ע��ɹ�ִ�г���
			} 
		}
//			switch(RecBuff[8]){
//				case 0x03:
//					// ���ָ�ƿ⴦�����
//				  break;
//				case 0x05:
//					// ע��ָ�ƴ������
//				  break;
//				case 0x08:
//					// ��ָ֤�ƴ������
//				  break;
//			}
	}
}
