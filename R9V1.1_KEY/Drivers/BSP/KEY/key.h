/**
 ****************************************************************************************************
 * @file        key.h
 * @author      Lisir
 * @version     V1.0
 * @date        2021-10-14
 * @brief       按键输入 驱动代码
 * @license     Copyright (c) 2024, 深圳复成医疗科技有限公司
 ****************************************************************************************************
 * @attention
	KEY1A - PA0 站姿
	KEY1B-  PB1 坐姿
	KEY2A-  PD0 垂直举升
	KE2B -PD1 垂直下降
	KE3A-PC0:靠背前倾
	KE3B-PC1: 靠背后倾
	KEY4A-PA7: 整体前倾 
	KEY4B-PA6: 整体后倾
	KEY5A-PC4:腿托上旋
	KEY5B-PC5: 腿托下旋
 ****************************************************************************************************
 */

#ifndef __KEY_H
#define __KEY_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/


/* 按键引脚 定义 */

// KEY1A - PA0 站姿
#define KEY1_GPIO_PORT                  GPIOA
#define KEY1_GPIO_PIN                   GPIO_PIN_0
#define KEY1_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)  
// KEY1B-  PB1 坐姿
#define KEY2_GPIO_PORT                  GPIOB
#define KEY2_GPIO_PIN                   GPIO_PIN_1
#define KEY2_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   
// KEY2A-  PD0 垂直举升
#define KEY3_GPIO_PORT                  GPIOD
#define KEY3_GPIO_PIN                   GPIO_PIN_0
#define KEY3_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   
// KE2B -PD1 垂直下降
#define KEY4_GPIO_PORT                  GPIOD
#define KEY4_GPIO_PIN                   GPIO_PIN_1
#define KEY4_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   
//  KE3A-PC0:靠背前倾
#define KEY5_GPIO_PORT                  GPIOC
#define KEY5_GPIO_PIN                   GPIO_PIN_0
#define KEY5_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   
//  KE3B-PC1: 靠背后倾
#define KEY6_GPIO_PORT                  GPIOC
#define KEY6_GPIO_PIN                   GPIO_PIN_1
#define KEY6_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)  
// KEY4A-PA7: 整体前倾 
#define KEY7_GPIO_PORT                  GPIOA
#define KEY7_GPIO_PIN                   GPIO_PIN_7
#define KEY7_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   
//  KEY4B-PA6: 整体后倾
#define KEY8_GPIO_PORT                  GPIOA
#define KEY8_GPIO_PIN                   GPIO_PIN_6
#define KEY8_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   
// KEY5A-PC4:腿托上旋
#define KEY9_GPIO_PORT                  GPIOC
#define KEY9_GPIO_PIN                   GPIO_PIN_4
#define KEY9_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   
// KEY5B-PC5: 腿托下旋
#define KEY10_GPIO_PORT                  GPIOC
#define KEY10_GPIO_PIN                   GPIO_PIN_5
#define KEY10_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   


/******************************************************************************************/


#define KEY1         HAL_GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN)    
#define KEY2         HAL_GPIO_ReadPin(KEY2_GPIO_PORT, KEY2_GPIO_PIN)    
#define KEY3         HAL_GPIO_ReadPin(KEY3_GPIO_PORT, KEY3_GPIO_PIN)    
#define KEY4         HAL_GPIO_ReadPin(KEY4_GPIO_PORT, KEY4_GPIO_PIN)    
#define KEY5         HAL_GPIO_ReadPin(KEY5_GPIO_PORT, KEY5_GPIO_PIN)    
#define KEY6         HAL_GPIO_ReadPin(KEY6_GPIO_PORT, KEY6_GPIO_PIN)    
#define KEY7         HAL_GPIO_ReadPin(KEY7_GPIO_PORT, KEY7_GPIO_PIN)    
#define KEY8         HAL_GPIO_ReadPin(KEY8_GPIO_PORT, KEY8_GPIO_PIN)    
#define KEY9         HAL_GPIO_ReadPin(KEY9_GPIO_PORT, KEY9_GPIO_PIN)    
#define KEY10        HAL_GPIO_ReadPin(KEY10_GPIO_PORT, KEY10_GPIO_PIN)    
 


void key_init(void);                /* 按键初始化函数 */
uint8_t key_scan1(void);
uint8_t key_scan2(void);
uint8_t key_scan3(void);
uint8_t key_scan4(void);
uint8_t key_scan5(void);
uint8_t key_scan6(void);
uint8_t key_scan7(void);
uint8_t key_scan8(void);
uint8_t key_scan9(void);
uint8_t key_scan10(void);
void KeyScan(void);
#endif


















