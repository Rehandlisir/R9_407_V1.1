/**
 * @FilePath     : /MDK-ARMc:/Users/fu/Desktop/Code/CodeV1.1/R9_407_V1.1/R9_407_V1.1/Drivers/BSP/API_Schedule.c
 * @Description  :  
 * @Author       : lisir
 * @Version      : V1.1
 * @LastEditors  : lisir
 * @LastEditTime : 2024-06-14 16:13:30
 * @Copyright (c) 2024 by Rehand Medical Technology Co., LTD, All Rights Reserved. 
**/
#include "API_Schedule.h"
#include "./BSP/task.h"
/**
 * @description: 任务初始化，每一个结构体都代表一个任务，添加任务和删减任务都在这里完成
 * @return {*}
 */
struct TaskStruct TaskST[] =
	{
	 	{0, 1, 0, linearactuatorDrive},
		{0, 2, 0, gyroscopeData},
		{0, 5, 0, UnderpanDrive},
		{0, 1, 0, GetADC_AllData},
		{0, 300, 0, LedFlash},
		{0, 1, 0, ModbusSlaveExecute},
		{0, 120, 0, ultrasonicreadExecute1},
		{0, 150, 0, ultrasonicreadExecute2},
		{0, 1, 0, CanKeyRun},
	};

// 记录任务数量
uint8_t TaskCount = sizeof(TaskST) / sizeof(TaskST[0]);
/**
 * @description: 任务轮询
 * @return {*}
 */
void OS_IT_RUN(void)
{
	uint8_t i;
	for (i = 0; i < TaskCount; i++) // 遍历所有循环
	{
		if (!TaskST[i].TaskStatus)
		{
			if (++TaskST[i].TaskTickNow >= TaskST[i].TaskTickMax) // 计时，并判断是否到达定时时间
			{
				TaskST[i].TaskTickNow = 0;
				TaskST[i].TaskStatus = 1;
			}
		}
	}
}
// 放在main函数中执行，自带死循环，用于执行挂起的任务
void PeachOSRun(void)
{
	Hard_devInit();

	uint8_t j = 0;
	while (1)
	{
		iwdg_feed(); /* 喂狗 */
		//printf("已喂狗\n");

		if (TaskST[j].TaskStatus) // 判断一个任务是否被挂起
		{
			TaskST[j].FC();			  // 执行该任务函数
			TaskST[j].TaskStatus = 0; // 取消任务的挂起状态
		}
		if (++j >= TaskCount) // 相当于不断循环遍历所有的任务
			j = 0;
	}
}
