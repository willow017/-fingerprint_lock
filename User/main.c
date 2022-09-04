#include "stm32f10x.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "exti.h"

int main(void)
{
	led_init();
	key_init();
	usart1_init();
	usart2_init();
  exti_init();
	Usart_SendString(USART1, "ÊÇ·ñ\n");
	
	while(1)
	{
		if(Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == 0)
		{
			LED1_TOGGLE;
		}
	}
}


