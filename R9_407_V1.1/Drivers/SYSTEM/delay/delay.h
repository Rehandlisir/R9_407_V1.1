/**
 ****************************************************************************************************
 * @file        delay.h
 * @author      Lisir
 * @version     V1.0
 * @date        2021-10-14
 * @brief       使用SysTick的普通计数模式对延迟进行管理(支持ucosii)
 *              提供delay_init初始化函数， delay_us和delay_ms等延时函数
 * @license     Copyright (c) 2024, 深圳复成医疗科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台：F407开发板
 * 在线视频：none
 * 技术论坛：none
 * 公司网址：none
 * 购买地址：none
 *
 * 修改说明
 * none
 * 第一次发布
 *
 ****************************************************************************************************
 */
 
#ifndef __DELAY_H
#define __DELAY_H

#include "./SYSTEM/sys/sys.h"


void delay_init(uint16_t sysclk);           /* 初始化延迟函数 */
void delay_ms(uint16_t nms);                /* 延时nms */
void delay_us(uint32_t nus);                /* 延时nus */

#if (!SYS_SUPPORT_OS)                       /* 没有使用Systick中断 */
    void HAL_Delay(uint32_t Delay);         /* HAL库的延时函数，SDIO等需要用到 */
#endif

#endif

