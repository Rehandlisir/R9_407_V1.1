#include "API_Schedule.h"
#include "./BSP/task.h"

/************�����ʼ����ÿһ���ṹ�嶼����һ��������������ɾ���������������************/
struct TaskStruct TaskST[]=
{
     { 0,       1,              0,           linearactuatorDrive},
     { 0,       2,              0,           gyroscopeData},
	 { 0,       3,              0,                   UnderpanDrive},
   	 { 0,       4,              0,                GetADC_AllData},
	 { 0,       300,            0,		  	           LedFlash},
	 { 0,       500,            0,                      Beep_run},
	 { 0,       1,            0,                      DypA21}, // 150 ms �����ȶ�
	 { 0,       10,              0,                      Tmxl90393},
	 
};

/*******************************�ʱ��Ƭ��ѯ���ƴ�����  ********************************/

//��¼��������
uint8_t TaskCount=	sizeof(TaskST)/sizeof(TaskST[0]);	

//���ڡ�TIM2_IRQHandler���ж�ִ�У����������ʱ
void OS_IT_RUN(void)
{
	uint8_t i;
	for(i=0;i<TaskCount;i++)//��������ѭ��
	{	
		if(!TaskST[i].TaskStatus)
		{		
			if(++TaskST[i].TaskTickNow >= TaskST[i].TaskTickMax)//��ʱ�����ж��Ƿ񵽴ﶨʱʱ��
			{	
				TaskST[i].TaskTickNow = 0;
				TaskST[i].TaskStatus = 1;
			}
		}
	}
}



//����main������ִ�У��Դ���ѭ��������ִ�й��������
void PeachOSRun(void)
{   
	Hard_devInit();

	uint8_t j=0;
	while(1)
	{
		iwdg_feed();                    /* ι�� */
		//printf("��ι��\n");
		
		if(TaskST[j].TaskStatus)//�ж�һ�������Ƿ񱻹���
		{		
			TaskST[j].FC();				//ִ�и�������
			TaskST[j].TaskStatus=0;		//ȡ������Ĺ���״̬
		}
		if(++j>=TaskCount)//�൱�ڲ���ѭ���������е�����
			j=0;		
	}
}


