#include "./BSP/R9/underpanControl.h"
#include "math.h"
#include "stdio.h"
#include "./BSP/R9/brake.h"
#include "./BSP/BEEP/beep.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/Common/common.h"
//#include "./BSP/R9/joystic.h"
#include "./BSP/R9/getadcdata.h"

//VELOCITY_POUT velocity_pout;
//uint16_t brakeflage ;	
//void velocity_plan(VELOCITY_PIn  velPlanIn)	
//{ 

//	
//    velocity_pout.underpanVelocity = velPlanIn.max_underpanVelocity * velPlanIn.adcy/(yadc_max-yadc_Dim) * 5.0/18.0;                   /* 车速 单位 m/s */
//    velocity_pout.steering_angle =   90 * velPlanIn.adcx/(xadc_max-xadc_Dim) * PI /180.0;                                 /*  转弯角度 弧度 rad */
//	velocity_pout.steering_angle = Value_limit(-PI/2.0,velocity_pout.steering_angle,PI/2.0);                             /*  转向角约束 */
//	
//	 /* 静止  */
//	if (velocity_pout.steering_angle == 0 && velocity_pout.underpanVelocity == 0)
//	{
//		velocity_pout.L_Velocity = 0;
//	    velocity_pout. R_Velocity = 0;
//		velocity_pout.presentation_velocity = 0;
//		velocity_pout.A_IN1 = 0;
//		velocity_pout.A_IN2 = 0;
//		velocity_pout.B_IN1 = 0;
//		velocity_pout.B_IN2 = 0;
//		velocity_pout.runstate = idle ;
//		
//	}
//	/* 直行 */
//	if (velocity_pout.steering_angle == 0 && velocity_pout.underpanVelocity !=0)
//	{
//		velocity_pout.L_Velocity = velocity_pout.underpanVelocity  ;
//		velocity_pout.R_Velocity = velocity_pout.underpanVelocity  ;
//		velocity_pout.presentation_velocity =(double)fabs ((velocity_pout.L_Velocity+velocity_pout.R_Velocity)/2.0 * 18.0/5.0) ;
//		if (velocity_pout.underpanVelocity > 0)  /*向前直行 */
//		 {			
//			velocity_pout.A_IN1 = 0;
//			velocity_pout.A_IN2 = fabs(velocity_pout.L_Dutycycle);
//			velocity_pout.B_IN1 = fabs(velocity_pout.R_Dutycycle);
//			velocity_pout.B_IN2 = 0;
//			//printf("%d,%d,%f前直行\n",velPlanIn.adcx,velPlanIn.adcy,velocity_pout.underpanVelocity);
//			 velocity_pout.runstate = forward ;
//		 }
//		else 										 /*向后直行 */
//		{			
//			velocity_pout.A_IN1 =  (fabs(velocity_pout.L_Dutycycle)-0.5)*0.75+0.5;
//			velocity_pout.A_IN2 = 0;
//			velocity_pout.B_IN1 = 0;
//			velocity_pout.B_IN2 =  (fabs(velocity_pout.R_Dutycycle)-0.5)*0.75+0.5;
//            velocity_pout.runstate = backward ;      
//		//	printf("%d,%d,%f后直行\n",velPlanIn.adcx,velPlanIn.adcy,velocity_pout.underpanVelocity);
//		}
//		
//	}
//	
//	
//	/* 转向 运行 */
//	if (velocity_pout.steering_angle != 0 && velocity_pout.underpanVelocity !=0)
//	{
//        
//        /* 数学模型 vl= V*(1 + H/2R);vr = V*(1 - H/2R);*/
//        
//        velocity_pout.L_Velocity = velocity_pout.underpanVelocity * (1 +  underpan_H*velocity_pout.steering_angle);
//        velocity_pout.R_Velocity = velocity_pout.underpanVelocity * (1 -  underpan_H*velocity_pout.steering_angle);
//        
//		velocity_pout.presentation_velocity = (double)fabs((velocity_pout.L_Velocity+velocity_pout.R_Velocity)/2 * 18/5.0) ;
//		
//		if (velocity_pout.steering_angle > 0 && velocity_pout.underpanVelocity > 0)
//			/*向右前转向 */
//		{
//			velocity_pout.A_IN1 = 0;
//			velocity_pout.A_IN2 = fabs(velocity_pout.L_Dutycycle);
//			velocity_pout.B_IN1 = fabs(velocity_pout.R_Dutycycle);
//			velocity_pout.B_IN2 = 0;
//			velocity_pout.runstate = front_right ; 
//			//printf("%d,%d,%f右前转向\n",velPlanIn.adcx,velPlanIn.adcy,velocity_pout.underpanVelocity);
//		}
//		if (velocity_pout.steering_angle < 0 && velocity_pout.underpanVelocity > 0) 
//		{/*向左前转向 */
//			velocity_pout.A_IN1 = 0;
//			velocity_pout.A_IN2 = fabs(velocity_pout.L_Dutycycle);
//			velocity_pout.B_IN1 = fabs(velocity_pout.R_Dutycycle);
//			velocity_pout.B_IN2 = 0;
//			velocity_pout.runstate = front_left ; 
//			//printf("%d,%d,%f左前转向\n",velPlanIn.adcx,velPlanIn.adcy,velocity_pout.underpanVelocity);
//		}
//		if (velocity_pout.steering_angle > 0 && velocity_pout.underpanVelocity < 0) /*向右后转向 */
//		{			
//			velocity_pout.A_IN1 =  (fabs(velocity_pout.R_Dutycycle)-0.5)*0.75+0.5;
//			velocity_pout.A_IN2 = 0;
//			velocity_pout.B_IN1 = 0;
//			velocity_pout.B_IN2 =  (fabs(velocity_pout.L_Dutycycle)-0.5)*0.75+0.5;
////            BEEP_TOGGLE();
//            velocity_pout.runstate = back_left ; 
//			//printf("%d,%d,%f右后转向\n",velPlanIn.adcx,velPlanIn.adcy,velocity_pout.underpanVelocity);
//		}
//		if (velocity_pout.steering_angle < 0 && velocity_pout.underpanVelocity <0) /*向左后转向 */
//		{
//			velocity_pout.A_IN1 = (fabs(velocity_pout.R_Dutycycle)-0.5)*0.75 +0.5;
//			velocity_pout.A_IN2 = 0;
//			velocity_pout.B_IN1 = 0;
//			velocity_pout.B_IN2 = (fabs(velocity_pout.L_Dutycycle)-0.5)*0.75+0.5;
////            BEEP_TOGGLE(); 
//			velocity_pout.runstate = back_right ; 
//			//printf("%d,%d,%f左后转向\n",velPlanIn.adcx,velPlanIn.adcy,velocity_pout.underpanVelocity);
//		}
//	}
//	
//	/* 原地转向  */
//	
//	if (velocity_pout.steering_angle != 0 && velocity_pout.underpanVelocity == 0)
//	{   

