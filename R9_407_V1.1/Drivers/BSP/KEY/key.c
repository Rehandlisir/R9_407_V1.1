/**
 ****************************************************************************************************
 * @file        key.c
 * @author      Lisir
 * @version     V1.0
 * @date        2021-10-14
 * @brief       �������� ��������
 * @license     Copyright (c) 2024, ���ڸ���ҽ�ƿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:F407������
 * none
 * none
 * none
 * none
 *
   KEY1: ��̬ 1
   KEY2: ��ת��
   KEY3: ��ת��
   KEY4: ������
   KEY5: ��̬2
   KEY6: ˫��
   KEY7: ���� 
   KEY8: ���ټ�
   KEY9: 360V iew
   KEY10: ����
   KEY11 :����Ԥ��
 *
 ****************************************************************************************************
 */

#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"


/**
 * @brief       ������ʼ������
 * @param       ��
 * @retval      ��
 */
void key_init(void)
{
    GPIO_InitTypeDef gpioE_init_struct;                          /* GPIO���ò����洢���� */
    GPIO_InitTypeDef gpioB_init_struct;                          /* GPIO���ò����洢���� */
    GPIO_InitTypeDef gpioG_init_struct;                          /* GPIO���ò����洢���� */


    gpioE_init_struct.Pin = KEY1_GPIO_PIN;                       /* KEY1���� */
    gpioE_init_struct.Mode = GPIO_MODE_INPUT;                    /* ���� */
    gpioE_init_struct.Pull = GPIO_NOPULL;                        /* ���� */
    gpioE_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* ���� */
    HAL_GPIO_Init(KEY1_GPIO_PORT, &gpioE_init_struct);           /* KEY1����ģʽ����,�������� */

    gpioE_init_struct.Pin = KEY2_GPIO_PIN;                       /* KEY2���� */
    gpioE_init_struct.Mode = GPIO_MODE_INPUT;                    /* ���� */
    gpioE_init_struct.Pull = GPIO_NOPULL;                        /* ���� */
    gpioE_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* ���� */
    HAL_GPIO_Init(KEY2_GPIO_PORT, &gpioE_init_struct);           /* KEY2����ģʽ����,�������� */

    gpioE_init_struct.Pin = KEY3_GPIO_PIN;                       /* KEY3���� */
    gpioE_init_struct.Mode = GPIO_MODE_INPUT;                    /* ���� */
    gpioE_init_struct.Pull = GPIO_NOPULL;                        /* ���� */
    gpioE_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* ���� */
    HAL_GPIO_Init(KEY3_GPIO_PORT, &gpioE_init_struct);           /* KEY3����ģʽ����,�������� */

    gpioE_init_struct.Pin = KEY4_GPIO_PIN;                       /* KEY4���� */
    gpioE_init_struct.Mode = GPIO_MODE_INPUT;                    /* ���� */
    gpioE_init_struct.Pull = GPIO_NOPULL;                        /* ���� */
    gpioE_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* ���� */
    HAL_GPIO_Init(KEY4_GPIO_PORT, &gpioE_init_struct);           /* KEY4����ģʽ����,�������� */

    gpioE_init_struct.Pin = KEY5_GPIO_PIN;                       /* KEY5���� */
    gpioE_init_struct.Mode = GPIO_MODE_INPUT;                    /* ���� */
    gpioE_init_struct.Pull = GPIO_NOPULL;                        /* ���� */
    gpioE_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* ���� */
    HAL_GPIO_Init(KEY5_GPIO_PORT, &gpioE_init_struct);           /* KEY5����ģʽ����,�������� */

    gpioG_init_struct.Pin = KEY6_GPIO_PIN;                       /* KEY6���� */
    gpioG_init_struct.Mode = GPIO_MODE_INPUT;                    /* ���� */
    gpioG_init_struct.Pull = GPIO_NOPULL;                        /* ���� */
    gpioG_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* ���� */
    HAL_GPIO_Init(KEY6_GPIO_PORT, &gpioG_init_struct);           /* KEY6����ģʽ����,�������� */

    gpioG_init_struct.Pin = KEY7_GPIO_PIN;                       /* KEY7���� */
    gpioG_init_struct.Mode = GPIO_MODE_INPUT;                    /* ���� */
    gpioG_init_struct.Pull = GPIO_NOPULL;                        /* ���� */
    gpioG_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* ���� */
    HAL_GPIO_Init(KEY7_GPIO_PORT, &gpioG_init_struct);           /* KEY7����ģʽ����,�������� */

    gpioE_init_struct.Pin = KEY8_GPIO_PIN;                       /* KEY8���� */
    gpioE_init_struct.Mode = GPIO_MODE_INPUT;                    /* ���� */
    gpioE_init_struct.Pull = GPIO_NOPULL;                        /* ���� */
    gpioE_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* ���� */
    HAL_GPIO_Init(KEY8_GPIO_PORT, &gpioE_init_struct);           /* KEY8����ģʽ����,�������� */

    gpioB_init_struct.Pin = KEY9_GPIO_PIN;                       /* KEY9���� */
    gpioB_init_struct.Mode = GPIO_MODE_INPUT;                    /* ���� */
    gpioB_init_struct.Pull = GPIO_NOPULL;                        /* ���� */
    gpioB_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* ���� */
    HAL_GPIO_Init(KEY9_GPIO_PORT, &gpioB_init_struct);           /* KEY9����ģʽ����,�������� */

    gpioB_init_struct.Pin = KEY10_GPIO_PIN;                       /* KEY10���� */
    gpioB_init_struct.Mode = GPIO_MODE_INPUT;                    /* ���� */
    gpioB_init_struct.Pull = GPIO_NOPULL;                        /* ���� */
    gpioB_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* ���� */
    HAL_GPIO_Init(KEY10_GPIO_PORT, &gpioB_init_struct);           /* KEY10����ģʽ����,�������� */

 

    gpioE_init_struct.Pin = KEY11_GPIO_PIN;                       /* KEY11���� */
    gpioE_init_struct.Mode = GPIO_MODE_INPUT;                    /* ���� */
    gpioE_init_struct.Pull = GPIO_NOPULL;                        /* ���� */
    gpioE_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* ���� */
    HAL_GPIO_Init(KEY11_GPIO_PORT, &gpioE_init_struct);           /* KEY11����ģʽ����,�������� */        
}

