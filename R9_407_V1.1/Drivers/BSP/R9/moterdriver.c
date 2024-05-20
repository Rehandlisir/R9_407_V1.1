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
 *     底盘 L  - gtim_tim3_pwm_chy_init - CH3 CH4 ----------
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

TIM_HandleTypeDef g_time3_pwm_chy_handle;  /* 底盘L电机 1 函数句柄*/
TIM_HandleTypeDef g_time9_pwm_chy_handle;  /* 底盘R电机 2 函数句柄*/
TIM_HandleTypeDef g_time1_pwm_chy_handle;  /* 推杆1 （举升）  TIME1 函数句柄*/
TIM_HandleTypeDef g_time8_pwm_chy_handle;  /* 推杆2 （座板）    推杆3 （靠背） TIME8 函数句柄*/
TIM_HandleTypeDef g_time4_pwm_chy_handle;  /* 推杆4 （脚踏旋转） 推杆5 （脚踏伸长）TIME1 函数句柄*/
TIM_HandleTypeDef g_time12_pwm_chy_handle; /*推杆6 （前支撑轮） TIME12 函数句柄*/

LineActor_State linerun_state;
// MoterL_pwm_chy_init(100 - 1, 42 - 1);    //* 84 000 000 / 100*42      L 20khz频率的PWM 波形*  /
// MoterR_pwm_chy_init(200 - 1, 42 - 1);    //* 168 000 000 / 200*42     R  20khz频率的PWM 波形*/
// MoterLift_pwm_chy_init(200 - 1, 42 - 1);    //* 168 000 000 / 200*42     T1  20khz频率的PWM 波形*/
// MoterPedestal_Backboard_pwm_chy_init(200 - 1, 42 - 1);//                   T2&T3
// MoterLeg_pwm_chy_init(100 - 1, 42 - 1);               //                   T4&T5
// MoterSupport_pwm_chy_init(100 - 1, 42 - 1);   //* 84 000 000 / 100*42      T6  20khz频率的PWM 波形*  /
void MoterL_pwm_chy_init(uint16_t arr, uint16_t psc) // 左轮电机
{
    /*左右轮电机*/
    TIM_OC_InitTypeDef tim3_oc_pwm_chy = {0};                                                /* 定时器3输出句柄 */
    g_time3_pwm_chy_handle.Instance = GTIM_TIM3_PWM;                                         /* 定时器x */
    g_time3_pwm_chy_handle.Init.Prescaler = psc;                                             /* 预分频系数 */
    g_time3_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;                            /* 递增计数模式 */
    g_time3_pwm_chy_handle.Init.Period = arr;                                                /* 自动重装载值 */
    HAL_TIM_PWM_Init(&g_time3_pwm_chy_handle);                                               /* 初始化PWM */
    tim3_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                                                /* 模式选择PWM1 */
    tim3_oc_pwm_chy.Pulse = 0;                                                               /* 设置比较值,此值用来确定占空比 */
    tim3_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;                                         /* 输出比较极性为低 */
    HAL_TIM_PWM_ConfigChannel(&g_time3_pwm_chy_handle, &tim3_oc_pwm_chy, GTIM_TIM3_PWM_CH3); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time3_pwm_chy_handle, GTIM_TIM3_PWM_CH3);                           /* 开启对应PWM通道 */
    HAL_TIM_PWM_ConfigChannel(&g_time3_pwm_chy_handle, &tim3_oc_pwm_chy, GTIM_TIM3_PWM_CH4); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time3_pwm_chy_handle, GTIM_TIM3_PWM_CH4);                           /* 开启对应PWM通道 */
}
void MoterR_pwm_chy_init(uint16_t arr, uint16_t psc) // 右轮电机
{
    /*右轮电机*/
    TIM_OC_InitTypeDef tim9_oc_pwm_chy = {0};                                                /* 定时器9输出句柄 */
    g_time9_pwm_chy_handle.Instance = GTIM_TIM9_PWM;                                         /* 定时器x */
    g_time9_pwm_chy_handle.Init.Prescaler = psc;                                             /* 预分频系数 */
    g_time9_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;                            /* 递增计数模式 */
    g_time9_pwm_chy_handle.Init.Period = arr;                                                /* 自动重装载值 */
    HAL_TIM_PWM_Init(&g_time9_pwm_chy_handle);                                               /* 初始化PWM */
    tim9_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                                                /* 模式选择PWM1 */
    tim9_oc_pwm_chy.Pulse = 0;                                                               /* 设置比较值,此值用来确定占空比 */
    tim9_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;                                         /* 输出比较极性为低 */
    HAL_TIM_PWM_ConfigChannel(&g_time9_pwm_chy_handle, &tim9_oc_pwm_chy, GTIM_TIM9_PWM_CH1); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time9_pwm_chy_handle, GTIM_TIM9_PWM_CH1);                           /* 开启对应PWM通道 */
    HAL_TIM_PWM_ConfigChannel(&g_time9_pwm_chy_handle, &tim9_oc_pwm_chy, GTIM_TIM9_PWM_CH2); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time9_pwm_chy_handle, GTIM_TIM9_PWM_CH2);                           /* 开启对应PWM通道 */
}

/*靠背角度撑杆A1*/
void MoterLift_pwm_chy_init(uint16_t arr, uint16_t psc)
{

    TIM_OC_InitTypeDef tim1_oc_pwm_chy = {0};                                                /* 定时器9输出句柄 */
    g_time1_pwm_chy_handle.Instance = GTIM_TIM1_PWM;                                         /* 定时器x */
    g_time1_pwm_chy_handle.Init.Prescaler = psc;                                             /* 预分频系数 */
    g_time1_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;                            /* 递增计数模式 */
    g_time1_pwm_chy_handle.Init.Period = arr;                                                /* 自动重装载值 */
    HAL_TIM_PWM_Init(&g_time1_pwm_chy_handle);                                               /* 初始化PWM */
    tim1_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                                                /* 模式选择PWM1 */
    tim1_oc_pwm_chy.Pulse = 0;                                                               /* 设置比较值,此值用来确定占空比 */
    tim1_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;                                         /* 输出比较极性为低 */
    HAL_TIM_PWM_ConfigChannel(&g_time1_pwm_chy_handle, &tim1_oc_pwm_chy, GTIM_TIM1_PWM_CH3); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH3);                           /* 开启对应PWM通道 */
    HAL_TIM_PWM_ConfigChannel(&g_time1_pwm_chy_handle, &tim1_oc_pwm_chy, GTIM_TIM1_PWM_CH4); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH4);                           /* 开启对应PWM通道 */
}

