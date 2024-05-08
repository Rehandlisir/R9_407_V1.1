/**
 ****************************************************************************************************
 * @file        led.c
 * @author      Lisir
 * @version     V1.0
 * @date        2021-10-14
 * @brief       LEDָʾ�� ��������
 * @license     Copyright (c) 2024, ���ڸ���ҽ�ƿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:F407������
 * none
 * none
 * none
 * none
 *
 * �޸�˵��
 * none
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/R9/modbus.h"

/**
 * @brief       ��ʼ��LED���IO��, ��ʹ��ʱ��
 * @param       ��
 * @retval      ��

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
	
	// ��ʼ�� �ر����� LED
	LED1G(0); 
	LED1R(0);
	LED1B(0);	

	LED2G(0); 
	LED2R(0);
	LED2B(0);	

	LED3G(0); 
	LED3R(0);
	LED3B(0);	
	
	LED4G(0); 
	LED4R(0);
	LED4B(0);	

	LED5G(0); 
	LED5R(0);
	LED5B(0);	
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
	 if (KeyReg[0X0091] == 0)
	 {
		LED1G(1);
		LED1R(0);
		LED1B(0);
		
	 }
	 else if (KeyReg[0X0091] == 1)
	 {
		LED1G(1);
		LED1R(1);
		LED1B(0);
	 }
	 else if (KeyReg[0X0091] == 2)
	 {
		LED1G(0);
		LED1R(1);
		LED1B(0);
	 }
	 else
	 {
		LED1G(0); 
		LED1R(0);
		LED1B(0);
	 };
	 

	 if (KeyReg[0X0092] == 0)
	 {
		LED2G(1);
		LED2R(0);
		LED2B(0);
	 }
	 else if (KeyReg[0X0092] == 1)
	 {
		LED2G(1);
		LED2R(1);
		LED2B(0);
	 }
	 else if (KeyReg[0X0092] == 2)
	 {
		LED2G(0);
		LED2R(1);
		LED2B(0);
	 }	
	 else
	 {
		LED2G(0);
		LED2R(0);
		LED2B(0);
	 };

	 if (KeyReg[0X0093] == 0)
	 {
		LED3G(1);
		LED3R(0);
		LED3B(0);
	 }
	 else if (KeyReg[0X0093] == 1)
	 {
		LED3G(1);
		LED3R(1);
		LED3B(0);
	 }
	 else if (KeyReg[0X0093] == 2)
	 {
		LED3G(0);
		LED3R(1);
		LED3B(0);
	 }
	 else
	 {		
		LED3G(0);
		LED3R(0);
		LED3B(0);

	 };

	 if (KeyReg[0X0094] == 0)
	 {
		LED4G(1);
		LED4R(0);
		LED4B(0);
	 }
	 else if (KeyReg[0X0094] == 1)
	 {
		LED4G(1);
		LED4R(1);
		LED4B(0);
	 }
	 else if (KeyReg[0X0094] == 2)
	 {
		LED4G(0);
		LED4R(1);
		LED4B(0);
	 }
	 else
	 {
		LED4G(0);
		LED4R(0);
		LED4B(0);
	 };

	 if (KeyReg[0X0095] == 0)
	 {
		LED5G(1);
		LED5R(0);
		LED5B(0);
	 }
	 else if (KeyReg[0X0095] == 1)
	 {
		LED5G(1);
		LED5R(1);
		LED5B(0);
	 }
	 else if (KeyReg[0X0095] == 2)
	 {
		LED5G(0);
		LED5R(1);
		LED5B(0);
	 }
	else
	{
		LED5G(0);
		LED5R(0);
		LED5B(0);	
	}
}
