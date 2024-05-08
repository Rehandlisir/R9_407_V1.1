/**
 ****************************************************************************************************
 * @file        key.c
 * @author      Lisir
 * @version     V1.0
 * @date        2021-10-14
 * @brief       按键输入 驱动代码
 * @license     Copyright (c) 2024, 深圳复成医疗科技有限公司
 ****************************************************************************************************
 * @attention
 ****************************************************************************************************
 */

#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/R9/modbus.h"

/**
 * @brief       按键初始化函数
 * @param       无
 * @retval      无
 */
void key_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;                          /* GPIO配置参数存储变量 */
	KEY1_GPIO_CLK_ENABLE(); 
	KEY2_GPIO_CLK_ENABLE();
	KEY3_GPIO_CLK_ENABLE();
	KEY4_GPIO_CLK_ENABLE();
	KEY5_GPIO_CLK_ENABLE();
	KEY6_GPIO_CLK_ENABLE();
	KEY7_GPIO_CLK_ENABLE();
	KEY8_GPIO_CLK_ENABLE();
	KEY9_GPIO_CLK_ENABLE();
	KEY10_GPIO_CLK_ENABLE();                                    /*使能KEY1~KEY10 时钟*/
	
    gpio_init_struct.Pin = KEY1_GPIO_PIN;                      
    gpio_init_struct.Mode = GPIO_MODE_INPUT;                    
    gpio_init_struct.Pull = GPIO_PULLUP;                       
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;           
    HAL_GPIO_Init(KEY1_GPIO_PORT, &gpio_init_struct); 
	
	gpio_init_struct.Pin = KEY2_GPIO_PIN;  
    HAL_GPIO_Init(KEY2_GPIO_PORT, &gpio_init_struct);  

	gpio_init_struct.Pin = KEY3_GPIO_PIN;  
    HAL_GPIO_Init(KEY3_GPIO_PORT, &gpio_init_struct); 
	
	gpio_init_struct.Pin = KEY4_GPIO_PIN;  
    HAL_GPIO_Init(KEY4_GPIO_PORT, &gpio_init_struct); 

	gpio_init_struct.Pin = KEY5_GPIO_PIN;  
    HAL_GPIO_Init(KEY5_GPIO_PORT, &gpio_init_struct); 

	gpio_init_struct.Pin = KEY6_GPIO_PIN;  
    HAL_GPIO_Init(KEY6_GPIO_PORT, &gpio_init_struct); 

	gpio_init_struct.Pin = KEY7_GPIO_PIN;  
    HAL_GPIO_Init(KEY7_GPIO_PORT, &gpio_init_struct); 
	
	gpio_init_struct.Pin = KEY8_GPIO_PIN;  
    HAL_GPIO_Init(KEY8_GPIO_PORT, &gpio_init_struct); 
	
	gpio_init_struct.Pin = KEY9_GPIO_PIN;  
    HAL_GPIO_Init(KEY9_GPIO_PORT, &gpio_init_struct); 
	
	gpio_init_struct.Pin = KEY10_GPIO_PIN;  
    HAL_GPIO_Init(KEY10_GPIO_PORT, &gpio_init_struct); 
}

/**

 */


void key_scan1(void)
{
    if(KEY1 == 0)
    {
        delay_ms(20);   /* 消抖 */
        if(KEY1== 0)
        {
			KeyReg[1] = 0x0001;
               /* 按键按下了 */
        }
    }
    else
    {  
		KeyReg[1] = 0x0000;
           /* 按键没有按下 */
     }
   
}


void key_scan2(void)
{
    if(KEY2 == 0)
    {
        delay_ms(20);   /* 消抖 */
        if(KEY2== 0)
        {
			KeyReg[2] = 0x0002;
               /* 按键按下了 */
        }
    }
    else
    { 
		KeyReg[2] = 0x0000;
           /* 按键没有按下 */
        }
   
}

void key_scan3(void)
{
    if(KEY3 == 0)
    {
        delay_ms(20);   /* 消抖 */
        if(KEY3== 0)
        {
			KeyReg[3] = 0x0003;
               /* 按键按下了 */
        }
    }
    else
    { 
        KeyReg[3] = 0x0000;
		   /* 按键没有按下 */
       
	}
   
}


void key_scan4(void)
{
    if(KEY4 == 0)
    {
        delay_ms(20);   /* 消抖 */
        if(KEY4== 0)
        {
			KeyReg[4] = 0x0004;
               /* 按键按下了 */
        }
    }
    else
    { 
		KeyReg[4] = 0x0000;
           /* 按键没有按下 */
        
	}
   
}

void key_scan5(void)
{
    if(KEY5 == 0)
    {
        delay_ms(20);   /* 消抖 */
        if(KEY5== 0)
        {
			KeyReg[5] = 0x0005;
               /* 按键按下了 */
        }
    }
    else
    { 
		KeyReg[5] = 0x0000;
           /* 按键没有按下 */
       
	}
   
}

void key_scan6(void)
{
    if(KEY6 == 0)
    {
        delay_ms(20);   /* 消抖 */
        if(KEY6== 0)
        {
			KeyReg[6] = 0x0006;
               /* 按键按下了 */
        }
    }
    else
    { 
		KeyReg[6] = 0x0000;
           /* 按键没有按下 */
        
	}
   
}

void key_scan7(void)
{
    if(KEY7 == 0)
    {
        delay_ms(20);   /* 消抖 */
        if(KEY7== 0)
        {
			KeyReg[7] = 0x0007;
               /* 按键按下了 */
        }
    }
    else
    { 
		KeyReg[7] = 0x0000;
           /* 按键没有按下 */
        
	}
   
}

void key_scan8(void)
{
    if(KEY8 == 0)
    {
        delay_ms(20);   /* 消抖 */
        if(KEY8== 0)
        {
			KeyReg[8] = 0x0008;
               /* 按键按下了 */
        }
    }
    else
    { 
		KeyReg[8] = 0x0000;
           /* 按键没有按下 */
        
	}
   
}

void key_scan9(void)
{
    if(KEY9 == 0)
    {
        delay_ms(20);   /* 消抖 */
        if(KEY9== 0)
        {
			KeyReg[9] = 0x0009;
               /* 按键按下了 */
        }
    }
    else
    { 
		KeyReg[9] = 0x0000;
           /* 按键没有按下 */
        
	}
   
}

void key_scan10(void)
{
    if(KEY10 == 0)
    {
        delay_ms(20);   /* 消抖 */
        if(KEY10== 0)
        {
			KeyReg[10] = 0x000A;
               /* 按键按下了 */
        }
    }
    else
    { 
		KeyReg[10] = 0x0000;
           /* 按键没有按下 */
    }
   
}
void KeyScan(void)
{
	key_scan1();
	key_scan2();
	key_scan3();
	key_scan4();
	key_scan5();
	key_scan6();
	key_scan7();
	key_scan8();
	key_scan9();
	key_scan10();
}



  