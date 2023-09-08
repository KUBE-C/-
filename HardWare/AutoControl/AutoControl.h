#ifndef __AUTOCONTROPL_H
#define __AUTOCONTROPL_H
#define Free_Value 1000

#include "stm32f10x.h"

extern uint8_t AutoControl_mode;

// ���� Flash �洢����ʼ��ַ
#define FLASH_SAVE_ADDR 0X08070000  // ����FLASH�����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000) 
extern char values[256];
#define SIZE sizeof(values)
extern uint8_t datatemp[SIZE];

struct TEM_Struct
{
	uint32_t  TEM_value;
	uint32_t TEM_FirstMaxValue;//��ֵ1
    uint32_t TEM_SecondMaxValue;//��ֵ2
	uint16_t TEM_MiniValue;//��С��ֵ
    uint16_t TEM_Status; //״̬
};

struct ENV_Struct
{
	uint32_t  ENV_value;
	uint32_t ENV_MaxValue;//�����ֵ
	uint16_t ENV_MiniValue;//��С��ֵ
    uint16_t ENV_Status; // ״̬
};
extern struct TEM_Struct tempControl;//�¶ȿ��ƽṹ��
extern struct ENV_Struct lightControl;//�¶ȿ��ƽṹ��

void value_change(uint32_t tem1, uint32_t tem2, uint32_t light);

void Control_Init(void);
void Control_Execute(void);
void ledlevel_change(void);//�Զ��ɿص�
void manual_control(void);//�ֶ�

#endif

