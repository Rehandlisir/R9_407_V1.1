/**
 ****************************************************************************************************
 * @file        moterdriver.h
 * @author      Lisir
 * @version     V1.0
 * @date        2021-10-14
 * @brief       电机 驱动代码
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
#ifndef __MOTERDRIVER_H
#define __MOTERDRIVER_H

#include "./SYSTEM/sys/sys.h"
#include "./BSP/R9/Hostmodbus.h"
#include "./BSP/R9/Slavemodbus.h"
#include "math.h"

/*********************************以下是通用定时器PWM输出实验相关宏定义*************************************/

/* TIMX PWM输出定义
 * 这里输出的PWM控制LED0(RED)的亮度
 * 默认是针对TIM2~TIM5
 * 注意: 通过修改这几个宏定义,可以支持TIM1~TIM8任意一个定时器,任意一个IO口输出PWM
 */
// #define GTIM_TIMX_PWM_CHY_GPIO_PORTA         GPIOA
// #define GTIM_TIMX_PWM_CHY_GPIO_PIN6          GPIO_PIN_6
// #define GTIM_TIMX_PWM_CHY_GPIO_PIN7          GPIO_PIN_7

// #define GTIM_TIMX_PWM_CHY_GPIO_PORTB         GPIOB
// #define GTIM_TIMX_PWM_CHY_GPIO_PIN0          GPIO_PIN_0
// #define GTIM_TIMX_PWM_CHY_GPIO_PIN1         GPIO_PIN_1

// #define GTIM_TIMX_PWM_CHY_GPIOA_CLK_ENABLE() do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)  /* PA口时钟使能 */
// #define GTIM_TIMX_PWM_CHY_GPIOB_CLK_ENABLE() do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)  /* PB口时钟使能 */

// #define GTIM_TIMX_PWM_CHY_GPIO_AF           GPIO_AF2_TIM3                               /* 端口复用到TIM3 */

// /* TIMX REMAP设置
//  * 开启TIM3的重映射功能, 才能将TIM3_CH1 TIM3_CH2 TIM3_CH3 TIM3_CH4 输出到PA6 PA7 PB0 PB1上
//  */

// #define GTIM_TIMX_PWM                       TIM3                                       /* TIM3 */
// #define GTIM_TIMX_PWM_CH1                   TIM_CHANNEL_1                                /* 通道Y,  1<= Y <=4 */
// #define GTIM_TIMX_PWM_CH2                   TIM_CHANNEL_2                                /* 通道Y,  1<= Y <=4 */
// #define GTIM_TIMX_PWM_CH3                   TIM_CHANNEL_3                                /* 通道Y,  1<= Y <=4 */
// #define GTIM_TIMX_PWM_CH4                   TIM_CHANNEL_4                                /* 通道Y,  1<= Y <=4 */

// #define GTIM_TIMX_PWM_CH1_CCRX              TIM3->CCR1                                  /* 通道Y的输出比较寄存器 */
// #define GTIM_TIMX_PWM_CH2_CCRX              TIM3->CCR2                                  /* 通道Y的输出比较寄存器 */
// #define GTIM_TIMX_PWM_CH3_CCRX              TIM3->CCR3                                  /* 通道Y的输出比较寄存器 */
// #define GTIM_TIMX_PWM_CH4_CCRX              TIM3->CCR4                                  /* 通道Y的输出比较寄存器 */

// #define GTIM_TIMX_PWM_CHY_CLK_ENABLE()      do{ __HAL_RCC_TIM3_CLK_ENABLE(); }while(0)  /* TIM3 时钟使能 */

/****************************************************************************************************/

/* // /*********************************底盘电机驱动输出 及 推杆电机 驱动输出  *************************************/

/* 底盘L 电机1 驱动输出 TIME5*/
/* TIMX REMAP设置* 开启TIM5的重映射功能, 将TIM5_CH1 及 TIM5_CH2 输出到PA0 PA1 上*/
#define GTIM_TIM5_PWM_CHY_GPIO_PORTA GPIOA
#define GTIM_TIM5_PWM_CHY_GPIO_PIN0 GPIO_PIN_0
#define GTIM_TIM5_PWM_CHY_GPIO_PIN1 GPIO_PIN_1
#define GTIM_TIM5_PWM TIM5						/* TIM5 */
#define GTIM_TIM5_PWM_CH1 TIM_CHANNEL_1			/* 通道Y,  1<= Y <=4 */
#define GTIM_TIM5_PWM_CH2 TIM_CHANNEL_2			/* 通道Y,  1<= Y <=4 */
#define GTIM_TIM5_PWM_CH1_CCRX TIM5->CCR1		/* 通道1的输出比较寄存器 */
#define GTIM_TIM5_PWM_CH2_CCRX TIM5->CCR2		/* 通道2的输出比较寄存器 */
#define GTIM_TIM5_PWM_CHY_GPIO_AF GPIO_AF2_TIM5 /* 端口复用到TIM5 */
#define GTIM_TIM5_PWM_CHY_GPIOA_CLK_ENABLE() \
	do                                       \
	{                                        \
		__HAL_RCC_GPIOA_CLK_ENABLE();        \
	} while (0) /* PA口时钟使能 */
