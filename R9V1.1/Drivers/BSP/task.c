#include "./BSP/task.h"


/*************************数据采集变量*****************************/
 /* 针对 R9系统的所有ADC 数据采集 ，
 *  一 、ADC1 采集10通道数据 包含   
 * (1)  摇杆数据采集         PA2 PA3
 * (2)  抱闸 数据监测        PA4 PA5
 * (3)  底盘电机电流检测     PA6 PA7
 * (4)  推杆 1~4 电流检测    PC2 PC3 PC0 PC1
 * 二、   ADC3 数据采集  包含
 * (1) 推杆1~6  位置检测  PF5 PF3 PF4 PF6 PF8 PF7
 * (2) 推杆 5~6 的电流检测  PF9 PF10
 */
extern ADCDATA adcdata;
extern VELOCITY_POUT velocity_pout;
VELOCITY_PIn  velPlanIn1;
short  Temperature;
short gyrox,gyroy,gyroz ;
short aacx,aacy,aacz ;
float pitch,roll,yaw; 			//欧拉角

//uint8_t rs485buf[8] = {ID,READ,REGISTERADRR1,REGISTERADRR2,REGISTERnum1,REGISTERnum2,CHECK1,CHECK2}; /*485发指令*/

//extern uint8_t g_RS485_rx_buf[RS485_REC_LEN];
//extern uint8_t g_RS485_rx_cnt;

extern MLX90393Data mlxdata;


/*************************电机驱动变量*****************************/
extern TIM_HandleTypeDef g_time5_pwm_chy_handle ;  /* 底盘L电机 1 函数句柄*/
extern TIM_HandleTypeDef g_time9_pwm_chy_handle ;  /* 底盘R电机 2 函数句柄*/
extern TIM_HandleTypeDef g_time1_pwm_chy_handle ;  /* 推杆1 （举升）  TIME1 函数句柄*/
extern TIM_HandleTypeDef g_time8_pwm_chy_handle ;  /* 推杆2 （座板）    推杆3 （靠背） TIME8 函数句柄*/
extern TIM_HandleTypeDef g_time4_pwm_chy_handle ;  /* 推杆4 （脚踏旋转） 推杆5 （脚踏伸长）TIME1 函数句柄*/
extern TIM_HandleTypeDef g_time12_pwm_chy_handle ;  /*推杆6 （前支撑轮） TIME12 函数句柄*/

/*************************主任务列表*****************************/
void Hard_devInit(void)
{
	
	HAL_Init();                                 //* 初始化HAl库 */
    MoterL_pwm_chy_init(100 - 1, 42 - 1);    //* 84 000 000 / 100*42      L 20khz频率的PWM 波形*  /  
	MoterR_pwm_chy_init(200 - 1, 42 - 1);    //* 168 000 000 / 200*42     R  20khz频率的PWM 波形*/
	MoterLift_pwm_chy_init(200 - 1, 42 - 1);    //* 168 000 000 / 200*42     T1  20khz频率的PWM 波形*/
	MoterPedestal_Backboard_pwm_chy_init(200 - 1, 42 - 1);//                   T2&T3
	MoterLeg_pwm_chy_init(100 - 1, 42 - 1);               //                   T4&T5
    MoterSupport_pwm_chy_init(100 - 1, 42 - 1);   //* 84 000 000 / 100*42      T6  20khz频率的PWM 波形*  /  
    sys_stm32_clock_init(336, 8, 2, 7);     /* 初始化时钟频率,168Mhz 主时钟*/
    delay_init(168);                        /*初始化延时时钟频率*/
    usart_init(115200);                     /* 串口通讯波特率 115200 */
    led_init();                             /* 转向灯初始化 */
   // lcd_init();                             /* LCD初始化 */
    // getjoysticInit();                       /*摇杆初始化*/
	key_init();								/*按键初始化*/
	btim_timx_int_init(10 - 1, 8400 - 1);   /*定时器中断初始化 产生固定 1ms 的定时器中断 */
    brake_init();                           /*抱闸初始化*/   
    beep_init();                            /*蜂鸣器初始化*/
	dcurrentpro_init();                     /*过流保护初始化*/
	tcurrentpro_init();
	getadcDataInit();                      /*ADC数据采集初始化*/
	MPU_Init();                            /*陀螺仪初始化*/
	mpu_dmp_init();
//	rs485_init(115200);                    /*超声波测距*/
    Modbus_Init();

	vSetUpMlx90393();
	iwdg_init(IWDG_PRESCALER_64, 500);      /* 预分频数为64,重载值为500,溢出时间约为1s */
	
	//printf("IWDG溢出\n");
	
	Reg[0] = 0x68;
}