/*底盘举升撑杆B1 / 座板角度撑杆B2*/
void MoterPedestal_Backboard_pwm_chy_init(uint16_t arr, uint16_t psc) // 电动推杆 2&3
{
    /*推杆电机2（座板）*/
    TIM_OC_InitTypeDef tim8_oc_pwm_chy = {0};                                                /* 定时器9输出句柄 */
    g_time8_pwm_chy_handle.Instance = GTIM_TIM8_PWM;                                         /* 定时器x */
    g_time8_pwm_chy_handle.Init.Prescaler = psc;                                             /* 预分频系数 */
    g_time8_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;                            /* 递增计数模式 */
    g_time8_pwm_chy_handle.Init.Period = arr;                                                /* 自动重装载值 */
    HAL_TIM_PWM_Init(&g_time8_pwm_chy_handle);                                               /* 初始化PWM */
    tim8_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                                                /* 模式选择PWM1 */
    tim8_oc_pwm_chy.Pulse = 0;                                                               /* 设置比较值,此值用来确定占空比 */
    tim8_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;                                         /* 输出比较极性为低 */
    HAL_TIM_PWM_ConfigChannel(&g_time8_pwm_chy_handle, &tim8_oc_pwm_chy, GTIM_TIM8_PWM_CH3); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH3);                           /* 开启对应PWM通道 */
    HAL_TIM_PWM_ConfigChannel(&g_time8_pwm_chy_handle, &tim8_oc_pwm_chy, GTIM_TIM8_PWM_CH4); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH4);                           /* 开启对应PWM通道 */
    /*举升*/
    HAL_TIM_PWM_ConfigChannel(&g_time8_pwm_chy_handle, &tim8_oc_pwm_chy, GTIM_TIM8_PWM_CH1); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH1);                           /* 开启对应PWM通道 */
    HAL_TIM_PWM_ConfigChannel(&g_time8_pwm_chy_handle, &tim8_oc_pwm_chy, GTIM_TIM8_PWM_CH2); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH2);                           /* 开启对应PWM通道 */
}

/*腿托角度撑杆A2 (L)、腿托长度撑杆A3(L)*/
void MoterLeg_pwm_chy_init(uint16_t arr, uint16_t psc) // 电动推杆 4&5
{
    // 腿托角度撑杆A2
    TIM_OC_InitTypeDef tim4_oc_pwm_chy = {0};                                                /* 定时器9输出句柄 */
    g_time4_pwm_chy_handle.Instance = GTIM_TIM4_PWM;                                         /* 定时器x */
    g_time4_pwm_chy_handle.Init.Prescaler = psc;                                             /* 预分频系数 */
    g_time4_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;                            /* 递增计数模式 */
    g_time4_pwm_chy_handle.Init.Period = arr;                                                /* 自动重装载值 */
    HAL_TIM_PWM_Init(&g_time4_pwm_chy_handle);                                               /* 初始化PWM */
    tim4_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                                                /* 模式选择PWM1 */
    tim4_oc_pwm_chy.Pulse = 0;                                                               /* 设置比较值,此值用来确定占空比 */
    tim4_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;                                         /* 输出比较极性为低 */
    HAL_TIM_PWM_ConfigChannel(&g_time4_pwm_chy_handle, &tim4_oc_pwm_chy, GTIM_TIM4_PWM_CH3); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH3);                           /* 开启对应PWM通道 */
    HAL_TIM_PWM_ConfigChannel(&g_time4_pwm_chy_handle, &tim4_oc_pwm_chy, GTIM_TIM4_PWM_CH4); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH4);                           /* 开启对应PWM通道 */
    /* 推杆5 腿托长度撑杆A3*/
    HAL_TIM_PWM_ConfigChannel(&g_time4_pwm_chy_handle, &tim4_oc_pwm_chy, GTIM_TIM4_PWM_CH1); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH1);                           /* 开启对应PWM通道 */
    HAL_TIM_PWM_ConfigChannel(&g_time4_pwm_chy_handle, &tim4_oc_pwm_chy, GTIM_TIM4_PWM_CH2); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH2);                           /* 开启对应PWM通道 */
}

void MoterSupport_pwm_chy_init(uint16_t arr, uint16_t psc) // 电动推杆 6
{
    /*推杆电机6 前支撑*/
    TIM_OC_InitTypeDef tim12_oc_pwm_chy = {0};                                                  /* 定时器12输出句柄 */
    g_time12_pwm_chy_handle.Instance = GTIM_TIM12_PWM;                                          /* 定时器x */
    g_time12_pwm_chy_handle.Init.Prescaler = psc;                                               /* 预分频系数 */
    g_time12_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;                              /* 递增计数模式 */
    g_time12_pwm_chy_handle.Init.Period = arr;                                                  /* 自动重装载值 */
    HAL_TIM_PWM_Init(&g_time12_pwm_chy_handle);                                                 /* 初始化PWM */
    tim12_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                                                  /* 模式选择PWM1 */
    tim12_oc_pwm_chy.Pulse = 0;                                                                 /* 设置比较值,此值用来确定占空比 */
    tim12_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;                                           /* 输出比较极性为低 */
    HAL_TIM_PWM_ConfigChannel(&g_time12_pwm_chy_handle, &tim12_oc_pwm_chy, GTIM_TIM12_PWM_CH1); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time12_pwm_chy_handle, GTIM_TIM12_PWM_CH1);                            /* 开启对应PWM通道 */
    HAL_TIM_PWM_ConfigChannel(&g_time12_pwm_chy_handle, &tim12_oc_pwm_chy, GTIM_TIM12_PWM_CH2); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_time12_pwm_chy_handle, GTIM_TIM12_PWM_CH2);                            /* 开启对应PWM通道 */
}

