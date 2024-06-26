/**
 * @FilePath     : /MDK-ARMc:/Users/fu/Desktop/Code/CodeV1.1/R9_407_V1.1/R9_407_Demo1/Drivers/BSP/R9/moterdriver.h
 * @Description  :  Moter Drive Header
 * @Author       : lisir lisir@rehand.com
 * @Version      : 0.0.1
 * @LastEditors  : lisir lisir@rehand.com
 * @LastEditTime : 2024-06-24 09:53:14
 * @2024 by Rehand Medical Technology Co., LTD, All Rights Reserved.
**/

#ifndef __MOTERDRIVER_H
#define __MOTERDRIVER_H
#include "./SYSTEM/sys/sys.h"
#include "./BSP/CAN/can.h"
#include "./BSP/R9/Slavemodbus.h"
#include "math.h"
#include "stdio.h"
#include "./BSP/R9/getadcdata.h"
/**********************************底盘L 电机1 驱动输出 TIME1 CH1 CH2 CH1N CH2N  *************************************/
#define ATIM_TIM1_CPLM_CH1_GPIO_PORT            GPIOA
#define ATIM_TIM1_CPLM_CH1_GPIO_PIN             GPIO_PIN_8
#define ATIM_TIM1_CPLM_CH1_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PE口时钟使能 */
#define ATIM_TIM1_CPLM_CH2_GPIO_PORT            GPIOE
#define ATIM_TIM1_CPLM_CH2_GPIO_PIN             GPIO_PIN_11
#define ATIM_TIM1_CPLM_CH2_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   /* PE口时钟使能 */
/* 互补输出通道引脚 */
#define ATIM_TIM1_CPLM_CH1N_GPIO_PORT           GPIOA
#define ATIM_TIM1_CPLM_CH1N_GPIO_PIN            GPIO_PIN_7
#define ATIM_TIM1_CPLM_CH1N_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PE口时钟使能 */
#define ATIM_TIM1_CPLM_CH2N_GPIO_PORT           GPIOE
#define ATIM_TIM1_CPLM_CH2N_GPIO_PIN            GPIO_PIN_10
#define ATIM_TIM1_CPLM_CH2N_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   /* PE口时钟使能 */
/* 刹车输入引脚 */
#define ATIM_TIM1_CPLM_BKIN_GPIO_PORT           GPIOE
#define ATIM_TIM1_CPLM_BKIN_GPIO_PIN            GPIO_PIN_15
#define ATIM_TIM1_CPLM_BKIN_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   /* PE口时钟使能 */

