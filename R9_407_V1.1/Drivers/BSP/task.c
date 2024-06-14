/*
 * @Author: lisir lisir@rehand.com
 * @Date: 2024-06-07 16:01:18
 * @LastEditors: lisir lisir@rehand.com
 * @LastEditTime: 2024-06-14 11:36:27
 * @FilePath: \MDK-ARMc:\Users\fu\Desktop\Code\CodeV1.1\R9_407_V1.1\R9_407_V1.1\Drivers\BSP\task.c
 * @Description: 主任务列表
 */
#include "./BSP/task.h"

extern ADCDATA adcdata;

/**
 * @description: 
 * @return {*}
 */

/**
 * @description: 所有任务初始化代码
 * @return {*}
 */
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
		can_init(CAN_SJW_1TQ, CAN_BS2_6TQ, CAN_BS1_7TQ, 6, CAN_MODE_NORMAL);  /* CAN初始化, 正常模式, 波特率500Kbps */
		iwdg_init(IWDG_PRESCALER_64, 1000);      /* 预分频数为64,重载值为1000,溢出时间约为2s */
		initializeFilter(&filter_L);                    /*初始化滤波器*/
		initializeFilter(&filter_R);                    /*初始化滤波器*/
		g_slaveReg[0] = 0x68;//本机设备作为Modbus从机时的设备ID
		printf("ERROR");
}

/**
 * @description: 灯控程序
 * @return {*}
 */
void LedFlash(void)
{
	led_beepControlRK3588();
//	led_beepControl();
	LED0_TOGGLE();

}

/**
 * @description: 
 *  针对 R9系统的所有ADC 数据采集 ，
 *  一 、ADC1 采集7通道数据 包含   
 * (1)  摇杆数据采集         PA2 PA3
 * (2)  抱闸 数据监测        PA4 PA5
 * (3)  底盘电机电流检测     PA6 PA7
 * (4)  电池电压            PC7 pc5
/
 * 二、   ADC3 数据采集  包含
 * (1) 推杆1~6  位置检测  PF5 PF3 PF4 PF6 PF8 PF7
 * (2) 推杆 1~6 的电流检测 PC2 PC3 PC0 PC1 PF9 PF10
 * @return {*}
 */
void GetADC_AllData(void)
{
	getadcData();
    /*数据采集及测试*/
	// printf("lift_pos:%d,pedestal_pos:%d,backboard_pos:%d,legangle_pos:%d,leglength_pos:%d,support_pos:%d\n",adcdata.lift_pos,adcdata.pedestal_pos,adcdata.backboard_pos,adcdata.legangle_pos,adcdata.leglength_pos,adcdata.support_pos);
	// printf("lift_current:%d,pedestal_current:%d,backboard_current:%d,legangle_current:%d,leglength_current:%d,support_current:%d\n",adcdata.lift_current,adcdata.pedestal_current,adcdata.backboard_current,adcdata.legangle_current,adcdata.leglength_current,adcdata.support_current);
	printf("adcdata.l_current :%d, adcdata.r_current %d\n",adcdata.l_current,adcdata.r_current);
	// printf("Xbase:%d,Ybase:%d,xdata:%d,ydata:%d\t\n",adcdata.adc_xbase,adcdata.adc_ybase,adcdata.adc_x,adcdata.adc_y);
}

/**
 * @description: 底盘控制既驱动执行
 * @return {*}
 */
void UnderpanDrive(void)
{
	underpanExcute();
}

/**
 * @description: 推杆控制及驱动
 * @return {*}
 */
void linearactuatorDrive(void)
{	
	linearactuatorTest(); 	
}

/**
 * @description: MPU6050 执行
 * @return {*}
 */
void gyroscopeData(void)
{
	MPU6050Excute();
}

/**
 * @description: 与RK3588 通讯程序执行
 * @return {*}
 */
void ModbusSlaveExecute (void)
{	
	SlaveModbus_Event();//Modbus事件处理函数(执行读或者写的判断)--从机地址0x01	
}

/**
 * @description: 超声波测距程序，目前单从没问题
 * @return {*}
 */
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

		}	
		// printf("distence: %d\n",dap21Data.dyplength1);
}
/**
 * @description: 按键板Can通讯实现
 * @return {*}
 */
void CanKeyRun(void)
{
      Canexcute();
}