/**
 * @brief       定时器底层驱动，时钟使能，引脚配置
                此函数会被HAL_TIM_PWM_Init()调用
 * @param       htim:定时器句柄
 * @retval      无
 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == GTIM_TIM3_PWM)
    { // 左轮
        GPIO_InitTypeDef gpio_init_structB;
        GTIM_TIM3_PWM_CHY_GPIOB_CLK_ENABLE();                                              /* 开启通道y的GPIO时钟 */
        GTIM_TIM3_PWM_CHY_CLK_ENABLE();                                                    /* 使能定时器时钟 */
        gpio_init_structB.Pin = GTIM_TIM3_PWM_CHY_GPIO_PIN0 | GTIM_TIM3_PWM_CHY_GPIO_PIN1; /* 通道1 2的CPIO口 */
        gpio_init_structB.Mode = GPIO_MODE_AF_PP;                                          /* 复用推完输出 */
        gpio_init_structB.Pull = GPIO_PULLUP;                                              /* 上拉 */
        gpio_init_structB.Speed = GPIO_SPEED_FREQ_HIGH;                                    /* 高速 */
        gpio_init_structB.Alternate = GTIM_TIM3_PWM_CHY_GPIO_AF;                           /* IO口REMAP设置, 是否必要查看头文件配置的说明! */
        HAL_GPIO_Init(GTIM_TIM3_PWM_CHY_GPIO_PORTB, &gpio_init_structB);
    }
    if (htim->Instance == GTIM_TIM9_PWM)
    { // 右轮
        GPIO_InitTypeDef gpio_init_structE;
        GTIM_TIM9_PWM_CHY_GPIOE_CLK_ENABLE();                                              /* 开启通道y的GPIO时钟 */
        GTIM_TIM9_PWM_CHY_CLK_ENABLE();                                                    /* 使能定时器时钟 */
        gpio_init_structE.Pin = GTIM_TIM9_PWM_CHY_GPIO_PIN5 | GTIM_TIM9_PWM_CHY_GPIO_PIN6; /* 通道1 2的CPIO口 */
        gpio_init_structE.Mode = GPIO_MODE_AF_PP;                                          /* 复用推完输出 */
        gpio_init_structE.Pull = GPIO_PULLUP;                                              /* 上拉 */
        gpio_init_structE.Speed = GPIO_SPEED_FREQ_HIGH;                                    /* 高速 */
        gpio_init_structE.Alternate = GTIM_TIM9_PWM_CHY_GPIO_AF;                           /* IO口REMAP设置, 是否必要查看头文件配置的说明! */
        HAL_GPIO_Init(GTIM_TIM9_PWM_CHY_GPIO_PORTE, &gpio_init_structE);
    }

    if (htim->Instance == GTIM_TIM1_PWM)
    {
        // 推杆1
        GPIO_InitTypeDef gpio_init_structA;
        GTIM_TIM1_PWM_CHY_GPIOA_CLK_ENABLE();                                                /* 开启通道y的GPIO时钟 */
        GTIM_TIM1_PWM_CHY_CLK_ENABLE();                                                      /* 使能定时器时钟 */
        gpio_init_structA.Pin = GTIM_TIM1_PWM_CHY_GPIO_PIN10 | GTIM_TIM1_PWM_CHY_GPIO_PIN11; /* 通道1 2的CPIO口 */
        gpio_init_structA.Mode = GPIO_MODE_AF_PP;                                            /* 复用推完输出 */
        gpio_init_structA.Pull = GPIO_PULLUP;                                                /* 上拉 */
        gpio_init_structA.Speed = GPIO_SPEED_FREQ_HIGH;                                      /* 高速 */
        gpio_init_structA.Alternate = GTIM_TIM1_PWM_CHY_GPIO_AF;                             /* IO口REMAP设置, 是否必要查看头文件配置的说明! */
        HAL_GPIO_Init(GTIM_TIM1_PWM_CHY_GPIO_PORTA, &gpio_init_structA);
    }

    if (htim->Instance == GTIM_TIM8_PWM)
    {
        // 推杆2&3
        GPIO_InitTypeDef gpio_init_structC;
        GTIM_TIM8_PWM_CHY_GPIOC_CLK_ENABLE();                                                                                                          /* 开启通道y的GPIO时钟 */
        GTIM_TIM8_PWM_CHY_CLK_ENABLE();                                                                                                                /* 使能定时器时钟 */
        gpio_init_structC.Pin = GTIM_TIM8_PWM_CHY_GPIO_PIN8 | GTIM_TIM8_PWM_CHY_GPIO_PIN9 | GTIM_TIM8_PWM_CHY_GPIO_PIN6 | GTIM_TIM8_PWM_CHY_GPIO_PIN7; /* 通道1 2的CPIO口 */
        gpio_init_structC.Mode = GPIO_MODE_AF_PP;                                                                                                      /* 复用推完输出 */
        gpio_init_structC.Pull = GPIO_PULLUP;                                                                                                          /* 上拉 */
        gpio_init_structC.Speed = GPIO_SPEED_FREQ_HIGH;                                                                                                /* 高速 */
        gpio_init_structC.Alternate = GTIM_TIM8_PWM_CHY_GPIO_AF;                                                                                       /* IO口REMAP设置, 是否必要查看头文件配置的说明! */
        HAL_GPIO_Init(GTIM_TIM8_PWM_CHY_GPIO_PORTC, &gpio_init_structC);
    }

    if (htim->Instance == GTIM_TIM4_PWM)
    {
        // 推杆4&5
        GPIO_InitTypeDef gpio_init_structD;
        GTIM_TIM4_PWM_CHY_GPIOD_CLK_ENABLE();                                                                                                              /* 开启通道y的GPIO时钟 */
        GTIM_TIM4_PWM_CHY_CLK_ENABLE();                                                                                                                    /* 使能定时器时钟 */
        gpio_init_structD.Pin = GTIM_TIM4_PWM_CHY_GPIO_PIN12 | GTIM_TIM4_PWM_CHY_GPIO_PIN13 | GTIM_TIM4_PWM_CHY_GPIO_PIN14 | GTIM_TIM4_PWM_CHY_GPIO_PIN15; /* 通道1 2的CPIO口 */
        gpio_init_structD.Mode = GPIO_MODE_AF_PP;                                                                                                          /* 复用推完输出 */
        gpio_init_structD.Pull = GPIO_PULLUP;                                                                                                              /* 上拉 */
        gpio_init_structD.Speed = GPIO_SPEED_FREQ_HIGH;                                                                                                    /* 高速 */
        gpio_init_structD.Alternate = GTIM_TIM4_PWM_CHY_GPIO_AF;                                                                                           /* IO口REMAP设置, 是否必要查看头文件配置的说明! */
        HAL_GPIO_Init(GTIM_TIM4_PWM_CHY_GPIO_PORTD, &gpio_init_structD);
    }

    if (htim->Instance == GTIM_TIM12_PWM)
    {
        // 推杆6
        GPIO_InitTypeDef gpio_init_structB;
        GTIM_TIM12_PWM_CHY_GPIOB_CLK_ENABLE();                                                 /* 开启通道y的GPIO时钟 */
        GTIM_TIM12_PWM_CHY_CLK_ENABLE();                                                       /* 使能定时器时钟 */
        gpio_init_structB.Pin = GTIM_TIM12_PWM_CHY_GPIO_PIN14 | GTIM_TIM12_PWM_CHY_GPIO_PIN15; /* 通道1 2的CPIO口 */
        gpio_init_structB.Mode = GPIO_MODE_AF_PP;                                              /* 复用推完输出 */
        gpio_init_structB.Pull = GPIO_PULLUP;                                                  /* 上拉 */
        gpio_init_structB.Speed = GPIO_SPEED_FREQ_HIGH;                                        /* 高速 */
        gpio_init_structB.Alternate = GTIM_TIM12_PWM_CHY_GPIO_AF;                              /* IO口REMAP设置, 是否必要查看头文件配置的说明! */
        HAL_GPIO_Init(GTIM_TIM12_PWM_CHY_GPIO_PORTB, &gpio_init_structB);
    }
}

