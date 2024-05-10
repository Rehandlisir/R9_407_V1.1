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
	KEY1 ~ KEY5  左/右转向  360View  喇叭 
 
 ****************************************************************************************************
 */

#ifndef __KEY_H
#define __KEY_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/


/* 按键引脚 定义 */

// KEY1 - PE15 
#define KEY1_GPIO_PORT                  GPIOE
#define KEY1_GPIO_PIN                   GPIO_PIN_15
#define KEY1_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)  
// KEY2-  PE14 
#define KEY2_GPIO_PORT                  GPIOE
#define KEY2_GPIO_PIN                   GPIO_PIN_14
#define KEY2_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   
// KEY3-  PE13  
#define KEY3_GPIO_PORT                  GPIOE
#define KEY3_GPIO_PIN                   GPIO_PIN_13
#define KEY3_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   
// KEY4 -PE12 
#define KEY4_GPIO_PORT                  GPIOE
#define KEY4_GPIO_PIN                   GPIO_PIN_12
#define KEY4_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   
//  KEY5-PE11: 
#define KEY5_GPIO_PORT                  GPIOE
#define KEY5_GPIO_PIN                   GPIO_PIN_11
#define KEY5_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   
////  KEY6-PG0: 双闪
//#define KEY6_GPIO_PORT                  GPIOG
//#define KEY6_GPIO_PIN                   GPIO_PIN_0
//#define KEY6_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)  
//// KEY7-PG1: 喇叭 
//#define KEY7_GPIO_PORT                  GPIOG
//#define KEY7_GPIO_PIN                   GPIO_PIN_1
//#define KEY7_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)   
////  KEY8-PE7: 调速减
//#define KEY8_GPIO_PORT                  GPIOE
//#define KEY8_GPIO_PIN                   GPIO_PIN_7
//#define KEY8_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   
//// KEY9-PB10: 360V iew
//#define KEY9_GPIO_PORT                  GPIOB
//#define KEY9_GPIO_PIN                   GPIO_PIN_10
//#define KEY9_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   
//// KEY10-PB11: 主灯
//#define KEY10_GPIO_PORT                  GPIOB
//#define KEY10_GPIO_PIN                   GPIO_PIN_11
//#define KEY10_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   

//// 调试预留-PE8
//#define KEY11_GPIO_PORT                  GPIOE
//#define KEY11_GPIO_PIN                   GPIO_PIN_8
//#define KEY11_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   

/******************************************************************************************/


#define KEY1         HAL_GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN)    
#define KEY2         HAL_GPIO_ReadPin(KEY2_GPIO_PORT, KEY2_GPIO_PIN)    
#define KEY3         HAL_GPIO_ReadPin(KEY3_GPIO_PORT, KEY3_GPIO_PIN)    
#define KEY4         HAL_GPIO_ReadPin(KEY4_GPIO_PORT, KEY4_GPIO_PIN)    
#define KEY5         HAL_GPIO_ReadPin(KEY5_GPIO_PORT, KEY5_GPIO_PIN)    
//#define KEY6         HAL_GPIO_ReadPin(KEY6_GPIO_PORT, KEY6_GPIO_PIN)    
//#define KEY7         HAL_GPIO_ReadPin(KEY7_GPIO_PORT, KEY7_GPIO_PIN)    
//#define KEY8         HAL_GPIO_ReadPin(KEY8_GPIO_PORT, KEY8_GPIO_PIN)    
//#define KEY9         HAL_GPIO_ReadPin(KEY9_GPIO_PORT, KEY9_GPIO_PIN)    
//#define KEY10        HAL_GPIO_ReadPin(KEY10_GPIO_PORT, KEY10_GPIO_PIN)    
//#define KEY11        HAL_GPIO_ReadPin(KEY11_GPIO_PORT, KEY11_GPIO_PIN)    


void key_init(void);                /* 按键初始化函数 */
uint8_t key_scan1(void);// KEY1 - PE15  
uint8_t key_scan2(void);// KEY2-  PE14 
uint8_t key_scan3(void);// KEY3-  PE13  
uint8_t key_scan4(void);// KEY4 -PE12 
uint8_t key_scan5(void);// KEY5-PE11: 
//uint8_t key_scan6(void);//  KEY6-PG0: 双闪
//uint8_t key_scan7(void);// KEY7-PG1: 喇叭 
//uint8_t key_scan8(void);//  KEY8-PE7: 调速减
//uint8_t key_scan9(void);// KEY9-PB10: 360V iew
//uint8_t key_scan10(void);// KEY10-PB11: 主灯
//uint8_t key_scan11(void);// 调试预留-PE8
#endif


















