
#ifndef __GETADCDATA_H
#define __GETADCDATA_H

#include "./SYSTEM/sys/sys.h"

/************************************** ҡ�����ݳ��� �� ADC ���ݽṹ�嶨�� ****************************************************/
#define yadc_max      1700
#define yadc_min      -1700
#define xadc_max      1700
#define xadc_min      -1700

#define yadc_Dim      500
#define xadc_Dim      500


typedef struct 
{
    int32_t l_brakcurrent;
    int32_t r_brakcurrent;



    int32_t adc_x;
    int32_t adc_y;
    int32_t adc_xbase;
    int32_t adc_ybase;
    

    int32_t l_current;
    int32_t r_current;

    int32_t lift_current;
    int32_t pedestal_current;
    int32_t backboard_current;
    int32_t legangle_current;
    int32_t leglength_current;
    int32_t support_current;


    int32_t lift_pos;
    int32_t pedestal_pos;
    int32_t backboard_pos;
    int32_t legangle_pos;
    int32_t leglength_pos;
    int32_t support_pos;
    int32_t bat_v;
} ADCDATA;

void Datareset(void);
void getadcDataInit(void);
void getadcData(void);
void getadc1Data(void);
void getadc3Data(void);
void joysticDatacalibrationInit(void);
#endif