void LedFlash(void)
{
	led_bling();
}

void Beep_run(void) 
{
	if (velocity_pout.runstate == backward || velocity_pout.runstate == back_left || velocity_pout.runstate == back_right)
	{
		BEEP_TOGGLE();
	}
	// else
	// {
	// 	BEEP(0);
	// }
}
/* 针对 R9系统的所有ADC 数据采集 ，
 *  一 、ADC1 采集7通道数据 包含   
 * (1)  摇杆数据采集         PA2 PA3
 * (2)  抱闸 数据监测        PA4 PA5
 * (3)  底盘电机电流检测     PA6 PA7
 * (4)  电池电压            PC7
/
 * 二、   ADC3 数据采集  包含
 * (1) 推杆1~6  位置检测  PF5 PF3 PF4 PF6 PF8 PF7
 * (2) 推杆 1~6 的电流检测 PC2 PC3 PC0 PC1 PF9 PF10
 */	
	
void GetADC_AllData(void)
{
	getadcData();
	// printf("%d,%d,%d,%d,%d,%d\n",adcdata.adc_x,adcdata.adc_y,adcdata.l_brakcurrent,adcdata.r_brakcurrent,adcdata.l_current,adcdata.r_current);
	//  printf("%d,%d,%d,%d,%d,%d\n",adcdata.lift_pos,adcdata.pedestal_pos,adcdata.backboard_pos,adcdata.legangle_pos,adcdata.leglength_pos,adcdata.support_pos);
	//  printf("%d,%d,%d,%d,%d,%d\n",adcdata.lift_current,adcdata.pedestal_current,adcdata.backboard_current,adcdata.legangle_current,adcdata.leglength_current,adcdata.support_current);
	//  printf("adcdata.l_current :%d, adcdata.r_current %d\n",adcdata.l_current,adcdata.r_current);
}


void UnderpanDrive(void)
{
	uint16_t rpwmvaAl = 0;
	uint16_t rpwmvaA2 = 0;
	uint16_t rpwmvaBl = 0;
	uint16_t rpwmvaB2 = 0;

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
	
	velPlanIn1.k  = 1.0;
	velPlanIn1.max_underpanVelocity = 3.0; 
	
	velocity_plan(velPlanIn1); /*速度规划 */	
	rpwmvaAl = 100 * (1.0 - velocity_pout.A_IN1);
	rpwmvaA2 = 100 * (1.0 - velocity_pout.A_IN2);
	rpwmvaBl = 200 * (1.0 - velocity_pout.B_IN1);
	rpwmvaB2 = 200 * (1.0 - velocity_pout.B_IN2);

	__HAL_TIM_SET_COMPARE(&g_time5_pwm_chy_handle, GTIM_TIM5_PWM_CH1, rpwmvaAl);
	__HAL_TIM_SET_COMPARE(&g_time5_pwm_chy_handle, GTIM_TIM5_PWM_CH2, rpwmvaA2);
	__HAL_TIM_SET_COMPARE(&g_time9_pwm_chy_handle, GTIM_TIM9_PWM_CH1, rpwmvaBl);
	__HAL_TIM_SET_COMPARE(&g_time9_pwm_chy_handle, GTIM_TIM9_PWM_CH2, rpwmvaB2);
	
    //  printf("%d,%d,%d,%d\r\n",velPlanIn1.adcx,velPlanIn1.adcy,adcdata.adc_xbase,adcdata.adc_ybase);	
	printf("%d,%d,%d,%d\r\n",rpwmvaAl,rpwmvaA2, rpwmvaBl,rpwmvaB2);	
}

