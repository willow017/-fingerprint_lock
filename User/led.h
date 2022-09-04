#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

#define LED1_GPIO_PORT    GPIOA
#define LED1_GPIO_CLK     RCC_APB2Periph_GPIOA
#define LED1_GPIO_PIN     GPIO_Pin_8

#define LED1(a)   if(a) GPIO_ResetBits(LED1_GPIO_PORT, LED1_GPIO_PIN); else GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN)

#define digitalToggle(p,i) {p->ODR ^=i;} //Êä³ö·´×ª×´Ì¬
#define LED1_TOGGLE		 digitalToggle(LED1_GPIO_PORT,LED1_GPIO_PIN)



void led_init(void);

#endif /* __LED_H */

