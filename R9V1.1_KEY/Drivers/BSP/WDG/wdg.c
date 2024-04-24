/**
 ****************************************************************************************************
 * @file        wdg.c
 * @author      
 * @version     V1.0
 * @date       
 * @brief       看门狗 驱动代码
 * @license    
 ****************************************************************************************************
 * @attention
 *

 *
 ****************************************************************************************************
 */

#include "./BSP/WDG/wdg.h"


IWDG_HandleTypeDef iwdg_handler; /*独立看门狗句柄 */

/**
 * @brief       初始化独立看门狗 
 * @param       prer: IWDG_PRESCALER_4~IWDG_PRESCALER_256,对应4~256分频
 *   @arg       分频因子 = 4 * 2^prer. 但最大值只能是256!
 * @param       rlr: 自动重装载值,0~0XFFF. 
 * @note        时间计算(大概):Tout=((4 * 2^prer) * rlr) / 32 (ms). 
 * @retval      无
 */
void iwdg_init(uint32_t prer, uint16_t rlr)
{
    iwdg_handler.Instance = IWDG;
    iwdg_handler.Init.Prescaler = prer; /* 设置IWDG分频系数 */
    iwdg_handler.Init.Reload = rlr;     /* 从加载寄存器 IWDG->RLR 重装载值 */
    HAL_IWDG_Init(&iwdg_handler);       /* 初始化IWDG并使能 */
}

/**
 * @brief       喂独立看门狗
 * @param       无
 * @retval      无
 */
void iwdg_feed(void)
{
    HAL_IWDG_Refresh(&iwdg_handler);    /* 喂狗 */
}
