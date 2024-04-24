/**
 ****************************************************************************************************
 * @file        moterdriver.c
 * @author      Lisir
 * @version     V1.0
 * @date        2021-10-14
 * @brief       moterdriver 驱动代码
 * @license     Copyright (c) 2024, 深圳复成医疗科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:F407开发板
 * none
 * none
 * none
 * none
 * 定时器描述：
 *     底盘 L  - gtim_tim5_pwm_chy_init - CH1 CH2 ----------
 *     底盘 R  - gtim_tim9_pwm_chy_init - CH1 CH2
 *     推杆1 （举升） gtim_tim1_pwm_chy_init - CH1 CH2
 *     推杆2 （座板） gtim_tim8_pwm_chy_init -  CH1 CH2
 *     推杆3 （靠背） gtim_tim8_pwm_chy_init -  CH3 CH4
 *     推杆4 （脚踏旋转） gtim_tim4_pwm_chy_init -  CH3 CH4
 *     推杆5 （脚踏伸长） gtim_tim4_pwm_chy_init -  CH1 CH2
 *     推杆6 （前支撑轮） gtim_tim12_pwm_chy_init -  CH1 CH2
 * 
 * 
 * 
 * 修改说明
 * none
 * 第一次发布
 *
 ****************************************************************************************************
 */

#include "./BSP/R9/moterdriver.h"
TIM_HandleTypeDef g_time5_pwm_chy_handle ;  /* 底盘L电机 1 函数句柄*/
TIM_HandleTypeDef g_time9_pwm_chy_handle ;  /* 底盘R电机 2 函数句柄*/
TIM_HandleTypeDef g_time1_pwm_chy_handle ;  /* 推杆1 （举升）  TIME1 函数句柄*/
TIM_HandleTypeDef g_time8_pwm_chy_handle ;  /* 推杆2 （座板）    推杆3 （靠背） TIME8 函数句柄*/
TIM_HandleTypeDef g_time4_pwm_chy_handle ;  /* 推杆4 （脚踏旋转） 推杆5 （脚踏伸长）TIME1 函数句柄*/
TIM_HandleTypeDef g_time12_pwm_chy_handle ;  /*推杆6 （前支撑轮） TIME12 函数句柄*/
void MoterL_pwm_chy_init(uint16_t arr, uint16_t psc)  // 左轮电机
{
    /*左右轮电机*/
    TIM_OC_InitTypeDef tim5_oc_pwm_chy = {0};                       /* 定时器5输出句柄 */
    g_time5_pwm_chy_handle.Instance = GTIM_TIM5_PWM;                 /* 定时器x */
    g_time5_pwm_chy_handle.Init.Prescaler = psc;                      /* 预分频系数 */
    g_time5_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;    /* 递增计数模式 */
    g_time5_pwm_chy_handle.Init.Period = arr;                        /* 自动重装载值 */
    HAL_TIM_PWM_Init(&g_time5_pwm_chy_handle);                       /* 初始化PWM */
    tim5_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                       /* 模式选择PWM1 */
    tim5_oc_pwm_chy.Pulse = 0;                                /* 设置比较值,此值用来确定占空比 */
    tim5_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;                                        /* 输出比较极性为低 */
    HAL_TIM_PWM_ConfigChannel(&g_time5_pwm_chy_handle, &tim5_oc_pwm_chy, GTIM_TIM5_PWM_CH1); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time5_pwm_chy_handle, GTIM_TIM5_PWM_CH1);                           /* 开启对应PWM通道 */
	HAL_TIM_PWM_ConfigChannel(&g_time5_pwm_chy_handle, &tim5_oc_pwm_chy, GTIM_TIM5_PWM_CH2); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time5_pwm_chy_handle, GTIM_TIM5_PWM_CH2);                           /* 开启对应PWM通道 */
}
void  MoterR_pwm_chy_init(uint16_t arr, uint16_t psc) // 右轮电机
{
    /*右轮电机*/
    TIM_OC_InitTypeDef tim9_oc_pwm_chy = {0};                       /* 定时器9输出句柄 */
    g_time9_pwm_chy_handle.Instance = GTIM_TIM9_PWM;                 /* 定时器x */
    g_time9_pwm_chy_handle.Init.Prescaler = psc;                     /* 预分频系数 */
    g_time9_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;    /* 递增计数模式 */
    g_time9_pwm_chy_handle.Init.Period = arr;                        /* 自动重装载值 */
    HAL_TIM_PWM_Init(&g_time9_pwm_chy_handle);                       /* 初始化PWM */
    tim9_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                       /* 模式选择PWM1 */
    tim9_oc_pwm_chy.Pulse = 0;                                /* 设置比较值,此值用来确定占空比 */
    tim9_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;                                        /* 输出比较极性为低 */
    HAL_TIM_PWM_ConfigChannel(&g_time9_pwm_chy_handle, &tim9_oc_pwm_chy, GTIM_TIM9_PWM_CH1); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time9_pwm_chy_handle, GTIM_TIM9_PWM_CH1);                           /* 开启对应PWM通道 */
	HAL_TIM_PWM_ConfigChannel(&g_time9_pwm_chy_handle, &tim9_oc_pwm_chy, GTIM_TIM9_PWM_CH2); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time9_pwm_chy_handle, GTIM_TIM9_PWM_CH2);                           /* 开启对应PWM通道 */
}

