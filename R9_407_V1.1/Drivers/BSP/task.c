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
float pitch,roll,yaw;  			//欧拉角


/*************************主任务列表*****************************/
void Hard_devInit(void)
{
		HAL_Init();                                 //* 初始化HAl库 */
		MoterdriveInit();
		sys_stm32_clock_init(336, 8, 2, 7);     /* 初始化时钟频率,168Mhz 主时钟*/
		delay_init(168);                        /*初始化延时时钟频率*/
		usart_init(115200);                     /* 串口通讯波特率 115200 */
		led_init();                             /* 转向灯初始化 */
		key_init();								/*按键初始化*/
		btim_timx_int_init(10 - 1, 8400 - 1);   /*定时器中断初始化 产生固定 1ms 的定时器中断 */
		brake_init();                           /*抱闸初始化*/   
		currentproInit();
		getadcDataInit();                      /*ADC数据采集初始化*/
		MPU_Init();                            /*陀螺仪初始化*/
		mpu_dmp_init();
		Host_ModbusDap21_Init();              /*与DYPA21通讯*/
		SlaveModbus_Init();                  /*与RK3588作为从机通讯*/
		Host_Modbuskey_Init();
		iwdg_init(IWDG_PRESCALER_64, 500);      /* 预分频数为64,重载值为500,溢出时间约为1s */
		initializeFilter(&filter_L);                    /*初始化滤波器*/
		g_slaveReg[0] = 0x68;//本机设备作为Modbus从机时的设备ID
		printf("ERROR");
}

void LedFlash(void)
{
	// led_beepControlRK3588();
	led_beepControl();
	LED0_TOGGLE();

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
   // printf("%d\t\n",adcdata.lift_pos);
   // printf("lift_pos:%d,pedestal_pos:%d,backboard_pos:%d,legangle_pos:%d,leglength_pos:%d,support_pos:%d\n",adcdata.lift_pos,adcdata.pedestal_pos,adcdata.backboard_pos,adcdata.legangle_pos,adcdata.leglength_pos,adcdata.support_pos);
	//printf("lift_current:%d,pedestal_current:%d,backboard_current:%d,legangle_current:%d,leglength_current:%d,support_current:%d\n",adcdata.lift_current,adcdata.pedestal_current,adcdata.backboard_current,adcdata.legangle_current,adcdata.leglength_current,adcdata.support_current);
	//  printf("adcdata.l_current :%d, adcdata.r_current %d\n",adcdata.l_current,adcdata.r_current);
	//printf("Xbase:%d,Ybase:%d,xdata:%d,ydata:%d\t\n",adcdata.adc_xbase,adcdata.adc_ybase,adcdata.adc_x,adcdata.adc_y);
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

	velPlanIn1.set_Maximum_Strspeed = 2.0;
	velPlanIn1.set_Maximum_Steespeed = velPlanIn1.set_Maximum_Strspeed/2.0; 
	
	velocity_maping(velPlanIn1); /*速度规划 */	
	rpwmvaAl = 100 * (1.0 - velocity_pout.A_IN1);
	rpwmvaA2 = 100 * (1.0 - velocity_pout.A_IN2);
	rpwmvaBl = 200 * (1.0 - velocity_pout.B_IN1);
	rpwmvaB2 = 200 * (1.0 - velocity_pout.B_IN2);

	__HAL_TIM_SET_COMPARE(&g_time3_pwm_chy_handle, GTIM_TIM3_PWM_CH3, rpwmvaAl);
	__HAL_TIM_SET_COMPARE(&g_time3_pwm_chy_handle, GTIM_TIM3_PWM_CH4, rpwmvaA2);
	__HAL_TIM_SET_COMPARE(&g_time9_pwm_chy_handle, GTIM_TIM9_PWM_CH1, rpwmvaBl);
	__HAL_TIM_SET_COMPARE(&g_time9_pwm_chy_handle, GTIM_TIM9_PWM_CH2, rpwmvaB2);
	
   
	
}