//        
//			/*原地右转 */
//		if (velocity_pout.steering_angle >0)// && velocity_pout.steering_angle < 0.651 )  
//                                      
//		{
//           // velocity_pout.underpanVelocity = velPlanIn.k *velPlanIn.max_underpanVelocity * adcx_valid/yadc_max * 5.0/18.0;  
//            //velocity_pout.L_Dutycycle =  fabs(velocity_pout.steering_angle /(PI/2))*0.5*(velPlanIn.max_underpanVelocity * 5.0/18.0)*60 * GEAR_RATIO/(PI * Diameter)/MoterMaxr* 0.5+ 0.5;
//			
//			velocity_pout.A_IN1 = 0 ;
//			velocity_pout.A_IN2 =  velocity_pout.L_Dutycycle;
//			velocity_pout.B_IN1 = 0;
//			velocity_pout.B_IN2 =  velocity_pout.L_Dutycycle;
//			velocity_pout.runstate = turnself_right ; 
//		}
//			
//			/*原地左转 */
//		if (velocity_pout.steering_angle < 0)// && velocity_pout.steering_angle > -0.651 )  
//                                      
//		{
//            //velocity_pout.R_Dutycycle =  fabs(velocity_pout.steering_angle /(PI/2))*0.5*(velPlanIn.max_underpanVelocity * 5.0/18.0)*60 * GEAR_RATIO/(PI * Diameter)/MoterMaxr* 0.5+0.5;
//			
//			velocity_pout.A_IN1 = velocity_pout.R_Dutycycle ; ;
//			velocity_pout.A_IN2 = 0 ; 
//			velocity_pout.B_IN1 = velocity_pout.R_Dutycycle ;
//			velocity_pout.B_IN2 = 0;
//			velocity_pout.runstate = turnself_left ; 
//		}

