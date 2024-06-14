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
 * @description: �����ʼ����ÿһ���ṹ�嶼����һ��������������ɾ���������������
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
