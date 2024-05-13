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
 *
 ****************************************************************************************************
 */

#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/R9/Slavemodbus.h"

/**
 * @brief       ������ʼ������
 * @param       ��
 * @retval      ��
 */
void key_init(void)
{
    GPIO_InitTypeDef gpioE_init_struct;                          /* GPIO���ò����洢���� */
    __HAL_RCC_GPIOE_CLK_ENABLE();
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
}

uint8_t key_scan1(void)
{
    if(KEY1 == 0)
    {
        delay_ms(10);   /* ���� */
        if(KEY1== 0)
        {
       
            return 1;   /* ���������� */
        }
    }
    else
    { 
        return 0 ;   /* ����û�а��� */
        }
   
}


uint8_t key_scan2(void)
{
    if(KEY2 == 0)
    {
        delay_ms(10);   /* ���� */
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
        delay_ms(10);   /* ���� */
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
        delay_ms(10);   /* ���� */
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
        delay_ms(10);   /* ���� */
        if(KEY5== 0)
        {
            while(KEY5 == 0);  /* �ȴ������ɿ� */
            return 1;   /* ���������� */
        }
    }
    else
    { 
        return 0;   /* ����û�а��� */
    }
   
}

uint8_t key_scandouble(void)
{
    if(KEY2 == 0 && KEY3 == 0)
     {

        delay_ms(10);
        if (KEY2 == 0 && KEY3 == 0)
        {
            while(KEY2 == 0 && KEY3 == 0);
            return 1;   /* ˫������������ */
        }


     }
    else
    {
        return 0; 
    }
}

uint16_t  keycmdbulb(void)  // ���������·�������ָ��
{

    if (g_slaveReg[117] == 1)
    {
         delay_ms(5); 
         if (g_slaveReg[117] == 1)
         {
            while(g_slaveReg[117] == 1); // �ȴ������ɿ�
             return 1; 
         }
        
    }
    else
    {
        return 0;
    }
}


uint16_t  keycmdleftbulb(void)  // ���������·�����ת���ָ��
{

    if (g_slaveReg[114] == 1)
    {
         delay_ms(5); 
         if (g_slaveReg[114] == 1)
         {
            while(g_slaveReg[114] == 1); // �ȴ������ɿ�
             return 1; 
         }    
    }
    else
    {
        return 0;
    }
    

}

uint8_t  keycmdrightbulb(void)  
{
    if (g_slaveReg[115] == 1)
    {
         delay_ms(5); 
         if (g_slaveReg[115] == 1)
         {
            while(g_slaveReg[115] == 1); // �ȴ������ɿ�
             return 1; 
         }
        
    }
    else
    {
        return 0;
    }
}