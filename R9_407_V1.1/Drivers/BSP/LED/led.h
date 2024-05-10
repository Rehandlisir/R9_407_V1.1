/**
 ****************************************************************************************************
 * @file        led.c
 * @author      Lisir
 * @version     V1.0
 * @date        2021-10-14
 * @brief       LED ��������
 * @license     Copyright (c) 2024, ���ڸ���ҽ�ƿƼ����޹�˾
 ****************************************************************************************************
 * @attention
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
#ifndef __LED_H
#define __LED_H

#include "./SYSTEM/sys/sys.h"

/******************************************************************************************/
/* ���Գ��� */

#define LED0_GPIO_PORT GPIOG
#define LED0_GPIO_PIN GPIO_PIN_2
#define LED0_GPIO_CLK_ENABLE()        \
    do                                \
    {                                 \
        __HAL_RCC_GPIOG_CLK_ENABLE(); \
    } while (0) /* PF��ʱ��ʹ�� */

#define LED1_GPIO_PORT GPIOG
#define LED1_GPIO_PIN GPIO_PIN_3
#define LED1_GPIO_CLK_ENABLE()        \
    do                                \
    {                                 \
        __HAL_RCC_GPIOG_CLK_ENABLE(); \
    } while (0) /* PF��ʱ��ʹ�� */

/*ת������*/
// ǰ��
#define LEFT_FRONT_TURE_GPIO_PORT GPIOC
#define LEFT_FRONT_TURE_GPIO_PIN GPIO_PIN_13
#define LEFT_FRONT_TURE_GPIO_CLK_ENABLE() \
    do                                    \
    {                                     \
        __HAL_RCC_GPIOC_CLK_ENABLE();     \
    } while (0) /* PC13 ????? */

#define RIGHT_FRONT_TURE_GPIO_PORT GPIOD
#define RIGHT_FRONT_TURE_GPIO_PIN GPIO_PIN_4
#define RIGHT_FRONT_TURE_GPIO_CLK_ENABLE() \
    do                                     \
    {                                      \
        __HAL_RCC_GPIOD_CLK_ENABLE();      \
    } while (0)

#define FRONT_MAIN_GPIO_PORT GPIOE
#define FRONT_MAIN_GPIO_PIN GPIO_PIN_1
#define FRONT_MAIN_GPIO_CLK_ENABLE()  \
    do                                \
    {                                 \
        __HAL_RCC_GPIOE_CLK_ENABLE(); \
    } while (0)

// ���

#define LEFT_BACK_TURE_GPIO_PORT GPIOE
#define LEFT_BACK_TURE_GPIO_PIN GPIO_PIN_3
#define LEFT_BACK_TURE_GPIO_CLK_ENABLE() \
    do                                   \
    {                                    \
        __HAL_RCC_GPIOE_CLK_ENABLE();    \
    } while (0) /* PC13 ????? */

#define RIGHT_BACK_TURE_GPIO_PORT GPIOE
#define RIGHT_BACK_TURE_GPIO_PIN GPIO_PIN_2
#define RIGHT_BACK_TURE_GPIO_CLK_ENABLE() \
    do                                    \
    {                                     \
        __HAL_RCC_GPIOE_CLK_ENABLE();     \
    } while (0)

#define BACK_MAIN_GPIO_PORT GPIOE
#define BACK_MAIN_GPIO_PIN GPIO_PIN_4
#define BACK_MAIN_GPIO_CLK_ENABLE()   \
    do                                \
    {                                 \
        __HAL_RCC_GPIOE_CLK_ENABLE(); \
    } while (0)

/******************************************************************************************/

/* LED���Ե� */
#define LED0(x)                                                                                                                                \
    do                                                                                                                                         \
    {                                                                                                                                          \
        x ? HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_GPIO_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_GPIO_PIN, GPIO_PIN_RESET); \
    } while (0) /* LED0 = RED */

#define LED1(x)                                                                                                                                \
    do                                                                                                                                         \
    {                                                                                                                                          \
        x ? HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_GPIO_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_GPIO_PIN, GPIO_PIN_RESET); \
    } while (0) /* LED1 = GREEN */

#define LED0_TOGGLE()                                      \
    do                                                     \
    {                                                      \
        HAL_GPIO_TogglePin(LED0_GPIO_PORT, LED0_GPIO_PIN); \
    } while (0) /* LED0 = !LED0 */
#define LED1_TOGGLE()                                      \
    do                                                     \
    {                                                      \
        HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_GPIO_PIN); \
    } while (0) /* LED1 = !LED1 */

