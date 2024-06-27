/**
 * @FilePath     : /MDK-ARMc:/Users/fu/Desktop/Code/CodeV1.1/R9_407_V1.1/R9_407_Demo1/Drivers/BSP/R9/WheelSpeedMap.c
 * @Description  :  
 * @Author       : lisir
 * @Version      : V1.1
 * @LastEditors  : lisir lisir@rehand.com
 * @LastEditTime : 2024-06-27 14:36:47
 * @Copyright (c) 2024 by Rehand Medical Technology Co., LTD, All Rights Reserved. 
**/
#include "./BSP/R9/underpanControl.h"
#include "math.h"
#include "stdio.h"
#include "./BSP/R9/brake.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/Common/common.h"
#include "./BSP/R9/Slavemodbus.h"
#include "./BSP/R9/getadcdata.h"
#include "./BSP/R9/moterdriver.h"
#include "./BSP/R9/mpu6050.h"
#include "./BSP/R9/inv_mpu.h"
#include "./BSP/R9/inv_mpu_dmp_motion_driver.h" 
VELOCITY_POUT velocity_pout; 
VELOCITY_PIn  velPlanIn1;
uint16_t brakeflage;
RunState drivestate;
RunState e_lastdrivestate;
static float pitch,roll,yaw;
#define whellspeedmapnew
/**
 * @description: 摇杆数据映射左右轮速方法
 * @param  set_Maximum_Strspeed ：最大直行速度  
 * @param  set_Maximum_Steespeed ：最大转向速度  
 * @return {*}
 */