void  MoterLift_pwm_chy_init(uint16_t arr, uint16_t psc) // 电动推杆 1 
{
    /*推杆电机1（举升）*/
    TIM_OC_InitTypeDef tim1_oc_pwm_chy = {0};                       /* 定时器9输出句柄 */
    g_time1_pwm_chy_handle.Instance = GTIM_TIM1_PWM;                 /* 定时器x */
    g_time1_pwm_chy_handle.Init.Prescaler = psc;                     /* 预分频系数 */
    g_time1_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;    /* 递增计数模式 */
    g_time1_pwm_chy_handle.Init.Period = arr;                        /* 自动重装载值 */
    HAL_TIM_PWM_Init(&g_time1_pwm_chy_handle);                       /* 初始化PWM */
    tim1_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                       /* 模式选择PWM1 */
    tim1_oc_pwm_chy.Pulse = 0;                                /* 设置比较值,此值用来确定占空比 */
    tim1_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;                                        /* 输出比较极性为低 */
    HAL_TIM_PWM_ConfigChannel(&g_time1_pwm_chy_handle, &tim1_oc_pwm_chy, GTIM_TIM1_PWM_CH1); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH1);                           /* 开启对应PWM通道 */
	HAL_TIM_PWM_ConfigChannel(&g_time1_pwm_chy_handle, &tim1_oc_pwm_chy, GTIM_TIM1_PWM_CH2); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH2);                           /* 开启对应PWM通道 */
}

void  MoterPedestal_Backboard_pwm_chy_init(uint16_t arr, uint16_t psc) // 电动推杆 2&3 
{
    /*推杆电机2（座板）*/
    TIM_OC_InitTypeDef tim8_oc_pwm_chy = {0};                       /* 定时器9输出句柄 */
    g_time8_pwm_chy_handle.Instance = GTIM_TIM8_PWM;                 /* 定时器x */
    g_time8_pwm_chy_handle.Init.Prescaler = psc;                     /* 预分频系数 */
    g_time8_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;    /* 递增计数模式 */
    g_time8_pwm_chy_handle.Init.Period = arr;                        /* 自动重装载值 */
    HAL_TIM_PWM_Init(&g_time8_pwm_chy_handle);                       /* 初始化PWM */
    tim8_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                       /* 模式选择PWM1 */
    tim8_oc_pwm_chy.Pulse = 0;                                /* 设置比较值,此值用来确定占空比 */
    tim8_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;                                        /* 输出比较极性为低 */
    HAL_TIM_PWM_ConfigChannel(&g_time8_pwm_chy_handle, &tim8_oc_pwm_chy, GTIM_TIM8_PWM_CH3); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH3);                           /* 开启对应PWM通道 */
	HAL_TIM_PWM_ConfigChannel(&g_time8_pwm_chy_handle, &tim8_oc_pwm_chy, GTIM_TIM8_PWM_CH4); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH4);                           /* 开启对应PWM通道 */
    /*推杆电机3（靠背）*/
    HAL_TIM_PWM_ConfigChannel(&g_time8_pwm_chy_handle, &tim8_oc_pwm_chy, GTIM_TIM8_PWM_CH1); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH1);                           /* 开启对应PWM通道 */
	HAL_TIM_PWM_ConfigChannel(&g_time8_pwm_chy_handle, &tim8_oc_pwm_chy, GTIM_TIM8_PWM_CH2); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH2);                           /* 开启对应PWM通道 */

}

