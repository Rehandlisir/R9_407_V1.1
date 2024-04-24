/**
 ****************************************************************************************************
 * @file        led.c
 * @author      Lisir
 * @version     V1.0
 * @date        2021-10-14
 * @brief       LED指示灯
 * @license     Copyright (c) 2024
 ****************************************************************************************************
 * @attention

 *
 ****************************************************************************************************
 */
#ifndef __LED_H
#define __LED_H

#include "./SYSTEM/sys/sys.h"

/******************************************************************************************/
/*  */

#define LED1G_GPIO_PORT GPIOE
#define LED1G_GPIO_PIN GPIO_PIN_1
#define LED1G_GPIO_CLK_ENABLE()        \
    do                                \
    {                                 \
        __HAL_RCC_GPIOE_CLK_ENABLE(); \
    } while (0) /*  */

	
#define LED1R_GPIO_PORT GPIOE
#define LED1R_GPIO_PIN GPIO_PIN_0
#define LED1R_GPIO_CLK_ENABLE()        \
    do                                \
    {                                 \
        __HAL_RCC_GPIOE_CLK_ENABLE(); \
    } while (0) /*  */


#define LED1B_GPIO_PORT GPIOD
#define LED1B_GPIO_PIN GPIO_PIN_2
#define LED1B_GPIO_CLK_ENABLE()        \
    do                                \
    {                                 \
        __HAL_RCC_GPIOD_CLK_ENABLE(); \
    } while (0) /*  */
	
	
#define LED2G_GPIO_PORT GPIOD
#define LED2G_GPIO_PIN GPIO_PIN_4
#define LED2G_GPIO_CLK_ENABLE()        \
    do                                \
    {                                 \
        __HAL_RCC_GPIOD_CLK_ENABLE(); \
    } while (0) /*  */

	
#define LED2R_GPIO_PORT GPIOD
#define LED2R_GPIO_PIN GPIO_PIN_5
#define LED2R_GPIO_CLK_ENABLE()        \
    do                                \
    {                                 \
        __HAL_RCC_GPIOD_CLK_ENABLE(); \
    } while (0) /*  */


#define LED2B_GPIO_PORT GPIOD
#define LED2B_GPIO_PIN GPIO_PIN_6
#define LED2B_GPIO_CLK_ENABLE()        \
    do                                \
    {                                 \
        __HAL_RCC_GPIOD_CLK_ENABLE(); \
    } while (0) /*  */	
	
	
#define LED3G_GPIO_PORT GPIOA
#define LED3G_GPIO_PIN GPIO_PIN_11
#define LED3G_GPIO_CLK_ENABLE()        \
    do                                \
    {                                 \
        __HAL_RCC_GPIOA_CLK_ENABLE(); \
    } while (0) /*  */

	
#define LED3R_GPIO_PORT GPIOA
#define LED3R_GPIO_PIN GPIO_PIN_12
#define LED3R_GPIO_CLK_ENABLE()        \
    do                                \
    {                                 \
        __HAL_RCC_GPIOA_CLK_ENABLE(); \
    } while (0) /*  */


#define LED3B_GPIO_PORT GPIOA
#define LED3B_GPIO_PIN GPIO_PIN_10
#define LED3B_GPIO_CLK_ENABLE()        \
    do                                \
    {                                 \
        __HAL_RCC_GPIOA_CLK_ENABLE(); \
    } while (0) /*  */	

#define LED4G_GPIO_PORT GPIOD
#define LED4G_GPIO_PIN GPIO_PIN_12
#define LED4G_GPIO_CLK_ENABLE()        \
    do                                \
    {                                 \
        __HAL_RCC_GPIOD_CLK_ENABLE(); \
    } while (0) /*  */

	
#define LED4R_GPIO_PORT GPIOD
#define LED4R_GPIO_PIN GPIO_PIN_13
#define LED4R_GPIO_CLK_ENABLE()        \
    do                                \
    {                                 \
        __HAL_RCC_GPIOD_CLK_ENABLE(); \
    } while (0) /*  */


