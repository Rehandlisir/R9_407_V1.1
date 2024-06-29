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
    BRAKE_TIM9_PWM_CHY_CLK_ENABLE();                                           /* 开启通道y的GPIO时钟 */
    BRAKE_TIM9_PWM_CH_GPIOE_CLK_ENABLE();                                                    /* 使能定时器时钟 */
    gpio_init_struct.Pin = BRAKE_TIM9_PWM_CH1_GPIO | BRAKE_TIM9_PWM_CH2_GPIO; /* 通道1 2的CPIO口 */
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                                          /* 复用推完输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                                              /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;                                    /* 高速 */
    gpio_init_struct.Alternate = BRAKE_TIM9_PWM_CH_GPIO_AF;                           /* IO口REMAP设置, 是否必要查看头文件配置的说明! */
    HAL_GPIO_Init(BRAKE_TIM9_PWM_CH_PORT, &gpio_init_struct);

    TIM_OC_InitTypeDef tim9_oc_pwm_chy = {0};                                                /* 定时器3输出句柄 */
    g_time9_pwm_chy_handle.Instance = BRAKE_TIM9_PWM;                                         /* 定时器x */
    g_time9_pwm_chy_handle.Init.Prescaler = psc;                                             /* 预分频系数 */
    g_time9_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;                            /* 递增计数模式 */
    g_time9_pwm_chy_handle.Init.Period = arr;                                                /* 自动重装载值 */
    HAL_TIM_PWM_Init(&g_time9_pwm_chy_handle);                                               /* 初始化PWM */
    tim9_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                                                /* 模式选择PWM1 */
    tim9_oc_pwm_chy.Pulse = 0;                                                            /* 设置比较值,此值用来确定占空比 */
    tim9_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_HIGH;                                         /* 输出比较极性为低 */
    HAL_TIM_PWM_ConfigChannel(&g_time9_pwm_chy_handle, &tim9_oc_pwm_chy, BRAKE_TIM9_PWM_CH1); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time9_pwm_chy_handle, BRAKE_TIM9_PWM_CH1);                           /* 开启对应PWM通道 */
    HAL_TIM_PWM_ConfigChannel(&g_time9_pwm_chy_handle, &tim9_oc_pwm_chy, BRAKE_TIM9_PWM_CH2); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time9_pwm_chy_handle, BRAKE_TIM9_PWM_CH2);                            /* 开启对应PWM通道 */
}
/**
 * @brief        : 离合器检测，有电流离合输入为低电平，初始化 
 *                 下拉，先默认是 Drive 状态
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
 * @param         {uint8_t} flage: 1:抱闸锁住轮子  0：抱闸松开轮子
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
 * @brief        : 抱闸初始化 ：定时器初始化、抱闸检测初始化
 * @param         {uint16_t} arr:
 * @param         {uint16_t} psc:
 * @return        {*}
**/
void brake_init(uint16_t arr, uint16_t psc)
{
    brakestatedetec_init();
    brakeTime9_init(arr - 1, psc - 1); //168 000 000 / 200*42 频率 20kHZ
    brake(1);
}


/**
 * @brief        : 离合器检测 
 * @return        {*}
**/
void brakeDetect(void) // 0 ： Drive  1：Push
{
    if  (LEFT_BREAK_STATE || RIGHT_BRAKE_STATE ) // 高电平 离合器断开
    {
        g_slaveReg[4] = 0;
    }

    else
    {
        g_slaveReg[4] = 1;   
    }

    
}