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
#include "stdio.h"
#include "./BSP/R9/getadcdata.h"

/*********************************以下是通用定时器PWM输出实验相关宏定义*************************************/

/**********************************底盘电机驱动输出 及 推杆电机 驱动输出  *************************************/

/* 底盘L 电机1 驱动输出 TIME5*/
/* TIMX REMAP设置* 开启TIM5的重映射功能, 将TIM5_CH1 及 TIM5_CH2 输出到PA0 PA1 上*/
#define GTIM_TIM3_PWM_CHY_GPIO_PORTB GPIOB
#define GTIM_TIM3_PWM_CHY_GPIO_PIN0 GPIO_PIN_0
#define GTIM_TIM3_PWM_CHY_GPIO_PIN1 GPIO_PIN_1
#define GTIM_TIM3_PWM TIM3						/* TIM3 */
#define GTIM_TIM3_PWM_CH3 TIM_CHANNEL_3			/* 通道Y,  1<= Y <=4 */
#define GTIM_TIM3_PWM_CH4 TIM_CHANNEL_4			/* 通道Y,  1<= Y <=4 */
#define GTIM_TIM3_PWM_CH3_CCRX TIM3->CCR3		/* 通道1的输出比较寄存器 */
#define GTIM_TIM3_PWM_CH4_CCRX TIM3->CCR4		/* 通道2的输出比较寄存器 */
#define GTIM_TIM3_PWM_CHY_GPIO_AF GPIO_AF2_TIM3 /* 端口复用到TIM3 */
#define GTIM_TIM3_PWM_CHY_GPIOB_CLK_ENABLE() \
	do                                       \
	{                                        \
		__HAL_RCC_GPIOB_CLK_ENABLE();        \
	} while (0) /* PA口时钟使能 */
#define GTIM_TIM3_PWM_CHY_CLK_ENABLE() \
	do                                 \
	{                                  \
		__HAL_RCC_TIM3_CLK_ENABLE();   \
	} while (0) /* TIM3 时钟使能 */

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

/* 靠背角度撑杆A1  */

#define GTIM_TIM1_PWM_CHY_GPIO_PORTA GPIOA
#define GTIM_TIM1_PWM_CHY_GPIO_PIN10 GPIO_PIN_10
#define GTIM_TIM1_PWM_CHY_GPIO_PIN11 GPIO_PIN_11
#define GTIM_TIM1_PWM TIM1						/* TIM1 */
#define GTIM_TIM1_PWM_CH3 TIM_CHANNEL_3			// T1                          /* 通道Y,  1<= Y <=4 */
#define GTIM_TIM1_PWM_CH4 TIM_CHANNEL_4		/* 通道Y,  1<= Y <=4 */
#define GTIM_TIM1_PWM_CH3_CCRX TIM1->CCR3		/* 通道1的输出比较寄存器 */
#define GTIM_TIM1_PWM_CH4_CCRX TIM1->CCR4		/* 通道2的输出比较寄存器 */
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

/* 底盘举升撑杆B1 / 座板角度撑杆B2 */

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

/*腿托角度撑杆A2 (L)、腿托长度撑杆A3(L)*/
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


/*前支撑轮撑杆C1*/
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
/*推杆1~6最大占空比*/
#define A1MAX 0.9    // 靠背角度撑杆A1（M）
#define A2MAX 0.9    //腿托角度撑杆A2 (M)
#define A3MAX 0.9    //腿托长度撑杆A3(M)   
#define B1MAX 0.9    //底盘举升撑杆B1(M)
#define B2MAX 0.9    //座板角度撑杆B2 (M)
#define C1MAX 0.9    //前支撑轮撑杆C1(M)

/*姿态状态宏定义*/
typedef enum
{
   
    iddle,
    Stand_run,
    Site_run,
    Lift_run,
    Down_run,
    Backf_run,
    Backb_run, 
    Alltiltfrun,
    Alltiltbrun,
    Legspintop_run,
    Legspindown_run,
    Legexten_run,
    Legunexten_run,
    Seat_tiltf_run,
    Seat_tiltb_run
    
} LineActor_State;
/*推杆限位参数*/
typedef struct 
{	
	uint16_t A1_Uppos;
	uint16_t A1_Downpos;
	uint16_t A2_Uppos;
	uint16_t A2_Downpos;
	uint16_t A3_Uppos;
	uint16_t A3_Downpos;
	uint16_t B1_Uppos;
	uint16_t B1_Downpos;
	uint16_t B2_Uppos;
	uint16_t B2_Downpos;
	uint16_t C1_Uppos;
	uint16_t C2_Downpos;		
} ACTORLIMITPARA;


#define SEAT_LIFTDROPRATIO 0.865   // 座板角度B2/B1底盘举升

/*************************电机驱动变量*****************************/
extern TIM_HandleTypeDef g_time3_pwm_chy_handle;  /* 底盘L电机 1 函数句柄*/
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

 void linearactuator(void);
 void SeatLiftDrop(void);
 void BackresetFB(void);
 void ThwartFB(void);
 void OverallFB(void);
 void LegSpinFB(void);
 void LegExtension(void);
 void OverallStandSit(void);
#endif
