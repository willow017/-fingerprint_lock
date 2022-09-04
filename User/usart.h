#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"



// 串口1-USART1
#define  USART1_CLK                RCC_APB2Periph_USART1
#define  USART1_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  USART1_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  USART1_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  USART1_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  USART1_TX_GPIO_PORT       GPIOA   
#define  USART1_TX_GPIO_PIN        GPIO_Pin_9
#define  USART1_RX_GPIO_PORT       GPIOA
#define  USART1_RX_GPIO_PIN        GPIO_Pin_10


//串口2-USART2

#define  USART2_CLK                RCC_APB1Periph_USART2
#define  USART2_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  USART2_BAUDRATE           57600

// USART GPIO 引脚宏定义
#define  USART2_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  USART2_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  USART2_TX_GPIO_PORT       GPIOA   
#define  USART2_TX_GPIO_PIN        GPIO_Pin_2
#define  USART2_RX_GPIO_PORT       GPIOA
#define  USART2_RX_GPIO_PIN        GPIO_Pin_3




void usart1_init(void);
void usart2_init(void);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_Send8(USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);
void Usart_PS_ValidTempleteNum(USART_TypeDef * pUSARTx);
void Usart_PS_Empty(USART_TypeDef * pUSARTx);
void Usart_PS_AutoEnroll(USART_TypeDef * pUSARTx, uint16_t num);
void KEY_PS_Autoldentify(USART_TypeDef * pUSARTx);

#endif  /* __USART_H */

