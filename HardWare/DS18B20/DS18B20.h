#ifndef __DS18B20_H
#define __DS18B20_H

#include "stm32f10x.h"

////IO��������											   

#define DS18B20_DQ_OUT(x)   do{ x ? \
                                GPIO_SetBits(GPIOG, GPIO_Pin_11) : \
                                GPIO_ResetBits(GPIOG, GPIO_Pin_11); \
                            }while(0)                                                       /* ���ݶ˿���� */
#define DS18B20_DQ_IN       GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_11)     /* ���ݶ˿����� */

uint8_t ds18b20_init(void);         /* ��ʼ��DS18B20 */
uint8_t ds18b20_check(void);        /* ����Ƿ����DS18B20 */
short ds18b20_get_temperature(void);/* ��ȡ�¶� */

#endif


