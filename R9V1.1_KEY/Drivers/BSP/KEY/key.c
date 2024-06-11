/**
 ****************************************************************************************************
 * @file        key.c
 * @author      Lisir
 * @version     V1.0
 * @date        2021-10-14
 * @brief       �������� ��������
 * @license     Copyright (c) 2024, ���ڸ���ҽ�ƿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 ****************************************************************************************************
 */

#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/R9/modbus.h"
#include "./BSP/CAN/can.h"

/**
 * @brief       ������ʼ������
 * @param       ��
 * @retval      ��
 */
void key_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;                          /* GPIO���ò����洢���� */
	KEY1_GPIO_CLK_ENABLE(); 
	KEY2_GPIO_CLK_ENABLE();
	KEY3_GPIO_CLK_ENABLE();
	KEY4_GPIO_CLK_ENABLE();
	KEY5_GPIO_CLK_ENABLE();
	KEY6_GPIO_CLK_ENABLE();
	KEY7_GPIO_CLK_ENABLE();
	KEY8_GPIO_CLK_ENABLE();
	KEY9_GPIO_CLK_ENABLE();
	KEY10_GPIO_CLK_ENABLE();                                    /*ʹ��KEY1~KEY10 ʱ��*/
	
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
���� ���� ɨ���ж�
 */

void KeyScan(void)
{

    if(KEY1 == 0)
    {
        delay_ms(10);
        if(KEY1== 0)
        {
			CanKeybufSendSend[1] = 0x0001;
               /* ����1������ */
        }
    }
    else if (KEY2 == 0)
    {  
        delay_ms(10);
        if(KEY2== 0)
        {
			CanKeybufSendSend[1] = 0x0002;
               /* ����2������ */
        }
     }
    else if (KEY3 == 0)
    {  
        delay_ms(10);
        if(KEY3== 0)
        {
			CanKeybufSendSend[1] = 0x0003;
               /* ����3������ */
        }
     } 
    else if (KEY4 == 0)
    {  
        delay_ms(10);
        if(KEY4== 0)
        {
			CanKeybufSendSend[1] = 0x0004;
               /* ����4������ */
        }
     }   
    else if (KEY5 == 0)
    {  
        delay_ms(10);
        if(KEY5== 0)
        {
			CanKeybufSendSend[1] = 0x0005;
               /* ����5������ */
        }
     }
    else if (KEY6 == 0)
    {  
        delay_ms(10);
        if(KEY6== 0)
        {
			CanKeybufSendSend[1] = 0x0006;
               /* ����5������ */
        }
     }
    else if (KEY7 == 0)
    {  
        delay_ms(10);
        if(KEY7== 0)
        {
			CanKeybufSendSend[1] = 0x0007;
               /* ����5������ */
        }
     }
    else if (KEY8 == 0)
    {  
        delay_ms(10);
        if(KEY8== 0)
        {
			CanKeybufSendSend[1] = 0x0008;
               /* ����5������ */
        }
     }
    else if (KEY9 == 0)
    {  
        delay_ms(10);
        if(KEY9== 0)
        {
			CanKeybufSendSend[1] = 0x0009;
               /* ����5������ */
        }
     }
    else if (KEY10 == 0)
    {  
        delay_ms(10);
        if(KEY10== 0)
        {
			CanKeybufSendSend[1] = 0X000A;
               /* ����5������ */
        }
     }
    else
    {
        CanKeybufSendSend[1] = 0;
        
    }
    
    Keysendbuf();

     
}

void Keysendbuf(void)
{
   static uint8_t res;
   res = can_send_msg(0x01, CanKeybufSendSend, 2);    /* ID = 0x01, ����2���ֽ� CanKeybufSendSend[0] 0x68 ; CanKeybufSendSend[1] ����״̬ */
    /*res 0 :�ɹ� 1�� ʧ��*/ 
}
    






  