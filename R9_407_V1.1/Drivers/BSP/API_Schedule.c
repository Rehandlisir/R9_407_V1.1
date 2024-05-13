#include "API_Schedule.h"
#include "./BSP/task.h"

/************任务初始化，每一个结构体都代表一个任务，添加任务和删减任务都在这里完成************/
struct TaskStruct TaskST[] =
	{
		{0, 1, 0, linearactuatorDrive},
		{0, 2, 0, gyroscopeData},
		{0, 5, 0, UnderpanDrive},
		{0, 1, 0, GetADC_AllData},
		{0, 300, 0, LedFlash},
		{0, 1, 0, ModbusSlaveExecute},
		{0, 150, 0, ultrasonicreadExecute},
		{0, 2, 0, Modbuskeyread_execute},
		{0, 200, 0, Modbuskeywrite_execute},

};

/*******************************搭建时间片轮询机制代码框架  ********************************/

// 记录任务数量
uint8_t TaskCount = sizeof(TaskST) / sizeof(TaskST[0]);

// 放在“TIM2_IRQHandler”中断执行，用于任务计时
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
		// printf("已喂狗\n");

		if (TaskST[j].TaskStatus) // 判断一个任务是否被挂起
		{
			TaskST[j].FC();			  // 执行该任务函数
			TaskST[j].TaskStatus = 0; // 取消任务的挂起状态
		}
		if (++j >= TaskCount) // 相当于不断循环遍历所有的任务
			j = 0;
	}
}
