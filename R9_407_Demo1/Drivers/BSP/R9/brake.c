/**
 * @FilePath     : /MDK-ARMc:/Users/fu/Desktop/Code/CodeV1.1/R9_407_V1.1/R9_407_Demo1/Drivers/BSP/R9/brake.c
 * @Description  :  Brake Control and Detection
 * @Author       : lisir
 * @Version      : V1.1
 * @LastEditors  : lisir lisir@rehand.com
 * @LastEditTime : 2024-06-29 10:31:34
 * @Copyright (c) 2024 by Rehand Medical Technology Co., LTD, All Rights Reserved. 
**/
#include "./BSP/R9/brake.h"

#include "./SYSTEM/delay/delay.h"
#include "./BSP/R9/Slavemodbus.h"
TIM_HandleTypeDef g_time9_pwm_chy_handle;

/**
 * @brief        : TIME9init
 * @param         {uint16_t} arr:
 * @param         {uint16_t} psc:
 * @return        {*}
**/
void brakeTime9_init(uint16_t arr, uint16_t psc) 
{
    GPIO_InitTypeDef gpio_init_struct;
    BRAKE_TIM9_PWM_CHY_CLK_ENABLE();                                           /* ����ͨ��y��GPIOʱ�� */
    BRAKE_TIM9_PWM_CH_GPIOE_CLK_ENABLE();                                                    /* ʹ�ܶ�ʱ��ʱ�� */
    gpio_init_struct.Pin = BRAKE_TIM9_PWM_CH1_GPIO | BRAKE_TIM9_PWM_CH2_GPIO; /* ͨ��1 2��CPIO�� */
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                                          /* ����������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                                              /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;                                    /* ���� */
    gpio_init_struct.Alternate = BRAKE_TIM9_PWM_CH_GPIO_AF;                           /* IO��REMAP����, �Ƿ��Ҫ�鿴ͷ�ļ����õ�˵��! */
    HAL_GPIO_Init(BRAKE_TIM9_PWM_CH_PORT, &gpio_init_struct);

    TIM_OC_InitTypeDef tim9_oc_pwm_chy = {0};                                                /* ��ʱ��3������ */
    g_time9_pwm_chy_handle.Instance = BRAKE_TIM9_PWM;                                         /* ��ʱ��x */
    g_time9_pwm_chy_handle.Init.Prescaler = psc;                                             /* Ԥ��Ƶϵ�� */
    g_time9_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;                            /* ��������ģʽ */
    g_time9_pwm_chy_handle.Init.Period = arr;                                                /* �Զ���װ��ֵ */
    HAL_TIM_PWM_Init(&g_time9_pwm_chy_handle);                                               /* ��ʼ��PWM */
    tim9_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                                                /* ģʽѡ��PWM1 */
    tim9_oc_pwm_chy.Pulse = 0;                                                            /* ���ñȽ�ֵ,��ֵ����ȷ��ռ�ձ� */
    tim9_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_HIGH;                                         /* ����Ƚϼ���Ϊ�� */
    HAL_TIM_PWM_ConfigChannel(&g_time9_pwm_chy_handle, &tim9_oc_pwm_chy, BRAKE_TIM9_PWM_CH1); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time9_pwm_chy_handle, BRAKE_TIM9_PWM_CH1);                           /* ������ӦPWMͨ�� */
    HAL_TIM_PWM_ConfigChannel(&g_time9_pwm_chy_handle, &tim9_oc_pwm_chy, BRAKE_TIM9_PWM_CH2); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time9_pwm_chy_handle, BRAKE_TIM9_PWM_CH2);                            /* ������ӦPWMͨ�� */
}
/**
 * @brief        : �������⣬�е����������Ϊ�͵�ƽ����ʼ�� 
 *                 ��������Ĭ���� Drive ״̬
 * @return        {*}
**/

void brakestatedetec_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;                         
    BRAKE1_GPIO_CLK_ENABLE();
    BRAKE2_GPIO_CLK_ENABLE();
    gpio_init_struct.Pin = BRAKE1_GPIO_PIN;                       
    gpio_init_struct.Mode = GPIO_MODE_INPUT;                    
    gpio_init_struct.Pull = GPIO_PULLDOWN;                       
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              
    HAL_GPIO_Init(BRAKE1_GPIO_PORT, &gpio_init_struct); 
    gpio_init_struct.Pin = BRAKE2_GPIO_PIN; 
    HAL_GPIO_Init(BRAKE2_GPIO_PORT, &gpio_init_struct); 
}

/**
 * @brief         : set cycle
 * @param         {uint16_t} ccr1:brake1 cycle  
 * @param         {uint16_t} ccr2:brake2 cycle
 * @return        {*}
**/
void brake_pwm_set(uint16_t ccr1,uint16_t ccr2)
{
    BRAKE_TIM9_PWM_CH1_CCRX = ccr1;

    BRAKE_TIM9_PWM_CH2_CCRX = ccr2;
}

/**
 * @brief        : 
 * @param         {uint8_t} flage: 1:��բ��ס����  0����բ�ɿ�����
 * @return        {*}
**/
void brake(uint8_t flage)
{
    if (flage)
    {
        brake_pwm_set(1 ,1);
    }
    else
    {
        brake_pwm_set(2000 ,2000); 
    }
}

/**
 * @brief        : ��բ��ʼ�� ����ʱ����ʼ������բ����ʼ��
 * @param         {uint16_t} arr:
 * @param         {uint16_t} psc:
 * @return        {*}
**/
void brake_init(uint16_t arr, uint16_t psc)
{
    brakestatedetec_init();
    brakeTime9_init(arr - 1, psc - 1); //168 000 000 / 200*42 Ƶ�� 20kHZ
    brake(1);
}


/**
 * @brief        : �������� 
 * @return        {*}
**/
void brakeDetect(void) // 0 �� Drive  1��Push
{
    if  (LEFT_BREAK_STATE || RIGHT_BRAKE_STATE ) // �ߵ�ƽ ������Ͽ�
    {
        g_slaveReg[4] = 0;
    }

    else
    {
        g_slaveReg[4] = 1;   
    }

    
}