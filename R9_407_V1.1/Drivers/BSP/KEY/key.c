/**
 ****************************************************************************************************
 * @file        key.c
 * @author      Lisir
 * @version     V1.0
 * @date        2021-10-14
 * @brief       按键输入 驱动代码
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
   KEY1: 姿态 1
   KEY2: 左转向
   KEY3: 右转向
   KEY4: 调速增
   KEY5: 姿态2
   KEY6: 双闪
   KEY7: 喇叭 
   KEY8: 调速减
   KEY9: 360V iew
   KEY10: 主灯
   KEY11 :调试预留
 *
 ****************************************************************************************************
 */

#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"


/**
 * @brief       按键初始化函数
 * @param       无
 * @retval      无
 */
void key_init(void)
{
    GPIO_InitTypeDef gpioE_init_struct;                          /* GPIO配置参数存储变量 */
    GPIO_InitTypeDef gpioB_init_struct;                          /* GPIO配置参数存储变量 */
    GPIO_InitTypeDef gpioG_init_struct;                          /* GPIO配置参数存储变量 */


    gpioE_init_struct.Pin = KEY1_GPIO_PIN;                       /* KEY1引脚 */
    gpioE_init_struct.Mode = GPIO_MODE_INPUT;                    /* 输入 */
    gpioE_init_struct.Pull = GPIO_NOPULL;                        /* 上拉 */
    gpioE_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* 高速 */
    HAL_GPIO_Init(KEY1_GPIO_PORT, &gpioE_init_struct);           /* KEY1引脚模式设置,上拉输入 */

    gpioE_init_struct.Pin = KEY2_GPIO_PIN;                       /* KEY2引脚 */
    gpioE_init_struct.Mode = GPIO_MODE_INPUT;                    /* 输入 */
    gpioE_init_struct.Pull = GPIO_NOPULL;                        /* 上拉 */
    gpioE_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* 高速 */
    HAL_GPIO_Init(KEY2_GPIO_PORT, &gpioE_init_struct);           /* KEY2引脚模式设置,上拉输入 */

    gpioE_init_struct.Pin = KEY3_GPIO_PIN;                       /* KEY3引脚 */
    gpioE_init_struct.Mode = GPIO_MODE_INPUT;                    /* 输入 */
    gpioE_init_struct.Pull = GPIO_NOPULL;                        /* 上拉 */
    gpioE_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* 高速 */
    HAL_GPIO_Init(KEY3_GPIO_PORT, &gpioE_init_struct);           /* KEY3引脚模式设置,上拉输入 */

    gpioE_init_struct.Pin = KEY4_GPIO_PIN;                       /* KEY4引脚 */
    gpioE_init_struct.Mode = GPIO_MODE_INPUT;                    /* 输入 */
    gpioE_init_struct.Pull = GPIO_NOPULL;                        /* 上拉 */
    gpioE_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* 高速 */
    HAL_GPIO_Init(KEY4_GPIO_PORT, &gpioE_init_struct);           /* KEY4引脚模式设置,上拉输入 */

    gpioE_init_struct.Pin = KEY5_GPIO_PIN;                       /* KEY5引脚 */
    gpioE_init_struct.Mode = GPIO_MODE_INPUT;                    /* 输入 */
    gpioE_init_struct.Pull = GPIO_NOPULL;                        /* 上拉 */
    gpioE_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* 高速 */
    HAL_GPIO_Init(KEY5_GPIO_PORT, &gpioE_init_struct);           /* KEY5引脚模式设置,上拉输入 */

    gpioG_init_struct.Pin = KEY6_GPIO_PIN;                       /* KEY6引脚 */
    gpioG_init_struct.Mode = GPIO_MODE_INPUT;                    /* 输入 */
    gpioG_init_struct.Pull = GPIO_NOPULL;                        /* 上拉 */
    gpioG_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* 高速 */
    HAL_GPIO_Init(KEY6_GPIO_PORT, &gpioG_init_struct);           /* KEY6引脚模式设置,上拉输入 */

    gpioG_init_struct.Pin = KEY7_GPIO_PIN;                       /* KEY7引脚 */
    gpioG_init_struct.Mode = GPIO_MODE_INPUT;                    /* 输入 */
    gpioG_init_struct.Pull = GPIO_NOPULL;                        /* 上拉 */
    gpioG_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* 高速 */
    HAL_GPIO_Init(KEY7_GPIO_PORT, &gpioG_init_struct);           /* KEY7引脚模式设置,上拉输入 */

    gpioE_init_struct.Pin = KEY8_GPIO_PIN;                       /* KEY8引脚 */
    gpioE_init_struct.Mode = GPIO_MODE_INPUT;                    /* 输入 */
    gpioE_init_struct.Pull = GPIO_NOPULL;                        /* 上拉 */
    gpioE_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* 高速 */
    HAL_GPIO_Init(KEY8_GPIO_PORT, &gpioE_init_struct);           /* KEY8引脚模式设置,上拉输入 */

    gpioB_init_struct.Pin = KEY9_GPIO_PIN;                       /* KEY9引脚 */
    gpioB_init_struct.Mode = GPIO_MODE_INPUT;                    /* 输入 */
    gpioB_init_struct.Pull = GPIO_NOPULL;                        /* 上拉 */
    gpioB_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* 高速 */
    HAL_GPIO_Init(KEY9_GPIO_PORT, &gpioB_init_struct);           /* KEY9引脚模式设置,上拉输入 */

    gpioB_init_struct.Pin = KEY10_GPIO_PIN;                       /* KEY10引脚 */
    gpioB_init_struct.Mode = GPIO_MODE_INPUT;                    /* 输入 */
    gpioB_init_struct.Pull = GPIO_NOPULL;                        /* 上拉 */
    gpioB_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* 高速 */
    HAL_GPIO_Init(KEY10_GPIO_PORT, &gpioB_init_struct);           /* KEY10引脚模式设置,上拉输入 */

 

    gpioE_init_struct.Pin = KEY11_GPIO_PIN;                       /* KEY11引脚 */
    gpioE_init_struct.Mode = GPIO_MODE_INPUT;                    /* 输入 */
    gpioE_init_struct.Pull = GPIO_NOPULL;                        /* 上拉 */
    gpioE_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* 高速 */
    HAL_GPIO_Init(KEY11_GPIO_PORT, &gpioE_init_struct);           /* KEY11引脚模式设置,上拉输入 */        
}