#define GTIM_TIM5_PWM_CHY_CLK_ENABLE() \
	do                                 \
	{                                  \
		__HAL_RCC_TIM5_CLK_ENABLE();   \
	} while (0) /* TIM5 时钟使能 */

/*底盘R 电机2驱动输出 TIME9 */
/* TIMX REMAP设置* 开启TIM9的重映射功能, 将TIM9_CH1  TIM9_CH2 输出到PE5 PE6 上*/
#define GTIM_TIM9_PWM_CHY_GPIO_PORTE GPIOE
#define GTIM_TIM9_PWM_CHY_GPIO_PIN5 GPIO_PIN_5
#define GTIM_TIM9_PWM_CHY_GPIO_PIN6 GPIO_PIN_6
#define GTIM_TIM9_PWM TIM9						/* TIM9 */
#define GTIM_TIM9_PWM_CH1 TIM_CHANNEL_1			/* 通道Y,  1<= Y <=2 */
#define GTIM_TIM9_PWM_CH2 TIM_CHANNEL_2			/* 通道Y,  1<= Y <=2 */
#define GTIM_TIM9_PWM_CH1_CCRX TIM9->CCR1		/* 通道1的输出比较寄存器 */
#define GTIM_TIM9_PWM_CH2_CCRX TIM9->CCR2		/* 通道2的输出比较寄存器 */
#define GTIM_TIM9_PWM_CHY_GPIO_AF GPIO_AF3_TIM9 /* 端口复用到TIM9 */
#define GTIM_TIM9_PWM_CHY_GPIOE_CLK_ENABLE() \
	do                                       \
	{                                        \
		__HAL_RCC_GPIOE_CLK_ENABLE();        \
	} while (0) /* PE口时钟使能 */
#define GTIM_TIM9_PWM_CHY_CLK_ENABLE() \
	do                                 \
	{                                  \
		__HAL_RCC_TIM9_CLK_ENABLE();   \
	} while (0) /* TIM9 时钟使能 */

/* 靠背推杆电机 1 驱动输出 TIME1  */
/* TIMX REMAP设置* 开启TIM1的重映射功能, 将TIM1_CH1 TIM1_CH2 输出到PA8 PA9 上，*/
/*推杆电机 1 输出*/
#define GTIM_TIM1_PWM_CHY_GPIO_PORTA GPIOA
#define GTIM_TIM1_PWM_CHY_GPIO_PIN8 GPIO_PIN_8
#define GTIM_TIM1_PWM_CHY_GPIO_PIN9 GPIO_PIN_9
#define GTIM_TIM1_PWM TIM1						/* TIM1 */
#define GTIM_TIM1_PWM_CH1 TIM_CHANNEL_1			// T1                          /* 通道Y,  1<= Y <=4 */
#define GTIM_TIM1_PWM_CH2 TIM_CHANNEL_2			/* 通道Y,  1<= Y <=4 */
#define GTIM_TIM1_PWM_CH1_CCRX TIM1->CCR1		/* 通道1的输出比较寄存器 */
#define GTIM_TIM1_PWM_CH2_CCRX TIM1->CCR2		/* 通道2的输出比较寄存器 */
#define GTIM_TIM1_PWM_CHY_GPIO_AF GPIO_AF1_TIM1 /* 端口复用到TIM1 */
#define GTIM_TIM1_PWM_CHY_GPIOA_CLK_ENABLE() \
	do                                       \
	{                                        \
		__HAL_RCC_GPIOA_CLK_ENABLE();        \
	} while (0) /* PA口时钟使能 */
#define GTIM_TIM1_PWM_CHY_CLK_ENABLE() \
	do                                 \
	{                                  \
		__HAL_RCC_TIM1_CLK_ENABLE();   \
	} while (0) /* TIM1 时钟使能 */

