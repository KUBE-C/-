#include "stm32f10x.h"                  // Device header
#include "Relay.h"
/*
   ʹ��PA11�������
   | �̵���        | STM32   |
   | ------------- | ------- |
   | VCC������ˣ� | 3V3     |
   | GND������ˣ� | GND     |
   | IN������ˣ�  | GPIO    |
   | NO������ˣ�  | ����VCC |
   | COM������ˣ� | ���GND |
   | NC������ˣ�  |         |
*/


void relay_init(void)
{
   RCC_APB2PeriphClockCmd(Relay_RCC, ENABLE);
   GPIO_InitTypeDef GPIO_InitStructure;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Pin = Relay_PIN;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(Relay_PORT, &GPIO_InitStructure);
   
}

// �ߵ�ƽ����
void relay_On(void) 
{
   GPIO_SetBits(Relay_PORT, Relay_PIN);
}

void relay_Off(void) 
{
   GPIO_ResetBits(Relay_PORT, Relay_PIN);
}
