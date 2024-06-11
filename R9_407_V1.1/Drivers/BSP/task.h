#ifndef __TASK_H__
#define __TASK_H__
#include "./SYSTEM/sys/sys.h"

#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"

#include "./BSP/R9/underpanControl.h"
#include "./BSP/R9/moterdriver.h"
#include "./BSP/R9/brake.h"
#include "./BSP/TIMER/btim.h"
#include "./BSP/Common/common.h"
#include "./BSP/R9/overcurrent_protection.h"
// #include "./BSP/R9/hallcap.h"
// #include "./BSP/R9/stmflash.h"
#include "./BSP/R9/getadcdata.h"
#include "./BSP/R9/mpu6050.h"
#include "./BSP/R9/inv_mpu.h"
#include "./BSP/R9/inv_mpu_dmp_motion_driver.h" 

//#include "./BSP/R9/rs485.h"
//#include "./BSP/R9/mlx90393.h"

#include "./BSP/WDG/wdg.h"
#include "./BSP/DAP21/hostdap21.h"
//#include "./BSP/R9/Hostmodbus.h"
#include "./BSP/CAN/can.h"
#include "./BSP/R9/Slavemodbus.h"

//任务列表
void Hard_devInit(void);
void LedFlash(void);
void GetADC_AllData(void);
void UnderpanDrive(void);
void linearactuatorDrive(void);
void gyroscopeData(void);
void ModbusSlaveExecute (void);
void ultrasonicreadExecute (void);
//void Modbuskeyread_execute(void);
//void Modbuskeywrite_execute(void);
void CanKeyRun(void);

#endif
