/**
 ****************************************************************************************************
 * @file        led.c
 * @author      Lisir
 * @version     V1.0
 * @date        2021-10-14
 * @brief       LED ????????
 * @license     Copyright (c) 2024, ??????????????????
 ****************************************************************************************************
 * @attention
 *
 * ?????:F407??????
 * none
 * none
 * none
 * none
 *
 * ??????
 * none
 * ????��???
 *
 ****************************************************************************************************
 */

#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/R9/Slavemodbus.h"

/**
 * @brief       ?????LED???IO??, ????????
 * @param       ??
 * @retval      ??

 */

void led_init(void)
{
	GPIO_InitTypeDef gpio_init_struct;

	LED0_GPIO_CLK_ENABLE(); /* //LED0?????? */
	LED1_GPIO_CLK_ENABLE(); /* //LED1?????? */

	gpio_init_struct.Pin = LED0_GPIO_PIN;			  /* //LED0???? */
	gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;	  /* ??????? */
	gpio_init_struct.Pull = GPIO_PULLUP;			  /* ???? */
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;	  /* ???? */
	HAL_GPIO_Init(LED0_GPIO_PORT, &gpio_init_struct); /* ?????//LED0???? */

	gpio_init_struct.Pin = LED1_GPIO_PIN;			  /* //LED1???? */
	HAL_GPIO_Init(LED1_GPIO_PORT, &gpio_init_struct); /* ?????//LED1???? */

	// ???????-----?

	LEFT_FRONT_TURE_GPIO_CLK_ENABLE();
	RIGHT_FRONT_TURE_GPIO_CLK_ENABLE();
	FRONT_MAIN_GPIO_CLK_ENABLE();

	gpio_init_struct.Pin = LEFT_FRONT_TURE_GPIO_PIN;
	gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;   /* ??????? */
	gpio_init_struct.Pull = GPIO_PULLUP;		   /* ???? */
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH; /* ???? */
	HAL_GPIO_Init(LEFT_FRONT_TURE_GPIO_PORT, &gpio_init_struct);

	gpio_init_struct.Pin = RIGHT_FRONT_TURE_GPIO_PIN;
	HAL_GPIO_Init(RIGHT_FRONT_TURE_GPIO_PORT, &gpio_init_struct);

	gpio_init_struct.Pin = FRONT_MAIN_GPIO_PIN;
	HAL_GPIO_Init(FRONT_MAIN_GPIO_PORT, &gpio_init_struct);

	// ???????-----??

	LEFT_BACK_TURE_GPIO_CLK_ENABLE();
	RIGHT_BACK_TURE_GPIO_CLK_ENABLE();
	BACK_MAIN_GPIO_CLK_ENABLE();

	gpio_init_struct.Pin = LEFT_BACK_TURE_GPIO_PIN | RIGHT_BACK_TURE_GPIO_PIN | BACK_MAIN_GPIO_PIN;
	HAL_GPIO_Init(BACK_MAIN_GPIO_PORT, &gpio_init_struct);

	LED0(1); /* ??? //LED0 */
	LED1(1); /* ??? //LED1 */

	LEFT_FRONT_TURE(0);
	g_slaveReg[14] = LeftBulbState;
	RIGHT_FRONT_TURE(0);
	g_slaveReg[15] = RightBulbState;
	FRONT_MAIN(0);
	g_slaveReg[13] = MainBulbState;
	LEFT_BACK_TURE(0);
	RIGHT_BACK_TURE(0);
	BACK_MAIN(0);
}

/**
 * @param
 *   @arg
 *
 *   @arg
 * @retval
 *   
 */

Led_State led_state;
Led_State lastled_state;
uint8_t doubleflingflage = 1;


