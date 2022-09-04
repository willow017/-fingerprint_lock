#include "exti.h"

static void EXTI_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
}

void exti_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	EXTI_NVIC_Config();
	
	RCC_APB2PeriphClockCmd(EXTI_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin   = EXTI_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(EXTI_GPIO_PORT, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}
