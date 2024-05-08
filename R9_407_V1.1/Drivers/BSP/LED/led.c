/**
 ****************************************************************************************************
 * @file        led.c
 * @author      Lisir
 * @version     V1.0
 * @date        2021-10-14
 * @brief       LED 驱动代码
 * @license     Copyright (c) 2024, 深圳复成医疗科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:F407开发板
 * none
 * none
 * none
 * none
 *
 * 修改说明
 * none
 * 第一次发布
 *
 ****************************************************************************************************
 */

#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/BEEP/beep.h"
#include "./BSP/R9/Slavemodbus.h"

/**
 * @brief       初始化LED相关IO口, 并使能时钟
 * @param       无
 * @retval      无

 */

void led_init(void)
{
	GPIO_InitTypeDef gpio_init_struct;

	LED0_GPIO_CLK_ENABLE(); /* //LED0时钟使能 */
	LED1_GPIO_CLK_ENABLE(); /* //LED1时钟使能 */

	gpio_init_struct.Pin = LED0_GPIO_PIN;			  /* //LED0引脚 */
	gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;	  /* 推挽输出 */
	gpio_init_struct.Pull = GPIO_PULLUP;			  /* 上拉 */
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;	  /* 高速 */
	HAL_GPIO_Init(LED0_GPIO_PORT, &gpio_init_struct); /* 初始化//LED0引脚 */

	gpio_init_struct.Pin = LED1_GPIO_PIN;			  /* //LED1引脚 */
	HAL_GPIO_Init(LED1_GPIO_PORT, &gpio_init_struct); /* 初始化//LED1引脚 */

	// 转向及主灯-----前

	LEFT_FRONT_TURE_GPIO_CLK_ENABLE();
	RIGHT_FRONT_TURE_GPIO_CLK_ENABLE();
	FRONT_MAIN_GPIO_CLK_ENABLE();

	gpio_init_struct.Pin = LEFT_FRONT_TURE_GPIO_PIN;
	gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;   /* 推挽输出 */
	gpio_init_struct.Pull = GPIO_PULLUP;		   /* 上拉 */
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH; /* 高速 */
	HAL_GPIO_Init(LEFT_FRONT_TURE_GPIO_PORT, &gpio_init_struct);

	gpio_init_struct.Pin = RIGHT_FRONT_TURE_GPIO_PIN;
	HAL_GPIO_Init(RIGHT_FRONT_TURE_GPIO_PORT, &gpio_init_struct);

	gpio_init_struct.Pin = FRONT_MAIN_GPIO_PIN;
	HAL_GPIO_Init(FRONT_MAIN_GPIO_PORT, &gpio_init_struct);

	// 转向及主灯-----后

	LEFT_BACK_TURE_GPIO_CLK_ENABLE();
	RIGHT_BACK_TURE_GPIO_CLK_ENABLE();
	BACK_MAIN_GPIO_CLK_ENABLE();

	gpio_init_struct.Pin = LEFT_BACK_TURE_GPIO_PIN | RIGHT_BACK_TURE_GPIO_PIN | BACK_MAIN_GPIO_PIN;
	HAL_GPIO_Init(BACK_MAIN_GPIO_PORT, &gpio_init_struct);

	LED0(1); /* 关闭 //LED0 */
	LED1(1); /* 关闭 //LED1 */

	LEFT_FRONT_TURE(0);
	RIGHT_FRONT_TURE(0);
	FRONT_MAIN(0);

	LEFT_BACK_TURE(0);
	RIGHT_BACK_TURE(0);
	BACK_MAIN(0);
}

/**
 * @brief       转向灯及 双闪灯 控制函数
 * @note        按下 K0 左转向  按下K2 右转向  按下K1 双闪灯; 同一个按键二次按下输出相反状态，
				其它按键按下释放之前的状态；
 * @param
 *   @arg
 *
 *   @arg
 * @retval
 *              KEY0_PRES, 1, KEY0按下
 *              KEY1_PRES, 2, KEY1按下
 *              KEY2_PRES, 3, KEY2按下
 *              WKUP_PRES, 4, WKUP按下
 */