/* 座椅举升 及 旋转 推杆电机 2 && 3  驱动输出 TIME8  */
/* TIMX REMAP设置* 开启TIM1的重映射功能, 将TIM8_CH1 TIM8_CH2 TIM8_CH3 TIM8_CH4 输出到PC6 PC7 PC8 PC9 上，*/
/*推杆电机 1 输出*/
#define GTIM_TIM8_PWM_CHY_GPIO_PORTC GPIOC
#define GTIM_TIM8_PWM_CHY_GPIO_PIN6 GPIO_PIN_6 // T3
#define GTIM_TIM8_PWM_CHY_GPIO_PIN7 GPIO_PIN_7
#define GTIM_TIM8_PWM_CHY_GPIO_PIN8 GPIO_PIN_8 // T2
#define GTIM_TIM8_PWM_CHY_GPIO_PIN9 GPIO_PIN_9

#define GTIM_TIM8_PWM TIM8				  /* TIM8 */
#define GTIM_TIM8_PWM_CH1 TIM_CHANNEL_1	  /* 通道Y,  1<= Y <=4 */
#define GTIM_TIM8_PWM_CH2 TIM_CHANNEL_2	  /* 通道Y,  1<= Y <=4 */
#define GTIM_TIM8_PWM_CH3 TIM_CHANNEL_3	  /* 通道Y,  1<= Y <=4 */
#define GTIM_TIM8_PWM_CH4 TIM_CHANNEL_4	  /* 通道Y,  1<= Y <=4 */
#define GTIM_TIM8_PWM_CH1_CCRX TIM8->CCR1 /* 通道1的输出比较寄存器 */
#define GTIM_TIM8_PWM_CH2_CCRX TIM8->CCR2 /* 通道2的输出比较寄存器 */
#define GTIM_TIM8_PWM_CH3_CCRX TIM8->CCR3 /* 通道3的输出比较寄存器 */
#define GTIM_TIM8_PWM_CH4_CCRX TIM8->CCR4 /* 通道4的输出比较寄存器 */

#define GTIM_TIM8_PWM_CHY_GPIO_AF GPIO_AF3_TIM8 /* 端口复用到TIM8 */
#define GTIM_TIM8_PWM_CHY_GPIOC_CLK_ENABLE() \
	do                                       \
	{                                        \
		__HAL_RCC_GPIOC_CLK_ENABLE();        \
	} while (0) /* PC口时钟使能 */
#define GTIM_TIM8_PWM_CHY_CLK_ENABLE() \
	do                                 \
	{                                  \
		__HAL_RCC_TIM8_CLK_ENABLE();   \
	} while (0) /* TIM8 时钟使能 */

/* 脚踏旋转及升降 推杆电机 4 && 5 驱动输出 TIME4  */
/* TIMX REMAP设置* 开启TIM4的重映射功能, 将TIM4_CH1 TIM4_CH2 TIM4_CH3 TIM4_CH4输出到     PD12 PD13 PD14 PD15 上，*/
/*推杆电机 1 输出*/
#define GTIM_TIM4_PWM_CHY_GPIO_PORTD GPIOD
#define GTIM_TIM4_PWM_CHY_GPIO_PIN12 GPIO_PIN_12 // T5
#define GTIM_TIM4_PWM_CHY_GPIO_PIN13 GPIO_PIN_13
#define GTIM_TIM4_PWM_CHY_GPIO_PIN14 GPIO_PIN_14 // T4
#define GTIM_TIM4_PWM_CHY_GPIO_PIN15 GPIO_PIN_15
#define GTIM_TIM4_PWM TIM4						/* TIM4 */
#define GTIM_TIM4_PWM_CH1 TIM_CHANNEL_1			/* 通道Y,  1<= Y <=4 */
#define GTIM_TIM4_PWM_CH2 TIM_CHANNEL_2			/* 通道Y,  1<= Y <=4 */
#define GTIM_TIM4_PWM_CH3 TIM_CHANNEL_3			/* 通道Y,  1<= Y <=4 */
#define GTIM_TIM4_PWM_CH4 TIM_CHANNEL_4			/* 通道Y,  1<= Y <=4 */
#define GTIM_TIM4_PWM_CH1_CCRX TIM4->CCR1		/* 通道1的输出比较寄存器 */
#define GTIM_TIM4_PWM_CH2_CCRX TIM4->CCR2		/* 通道2的输出比较寄存器 */
#define GTIM_TIM4_PWM_CH3_CCRX TIM4->CCR3		/* 通道3的输出比较寄存器 */
#define GTIM_TIM4_PWM_CH4_CCRX TIM4->CCR4		/* 通道4的输出比较寄存器 */
#define GTIM_TIM4_PWM_CHY_GPIO_AF GPIO_AF2_TIM4 /* 端口复用到TIM4 */
#define GTIM_TIM4_PWM_CHY_GPIOD_CLK_ENABLE() \
	do                                       \
	{                                        \
		__HAL_RCC_GPIOD_CLK_ENABLE();        \
	} while (0) /* PA口时钟使能 */
