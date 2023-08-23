#ifndef __LSENS_H
#define __LSENS_H

#include "sys.h"


/******************************************************************************************/
/* ������������ӦADC3���������ź�ͨ�� ���� */

#define LSENS_ADC3_CHX_GPIO_PORT            GPIOF
#define LSENS_ADC3_CHX_GPIO_PIN             SYS_GPIO_PIN8
#define LSENS_ADC3_CHX_GPIO_CLK_ENABLE()    do{ RCC->APB2ENR |= 1 << 7; }while(0)   /* PF��ʱ��ʹ�� */


#define LSENS_ADC3_CHX                      6       /* ͨ��Y,  0 <= Y <= 17 */ 

/******************************************************************************************/
 

void lsens_init(void);          /* ��ʼ������������ */
uint8_t lsens_get_val(void);    /* ��ȡ������������ֵ */
#endif 


