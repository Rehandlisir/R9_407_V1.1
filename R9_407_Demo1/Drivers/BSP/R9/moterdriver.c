/**
 * @FilePath     : /MDK-ARMc:/Users/fu/Desktop/Code/CodeV1.1/R9_407_V1.1/R9_407_Demo1/Drivers/BSP/R9/moterdriver.c
 * @Description  :  
 * @Author       : lisir lisir@rehand.com
 * @Version      : 0.0.1
 * @LastEditors  : lisir lisir@rehand.com
 * @LastEditTime : 2024-06-24 10:48:24
 * @2024 by Rehand Medical Technology Co., LTD, All Rights Reserved.
**/
/**
 ****************************************************************************************************
 * @file        moterdriver.c
 * @author      Lisir
 * @version     V1.0
 * @date        2021-10-14
 * @brief       moterdriver ��������
 * @license     Copyright (c) 2024, ���ڸ���ҽ�ƿƼ����޹�˾
 */
#include "./BSP/R9/moterdriver.h"
TIM_HandleTypeDef g_time1_pwm_chy_handle;  /* �����Ƹ� */
TIM_HandleTypeDef g_time8_pwm_chy_handle;  /* �����Ƹ�*/
TIM_HandleTypeDef g_time3_pwm_chy_handle;  /*�����Ƹ� */
TIM_HandleTypeDef g_time4_pwm_chy_handle;  /*���г��ȼ��Ƕ�*/
TIM_HandleTypeDef g_time12_pwm_chy_handle; /* ǰ֧����*/
LineActor_State linerun_state;
ACTORLIMITPARA ActorLimitPara;

/**********************************����L ���1 ������� TIME1 CH1 CH2 CH1N CH2N  *************************************/
TIM_HandleTypeDef g_tim1_cplm_pwm_handle;                              /* ��ʱ��1��� */
TIM_BreakDeadTimeConfigTypeDef g_sbreak1_dead_time1_config = {0};        /* ����ʱ������ */

/**
 *  @brief       �߼���ʱ��TIM1 ������� ��ʼ��������ʹ��PWMģʽ1��
 * @note
 *              ���ø߼���ʱ��TIMX �������, һ·OCy һ·OCyN, ���ҿ�����������ʱ��
 *              �߼���ʱ����ʱ������APB2, ��PCLK2 = 168Mhz, ��������PPRE2����Ƶ, ���
 *              �߼���ʱ��ʱ�� = 168Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��, ��λ : Mhz
 * @param         {uint16_t} arr:�Զ���װֵ��
 * @param         {uint16_t} psc:Ԥ��Ƶϵ��
 * @param         {uint8_t} dtg: 
 * 
**/
void MoterL_pwm_chy_init(uint16_t arr, uint16_t psc) // ���ֵ�� atim_timx_cplm_pwm_init(100 - 1, 84 - 1); /* 20Khz����ݔ��. */
{
    /*���ֵ��*/ 
    GPIO_InitTypeDef gpio_init_struct = {0};
    TIM_OC_InitTypeDef tim_oc_cplm_pwm = {0};

    ATIM_TIM1_CPLM_CLK_ENABLE();            /* TIMx ʱ��ʹ�� */
    ATIM_TIM1_CPLM_CH1_GPIO_CLK_ENABLE();   /* ͨ��X��ӦIO��ʱ��ʹ�� */
    ATIM_TIM1_CPLM_CH1N_GPIO_CLK_ENABLE();  /* ͨ��X����ͨ����ӦIO��ʱ��ʹ�� */
    ATIM_TIM1_CPLM_CH2_GPIO_CLK_ENABLE();   /* ͨ��X��ӦIO��ʱ��ʹ�� */
    ATIM_TIM1_CPLM_CH2N_GPIO_CLK_ENABLE();  /* ͨ��X����ͨ����ӦIO��ʱ��ʹ�� */
    
    ATIM_TIM1_CPLM_BKIN_GPIO_CLK_ENABLE();  /* ͨ��Xɲ�������ӦIO��ʱ��ʹ�� */
  
    gpio_init_struct.Pin = ATIM_TIM1_CPLM_BKIN_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;
    gpio_init_struct.Pull = GPIO_PULLDOWN;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_init_struct.Alternate = ATIM_TIM1_CPLM_CHY_GPIO_AF;                /* �˿ڸ��� */
    HAL_GPIO_Init(ATIM_TIM1_CPLM_BKIN_GPIO_PORT, &gpio_init_struct);

    gpio_init_struct.Pin = ATIM_TIM1_CPLM_CH1_GPIO_PIN;
    HAL_GPIO_Init(ATIM_TIM1_CPLM_CH1_GPIO_PORT, &gpio_init_struct);

    gpio_init_struct.Pin = ATIM_TIM1_CPLM_CH1N_GPIO_PIN;
    HAL_GPIO_Init(ATIM_TIM1_CPLM_CH1N_GPIO_PORT, &gpio_init_struct);
    
    gpio_init_struct.Pin = ATIM_TIM1_CPLM_CH2_GPIO_PIN;
    HAL_GPIO_Init(ATIM_TIM1_CPLM_CH2_GPIO_PORT, &gpio_init_struct);

    gpio_init_struct.Pin = ATIM_TIM1_CPLM_CH2N_GPIO_PIN;
    HAL_GPIO_Init(ATIM_TIM1_CPLM_CH2N_GPIO_PORT, &gpio_init_struct);

    g_tim1_cplm_pwm_handle.Instance = ATIM_TIM1_CPLM;                       /* ��ʱ��x */
    g_tim1_cplm_pwm_handle.Init.Prescaler = psc;                            /* Ԥ��Ƶϵ�� */
    g_tim1_cplm_pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;           /* ���ϼ���ģʽ */
    g_tim1_cplm_pwm_handle.Init.Period = arr;                               /* �Զ���װ��ֵ */
    g_tim1_cplm_pwm_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;     /* CKD[1:0] = 10, tDTS = 4 * tCK_INT = Ft / 4 = 42Mhz*/
    g_tim1_cplm_pwm_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;  /* ʹ��Ӱ�ӼĴ���TIMx_ARR */
  
    HAL_TIM_PWM_Init(&g_tim1_cplm_pwm_handle) ;

    tim_oc_cplm_pwm.OCMode = TIM_OCMODE_PWM1;                               /* PWMģʽ1 */
    tim_oc_cplm_pwm.OCPolarity = TIM_OCPOLARITY_HIGH;                       /* OCy �ߵ�ƽ��Ч */
    tim_oc_cplm_pwm.OCNPolarity = TIM_OCPOLARITY_HIGH;                      /* OCyN �ߵ�ƽ��Ч */
    tim_oc_cplm_pwm.OCIdleState = TIM_OCIDLESTATE_RESET;                      /* ��MOE=0��OCx=0 */
    tim_oc_cplm_pwm.OCNIdleState = TIM_OCNIDLESTATE_RESET;                    /* ��MOE=0��OCxN=0 */
    HAL_TIM_PWM_ConfigChannel(&g_tim1_cplm_pwm_handle, &tim_oc_cplm_pwm, ATIM_TIM1_CPLM_CH1);
    HAL_TIM_PWM_ConfigChannel(&g_tim1_cplm_pwm_handle, &tim_oc_cplm_pwm, ATIM_TIM1_CPLM_CH2);
    
    /* �����������������������ж� */
    g_sbreak1_dead_time1_config.OffStateRunMode = TIM_OSSR_DISABLE;           /* ����ģʽ�Ĺر����״̬ */
    g_sbreak1_dead_time1_config.OffStateIDLEMode = TIM_OSSI_DISABLE;          /* ����ģʽ�Ĺر����״̬ */
    g_sbreak1_dead_time1_config.LockLevel = TIM_LOCKLEVEL_OFF;                /* ���üĴ��������� */
    g_sbreak1_dead_time1_config.BreakState = TIM_BREAK_ENABLE;                /* ʹ��ɲ������ */
    g_sbreak1_dead_time1_config.BreakPolarity = TIM_BREAKPOLARITY_HIGH;       /* ɲ��������Ч�źż���Ϊ�� */
    g_sbreak1_dead_time1_config.AutomaticOutput = TIM_AUTOMATICOUTPUT_ENABLE; /* ʹ��AOEλ������ɲ���������Զ��ָ���� */
    HAL_TIMEx_ConfigBreakDeadTime(&g_tim1_cplm_pwm_handle, &g_sbreak1_dead_time1_config);

    HAL_TIM_PWM_Start(&g_tim1_cplm_pwm_handle, ATIM_TIM1_CPLM_CH1);         /* OC1 ���ʹ�� */
    HAL_TIMEx_PWMN_Start(&g_tim1_cplm_pwm_handle, ATIM_TIM1_CPLM_CH1);      /* OC1N ���ʹ�� */
    
    HAL_TIM_PWM_Start(&g_tim1_cplm_pwm_handle, ATIM_TIM1_CPLM_CH2);         /* OC2 ���ʹ�� */
    HAL_TIMEx_PWMN_Start(&g_tim1_cplm_pwm_handle, ATIM_TIM1_CPLM_CH2);      /* OC2N ���ʹ�� */                       /* ������ӦPWMͨ�� */
    LEFT_SOFTBRAKE(0); //��ɲ��ֹͣ
}