/**
 * @brief       ����ɨ�躯��
 * @note        �ú�������Ӧ���ȼ�(ͬʱ���¶������): WK_UP > KEY2 > KEY1 > KEY0!!
 * @param       mode:0 / 1, ���庬������:
 *   @arg       0,  ��֧��������(���������²���ʱ, ֻ�е�һ�ε��û᷵�ؼ�ֵ,
 *                  �����ɿ��Ժ�, �ٴΰ��²Ż᷵��������ֵ)
 *   @arg       1,  ֧��������(���������²���ʱ, ÿ�ε��øú������᷵�ؼ�ֵ)
 * @retval      ��ֵ, ��������:
 *              KEY0_PRES, 1, KEY0����
 *              KEY1_PRES, 2, KEY1����
 *              KEY2_PRES, 3, KEY2����
 *              WKUP_PRES, 4, WKUP����
 */


uint8_t key_scan1(void)
{
    if(KEY1 == 0)
    {
        delay_ms(20);   /* ���� */
        if(KEY1== 0)
        {
           // while(KEY_T1PUSH == 0);  /* �ȴ������ɿ� */
            return 1;   /* ���������� */
        }
    }
    else
    { 
        return 0;   /* ����û�а��� */
        }
   
}


uint8_t key_scan2(void)
{
    if(KEY2 == 0)
    {
        delay_ms(20);   /* ���� */
        if(KEY2== 0)
        {
            while(KEY2 == 0);  /* �ȴ������ɿ� */
            return 1;   /* ���������� */
        }
    }
    else
    { 
        return 0;   /* ����û�а��� */
        }
   
}

uint8_t key_scan3(void)
{
    if(KEY3 == 0)
    {
        delay_ms(20);   /* ���� */
        if(KEY3== 0)
        {
            while(KEY3 == 0);  /* �ȴ������ɿ� */
            return 1;   /* ���������� */
        }
    }
    else
    { 
        return 0;   /* ����û�а��� */
        }
   
}


uint8_t key_scan4(void)
{
    if(KEY4 == 0)
    {
        delay_ms(20);   /* ���� */
        if(KEY4== 0)
        {
            while(KEY4 == 0);  /* �ȴ������ɿ� */
            return 1;   /* ���������� */
        }
    }
    else
    { 
        return 0;   /* ����û�а��� */
        }
   
}

uint8_t key_scan5(void)
{
    if(KEY5 == 0)
    {
        delay_ms(20);   /* ���� */
        if(KEY5== 0)
        {
            //while(KEY_T1PUSH == 0);  /* �ȴ������ɿ� */
            return 1;   /* ���������� */
        }
    }
    else
    { 
        return 0;   /* ����û�а��� */
        }
   
}

uint8_t key_scan6(void)
{
    if(KEY6 == 0)
    {
        delay_ms(20);   /* ���� */
        if(KEY6== 0)
        {
            while(KEY6 == 0);  /* �ȴ������ɿ� */
            return 1;   /* ���������� */
        }
    }
    else
    { 
        return 0;   /* ����û�а��� */
        }
   
}

uint8_t key_scan7(void)
{
    if(KEY7 == 0)
    {
        delay_ms(20);   /* ���� */
        if(KEY7== 0)
        {
            //while(KEY7 == 0);  /* �ȴ������ɿ� */
            return 1;   /* ���������� */
        }
    }
    else
    { 
        return 0;   /* ����û�а��� */
        }
   
}

uint8_t key_scan8(void)
{
    if(KEY8 == 0)
    {
        delay_ms(20);   /* ���� */
        if(KEY8== 0)
        {
            while(KEY8 == 0);  /* �ȴ������ɿ� */
            return 1;   /* ���������� */
        }
    }
    else
    { 
        return 0;   /* ����û�а��� */
        }
   
}

uint8_t key_scan9(void)
{
    if(KEY9 == 0)
    {
        delay_ms(20);   /* ���� */
        if(KEY9== 0)
        {
            while(KEY9 == 0);  /* �ȴ������ɿ� */
            return 1;   /* ���������� */
        }
    }
    else
    { 
        return 0;   /* ����û�а��� */
        }
   
}

uint8_t key_scan10(void)
{
    if(KEY10 == 0)
    {
        delay_ms(20);   /* ���� */
        if(KEY10== 0)
        {
            //while(KEY10 == 0);  /* �ȴ������ɿ� */
            return 1;   /* ���������� */
        }
    }
    else
    { 
        return 0;   /* ����û�а��� */
    }
   
}

uint8_t key_scan11(void)
{
    if(KEY11 == 0)
    {
        delay_ms(20);   /* ���� */
        if(KEY11== 0)
        {
            while(KEY11 == 0);  /* �ȴ������ɿ� */
            return 1;   /* ���������� */
        }
    }
    else
    { 
        return 0;   /* ����û�а��� */
        }
   
}