void linearactuatorDrive(void)
 {
		uint16_t T1_IN1 = 0;
		uint16_t T1_IN2 = 0;
		uint16_t T2_IN1 = 0;
		uint16_t T2_IN2 = 0;
		uint16_t T3_IN1 = 0;
		uint16_t T3_IN2 = 0;
		uint16_t T4_IN1 = 0;
		uint16_t T4_IN2 = 0;
		uint16_t T5_IN1 = 0;
		uint16_t T5_IN2 = 0;
		uint16_t T6_IN1 = 0;
		uint16_t T6_IN2 = 0;	

        if (key_scan5() == 1) 
		{
			T1_IN1 = 200 * (1.0 - 0);
			T1_IN2 = 200 * (1.0 - 0.7);	
			T2_IN1 = 200 * (1.0 - 0);
			T2_IN2 = 200 * (1.0 - 0.7);	
			T3_IN1 = 200 * (1.0 - 0);
			T3_IN2 = 200 * (1.0 - 0.7);	
			T4_IN1 = 100 * (1.0 - 0);
			T4_IN2 = 100 * (1.0 - 0.7);	
			T5_IN1 = 100 * (1.0 - 0);
			T5_IN2 = 100 * (1.0 - 0.7);	
			T6_IN1 = 100 * (1.0 - 0);
			T6_IN2 = 100 * (1.0 - 0.7);	
			__HAL_TIM_SET_COMPARE(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH1, T1_IN1);
			__HAL_TIM_SET_COMPARE(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH2, T1_IN2);

			__HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH3, T2_IN1);
			__HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH4, T2_IN2);
			__HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH1, T3_IN1);
			__HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH2, T3_IN2);

			__HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH3, T4_IN1);
			__HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH4, T4_IN2);			
			__HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH1, T5_IN1);
			__HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH2, T5_IN2);	

			__HAL_TIM_SET_COMPARE(&g_time12_pwm_chy_handle, GTIM_TIM12_PWM_CH1, T6_IN1);
			__HAL_TIM_SET_COMPARE(&g_time12_pwm_chy_handle, GTIM_TIM12_PWM_CH2, T6_IN2);

		}

		else if (key_scan1() == 1)
		{
			T1_IN2 = 200 * (1.0 - 0);
			T1_IN1 = 200 * (1.0 - 0.7);	
			T2_IN2 = 200 * (1.0 - 0);
			T2_IN1 = 200 * (1.0 - 0.7);	
			T3_IN2 = 200 * (1.0 - 0);
			T3_IN1 = 200 * (1.0 - 0.7);	
			T4_IN2 = 100 * (1.0 - 0);
			T4_IN1 = 100 * (1.0 - 0.7);	
			T5_IN2 = 100 * (1.0 - 0);
			T5_IN1 = 100 * (1.0 - 0.7);	
			T6_IN2 = 100 * (1.0 - 0);
			T6_IN1 = 100 * (1.0 - 0.7);		
			__HAL_TIM_SET_COMPARE(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH1, T1_IN1);
			__HAL_TIM_SET_COMPARE(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH2, T1_IN2);

			__HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH3, T2_IN1);
			__HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH4, T2_IN2);
			__HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH1, T3_IN1);
			__HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH2, T3_IN2);

			__HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH3, T4_IN1);
			__HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH4, T4_IN2);			
			__HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH1, T5_IN1);
			__HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH2, T5_IN2);	

			__HAL_TIM_SET_COMPARE(&g_time12_pwm_chy_handle, GTIM_TIM12_PWM_CH1, T6_IN1);
			__HAL_TIM_SET_COMPARE(&g_time12_pwm_chy_handle, GTIM_TIM12_PWM_CH2, T6_IN2);
		}	
        else if (key_scan1() == 0 && key_scan5() == 0)
		{
			T1_IN1 = 200 * (1.0 - 0);
			T1_IN2 = 200 * (1.0 - 0);	
			T2_IN1 = 200 * (1.0 - 0);
			T2_IN2 = 200 * (1.0 - 0);	
			T3_IN1 = 200 * (1.0 - 0);
			T3_IN2 = 200 * (1.0 - 0);	
			T4_IN1 = 100 * (1.0 - 0);
			T4_IN2 = 100 * (1.0 - 0);	
			T5_IN1 = 100 * (1.0 - 0);
			T5_IN2 = 100 * (1.0 - 0);	
			T6_IN1 = 100 * (1.0 - 0);
			T6_IN2 = 100 * (1.0 - 0);	   
			__HAL_TIM_SET_COMPARE(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH1, T1_IN1);
			__HAL_TIM_SET_COMPARE(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH2, T1_IN2);

			__HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH3, T2_IN1);
			__HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH4, T2_IN2);
			__HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH1, T3_IN1);
			__HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH2, T3_IN2);

			__HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH3, T4_IN1);
			__HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH4, T4_IN2);			
			__HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH1, T5_IN1);
			__HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH2, T5_IN2);	

			__HAL_TIM_SET_COMPARE(&g_time12_pwm_chy_handle, GTIM_TIM12_PWM_CH1, T6_IN1);
			__HAL_TIM_SET_COMPARE(&g_time12_pwm_chy_handle, GTIM_TIM12_PWM_CH2, T6_IN2);
	
		}
		
	    	
 }