/**
 * @brief       ��ʱ��TIMX ��������Ƚ�ֵ & ����ʱ��
 * @param       ccr: ����Ƚ�ֵ
 * @param       dtg: ����ʱ��
 *   @arg       dtg[7:5]=0xxʱ, ����ʱ�� = dtg[7:0] * tDTS
 *   @arg       dtg[7:5]=10xʱ, ����ʱ�� = (64 + dtg[6:0]) * 2  * tDTS
 *   @arg       dtg[7:5]=110ʱ, ����ʱ�� = (32 + dtg[5:0]) * 8  * tDTS
 *   @arg       dtg[7:5]=111ʱ, ����ʱ�� = (32 + dtg[5:0]) * 16 * tDTS
 *   @note      tDTS = 1 / (Ft /  CKD[1:0]) = 1 / 42M = 23.8ns
 * @retval      ��
 * @return        {*}
**/
void atim_tim1_cplm_pwm_set(uint16_t ccr1,uint16_t ccr2)
{
    g_sbreak1_dead_time1_config.DeadTime = 50;
    HAL_TIMEx_ConfigBreakDeadTime(&g_tim1_cplm_pwm_handle, &g_sbreak1_dead_time1_config);      /*��������ʱ��*/
    __HAL_TIM_MOE_ENABLE(&g_tim1_cplm_pwm_handle);      /* MOE=1,ʹ������� */    
    ATIM_TIM1_CPLM_CH1_CCR1 = ccr1;                      /* ���ñȽϼĴ��� */
    ATIM_TIM1_CPLM_CH2_CCR2 = ccr2;  
}
void LeftMoterMove(uint8_t dir,double duty_cycle)
{
    LEFT_SOFTBRAKE(0); // �ر���ɲ��
    if (dir==1)
    {
        atim_tim1_cplm_pwm_set((uint16_t)duty_cycle*100,0);
    }
    else if(dir==0)
    {
        atim_tim1_cplm_pwm_set(0,(uint16_t)duty_cycle*100);
    }
    else
    {
        LeftMoterStop();   
    }
}

