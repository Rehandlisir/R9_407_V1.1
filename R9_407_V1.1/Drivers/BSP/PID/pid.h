/**
 ****************************************************************************************************
 * @file        pid.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-14
 * @brief       PID�㷨����
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F407���������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com/forum.php
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20211014
 * ��һ�η���
 *
 ****************************************************************************************************
 */
#ifndef __PID_H
#define __PID_H

#include "./SYSTEM/sys/sys.h"

/******************************************************************************************/

#define  INCR_LOCT_SELECT  1         /* 0:λ��ʽPID ��1������ʽPID */

#if INCR_LOCT_SELECT

/* ����ʽPID���� */
#define  KP      8.50f               /* P����*/
#define  KI      5.00f               /* I����*/
#define  KD      0.10f               /* D����*/

#else

/* λ��ʽPID���� */
#define  KP      10.0f               /* P����*/
#define  KI      8.00f               /* I����*/
#define  KD      0.5f                /* D����*/

#endif

/*PID�ṹ��*/
typedef struct
{
    __IO float  SetPoint;            /* Ŀ��ֵ */
    __IO float  ActualValue;         /* �������ֵ */
    __IO float  SumError;            /* �ۼ�ƫ�� */
    __IO float  Proportion;          /* �������� P */
    __IO float  Integral;            /* ���ֳ��� I */
    __IO float  Derivative;          /* ΢�ֳ��� D */
    __IO float  Error;               /* Error[1] */
    __IO float  LastError;           /* Error[-1] */
    __IO float  PrevError;           /* Error[-2] */
} PID_TypeDef;

extern PID_TypeDef  g_speed_pid;     /*�ٶȻ�PID�����ṹ��*/

/******************************************************************************************/

void pid_init(void);                 /* pid��ʼ�� */
int32_t increment_pid_ctrl(PID_TypeDef *PID,float Feedback_value);      /* pid�ջ����� */

#endif
