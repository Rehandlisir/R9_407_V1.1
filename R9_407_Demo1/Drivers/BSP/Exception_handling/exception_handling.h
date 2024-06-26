#ifndef __EXCEPT_H
#define __EXCEPT_H
#include "./SYSTEM/sys/sys.h"
#include "./BSP/R9/brake.h"
#include "./BSP/R9/Slavemodbus.h"
#include "./BSP/R9/getadcdata.h"
#include "./BSP/CAN/can.h"
void ex_handl_Init(void);
void ex_handl_brake(void);
void ex_handl_LRmoter(void);
void ex_handl_linearActuator(void);
void ex_handl_battary(void);
void ex_handl_joystic(void);
void ex_handl_indicatorlight(void);
void ex_handl_excute(void);
#endif