#define GTIM_TIM4_PWM_CHY_CLK_ENABLE() \
	do                                 \
	{                                  \
		__HAL_RCC_TIM4_CLK_ENABLE();   \
	} while (0) /* TIM4 时钟使能 */

/* 推杆电机 6 驱动输出 TIME12  */
/* TIMX REMAP设置* 开启TIM1的重映射功能, 将TIM12_CH1 TIM12_CH2 输出到PB14 PB15 上，*/
/*推杆电机 6 输出*/
#define GTIM_TIM12_PWM_CHY_GPIO_PORTB GPIOB
#define GTIM_TIM12_PWM_CHY_GPIO_PIN14 GPIO_PIN_14 // T6
#define GTIM_TIM12_PWM_CHY_GPIO_PIN15 GPIO_PIN_15
#define GTIM_TIM12_PWM TIM12					  /* TIM12 */
#define GTIM_TIM12_PWM_CH1 TIM_CHANNEL_1		  /* 通道Y,  1<= Y <=4 */
#define GTIM_TIM12_PWM_CH2 TIM_CHANNEL_2		  /* 通道Y,  1<= Y <=4 */
#define GTIM_TIM12_PWM_CH1_CCRX TIM12->CCR1		  /* 通道1的输出比较寄存器 */
#define GTIM_TIM12_PWM_CH2_CCRX TIM12->CCR2		  /* 通道2的输出比较寄存器 */
#define GTIM_TIM12_PWM_CHY_GPIO_AF GPIO_AF9_TIM12 /* 端口复用到TIM12 */
#define GTIM_TIM12_PWM_CHY_GPIOB_CLK_ENABLE() \
	do                                        \
	{                                         \
		__HAL_RCC_GPIOB_CLK_ENABLE();         \
	} while (0) /* PB口时钟使能 */
#define GTIM_TIM12_PWM_CHY_CLK_ENABLE() \
	do                                  \
	{                                   \
		__HAL_RCC_TIM12_CLK_ENABLE();   \
	} while (0) /* TIM12 时钟使能 */
/*姿态指令控制宏定义*/
#define NONE 0
#define STANCE 1
#define SITTING 2
#define SEAT_LIFT 3
#define SEAT_DROP 4
#define BACKREST_FORWARD 5
#define BACKREST_BACK 6
#define ALL_FORWARD 7
#define ALL_BACK 8
#define LEG_TOPSPIN 9
#define LEG_BACKSPIN 10
#define SEAT_LIFTDROPRATIO 0.819   // 座板角度B2/B1底盘举升

/*************************电机驱动变量*****************************/
extern TIM_HandleTypeDef g_time5_pwm_chy_handle;  /* 底盘L电机 1 函数句柄*/
extern TIM_HandleTypeDef g_time9_pwm_chy_handle;  /* 底盘R电机 2 函数句柄*/
extern TIM_HandleTypeDef g_time1_pwm_chy_handle;  /* 推杆1 （举升）  TIME1 函数句柄*/
extern TIM_HandleTypeDef g_time8_pwm_chy_handle;  /* 推杆2 （座板）    推杆3 （靠背） TIME8 函数句柄*/
extern TIM_HandleTypeDef g_time4_pwm_chy_handle;  /* 推杆4 （脚踏旋转） 推杆5 （脚踏伸长）TIME1 函数句柄*/
extern TIM_HandleTypeDef g_time12_pwm_chy_handle; /*推杆6 （前支撑轮） TIME12 函数句柄*/
//extern SlAVEKEYSTATE slavekeystate ;

void MoterL_pwm_chy_init(uint16_t arr, uint16_t psc);				   // 左轮电机
void MoterR_pwm_chy_init(uint16_t arr, uint16_t psc);				   // 右轮电机
void MoterLift_pwm_chy_init(uint16_t arr, uint16_t psc);			   // 电动推杆 1
void MoterPedestal_Backboard_pwm_chy_init(uint16_t arr, uint16_t psc); // 电动推杆 2&3
void MoterLeg_pwm_chy_init(uint16_t arr, uint16_t psc);				   // 电动推杆 4&5
void MoterSupport_pwm_chy_init(uint16_t arr, uint16_t psc);			   // 电动推杆 6
void MoterdriveInit(void);
void linearactuatorTest(void);
void SeatLiftDrop(void);
void BackresetFB(void);
void ThwartFB(void);
void OverallFB(void);
void LegSpinFB(void);
void OverallStandSit(void);
#endif
