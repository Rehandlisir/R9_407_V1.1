/**
 ****************************************************************************************************
 * @file        led.c
 * @author      Lisir
 * @version     V1.0
 * @date        2021-10-14
 * @brief       LED指示灯 驱动代码
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
#include "./BSP/R9/modbus.h"

/**
 * @brief       初始化LED相关IO口, 并使能时钟
 * @param       无
 * @retval      无

 */

void Led_Init(void)
{
	GPIO_InitTypeDef gpio_init_struct;
	LED1G_GPIO_CLK_ENABLE();
	LED1R_GPIO_CLK_ENABLE();
	LED1B_GPIO_CLK_ENABLE();
	LED2G_GPIO_CLK_ENABLE();
	LED2R_GPIO_CLK_ENABLE();
	LED2B_GPIO_CLK_ENABLE();
	LED3G_GPIO_CLK_ENABLE();
	LED3R_GPIO_CLK_ENABLE();
	LED3B_GPIO_CLK_ENABLE();
	LED4G_GPIO_CLK_ENABLE();
	LED4R_GPIO_CLK_ENABLE();
	LED4B_GPIO_CLK_ENABLE();
	LED5G_GPIO_CLK_ENABLE();
	LED5R_GPIO_CLK_ENABLE();
	LED5B_GPIO_CLK_ENABLE();

	gpio_init_struct.Pin = LED1G_GPIO_PIN;			 
	gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;	  
	gpio_init_struct.Pull = GPIO_PULLUP;			  
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;	 
	HAL_GPIO_Init(LED1G_GPIO_PORT, &gpio_init_struct);
	gpio_init_struct.Pin = LED1R_GPIO_PIN;			  
	HAL_GPIO_Init(LED1R_GPIO_PORT, &gpio_init_struct);
	gpio_init_struct.Pin = LED1B_GPIO_PIN;			  
	HAL_GPIO_Init(LED1B_GPIO_PORT, &gpio_init_struct);
	
	
	gpio_init_struct.Pin = LED2G_GPIO_PIN;			  
	HAL_GPIO_Init(LED2G_GPIO_PORT, &gpio_init_struct);
	gpio_init_struct.Pin = LED2R_GPIO_PIN;			  
	HAL_GPIO_Init(LED2R_GPIO_PORT, &gpio_init_struct);
	gpio_init_struct.Pin = LED2B_GPIO_PIN;			  
	HAL_GPIO_Init(LED2B_GPIO_PORT, &gpio_init_struct);
	
	gpio_init_struct.Pin = LED3G_GPIO_PIN;			  
	HAL_GPIO_Init(LED3G_GPIO_PORT, &gpio_init_struct);
	gpio_init_struct.Pin = LED3R_GPIO_PIN;			  
	HAL_GPIO_Init(LED3R_GPIO_PORT, &gpio_init_struct);
	gpio_init_struct.Pin = LED3B_GPIO_PIN;			  
	HAL_GPIO_Init(LED3B_GPIO_PORT, &gpio_init_struct);

	gpio_init_struct.Pin = LED4G_GPIO_PIN;			  
	HAL_GPIO_Init(LED4G_GPIO_PORT, &gpio_init_struct);
	gpio_init_struct.Pin = LED4R_GPIO_PIN;			  
	HAL_GPIO_Init(LED4R_GPIO_PORT, &gpio_init_struct);
	gpio_init_struct.Pin = LED4B_GPIO_PIN;			  
	HAL_GPIO_Init(LED4B_GPIO_PORT, &gpio_init_struct);
	
	gpio_init_struct.Pin = LED5G_GPIO_PIN;			  
	HAL_GPIO_Init(LED5G_GPIO_PORT, &gpio_init_struct);
	gpio_init_struct.Pin = LED5R_GPIO_PIN;			  
	HAL_GPIO_Init(LED5R_GPIO_PORT, &gpio_init_struct);
	gpio_init_struct.Pin = LED5B_GPIO_PIN;			  
	HAL_GPIO_Init(LED5B_GPIO_PORT, &gpio_init_struct);
	
	// 初始化 关闭所有 LED
	LED1G(1); 
	LED1R(0);
	LED1B(0);	

	LED2G(0); 
	LED2R(1);
	LED2B(0);	

	LED3G(0); 
	LED3R(0);
	LED3B(1);	
	
	LED4G(1); 
	LED4R(1);
	LED4B(0);	

	LED5G(0); 
	LED5R(1);
	LED5B(1);	
}

/**
 * @brief       
 * @note        
				
 *   @arg
 *
 *   @arg
 * @retval
 *              
 *             
 *             
 */


void LedControl(void)
{
	;


}
