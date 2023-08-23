#ifndef __ADC3_H
#define __ADC3_H

#include "stm32f10x.h"


/******************************************************************************************/
/* ADC3������ ���� */

#define ADC3_CHY_GPIO_PORT              GPIOA
#define ADC3_CHY_GPIO_PIN               GPIO_Pin_1 
#define ADC3_CHY_GPIO_CLK_ENABLE()      do{ RCC->APB2ENR |= 1 << 2; }while(0)   /* PA��ʱ��ʹ�� */

#define ADC3_CHY                        1                                       /* ͨ��Y,  0 <= Y <= 17 */ 

/******************************************************************************************/

void adc3_init(void);                               /* ADC3��ʼ�� */
void adc3_channel_set(uint8_t ch, uint8_t stime);   /* ADC3ͨ������ */
uint32_t adc3_get_result(uint8_t ch);               /* ���ĳ��ͨ��ֵ  */
uint32_t adc3_get_result_average(uint8_t ch, uint8_t times);/* �õ�ĳ��ͨ����������������ƽ��ֵ */

#endif 





