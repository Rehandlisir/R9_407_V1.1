#ifndef __UNDERPANCONTROL_H
#define __UNDERPANCONTROL_H

#include "./SYSTEM/sys/sys.h"


/* �1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7 */
#define underpan_H    0.55   

#define PI            3.1415926

#define GEAR_RATIO		16.75               /* �1�7�1�7�1�7�1�3�1�7  */
#define Diameter        0.270                 /* �1�7�1�7�1�7�1�7�0�1�1�7�1�7  */
#define MoterMaxr      2200.0                /*�1�7�1�7�1�7�0�8�1�7�1�7 r/min  */

/* �1�7�1�7�1�7�0�9�5�5�1�7�1�7  */

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

typedef struct /*�1�7�1�8�0�4���1�7�1�7�1�7�1�7*/
{
 int16_t adcx;
 int16_t adcy;
 float max_underpanVelocity;
 double  k;
}VELOCITY_PIn;

typedef struct /* �1�7�1�8�0�4���1�7�1�7�1�7*/
{
	double underpanVelocity ; /*�1�7���0�6�0�2 �1�7�1�8�1�7 �1�7�1�7�� m/s */
	double presentation_velocity; /* �1�7�1�7�˄1�7�1�7�1�7�1�7�0�5�1�7�1�8�1�7 m/s*/

	double steering_angle;
	double L_Velocity ;      /* �1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�8�1�7 */
	double R_Velocity ;      /* �1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�8�1�7*/
	double L_NVelocity;      /* �1�7�1�7�1�7�0�5�1�7�1�7�0�8�1�7�1�7*/
	double R_NVelocity;      /* �1�7�1�7�1�7�0�5�1�7�1�7�0�8�1�7�1�7*/
    double L_Dutycycle;      /* �1�7�1�7�1�7�1�7�0�8�1�7�0�7�1�7*/
	double R_Dutycycle;      /* �1�7�1�7�1�7�1�7�0�8�1�7�0�7�1�7 */

	double A_IN1       ;    /*�1�7�1�7�1�7�1�7�0�8�1�7�0�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�0�2�0�5�1�71 */
	double A_IN2       ;    /*�1�7�1�7�1�7�1�7�0�8�1�7�0�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�0�2�0�5�1�72 */
	double B_IN1       ;    /*�1�7�0�9�1�7�1�7�0�8�1�7�0�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�0�2�0�5�1�71 */
	double B_IN2       ;    /*�1�7�0�9�1�7�1�7�0�8�1�7�0�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�0�2�0�5�1�72 */
	RunState runstate  ;
	
	
	
} VELOCITY_POUT;


int32_t  Value_Resetzero(int32_t min_value ,int32_t current_value ,int32_t max_value);
void velocity_plan(VELOCITY_PIn);	

#endif

