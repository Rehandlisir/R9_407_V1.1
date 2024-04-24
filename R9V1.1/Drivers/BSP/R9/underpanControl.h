#ifndef __UNDERPANCONTROL_H
#define __UNDERPANCONTROL_H

#include "./SYSTEM/sys/sys.h"


/* �������� */
#define underpan_H    0.55   

#define PI            3.1415926

#define GEAR_RATIO		16.75               /* ���ٱ�  */
#define Diameter        0.270                 /* ����ֱ��  */
#define MoterMaxr      2200.0                /*���ת�� r/min  */

/* ���ƽṹ��  */

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
}RunState;

typedef struct /*�ٶȹ滮����*/
{
 int16_t adcx;
 int16_t adcy;
 float max_underpanVelocity;
 double  k;
}VELOCITY_PIn;

typedef struct /* �ٶȹ滮���*/
{
	double underpanVelocity ; /*�滮ʵʱ �ٶ� ��λ m/s */
	double presentation_velocity; /* ��λ����ʾ�ٶ� m/s*/

	double steering_angle;
	double L_Velocity ;      /* �������ٶ� */
	double R_Velocity ;      /* �������ٶ�*/
	double L_NVelocity;      /* ���ֵ��ת��*/
	double R_NVelocity;      /* ���ֵ��ת��*/
    double L_Dutycycle;      /* ����ռ�ձ�*/
	double R_Dutycycle;      /* ����ռ�ձ� */

	double A_IN1       ;    /*����ռ�ձ�������ƶ˿�1 */
	double A_IN2       ;    /*����ռ�ձ�������ƶ˿�2 */
	double B_IN1       ;    /*�ҵ��ռ�ձ�������ƶ˿�1 */
	double B_IN2       ;    /*�ҵ��ռ�ձ�������ƶ˿�2 */
	RunState runstate  ;
	
	
	
} VELOCITY_POUT;


int32_t  Value_Resetzero(int32_t min_value ,int32_t current_value ,int32_t max_value);
void velocity_plan(VELOCITY_PIn);	

#endif