void MoterdriveInit(void)
{
    MoterL_pwm_chy_init(100 - 1, 42 - 1);                  //* 84 000 000 / 100*42      L 20khz频率的PWM 波形*  /
    MoterR_pwm_chy_init(200 - 1, 42 - 1);                  //* 168 000 000 / 200*42     R  20khz频率的PWM 波形*/
    MoterLift_pwm_chy_init(200 - 1, 42 - 1);               //* 168 000 000 / 200*42     T1  20khz频率的PWM 波形*/
    MoterPedestal_Backboard_pwm_chy_init(200 - 1, 42 - 1); //                   T2&T3
    MoterLeg_pwm_chy_init(100 - 1, 42 - 1);                //                   T4&T5
    MoterSupport_pwm_chy_init(100 - 1, 42 - 1);            //* 84 000 000 / 100*42      T6  20khz频率的PWM 波形*  /
}
// 推杆测试程序
void linearactuatorTest(void)
{

    // SeatLiftDrop();
//    SeatLiftDrop();
    // ThwartFB(); // 仅远程端
    // OverallFB();
    // LegSpinFB();
    // LegExtension(); // 仅远程端
    // OverallStandSit();
//    OverallFB();
    linearactuator();
}

void linearactuator(void)
{

    uint16_t T1_IN1 = 0;
    uint16_t T1_IN2 = 0;

    uint16_t T2_IN1 = 0;
    uint16_t T2_IN2 = 0;

    uint16_t T3_IN1 = 0;
    uint16_t T3_IN2 = 0;

    uint16_t T4_IN1 = 0;
    uint16_t T4_IN2 = 0;

    uint16_t T5_IN1 = 0;
    uint16_t T5_IN2 = 0;

    uint16_t T6_IN1 = 0;
    uint16_t T6_IN2 = 0;
    // 先屏蔽上位机信号
    g_slaveReg[98] = 0;
    g_slaveReg[99] = 0;
    g_slaveReg[100] = 0;
    g_slaveReg[101] = 0;
    g_slaveReg[102] = 0;
    g_slaveReg[103] = 0;
    g_slaveReg[104] = 0;

    static float acctemp = 0, acct = 0;
    static uint8_t accdoneflage = 0;
    static float Kp;

    Kp = B1MAX * SEAT_LIFTDROPRATIO / 2.0 + 0.5;

    /*座椅举升控制*/
    if (KeyStateRecive[3] == SEAT_LIFT || g_slaveReg[99] == 1)
    {
        linerun_state = Lift_run;
    }

    else if (KeyStateRecive[4] == SEAT_DROP || g_slaveReg[99] == 2)
    {

        linerun_state = Down_run;
    }

    /* 座椅靠背控制*/
    else if (KeyStateRecive[5] == BACKREST_FORWARD || g_slaveReg[100] == 1)
    {
        linerun_state = Backf_run;
    }
    
    
    else if (KeyStateRecive[6] == BACKREST_BACK || g_slaveReg[100] == 2)
        
    {
        linerun_state = Backb_run;
        
    }

    /*座板控制*/

    else if (g_slaveReg[104] == 1)
    {
        linerun_state = Seat_tiltf_run;
    }
    else if (g_slaveReg[104] == 2)
    {
        linerun_state = Seat_tiltb_run;
    }

    /*整体前后倾*/
    else if (KeyStateRecive[7] == ALL_FORWARD || g_slaveReg[101] == 1)
    {

        linerun_state = Alltiltfrun;
    }

    else if (KeyStateRecive[8] == 8 || g_slaveReg[101] == 2)
    {

        linerun_state = Alltiltbrun;
    }
    /*腿托上下旋转*/
    else if (KeyStateRecive[9] == LEG_TOPSPIN || g_slaveReg[102] == 1)
    {

        linerun_state = Legspintop_run;
    }
    else if (KeyStateRecive[10] == LEG_BACKSPIN || g_slaveReg[102] == 2)
    {
        linerun_state = Legspindown_run;
    }
    /*腿托独立调节长度*/
    else if (g_slaveReg[103] == 1)
    {
        linerun_state = Legexten_run;
    }

    else if (g_slaveReg[103] == 2)
    {
        linerun_state = Legunexten_run;
    }
    /*一键站立/坐下*/
    else if (KeyStateRecive[1] == STANCE || g_slaveReg[98] == 1)
    {

        linerun_state = Stand_run;
    }

    else if (KeyStateRecive[2] == SITTING || g_slaveReg[98] == 2)

    {
        linerun_state = Site_run;
    }

    else

    {

        linerun_state = iddle;
    }

    
    
    switch (linerun_state)
    {
    case iddle:
        T1_IN1 = 0;
        T1_IN2 = 0;
        T2_IN1 = 0;
        T2_IN2 = 0;
        T3_IN1 = 0;
        T3_IN2 = 0;
        T4_IN1 = 0;
        T4_IN2 = 0;
        T5_IN1 = 0;
        T5_IN2 = 0;
        T6_IN1 = 0;
        T6_IN2 = 0;
        acct = 0;
        accdoneflage = 0;
        acctemp = 0;
    
    
        // 靠背角度撑杆A1
        __HAL_TIM_SET_COMPARE(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH3, T3_IN1);
        __HAL_TIM_SET_COMPARE(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH4, T3_IN2);
        // 腿托角度撑杆A2
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH1, T5_IN1);
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH2, T5_IN2);
        // 腿托长度撑杆A3
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH3, T4_IN1);
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH4, T4_IN2);
        // 底盘举升撑杆B1(M)
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH1, T1_IN1);
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH2, T1_IN2);
        // 座板角度撑杆B2(L)
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH3, T2_IN1);
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH4, T2_IN2); 
        // 前支撑轮推杆C1
        __HAL_TIM_SET_COMPARE(&g_time12_pwm_chy_handle, GTIM_TIM12_PWM_CH1, T6_IN1);
        __HAL_TIM_SET_COMPARE(&g_time12_pwm_chy_handle, GTIM_TIM12_PWM_CH2, T6_IN2);
       // printf("iddle");
        break;
    case Stand_run:

        break;

    case Site_run:

        break;

    case Lift_run:
        if ((acct < 200) && (accdoneflage == 0)) // 0 --- 0.95  200ms
        {
            acct++;
            acctemp = 8.437500000000000e-12 * pow(acct, 5) - 4.218750000000000e-09 * pow(acct, 4) + 5.625000000000000e-07 * pow(acct, 3) + 0.5;
        }
        else
        {
            accdoneflage = 1;
            acct = 0;
        }
        T1_IN1 = 200 * (1.0 - 0);
        T1_IN2 = 200 * (1.0 - acctemp);
        T2_IN1 = 200 * (1.0 - 0);
        T2_IN2 = 200 * (1.0 - Kp * acctemp);
        // 底盘举升撑杆B1(M)
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH1, T1_IN1);
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH2, T1_IN2);
        // 座板角度撑杆B2(L)
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH3, T2_IN1);
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH4, T2_IN2);
        break;

    case Down_run:
        if ((acct < 200) && (accdoneflage == 0))
        {
            acct++;
            acctemp = 8.437500000000000e-12 * pow(acct, 5) - 4.218750000000000e-09 * pow(acct, 4) + 5.625000000000000e-07 * pow(acct, 3) + 0.5;
        }
        else
        {
            accdoneflage = 1;
            acct = 0;
        }
        T1_IN1 = 200 * (1.0 - acctemp);
        T1_IN2 = 200 * (1.0 - 0);
        T2_IN1 = 200 * (1.0 - Kp * acctemp);
        T2_IN2 = 200 * (1.0 - 0);
        // 底盘举升撑杆B1(M)
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH1, T1_IN1);
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH2, T1_IN2);
        // 座板角度撑杆B2(L)
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH3, T2_IN1);
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH4, T2_IN2);
        break;

    case Backf_run:
        if ((acct < 200) && (accdoneflage == 0))
        {
            acct++;
            acctemp = 8.437500000000000e-12 * pow(acct, 5) - 4.218750000000000e-09 * pow(acct, 4) + 5.625000000000000e-07 * pow(acct, 3) + 0.5;
        }
        else
        {
            accdoneflage = 1;
            acct = 0;
        }
        T3_IN1 = 200 * (1.0 - 0);
        T3_IN2 = 200 * (1.0 - acctemp);
        __HAL_TIM_SET_COMPARE(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH3, T3_IN1);
        __HAL_TIM_SET_COMPARE(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH4, T3_IN2);
        break;

    case Backb_run:
        if ((acct < 200) && (accdoneflage == 0))
        {
            acct++;
            acctemp = 8.437500000000000e-12 * pow(acct, 5) - 4.218750000000000e-09 * pow(acct, 4) + 5.625000000000000e-07 * pow(acct, 3) + 0.5;
        }
        else
        {
            accdoneflage = 1;
            acct = 0;
        }
        T3_IN1 = 200 * (1.0 - acctemp);
        T3_IN2 = 200 * (1.0 - 0);
        __HAL_TIM_SET_COMPARE(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH3, T3_IN1);
        __HAL_TIM_SET_COMPARE(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH4, T3_IN2);
        break;

    case Alltiltfrun:
        if ((acct < 200) && (accdoneflage == 0))
        {
            acct++;
            acctemp = 8.437500000000000e-12 * pow(acct, 5) - 4.218750000000000e-09 * pow(acct, 4) + 5.625000000000000e-07 * pow(acct, 3) + 0.5;
        }
        else
        {
            accdoneflage = 1;
            acct = 0;
        }
        T1_IN1 = 200 * (1.0 - 0);
        T1_IN2 = 200 * (1.0 - acctemp);
        // 底盘举升撑杆B1(M)
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH1, T1_IN1);
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH2, T1_IN2);
        break;

    case Alltiltbrun:
        if ((acct < 200) && (accdoneflage == 0))
        {
            acct++;
            acctemp = 8.437500000000000e-12 * pow(acct, 5) - 4.218750000000000e-09 * pow(acct, 4) + 5.625000000000000e-07 * pow(acct, 3) + 0.5;
        }
        else
        {
            accdoneflage = 1;
            acct = 0;
        }
        T1_IN1 = 200 * (1.0 - acctemp);
        T1_IN2 = 200 * (1.0 - 0);
        // 底盘举升撑杆B1(M)
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH1, T1_IN1);
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH2, T1_IN2);
        //printf("houqing_run\n");
        break;

    case Legspintop_run:
        // 腿托上旋
        if ((acct < 200) && (accdoneflage == 0))
        {
            acct++;
            acctemp = 8.437500000000000e-12 * pow(acct, 5) - 4.218750000000000e-09 * pow(acct, 4) + 5.625000000000000e-07 * pow(acct, 3) + 0.5;
        }
        else
        {
            accdoneflage = 1;
            acct = 0;
        }

        T4_IN1 = 100 * (1.0 - 0);
        T4_IN2 = 100 * (1.0 - acctemp);
        T5_IN1 = 100 * (1.0 - 0);
        T5_IN2 = 100 * (1.0 - acctemp);
        // 腿托长度撑杆A3
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH3, T4_IN1);
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH4, T4_IN2);
        // 腿托角度撑杆A2
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH1, T5_IN1);
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH2, T5_IN2);
        break;
    case Legspindown_run:
        // 腿托下旋
        if ((acct < 200) && (accdoneflage == 0))
        {
            acct++;
            acctemp = 8.437500000000000e-12 * pow(acct, 5) - 4.218750000000000e-09 * pow(acct, 4) + 5.625000000000000e-07 * pow(acct, 3) + 0.5;
        }
        else
        {
            accdoneflage = 1;
            acct = 0;
        }
        T4_IN1 = 100 * (1.0 - acctemp);
        T4_IN2 = 100 * (1.0 - 0);
        T5_IN1 = 100 * (1.0 - acctemp);
        T5_IN2 = 100 * (1.0 - 0);
        // 腿托长度撑杆A3
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH3, T4_IN1);
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH4, T4_IN2);
        // 腿托角度撑杆A2
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH1, T5_IN1);
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH2, T5_IN2);
        break;
    case Legexten_run:
        // 腿托独立伸长调节
        if (g_slaveReg[103] == 1)
        {
            if ((acct < 200) && (accdoneflage == 0))
            {
                acct++;
                acctemp = 8.437500000000000e-12 * pow(acct, 5) - 4.218750000000000e-09 * pow(acct, 4) + 5.625000000000000e-07 * pow(acct, 3) + 0.5;
            }
            else
            {
                accdoneflage = 1;
                acct = 0;
            }
            T4_IN1 = 100 * (1.0 - acctemp);
            T4_IN2 = 100 * (1.0 - 0);
        }
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH3, T4_IN1);
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH4, T4_IN2);
        break;

    case Legunexten_run:
        if ((acct < 200) && (accdoneflage == 0))
        {
            acct++;
            acctemp = 8.437500000000000e-12 * pow(acct, 5) - 4.218750000000000e-09 * pow(acct, 4) + 5.625000000000000e-07 * pow(acct, 3) + 0.5;
        }
        else
        {
            accdoneflage = 1;
            acct = 0;
        }
        T3_IN1 = 100 * (1.0 - 0);
        T3_IN2 = 100 * (1.0 - acctemp);
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH3, T4_IN1);
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH4, T4_IN2);

        break;

    case Seat_tiltf_run:
        if ((acct < 200) && (accdoneflage == 0))
        {
            acct++;
            acctemp = 8.437500000000000e-12 * pow(acct, 5) - 4.218750000000000e-09 * pow(acct, 4) + 5.625000000000000e-07 * pow(acct, 3) + 0.5;
        }
        else
        {
            accdoneflage = 1;
            acct = 0;
        }
        T2_IN1 = 200 * (1.0 - 0);
        T2_IN2 = 200 * (1.0 - acctemp);
        // 座板角度撑杆B2(L)
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH3, T2_IN1);
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH4, T2_IN2);
        break;
    case Seat_tiltb_run:
        if ((acct < 200) && (accdoneflage == 0))
        {
            acct++;
            acctemp = 8.437500000000000e-12 * pow(acct, 5) - 4.218750000000000e-09 * pow(acct, 4) + 5.625000000000000e-07 * pow(acct, 3) + 0.5;
        }
        else
        {
            accdoneflage = 1;
            acct = 0;
        }
        T2_IN1 = 200 * (1.0 - acctemp);
        T2_IN2 = 200 * (1.0 - 0);
        // 座板角度撑杆B2(L)
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH3, T2_IN1);
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH4, T2_IN2);

        break;

    default:
        break;
    
    }
}

 /*座椅举升控制*/
 void SeatLiftDrop(void)
 {
     // 底盘举升撑杆B1(M)
     uint16_t T1_IN1 = 0;
     uint16_t T1_IN2 = 0;
     // 座板角度撑杆B2(L)
     uint16_t T2_IN1 = 0;
     uint16_t T2_IN2 = 0;
     static float acctemp = 0, acct = 0;
     static uint8_t accdoneflage = 0;
     static float Kp;

     Kp = B1MAX * SEAT_LIFTDROPRATIO / 2.0 + 0.5;
     if (KeyStateRecive[3] == SEAT_LIFT || g_slaveReg[99] == 1)
     {
         // 举升软起动 相关代码
         if ((acct < 200) && (accdoneflage == 0)) // 0 --- 0.95  200ms
         {
             acct++;
             acctemp = 8.437500000000000e-12 * pow(acct, 5) - 4.218750000000000e-09 * pow(acct, 4) + 5.625000000000000e-07 * pow(acct, 3) + 0.5;
         }
         else
         {
             accdoneflage = 1;
             acct = 0;
         }

         T1_IN1 = 200 * (1.0 - 0);
         T1_IN2 = 200 * (1.0 - acctemp);
         T2_IN1 = 200 * (1.0 - 0);
         T2_IN2 = 200 * (1.0 - Kp * acctemp);
     }
     else if (KeyStateRecive[4] == SEAT_DROP || g_slaveReg[99] == 2)
     {
         // 下降软起动 相关代码

         if ((acct < 200) && (accdoneflage == 0))
         {
             acct++;
             acctemp = 8.437500000000000e-12 * pow(acct, 5) - 4.218750000000000e-09 * pow(acct, 4) + 5.625000000000000e-07 * pow(acct, 3) + 0.5;
         }
         else
         {
             accdoneflage = 1;
             acct = 0;
         }
         T1_IN1 = 200 * (1.0 - acctemp);
         T1_IN2 = 200 * (1.0 - 0);
         T2_IN1 = 200 * (1.0 - Kp * acctemp);
         T2_IN2 = 200 * (1.0 - 0);
     }
     else
     {
         acct = 0;
         accdoneflage = 0;
         acctemp = 0;
         T1_IN1 = 200 * (1.0 - 0);
         T1_IN2 = 200 * (1.0 - 0);
         T2_IN1 = 200 * (1.0 - 0);
         T2_IN2 = 200 * (1.0 - 0);
     }
     // 底盘举升撑杆B1(M)
     __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH1, T1_IN1);
     __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH2, T1_IN2);
     // 座板角度撑杆B2(L)
     __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH3, T2_IN1);
     __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH4, T2_IN2);
 }

 /*座椅靠背控制-靠背角度撑杆A1*/
 void BackresetFB(void)
 {
     uint16_t T3_IN1 = 0;
     uint16_t T3_IN2 = 0;
     static float acctemp = 0, acct = 0;
     static uint8_t accdoneflage = 0;
     // 靠背前倾
     if (KeyStateRecive[5] == BACKREST_FORWARD || g_slaveReg[100] == 1)
     {
         if ((acct < 200) && (accdoneflage == 0))
         {
             acct++;
             acctemp = 8.437500000000000e-12 * pow(acct, 5) - 4.218750000000000e-09 * pow(acct, 4) + 5.625000000000000e-07 * pow(acct, 3) + 0.5;
         }
         else
         {
             accdoneflage = 1;
             acct = 0;
         }
         T3_IN1 = 200 * (1.0 - 0);
         T3_IN2 = 200 * (1.0 - acctemp);
     }
     // 靠背后倾
     else if (KeyStateRecive[6] == BACKREST_BACK || g_slaveReg[100] == 2)
     {
         if ((acct < 200) && (accdoneflage == 0))
         {
             acct++;
             acctemp = 8.437500000000000e-12 * pow(acct, 5) - 4.218750000000000e-09 * pow(acct, 4) + 5.625000000000000e-07 * pow(acct, 3) + 0.5;
         }
         else
         {
             accdoneflage = 1;
             acct = 0;
         }
         T3_IN1 = 200 * (1.0 - acctemp);
         T3_IN2 = 200 * (1.0 - 0);
     }
     // 靠背前后倾STOP
     else
     {
         acct = 0;
         accdoneflage = 0;
         acctemp = 0;
         T3_IN2 = 0;
         T3_IN1 = 0;
     }
     __HAL_TIM_SET_COMPARE(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH3, T3_IN1);
     __HAL_TIM_SET_COMPARE(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH4, T3_IN2);
 }

 /*座板控制-座板角度撑杆B2*/
 void ThwartFB(void)
 {
     uint16_t T2_IN1 = 0;
     uint16_t T2_IN2 = 0;
     static float acctemp = 0, acct = 0;
     static uint8_t accdoneflage = 0;

     // 座板前倾
     if (g_slaveReg[104] == 1)
     {
         if ((acct < 200) && (accdoneflage == 0))
         {
             acct++;
             acctemp = 8.437500000000000e-12 * pow(acct, 5) - 4.218750000000000e-09 * pow(acct, 4) + 5.625000000000000e-07 * pow(acct, 3) + 0.5;
         }
         else
         {
             accdoneflage = 1;
             acct = 0;
         }
         T2_IN1 = 200 * (1.0 - 0);
         T2_IN2 = 200 * (1.0 - acctemp);
     }
     // 座板后倾
     else if (g_slaveReg[104] == 2)
     {
         if ((acct < 200) && (accdoneflage == 0))
         {
             acct++;
             acctemp = 8.437500000000000e-12 * pow(acct, 5) - 4.218750000000000e-09 * pow(acct, 4) + 5.625000000000000e-07 * pow(acct, 3) + 0.5;
         }
         else
         {
             accdoneflage = 1;
             acct = 0;
         }
         T2_IN1 = 200 * (1.0 - acctemp);
         T2_IN2 = 200 * (1.0 - 0);
     }
     // 座板前后倾STOP
     else
     {
         acct = 0;
         accdoneflage = 0;
         acctemp = 0;
         T2_IN1 = 0;
         T2_IN2 = 0;
     }
     __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH3, T2_IN1);
     __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH4, T2_IN2);
 }
 /*整体前后倾-底盘举升撑杆B1*/
 void OverallFB(void)
 {
     uint16_t T1_IN1 = 0;
     uint16_t T1_IN2 = 0;
     static float acctemp = 0, acct = 0;
     static uint8_t accdoneflage = 0;
     // 整体前倾
     if (KeyStateRecive[7] == ALL_FORWARD || g_slaveReg[101] == 1)
     {
         if ((acct < 200) && (accdoneflage == 0))
         {
             acct++;
             acctemp = 8.437500000000000e-12 * pow(acct, 5) - 4.218750000000000e-09 * pow(acct, 4) + 5.625000000000000e-07 * pow(acct, 3) + 0.5;
         }
         else
         {
             accdoneflage = 1;
             acct = 0;
         }
         T1_IN1 = 200 * (1.0 - 0);
         T1_IN2 = 200 * (1.0 - acctemp);
     }

     // 整体后倾
     else if (KeyStateRecive[8] == ALL_BACK || g_slaveReg[101] == 2)
     {

         if ((acct < 200) && (accdoneflage == 0))
         {
             acct++;
             acctemp = 8.437500000000000e-12 * pow(acct, 5) - 4.218750000000000e-09 * pow(acct, 4) + 5.625000000000000e-07 * pow(acct, 3) + 0.5;
         }
         else
         {
             accdoneflage = 1;
             acct = 0;
         }
         T1_IN1 = 200 * (1.0 - acctemp);
         T1_IN2 = 200 * (1.0 - 0);
     }
     else
     {
         acct = 0;
         accdoneflage = 0;
         acctemp = 0;
         T1_IN1 = 0;
         T1_IN2 = 0;
     }
     __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH1, T1_IN1);
     __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH2, T1_IN2);
 }

 /*腿托上下旋转调节 腿托角度撑杆A2 腿托长度撑杆A3*/
 void LegSpinFB(void)
 {
     // 腿托长度撑杆A3
     uint16_t T4_IN1 = 0;
     uint16_t T4_IN2 = 0;
     // 腿托角度撑杆A2
     uint16_t T5_IN1 = 0;
     uint16_t T5_IN2 = 0;
     static float acctemp = 0, acct = 0;
     static uint8_t accdoneflage = 0;

     if (KeyStateRecive[9] == LEG_TOPSPIN || g_slaveReg[102] == 1)
     {
         // 腿托上旋
         if ((acct < 200) && (accdoneflage == 0))
         {
             acct++;
             acctemp = 8.437500000000000e-12 * pow(acct, 5) - 4.218750000000000e-09 * pow(acct, 4) + 5.625000000000000e-07 * pow(acct, 3) + 0.5;
         }
         else
         {
             accdoneflage = 1;
             acct = 0;
         }

         T4_IN1 = 100 * (1.0 - 0);
         T4_IN2 = 100 * (1.0 - acctemp);
         T5_IN1 = 100 * (1.0 - 0);
         T5_IN2 = 100 * (1.0 - acctemp);
     }
     else if (KeyStateRecive[10] == LEG_BACKSPIN || g_slaveReg[102] == 2)
     {
         // 腿托下旋

         if ((acct < 200) && (accdoneflage == 0))
         {
             acct++;
             acctemp = 8.437500000000000e-12 * pow(acct, 5) - 4.218750000000000e-09 * pow(acct, 4) + 5.625000000000000e-07 * pow(acct, 3) + 0.5;
         }
         else
         {
             accdoneflage = 1;
             acct = 0;
         }
         T4_IN1 = 100 * (1.0 - acctemp);
         T4_IN2 = 100 * (1.0 - 0);
         T5_IN1 = 100 * (1.0 - acctemp);
         T5_IN2 = 100 * (1.0 - 0);
     }
     else
     {
         acct = 0;
         accdoneflage = 0;
         acctemp = 0;
         T4_IN1 = 100 * (1.0 - 0);
         T4_IN2 = 100 * (1.0 - 0);
         T5_IN1 = 100 * (1.0 - 0);
         T5_IN2 = 100 * (1.0 - 0);
     }
     // 腿托长度撑杆A3
     __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH3, T4_IN1);
     __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH4, T4_IN2);
     // 腿托角度撑杆A2
     __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH1, T5_IN1);
     __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH2, T5_IN2);
 }

 void LegExtension(void)
 {

     uint16_t T3_IN1 = 0;
     uint16_t T3_IN2 = 0;
     static float acctemp = 0, acct = 0;
     static uint8_t accdoneflage = 0;
     // 腿托独立伸长调节
     if (g_slaveReg[103] == 1)
     {
         if ((acct < 200) && (accdoneflage == 0))
         {
             acct++;
             acctemp = 8.437500000000000e-12 * pow(acct, 5) - 4.218750000000000e-09 * pow(acct, 4) + 5.625000000000000e-07 * pow(acct, 3) + 0.5;
         }
         else
         {
             accdoneflage = 1;
             acct = 0;
         }
         T3_IN2 = 200 * (1.0 - 0);
         T3_IN1 = 200 * (1.0 - acctemp);
     }
     // 腿托缩短伸长调节
     else if (g_slaveReg[103] == 2) // || Reg[100] ==2 )
     {
         if ((acct < 200) && (accdoneflage == 0))
         {
             acct++;
             acctemp = 8.437500000000000e-12 * pow(acct, 5) - 4.218750000000000e-09 * pow(acct, 4) + 5.625000000000000e-07 * pow(acct, 3) + 0.5;
         }
         else
         {
             accdoneflage = 1;
             acct = 0;
         }
         T3_IN2 = 200 * (1.0 - acctemp);
         T3_IN1 = 200 * (1.0 - 0);
     }
     // 腿托独立伸长调节
     else
     {
         acct = 0;
         accdoneflage = 0;
         acctemp = 0;
         T3_IN2 = 0;
         T3_IN1 = 0;
     }
     __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH3, T3_IN1);
     __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH4, T3_IN2);
 }

 void OverallStandSit()
 {
     static float acctemp = 0, acct = 0;
     static uint8_t accdoneflage = 0;
     NONE;
 }