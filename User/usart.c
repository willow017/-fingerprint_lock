#include "usart.h"
#include "led.h"


//指纹模块代码
//指令包
/************************************

海凌科 HLK-ZW651指纹模块连线表
 线色         引脚名        stm32F103RC芯片IO口
红色线        VCC3_3V             3V3
黄色线        INT_OUT             PB1
绿色线        MCU3_3V             3V3
蓝色线        TTL_TXD             PA3
白色线        TTL_RXD             PA2
橙色线          GND               GND

************************************/
//清除指纹库
uint8_t PS_Empty[12]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x03,0x0d,0x00,0x11};
//自动注册模板 依次为1~10 录入次数为两次
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
//自动验证指纹 
uint8_t PS_Autoldentify[17]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x08,0x32,0x00,0xFF,0xFF,0x00,0x06,0x02,0x3F};
//删除模板 第11，12为指纹库模板号
uint8_t PS_DeletChar[16]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x07,0x0c,0x00,0x01,0x00,0x01,0x00,0x16};
//休眠指令
uint8_t PS_Sleep[12]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x03,0x33,0x00,0x37};
//读有效模板个数
uint8_t PS_ValidTempleteNum[12]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x03,0x1d,0x00,0x21};




/*
应答包
清除指纹库应答包
PS_Empty                EF01FFFFFFFF070003 00清空成功 0A 
                                           01收包有错 0B
                                           11清空失败 1B

自动注册指纹应答包
PS_AutoEnroll           EF01FFFFFFFF070005 00成功             00指纹合法性检测 00指纹合法性检测       sum 指纹合法性检测
                                           01失败             01获取图像       F0合并模板                 采图结果
                                           07生成特征失败     02生产特征       F1检验该手指是否已注册     生成特征结果
                                           0a合并模板失败     03判断手指离开   F2存储模板                 手指离开，第1次录入成功
                                           0bID号超出范围     04合并模板       n当前录入第n次数           合并模板
                                           1f指纹库已满       05注册检验                                  已注册检测
                                           22指纹模板非空     06存储模板                                  模板存储结果
                                           25录入次数设置错误
                                           26超时
																					 27指纹已存在
												指令说明：1 若指定ID号无效，则确认码、参数1和参数2返回：0b 00 00H；合法性检测：
                                    若录入次数配置错误，则返回25 00 00H。在不覆盖指纹状态下，若指纹库已满则返回1f 00 00H。
                                    若指定ID号已存在模板则返回22 00 00H。
                                    指令合法性检测成功，则返回00 00 00H，并进入第一次指纹录入。
                                  2 等待彩图成功（返回00 01 0nH）。
                                  3 等待生成特征成功（00 02 0nH），如果失败（07 02 0nH），重新等待彩图成功。
                                  4 等待手指离开，第一次录入成功（00 03 0nH），手指离开后跳转到步骤2，进入下一次循环，直到n为设置录入的次数。注：若录入过
                         程中设置为手指不需要离开，那么直接返回第一次录入成功，并跳转到步骤2；最后一次采集指纹，没有手指离开录入成功的应答。
                                  5 合成模板，将之前获取的手指特征组合成一个手指模板，成功返回00 04 F0H，失败返回0A 04 F0H。
                                  6 指纹重复检查，指将新录入的手指与已经存储的手指进行匹配检查（通过设置参数bit4开启或者关闭此功能），若有相同指纹，则返回
                         27 05 F1H,结束流程；若没有相同指纹，则返回00 05 F1H。
                                  7 登记该模板数据，存储失败返回01 06 F2H,结束流程；成功返回00 06 F2H。
                                  8 若收到PS_Cancel指令，则终止该指令并返回应答。		

自动验证指纹应答包哦
PS_Autoldentify      EF01FFFFFFFF070008 00成功           00指纹合法性检测 0000ID号 0000得分 sum 指纹合法性检测
                                        01失败           01获取图像                             采图结果
																				07生成特征失败   05已注册指纹对比                       搜索结果
																				09没搜索到指纹
																				0bID号超出范围
																				17残留指纹
																				23指纹模板为空
																				24指纹库为空
																				26超时
																				27表示指纹已存在
												指令说明：1 若指纹库为空，则确认码和参数返回：24 00H。若指定ID号无效，则返回0b 00H。若已登记的Template不存在，则返回23 00H。
												          2 指令合法性检测成功，返回00 00H，并进入指纹录入。
																	3 在设定的超时时间内，若没有完成一次完整的指纹录入，则返回26 00H，结束流程。
																	4 检查输入的指纹图像的正确性。若不正确，则等待下次采集图像。
																	5 若输入指纹正确，则返回00 01H，即录入指纹获取图像成功。
																	6 若生产特征失败，则返回09 05H，结束流程。
																	7 生成特征成功后，把当前采集到的指纹模板与已登记的指纹模板之间进行对比，并返回其结果。若比对失败，则返回09 05H，结束流程；
													若对比成功，则返回00 05H，以及正确的ID号码和得分。
													        8 若收到FpCancel指令，则终止该指令并返回应答。

删除指纹应答包
PS_DeletChar          EF01FFFFFFFF070003 00删除模板成功 sum
                                         01收包有错     sum
																			   10删除模板失败 sum

休眠指令应答包
PS_Sleep              EF01FFFFFFFF070003 00休眠设置成功 sum
                                         01休眠设置失败 sum

有效指纹个数应答包
PS_ValidTempleteNum   EF01FFFFFFFF070005 00读取成功 0000有效模板个数 sum
                                         01收包有错
*/