#ifdef whellspeeedmap
void velocity_maping(VELOCITY_PIn velPlanIn)
{
	static double precess_var1, precess_var2, precess_var3, precess_var4, precess_var5;
	static double straight_k;
	static double steering_k;
	velocity_pout.underpanVelocity = velPlanIn.set_Maximum_Strspeed * velPlanIn.adcy / (yadc_max - yadc_Dim);/* 用于底盘运动状态判断 */																							   
	velocity_pout.steering_angle = 90 * velPlanIn.adcx / (xadc_max - xadc_Dim) * PI / 180.0;	   /* 用于底盘运动状态判断 */
	velocity_pout.steering_angle = Value_limitf(-PI / 2.0, velocity_pout.steering_angle, PI / 2.0); /* 用于底盘运动状态判断*/
	/*上位机显示 摇杆数据*/
	g_slaveReg[10] =  velPlanIn.adcx;
    g_slaveReg[11] =  velPlanIn.adcy;
    /*本地摇杆操控轮椅状态*/
	if (velPlanIn.adcx!=0 || velPlanIn.adcy!=0)
	{
		g_slaveReg[22] = 1;
	}
	else
	{
		g_slaveReg[22] = 0;
	}

	/*待补充 对velPlanIn.adcx 的极小值约束*/
	if (velPlanIn.adcx == 0)
	{
		velocity_pout.theta = 1.57;
	}
	else
	{
		velocity_pout.theta = atan((double)(velPlanIn.adcy) / (double)(velPlanIn.adcx)); /*theta 输出单位为弧度 范围取值 -1.57 ~ 1.57 */
	}
	precess_var1 = pow(tan(velocity_pout.theta), 2.0);						/*tan(theta)^2*/
	precess_var2 = pow(velPlanIn.set_Maximum_Strspeed, 2.0);	/*a^2*/
	precess_var3 = pow(velPlanIn.set_Maximum_Steespeed, 2.0); /*b^2*/
	precess_var4 = (1 + precess_var1) / (precess_var2 + precess_var3 * precess_var1);
	precess_var5 = sqrt(precess_var4);
	velocity_pout.acceleration_coeff = velPlanIn.set_Maximum_Strspeed * velPlanIn.set_Maximum_Steespeed * precess_var5;

	/*左右轮目标线速度 Km/h*/
	velocity_pout.L_Velocity = velocity_pout.acceleration_coeff * (straight_k * velPlanIn.adcy / (yadc_max - yadc_Dim) + steering_k * velPlanIn.adcx / (xadc_max - xadc_Dim));
	velocity_pout.R_Velocity = velocity_pout.acceleration_coeff * (straight_k * velPlanIn.adcy / (yadc_max - yadc_Dim) - steering_k * velPlanIn.adcx / (xadc_max - xadc_Dim));
	/*线速度约束*/
	velocity_pout.L_Velocity = Value_limitf(-velPlanIn.set_Maximum_Strspeed,velocity_pout.L_Velocity,velPlanIn.set_Maximum_Strspeed);
	velocity_pout.R_Velocity = Value_limitf(-velPlanIn.set_Maximum_Strspeed,velocity_pout.R_Velocity,velPlanIn.set_Maximum_Strspeed);
  if (drivestate != backward)
	{
		velocity_pout.presentation_velocity = (fabs(velocity_pout.L_Velocity) + fabs(velocity_pout.R_Velocity))/2.0;
	}
	else
	{
		velocity_pout.presentation_velocity = (fabs(velocity_pout.L_Velocity) + fabs(velocity_pout.R_Velocity))/4.0;
	}
		
		g_slaveReg[3] = velocity_pout.presentation_velocity * 100; // RK3588 接受车速信息KM/H
      /*补充PID 左右轮速闭环调节*/
	/*KM/h —— RPM—— Voltage - Duty cycle*/
    
	/*左右目标轮线速度 转换为 占空比*/
	velocity_pout.L_Dutycycle = fabs(velocity_pout.L_Velocity * KMPH_TO_Duty);//* 0.5 + 0.5; /*占空比大于50% 方可驱动电机启动 */
	velocity_pout.R_Dutycycle = fabs(velocity_pout.R_Velocity * KMPH_TO_Duty);// * 0.5 + 0.5;
	/*算术平均滤波占空比滤波处理*/
    velocity_pout.L_Dutycycle = filterValue(&filter_L,velocity_pout.L_Dutycycle);
	velocity_pout.R_Dutycycle = filterValue(&filter_R,velocity_pout.R_Dutycycle);

	/* 占空比约束*/
	velocity_pout.L_Dutycycle = Value_limitf(0, velocity_pout.L_Dutycycle, 1);
	velocity_pout.R_Dutycycle = Value_limitf(0, velocity_pout.R_Dutycycle, 1);	
	printf("%lf,%lf,%f\r\n",velocity_pout.acceleration_coeff,velocity_pout.L_Dutycycle,velocity_pout.R_Dutycycle);//,g_slaveReg[5]);	
	/*待补充占空比曲线规划*/
	   
	e_lastdrivestate = idle ; /*上一次操控方向记录 */
	if (drivestate!=e_lastdrivestate)
	{
		e_lastdrivestate = drivestate;
	}
    
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

	switch (drivestate)
	{
		case idle:
			LeftMoterStop();
			RightMoterStop();
			break;
		case forward:
			LeftMoterMove(1,velocity_pout.L_Dutycycle);
			RightMoterMove(0,velocity_pout.R_Dutycycle);
			break;
		case front_right:
			LeftMoterMove(1,velocity_pout.L_Dutycycle);
			RightMoterMove(0,velocity_pout.R_Dutycycle);
			break;
		case front_left:
			LeftMoterMove(1,velocity_pout.L_Dutycycle);
			RightMoterMove(0,velocity_pout.R_Dutycycle);
			break;
		case backward : /*backward velocity is half of set_Maximum_Strspeed*/
			LeftMoterMove(0,velocity_pout.L_Dutycycle);
			RightMoterMove(1,velocity_pout.R_Dutycycle);
			break;
		case back_left:
			LeftMoterMove(0,velocity_pout.L_Dutycycle);
			RightMoterMove(1,velocity_pout.R_Dutycycle);
			break;
		case back_right:
			LeftMoterMove(0,velocity_pout.L_Dutycycle);
			RightMoterMove(1,velocity_pout.R_Dutycycle);
			break;
		case turnself_right:
			LeftMoterStop();
			RightMoterMove(1,velocity_pout.R_Dutycycle);
			break;
		case turnself_left:
			LeftMoterMove(0,velocity_pout.L_Dutycycle);
			RightMoterStop();
			break;
		default:
			break;
	}

	if (velocity_pout.L_Dutycycle <= 0.5 && velocity_pout.R_Dutycycle <= 0.5)
	{
		brakeflage++;
		if (brakeflage > 250)
		{ // 抱住
			brake(1);
			brakeflage = 0;
		}
	}
	else
	{
		brakeflage = 0;
	}

	if (velPlanIn.adcx < -50 || velPlanIn.adcx > 50 || velPlanIn.adcy > 50 || velPlanIn.adcy < -50)

	{
		// 松开
			brake(0);
	}
	// printf("adcx:%d,adcy:%d\r\n",velPlanIn.adcx,velPlanIn.adcy);//,velocity_pout.L_Dutycycle,velocity_pout.R_Dutycycle);
}

