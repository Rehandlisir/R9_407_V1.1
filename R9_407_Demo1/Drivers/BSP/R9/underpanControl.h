#ifndef __UNDERPANCONTROL_H
#define __UNDERPANCONTROL_H

#include "./SYSTEM/sys/sys.h"

/* ???? */
#define underpan_H 0.55 /*???*/

#define pi 3.1415926 /*????PI*/

#define GEAR_RATIO 29.5		 /* 减速比 */
#define Diameter 0.354		 /* 轮子直径354mm  */
#define MoterMaxrN 175.0	 /*输出轴额定转速 175PRM */
#define KMPH_TO_RPM    15.0  /*1km/h 约 15RPM*/
#define VelocityConst  7.3   /*电机速率常数 单位 RPM/V*/
#define KMPH_TO_Voltage 2.055 /*V/KMPH*/
#define KMPH_TO_Duty 0.0856  /*1km/h 占空比约 8.56%*/

/*   */

typedef enum
{
	 idle = 0,
	 forward,
	 backward,
	 front_left,
	 front_right,
	 back_left,
	 back_right,
	 turnself_left,
	 turnself_right
} RunState;

typedef struct /**/
{
	int16_t adcx;
	int16_t adcy;
	double max_underpanVelocity;
	double set_Maximum_Strspeed;
	double set_Maximum_Steespeed;

} VELOCITY_PIn;

typedef struct /*??????????*/
{
	double underpanVelocity;	  
	double presentation_velocity; 
	double theta;				  
	double acceleration_coeff;	  
	double steering_angle;		  
	double L_Velocity;			  
	double R_Velocity;			  
	double L_NVelocity;			  
	double R_NVelocity;			
	double L_Dutycycle;			  
	double R_Dutycycle;			 

	double A_IN1;	   
	double A_IN2;	  
	double B_IN1;	  
	double B_IN2;	   
	RunState runstate; 
} VELOCITY_POUT;


/*  */
typedef enum SpeedCurve
{
	CURVE_NONE = 0, // ֱ��
	CURVE_TRAP = 1, // ��������
	CURVE_SPTA = 2	// S������
} SpeedCurveType;

/* �������ٶ����߶��� */
typedef struct CurveObject
{
	float startSpeed;		  // ��ʼ����ʱ�ĳ�ʼ�ٶ�
	float currentSpeed;		  // ��ǰ�ٶ�
	float targetSpeed;		  // Ŀ���ٶ�
	float stepSpeed;		  // ���ٶ�
	float speedMax;			  // ����ٶ�
	float speedMin;			  // ��С�ٶ�
	uint32_t aTimes;		  // ����ʱ��
	uint32_t maxTimes;		  // ���ٿ��
	SpeedCurveType curveMode; // ��������
	float flexible;			  // S���������
} CurveObjectType;

int32_t Value_Resetzero(int32_t min_value, int32_t current_value, int32_t max_value);
void velocity_plan(VELOCITY_PIn);
void velocity_maping(VELOCITY_PIn velPlanIn);
void underpanExcute(void);
void MPU6050Excute(void);
/*以下待测试函数*/
void MotorVelocityCurve(CurveObjectType *curve);
static void CalCurveTRAP(CurveObjectType *trap);
static void CalCurveSPTA(CurveObjectType *spta);

#endif