/**
 * @brief       按键扫描函数
 * @note        该函数有响应优先级(同时按下多个按键): WK_UP > KEY2 > KEY1 > KEY0!!
 * @param       mode:0 / 1, 具体含义如下:
 *   @arg       0,  不支持连续按(当按键按下不放时, 只有第一次调用会返回键值,
 *                  必须松开以后, 再次按下才会返回其他键值)
 *   @arg       1,  支持连续按(当按键按下不放时, 每次调用该函数都会返回键值)
 * @retval      键值, 定义如下:
 *              KEY0_PRES, 1, KEY0按下
 *              KEY1_PRES, 2, KEY1按下
 *              KEY2_PRES, 3, KEY2按下
 *              WKUP_PRES, 4, WKUP按下
 */


uint8_t key_scan1(void)
{
    if(KEY1 == 0)
    {
        delay_ms(20);   /* 消抖 */
        if(KEY1== 0)
        {
           // while(KEY_T1PUSH == 0);  /* 等待按键松开 */
            return 1;   /* 按键按下了 */
        }
    }
    else
    { 
        return 0;   /* 按键没有按下 */
        }
   
}


uint8_t key_scan2(void)
{
    if(KEY2 == 0)
    {
        delay_ms(20);   /* 消抖 */
        if(KEY2== 0)
        {
            while(KEY2 == 0);  /* 等待按键松开 */
            return 1;   /* 按键按下了 */
        }
    }
    else
    { 
        return 0;   /* 按键没有按下 */
        }
   
}

uint8_t key_scan3(void)
{
    if(KEY3 == 0)
    {
        delay_ms(20);   /* 消抖 */
        if(KEY3== 0)
        {
            while(KEY3 == 0);  /* 等待按键松开 */
            return 1;   /* 按键按下了 */
        }
    }
    else
    { 
        return 0;   /* 按键没有按下 */
        }
   
}


uint8_t key_scan4(void)
{
    if(KEY4 == 0)
    {
        delay_ms(20);   /* 消抖 */
        if(KEY4== 0)
        {
            while(KEY4 == 0);  /* 等待按键松开 */
            return 1;   /* 按键按下了 */
        }
    }
    else
    { 
        return 0;   /* 按键没有按下 */
        }
   
}

uint8_t key_scan5(void)
{
    if(KEY5 == 0)
    {
        delay_ms(20);   /* 消抖 */
        if(KEY5== 0)
        {
            //while(KEY_T1PUSH == 0);  /* 等待按键松开 */
            return 1;   /* 按键按下了 */
        }
    }
    else
    { 
        return 0;   /* 按键没有按下 */
        }
   
}

uint8_t key_scan6(void)
{
    if(KEY6 == 0)
    {
        delay_ms(20);   /* 消抖 */
        if(KEY6== 0)
        {
            while(KEY6 == 0);  /* 等待按键松开 */
            return 1;   /* 按键按下了 */
        }
    }
    else
    { 
        return 0;   /* 按键没有按下 */
        }
   
}

uint8_t key_scan7(void)
{
    if(KEY7 == 0)
    {
        delay_ms(20);   /* 消抖 */
        if(KEY7== 0)
        {
            //while(KEY7 == 0);  /* 等待按键松开 */
            return 1;   /* 按键按下了 */
        }
    }
    else
    { 
        return 0;   /* 按键没有按下 */
        }
   
}

uint8_t key_scan8(void)
{
    if(KEY8 == 0)
    {
        delay_ms(20);   /* 消抖 */
        if(KEY8== 0)
        {
            while(KEY8 == 0);  /* 等待按键松开 */
            return 1;   /* 按键按下了 */
        }
    }
    else
    { 
        return 0;   /* 按键没有按下 */
        }
   
}

uint8_t key_scan9(void)
{
    if(KEY9 == 0)
    {
        delay_ms(20);   /* 消抖 */
        if(KEY9== 0)
        {
            while(KEY9 == 0);  /* 等待按键松开 */
            return 1;   /* 按键按下了 */
        }
    }
    else
    { 
        return 0;   /* 按键没有按下 */
        }
   
}

uint8_t key_scan10(void)
{
    if(KEY10 == 0)
    {
        delay_ms(20);   /* 消抖 */
        if(KEY10== 0)
        {
            //while(KEY10 == 0);  /* 等待按键松开 */
            return 1;   /* 按键按下了 */
        }
    }
    else
    { 
        return 0;   /* 按键没有按下 */
    }
   
}

uint8_t key_scan11(void)
{
    if(KEY11 == 0)
    {
        delay_ms(20);   /* 消抖 */
        if(KEY11== 0)
        {
            while(KEY11 == 0);  /* 等待按键松开 */
            return 1;   /* 按键按下了 */
        }
    }
    else
    { 
        return 0;   /* 按键没有按下 */
        }
   
}