#define LEFT_SOFTBRAKE(x)                                                                                                                                \
    do                                                                                                                                         \
    {                                                                                                                                          \
        x ? HAL_GPIO_WritePin(ATIM_TIM1_CPLM_BKIN_GPIO_PORT, ATIM_TIM1_CPLM_BKIN_GPIO_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(ATIM_TIM1_CPLM_BKIN_GPIO_PORT, ATIM_TIM1_CPLM_BKIN_GPIO_PIN, GPIO_PIN_RESET); \
    } while (0) /*  */
/*TIM1 REMAP设置*/
#define ATIM_TIM1_CPLM_CHY_GPIO_AF             GPIO_AF1_TIM1
/* 互补输出使用的定时器 */
#define ATIM_TIM1_CPLM                          TIM1
#define ATIM_TIM1_CPLM_CH1                      TIM_CHANNEL_1
#define ATIM_TIM1_CPLM_CH2                      TIM_CHANNEL_2
#define ATIM_TIM1_CPLM_CH1_CCR1                 ATIM_TIM1_CPLM->CCR1
#define ATIM_TIM1_CPLM_CH2_CCR2                 ATIM_TIM1_CPLM->CCR2
#define ATIM_TIM1_CPLM_CLK_ENABLE()             do{ __HAL_RCC_TIM1_CLK_ENABLE(); }while(0)    /* TIM1 时钟使能 */

/**********************************底盘R 电机2驱动输出 TIME8 CH1 CH2 CH1N CH2N*************************************/
/* 输出通道引脚 */
#define ATIM_TIM8_CPLM_CH1_GPIO_PORT            GPIOC
#define ATIM_TIM8_CPLM_CH1_GPIO_PIN             GPIO_PIN_6
#define ATIM_TIM8_CPLM_CH1_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* PE口时钟使能 */

#define ATIM_TIM8_CPLM_CH2_GPIO_PORT            GPIOC
#define ATIM_TIM8_CPLM_CH2_GPIO_PIN             GPIO_PIN_7
#define ATIM_TIM8_CPLM_CH2_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* PE口时钟使能 */

/* 互补输出通道引脚 */
#define ATIM_TIM8_CPLM_CH1N_GPIO_PORT           GPIOA
#define ATIM_TIM8_CPLM_CH1N_GPIO_PIN            GPIO_PIN_5
#define ATIM_TIM8_CPLM_CH1N_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PE口时钟使能 */

#define ATIM_TIM8_CPLM_CH2N_GPIO_PORT           GPIOB
#define ATIM_TIM8_CPLM_CH2N_GPIO_PIN            GPIO_PIN_0
#define ATIM_TIM8_CPLM_CH2N_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PE口时钟使能 */
/* 刹车输入引脚 */
#define ATIM_TIM8_CPLM_BKIN_GPIO_PORT           GPIOA
#define ATIM_TIM8_CPLM_BKIN_GPIO_PIN            GPIO_PIN_6
#define ATIM_TIM8_CPLM_BKIN_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PE口时钟使能 */
#define RIGHT_SOFTBRAKE(x)                                                                                                                     \
    do                                                                                                                                         \
    {                                                                                                                                          \
        x ? HAL_GPIO_WritePin(ATIM_TIM8_CPLM_BKIN_GPIO_PORT, ATIM_TIM8_CPLM_BKIN_GPIO_PIN,GPIO_PIN_SET) : HAL_GPIO_WritePin(ATIM_TIM8_CPLM_BKIN_GPIO_PORT, ATIM_TIM8_CPLM_BKIN_GPIO_PIN, GPIO_PIN_RESET); \
    } while (0) /*  */
/* TIM8 REMAP设置*/
#define ATIM_TIM8_CPLM_CHY_GPIO_AF             GPIO_AF3_TIM8

/* 互补输出使用的定时器 */
#define ATIM_TIM8_CPLM                          TIM8
#define ATIM_TIM8_CPLM_CH1                      TIM_CHANNEL_1
#define ATIM_TIM8_CPLM_CH2                      TIM_CHANNEL_2
#define ATIM_TIM8_CPLM_CH1_CCR1                 ATIM_TIM8_CPLM->CCR1
#define ATIM_TIM8_CPLM_CH2_CCR2                 ATIM_TIM8_CPLM->CCR2
#define ATIM_TIM8_CPLM_CLK_ENABLE()             do{ __HAL_RCC_TIM8_CLK_ENABLE(); }while(0)    /* TIM1 时钟使能 */

/**********************************靠背角度撑杆A1  *************************************/
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


/**********************************座板角度撑杆B2  *************************************/
#define GTIM_TIM8_PWM_CHY_GPIO_PORTC GPIOC
#define GTIM_TIM8_PWM_CHY_GPIO_PIN8 GPIO_PIN_8 // T2
#define GTIM_TIM8_PWM_CHY_GPIO_PIN9 GPIO_PIN_9

#define GTIM_TIM8_PWM TIM8				  /* TIM8 */
#define GTIM_TIM8_PWM_CH3 TIM_CHANNEL_3	  /* 通道Y,  1<= Y <=4 */
#define GTIM_TIM8_PWM_CH4 TIM_CHANNEL_4	  /* 通道Y,  1<= Y <=4 */
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

/**********************************底盘举升撑杆B1(M)  *************************************/
#define GTIM_TIM3_PWM_CHY_GPIO_PORTB GPIOB
#define GTIM_TIM3_PWM_CHY_GPIO_PIN4 GPIO_PIN_4 // T2
#define GTIM_TIM3_PWM_CHY_GPIO_PIN5 GPIO_PIN_5

#define GTIM_TIM3_PWM TIM3				  /* TIM8 */
#define GTIM_TIM3_PWM_CH1 TIM_CHANNEL_1	  /* 通道Y,  1<= Y <=4 */
#define GTIM_TIM3_PWM_CH2 TIM_CHANNEL_2	  /* 通道Y,  1<= Y <=4 */

#define GTIM_TIM3_PWM_CH1_CCRX TIM3->CCR1 /* 通道3的输出比较寄存器 */
#define GTIM_TIM3_PWM_CH2_CCRX TIM3->CCR2 /* 通道4的输出比较寄存器 */

#define GTIM_TIM3_PWM_CHY_GPIO_AF GPIO_AF2_TIM3 /* 端口复用到TIM3 */

#define GTIM_TIM3_PWM_CHY_GPIOB_CLK_ENABLE() \
	do                                       \
	{                                        \
		__HAL_RCC_GPIOB_CLK_ENABLE();        \
	} while (0) /* PC口时钟使能 */
#define GTIM_TIM3_PWM_CHY_CLK_ENABLE() \
	do                                 \
	{                                  \
		__HAL_RCC_TIM3_CLK_ENABLE();   \
	} while (0) /* TIM3 时钟使能 */

/**********************************腿托角度撑杆A2 (L)、腿托长度撑杆A3(L)  *************************************/
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

/**********************************前支撑轮撑杆C1  *************************************/
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
extern TIM_HandleTypeDef g_time1_pwm_chy_handle;  /* 靠背推杆 */
extern TIM_HandleTypeDef g_time8_pwm_chy_handle;  /* 座板推杆*/
extern TIM_HandleTypeDef g_time3_pwm_chy_handle;  /*举升推杆 */
extern TIM_HandleTypeDef g_time4_pwm_chy_handle;  /*腿托长度及角度*/
extern TIM_HandleTypeDef g_time12_pwm_chy_handle; /* 前支撑轮*/

//extern SlAVEKEYSTATE slavekeystate ;

void MoterL_pwm_chy_init(uint16_t arr, uint16_t psc);				   // 左轮电机
void atim_tim1_cplm_pwm_set(uint16_t ccr1,uint16_t ccr2);
void LeftMoterMove(uint8_t dir,double duty_cycle);
void LeftMoterStop(void);

void MoterR_pwm_chy_init(uint16_t arr, uint16_t psc);				   // 右轮电机
void atim_tim8_cplm_pwm_set(uint16_t ccr1,uint16_t ccr2);
void RightMoterMove(uint8_t dir,double duty_cycle);
void RightMoterStop(void);


void Moterbackrest_pwm_chy_init(uint16_t arr, uint16_t psc);
void MoterPedestal_pwm_chy_init(uint16_t arr, uint16_t psc);
void MoterLift_pwm_chy_init(uint16_t arr, uint16_t psc);
void MoterLeg_pwm_chy_init(uint16_t arr, uint16_t psc);
void MoterSupport_pwm_chy_init(uint16_t arr, uint16_t psc);
 
void MoterdriveInit(void);
void linearactuatorTest(void);
void linearactuator(void);



#endif
