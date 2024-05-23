#include "./BSP/R9/underpanControl.h"
#include "math.h"
#include "stdio.h"
#include "./BSP/R9/brake.h"
#include "./BSP/BEEP/beep.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/Common/common.h"
#include "./BSP/R9/Slavemodbus.h"
#include "./BSP/R9/getadcdata.h"

VELOCITY_POUT velocity_pout;
uint16_t brakeflage;
RunState drivestate;

void velocity_maping(VELOCITY_PIn velPlanIn)
{
	static double precess_var1, precess_var2, precess_var3, precess_var4, precess_var5;
	static double straight_k;
	static double steering_k;
	velocity_pout.underpanVelocity = velPlanIn.max_underpanVelocity * velPlanIn.adcy / (yadc_max - yadc_Dim) * KMPH_TO_MPS;/* 用于底盘运动状态判断 */																							   
	velocity_pout.steering_angle = 90 * velPlanIn.adcx / (xadc_max - xadc_Dim) * PI / 180.0;	   /* 用于底盘运动状态判断 */
	velocity_pout.steering_angle = Value_limit(-PI / 2.0, velocity_pout.steering_angle, PI / 2.0); /* 用于底盘运动状态判断*/
	/*待补充 对velPlanIn.adcx 的极小值约束*/
	if (velPlanIn.adcx == 0)
	{
		velocity_pout.theta = 1.57;
	}
	else
	{
		velocity_pout.theta = atan(velPlanIn.adcy / velPlanIn.adcx); /*theta 输出单位为弧度 范围取值 -1.57 ~ 1.57 */
	}
	precess_var1 = pow(tan(velocity_pout.theta), 2.0);						/*tan(theta)^2*/
	precess_var2 = pow(velPlanIn.set_Maximum_Strspeed * KMPH_TO_MPS, 2.0);	/*a^2*/
	precess_var3 = pow(velPlanIn.set_Maximum_Steespeed * KMPH_TO_MPS, 2.0); /*b^2*/
	precess_var4 = (1 + precess_var1) / (precess_var2 + precess_var3 * precess_var1);
	precess_var5 = pow(precess_var4, 0.5);
	velocity_pout.acceleration_coeff = velPlanIn.set_Maximum_Strspeed * KMPH_TO_MPS * velPlanIn.set_Maximum_Steespeed * KMPH_TO_MPS * precess_var5;
	/* 静止  */
	if (velocity_pout.steering_angle == 0 && velocity_pout.underpanVelocity == 0)
	{
		straight_k = 0;
		steering_k = 0;
		velocity_pout.runstate = idle;
		drivestate = idle;
		g_slaveReg[5] = 1 ;

	}
	/* 直行 */
	if (velocity_pout.steering_angle == 0 && velocity_pout.underpanVelocity != 0)
	{
		steering_k = 0; /*直行不需要 X 项参与*/
		straight_k = 1;
		if (velocity_pout.underpanVelocity > 0) /*向前直行 */
		{
			velocity_pout.runstate = forward;
			drivestate = forward;
			g_slaveReg[5] = 2 ;
		}
		else /*向后直行 */
		{
			velocity_pout.runstate = backward;
			drivestate = backward;
			g_slaveReg[5] = 3 ;
		}
	}
	/* 转向 运行 */
	if (velocity_pout.steering_angle != 0 && velocity_pout.underpanVelocity != 0)
	{
		straight_k = 1.0;
		steering_k = 1.0;

		if (velocity_pout.steering_angle > 0 && velocity_pout.underpanVelocity > 0) /*向右前转向 */
		{
			velocity_pout.runstate = front_right;
			drivestate = front_right;
			g_slaveReg[5] = 7;
		}
		if (velocity_pout.steering_angle < 0 && velocity_pout.underpanVelocity > 0) /*向左前转向 */
		{
			velocity_pout.runstate = front_left;
			drivestate = front_left;
			g_slaveReg[5] = 6;
		}
		if (velocity_pout.steering_angle > 0 && velocity_pout.underpanVelocity < 0) /*向左后转向 */
		{
			velocity_pout.runstate = back_left;
			drivestate = back_left;
			g_slaveReg[5] = 8;
		}
		if (velocity_pout.steering_angle < 0 && velocity_pout.underpanVelocity < 0) /*向右后转向 */
		{
			velocity_pout.runstate = back_right;
			drivestate = back_right;
			g_slaveReg[5] = 9;
		}
	}
	/* 原地转向  */
	if (velocity_pout.steering_angle != 0 && velocity_pout.underpanVelocity == 0)
	{
		straight_k = 0;
		steering_k = 1.0;
		/*原地右转 */
		if (velocity_pout.steering_angle > 0)
		{
			velocity_pout.runstate = turnself_right;
			drivestate = turnself_right;
			g_slaveReg[5] = 5;
		}
		/*原地左转 */
		if (velocity_pout.steering_angle < 0)
		{
			velocity_pout.runstate = turnself_left;
			drivestate = turnself_left;
			g_slaveReg[5] = 4;
		}
	}
		/*左右轮目标线速度 m/s*/
	velocity_pout.L_Velocity = velocity_pout.acceleration_coeff * (straight_k * velPlanIn.adcy / (yadc_max - yadc_Dim) + steering_k * velPlanIn.adcx / (xadc_max - xadc_Dim));
	velocity_pout.R_Velocity = velocity_pout.acceleration_coeff * (straight_k * velPlanIn.adcy / (yadc_max - yadc_Dim) - steering_k * velPlanIn.adcx / (xadc_max - xadc_Dim));
	velocity_pout.presentation_velocity = (velocity_pout.L_Velocity + velocity_pout.R_Velocity)/2.0 * MPS_TO_KMPH;
	g_slaveReg[3] = velocity_pout.presentation_velocity; // RK3588 接受车速信息KM/H

	/*左右目标轮线速度 转换为 占空比*/
	velocity_pout.L_Dutycycle = fabs(velocity_pout.L_Velocity / MPS_TO_DUTY) * 0.5 + 0.5; /*占空比大于50% 方可驱动电机启动 */
	velocity_pout.R_Dutycycle = fabs(velocity_pout.R_Velocity / MPS_TO_DUTY) * 0.5 + 0.5;
	/* 占空比约束*/
	velocity_pout.L_Dutycycle = Value_limit(0, velocity_pout.L_Dutycycle, 1);
	velocity_pout.R_Dutycycle = Value_limit(0, velocity_pout.R_Dutycycle, 1);
	/*待补充占空比曲线规划*/
	// CurveObjectType L_curve;
	// CurveObjectType R_curve;
	// L_curve.curveMode = CURVE_SPTA;
    // L_curve.startSpeed = 0.5;
	// L_curve.targetSpeed = velocity_pout.L_Dutycycle ;
	// L_curve.stepSpeed = 1.0;
	// L_curve.speedMax = 1.0;
	// L_curve.speedMin = 0;
	// L_curve.aTimes = 1;
	// L_curve.maxTimes =100 ;
	// L_curve.flexible = 2;
	// MotorVelocityCurve (&L_curve);

    // printf("L_curve.currentSpeed : %f\r\n",L_curve.currentSpeed);


	switch (drivestate)
	{
		case idle:
			velocity_pout.presentation_velocity = 0;
			velocity_pout.A_IN1 = 0;
			velocity_pout.A_IN2 = 0;
			velocity_pout.B_IN1 = 0;
			velocity_pout.B_IN2 = 0;
			break;
		case forward:
			velocity_pout.A_IN1 = 0;
			velocity_pout.A_IN2 = fabs(velocity_pout.L_Dutycycle);
			velocity_pout.B_IN1 = fabs(velocity_pout.R_Dutycycle);
			velocity_pout.B_IN2 = 0;
			break;
		case backward:
			velocity_pout.A_IN1 = (fabs(velocity_pout.L_Dutycycle) - 0.5) * 0.75 + 0.5;
			velocity_pout.A_IN2 = 0;
			velocity_pout.B_IN1 = 0;
			velocity_pout.B_IN2 = (fabs(velocity_pout.R_Dutycycle) - 0.5) * 0.75 + 0.5;
			break;
		case front_right:
			velocity_pout.A_IN1 = 0;
			velocity_pout.A_IN2 = fabs(velocity_pout.L_Dutycycle);
			velocity_pout.B_IN1 = fabs(velocity_pout.R_Dutycycle);
			velocity_pout.B_IN2 = 0;
			break;
		case front_left:
			velocity_pout.A_IN1 = 0;
			velocity_pout.A_IN2 = fabs(velocity_pout.L_Dutycycle);
			velocity_pout.B_IN1 = fabs(velocity_pout.R_Dutycycle);
			velocity_pout.B_IN2 = 0;
			break;
		case back_left:
			velocity_pout.A_IN1 = (fabs(velocity_pout.R_Dutycycle) - 0.5) * 0.75 + 0.5;
			velocity_pout.A_IN2 = 0;
			velocity_pout.B_IN1 = 0;
			velocity_pout.B_IN2 = (fabs(velocity_pout.L_Dutycycle) - 0.5) * 0.75 + 0.5;
			break;
		case back_right:
			velocity_pout.A_IN1 = (fabs(velocity_pout.R_Dutycycle) - 0.5) * 0.75 + 0.5;
			velocity_pout.A_IN2 = 0;
			velocity_pout.B_IN1 = 0;
			velocity_pout.B_IN2 = (fabs(velocity_pout.L_Dutycycle) - 0.5) * 0.75 + 0.5;
			break;
		case turnself_right:
			velocity_pout.A_IN1 = 0;
			velocity_pout.A_IN2 = velocity_pout.L_Dutycycle;
			velocity_pout.B_IN1 = 0;
			velocity_pout.B_IN2 = velocity_pout.R_Dutycycle;
			break;
		case turnself_left:
			velocity_pout.A_IN1 = velocity_pout.L_Dutycycle;
			velocity_pout.A_IN2 = 0;
			velocity_pout.B_IN1 = velocity_pout.R_Dutycycle;
			velocity_pout.B_IN2 = 0;
			break;
		default:
			break;
	}

	if (velocity_pout.L_Dutycycle <= 0.5 && velocity_pout.R_Dutycycle <= 0.5)

	{
		brakeflage++;
		if (brakeflage > 600)
		{ // 抱住
			BRAKE1(1);
			BRAKE2(1);
			brakeflage = 0;
		}
	}
	if (velPlanIn.adcx < -100 || velPlanIn.adcx > 100 || velPlanIn.adcy > 100 || velPlanIn.adcy < -100)

	{
		// 松开
		BRAKE1(0);
		BRAKE2(0);
	}
}

