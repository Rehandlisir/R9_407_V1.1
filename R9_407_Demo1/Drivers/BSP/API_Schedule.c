/**
 * @FilePath     : /MDK-ARMc:/Users/fu/Desktop/Code/CodeV1.1/R9_407_V1.1/R9_407_Demo1/Drivers/BSP/API_Schedule.c
 * @Description  :  
 * @Author       : lisir
 * @Version      : V1.1
 * @LastEditors  : lisir lisir@rehand.com
 * @LastEditTime : 2024-06-24 14:20:22
 * @Copyright (c) 2024 by Rehand Medical Technology Co., LTD, All Rights Reserved. 
**/
#include "API_Schedule.h"
#include "./BSP/task.h"
/**
 * @description: �����ʼ����ÿһ���ṹ�嶼����һ��������������ɾ���������������
 * @return {*}
 */
struct TaskStruct TaskST[] =
	{
	 	{0, 1, 0, Task_linearactuatorDrive},
		{0, 2, 0, Task_gyroscopeData},
		{0, 5, 0, Task_UnderpanDrive},
		{0, 1, 0, Task_GetADC_AllData},
		{0, 300, 0, Task_led_control},
		{0, 1, 0, Task_ModbusSlaveExecute},
		{0, 120, 0, Task_ultrasonicreadExecute1},
		{0, 150, 0, Task_ultrasonicreadExecute2},
		{0, 1, 0, Task_CanKeyRun},
		{0, 1, 0, Task_ex_handl},
	};

// ��¼��������
uint8_t TaskCount = sizeof(TaskST) / sizeof(TaskST[0]);
/**
 * @description: ������ѯ
 * @return {*}
 */
void OS_IT_RUN(void)
{
	uint8_t i;
	for (i = 0; i < TaskCount; i++) // ��������ѭ��
	{
		if (!TaskST[i].TaskStatus)
		{
			if (++TaskST[i].TaskTickNow >= TaskST[i].TaskTickMax) // ��ʱ�����ж��Ƿ񵽴ﶨʱʱ��
			{
				TaskST[i].TaskTickNow = 0;
				TaskST[i].TaskStatus = 1;
			}
		}
	}
}
// ����main������ִ�У��Դ���ѭ��������ִ�й��������
void PeachOSRun(void)
{
	Hard_devInit();

	uint8_t j = 0;
	while (1)
	{
		iwdg_feed(); /* ι�� */
		//printf("��ι��\n");

		if (TaskST[j].TaskStatus) // �ж�һ�������Ƿ񱻹���
		{
			TaskST[j].FC();			  // ִ�и�������
			TaskST[j].TaskStatus = 0; // ȡ������Ĺ���״̬
		}
		if (++j >= TaskCount) // �൱�ڲ���ѭ���������е�����
			j = 0;
	}
}
