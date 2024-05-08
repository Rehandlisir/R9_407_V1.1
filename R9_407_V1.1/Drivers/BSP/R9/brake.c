/**
 ****************************************************************************************************
 * @file        brake.c
 * @author      Lisir
 * @version     V1.0
 * @date        2021-10-14
 * @brief       ��բ ��������
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
 * �޸�˵��
 * none
 * ��һ�η���  
 *
 ****************************************************************************************************
 */
#include "./BSP/R9/brake.h"

#include "./SYSTEM/delay/delay.h"

/**
 * @brief       ��ʼ��BRAKE���IO��, ��ʹ��ʱ��
 * @param       ��
 * @retval      ��

 */
void brake_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    BRAKE1_GPIO_CLK_ENABLE();                                 /* ��բ1ʱ��ʹ�� */
    BRAKE2_GPIO_CLK_ENABLE();                                 /* ��բ2ʱ��ʹ�� */

    gpio_init_struct.Pin = BRAKE1_GPIO_PIN;                   /* ��բ1���� */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* ������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* ���� */
    HAL_GPIO_Init(BRAKE1_GPIO_PORT, &gpio_init_struct);       /* ��ʼ����բ1���� */

    gpio_init_struct.Pin = BRAKE2_GPIO_PIN;                   /* ��բ2���� */
    HAL_GPIO_Init(BRAKE2_GPIO_PORT, &gpio_init_struct);       /* ��ʼ����բ2���� */
    
    BRAKE1(1);                                                /* �ر� ��բ1 */
    BRAKE2(1);                                                /* �ر� ��բ1 */
}