void gyroscopeData(void)
{
//	uint8_t res;
//	Temperature =  MPU_Get_Temperature();
//	MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);
//	MPU_Get_Accelerometer(&aacx,&aacy,&aacz);
//    printf("%d,%d,%d,%d,%d,%d,%d\n",Temperature,gyrox,gyroy,gyroz,aacx,aacy,aacz);
//	res=MPU_Read_Byte(MPU_DEVICE_ID_REG);
//	printf("%d\n",res);
	
	
	mpu_dmp_get_data(&pitch,&roll,&yaw);       
    // printf("roll:%f,pitch:%f,yaw:%f\t\n",roll,pitch,yaw); 
		    

}

void DypA21 (void)
{
//	uint16_t dyplength;
//	uint8_t bufre[7];
//	uint8_t len;
//	uint8_t buf[2];
//    rs485_send_data(rs485buf, 8);   /* 发送8个字节 */
//	rs485_receive_data(bufre,&len);
//    //低八位和高八位组合为 16位 数据转换输出
//    buf[0] =bufre[4];// g_RS485_rx_buf[4];
//    buf[1] =bufre[3];// g_RS485_rx_buf[3];
//    dyplength = ((*((uint8_t *)buf+ 1)<< 8))| *(uint8_t *)buf;
//	printf("%d,%d\r\n",dyplength,g_RS485_rx_cnt);	
	
	Modbus_Event();//Modbus事件处理函数(执行读或者写的判断)--从机地址01
	
	
	
}
void Tmxl90393(void)    
{
	int32_t mlx_xdata,mlx_ydata;
	vInMeasurementNormal();
	// // printf("ucMlx90393ErroType:%d\n  Xdata:%d\n Ydata:%d\n Xbase:%d\n Ybase:%d\n",mlxdata.ucMlx90393ErroType,mlxdata.xdata-16800,mlxdata.ydata-16500,mlxdata.basex,mlxdata.basey);
	mlx_xdata = mlxdata.xdata-16800;
	mlx_ydata = mlxdata.ydata-16500;
	// printf("%d,%d\r\n",mlx_xdata,mlx_ydata);
}


	