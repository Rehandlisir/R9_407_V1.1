#ifndef __OVERCURRENT_PROTECTION_H
#define __OVERCURRENT_PROTECTION_H

#include "./SYSTEM/sys/sys.h"


//#define D_MOTOR_PROTECT_POART                 	   GPIOE
//#define D_MOTOR_PROTECT_GPIO_PIN                   GPIO_PIN_0
//#define D_MOTOR_PROTECT_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)             /* 作为输入使用 */

#define D_MOTOR1_SLEEP_POART                 	    GPIOB
#define D_MOTOR1_SLEEP_GPIO_PIN                           GPIO_PIN_12
#define D_MOTOR1_SLEEP_GPIO_CLK_ENABLE()                      do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)             /*电机1 驱动IC启动（高电平启动，低电平关闭） */


#define D_MOTOR2_SLEEP_POART                 	    GPIOB
#define D_MOTOR2_SLEEP_GPIO_PIN                        GPIO_PIN_13
#define D_MOTOR2_SLEEP_GPIO_CLK_ENABLET()                      do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)             /* 电机2 驱动IC启动（高电平启动，低电平关闭） */


#define T_MOTOR_PROTECT_GPIO_PORT                  GPIOD
#define T_MOTOR_PROTECT_GPIO_PIN                   GPIO_PIN_7
#define T_MOTOR_PROTECT_CLK_ENABLE()          	   do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)             /* 推杆过流保护（初始化为高，低电平时保护） */


/******************************************************************************************/

/* ????????? *//* ????�F????*/
#define D_MOTOR_PROTECT(x)   do{ x ? \
                      HAL_GPIO_WritePin(D_MOTOR_PROTECT_POART, D_MOTOR_PROTECT_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(D_MOTOR_PROTECT_POART, D_MOTOR_PROTECT_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)       /* LED0 = RED */

#define T_MOTOR_PROTECT(x)   do{ x ? \
                      HAL_GPIO_WritePin(T_MOTOR_PROTECT_GPIO_PORT, T_MOTOR_PROTECT_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(T_MOTOR_PROTECT_GPIO_PORT, T_MOTOR_PROTECT_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)       /* LED1 = GREEN */

#define D_MOTOR1_SLEEP(x)   do{ x ? \
                      HAL_GPIO_WritePin(D_MOTOR1_SLEEP_POART, D_MOTOR1_SLEEP_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(D_MOTOR1_SLEEP_POART, D_MOTOR1_SLEEP_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)       /* LED0 = RED */

#define D_MOTOR2_SLEEP(x)   do{ x ? \
                      HAL_GPIO_WritePin(D_MOTOR2_SLEEP_POART, D_MOTOR2_SLEEP_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(D_MOTOR2_SLEEP_POART, D_MOTOR2_SLEEP_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)       /* LED0 = RED */



void dcurrentpro_init(void);
				  
void tcurrentpro_init(void);
void currentproInit(void);
#endif

