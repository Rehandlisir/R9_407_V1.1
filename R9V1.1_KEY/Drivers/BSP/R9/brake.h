/**
 ****************************************************************************************************
 * @file          brake.h
 * @author      Lisir
 * @version     V1.0
 * @date        2021-10-14
 * @brief       LED ��������
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
#ifndef __BRAKE_H
#define __BRAKE_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* ���� ���� */

#define BRAKE1_GPIO_PORT                  GPIOD
#define BRAKE1_GPIO_PIN                   GPIO_PIN_8
#define BRAKE1_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)             /* PC��ʱ��ʹ�� */

#define BRAKE2_GPIO_PORT                  GPIOD
#define BRAKE2_GPIO_PIN                   GPIO_PIN_9
#define BRAKE2_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)             /* PC��ʱ��ʹ�� */





/******************************************************************************************/

/* ��բ�˿ڶ��� *//* �߼��汧ס���*/
#define BRAKE1(x)   do{ x ? \
                      HAL_GPIO_WritePin(BRAKE1_GPIO_PORT, BRAKE1_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(BRAKE1_GPIO_PORT, BRAKE1_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)       /* LED0 = RED */

#define BRAKE2(x)   do{ x ? \
                      HAL_GPIO_WritePin(BRAKE2_GPIO_PORT, BRAKE2_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(BRAKE2_GPIO_PORT, BRAKE2_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)       /* LED1 = GREEN */

/* LEDȡ������ */
//#define LED0_TOGGLE()    do{ HAL_GPIO_TogglePin(LED0_GPIO_PORT, LED0_GPIO_PIN); }while(0)       /* LED0 = !LED0 */
//#define LED1_TOGGLE()    do{ HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_GPIO_PIN); }while(0)       /* LED1 = !LED1 */

/******************************************************************************************/


/* �ⲿ�ӿں���*/
void brake_init(void);  
                                                                                                  /* ��ʼ�� */

#endif                                                                                                                       /*��ת��ƿ���*/
