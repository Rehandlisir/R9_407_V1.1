/**
 ****************************************************************************************************
 * @file        wdg.h
 * @author      R9
 * @version     V1.0
 * @date        2021-10-14
 * @brief       看门狗 驱动代码
 * @license     R9
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 探索者 F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20211014
 * 第一次发布
 *
 ****************************************************************************************************
 */

#ifndef __WDG_H
#define __WDG_H

#include "./SYSTEM/sys/sys.h"


void iwdg_init(uint32_t prer, uint16_t rlr);        /* 初始化IWDG，并使能IWDG */
void iwdg_feed(void);                               /* 喂狗 */

#endif




