Led_State led_state;
Led_State lastled_state;
uint8_t doubleflingflage = 1;
void led_bling(void)
{
	static uint16_t KEY0_PRES_contes = 1;
	static uint16_t KEY1_PRES_contes = 1;
	static uint16_t KEY2_PRES_contes = 1;
	static uint16_t KEY3_PRES_contes = 1;
	lastled_state = None;
	if (led_state != lastled_state)
	{
		lastled_state = led_state;
	}

	if (KEY0_PRES_contes == 1 && KEY1_PRES_contes == 1 && KEY2_PRES_contes == 1 && KEY3_PRES_contes == 1)
	{
		led_state = idle_state;
	}
	//  主灯控制
	// if (Reg[1] == 0x0001)
	// {
	// 	led_state = open_mainbulb;
		
	// }
	// else  if(Reg[1] == 0x0000)
	// {
	// 	led_state = close_mainbulb;
		
	// }

	if (key_scan4() == 1 ) 
	{

		KEY3_PRES_contes++;

		if (KEY3_PRES_contes > 2)
			KEY3_PRES_contes = 1;
		if (KEY3_PRES_contes % 2 == 0 )
		{
			led_state = open_mainbulb;
			KEY1_PRES_contes = 1;
		}
		else
		{
			led_state = close_mainbulb;
		}
	}



	/*左转向控制*/
	if (key_scan2() == 1)
	{

		KEY2_PRES_contes++;

		if (KEY2_PRES_contes > 2)
			KEY2_PRES_contes = 1;
		if (KEY2_PRES_contes % 2 == 0)
		{
			led_state = open_leftbling;
			KEY0_PRES_contes = 1;
			KEY1_PRES_contes = 1;
			//			   KEY3_PRES_contes = 1;
		}
		else
		{
			led_state = close_leftbling;
		}
	}

	/*右转向控制 */
	if (key_scan3() == 1)
	{

		KEY0_PRES_contes++;

		if (KEY0_PRES_contes > 2)
			KEY0_PRES_contes = 1;
		if (KEY0_PRES_contes % 2 == 0)
		{
			led_state = open_rightbling;
			KEY2_PRES_contes = 1;
			KEY1_PRES_contes = 1;
			//			   KEY3_PRES_contes = 1;
		}
		else
		{
			led_state = close_rightbling;
		}
	}
	/*双闪控制*/
	if (key_scan6() == 1)
	{

		KEY1_PRES_contes++;

		if (KEY1_PRES_contes > 2)
			KEY1_PRES_contes = 1;
		if (KEY1_PRES_contes % 2 == 0)
		{
			led_state = open_doublebling;
			KEY0_PRES_contes = 1;
			KEY3_PRES_contes = 1;
			KEY2_PRES_contes = 1;
		}
		else
		{
			led_state = close_doublebling;
		}
	}

	switch (led_state)
	{
	case idle_state:

		LED0(1);
		LED1(1);
		LEFT_FRONT_TURE(0);
		RIGHT_FRONT_TURE(0);
		FRONT_MAIN(0);

		LEFT_BACK_TURE(0);
		RIGHT_BACK_TURE(0);
		BACK_MAIN(0);
		break;

	case open_leftbling:
		LED0_TOGGLE();
		LED1(1);
		LEFT_FRONT_TURE_TOGGLE();
		RIGHT_FRONT_TURE(0);

		LEFT_BACK_TURE(1);
		RIGHT_BACK_TURE(0);

		break;

	case close_leftbling:
		LED0(1);
		LEFT_FRONT_TURE(0);
		LEFT_BACK_TURE(0);
		break;

	case open_rightbling:
		LED1_TOGGLE();
		LED0(1);
		RIGHT_FRONT_TURE_TOGGLE();
		LEFT_FRONT_TURE(0);

		LEFT_BACK_TURE(0);
		RIGHT_BACK_TURE(1);

		break;

	case close_rightbling:
		LED1(1);
		RIGHT_FRONT_TURE(0);
		RIGHT_BACK_TURE(0);

		break;

	case open_doublebling:
		FRONT_MAIN(0);
		BACK_MAIN(0);
		LEFT_BACK_TURE(1);
		RIGHT_BACK_TURE(1);
		if (doubleflingflage)
		{
			LED0(1);
			LED1(1);
			LEFT_FRONT_TURE(0);
			RIGHT_FRONT_TURE(0);
			FRONT_MAIN(0);
			doubleflingflage = 0;
		}
		else
		{
			doubleflingflage = 1;
			LED0(0);
			LED1(0);
			FRONT_MAIN(0);
			LEFT_FRONT_TURE(1);
			RIGHT_FRONT_TURE(1);
		}

		if (lastled_state == open_leftbling || lastled_state == open_rightbling) // 如果上一个状态是 左转或者右转向则 先关闭一下灯再进入双闪保证双闪同步
		{
			LEFT_BACK_TURE(0);
			RIGHT_BACK_TURE(0);
			delay_ms(10);
		}

		break;

	case close_doublebling:
		LED0(1);
		LED1(1);
		LEFT_FRONT_TURE(0);
		RIGHT_FRONT_TURE(0);

		LEFT_BACK_TURE(0);
		RIGHT_BACK_TURE(0);

		break;

	case open_mainbulb:
		BACK_MAIN(1);

		FRONT_MAIN(1);
		if (lastled_state == open_leftbling) // 如果上一个状态是 左转向则 继续跳往左转向
		{
			led_state = open_leftbling;
		}
		if (lastled_state == open_rightbling) // 如果上一个状态是 右转向则 继续跳往右转向
		{
			led_state = open_rightbling;
		}
		if (lastled_state == open_doublebling) // 如果上一个状态是 双闪 则关闭双闪 停留在主光灯状态
		{
			LEFT_FRONT_TURE(0);
			RIGHT_FRONT_TURE(0);
			LEFT_BACK_TURE(0);
			RIGHT_BACK_TURE(0);
			FRONT_MAIN(0);
			led_state = open_mainbulb;
		}

		break;

	case close_mainbulb:
		FRONT_MAIN(0);
		BACK_MAIN(0);
		if (lastled_state == open_leftbling) // 如果上一个状态是 左转向则 继续跳往左转向
		{
			led_state = open_leftbling;
		}
		if (lastled_state == open_rightbling) // 如果上一个状态是 右转向则 继续跳往右转向
		{
			led_state = open_rightbling;
		}

		break;

	default:
		break;
	}

	//  喇叭 控制
	if (key_scan7() == 1)
	{
		BEEP(1);
	}
	else
	{
		BEEP(0);
	}
}