#define LED4B_GPIO_PORT GPIOD
#define LED4B_GPIO_PIN GPIO_PIN_14
#define LED4B_GPIO_CLK_ENABLE()        \
    do                                \
    {                                 \
        __HAL_RCC_GPIOD_CLK_ENABLE(); \
    } while (0) /*  */	
	
	
#define LED5G_GPIO_PORT GPIOA
#define LED5G_GPIO_PIN GPIO_PIN_1
#define LED5G_GPIO_CLK_ENABLE()        \
    do                                \
    {                                 \
        __HAL_RCC_GPIOA_CLK_ENABLE(); \
    } while (0) /*  */

	
#define LED5R_GPIO_PORT GPIOA
#define LED5R_GPIO_PIN GPIO_PIN_2
#define LED5R_GPIO_CLK_ENABLE()        \
    do                                \
    {                                 \
        __HAL_RCC_GPIOA_CLK_ENABLE(); \
    } while (0) /*  */


#define LED5B_GPIO_PORT GPIOA
#define LED5B_GPIO_PIN GPIO_PIN_3
#define LED5B_GPIO_CLK_ENABLE()        \
    do                                \
    {                                 \
        __HAL_RCC_GPIOA_CLK_ENABLE(); \
    } while (0) /*  */	
	
	
	
	
/******************************************************************************************/

	/* LED 参数为1 点亮 反之熄灭*/
#define LED1G(x)                                                                                                                                \
    do                                                                                                                                         \
    {                                                                                                                                          \
        x ? HAL_GPIO_WritePin(LED1G_GPIO_PORT, LED1G_GPIO_PIN, GPIO_PIN_RESET) : HAL_GPIO_WritePin(LED1G_GPIO_PORT, LED1G_GPIO_PIN, GPIO_PIN_SET); \
    } while (0) /* 正常 */

#define LED1R(x)                                                                                                                                \
    do                                                                                                                                         \
    {                                                                                                                                          \
        x ? HAL_GPIO_WritePin(LED1R_GPIO_PORT, LED1R_GPIO_PIN, GPIO_PIN_RESET) : HAL_GPIO_WritePin(LED1R_GPIO_PORT, LED1R_GPIO_PIN, GPIO_PIN_SET); \
    } while (0) /*报 错*/

	
#define LED1B(x)                                                                                                                                \
     do                                                                                                                                         \
    {                                                                                                                                          \
        x ? HAL_GPIO_WritePin(LED1B_GPIO_PORT, LED1B_GPIO_PIN, GPIO_PIN_RESET) : HAL_GPIO_WritePin(LED1B_GPIO_PORT, LED1B_GPIO_PIN, GPIO_PIN_SET) ; \
		} while (0) /* 警示*/

	
#define LED2G(x)                                                                                                                                \
    do                                                                                                                                         \
    {                                                                                                                                          \
        x ? HAL_GPIO_WritePin(LED2G_GPIO_PORT, LED2G_GPIO_PIN, GPIO_PIN_RESET) :HAL_GPIO_WritePin(LED2G_GPIO_PORT, LED2G_GPIO_PIN, GPIO_PIN_SET); \
    } while (0) /* 正常 */

#define LED2R(x)                                                                                                                                \
    do                                                                                                                                         \
    {                                                                                                                                          \
        x ? HAL_GPIO_WritePin(LED2R_GPIO_PORT, LED2R_GPIO_PIN, GPIO_PIN_RESET) : HAL_GPIO_WritePin(LED2R_GPIO_PORT, LED2R_GPIO_PIN, GPIO_PIN_SET); \
    } while (0) /*报 错*/

	
#define LED2B(x)                                                                                                                                \
     do                                                                                                                                         \
    {                                                                                                                                          \
        x ? HAL_GPIO_WritePin(LED2B_GPIO_PORT, LED2B_GPIO_PIN, GPIO_PIN_RESET) : HAL_GPIO_WritePin(LED2B_GPIO_PORT, LED2B_GPIO_PIN, GPIO_PIN_SET); \
		} while (0) /* 警示*/	
	
	
