#ifndef  __STMFLASH_H
#define __STMFLASH_H

#include "stm32f10x.h"

/* ���� ----------------------------------------------------------------------*/
#define STM32_FLASH_SIZE 512                    /* STM32F103ZET6��FLASH��512K */
#define STM32_FLASH_WREN 1                 /* ʹ��FLASHд��(0��������;1��ʹ��) */
#define STM32_FLASH_BASE 0x08000000                  /* STM32 FLASH����ʼ��ַ */
/* �������� ------------------------------------------------------------------*/
uint16_t  STMFLASH_ReadHalfWord(u32 faddr);                            /* �������� */  
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);        /* ��ָ����ַ��ʼд��ָ�����ȵ����� */
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);           /* ��ָ����ַ��ʼ����ָ�����ȵ����� */   


#endif

