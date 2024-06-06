/**
 ****************************************************************************************************
 * @file        beep.h
 * @author      Lisir
 * @version     V1.0
 * @date        2021-10-14
 * @brief       �������� ��������
 * @license     Copyright (c) 2024, ���ڸ���ҽ�ƿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:F407������
 * none
 * none
 * none
 * none
 *
 * �޸�˵��
 * none
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef __COMMON_H
#define __COMMON_H

#include "./SYSTEM/sys/sys.h"


#define WINDOW_SIZE 10
 
// ��������ƽ���˲����ṹ��
typedef struct {
    double window[WINDOW_SIZE];
    uint8_t index;
} AverageFilter;

extern  AverageFilter filter_L;
extern  AverageFilter filter_R;
int32_t Value_limit(int32_t min_value ,int32_t current_value ,int32_t max_value);
int32_t  Value_Resetzero(int32_t min_value ,int32_t current_value ,int32_t max_value);
int32_t slopelimitx( int32_t value,int32_t increvalue) ;   
int32_t slopelimity( int32_t value,int32_t increvalue) ;   

float Value_limitf(float min_value ,float current_value ,float max_value);


void initializeFilter(AverageFilter* filter);

double filterValue(AverageFilter* filter, double input);



#endif

