void  MoterLeg_pwm_chy_init(uint16_t arr, uint16_t psc) // 电动推杆 4&5 
{
   // 推杆4 （脚踏旋转）
    TIM_OC_InitTypeDef tim4_oc_pwm_chy = {0};                       /* 定时器9输出句柄 */
    g_time4_pwm_chy_handle.Instance = GTIM_TIM4_PWM;                 /* 定时器x */
    g_time4_pwm_chy_handle.Init.Prescaler = psc;                     /* 预分频系数 */
    g_time4_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;    /* 递增计数模式 */
    g_time4_pwm_chy_handle.Init.Period = arr;                        /* 自动重装载值 */
    HAL_TIM_PWM_Init(&g_time4_pwm_chy_handle);                       /* 初始化PWM */
    tim4_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                       /* 模式选择PWM1 */
    tim4_oc_pwm_chy.Pulse = 0;                                /* 设置比较值,此值用来确定占空比 */
    tim4_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;                                        /* 输出比较极性为低 */
    HAL_TIM_PWM_ConfigChannel(&g_time4_pwm_chy_handle, &tim4_oc_pwm_chy, GTIM_TIM4_PWM_CH3); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH3);                           /* 开启对应PWM通道 */
	HAL_TIM_PWM_ConfigChannel(&g_time4_pwm_chy_handle, &tim4_oc_pwm_chy, GTIM_TIM4_PWM_CH4); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH4);                           /* 开启对应PWM通道 */
    /* 推杆5 （脚踏伸长）*/
    HAL_TIM_PWM_ConfigChannel(&g_time4_pwm_chy_handle, &tim4_oc_pwm_chy, GTIM_TIM4_PWM_CH1); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH1);                           /* 开启对应PWM通道 */
	HAL_TIM_PWM_ConfigChannel(&g_time4_pwm_chy_handle, &tim4_oc_pwm_chy, GTIM_TIM4_PWM_CH2); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH2);                           /* 开启对应PWM通道 */

}

void MoterSupport_pwm_chy_init(uint16_t arr, uint16_t psc) // 电动推杆 6 
{
    /*推杆电机6 前支撑*/
    TIM_OC_InitTypeDef tim12_oc_pwm_chy = {0};                       /* 定时器12输出句柄 */
    g_time12_pwm_chy_handle.Instance = GTIM_TIM12_PWM;                 /* 定时器x */
    g_time12_pwm_chy_handle.Init.Prescaler = psc;                     /* 预分频系数 */
    g_time12_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;    /* 递增计数模式 */
    g_time12_pwm_chy_handle.Init.Period = arr;                        /* 自动重装载值 */
    HAL_TIM_PWM_Init(&g_time12_pwm_chy_handle);                       /* 初始化PWM */
    tim12_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                       /* 模式选择PWM1 */
    tim12_oc_pwm_chy.Pulse = 0;                                /* 设置比较值,此值用来确定占空比 */
    tim12_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;                                        /* 输出比较极性为低 */
    HAL_TIM_PWM_ConfigChannel(&g_time12_pwm_chy_handle, &tim12_oc_pwm_chy, GTIM_TIM12_PWM_CH1); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time12_pwm_chy_handle, GTIM_TIM12_PWM_CH1);                           /* 开启对应PWM通道 */
	HAL_TIM_PWM_ConfigChannel(&g_time12_pwm_chy_handle, &tim12_oc_pwm_chy, GTIM_TIM12_PWM_CH2); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time12_pwm_chy_handle, GTIM_TIM12_PWM_CH2);                           /* 开启对应PWM通道 */
}