//	}
//	/* 左右轮线速度约束*/
//	velocity_pout.L_Velocity = Value_limit(-velPlanIn.max_underpanVelocity * 5.0/18.0,velocity_pout.L_Velocity,velPlanIn.max_underpanVelocity * 5.0/18.0);
//	
//	velocity_pout.R_Velocity = Value_limit(-velPlanIn.max_underpanVelocity * 5.0/18.0,velocity_pout.R_Velocity,velPlanIn.max_underpanVelocity * 5.0/18.0);
//	
//	/* 轮线速转换为电机转速 转/min*/
//	velocity_pout.L_NVelocity = velocity_pout.L_Velocity * 60 * GEAR_RATIO/(PI * Diameter);
//	
//	velocity_pout.R_NVelocity  = velocity_pout.R_Velocity * 60 * GEAR_RATIO/(PI * Diameter);
//	
//	/* 电机转速转换为 占空比*/
//	

//   
//	velocity_pout.L_Dutycycle = fabs( velocity_pout.L_NVelocity / MoterMaxr) * 0.5 + 0.5;  /*占空比大于50% 方可驱动电机启动 */
//	
//	velocity_pout.R_Dutycycle = fabs(velocity_pout.R_NVelocity /MoterMaxr) * 0.5 + 0.5;


//	if (velocity_pout.runstate == turnself_left || velocity_pout.runstate == turnself_right)
//	 {
//		 
//		  velocity_pout.L_Dutycycle =  fabs(velocity_pout.steering_angle /(PI/2))*0.5*(velPlanIn.max_underpanVelocity * 5.0/18.0)*60 * GEAR_RATIO/(PI * Diameter)/MoterMaxr* 0.70+ 0.5;
//		  velocity_pout.R_Dutycycle =  fabs(velocity_pout.steering_angle /(PI/2))*0.5*(velPlanIn.max_underpanVelocity * 5.0/18.0)*60 * GEAR_RATIO/(PI * Diameter)/MoterMaxr* 0.70+0.5;
//	 }

//	/* 占空比约束*/

//    velocity_pout.L_Dutycycle = Value_limit(0 ,velocity_pout.L_Dutycycle ,1);
//    
//	velocity_pout.R_Dutycycle = Value_limit(0 ,velocity_pout.R_Dutycycle ,1); 
//    
//    
//    if (velocity_pout.L_Dutycycle <=0.5 && velocity_pout.R_Dutycycle <=0.5)

//        {
//            brakeflage++;
//           if  (brakeflage > 600 )           
//           { //抱住
//            BRAKE1(1);
//            BRAKE2(1);
//            brakeflage = 0; 
//           }
//        }
//     if (velPlanIn.adcx< -100 || velPlanIn.adcx> 100 || velPlanIn.adcy>100 || velPlanIn.adcy<-100 )
//        
//        {    // 松开
//            
//            BRAKE1(0);
//        
//            BRAKE2(0);
//            
//        }

//}



/*目标速度可变的 五次多项式   S 型曲线规划 */

// 插补标识判别    



//插补参数计算




//插补点输出