void LeftMoterStop(void)
{
    LEFT_SOFTBRAKE(1);
}
/**********************************����R ���2������� TIME8 CH1 CH2 CH1N CH2N*************************************/
TIM_HandleTypeDef g_tim8_cplm_pwm_handle;                              /* ��ʱ��1��� */
TIM_BreakDeadTimeConfigTypeDef g_sbreak2_dead_time8_config = {0};        /* ����ʱ������ */
void MoterR_pwm_chy_init(uint16_t arr, uint16_t psc) // ���ֵ��
{
    /*���ֵ��*/
    GPIO_InitTypeDef gpio_init_struct = {0};
    TIM_OC_InitTypeDef tim_oc_cplm_pwm = {0};

    ATIM_TIM8_CPLM_CLK_ENABLE();            /* TIMx ʱ��ʹ�� */
    ATIM_TIM8_CPLM_CH1_GPIO_CLK_ENABLE();   /* ͨ��X��ӦIO��ʱ��ʹ�� */
    ATIM_TIM8_CPLM_CH1N_GPIO_CLK_ENABLE();  /* ͨ��X����ͨ����ӦIO��ʱ��ʹ�� */
    ATIM_TIM8_CPLM_CH2_GPIO_CLK_ENABLE();   /* ͨ��X��ӦIO��ʱ��ʹ�� */
    ATIM_TIM8_CPLM_CH2N_GPIO_CLK_ENABLE();  /* ͨ��X����ͨ����ӦIO��ʱ��ʹ�� */
    
    ATIM_TIM8_CPLM_BKIN_GPIO_CLK_ENABLE();  /* ͨ��Xɲ�������ӦIO��ʱ��ʹ�� */
    gpio_init_struct.Pin = ATIM_TIM8_CPLM_BKIN_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;
    gpio_init_struct.Pull = GPIO_PULLDOWN;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_init_struct.Alternate = ATIM_TIM8_CPLM_CHY_GPIO_AF;                /* �˿ڸ��� */

    gpio_init_struct.Pin = ATIM_TIM8_CPLM_CH1_GPIO_PIN;
    HAL_GPIO_Init(ATIM_TIM8_CPLM_CH1_GPIO_PORT, &gpio_init_struct);

    gpio_init_struct.Pin = ATIM_TIM8_CPLM_CH1N_GPIO_PIN;
    HAL_GPIO_Init(ATIM_TIM8_CPLM_CH1N_GPIO_PORT, &gpio_init_struct);
    
    gpio_init_struct.Pin = ATIM_TIM8_CPLM_CH2_GPIO_PIN;
    HAL_GPIO_Init(ATIM_TIM8_CPLM_CH2_GPIO_PORT, &gpio_init_struct);

    gpio_init_struct.Pin = ATIM_TIM8_CPLM_CH2N_GPIO_PIN;
    HAL_GPIO_Init(ATIM_TIM8_CPLM_CH2N_GPIO_PORT, &gpio_init_struct);

    g_tim8_cplm_pwm_handle.Instance = ATIM_TIM8_CPLM;                       /* ��ʱ��x */
    g_tim8_cplm_pwm_handle.Init.Prescaler = psc;                            /* Ԥ��Ƶϵ�� */
    g_tim8_cplm_pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;           /* ���ϼ���ģʽ */
    g_tim8_cplm_pwm_handle.Init.Period = arr;                               /* �Զ���װ��ֵ */
    g_tim8_cplm_pwm_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;     /* CKD[1:0] = 10, tDTS = 4 * tCK_INT = Ft / 4 = 42Mhz*/
    g_tim8_cplm_pwm_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;  /* ʹ��Ӱ�ӼĴ���TIMx_ARR */
  
    HAL_TIM_PWM_Init(&g_tim8_cplm_pwm_handle) ;

    tim_oc_cplm_pwm.OCMode = TIM_OCMODE_PWM1;                               /* PWMģʽ1 */
    tim_oc_cplm_pwm.OCPolarity = TIM_OCPOLARITY_HIGH;                       /* OCy �ߵ�ƽ��Ч */
    tim_oc_cplm_pwm.OCNPolarity = TIM_OCPOLARITY_HIGH;                      /* OCyN �ߵ�ƽ��Ч */
    tim_oc_cplm_pwm.OCIdleState = TIM_OCIDLESTATE_RESET;                      /* ��MOE=0��OCx=0 */
    tim_oc_cplm_pwm.OCNIdleState = TIM_OCNIDLESTATE_RESET;                    /* ��MOE=0��OCxN=0 */
    HAL_TIM_PWM_ConfigChannel(&g_tim8_cplm_pwm_handle, &tim_oc_cplm_pwm, ATIM_TIM8_CPLM_CH1);
    HAL_TIM_PWM_ConfigChannel(&g_tim8_cplm_pwm_handle, &tim_oc_cplm_pwm, ATIM_TIM8_CPLM_CH2);
    
    /* �����������������������ж� */
    g_sbreak2_dead_time8_config.OffStateRunMode = TIM_OSSR_DISABLE;           /* ����ģʽ�Ĺر����״̬ */
    g_sbreak2_dead_time8_config.OffStateIDLEMode = TIM_OSSI_DISABLE;          /* ����ģʽ�Ĺر����״̬ */
    g_sbreak2_dead_time8_config.LockLevel = TIM_LOCKLEVEL_OFF;                /* ���üĴ��������� */
    g_sbreak2_dead_time8_config.BreakState = TIM_BREAK_ENABLE;                /* ʹ��ɲ������ */
    g_sbreak2_dead_time8_config.BreakPolarity = TIM_BREAKPOLARITY_HIGH;       /* ɲ��������Ч�źż���Ϊ�� */
    g_sbreak2_dead_time8_config.AutomaticOutput = TIM_AUTOMATICOUTPUT_ENABLE; /* ʹ��AOEλ������ɲ���������Զ��ָ���� */
    HAL_TIMEx_ConfigBreakDeadTime(&g_tim8_cplm_pwm_handle, &g_sbreak2_dead_time8_config);

    HAL_TIM_PWM_Start(&g_tim8_cplm_pwm_handle, ATIM_TIM8_CPLM_CH1);         /* OC1 ���ʹ�� */
    HAL_TIMEx_PWMN_Start(&g_tim8_cplm_pwm_handle, ATIM_TIM8_CPLM_CH1);      /* OC1N ���ʹ�� */
    
    HAL_TIM_PWM_Start(&g_tim8_cplm_pwm_handle, ATIM_TIM8_CPLM_CH2);         /* OC2 ���ʹ�� */
    HAL_TIMEx_PWMN_Start(&g_tim8_cplm_pwm_handle, ATIM_TIM8_CPLM_CH2);      /* OC2N ���ʹ�� */   
    RIGHT_SOFTBRAKE(0);//��ɲ��ֹͣ
}
void atim_tim8_cplm_pwm_set(uint16_t ccr1,uint16_t ccr2)
{
    g_sbreak2_dead_time8_config.DeadTime = 50;
    HAL_TIMEx_ConfigBreakDeadTime(&g_tim8_cplm_pwm_handle, &g_sbreak2_dead_time8_config);      /*��������ʱ��*/
    __HAL_TIM_MOE_ENABLE(&g_tim8_cplm_pwm_handle);      /* MOE=1,ʹ������� */    
    ATIM_TIM8_CPLM_CH1_CCR1 = ccr1;                      /* ���ñȽϼĴ��� */
    ATIM_TIM8_CPLM_CH2_CCR2 = ccr2;  
}

