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
ACTORLIMITPARA ActorLimitPara;
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
    ActorLimitPara.A1_Downpos =1400;
    ActorLimitPara.A1_Uppos = 4100; // 前倾已经超出限位计，但是机械上暂时不约束;

    ActorLimitPara.A2_Downpos = 2200;
    ActorLimitPara.A2_Uppos = 3000; // 上旋已经超出限位计长度，机械上暂时不约束

    ActorLimitPara.A3_Downpos = 0; // 下限位无约束
    ActorLimitPara.A3_Uppos = 3000;

    ActorLimitPara.B1_Downpos = 0;
    ActorLimitPara.B1_Uppos = 2500;

    ActorLimitPara.B2_Downpos = 0;
    ActorLimitPara.B2_Uppos = 2400;

    ActorLimitPara.C1_Uppos = 10000;
    ActorLimitPara.C2_Downpos = 0;

    static float acctemp = 0, acct = 0;
    static uint8_t accdoneflage = 0;
    static float Kp;

    Kp = B1MAX * SEAT_LIFTDROPRATIO / 2.0 + 0.5;

    /*座椅举升控制*/
    if (CanKeybufReceive[1] == SEAT_LIFT || g_slaveReg[67] == 1)
    {
        linerun_state = Lift_run;
        g_slaveReg[16] = 1;
    }

    else if (CanKeybufReceive[1] == SEAT_DROP || g_slaveReg[67] == 2)
    {

        linerun_state = Down_run;
        g_slaveReg[16] = 2;
    }

    /* 座椅靠背控制*/
    else if (CanKeybufReceive[1] == BACKREST_FORWARD || g_slaveReg[68] == 1)
    {
        linerun_state = Backf_run;
        g_slaveReg[17] = 1;
    }
    
    
    else if (CanKeybufReceive[1] == BACKREST_BACK || g_slaveReg[68] == 2)
        
    {
        linerun_state = Backb_run;
        g_slaveReg[17] = 2;
        
    }

    /*座板控制*/

    else if (g_slaveReg[72] == 1)
    {
        linerun_state = Seat_tiltf_run;
    }
    else if (g_slaveReg[72] == 2)
    {
        linerun_state = Seat_tiltb_run;
    }

    /*整体前后倾*/
    else if (CanKeybufReceive[1] == ALL_FORWARD || g_slaveReg[69] == 1)
    {

        linerun_state = Alltiltfrun;
        g_slaveReg[19] = 1;
    }

    else if (CanKeybufReceive[1] == 8 || g_slaveReg[69] == 2)
    {

        linerun_state = Alltiltbrun;
        g_slaveReg[19] = 2;
    }
    /*腿托上下旋转*/
    else if (CanKeybufReceive[1] == LEG_TOPSPIN || g_slaveReg[70] == 1)
    {

        linerun_state = Legspintop_run;
        g_slaveReg[20] = 1;
    }
    else if (CanKeybufReceive[1] == LEG_BACKSPIN || g_slaveReg[70] == 2)
    {
        linerun_state = Legspindown_run;
        g_slaveReg[20] = 2;
    }
    /*腿托独立调节长度*/
    else if (g_slaveReg[71] == 1)
    {
        linerun_state = Legexten_run;
    }

    else if (g_slaveReg[71] == 2)
    {
        linerun_state = Legunexten_run;
    }
    /*一键站立/坐下*/
    else if (CanKeybufReceive[1] == STANCE || g_slaveReg[66] == 1)
    {

        linerun_state = Stand_run;
        g_slaveReg[18] = 1;
    }

    else if (CanKeybufReceive[1] == SITTING || g_slaveReg[66] == 2)

    {
        linerun_state = Site_run;
        g_slaveReg[18] = 2;
    }

    else

    {

        linerun_state = iddle;
        g_slaveReg[16] = 0;
        g_slaveReg[17] = 0;
        g_slaveReg[18] = 0;
        g_slaveReg[19] = 0;
        g_slaveReg[20] = 0;

    }

    if (CanKeybufReceive[1]!= 0 ) /*按键板有按键按下*/
    {
        g_slaveReg[21] = 1 ;
    }
    else
    {
        g_slaveReg[21] = 0 ;
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
        // 举升状态下的 限位保护
        if ( adcdata.lift_pos  > ActorLimitPara.B1_Uppos)
        {
            T1_IN1 = 200;
            T1_IN2 = 200;
            //垂直举升到极限状态下，底座推杆应该停止
            T2_IN1 = 200;
            T2_IN2 = 200;
        }

        if (adcdata.pedestal_pos > ActorLimitPara.B2_Uppos)
        {
            T2_IN1 = 200;
            T2_IN2 = 200;
        }
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
        /*前倾推杆位置约束*/
        if (adcdata.backboard_pos > ActorLimitPara.A1_Uppos)
        {
                T3_IN1 = 200;
                T3_IN2 = 200;
        }
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
        /*后倾推杆位置约束*/
        if (adcdata.backboard_pos < ActorLimitPara.A1_Downpos)
        {
                T3_IN1 = 200;
                T3_IN2 = 200;
        }
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
        T1_IN1 = 200 * (1.0 - acctemp);
        T1_IN2 = 200 * (1.0 - 0);
        /*整体前/后倾约束*/
        // if ( adcdata.lift_pos  > ActorLimitPara.B1_Uppos)
        // {
        //     T1_IN1 = 200;
        //     T1_IN2 = 200;
        // }
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
        T1_IN1 = 200 * (1.0 - 0);
        T1_IN2 = 200 * (1.0 - acctemp);
        /*整体后倾约束*/
        if ( adcdata.lift_pos  > ActorLimitPara.B1_Uppos)
        {
            T1_IN1 = 200;
            T1_IN2 = 200;
        }    
        // 底盘举升撑杆B1(M)
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH1, T1_IN1);
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH2, T1_IN2);
       printf("houqing_run\n");
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
        /*上旋腿托伸长约束*/
        if (adcdata.leglength_pos > ActorLimitPara.A3_Uppos)
        {
            T4_IN1 = 100;
            T4_IN2 = 100;
        }
        /*腿托旋转无约束*/
        // if (adcdata.legangle_pos > ActorLimitPara.A2_Uppos)
        // {
        //     T4_IN1 = 0;

        //     T4_IN2 = 0;
        // }
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
        /*腿托下旋腿托长度无约束*/
        // if (adcdata.leglength_pos > ActorLimitPara.A3_Uppos)
        // {
        //     T4_IN1 = 0;
        //     T4_IN2 = 0;
        // }
        /*腿托角度旋转约束*/
        if (adcdata.legangle_pos < ActorLimitPara.A2_Downpos)
        {
            T5_IN1 = 100;

            T5_IN2 = 100;
        }

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
            /*腿托长度约束*/
            if (adcdata.leglength_pos > ActorLimitPara.A3_Uppos)
            {
                T4_IN1 = 100;
                T4_IN2 = 100;
            }
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
        /*座板前倾约束*/
        if (adcdata.pedestal_pos > ActorLimitPara.B2_Uppos)
        {
            T2_IN1 = 200;
            T2_IN2 = 200;
        }
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