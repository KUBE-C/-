#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include <string.h>

//Ӳ������
#include "Key.h"
#include "Timer.h"
#include "usart.h"

/**
  * @brief  ������ʼ��������һ�˽ӵ�Ƭ�����ţ�һ�˽�GND
  * @param  None
  * @retval None
  */
void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4  ;
	GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_50MHz ;
	GPIO_Init (GPIOE,&GPIO_InitStructure);
	
}

uint8_t Key_GetNum(void)
{
   uint8_t KeyNum = 0;
	if ( GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 0)
	{
	  Delay_ms (20);
		KeyNum = 1;
	}
	if ( GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4) == 0)
	{
	  Delay_ms (20);
		KeyNum = 2;
	}
	
		return KeyNum;
	}