void linearactuatorDrive(void)
 {	
	linearactuatorTest(); 	
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
    //printf("roll:%f,pitch:%f,yaw:%f\t\n",roll,pitch,yaw); 
		    

}
void ModbusSlaveExecute (void)
{
	
	SlaveModbus_Event();//Modbus事件处理函数(执行读或者写的判断)--从机地址0x01
	
}



void ultrasonicreadExecute (void)
{
		if(modbus_dap21.Host_time_flag)//每1s发送一次数据
		{
			HostDap21_Read03_slave(0x01,0x0101,0x0001);//参数1从机地址，参数2起始地址，参数3寄存器个数
			if(modbus_dap21.Host_send_flag)
			{
				modbus_dap21.Host_Sendtime=0;//发送完毕后计数清零（距离上次的时间）
				modbus_dap21.Host_time_flag=0;//发送数据标志位清零
				modbus_dap21.Host_send_flag=0;//清空发送结束数据标志位

				HOST_ModbusDap21RX();//接收数据进行处理
			}	
//			HostDap21_Read03_slave(0x02,0x0101,0x0001);//参数1从机地址，参数2起始地址，参数3寄存器个数
//			if(modbus_dap21.Host_send_flag)
//			{
//				modbus_dap21.Host_Sendtime=0;//发送完毕后计数清零（距离上次的时间）
//				modbus_dap21.Host_time_flag=0;//发送数据标志位清零
//				modbus_dap21.Host_send_flag=0;//清空发送结束数据标志位

//				HOST_ModbusDap21RX();//接收数据进行处理
//			}	
//			HostDap21_Read03_slave(0x03,0x0101,0x0001);//参数1从机地址，参数2起始地址，参数3寄存器个数
//			if(modbus_dap21.Host_send_flag)
//			{
//				modbus_dap21.Host_Sendtime=0;//发送完毕后计数清零（距离上次的时间）
//				modbus_dap21.Host_time_flag=0;//发送数据标志位清零
//				modbus_dap21.Host_send_flag=0;//清空发送结束数据标志位

//				HOST_ModbusDap21RX();//接收数据进行处理
//			}
//			
//			HostDap21_Read03_slave(0x04,0x0101,0x0001);//参数1从机地址，参数2起始地址，参数3寄存器个数
//			if(modbus_dap21.Host_send_flag)
//			{
//				modbus_dap21.Host_Sendtime=0;//发送完毕后计数清零（距离上次的时间）
//				modbus_dap21.Host_time_flag=0;//发送数据标志位清零
//				modbus_dap21.Host_send_flag=0;//清空发送结束数据标志位

//				HOST_ModbusDap21RX();//接收数据进行处理
//			}
		}	
	//	printf("distence: %d\n",dap21Data.dyplength1);
}

void Modbuskeyread_execute(void)
{
		if(modbus.Host_time_flag)//每1s发送一次数据
		{
			Host_Read03_slave(0x11,0x0000,0x000B);//参数1从机地址，参数2起始地址，参数3寄存器个数
			if(modbus.Host_send_flag)
			{
				modbus.Host_Sendtime=0;//发送完毕后计数清零（距离上次的时间）
				modbus.Host_time_flag=0;//发送数据标志位清零
				modbus.Host_send_flag=0;//清空发送结束数据标志位

				HOST_ModbusRX();//接收数据进行处理
			}	
		}
	printf("%d ,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",KeyStateRecive[0],KeyStateRecive[1],KeyStateRecive[2],KeyStateRecive[3],KeyStateRecive[4],KeyStateRecive[5],KeyStateRecive[6],KeyStateRecive[7],KeyStateRecive[8],KeyStateRecive[9],KeyStateRecive[10]);
}
	
void Modbuskeywrite_execute(void)
{
//  if(modbus.Host_time_flag)//每1s发送一次数据
// 	{
		Host_write06_slave(0x11,0x06,0x0091,0X01);
		if(modbus.Host_send_flag)
		{
			modbus.Host_Sendtime=0;//发送完毕后计数清零（距离上次的时间）
			modbus.Host_time_flag=0;//发送数据标志位清零
			modbus.Host_send_flag=0;//清空发送结束数据标志位
			Host_Func6();//从机返回数据处理
		}  
	// }	
}