void led_beepControl(void)
{
	static uint8_t cmdBulb_contes = 1; // 移动端主灯指令计数器
	static uint8_t KEY2_PRES_contes = 1; // 左转向灯指令计数器
	static uint8_t KEY3_PRES_contes = 1; // 右转向灯指令计数器
	static uint8_t KEY4_PRES_contes = 1; // 360view全景指令计数器
	static uint8_t KEYDouble_PRES_contes = 1; // 双闪指令计数器
	lastled_state = None;
	if (led_state != lastled_state)
	{
		lastled_state = led_state;
	}
// beepcontrol 
	if (key_scan1() == 1 ) 
	{
		g_slaveReg[35] = 1;
	}
	else
	{
		g_slaveReg[35] = 0;
	}

/* left   right and  maibulb control*/
	if (cmdBulb_contes==1  && KEY2_PRES_contes==1 && KEY3_PRES_contes==1 && KEYDouble_PRES_contes==1)
	{
		led_state = idle_state;
	}

// maibulb control
	if (keycmdbulb() == 1 ) 
	{

		cmdBulb_contes++;

		if (cmdBulb_contes > 2)
			cmdBulb_contes = 1;
		if (cmdBulb_contes % 2 == 0 )
		{
			led_state = open_mainbulb;
			cmdBulb_contes = 1;
		}
		else
		{
			led_state = close_mainbulb;
		}
	}

// left control
	if (key_scan2() == 1 || keycmdleftbulb()==1)
	{

		KEY2_PRES_contes++;

		if (KEY2_PRES_contes > 2)
			KEY2_PRES_contes = 1;
		if (KEY2_PRES_contes % 2 == 0)
		{
			led_state = open_leftbling;
			KEY3_PRES_contes = 1;           // close right
			KEYDouble_PRES_contes = 1;     // close double
		}
		else
		{
			led_state = close_leftbling;
		}
	}

	/*right */
	if (key_scan3() == 1 || keycmdrightbulb()==1 )
	{

		KEY3_PRES_contes++;

		if (KEY3_PRES_contes > 2)
			KEY3_PRES_contes = 1;
		if (KEY3_PRES_contes % 2 == 0)
		{
			led_state = open_rightbling;
			KEY2_PRES_contes = 1;       // close left
			KEYDouble_PRES_contes = 1;     // close doubles
		}
		else
		{
			led_state = close_rightbling;
		}
	}
	/*double*/
	if (key_scandouble() == 1)
	{

		KEYDouble_PRES_contes++;

		if (KEYDouble_PRES_contes > 2)
			KEYDouble_PRES_contes = 1;
		if (KEYDouble_PRES_contes % 2 == 0)
		{
			led_state = open_doublebling;
			
			KEY3_PRES_contes = 1; // close right
			KEY2_PRES_contes = 1;// close left
			cmdBulb_contes =1 ; //  close mainbulb
		}
		else
		{
			led_state = close_doublebling;
		}
	}

	switch (led_state)
	{
	case idle_state:
		LEFT_FRONT_TURE(0);
		g_slaveReg[14] = LeftBulbState;
		RIGHT_FRONT_TURE(0);
		g_slaveReg[15] = RightBulbState;
		FRONT_MAIN(0);
		g_slaveReg[13] = MainBulbState;
		LEFT_BACK_TURE(0);		
		RIGHT_BACK_TURE(0);	
		BACK_MAIN(0);
		break;

	case open_leftbling:
		LEFT_FRONT_TURE_TOGGLE();
		g_slaveReg[14] = LeftBulbState;
		RIGHT_FRONT_TURE(0);
		g_slaveReg[15] = RightBulbState;
		LEFT_BACK_TURE(1);	
		RIGHT_BACK_TURE(0);
	
		break;

	case close_leftbling:

		LEFT_FRONT_TURE(0);
		g_slaveReg[14] = LeftBulbState;
		LEFT_BACK_TURE(0);
		
		break;

	case open_rightbling:
		RIGHT_FRONT_TURE_TOGGLE();
		g_slaveReg[15] = RightBulbState;
		LEFT_FRONT_TURE(0);
		g_slaveReg[14] = LeftBulbState;

		LEFT_BACK_TURE(0);
		
		RIGHT_BACK_TURE(1);
		

		break;

	case close_rightbling:

		RIGHT_FRONT_TURE(0);
		g_slaveReg[15] = RightBulbState;
		RIGHT_BACK_TURE(0);
		

		break;

	case open_doublebling:
		FRONT_MAIN(0);
		g_slaveReg[13] = MainBulbState;
		BACK_MAIN(0);
		LEFT_BACK_TURE(1);
		
		RIGHT_BACK_TURE(1);
		
		if (doubleflingflage)
		{
			LEFT_FRONT_TURE(0);
			g_slaveReg[14] = LeftBulbState;
			RIGHT_FRONT_TURE(0);
			g_slaveReg[15] = RightBulbState;
			FRONT_MAIN(0);
			g_slaveReg[13] = MainBulbState;
			doubleflingflage = 0;
		}
		else
		{
			doubleflingflage = 1;
			FRONT_MAIN(0);
			g_slaveReg[13] = MainBulbState;
			LEFT_FRONT_TURE(1);
			g_slaveReg[14] = LeftBulbState;
			RIGHT_FRONT_TURE(1);
			g_slaveReg[15] = RightBulbState;
		}

		if (lastled_state == open_leftbling || lastled_state == open_rightbling) 
		{
			LEFT_BACK_TURE(0);
			
			RIGHT_BACK_TURE(0);
			
			delay_ms(10);
		}

		break;

	case close_doublebling:
		LEFT_FRONT_TURE(0);
		g_slaveReg[14] = LeftBulbState;
		RIGHT_FRONT_TURE(0);
		g_slaveReg[15] = RightBulbState;
		LEFT_BACK_TURE(0);
		
		RIGHT_BACK_TURE(0);
		

		break;

	case open_mainbulb:
		BACK_MAIN(1);

		FRONT_MAIN(1);
		g_slaveReg[13] = MainBulbState;

		if (lastled_state == open_leftbling) 
		{
			led_state = open_leftbling;
		}
		if (lastled_state == open_rightbling) 
		{
			led_state = open_rightbling;
		}
		if (lastled_state == open_doublebling) 
		{
			LEFT_FRONT_TURE(0);
			g_slaveReg[14] = LeftBulbState;
			RIGHT_FRONT_TURE(0);
			g_slaveReg[15] = RightBulbState;
			LEFT_BACK_TURE(0);
			
			RIGHT_BACK_TURE(0);
			
			FRONT_MAIN(0);
			g_slaveReg[13] = MainBulbState;
			led_state = open_mainbulb;
		}

		break;

	case close_mainbulb:
		FRONT_MAIN(0);
		g_slaveReg[13] = MainBulbState;
		BACK_MAIN(0);
		if (lastled_state == open_leftbling) 
		{
			led_state = open_leftbling;
		}
		if (lastled_state == open_rightbling) 
		{
			led_state = open_rightbling;
		}
		break;
	default:
		break;
	}
}