void RightMoterMove(uint8_t dir,double duty_cycle)
{
    RIGHT_SOFTBRAKE(0);
    if (dir==1)
    {
        atim_tim8_cplm_pwm_set((uint16_t)duty_cycle*100,0);
    }
    else if(dir==0)
    {
        atim_tim8_cplm_pwm_set(0,(uint16_t)duty_cycle*100);
    }
    else
    {
        RightMoterStop();   
    }
}

void RightMoterStop(void)
{
    RIGHT_SOFTBRAKE(1);
}

/**********************************�����ǶȳŸ�A1*************************************/

void Moterbackrest_pwm_chy_init(uint16_t arr, uint16_t psc)
{

    TIM_OC_InitTypeDef tim1_oc_pwm_chy = {0};                                                /* ��ʱ��9������ */
    g_time1_pwm_chy_handle.Instance = GTIM_TIM1_PWM;                                         /* ��ʱ��x */
    g_time1_pwm_chy_handle.Init.Prescaler = psc;                                             /* Ԥ��Ƶϵ�� */
    g_time1_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;                            /* ��������ģʽ */
    g_time1_pwm_chy_handle.Init.Period = arr;                                                /* �Զ���װ��ֵ */
    HAL_TIM_PWM_Init(&g_time1_pwm_chy_handle);                                               /* ��ʼ��PWM */
    tim1_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                                                /* ģʽѡ��PWM1 */
    tim1_oc_pwm_chy.Pulse = 0;                                                               /* ���ñȽ�ֵ,��ֵ����ȷ��ռ�ձ� */
    tim1_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;                                         /* ����Ƚϼ���Ϊ�� */
    HAL_TIM_PWM_ConfigChannel(&g_time1_pwm_chy_handle, &tim1_oc_pwm_chy, GTIM_TIM1_PWM_CH3); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH3);                           /* ������ӦPWMͨ�� */
    HAL_TIM_PWM_ConfigChannel(&g_time1_pwm_chy_handle, &tim1_oc_pwm_chy, GTIM_TIM1_PWM_CH4); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH4);                           /* ������ӦPWMͨ�� */
}
/**********************************����ǶȳŸ�B2*************************************/

void MoterPedestal_pwm_chy_init(uint16_t arr, uint16_t psc) // �綯�Ƹ� 2&3
{
    /*�Ƹ˵��2�����壩*/
    TIM_OC_InitTypeDef tim8_oc_pwm_chy = {0};                                                /* ��ʱ��9������ */
    g_time8_pwm_chy_handle.Instance = GTIM_TIM8_PWM;                                         /* ��ʱ��x */
    g_time8_pwm_chy_handle.Init.Prescaler = psc;                                             /* Ԥ��Ƶϵ�� */
    g_time8_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;                            /* ��������ģʽ */
    g_time8_pwm_chy_handle.Init.Period = arr;                                                /* �Զ���װ��ֵ */
    HAL_TIM_PWM_Init(&g_time8_pwm_chy_handle);                                               /* ��ʼ��PWM */
    tim8_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                                                /* ģʽѡ��PWM1 */
    tim8_oc_pwm_chy.Pulse = 0;                                                               /* ���ñȽ�ֵ,��ֵ����ȷ��ռ�ձ� */
    tim8_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;                                         /* ����Ƚϼ���Ϊ�� */
    HAL_TIM_PWM_ConfigChannel(&g_time8_pwm_chy_handle, &tim8_oc_pwm_chy, GTIM_TIM8_PWM_CH3); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH3);                           /* ������ӦPWMͨ�� */
    HAL_TIM_PWM_ConfigChannel(&g_time8_pwm_chy_handle, &tim8_oc_pwm_chy, GTIM_TIM8_PWM_CH4); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH4);                           /* ������ӦPWMͨ�� */

}
/**********************************���̾����Ÿ�B1(M)*************************************/
void MoterLift_pwm_chy_init(uint16_t arr, uint16_t psc) 
{

    TIM_OC_InitTypeDef tim3_oc_pwm_chy = {0};                                                /* ��ʱ��9������ */
    g_time3_pwm_chy_handle.Instance = GTIM_TIM3_PWM;                                         /* ��ʱ��x */
    g_time3_pwm_chy_handle.Init.Prescaler = psc;                                             /* Ԥ��Ƶϵ�� */
    g_time3_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;                            /* ��������ģʽ */
    g_time3_pwm_chy_handle.Init.Period = arr;                                                /* �Զ���װ��ֵ */
    HAL_TIM_PWM_Init(&g_time3_pwm_chy_handle);                                               /* ��ʼ��PWM */
    tim3_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                                                /* ģʽѡ��PWM1 */
    tim3_oc_pwm_chy.Pulse = 0;                                                               /* ���ñȽ�ֵ,��ֵ����ȷ��ռ�ձ� */
    tim3_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;                                         /* ����Ƚϼ���Ϊ�� */
    HAL_TIM_PWM_ConfigChannel(&g_time3_pwm_chy_handle, &tim3_oc_pwm_chy, GTIM_TIM3_PWM_CH1); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time3_pwm_chy_handle, GTIM_TIM3_PWM_CH1);                           /* ������ӦPWMͨ�� */
    HAL_TIM_PWM_ConfigChannel(&g_time3_pwm_chy_handle, &tim3_oc_pwm_chy, GTIM_TIM3_PWM_CH2); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time3_pwm_chy_handle, GTIM_TIM3_PWM_CH2);         
}
/**********************************���нǶȳŸ�A2 (L)�����г��ȳŸ�A3(L)*************************************/

