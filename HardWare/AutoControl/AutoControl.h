#ifndef __AUTOCONTROPL_H
#define __AUTOCONTROPL_H
#define Free_Value 1000

#include "stm32f10x.h"

extern uint8_t AutoControl_mode;

struct TEM_Struct
{
	uint16_t TEM_free;//������ʱ
	uint32_t  TEM_value;
	uint32_t TEM_FirstMaxValue;//��ֵ1
    uint32_t TEM_SecondMaxValue;//��ֵ2
	uint16_t TEM_MiniValue;//��С��ֵ
    uint16_t TEM_Status; //״̬
};

struct ENV_Struct
{
    uint16_t ENV_free;//������ʱ
	uint32_t  ENV_value;
	uint32_t ENV_MaxValue;//�����ֵ
	uint16_t ENV_MiniValue;//��С��ֵ
    uint16_t ENV_Status; // ״̬
};
extern struct TEM_Struct tempControl;//�¶ȿ��ƽṹ��
extern struct ENV_Struct lightControl;//�¶ȿ��ƽṹ��



void Control_Init(void);
void Control_Execute(void);
void ledlevel_change(void);//�Զ��ɿص�
void manual_control(void);//�ֶ�

#endif