void (*pCalCurve[])(CurveObjectType *curve)={CalCurveTRAP,CalCurveSPTA};

/* 电机曲线加减速操作-------------------------------------------------------- */
void MotorVelocityCurve(CurveObjectType *curve)
{
	 float temp=0;
	 
	 if(curve->targetSpeed>curve->speedMax)
	{
	   curve->targetSpeed=curve->speedMax;
	}
	 
	 if(curve->targetSpeed<curve->speedMin)
	{
	   curve->targetSpeed=curve->speedMin;
	}

	 if((fabs(curve->currentSpeed-curve->startSpeed)<=curve->stepSpeed)&&(curve->maxTimes==0))
	{
	   if(curve->startSpeed<curve->speedMin)
	  {
		 curve->startSpeed=curve->speedMin;
	  }
	   
	   temp=fabs(curve->targetSpeed-curve->startSpeed);
	   temp=temp/curve->stepSpeed;
	   curve->maxTimes=(uint32_t)(temp)+1;
	   curve->aTimes=0;
	}
	 
	 if(curve->aTimes<curve->maxTimes)
	{
	   pCalCurve[curve->curveMode](curve);
	   curve->aTimes++;
	}
	 else
	{
	   curve->currentSpeed=curve->targetSpeed;
	   curve->maxTimes=0;
	   curve->aTimes=0;
	}
}

