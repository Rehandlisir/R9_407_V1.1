// /**
//  ****************************************************************************************************
//  * @file        exti.h
//  * @author      Lisir
//  * @version     V1.0
//  * @date        2021-10-14
//  * @brief       外部中断 驱动代码
//  * @license     Copyright (c) 2024, 深圳复成医疗科技有限公司
//  ****************************************************************************************************
//  * @attention
//  *
//  * 实验平台:F407开发板
//  * none
//  * none
//  * none
//  * none
//  *
//  * 修改说明
//  * none
//  * 第一次发布
//  *
//  ****************************************************************************************************
//  */

// #ifndef __EXTI_H
// #define __EXTI_H

// #include "./SYSTEM/sys/sys.h"


// /******************************************************************************************/
// /* 引脚 和 中断编号 & 中断服务函数 定义 */ 

// #define KEY0_INT_GPIO_PORT              GPIOE
// #define KEY0_INT_GPIO_PIN               GPIO_PIN_4
// #define KEY0_INT_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   /* PE口时钟使能 */
// #define KEY0_INT_IRQn                   EXTI4_IRQn
// #define KEY0_INT_IRQHandler             EXTI4_IRQHandler

// #define KEY1_INT_GPIO_PORT              GPIOE
// #define KEY1_INT_GPIO_PIN               GPIO_PIN_3
// #define KEY1_INT_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   /* PE口时钟使能 */
// #define KEY1_INT_IRQn                   EXTI3_IRQn
// #define KEY1_INT_IRQHandler             EXTI3_IRQHandler

// #define KEY2_INT_GPIO_PORT              GPIOE
// #define KEY2_INT_GPIO_PIN               GPIO_PIN_2
// #define KEY2_INT_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   /* PE口时钟使能 */
// #define KEY2_INT_IRQn                   EXTI2_IRQn
// #define KEY2_INT_IRQHandler             EXTI2_IRQHandler

// #define WKUP_INT_GPIO_PORT              GPIOA
// #define WKUP_INT_GPIO_PIN               GPIO_PIN_0
// #define WKUP_INT_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */
// #define WKUP_INT_IRQn                   EXTI0_IRQn
// #define WKUP_INT_IRQHandler             EXTI0_IRQHandler

// /******************************************************************************************/


// void extix_init(void);  /* 外部中断初始化 */

// #endif

