void MoterLeg_pwm_chy_init(uint16_t arr, uint16_t psc)
{
    // ���нǶȳŸ�A2
    TIM_OC_InitTypeDef tim4_oc_pwm_chy = {0};                                                /* ��ʱ��9������ */
    g_time4_pwm_chy_handle.Instance = GTIM_TIM4_PWM;                                         /* ��ʱ��x */
    g_time4_pwm_chy_handle.Init.Prescaler = psc;                                             /* Ԥ��Ƶϵ�� */
    g_time4_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;                            /* ��������ģʽ */
    g_time4_pwm_chy_handle.Init.Period = arr;                                                /* �Զ���װ��ֵ */
    HAL_TIM_PWM_Init(&g_time4_pwm_chy_handle);                                               /* ��ʼ��PWM */
    tim4_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                                                /* ģʽѡ��PWM1 */
    tim4_oc_pwm_chy.Pulse = 0;                                                               /* ���ñȽ�ֵ,��ֵ����ȷ��ռ�ձ� */
    tim4_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;                                         /* ����Ƚϼ���Ϊ�� */
    HAL_TIM_PWM_ConfigChannel(&g_time4_pwm_chy_handle, &tim4_oc_pwm_chy, GTIM_TIM4_PWM_CH3); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH3);                           /* ������ӦPWMͨ�� */
    HAL_TIM_PWM_ConfigChannel(&g_time4_pwm_chy_handle, &tim4_oc_pwm_chy, GTIM_TIM4_PWM_CH4); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH4);                           /* ������ӦPWMͨ�� */
    /* �Ƹ�5 ���г��ȳŸ�A3*/
    HAL_TIM_PWM_ConfigChannel(&g_time4_pwm_chy_handle, &tim4_oc_pwm_chy, GTIM_TIM4_PWM_CH1); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH1);                           /* ������ӦPWMͨ�� */
    HAL_TIM_PWM_ConfigChannel(&g_time4_pwm_chy_handle, &tim4_oc_pwm_chy, GTIM_TIM4_PWM_CH2); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH2);                           /* ������ӦPWMͨ�� */
}
/**********************************ǰ֧�����Ƹ�C1*************************************/
void MoterSupport_pwm_chy_init(uint16_t arr, uint16_t psc) // �綯�Ƹ� 6
{
    
    /*�Ƹ˵��6 ǰ֧��*/
    TIM_OC_InitTypeDef tim12_oc_pwm_chy = {0};                                                  /* ��ʱ��12������ */
    g_time12_pwm_chy_handle.Instance = GTIM_TIM12_PWM;                                          /* ��ʱ��x */
    g_time12_pwm_chy_handle.Init.Prescaler = psc;                                               /* Ԥ��Ƶϵ�� */
    g_time12_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;                              /* ��������ģʽ */
    g_time12_pwm_chy_handle.Init.Period = arr;                                                  /* �Զ���װ��ֵ */
    HAL_TIM_PWM_Init(&g_time12_pwm_chy_handle);                                                 /* ��ʼ��PWM */
    tim12_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                                                  /* ģʽѡ��PWM1 */
    tim12_oc_pwm_chy.Pulse = 0;                                                                 /* ���ñȽ�ֵ,��ֵ����ȷ��ռ�ձ� */
    tim12_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;                                           /* ����Ƚϼ���Ϊ�� */
    HAL_TIM_PWM_ConfigChannel(&g_time12_pwm_chy_handle, &tim12_oc_pwm_chy, GTIM_TIM12_PWM_CH1); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time12_pwm_chy_handle, GTIM_TIM12_PWM_CH1);                            /* ������ӦPWMͨ�� */
    HAL_TIM_PWM_ConfigChannel(&g_time12_pwm_chy_handle, &tim12_oc_pwm_chy, GTIM_TIM12_PWM_CH2); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time12_pwm_chy_handle, GTIM_TIM12_PWM_CH2);                            /* ������ӦPWMͨ�� */
}

