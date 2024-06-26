/*
 * @Author: lisir lisir@rehand.com
 * @Date: 2024-06-07 16:01:18
 * @LastEditors: lisir lisir@rehand.com
 * @LastEditTime: 2024-06-14 15:14:08
 * @FilePath: \MDK-ARMc:\Users\fu\Desktop\Code\CodeV1.1\R9_407_V1.1\R9_407_V1.1\Drivers\BSP\task.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
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
#include "./BSP/Exception_handling/exception_handling.h"
//任务列表
void Hard_devInit(void);
void Task_led_control(void);
void Task_GetADC_AllData(void);
void Task_UnderpanDrive(void);
void Task_linearactuatorDrive(void);
void Task_gyroscopeData(void);
void Task_ModbusSlaveExecute (void);
void Task_ultrasonicreadExecute1 (void);
void Task_ultrasonicreadExecute2 (void);
void Task_CanKeyRun(void);
void Task_ex_handl(void);
#endif
