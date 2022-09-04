#include "key.h"


void key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin   = KEY1_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	
}

uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/*检测是否有按键按下 */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
	{	 
		/*等待按键释放 */
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);   
		return 	KEY_ON;	 
	}
	else
		return KEY_OFF;
}