/**
 * @brief       ��ʱ���ײ�������ʱ��ʹ�ܣ���������
                �˺����ᱻHAL_TIM_PWM_Init()����
 * @param       htim:��ʱ�����
 * @retval      ��
 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == GTIM_TIM3_PWM)
    {  //����
        GPIO_InitTypeDef gpio_init_structB;
        GTIM_TIM3_PWM_CHY_GPIOB_CLK_ENABLE();                                              /* ����ͨ��y��GPIOʱ�� */
        GTIM_TIM3_PWM_CHY_CLK_ENABLE();                                                    /* ʹ�ܶ�ʱ��ʱ�� */
        gpio_init_structB.Pin = GTIM_TIM3_PWM_CHY_GPIO_PIN4 | GTIM_TIM3_PWM_CHY_GPIO_PIN5; /* ͨ��1 2��CPIO�� */
        gpio_init_structB.Mode = GPIO_MODE_AF_PP;                                          /* ����������� */
        gpio_init_structB.Pull = GPIO_PULLUP;                                              /* ���� */
        gpio_init_structB.Speed = GPIO_SPEED_FREQ_HIGH;                                    /* ���� */
        gpio_init_structB.Alternate = GTIM_TIM3_PWM_CHY_GPIO_AF;                           /* IO��REMAP����, �Ƿ��Ҫ�鿴ͷ�ļ����õ�˵��! */
        HAL_GPIO_Init(GTIM_TIM3_PWM_CHY_GPIO_PORTB, &gpio_init_structB);
    }

    if (htim->Instance == GTIM_TIM1_PWM)
    {
        //����
        GPIO_InitTypeDef gpio_init_structA;
        GTIM_TIM1_PWM_CHY_GPIOA_CLK_ENABLE();                                                /* ����ͨ��y��GPIOʱ�� */
        GTIM_TIM1_PWM_CHY_CLK_ENABLE();                                                      /* ʹ�ܶ�ʱ��ʱ�� */
        gpio_init_structA.Pin = GTIM_TIM1_PWM_CHY_GPIO_PIN10 | GTIM_TIM1_PWM_CHY_GPIO_PIN11; /* ͨ��1 2��CPIO�� */
        gpio_init_structA.Mode = GPIO_MODE_AF_PP;                                            /* ����������� */
        gpio_init_structA.Pull = GPIO_PULLUP;                                                /* ���� */
        gpio_init_structA.Speed = GPIO_SPEED_FREQ_HIGH;                                      /* ���� */
        gpio_init_structA.Alternate = GTIM_TIM1_PWM_CHY_GPIO_AF;                             /* IO��REMAP����, �Ƿ��Ҫ�鿴ͷ�ļ����õ�˵��! */
        HAL_GPIO_Init(GTIM_TIM1_PWM_CHY_GPIO_PORTA, &gpio_init_structA);
    }

    if (htim->Instance == GTIM_TIM8_PWM)
    {
        // ����
        GPIO_InitTypeDef gpio_init_structC;
        GTIM_TIM8_PWM_CHY_GPIOC_CLK_ENABLE();                                                                                                          /* ����ͨ��y��GPIOʱ�� */
        GTIM_TIM8_PWM_CHY_CLK_ENABLE();                                                                                                                /* ʹ�ܶ�ʱ��ʱ�� */
        gpio_init_structC.Pin = GTIM_TIM8_PWM_CHY_GPIO_PIN8 | GTIM_TIM8_PWM_CHY_GPIO_PIN9 ; /* ͨ��3 4��CPIO�� */
        gpio_init_structC.Mode = GPIO_MODE_AF_PP;                                                                                                      /* ����������� */
        gpio_init_structC.Pull = GPIO_PULLUP;                                                                                                          /* ���� */
        gpio_init_structC.Speed = GPIO_SPEED_FREQ_HIGH;                                                                                                /* ���� */
        gpio_init_structC.Alternate = GTIM_TIM8_PWM_CHY_GPIO_AF;                                                                                       /* IO��REMAP����, �Ƿ��Ҫ�鿴ͷ�ļ����õ�˵��! */
        HAL_GPIO_Init(GTIM_TIM8_PWM_CHY_GPIO_PORTC, &gpio_init_structC);
    }

    if (htim->Instance == GTIM_TIM4_PWM)
    {
        // �Ƹ�4&5
        GPIO_InitTypeDef gpio_init_structD;
        GTIM_TIM4_PWM_CHY_GPIOD_CLK_ENABLE();                                                                                                              /* ����ͨ��y��GPIOʱ�� */
        GTIM_TIM4_PWM_CHY_CLK_ENABLE();                                                                                                                    /* ʹ�ܶ�ʱ��ʱ�� */
        gpio_init_structD.Pin = GTIM_TIM4_PWM_CHY_GPIO_PIN12 | GTIM_TIM4_PWM_CHY_GPIO_PIN13 | GTIM_TIM4_PWM_CHY_GPIO_PIN14 | GTIM_TIM4_PWM_CHY_GPIO_PIN15; /* ͨ��1 2��CPIO�� */
        gpio_init_structD.Mode = GPIO_MODE_AF_PP;                                                                                                          /* ����������� */
        gpio_init_structD.Pull = GPIO_PULLUP;                                                                                                              /* ���� */
        gpio_init_structD.Speed = GPIO_SPEED_FREQ_HIGH;                                                                                                    /* ���� */
        gpio_init_structD.Alternate = GTIM_TIM4_PWM_CHY_GPIO_AF;                                                                                           /* IO��REMAP����, �Ƿ��Ҫ�鿴ͷ�ļ����õ�˵��! */
        HAL_GPIO_Init(GTIM_TIM4_PWM_CHY_GPIO_PORTD, &gpio_init_structD);
    }

    if (htim->Instance == GTIM_TIM12_PWM)
    {
        // �Ƹ�6
        GPIO_InitTypeDef gpio_init_structB;
        GTIM_TIM12_PWM_CHY_GPIOB_CLK_ENABLE();                                                 /* ����ͨ��y��GPIOʱ�� */
        GTIM_TIM12_PWM_CHY_CLK_ENABLE();                                                       /* ʹ�ܶ�ʱ��ʱ�� */
        gpio_init_structB.Pin = GTIM_TIM12_PWM_CHY_GPIO_PIN14 | GTIM_TIM12_PWM_CHY_GPIO_PIN15; /* ͨ��1 2��CPIO�� */
        gpio_init_structB.Mode = GPIO_MODE_AF_PP;                                              /* ����������� */
        gpio_init_structB.Pull = GPIO_PULLUP;                                                  /* ���� */
        gpio_init_structB.Speed = GPIO_SPEED_FREQ_HIGH;                                        /* ���� */
        gpio_init_structB.Alternate = GTIM_TIM12_PWM_CHY_GPIO_AF;                              /* IO��REMAP����, �Ƿ��Ҫ�鿴ͷ�ļ����õ�˵��! */
        HAL_GPIO_Init(GTIM_TIM12_PWM_CHY_GPIO_PORTB, &gpio_init_structB);
    }   
}

