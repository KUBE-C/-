#include "Stmflash.h"
#include "Delay.h"
#include "stm32f10x_flash.h"

uint16_t  STMFLASH_ReadHalfWord(u32 faddr)
{
    return *(vu16*)faddr;
}

#if STM32_FLASH_WREN  // ���ʹ��д�룬Ĭ��ʹ��
/***************************************************************************
�������ƣ�STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)  
�������ܣ�������д��
��ڲ�����WriteAddr:��ʼ��ַ��pBuffer:����ָ�룻NumToWrite:����(16λ)�� 
���ز�������
****************************************************************************/
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{
    uint16_t i;
    for (i = 0;i < NumToWrite;i++)
    {
        FLASH_ProgramHalfWord(WriteAddr, pBuffer[i]);
        WriteAddr += 2; 
    }
    
}

/***************************************************************************
�������ƣ�STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)  
�������ܣ���ָ����ַ��ʼд��ָ�����ȵ�����
��ڲ�����WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)��
         pBuffer:����ָ�룻
         NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
���ز�������
****************************************************************************/
#if STM32_FLASH_SIZE < 256
#define STM_SECTOR_SIZE 1024  			/* �ֽ� */
#else 
#define STM_SECTOR_SIZE 2048
#endif   
uint16_t STMFLASH_BUF[STM_SECTOR_SIZE/2];      /* �����2K�ֽ� */

void STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)
{
    uint32_t secpos;    // ������ַ
    uint16_t secoff;    // ������ƫ�Ƶ�ַ��16λ�ּ��㣩
    uint16_t secremain; // ������ʣ���ַ��16λ�ּ��㣩
    uint16_t i;
    uint32_t offaddr;   // ȥ��0x08000000��ĵ�ַ
    if (WriteAddr < STM32_FLASH_BASE || (WriteAddr >= (STM32_FLASH_BASE + 1024*STM32_FLASH_SIZE))) // �Ƿ���ַ
        return;
     
    FLASH_Unlock(); //����
    offaddr = WriteAddr - STM32_FLASH_BASE;   // ʵ��ƫ�Ƶ�ַ
    secpos = offaddr / STM_SECTOR_SIZE;       // ������ַ  0~127 for STM32F103RBT6
    secoff = (offaddr % STM_SECTOR_SIZE) / 2; // �������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
    secremain = STM_SECTOR_SIZE/2 - secoff;   // ����ʣ��ռ��С

    if (NumToWrite <= secremain)  secremain = NumToWrite;
    
    while(1)
    {
        STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE, STMFLASH_BUF, STM_SECTOR_SIZE/2); // ��ȡ��������������
        for (i = 0; i < secremain; i++) // У������
        {
            if (STMFLASH_BUF[secoff+i]!=0xffff) break;
        }
        if (i < secremain) //��Ҫ����
        {
            FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE); // �����������
            for (i = 0;i < secremain; i++)  // ����
            {
                STMFLASH_BUF[i+secoff] = pBuffer[i];
            }
            STMFLASH_Write_NoCheck(WriteAddr, pBuffer, secremain);
        }
        else
        {
            STMFLASH_Write_NoCheck(WriteAddr, pBuffer, secremain); // д�Ѿ������˵�,ֱ��д������ʣ������
        }
        
        if (NumToWrite == secremain) break;
        else
        {
            secpos++;
            secoff = 0;
            pBuffer += secremain;
            WriteAddr += secremain;
            NumToWrite -= secremain;
            
            if (NumToWrite > (STM_SECTOR_SIZE/2)) secremain = STM_SECTOR_SIZE/2; // ��һ����������д����
            else secremain = NumToWrite;
        }
    };
    
    FLASH_Lock();

}

#endif

/******************************************************************************
�������ƣ�STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   
�������ܣ���ָ����ַ��ʼ����ָ�����ȵ�����
��ڲ�����ReadAddr:��ʼ��ַ��pBuffer:����ָ�룻NumToWrite:����(16λ)��
���ز�������
*******************************************************************************/
void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead)   
{
    uint16_t i;
    for (i = 0;i < NumToRead; i++)
    {
        pBuffer[i] = STMFLASH_ReadHalfWord(ReadAddr);  // ��ȡ�����ֽ�
        ReadAddr += 2;
    }
}