/**
 * @description: 获取陀螺仪 的pry欧拉角
 * @return 空 rpy欧拉角数值放置静态变量 pitch roll yaw 中
 */
void MPU6050Excute(void)
{

	//	uint8_t res;
    //	Temperature =  MPU_Get_Temperature();
	//	MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);
	//	MPU_Get_Accelerometer(&aacx,&aacy,&aacz);
	//    printf("%d,%d,%d,%d,%d,%d,%d\n",Temperature,gyrox,gyroy,gyroz,aacx,aacy,aacz);
	//	res=MPU_Read_Byte(MPU_DEVICE_ID_REG);
	//	printf("%d\n",res);
    
	mpu_dmp_get_data(&pitch,&roll,&yaw);       
    //printf("roll:%f,pitch:%f,yaw:%f\t\n",roll,pitch,yaw); 

}

/**
 * @description: 底盘驱动程序代码 主要用到定时器 3 和 定时器 9 控制左右电机占空比
 * @return 无
 */
void underpanExcute(void)
{
    switch (g_slaveReg[64])
	{
		case 1: /*normal mode*/
			if (g_slaveReg[73] == 1)/*one level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 1.0 ;
				velPlanIn1.set_Maximum_Steespeed = 1.0 ;
			}
			else if (g_slaveReg[73] == 2)/*two level*/
	   	    {
				velPlanIn1.set_Maximum_Strspeed = 2.0 ;
				velPlanIn1.set_Maximum_Steespeed = 2.0 ;
			}
			else if (g_slaveReg[73] == 3)/*three level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 3.0 ;
				velPlanIn1.set_Maximum_Steespeed = 3.0 ;
			}
			else if (g_slaveReg[73] == 4)/*four level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 4.0 ;
				velPlanIn1.set_Maximum_Steespeed = 4.0 ;
			}
			else if (g_slaveReg[73] == 5)/*five level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 5.0 ;
				velPlanIn1.set_Maximum_Steespeed = 5.0 ;
			}
			else
			{
				velPlanIn1.set_Maximum_Strspeed = 1.0 ;
				velPlanIn1.set_Maximum_Steespeed = 1.0 ;	
			}
			break;
		case 2 :/*indoor mode*/
			if (g_slaveReg[73] == 1)/*one level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 1.0 ;
				velPlanIn1.set_Maximum_Steespeed = 1.0 ;
			}
			else if (g_slaveReg[73] == 2)/*two level*/
	   	    {
				velPlanIn1.set_Maximum_Strspeed = 2.0 ;
				velPlanIn1.set_Maximum_Steespeed = 2.0 ;
			}
			else if (g_slaveReg[73] == 3)/*three level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 3.0 ;
				velPlanIn1.set_Maximum_Steespeed = 3.0 ;
			}
			else if (g_slaveReg[73] == 4)/*four level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 4.0 ;
				velPlanIn1.set_Maximum_Steespeed = 4.0 ;
			}
			else if (g_slaveReg[73] == 5)/*five level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 5.0 ;
				velPlanIn1.set_Maximum_Steespeed = 5.0 ;
			}
			else
			{
				velPlanIn1.set_Maximum_Strspeed = 1.0 ;
				velPlanIn1.set_Maximum_Steespeed = 1.0 ;	
			}
			break;			
		case 3 :/*outdoor mode*/
			if (g_slaveReg[73] == 1)/*one level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 1.0 ;
				velPlanIn1.set_Maximum_Steespeed = 1.0 ;
			}
			else if (g_slaveReg[73] == 2)/*two level*/
	   	    {
				velPlanIn1.set_Maximum_Strspeed = 4.0 ;
				velPlanIn1.set_Maximum_Steespeed = 4.0 ;
			}
			else if (g_slaveReg[73] == 3)/*three level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 6.0 ;
				velPlanIn1.set_Maximum_Steespeed = 6.0 ;
			}
			else if (g_slaveReg[73] == 4)/*four level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 9.0 ;
				velPlanIn1.set_Maximum_Steespeed = 9.0 ;
			}
			else if (g_slaveReg[73] == 5)/*five level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 12.0 ;
				velPlanIn1.set_Maximum_Steespeed = 12.0 ;
			}
			else
			{
				velPlanIn1.set_Maximum_Strspeed = 1.0 ;
				velPlanIn1.set_Maximum_Steespeed = 1.0 ;	
			}
			break;
		default:
				velPlanIn1.set_Maximum_Strspeed = 12.0 ;
				velPlanIn1.set_Maximum_Steespeed = 6.0 ;
			break;
	}
	/* X 数据清偏 */
	if (adcdata.adc_x > 0)
	{
		velPlanIn1.adcx = adcdata.adc_x - xadc_Dim;
	}
	else if (adcdata.adc_x < 0)
	{
		velPlanIn1.adcx = adcdata.adc_x + xadc_Dim;
	}
	else
	{
		velPlanIn1.adcx = 0;
	}
	
	velPlanIn1.adcx = slopelimitx( velPlanIn1.adcx,25);  
	
	/* Y 数据清偏移*/
	if (adcdata.adc_y > 0)	
	{
		velPlanIn1.adcy = adcdata.adc_y - yadc_Dim;
	}
    else if (adcdata.adc_y < 0)
	{
		velPlanIn1.adcy = adcdata.adc_y+ yadc_Dim;
	}	
	else
	{
		velPlanIn1.adcy = 0 ;
	}
	velPlanIn1.adcy = slopelimity( velPlanIn1.adcy,25); 

	velocity_maping(velPlanIn1); /*速度规划 */	
}

/**
 * @description: 电机曲线加减速操作
 * @param {CurveObjectType} *curve
 * @return {*}
 */

void (*pCalCurve[])(CurveObjectType *curve)={CalCurveTRAP,CalCurveSPTA};

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


/**
 * @description: 电机曲线加减速操作
 * @param {CurveObjectType} *curve
 * @return {*}
 */
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
#endif
#ifdef whellspeedmapnew
/**
 * @brief        : 
 * @param         {VELOCITY_PIn} velPlanIn:steering_angle: 0~2pi
 * @return        {*} 
**/
void velocity_maping(VELOCITY_PIn velPlanIn)
{
	/*上位机显示 摇杆数据*/
	  g_slaveReg[10] =  velPlanIn.adcx;
    g_slaveReg[11] =  velPlanIn.adcy;
    /*本地摇杆操控轮椅状态*/
	if (velPlanIn.adcx!=0 || velPlanIn.adcy!=0)
	{
		g_slaveReg[22] = 1;
	}
	else
	{
		g_slaveReg[22] = 0;
	}
	/*转向角计算*/
    velocity_pout.steering_angle = atan2((double)velPlanIn.adcy, (double)velPlanIn.adcx);
    if (velPlanIn.adcy < 0)
		{
		velocity_pout.steering_angle = velocity_pout.steering_angle +2*pi;
		}
    /*速度模长*/
    velocity_pout.underpanVelocity = sqrt(pow(velPlanIn.adcx,2.0)+pow(velPlanIn.adcy,2.0));
	
	/*左右轮目标线速度 Km/h*/
	velocity_pout.L_Velocity = velPlanIn.set_Maximum_Strspeed * velocity_pout.underpanVelocity/ (yadc_max - yadc_Dim) * \
	(sin(velocity_pout.steering_angle-pi/6.0) + cos(velocity_pout.steering_angle-pi/6.0)) / 1.0 ;

	velocity_pout.R_Velocity = velPlanIn.set_Maximum_Strspeed * velocity_pout.underpanVelocity/ (yadc_max - yadc_Dim) * \
	(sin(velocity_pout.steering_angle+pi/6.0) - cos(velocity_pout.steering_angle+pi/6.0)) / 1.0 ;
  
	velocity_pout.L_Velocity = Value_limitf(-velPlanIn.set_Maximum_Strspeed,velocity_pout.L_Velocity,velPlanIn.set_Maximum_Strspeed);
	velocity_pout.R_Velocity = Value_limitf(-velPlanIn.set_Maximum_Strspeed,velocity_pout.R_Velocity,velPlanIn.set_Maximum_Strspeed);
	
  if (drivestate != backward)
	{
		velocity_pout.presentation_velocity = (fabs(velocity_pout.L_Velocity) + fabs(velocity_pout.R_Velocity))/2.0;
	}
	else
	{
		velocity_pout.presentation_velocity = (fabs(velocity_pout.L_Velocity) + fabs(velocity_pout.R_Velocity))/4.0;
	}
		
		g_slaveReg[3] = velocity_pout.presentation_velocity * 100; // RK3588 接受车速信息KM/H

	/*KM/h —— RPM—— Voltage - Duty cycle*/
    
	/*左右目标轮线速度 转换为 占空比*/
	velocity_pout.L_Dutycycle = fabs(velocity_pout.L_Velocity) * KMPH_TO_Duty;// * 0.5 + 0.5; /*占空比大于50% 方可驱动电机启动 */
	velocity_pout.R_Dutycycle = fabs(velocity_pout.R_Velocity) * KMPH_TO_Duty;// * 0.5 + 0.5;
	/*算术平均滤波占空比滤波处理*/
    velocity_pout.L_Dutycycle = filterValue(&filter_L,velocity_pout.L_Dutycycle);
	velocity_pout.R_Dutycycle = filterValue(&filter_R,velocity_pout.R_Dutycycle);

	/* 占空比约束*/
	if (fabs(velocity_pout.L_Dutycycle) < 0.51)
	{
		velocity_pout.L_Dutycycle = 0;
		
	}
	
	if (fabs(velocity_pout.R_Dutycycle) < 0.51)
	{
		velocity_pout.R_Dutycycle = 0;
	}
	velocity_pout.L_Dutycycle = slopelimitLDuty(velocity_pout.L_Dutycycle,0.08,0.2);
	velocity_pout.R_Dutycycle = slopelimitRDuty(velocity_pout.R_Dutycycle,0.08,0.2);
	
//	velocity_pout.L_Dutycycle = fabs(velocity_pout.L_Dutycycle);
//	velocity_pout.R_Dutycycle = fabs(velocity_pout.R_Dutycycle);
	
	velocity_pout.L_Dutycycle = Value_limitf(0, velocity_pout.L_Dutycycle, 1);
	velocity_pout.R_Dutycycle = Value_limitf(0, velocity_pout.R_Dutycycle, 1);	
	
	
	
	/* 静止  */
	if (velPlanIn.adcx == 0 && velPlanIn.adcy  == 0)
	{
		velocity_pout.runstate = idle;
		drivestate = idle;
		g_slaveReg[5] = 1 ;

	}
	/*前直行 */
	if (velPlanIn.adcx == 0 && velPlanIn.adcy > 0)
	{

		velocity_pout.runstate = forward;
		drivestate = forward;
		g_slaveReg[5] = 2 ;
	}
	/*向后直行 */
	if (velPlanIn.adcx == 0 && velPlanIn.adcy < 0)
	{
		velocity_pout.runstate = backward;
		drivestate = backward;
		g_slaveReg[5] = 3 ;
	}
	
	/*向左前转向 */
//	if (velPlanIn.adcx < 0 && velPlanIn.adcy > 0)
	if (velocity_pout.steering_angle > pi/2 && velocity_pout.steering_angle <(11/12.0)*pi && velPlanIn.adcx!=0 )
	{
		velocity_pout.runstate = front_left;
		drivestate = front_left;
		g_slaveReg[5] = 6;
	}
	/*向右前转向 */
	if (velocity_pout.steering_angle > pi*1.0/12.0 && velocity_pout.steering_angle <1/2.0 *pi && velPlanIn.adcx!=0)
	{
		velocity_pout.runstate = front_right;
		drivestate = front_right;
		g_slaveReg[5] = 7;
	}
	/*向左后转向 */
	if (velocity_pout.steering_angle > 1.5*pi  && velocity_pout.steering_angle <23/12.0 *pi && velPlanIn.adcx!=0)
	{
		velocity_pout.runstate = back_left;
		drivestate = back_left;
		g_slaveReg[5] = 8;
	}
	/*向右后转向 */
if (velocity_pout.steering_angle > 11/12.0 *pi  && velocity_pout.steering_angle <1.5 *pi && velPlanIn.adcx!=0 )
	{
		velocity_pout.runstate = back_right;
		drivestate = back_right;
		g_slaveReg[5] = 9;
	}
	
	/*原地右转 */
if ((velocity_pout.steering_angle >=0 &&  velPlanIn.adcx!=0 && velocity_pout.steering_angle < 1/12.0 *pi) \
	|| (velocity_pout.steering_angle >23/12.0*pi  && velocity_pout.steering_angle <2 *pi))	
	{
		velocity_pout.runstate = turnself_right;
		drivestate = turnself_right;
		g_slaveReg[5] = 5;
	}

	/*原地左转 */
if (velocity_pout.steering_angle >11/12.0*pi && velocity_pout.steering_angle < 13/12.0 *pi)	
	{
		velocity_pout.runstate = turnself_left;
		drivestate = turnself_left;
		g_slaveReg[5] = 4;
	}
	//printf("L:%lf,R:%lf,ADCX:%d,ADCY:%d\r\n",velocity_pout.L_Dutycycle,velocity_pout.R_Dutycycle,velPlanIn.adcx,velPlanIn.adcy);	
	switch (drivestate)
	{
		case idle:
			LeftMoterStop();
			RightMoterStop();
			break;
		case forward:
			LeftMoterMove(1,velocity_pout.L_Dutycycle);
			RightMoterMove(0,velocity_pout.R_Dutycycle);
			break;
		case front_right:
			LeftMoterMove(1,velocity_pout.L_Dutycycle);
			RightMoterMove(0,velocity_pout.R_Dutycycle);
			break;
		case front_left:
			LeftMoterMove(1,velocity_pout.L_Dutycycle);
			RightMoterMove(0,velocity_pout.R_Dutycycle);
			break;
		case backward : /*backward velocity is half of set_Maximum_Strspeed*/
			LeftMoterMove(0,velocity_pout.L_Dutycycle);
			RightMoterMove(1,velocity_pout.R_Dutycycle);
			break;
		case back_left:
			LeftMoterMove(0,velocity_pout.L_Dutycycle);
			RightMoterMove(1,velocity_pout.R_Dutycycle);
			break;
		case back_right:
			LeftMoterMove(0,velocity_pout.L_Dutycycle);
			RightMoterMove(1,velocity_pout.R_Dutycycle);
			break;
		case turnself_right:
			LeftMoterMove(1,velocity_pout.L_Dutycycle);
			RightMoterMove(1,velocity_pout.R_Dutycycle);
			break;
		case turnself_left:
			LeftMoterMove(0,velocity_pout.L_Dutycycle);
			RightMoterMove(0,velocity_pout.R_Dutycycle);
			break;
		default:
			break;
	}

	if (velocity_pout.L_Dutycycle <= 0 && velocity_pout.R_Dutycycle <= 0)
	{
		brakeflage++;
		if (brakeflage > 250)
		{ // 抱住
			brake(1);
			brakeflage = 0;
		}
	}
	else
	{
		brakeflage = 0;
	}

	if (velPlanIn.adcx < -50 || velPlanIn.adcx > 50 || velPlanIn.adcy > 50 || velPlanIn.adcy < -50)

	{
			brake(0);
	}
	// printf("adcx:%d,adcy:%d\r\n",velPlanIn.adcx,velPlanIn.adcy);//,velocity_pout.L_Dutycycle,velocity_pout.R_Dutycycle);
}

/**
 * @description: 获取陀螺仪 的pry欧拉角
 * @return 空 rpy欧拉角数值放置静态变量 pitch roll yaw 中
 */
void MPU6050Excute(void)
{

	//	uint8_t res;
    //	Temperature =  MPU_Get_Temperature();
	//	MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);
	//	MPU_Get_Accelerometer(&aacx,&aacy,&aacz);
	//    printf("%d,%d,%d,%d,%d,%d,%d\n",Temperature,gyrox,gyroy,gyroz,aacx,aacy,aacz);
	//	res=MPU_Read_Byte(MPU_DEVICE_ID_REG);
	//	printf("%d\n",res);
    
	mpu_dmp_get_data(&pitch,&roll,&yaw);       
    //printf("roll:%f,pitch:%f,yaw:%f\t\n",roll,pitch,yaw); 

}

/**
 * @description: 底盘驱动程序代码 主要用到定时器 3 和 定时器 9 控制左右电机占空比
 * @return 无
 */
void underpanExcute(void)
{
    switch (g_slaveReg[64])
	{
		case 1: /*normal mode*/
			if (g_slaveReg[73] == 1)/*one level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 1.0 ;
				velPlanIn1.set_Maximum_Steespeed = 1.0 ;
			}
			else if (g_slaveReg[73] == 2)/*two level*/
	   	    {
				velPlanIn1.set_Maximum_Strspeed = 2.0 ;
				velPlanIn1.set_Maximum_Steespeed = 2.0 ;
			}
			else if (g_slaveReg[73] == 3)/*three level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 3.0 ;
				velPlanIn1.set_Maximum_Steespeed = 3.0 ;
			}
			else if (g_slaveReg[73] == 4)/*four level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 4.0 ;
				velPlanIn1.set_Maximum_Steespeed = 4.0 ;
			}
			else if (g_slaveReg[73] == 5)/*five level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 5.0 ;
				velPlanIn1.set_Maximum_Steespeed = 5.0 ;
			}
			else
			{
				velPlanIn1.set_Maximum_Strspeed = 1.0 ;
				velPlanIn1.set_Maximum_Steespeed = 1.0 ;	
			}
			break;
		case 2 :/*indoor mode*/
			if (g_slaveReg[73] == 1)/*one level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 1.0 ;
				velPlanIn1.set_Maximum_Steespeed = 1.0 ;
			}
			else if (g_slaveReg[73] == 2)/*two level*/
	   	    {
				velPlanIn1.set_Maximum_Strspeed = 2.0 ;
				velPlanIn1.set_Maximum_Steespeed = 2.0 ;
			}
			else if (g_slaveReg[73] == 3)/*three level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 3.0 ;
				velPlanIn1.set_Maximum_Steespeed = 3.0 ;
			}
			else if (g_slaveReg[73] == 4)/*four level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 4.0 ;
				velPlanIn1.set_Maximum_Steespeed = 4.0 ;
			}
			else if (g_slaveReg[73] == 5)/*five level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 5.0 ;
				velPlanIn1.set_Maximum_Steespeed = 5.0 ;
			}
			else
			{
				velPlanIn1.set_Maximum_Strspeed = 1.0 ;
				velPlanIn1.set_Maximum_Steespeed = 1.0 ;	
			}
			break;			
		case 3 :/*outdoor mode*/
			if (g_slaveReg[73] == 1)/*one level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 1.0 ;
				velPlanIn1.set_Maximum_Steespeed = 1.0 ;
			}
			else if (g_slaveReg[73] == 2)/*two level*/
	   	    {
				velPlanIn1.set_Maximum_Strspeed = 4.0 ;
				velPlanIn1.set_Maximum_Steespeed = 4.0 ;
			}
			else if (g_slaveReg[73] == 3)/*three level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 6.0 ;
				velPlanIn1.set_Maximum_Steespeed = 6.0 ;
			}
			else if (g_slaveReg[73] == 4)/*four level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 9.0 ;
				velPlanIn1.set_Maximum_Steespeed = 9.0 ;
			}
			else if (g_slaveReg[73] == 5)/*five level*/
			{
				velPlanIn1.set_Maximum_Strspeed = 12.0 ;
				velPlanIn1.set_Maximum_Steespeed = 12.0 ;
			}
			else
			{
				velPlanIn1.set_Maximum_Strspeed = 1.0 ;
				velPlanIn1.set_Maximum_Steespeed = 1.0 ;	
			}
			break;
		default:
				velPlanIn1.set_Maximum_Strspeed = 4.0 ;
				velPlanIn1.set_Maximum_Steespeed = 6.0 ;
			break;
	}
	/* X 数据清偏 */
	if (adcdata.adc_x > 0)
	{
		velPlanIn1.adcx = adcdata.adc_x - xadc_Dim;
	}
	else if (adcdata.adc_x < 0)
	{
		velPlanIn1.adcx = adcdata.adc_x + xadc_Dim;
	}
	else
	{
		velPlanIn1.adcx = 0;
	}
	
	velPlanIn1.adcx = slopelimitx( velPlanIn1.adcx,25);  
	
	/* Y 数据清偏移*/
	if (adcdata.adc_y > 0)	
	{
		velPlanIn1.adcy = adcdata.adc_y - yadc_Dim;
	}
    else if (adcdata.adc_y < 0)
	{
		velPlanIn1.adcy = adcdata.adc_y+ yadc_Dim;
	}	
	else
	{
		velPlanIn1.adcy = 0 ;
	}
	velPlanIn1.adcy = slopelimity( velPlanIn1.adcy,25); 

	velocity_maping(velPlanIn1); /*速度规划 */	
}

/**
 * @description: 电机曲线加减速操作
 * @param {CurveObjectType} *curve
 * @return {*}
 */

void (*pCalCurve[])(CurveObjectType *curve)={CalCurveTRAP,CalCurveSPTA};

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


/**
 * @description: 电机曲线加减速操作
 * @param {CurveObjectType} *curve
 * @return {*}
 */
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
#endif