void MoterdriveInit(void)
{
    MoterL_pwm_chy_init(100 - 1, 84 - 1);                  //* 168 000 000 / 100*84   TIME1   L 20khzƵ�ʵ�PWM ����*  /
    MoterR_pwm_chy_init(100 - 1, 84 - 1);                  //* 168 000 000 / 100*84   TIME8   R  20khzƵ�ʵ�PWM ����*/
    Moterbackrest_pwm_chy_init(100 - 1, 84 - 1);           //* 168 000 000 / 100*84   TIME1   ���� 20khzƵ�ʵ�PWM ����*  /
    MoterPedestal_pwm_chy_init(100 - 1, 84 - 1);           //* 168 000 000 / 100*84   TIME8   ���� 20khzƵ�ʵ�PWM ����*  /
    MoterLift_pwm_chy_init(100-1,42-1);                     //* 84 000 000 / 100*42    TIME3   ���� 20khzƵ�ʵ�PWM ����*  /
    MoterLeg_pwm_chy_init(100-1,42-1);                      //* 84 000 000 / 100*42    TIME4   ���г���+�Ƕ� 20khzƵ�ʵ�PWM ����*  /
    MoterSupport_pwm_chy_init(100-1,42-1);                  //* 84 000 000 / 100*42    TIME12  ֧���� 20khzƵ�ʵ�PWM ����*  /
}
// �Ƹ˲��Գ���
void linearactuatorTest(void)
{

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
    // ��������λ���ź�
    ActorLimitPara.A1_Downpos =1400;
    ActorLimitPara.A1_Uppos = 4100; // ǰ���Ѿ�������λ�ƣ����ǻ�е����ʱ��Լ��;

    ActorLimitPara.A2_Downpos = 2200;
    ActorLimitPara.A2_Uppos = 3000; // �����Ѿ�������λ�Ƴ��ȣ���е����ʱ��Լ��

    ActorLimitPara.A3_Downpos = 0; // ����λ��Լ��
    ActorLimitPara.A3_Uppos = 3000;

    ActorLimitPara.B1_Downpos = 200;
    ActorLimitPara.B1_Uppos = 2500;

    ActorLimitPara.B2_Downpos = 200  ;
    ActorLimitPara.B2_Uppos = 2400;

    ActorLimitPara.C1_Uppos = 10000;
    ActorLimitPara.C2_Downpos = 0;

    static float acctemp = 0, acct = 0;
    static uint8_t accdoneflage = 0;
    static float Kp;

    Kp = B1MAX * SEAT_LIFTDROPRATIO / 2.0 + 0.5;

    /*���ξ�������*/
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

    /* ���ο�������*/
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

    /*�������*/

    else if (g_slaveReg[72] == 1)
    {
        linerun_state = Seat_tiltf_run;
    }
    else if (g_slaveReg[72] == 2)
    {
        linerun_state = Seat_tiltb_run;
    }

    /*����ǰ����*/
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
    /*����������ת*/
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
    /*���ж������ڳ���*/
    else if (g_slaveReg[71] == 1)
    {
        linerun_state = Legexten_run;
    }

    else if (g_slaveReg[71] == 2)
    {
        linerun_state = Legunexten_run;
    }
    /*һ��վ��/����*/
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

    if (CanKeybufReceive[1]!= 0 ) /*�������а�������*/
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
        // �����ǶȳŸ�A1
        __HAL_TIM_SET_COMPARE(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH3, T3_IN1);
        __HAL_TIM_SET_COMPARE(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH4, T3_IN2);
        // ���нǶȳŸ�A2
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH1, T5_IN1);
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH2, T5_IN2);
        // ���г��ȳŸ�A3
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH3, T4_IN1);
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH4, T4_IN2);
        // ���̾����Ÿ�B1(M)
        __HAL_TIM_SET_COMPARE(&g_time3_pwm_chy_handle, GTIM_TIM3_PWM_CH1, T1_IN1);
        __HAL_TIM_SET_COMPARE(&g_time3_pwm_chy_handle, GTIM_TIM3_PWM_CH2, T1_IN2);
        // ����ǶȳŸ�B2(L)
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH3, T2_IN1);
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH4, T2_IN2); 
        // ǰ֧�����Ƹ�C1
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
        T1_IN1 = 100 * (1.0 - 0);
        T1_IN2 = 100 * (1.0 - acctemp);
        T2_IN1 = 100 * (1.0 - 0);
        T2_IN2 = 100 * (1.0 - Kp * acctemp);
        // ����״̬�µ� ��λ����
        if ( adcdata.lift_pos  > ActorLimitPara.B1_Uppos)
        {
            T1_IN1 = 100;
            T1_IN2 = 100;
            //��ֱ����������״̬�£������Ƹ�Ӧ��ֹͣ
            T2_IN1 = 100;
            T2_IN2 = 100;
        }

        if (adcdata.pedestal_pos > ActorLimitPara.B2_Uppos)
        {
            T2_IN1 = 100;
            T2_IN2 = 100;
        }
        // ���̾����Ÿ�B1(M)
        __HAL_TIM_SET_COMPARE(&g_time3_pwm_chy_handle, GTIM_TIM3_PWM_CH1, T1_IN1);
        __HAL_TIM_SET_COMPARE(&g_time3_pwm_chy_handle, GTIM_TIM3_PWM_CH2, T1_IN2);
        // ����ǶȳŸ�B2(L)
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
        T1_IN1 = 100 * (1.0 - acctemp);
        T1_IN2 = 100 * (1.0 - 0);
        T2_IN1 = 100 * (1.0 - Kp * acctemp);
        T2_IN2 = 100 * (1.0 - 0);
        // ���̾����Ÿ�B1(M)
        __HAL_TIM_SET_COMPARE(&g_time3_pwm_chy_handle, GTIM_TIM3_PWM_CH1, T1_IN1);
        __HAL_TIM_SET_COMPARE(&g_time3_pwm_chy_handle, GTIM_TIM3_PWM_CH2, T1_IN2);
        // ����ǶȳŸ�B2(L)
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
        T3_IN1 = 100 * (1.0 - 0);
        T3_IN2 = 100 * (1.0 - acctemp);
        /*ǰ���Ƹ�λ��Լ��*/
        if (adcdata.backboard_pos > ActorLimitPara.A1_Uppos)
        {
                T3_IN1 = 100;
                T3_IN2 = 100;
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
        T3_IN1 = 100 * (1.0 - acctemp);
        T3_IN2 = 100 * (1.0 - 0);
        /*�����Ƹ�λ��Լ��*/
        if (adcdata.backboard_pos < ActorLimitPara.A1_Downpos)
        {
                T3_IN1 = 100;
                T3_IN2 = 100;
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
        T1_IN1 = 100 * (1.0 - acctemp);
        T1_IN2 = 100 * (1.0 - 0);
        /*����ǰ/����Լ��*/
        // if ( adcdata.lift_pos  > ActorLimitPara.B1_Uppos)
        // {
        //     T1_IN1 = 200;
        //     T1_IN2 = 200;
        // }
        // ���̾����Ÿ�B1(M)
        __HAL_TIM_SET_COMPARE(&g_time3_pwm_chy_handle, GTIM_TIM3_PWM_CH1, T1_IN1);
        __HAL_TIM_SET_COMPARE(&g_time3_pwm_chy_handle, GTIM_TIM3_PWM_CH2, T1_IN2);
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
        T1_IN1 = 100 * (1.0 - 0);
        T1_IN2 = 100 * (1.0 - acctemp);
        /*�������Լ��*/
        if ( adcdata.lift_pos  > ActorLimitPara.B1_Uppos)
        {
            T1_IN1 = 100;
            T1_IN2 = 100;
        }    
        // ���̾����Ÿ�B1(M)
        __HAL_TIM_SET_COMPARE(&g_time3_pwm_chy_handle, GTIM_TIM3_PWM_CH1, T1_IN1);
        __HAL_TIM_SET_COMPARE(&g_time3_pwm_chy_handle, GTIM_TIM3_PWM_CH2, T1_IN2);
       printf("houqing_run\n");
        break;

    case Legspintop_run:
        // ��������
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
        /*���������쳤Լ��*/
        if (adcdata.leglength_pos > ActorLimitPara.A3_Uppos)
        {
            T4_IN1 = 100;
            T4_IN2 = 100;
        }
        /*������ת��Լ��*/
        // if (adcdata.legangle_pos > ActorLimitPara.A2_Uppos)
        // {
        //     T4_IN1 = 0;

        //     T4_IN2 = 0;
        // }
        // ���г��ȳŸ�A3
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH3, T4_IN1);
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH4, T4_IN2);
        // ���нǶȳŸ�A2
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH1, T5_IN1);
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH2, T5_IN2);
        break;
    case Legspindown_run:
        // ��������
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
        /*�����������г�����Լ��*/
        // if (adcdata.leglength_pos > ActorLimitPara.A3_Uppos)
        // {
        //     T4_IN1 = 0;
        //     T4_IN2 = 0;
        // }
        /*���нǶ���תԼ��*/
        if (adcdata.legangle_pos < ActorLimitPara.A2_Downpos)
        {
            T5_IN1 = 100;

            T5_IN2 = 100;
        }

        // ���г��ȳŸ�A3
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH3, T4_IN1);
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH4, T4_IN2);
        // ���нǶȳŸ�A2
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH1, T5_IN1);
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH2, T5_IN2);
        break;
    case Legexten_run:
        // ���ж����쳤����
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
            /*���г���Լ��*/
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
        T2_IN1 = 100 * (1.0 - 0);
        T2_IN2 = 100 * (1.0 - acctemp);
        /*����ǰ��Լ��*/
        if (adcdata.pedestal_pos > ActorLimitPara.B2_Uppos)
        {
            T2_IN1 = 100;
            T2_IN2 = 100;
        }
        // ����ǶȳŸ�B2(L)
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
        T2_IN1 = 100 * (1.0 - acctemp);
        T2_IN2 = 100 * (1.0 - 0);
        // ����ǶȳŸ�B2(L)
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH3, T2_IN1);
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH4, T2_IN2);

        break;

    default:
        break;
    
    }
}