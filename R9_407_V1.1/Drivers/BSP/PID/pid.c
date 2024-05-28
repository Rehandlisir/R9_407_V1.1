/**
 ****************************************************************************************************
 * @file        pid.c
 * @author      R9团队
 * @version     V1.0
 * @date        2021-10-14
 * @brief       PID算法代码
 * @license     复成医疗
 ****************************************************************************************************
 * @attention
 *
 *   复成医疗
 *
 ****************************************************************************************************
 */

#include "./BSP/PID/pid.h"
//#include "./BSP/DC_MOTOR/dc_motor.h"

PID_TypeDef  g_speed_pid;           /* 速度环PID参数结构体 */

/**
 * @brief       pid初始化
 * @param       无
 * @retval      无
 */
void pid_init(void)
{
    g_speed_pid.SetPoint = 0;       /* 设定目标值 */
    g_speed_pid.ActualValue = 0.0;  /* 输出期望值清零 */
    g_speed_pid.SumError = 0.0;     /* 累计偏差 */
    g_speed_pid.Error = 0.0;        /* Error[1] */
    g_speed_pid.LastError = 0.0;    /* Error[-1] */
    g_speed_pid.PrevError = 0.0;    /* Error[-2] */
    g_speed_pid.Proportion = KP;    /* 比例常数 P */
    g_speed_pid.Integral = KI;      /* 积分常数 I */
    g_speed_pid.Derivative = KD;    /* 微分常数 D */ 
}

/**
 * @brief       pid闭环控制
 * @param       当前实际值
 * @retval      期望控制值
 */
int32_t increment_pid_ctrl(PID_TypeDef *PID,float Feedback_value)
{
    PID->Error = (float)(PID->SetPoint - Feedback_value);                   /* 计算偏差 */
    
#if  INCR_LOCT_SELECT                                                       /* 增量式PID */
    
    PID->ActualValue += (PID->Proportion * (PID->Error - PID->LastError))                           /* 比例项 */
                        + (PID->Integral * PID->Error)                                              /* 积分项 */
                        + (PID->Derivative * (PID->Error - 2 * PID->LastError + PID->PrevError));   /* 微分项 */
    
    PID->PrevError = PID->LastError;                                        /* 存储偏差，用于下次计算 */
    PID->LastError = PID->Error;
    
#else                                                                       /* 位置式PID */
    
    PID->SumError += PID->Error;                                            /* 偏差e累加求和 */
    PID->ActualValue = (PID->Proportion * PID->Error)                       /* 比例项 */
                       + (PID->Integral * PID->SumError)                    /* 积分项 */
                       + (PID->Derivative * (PID->Error - PID->LastError)); /* 微分项 */
    PID->LastError = PID->Error;                                            /* 存储偏差，用于下次计算 */
    
#endif
    return ((int32_t)(PID->ActualValue));                                   /* 返回计算后输出的期望值 */
    

}