#define LED3G(x)                                                                                                                                \
    do                                                                                                                                         \
    {                                                                                                                                          \
        x ? HAL_GPIO_WritePin(LED3G_GPIO_PORT, LED3G_GPIO_PIN, GPIO_PIN_RESET) : HAL_GPIO_WritePin(LED3G_GPIO_PORT, LED3G_GPIO_PIN, GPIO_PIN_SET); \
    } while (0) /* 正常 */

#define LED3R(x)                                                                                                                                \
    do                                                                                                                                         \
    {                                                                                                                                          \
        x ? HAL_GPIO_WritePin(LED3R_GPIO_PORT, LED3R_GPIO_PIN, GPIO_PIN_RESET) : HAL_GPIO_WritePin(LED3R_GPIO_PORT, LED3R_GPIO_PIN, GPIO_PIN_SET); \
    } while (0) /*报 错*/

	
#define LED3B(x)                                                                                                                                \
     do                                                                                                                                         \
    {                                                                                                                                          \
        x ? HAL_GPIO_WritePin(LED3B_GPIO_PORT, LED3B_GPIO_PIN, GPIO_PIN_RESET) : HAL_GPIO_WritePin(LED3B_GPIO_PORT, LED3B_GPIO_PIN, GPIO_PIN_SET); \
		} while (0) /* 警示*/

#define LED4G(x)                                                                                                                                \
    do                                                                                                                                         \
    {                                                                                                                                          \
        x ? HAL_GPIO_WritePin(LED4G_GPIO_PORT, LED4G_GPIO_PIN, GPIO_PIN_RESET) : HAL_GPIO_WritePin(LED4G_GPIO_PORT, LED4G_GPIO_PIN, GPIO_PIN_SET); \
    } while (0) /* 正常 */

#define LED4R(x)                                                                                                                                \
    do                                                                                                                                         \
    {                                                                                                                                          \
        x ? HAL_GPIO_WritePin(LED4R_GPIO_PORT, LED4R_GPIO_PIN, GPIO_PIN_RESET) : HAL_GPIO_WritePin(LED4R_GPIO_PORT, LED4R_GPIO_PIN, GPIO_PIN_SET); \
    } while (0) /*报 错*/

	
#define LED4B(x)                                                                                                                                \
     do                                                                                                                                         \
    {                                                                                                                                          \
        x ? HAL_GPIO_WritePin(LED4B_GPIO_PORT, LED4B_GPIO_PIN, GPIO_PIN_RESET) : HAL_GPIO_WritePin(LED4B_GPIO_PORT, LED4B_GPIO_PIN, GPIO_PIN_SET) ; \
		} while (0) /* 警示*/	


#define LED5G(x)                                                                                                                                \
    do                                                                                                                                         \
    {                                                                                                                                          \
        x ? HAL_GPIO_WritePin(LED5G_GPIO_PORT, LED5G_GPIO_PIN, GPIO_PIN_RESET) : HAL_GPIO_WritePin(LED5G_GPIO_PORT, LED5G_GPIO_PIN, GPIO_PIN_SET); \
    } while (0) /* 正常 */

#define LED5R(x)                                                                                                                                \
    do                                                                                                                                         \
    {                                                                                                                                          \
        x ? HAL_GPIO_WritePin(LED5R_GPIO_PORT, LED5R_GPIO_PIN, GPIO_PIN_RESET) : HAL_GPIO_WritePin(LED5R_GPIO_PORT, LED5R_GPIO_PIN, GPIO_PIN_SET); \
    } while (0) /*报 错*/

	
#define LED5B(x)                                                                                                                                \
     do                                                                                                                                         \
    {                                                                                                                                          \
        x ? HAL_GPIO_WritePin(LED5B_GPIO_PORT, LED5B_GPIO_PIN, GPIO_PIN_RESET) : HAL_GPIO_WritePin(LED5B_GPIO_PORT, LED5B_GPIO_PIN, GPIO_PIN_SET); \
		} while (0) /* 警示*/	


/******************************************************************************************/

void Led_Init(void);
void LedControl(void);

#endif 
