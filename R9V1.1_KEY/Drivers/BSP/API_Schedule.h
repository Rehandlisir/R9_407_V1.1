#ifndef __API_SCHEDULE_H__
#define __API_SCHEDULE_H__
#include "./SYSTEM/sys/sys.h"
/*
用与存储一个任务运行的数据
*/
struct TaskStruct
{
	uint16_t	TaskTickNow;//用于计时
	uint16_t	TaskTickMax;//设置计时时间
	uint8_t	TaskStatus;//任务运行标志位
	void (*FC)();//任务函数指针
};

extern struct TaskStruct TaskST[];	//声明为结构体数据，那样多个任务时方便管理

//框架运行所需的函数声明
void PeachOSRun(void);
void OS_IT_RUN(void);


#endif