/**
 * @brief       定时器底层驱动，时钟使能，引脚配置
                此函数会被HAL_TIM_PWM_Init()调用
 * @param       htim:定时器句柄
 * @retval      无
 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == GTIM_TIM5_PWM)
    {   // 左轮
        GPIO_InitTypeDef gpio_init_structA;
		GTIM_TIM5_PWM_CHY_GPIOA_CLK_ENABLE();                            /* 开启通道y的GPIO时钟 */
        GTIM_TIM5_PWM_CHY_CLK_ENABLE();                                 /* 使能定时器时钟 */
        gpio_init_structA.Pin = GTIM_TIM5_PWM_CHY_GPIO_PIN0 | GTIM_TIM5_PWM_CHY_GPIO_PIN1;              /* 通道1 2的CPIO口 */
        gpio_init_structA.Mode = GPIO_MODE_AF_PP;                        /* 复用推完输出 */
        gpio_init_structA.Pull = GPIO_PULLUP;                            /* 上拉 */
        gpio_init_structA.Speed = GPIO_SPEED_FREQ_HIGH;                  /* 高速 */
        gpio_init_structA.Alternate = GTIM_TIM5_PWM_CHY_GPIO_AF;         /* IO口REMAP设置, 是否必要查看头文件配置的说明! */
        HAL_GPIO_Init(GTIM_TIM5_PWM_CHY_GPIO_PORTA, &gpio_init_structA);

    }
    if (htim->Instance == GTIM_TIM9_PWM)
    {   // 右轮
        GPIO_InitTypeDef gpio_init_structE;
		GTIM_TIM9_PWM_CHY_GPIOE_CLK_ENABLE();                            /* 开启通道y的GPIO时钟 */
        GTIM_TIM9_PWM_CHY_CLK_ENABLE();                                 /* 使能定时器时钟 */
        gpio_init_structE.Pin = GTIM_TIM9_PWM_CHY_GPIO_PIN5 | GTIM_TIM9_PWM_CHY_GPIO_PIN6;              /* 通道1 2的CPIO口 */
        gpio_init_structE.Mode = GPIO_MODE_AF_PP;                        /* 复用推完输出 */
        gpio_init_structE.Pull = GPIO_PULLUP;                            /* 上拉 */
        gpio_init_structE.Speed = GPIO_SPEED_FREQ_HIGH;                  /* 高速 */
        gpio_init_structE.Alternate = GTIM_TIM9_PWM_CHY_GPIO_AF;         /* IO口REMAP设置, 是否必要查看头文件配置的说明! */
        HAL_GPIO_Init(GTIM_TIM9_PWM_CHY_GPIO_PORTE, &gpio_init_structE);
		
    }

    if (htim->Instance == GTIM_TIM1_PWM)
    {
        // 推杆1
        GPIO_InitTypeDef gpio_init_structA;
		GTIM_TIM1_PWM_CHY_GPIOA_CLK_ENABLE();                            /* 开启通道y的GPIO时钟 */
        GTIM_TIM1_PWM_CHY_CLK_ENABLE();                                 /* 使能定时器时钟 */
        gpio_init_structA.Pin = GTIM_TIM1_PWM_CHY_GPIO_PIN8 | GTIM_TIM1_PWM_CHY_GPIO_PIN9;              /* 通道1 2的CPIO口 */
        gpio_init_structA.Mode = GPIO_MODE_AF_PP;                        /* 复用推完输出 */
        gpio_init_structA.Pull = GPIO_PULLUP;                            /* 上拉 */
        gpio_init_structA.Speed = GPIO_SPEED_FREQ_HIGH;                  /* 高速 */
        gpio_init_structA.Alternate = GTIM_TIM1_PWM_CHY_GPIO_AF;         /* IO口REMAP设置, 是否必要查看头文件配置的说明! */
        HAL_GPIO_Init(GTIM_TIM1_PWM_CHY_GPIO_PORTA, &gpio_init_structA);
		
    }

    if (htim->Instance == GTIM_TIM8_PWM)
    {
        // 推杆2&3
        GPIO_InitTypeDef gpio_init_structC;
		GTIM_TIM8_PWM_CHY_GPIOC_CLK_ENABLE();                            /* 开启通道y的GPIO时钟 */
        GTIM_TIM8_PWM_CHY_CLK_ENABLE();                                 /* 使能定时器时钟 */
        gpio_init_structC.Pin = GTIM_TIM8_PWM_CHY_GPIO_PIN8 | GTIM_TIM8_PWM_CHY_GPIO_PIN9|GTIM_TIM8_PWM_CHY_GPIO_PIN6|GTIM_TIM8_PWM_CHY_GPIO_PIN7;              /* 通道1 2的CPIO口 */
        gpio_init_structC.Mode = GPIO_MODE_AF_PP;                        /* 复用推完输出 */
        gpio_init_structC.Pull = GPIO_PULLUP;                            /* 上拉 */
        gpio_init_structC.Speed = GPIO_SPEED_FREQ_HIGH;                  /* 高速 */
        gpio_init_structC.Alternate = GTIM_TIM8_PWM_CHY_GPIO_AF;         /* IO口REMAP设置, 是否必要查看头文件配置的说明! */
        HAL_GPIO_Init(GTIM_TIM8_PWM_CHY_GPIO_PORTC, &gpio_init_structC);
		
    }


    if (htim->Instance == GTIM_TIM4_PWM)
    {
        // 推杆4&5
        GPIO_InitTypeDef gpio_init_structD;
		GTIM_TIM4_PWM_CHY_GPIOD_CLK_ENABLE();                            /* 开启通道y的GPIO时钟 */
        GTIM_TIM4_PWM_CHY_CLK_ENABLE();                                 /* 使能定时器时钟 */
        gpio_init_structD.Pin = GTIM_TIM4_PWM_CHY_GPIO_PIN12 | GTIM_TIM4_PWM_CHY_GPIO_PIN13|GTIM_TIM4_PWM_CHY_GPIO_PIN14|GTIM_TIM4_PWM_CHY_GPIO_PIN15;              /* 通道1 2的CPIO口 */
        gpio_init_structD.Mode = GPIO_MODE_AF_PP;                        /* 复用推完输出 */
        gpio_init_structD.Pull = GPIO_PULLUP;                            /* 上拉 */
        gpio_init_structD.Speed = GPIO_SPEED_FREQ_HIGH;                  /* 高速 */
        gpio_init_structD.Alternate = GTIM_TIM4_PWM_CHY_GPIO_AF;         /* IO口REMAP设置, 是否必要查看头文件配置的说明! */
        HAL_GPIO_Init(GTIM_TIM4_PWM_CHY_GPIO_PORTD, &gpio_init_structD);
		
    }

    if (htim->Instance == GTIM_TIM12_PWM)
    {
        // 推杆6
        GPIO_InitTypeDef gpio_init_structB;
		GTIM_TIM12_PWM_CHY_GPIOB_CLK_ENABLE();                            /* 开启通道y的GPIO时钟 */
        GTIM_TIM12_PWM_CHY_CLK_ENABLE();                                 /* 使能定时器时钟 */
        gpio_init_structB.Pin = GTIM_TIM12_PWM_CHY_GPIO_PIN14 | GTIM_TIM12_PWM_CHY_GPIO_PIN15;              /* 通道1 2的CPIO口 */
        gpio_init_structB.Mode = GPIO_MODE_AF_PP;                        /* 复用推完输出 */
        gpio_init_structB.Pull = GPIO_PULLUP;                            /* 上拉 */
        gpio_init_structB.Speed = GPIO_SPEED_FREQ_HIGH;                  /* 高速 */
        gpio_init_structB.Alternate = GTIM_TIM12_PWM_CHY_GPIO_AF;         /* IO口REMAP设置, 是否必要查看头文件配置的说明! */
        HAL_GPIO_Init(GTIM_TIM12_PWM_CHY_GPIO_PORTB, &gpio_init_structB);
		
    }
    
}



	