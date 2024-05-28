/**
 ****************************************************************************************************
 * @file        pid.c
 * @author      R9�Ŷ�
 * @version     V1.0
 * @date        2021-10-14
 * @brief       PID�㷨����
 * @license     ����ҽ��
 ****************************************************************************************************
 * @attention
 *
 *   ����ҽ��
 *
 ****************************************************************************************************
 */

#include "./BSP/PID/pid.h"
//#include "./BSP/DC_MOTOR/dc_motor.h"

PID_TypeDef  g_speed_pid;           /* �ٶȻ�PID�����ṹ�� */

/**
 * @brief       pid��ʼ��
 * @param       ��
 * @retval      ��
 */
void pid_init(void)
{
    g_speed_pid.SetPoint = 0;       /* �趨Ŀ��ֵ */
    g_speed_pid.ActualValue = 0.0;  /* �������ֵ���� */
    g_speed_pid.SumError = 0.0;     /* �ۼ�ƫ�� */
    g_speed_pid.Error = 0.0;        /* Error[1] */
    g_speed_pid.LastError = 0.0;    /* Error[-1] */
    g_speed_pid.PrevError = 0.0;    /* Error[-2] */
    g_speed_pid.Proportion = KP;    /* �������� P */
    g_speed_pid.Integral = KI;      /* ���ֳ��� I */
    g_speed_pid.Derivative = KD;    /* ΢�ֳ��� D */ 
}

/**
 * @brief       pid�ջ�����
 * @param       ��ǰʵ��ֵ
 * @retval      ��������ֵ
 */
int32_t increment_pid_ctrl(PID_TypeDef *PID,float Feedback_value)
{
    PID->Error = (float)(PID->SetPoint - Feedback_value);                   /* ����ƫ�� */
    
#if  INCR_LOCT_SELECT                                                       /* ����ʽPID */
    
    PID->ActualValue += (PID->Proportion * (PID->Error - PID->LastError))                           /* ������ */
                        + (PID->Integral * PID->Error)                                              /* ������ */
                        + (PID->Derivative * (PID->Error - 2 * PID->LastError + PID->PrevError));   /* ΢���� */
    
    PID->PrevError = PID->LastError;                                        /* �洢ƫ������´μ��� */
    PID->LastError = PID->Error;
    
#else                                                                       /* λ��ʽPID */
    
    PID->SumError += PID->Error;                                            /* ƫ��e�ۼ���� */
    PID->ActualValue = (PID->Proportion * PID->Error)                       /* ������ */
                       + (PID->Integral * PID->SumError)                    /* ������ */
                       + (PID->Derivative * (PID->Error - PID->LastError)); /* ΢���� */
    PID->LastError = PID->Error;                                            /* �洢ƫ������´μ��� */
    
#endif
    return ((int32_t)(PID->ActualValue));                                   /* ���ؼ�������������ֵ */
    

}
