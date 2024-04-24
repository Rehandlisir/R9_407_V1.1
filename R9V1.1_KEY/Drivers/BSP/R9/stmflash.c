/**
 ****************************************************************************************************
 * @file        stmflash.c
 * @author      rehand�Ŷ�(REHAND)
 * @version     V1.0
 * @date        2021-10-28
 * @brief       STM32�ڲ�FLASH��д ��������
 * @license     Copyright (c) 2020-2032, ����ҽ��
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:rehand ̽���� F407������
 * ����ҽ��
 * rehand
 *����ҽ��
 * ����ҽ��
 *
 * �޸�˵��
 * ����ҽ��
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/R9/stmflash.h"

/**
 * @brief       ��ָ����ַ��ȡһ���� (32λ����)
 * @param       faddr   : ��ȡ��ַ (�˵�ַ����Ϊ4����!!)
 * @retval      ��ȡ�������� (32λ)
 */
uint32_t stmflash_read_word(uint32_t faddr)
{
    return *(volatile uint32_t *)faddr;
}

/**
 * @brief       ��ȡĳ����ַ���ڵ�flash����
 * @param       addr    : lash��ַ
 * @retval      0~11,��addr���ڵ�����
 */
uint8_t  stmflash_get_flash_sector(uint32_t addr)
{
    if (addr < ADDR_FLASH_SECTOR_1) return FLASH_SECTOR_0;
    else if (addr < ADDR_FLASH_SECTOR_2) return FLASH_SECTOR_1;
    else if (addr < ADDR_FLASH_SECTOR_3) return FLASH_SECTOR_2;
    else if (addr < ADDR_FLASH_SECTOR_4) return FLASH_SECTOR_3;
    else if (addr < ADDR_FLASH_SECTOR_5) return FLASH_SECTOR_4;
    else if (addr < ADDR_FLASH_SECTOR_6) return FLASH_SECTOR_5;
    else if (addr < ADDR_FLASH_SECTOR_7) return FLASH_SECTOR_6;
    else if (addr < ADDR_FLASH_SECTOR_8) return FLASH_SECTOR_7;
    else if (addr < ADDR_FLASH_SECTOR_9) return FLASH_SECTOR_8;
    else if (addr < ADDR_FLASH_SECTOR_10) return FLASH_SECTOR_9;
    else if (addr < ADDR_FLASH_SECTOR_11) return FLASH_SECTOR_10;
    return FLASH_SECTOR_11;
}

/**
 * @brief       ��FLASH ָ��λ��, д��ָ�����ȵ�����(�Զ�����)
 *   @note      ��ΪSTM32F4������ʵ��̫��,û�취���ر�����������,���Ա�����д��ַ�����0XFF
 *              ,��ô���Ȳ������������Ҳ�������������.����д��0XFF�ĵ�ַ,�����������������ݶ�ʧ.
 *              ����д֮ǰȷ��������û����Ҫ����,��������������Ȳ�����,Ȼ����������д.
 *              �ú�����OTP����Ҳ��Ч!��������дOTP��!
 *              OTP�����ַ��Χ:0X1FFF7800~0X1FFF7A0F(ע�⣺���16�ֽڣ�����OTP���ݿ�����������д����)
 * @param       waddr   : ��ʼ��ַ (�˵�ַ����Ϊ4�ı���!!,����д�����!)
 * @param       pbuf    : ����ָ��
 * @param       length  : Ҫд��� ��(32λ)��(����Ҫд���32λ���ݵĸ���)
 * @retval      ��
 */
void stmflash_write(uint32_t waddr, uint32_t *pbuf, uint32_t length)
{
    FLASH_EraseInitTypeDef flasheraseinit;
    HAL_StatusTypeDef FlashStatus=HAL_OK;

    uint32_t addrx = 0;
    uint32_t endaddr = 0;
    uint32_t sectorerror=0;
    
    if (waddr < STM32_FLASH_BASE || waddr % 4 ||        /* д���ַС�� STM32_FLASH_BASE, ����4��������, �Ƿ�. */
        waddr > (STM32_FLASH_BASE + STM32_FLASH_SIZE))  /* д���ַ���� STM32_FLASH_BASE + STM32_FLASH_SIZE, �Ƿ�. */
    {
        return;
    }

    HAL_FLASH_Unlock();             /* ���� */
    FLASH->ACR &= ~(1 << 10);       /* FLASH�����ڼ�,�����ֹ���ݻ���!!! */

    addrx = waddr;                  /* д�����ʼ��ַ */
    endaddr = waddr + length * 4;   /* д��Ľ�����ַ */

    if (addrx < 0X1FFF0000)         /* ֻ�����洢��,����Ҫִ�в�������!! */
    {
        while (addrx < endaddr)     /* ɨ��һ���ϰ�.(�Է�FFFFFFFF�ĵط�,�Ȳ���) */
        {
            if (stmflash_read_word(addrx) != 0XFFFFFFFF)    /* �з�0XFFFFFFFF�ĵط�,Ҫ����������� */
            {
                flasheraseinit.TypeErase=FLASH_TYPEERASE_SECTORS;       /* �������ͣ��������� */
                flasheraseinit.Sector=stmflash_get_flash_sector(addrx); /* Ҫ���������� */
                flasheraseinit.NbSectors=1;                             /* һ��ֻ����һ������ */
                flasheraseinit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      /* ��ѹ��Χ��VCC=2.7~3.6V֮��!! */

                if(HAL_FLASHEx_Erase(&flasheraseinit, &sectorerror) != HAL_OK) 
                {
                    break;/* ���������� */
                }

            }
            else
            {
                addrx += 4;
            }
            FLASH_WaitForLastOperation(FLASH_WAITETIME);                 /* �ȴ��ϴβ������ */
        }
    }

    FlashStatus=FLASH_WaitForLastOperation(FLASH_WAITETIME);             /* �ȴ��ϴβ������ */

    if (FlashStatus==HAL_OK)
    {
        while (waddr < endaddr)     /* д���� */
        {
            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, waddr, *pbuf) != HAL_OK)  /* д������ */
            {
                break;              /* д���쳣 */
            }

            waddr += 4;
            pbuf++;
        }
    }
    
    FLASH->ACR |= 1 << 10;          /* FLASH��������,��������fetch */

    HAL_FLASH_Lock();               /* ���� */
}   

/**
 * @brief       ��ָ����ַ��ʼ����ָ�����ȵ�����
 * @param       raddr : ��ʼ��ַ
 * @param       pbuf  : ����ָ��
 * @param       length: Ҫ��ȡ����(32)��,��4���ֽڵ�������
 * @retval      ��
 */
void stmflash_read(uint32_t raddr, uint32_t *pbuf, uint32_t length)
{
    uint32_t i;

    for (i = 0; i < length; i++)
    {
        pbuf[i] = stmflash_read_word(raddr);    /* ��ȡ4���ֽ�. */
        raddr += 4; /* ƫ��4���ֽ�. */
    }
}

/******************************************************************************************/
/* �����ô��� */

/**
 * @brief       ����д����(д1����)
 * @param       waddr : ��ʼ��ַ
 * @param       wdata : Ҫд�������
 * @retval      ��ȡ��������
 */
void test_write(uint32_t waddr, uint32_t wdata)
{
    stmflash_write(waddr, &wdata, 1);   /* д��һ���� */
}


   

