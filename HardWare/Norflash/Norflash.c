#include "Norflash.h"
#include "Spi.h"

uint16_t g_norflash_type = NM25Q128;     /* Ĭ����NM25Q128 */

void Norflash_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTBʱ��ʹ�� 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  // PB12 ���� 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOB,GPIO_Pin_12);
 
     Norflash_CS(1);				//SPI FLASH��ѡ��
	SPI2_Init();		   	//��ʼ��SPI
	g_norflash_type=Norflash_read_id();//��ȡFLASH ID.  
}

uint16_t Norflash_read_id(void)
{
    uint16_t deviceid;
    
    Norflash_CS(0);
    SPI_SwapByte(FLASH_ManufactDeviceID);
    SPI_SwapByte(0x00);
    SPI_SwapByte(0x00);
    SPI_SwapByte(0x00);
    deviceid = SPI_SwapByte(0xFF) << 8;     /* ��ȡ��8λ�ֽ� */
    deviceid |= SPI_SwapByte(0xFF);         /* ��ȡ��8λ�ֽ� */
    Norflash_CS(1);
    
    return deviceid;
}

void Norflash_WriteEnable(void)
{
	Norflash_CS(0);
	SPI_SwapByte(FLASH_WriteEnable);
	Norflash_CS(1);
}

void Norflash_WaitBusy(void)
{
	uint32_t Timeout;
	Norflash_CS(0);
	SPI_SwapByte(FLASH_ReadStatusReg1);
	Timeout = 100000;
	while ((SPI_SwapByte(0xff) & 0x01) == 0x01)
	{
		Timeout --;
		if (Timeout == 0)
		{
			break;
		}
	}
	Norflash_CS(1);
}

void Norflash_Read(uint32_t Address, uint8_t *DataArray, uint32_t Count)
{
	uint32_t i;
	Norflash_CS(0);
	SPI_SwapByte(FLASH_ReadData);
	SPI_SwapByte(Address >> 16);
	SPI_SwapByte(Address >> 8);
	SPI_SwapByte(Address);
	for (i = 0; i < Count; i ++)
	{
		DataArray[i] = SPI_SwapByte(0xff);
	}
	Norflash_CS(1);
}

void Norflash_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
	uint16_t i;
	
	Norflash_WriteEnable();
	
	Norflash_CS(0);
	SPI_SwapByte(FLASH_PageProgram);
	SPI_SwapByte(Address >> 16);
	SPI_SwapByte(Address >> 8);
	SPI_SwapByte(Address);
	for (i = 0; i < Count; i ++)
	{
		SPI_SwapByte(DataArray[i]);
	}
	Norflash_CS(1);
	
	Norflash_WaitBusy();
}

/**
 * @brief       �޼���дSPI FLASH
 *   @note      ����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
 *              �����Զ���ҳ����
 *              ��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
 *
 * @param       DataArray    : ���ݴ洢��
 * @param       addr    : ��ʼд��ĵ�ַ(���32bit)
 * @param       Count : Ҫд����ֽ���(���65535)
 * @retval      ��
 */
static void Norflash_Write_Nocheck(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
    uint16_t pageremain;
    pageremain = 256 - Address % 256;  /* ��ҳʣ����ֽ��� */

    if (Count <= pageremain)      /* ������256���ֽ� */
    {
        pageremain = Count;
    }

    while (1)
    {
        /* ��д���ֽڱ�ҳ��ʣ���ַ���ٵ�ʱ��, һ����д��
         * ��д��ֱ�ӱ�ҳ��ʣ���ַ�����ʱ��, ��д������ҳ��ʣ���ַ, Ȼ�����ʣ�೤�Ƚ��в�ͬ����
         */
        Norflash_PageProgram(Address, DataArray, pageremain);

        if (Count == pageremain)   /* д������� */
        {
            break;
        }
        else     /* Count > pageremain */
        {
            DataArray += pageremain;         /* DataArrayָ���ַƫ��,ǰ���Ѿ�д��pageremain�ֽ� */
            Address += pageremain;         /* д��ַƫ��,ǰ���Ѿ�д��pageremain�ֽ� */
            Count -= pageremain;      /* д���ܳ��ȼ�ȥ�Ѿ�д���˵��ֽ��� */

            if (Count > 256)          /* ʣ�����ݻ�����һҳ,����һ��дһҳ */
            {
                pageremain = 256;       /* һ�ο���д��256���ֽ� */
            }
            else     /* ʣ������С��һҳ,����һ��д�� */
            {
                pageremain = Count;   /* ����256���ֽ��� */
            }
        }
    }
}

/**
 * @brief       дSPI FLASH
 *   @note      ��ָ����ַ��ʼд��ָ�����ȵ����� , �ú�������������!
 *              SPI FLASH һ����: 256���ֽ�Ϊһ��Page, 4KbytesΪһ��Sector, 16������Ϊ1��Block
 *              ��������С��λΪSector.
 *
 * @param       DataArray    : ���ݴ洢��
 * @param       addr    : ��ʼд��ĵ�ַ(���32bit)
 * @param       Count : Ҫд����ֽ���(���65535)
 * @retval      ��
 */
uint8_t g_norflash_buf[4096];   /* �������� */

void Norflash_Write(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;
    uint8_t *norflash_buf;

    norflash_buf = g_norflash_buf;
    secpos = Address / 4096;       /* ������ַ */
    secoff = Address % 4096;       /* �������ڵ�ƫ�� */
    secremain = 4096 - secoff;  /* ����ʣ��ռ��С */

    //printf("ad:%X,nb:%X\r\n", addr, Count); /* ������ */
    if (Count <= secremain)
    {
        secremain = Count;    /* ������4096���ֽ� */
    }

    while (1)
    {
        Norflash_Read(secpos * 4096, norflash_buf, 4096);   /* ������������������ */

        for (i = 0; i < secremain; i++)   /* У������ */
        {
            if (norflash_buf[secoff + i] != 0XFF)
            {
                break;      /* ��Ҫ����, ֱ���˳�forѭ�� */
            }
        }

        if (i < secremain)   /* ��Ҫ���� */
        {
            Norflash_SectorErase(secpos* 4096);  /* ����������� */

            for (i = 0; i < secremain; i++)   /* ���� */
            {
                norflash_buf[i + secoff] = DataArray[i];
            }

            Norflash_Write_Nocheck(secpos * 4096, norflash_buf, 4096);  /* д���������� */
        }
        else        /* д�Ѿ������˵�,ֱ��д������ʣ������. */
        {
            Norflash_Write_Nocheck(Address, DataArray, secremain);  /* ֱ��д���� */
        }

        if (Count == secremain)
        {
            break;  /* д������� */
        }
        else        /* д��δ���� */
        {
            secpos++;               /* ������ַ��1 */
            secoff = 0;             /* ƫ��λ��Ϊ0 */

            DataArray += secremain;      /* ָ��ƫ�� */
            Address += secremain;      /* д��ַƫ�� */
            Count -= secremain;   /* �ֽ����ݼ� */

            if (Count > 4096)
            {
                secremain = 4096;   /* ��һ����������д���� */
            }
            else
            {
                secremain = Count;/* ��һ����������д���� */
            }
        }
    }
}

void Norflash_SectorErase(uint32_t Address)
{
	Norflash_WriteEnable();
	
	Norflash_CS(0);
	SPI_SwapByte(FLASH_SectorErase);
	SPI_SwapByte(Address >> 16);
	SPI_SwapByte(Address >> 8);
	SPI_SwapByte(Address);
	Norflash_CS(1);
	
	Norflash_WaitBusy();
}



