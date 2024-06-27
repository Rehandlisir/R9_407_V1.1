/**
 ****************************************************************************************************
 * @file        brake.c
 * @author      Lisir
 * @version     V1.0
 * @date        2021-10-14
 * @brief       抱闸 驱动代码
 * @license     Copyright (c) 2024, 深圳复成医疗科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:F407开发板
 * none
 * none
 * none
 * none
 *
 * 修改说明
 * none
 * 第一次发布  
 *
 ****************************************************************************************************
 */
#include "./BSP/R9/brake.h"

#include "./SYSTEM/delay/delay.h"

/**
 * @brief       初始化BRAKE相关IO口, 并使能时钟
 * @param       无
 * @retval      无

 */
void brake_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    BRAKE1_GPIO_CLK_ENABLE();                                 /* 抱闸1时钟使能 */
    BRAKE2_GPIO_CLK_ENABLE();                                 /* 抱闸2时钟使能 */

    gpio_init_struct.Pin = BRAKE1_GPIO_PIN;                   /* 抱闸1引脚 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(BRAKE1_GPIO_PORT, &gpio_init_struct);       /* 初始化抱闸1引脚 */

    gpio_init_struct.Pin = BRAKE2_GPIO_PIN;                   /* 抱闸2引脚 */
    HAL_GPIO_Init(BRAKE2_GPIO_PORT, &gpio_init_struct);       /* 初始化抱闸2引脚 */
    
    BRAKE1(1);                                                /* 关闭 抱闸1 */
    BRAKE2(1);                                                /* 关闭 抱闸1 */
 
}


