#include "./BSP/task.h"


/*************************主任务列表*****************************/
void Hard_devInit(void)
{
	
	HAL_Init();                                 //* 初始化HAl库 */
    sys_stm32_clock_init(336, 8, 2, 7);     /* 初始化时钟频率,168Mhz 主时钟*/
    delay_init(168);                        /*初始化延时时钟频率*/
    usart_init(115200);                     /* 串口通讯波特率 115200 */
    Led_Init();                             /* 转向灯初始化 */
	key_init();								/*按键初始化*/
	btim_timx_int_init(10 - 1, 8400 - 1);   /*定时器中断初始化 产生固定 1ms 的定时器中断 */
    Modbuskey_Init();
	iwdg_init(IWDG_PRESCALER_64, 500);      /* 预分频数为64,重载值为500,溢出时间约为1s */
	KeyReg[0] = 0x88;
}


void ModbusKey (void)
{
    
	Modbuskey_Event();//Modbus事件处理函数(执行读或者写的判断)--从机地址011
	
}




void Led_control (void)
{
	KeyScan();
	LedControl();
}	