#ifndef __EXTI_H
#define __EXTI_H

#include "stm32f10x.h"


#define EXTI_GPIO_PORT    GPIOB
#define EXTI_GPIO_CLK     RCC_APB2Periph_GPIOB
#define EXTI_GPIO_PIN     GPIO_Pin_1

void exti_init(void);

#endif /* __EXTI_H */

