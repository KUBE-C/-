#ifndef __Key_H
#define __Key_H
#include "stdbool.h"

#include "stm32f10x.h"

void Key_Init(void);
void Key_Execute(uint8_t key);
uint8_t Key_GetNum(void);


//������Ϣ
#define KEY0_PRES      1      /* KEY0���� */
#define KEY1_PRES      2      /* KEY1���� */
#define KEYUP_PRES     3      /* KEY_UP����(��WK_UP) */

#define KEY_UP     GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)
#define KEY0       GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)
#define KEY1       GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)

#endif