// ǰ��
#define LEFT_FRONT_TURE(x)                                                                                                                                                                 \
    do                                                                                                                                                                                     \
    {                                                                                                                                                                                      \
        x ? HAL_GPIO_WritePin(LEFT_FRONT_TURE_GPIO_PORT, LEFT_FRONT_TURE_GPIO_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(LEFT_FRONT_TURE_GPIO_PORT, LEFT_FRONT_TURE_GPIO_PIN, GPIO_PIN_RESET); \
    } while (0) /* ????? */

#define RIGHT_FRONT_TURE(x)                                                                                                                                                                    \
    do                                                                                                                                                                                         \
    {                                                                                                                                                                                          \
        x ? HAL_GPIO_WritePin(RIGHT_FRONT_TURE_GPIO_PORT, RIGHT_FRONT_TURE_GPIO_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(RIGHT_FRONT_TURE_GPIO_PORT, RIGHT_FRONT_TURE_GPIO_PIN, GPIO_PIN_RESET); \
    } while (0)
#define FRONT_MAIN(x)                                                                                                                                                  \
    do                                                                                                                                                                 \
    {                                                                                                                                                                  \
        x ? HAL_GPIO_WritePin(FRONT_MAIN_GPIO_PORT, FRONT_MAIN_GPIO_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(FRONT_MAIN_GPIO_PORT, FRONT_MAIN_GPIO_PIN, GPIO_PIN_RESET); \
    } while (0)

#define LEFT_FRONT_TURE_TOGGLE()                                                 \
    do                                                                           \
    {                                                                            \
        HAL_GPIO_TogglePin(LEFT_FRONT_TURE_GPIO_PORT, LEFT_FRONT_TURE_GPIO_PIN); \
    } while (0) /*????? ?? */
#define RIGHT_FRONT_TURE_TOGGLE()                                                  \
    do                                                                             \
    {                                                                              \
        HAL_GPIO_TogglePin(RIGHT_FRONT_TURE_GPIO_PORT, RIGHT_FRONT_TURE_GPIO_PIN); \
    } while (0) /*????? ?? */

// ���

#define LEFT_BACK_TURE(x)                                                                                                                                                              \
    do                                                                                                                                                                                 \
    {                                                                                                                                                                                  \
        x ? HAL_GPIO_WritePin(LEFT_BACK_TURE_GPIO_PORT, LEFT_BACK_TURE_GPIO_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(LEFT_BACK_TURE_GPIO_PORT, LEFT_BACK_TURE_GPIO_PIN, GPIO_PIN_RESET); \
    } while (0) /* ????? */

#define RIGHT_BACK_TURE(x)                                                                                                                                                                 \
    do                                                                                                                                                                                     \
    {                                                                                                                                                                                      \
        x ? HAL_GPIO_WritePin(RIGHT_BACK_TURE_GPIO_PORT, RIGHT_BACK_TURE_GPIO_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(RIGHT_BACK_TURE_GPIO_PORT, RIGHT_BACK_TURE_GPIO_PIN, GPIO_PIN_RESET); \
    } while (0)
#define BACK_MAIN(x)                                                                                                                                               \
    do                                                                                                                                                             \
    {                                                                                                                                                              \
        x ? HAL_GPIO_WritePin(BACK_MAIN_GPIO_PORT, BACK_MAIN_GPIO_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(BACK_MAIN_GPIO_PORT, BACK_MAIN_GPIO_PIN, GPIO_PIN_RESET); \
    } while (0)

// #define LEFT_FRONT_TURE_TOGGLE()    do{ HAL_GPIO_TogglePin(LEFT_FRONT_TURE_GPIO_PORT, LEFT_FRONT_TURE_GPIO_PIN); }while(0)       /*????? ?? */
// #define RIGHT_FRONT_TURE_TOGGLE()    do{ HAL_GPIO_TogglePin(RIGHT_FRONT_TURE_GPIO_PORT, RIGHT_FRONT_TURE_GPIO_PIN); }while(0)      /*????? ?? */

/******************************************************************************************/

typedef enum
{
    None,
    idle_state,
    open_leftbling,
    open_rightbling,
    open_doublebling,
    open_mainbulb,

    close_leftbling,
    close_rightbling,
    close_doublebling,
    close_mainbulb
} Led_State;

/* �ⲿ�ӿں���*/
void led_init(void);
void led_bling(void); /* ��ʼ�� */

#endif /*��ת��ƿ��ￄ1�7*/