void led_beepControlRK3588(void)
{
	lastled_state = None;
	if (led_state != lastled_state)
	{
		lastled_state = led_state;
	}
/* left   right and  maibulb control
72 : 左转向指令
73 ：右转向指令
74 ：照明指令
75： 双闪指令
*/
	if (g_slaveReg[72] == 0  && g_slaveReg[73] == 0 && g_slaveReg[74] == 0 && g_slaveReg[75]==0 )
	{
		led_state = idle_state;
	}
// maibulb control
	if (g_slaveReg[74] ) 
	{
		led_state = open_mainbulb;
	}
	else
	{		
		led_state = close_mainbulb;	
	}

// left control
	if (g_slaveReg[72])
	{
		led_state = open_leftbling;
	}
	else
	{
		led_state = close_leftbling;
	}

/*right */
	if (g_slaveReg[73])
	{
		led_state = open_rightbling;
	}
	else
	{		
		led_state = close_rightbling;
	}
/*double*/
	if (g_slaveReg[75])
	{
		led_state = open_doublebling;
	}		
	else
	{
		led_state = close_doublebling;
	}
/*bulb driving*/
	switch (led_state)
	{
	case idle_state:
		LEFT_FRONT_TURE(0);
		g_slaveReg[14] = LeftBulbState;
		RIGHT_FRONT_TURE(0);
		g_slaveReg[15] = RightBulbState;
		FRONT_MAIN(0);
		g_slaveReg[13] = MainBulbState;
		LEFT_BACK_TURE(0);		
		RIGHT_BACK_TURE(0);	
		BACK_MAIN(0);
		break;
	case open_leftbling:
		LEFT_FRONT_TURE_TOGGLE();
		g_slaveReg[14] = LeftBulbState;
		RIGHT_FRONT_TURE(0);
		g_slaveReg[15] = RightBulbState;
		LEFT_BACK_TURE(1);	
		RIGHT_BACK_TURE(0);
		break;
	case close_leftbling:
		LEFT_FRONT_TURE(0);
		g_slaveReg[14] = LeftBulbState;
		LEFT_BACK_TURE(0);	
		break;
	case open_rightbling:
		RIGHT_FRONT_TURE_TOGGLE();
		g_slaveReg[15] = RightBulbState;
		LEFT_FRONT_TURE(0);
		g_slaveReg[14] = LeftBulbState;
		LEFT_BACK_TURE(0);	
		RIGHT_BACK_TURE(1);
		break;
	case close_rightbling:
		RIGHT_FRONT_TURE(0);
		g_slaveReg[15] = RightBulbState;
		RIGHT_BACK_TURE(0);
		break;
	case open_doublebling:
		g_slaveReg[13] = MainBulbState;
		BACK_MAIN(0);
		LEFT_BACK_TURE(1);	
		RIGHT_BACK_TURE(1);	
		if (doubleflingflage)
		{
			LEFT_FRONT_TURE(0);
			g_slaveReg[14] = LeftBulbState;
			RIGHT_FRONT_TURE(0);
			g_slaveReg[15] = RightBulbState;
			FRONT_MAIN(0);
			g_slaveReg[13] = MainBulbState;
			doubleflingflage = 0;
		}
		else
		{
			doubleflingflage = 1;
			FRONT_MAIN(0);
			g_slaveReg[13] = MainBulbState;
			LEFT_FRONT_TURE(1);
			g_slaveReg[14] = LeftBulbState;
			RIGHT_FRONT_TURE(1);
			g_slaveReg[15] = RightBulbState;
		}
		if (lastled_state == open_leftbling || lastled_state == open_rightbling) 
		{
			LEFT_BACK_TURE(0);	
			RIGHT_BACK_TURE(0);	
			delay_ms(10);
		}
		break;
	case close_doublebling:
		LEFT_FRONT_TURE(0);
		g_slaveReg[14] = LeftBulbState;
		RIGHT_FRONT_TURE(0);
		g_slaveReg[15] = RightBulbState;
		LEFT_BACK_TURE(0);	
		RIGHT_BACK_TURE(0);
		break;
	case open_mainbulb:
		BACK_MAIN(1);
		FRONT_MAIN(1);
		g_slaveReg[13] = MainBulbState;
		if (lastled_state == open_leftbling) 
		{
			led_state = open_leftbling;
		}
		if (lastled_state == open_rightbling) 
		{
			led_state = open_rightbling;
		}
		if (lastled_state == open_doublebling) 
		{
			LEFT_FRONT_TURE(0);
			g_slaveReg[14] = LeftBulbState;
			RIGHT_FRONT_TURE(0);
			g_slaveReg[15] = RightBulbState;
			LEFT_BACK_TURE(0);	
			RIGHT_BACK_TURE(0);	
			FRONT_MAIN(0);
			g_slaveReg[13] = MainBulbState;
			led_state = open_mainbulb;
		}
		break;

	case close_mainbulb:
		FRONT_MAIN(0);
		g_slaveReg[13] = MainBulbState;
		BACK_MAIN(0);
		if (lastled_state == open_leftbling) 
		{
			led_state = open_leftbling;
		}
		if (lastled_state == open_rightbling) 
		{
			led_state = open_rightbling;
		}

		break;

	default:
		break;
	}
}