static void NVIC1_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 嵌套向量中断控制器组选择 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

static void NVIC2_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 嵌套向量中断控制器组选择 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}


void usart1_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟
	USART1_GPIO_APBxClkCmd(USART1_GPIO_CLK, ENABLE);
	
	// 打开串口外设的时钟
	USART1_APBxClkCmd(USART1_CLK, ENABLE);

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = USART1_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART1_TX_GPIO_PORT, &GPIO_InitStructure);

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = USART1_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART1_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = USART1_BAUDRATE;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(USART1, &USART_InitStructure);
	
	// 串口中断优先级配置
	NVIC1_Configuration();
	
	// 使能串口接收中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	
	
	// 使能串口
	USART_Cmd(USART1, ENABLE);	    
}

void usart2_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟
	USART2_GPIO_APBxClkCmd(USART2_GPIO_CLK, ENABLE);
	
	// 打开串口外设的时钟
	USART2_APBxClkCmd(USART2_CLK, ENABLE);
	
	// 指纹感应上电信号角

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = USART2_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART2_TX_GPIO_PORT, &GPIO_InitStructure);

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = USART2_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART2_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = USART2_BAUDRATE;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_2;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(USART2, &USART_InitStructure);
	
	// 串口中断优先级配置
	NVIC2_Configuration();
	
	// 使能串口接收中断
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	
	
	// 使能串口
	USART_Cmd(USART2, ENABLE);	    
}

//发送字符串
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* 等待发送完成 */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

//指纹模块发送函数
/*
参数1：串口号；
参数2：代码数组；
参数3：数组大小；
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
清除指纹库函数
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
 指纹数量查询函数
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
 指纹注册函数
 参数1：串口号
 参数2：指纹号
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
 指纹验证函数
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
 指纹模块返回代码变量
*/
// 验证指纹
uint8_t RecBuff[17]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t cont=0;

/*
 串口2中断函数
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
			if(RecBuff[10] == 0x05){  // 判断指纹是否识别正确
				// 添加指纹通过执行程序
				LED1_TOGGLE;
				USART_SendData(USART1,0x99);
			}
		}
		if(cont == 14){
			cont = 0;
			if(RecBuff[9] ==0x00 && RecBuff[10] == 0x06 && RecBuff[11] == 0xF2){ // 判断指纹是否注册成功
				//添加指纹注册成功执行程序
			} 
		}
//			switch(RecBuff[8]){
//				case 0x03:
//					// 清除指纹库处理程序
//				  break;
//				case 0x05:
//					// 注册指纹处理程序
//				  break;
//				case 0x08:
//					// 验证指纹处理程序
//				  break;
//			}
	}
}