/*梯形曲线速度计算*/
static void CalCurveTRAP(CurveObjectType *trap)
{
	 float slope=0.0;
	 
	 slope=(trap->targetSpeed-trap->startSpeed)/trap->maxTimes;
	   
	 trap->currentSpeed=trap->startSpeed+slope*trap->aTimes;

	 if(trap->currentSpeed>trap->speedMax)
	{
	   trap->currentSpeed=trap->speedMax;
	}
	 
	 if(trap->currentSpeed<trap->speedMin)
	{
	   trap->currentSpeed=trap->speedMin;
	}
}
/*S型曲线速度计算*/
static void CalCurveSPTA(CurveObjectType *spta)
{
	 float power=0.0;
	 float speed=0.0;
	 
	 power=(2*((float)spta->aTimes)-((float)spta->maxTimes))/((float)spta->maxTimes);
	 power=(0.0-spta->flexible)*power;
	 
	 speed=1+expf(power);
	 speed=(spta->targetSpeed-spta->startSpeed)/speed;
	 spta->currentSpeed=speed+spta->startSpeed;
	 
	 if(spta->currentSpeed>spta->speedMax)
	{
		spta->currentSpeed=spta->speedMax;
	}
	 
	 if(spta->currentSpeed<spta->speedMin)
	{
		spta->currentSpeed=spta->speedMin;
	}
}
