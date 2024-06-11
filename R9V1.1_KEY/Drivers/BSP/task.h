#ifndef __TASK_H__
#define __TASK_H__
#include "./SYSTEM/sys/sys.h"

#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/TIMER/btim.h"
#include "./BSP/R9/modbus.h"
#include "./BSP/WDG/wdg.h"
#include "./BSP/CAN/can.h"
//任务列表
void Hard_devInit(void);
void ModbusKey (void);
void Led_control (void);
#endif
