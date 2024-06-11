/**
 ****************************************************************************************************
 * @file        moterdriver.c
 * @author      Lisir
 * @version     V1.0
 * @date        2021-10-14
 * @brief       moterdriver ��������
 * @license     Copyright (c) 2024, ���ڸ���ҽ�ƿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:F407������
 * none
 * none
 * none
 * none
 * ��ʱ��������
 *     ���� L  - gtim_tim3_pwm_chy_init - CH3 CH4 ----------
 *     ���� R  - gtim_tim9_pwm_chy_init - CH1 CH2
 *     �Ƹ�1 �������� gtim_tim1_pwm_chy_init - CH1 CH2
 *     �Ƹ�2 �����壩 gtim_tim8_pwm_chy_init -  CH1 CH2
 *     �Ƹ�3 �������� gtim_tim8_pwm_chy_init -  CH3 CH4
 *     �Ƹ�4 ����̤��ת�� gtim_tim4_pwm_chy_init -  CH3 CH4
 *     �Ƹ�5 ����̤�쳤�� gtim_tim4_pwm_chy_init -  CH1 CH2
 *     �Ƹ�6 ��ǰ֧���֣� gtim_tim12_pwm_chy_init -  CH1 CH2
 *
 *
 *
 * �޸�˵��
 * none
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./BSP/R9/moterdriver.h"

TIM_HandleTypeDef g_time3_pwm_chy_handle;  /* ����L��� 1 �������*/
TIM_HandleTypeDef g_time9_pwm_chy_handle;  /* ����R��� 2 �������*/
TIM_HandleTypeDef g_time1_pwm_chy_handle;  /* �Ƹ�1 ��������  TIME1 �������*/
TIM_HandleTypeDef g_time8_pwm_chy_handle;  /* �Ƹ�2 �����壩    �Ƹ�3 �������� TIME8 �������*/
TIM_HandleTypeDef g_time4_pwm_chy_handle;  /* �Ƹ�4 ����̤��ת�� �Ƹ�5 ����̤�쳤��TIME1 �������*/
TIM_HandleTypeDef g_time12_pwm_chy_handle; /*�Ƹ�6 ��ǰ֧���֣� TIME12 �������*/

LineActor_State linerun_state;
ACTORLIMITPARA ActorLimitPara;
void MoterL_pwm_chy_init(uint16_t arr, uint16_t psc) // ���ֵ��
{
    /*�����ֵ��*/
    TIM_OC_InitTypeDef tim3_oc_pwm_chy = {0};                                                /* ��ʱ��3������ */
    g_time3_pwm_chy_handle.Instance = GTIM_TIM3_PWM;                                         /* ��ʱ��x */
    g_time3_pwm_chy_handle.Init.Prescaler = psc;                                             /* Ԥ��Ƶϵ�� */
    g_time3_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;                            /* ��������ģʽ */
    g_time3_pwm_chy_handle.Init.Period = arr;                                                /* �Զ���װ��ֵ */
    HAL_TIM_PWM_Init(&g_time3_pwm_chy_handle);                                               /* ��ʼ��PWM */
    tim3_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                                                /* ģʽѡ��PWM1 */
    tim3_oc_pwm_chy.Pulse = 0;                                                               /* ���ñȽ�ֵ,��ֵ����ȷ��ռ�ձ� */
    tim3_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;                                         /* ����Ƚϼ���Ϊ�� */
    HAL_TIM_PWM_ConfigChannel(&g_time3_pwm_chy_handle, &tim3_oc_pwm_chy, GTIM_TIM3_PWM_CH3); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time3_pwm_chy_handle, GTIM_TIM3_PWM_CH3);                           /* ������ӦPWMͨ�� */
    HAL_TIM_PWM_ConfigChannel(&g_time3_pwm_chy_handle, &tim3_oc_pwm_chy, GTIM_TIM3_PWM_CH4); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time3_pwm_chy_handle, GTIM_TIM3_PWM_CH4);                           /* ������ӦPWMͨ�� */
}
void MoterR_pwm_chy_init(uint16_t arr, uint16_t psc) // ���ֵ��
{
    /*���ֵ��*/
    TIM_OC_InitTypeDef tim9_oc_pwm_chy = {0};                                                /* ��ʱ��9������ */
    g_time9_pwm_chy_handle.Instance = GTIM_TIM9_PWM;                                         /* ��ʱ��x */
    g_time9_pwm_chy_handle.Init.Prescaler = psc;                                             /* Ԥ��Ƶϵ�� */
    g_time9_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;                            /* ��������ģʽ */
    g_time9_pwm_chy_handle.Init.Period = arr;                                                /* �Զ���װ��ֵ */
    HAL_TIM_PWM_Init(&g_time9_pwm_chy_handle);                                               /* ��ʼ��PWM */
    tim9_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                                                /* ģʽѡ��PWM1 */
    tim9_oc_pwm_chy.Pulse = 0;                                                               /* ���ñȽ�ֵ,��ֵ����ȷ��ռ�ձ� */
    tim9_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;                                         /* ����Ƚϼ���Ϊ�� */
    HAL_TIM_PWM_ConfigChannel(&g_time9_pwm_chy_handle, &tim9_oc_pwm_chy, GTIM_TIM9_PWM_CH1); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time9_pwm_chy_handle, GTIM_TIM9_PWM_CH1);                           /* ������ӦPWMͨ�� */
    HAL_TIM_PWM_ConfigChannel(&g_time9_pwm_chy_handle, &tim9_oc_pwm_chy, GTIM_TIM9_PWM_CH2); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time9_pwm_chy_handle, GTIM_TIM9_PWM_CH2);                           /* ������ӦPWMͨ�� */
}

/*�����ǶȳŸ�A1*/
void MoterLift_pwm_chy_init(uint16_t arr, uint16_t psc)
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

/*���̾����Ÿ�B1 / ����ǶȳŸ�B2*/
void MoterPedestal_Backboard_pwm_chy_init(uint16_t arr, uint16_t psc) // �綯�Ƹ� 2&3
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
    /*����*/
    HAL_TIM_PWM_ConfigChannel(&g_time8_pwm_chy_handle, &tim8_oc_pwm_chy, GTIM_TIM8_PWM_CH1); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH1);                           /* ������ӦPWMͨ�� */
    HAL_TIM_PWM_ConfigChannel(&g_time8_pwm_chy_handle, &tim8_oc_pwm_chy, GTIM_TIM8_PWM_CH2); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH2);                           /* ������ӦPWMͨ�� */
}

/*���нǶȳŸ�A2 (L)�����г��ȳŸ�A3(L)*/
void MoterLeg_pwm_chy_init(uint16_t arr, uint16_t psc) // �綯�Ƹ� 4&5
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
    { // ����
        GPIO_InitTypeDef gpio_init_structB;
        GTIM_TIM3_PWM_CHY_GPIOB_CLK_ENABLE();                                              /* ����ͨ��y��GPIOʱ�� */
        GTIM_TIM3_PWM_CHY_CLK_ENABLE();                                                    /* ʹ�ܶ�ʱ��ʱ�� */
        gpio_init_structB.Pin = GTIM_TIM3_PWM_CHY_GPIO_PIN0 | GTIM_TIM3_PWM_CHY_GPIO_PIN1; /* ͨ��1 2��CPIO�� */
        gpio_init_structB.Mode = GPIO_MODE_AF_PP;                                          /* ����������� */
        gpio_init_structB.Pull = GPIO_PULLUP;                                              /* ���� */
        gpio_init_structB.Speed = GPIO_SPEED_FREQ_HIGH;                                    /* ���� */
        gpio_init_structB.Alternate = GTIM_TIM3_PWM_CHY_GPIO_AF;                           /* IO��REMAP����, �Ƿ��Ҫ�鿴ͷ�ļ����õ�˵��! */
        HAL_GPIO_Init(GTIM_TIM3_PWM_CHY_GPIO_PORTB, &gpio_init_structB);
    }
    if (htim->Instance == GTIM_TIM9_PWM)
    { // ����
        GPIO_InitTypeDef gpio_init_structE;
        GTIM_TIM9_PWM_CHY_GPIOE_CLK_ENABLE();                                              /* ����ͨ��y��GPIOʱ�� */
        GTIM_TIM9_PWM_CHY_CLK_ENABLE();                                                    /* ʹ�ܶ�ʱ��ʱ�� */
        gpio_init_structE.Pin = GTIM_TIM9_PWM_CHY_GPIO_PIN5 | GTIM_TIM9_PWM_CHY_GPIO_PIN6; /* ͨ��1 2��CPIO�� */
        gpio_init_structE.Mode = GPIO_MODE_AF_PP;                                          /* ����������� */
        gpio_init_structE.Pull = GPIO_PULLUP;                                              /* ���� */
        gpio_init_structE.Speed = GPIO_SPEED_FREQ_HIGH;                                    /* ���� */
        gpio_init_structE.Alternate = GTIM_TIM9_PWM_CHY_GPIO_AF;                           /* IO��REMAP����, �Ƿ��Ҫ�鿴ͷ�ļ����õ�˵��! */
        HAL_GPIO_Init(GTIM_TIM9_PWM_CHY_GPIO_PORTE, &gpio_init_structE);
    }

    if (htim->Instance == GTIM_TIM1_PWM)
    {
        // �Ƹ�1
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
        // �Ƹ�2&3
        GPIO_InitTypeDef gpio_init_structC;
        GTIM_TIM8_PWM_CHY_GPIOC_CLK_ENABLE();                                                                                                          /* ����ͨ��y��GPIOʱ�� */
        GTIM_TIM8_PWM_CHY_CLK_ENABLE();                                                                                                                /* ʹ�ܶ�ʱ��ʱ�� */
        gpio_init_structC.Pin = GTIM_TIM8_PWM_CHY_GPIO_PIN8 | GTIM_TIM8_PWM_CHY_GPIO_PIN9 | GTIM_TIM8_PWM_CHY_GPIO_PIN6 | GTIM_TIM8_PWM_CHY_GPIO_PIN7; /* ͨ��1 2��CPIO�� */
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
    MoterL_pwm_chy_init(100 - 1, 42 - 1);                  //* 84 000 000 / 100*42      L 20khzƵ�ʵ�PWM ����*  /
    MoterR_pwm_chy_init(200 - 1, 42 - 1);                  //* 168 000 000 / 200*42     R  20khzƵ�ʵ�PWM ����*/
    MoterLift_pwm_chy_init(200 - 1, 42 - 1);               //* 168 000 000 / 200*42     T1  20khzƵ�ʵ�PWM ����*/
    MoterPedestal_Backboard_pwm_chy_init(200 - 1, 42 - 1); //                   T2&T3
    MoterLeg_pwm_chy_init(100 - 1, 42 - 1);                //                   T4&T5
    MoterSupport_pwm_chy_init(100 - 1, 42 - 1);            //* 84 000 000 / 100*42      T6  20khzƵ�ʵ�PWM ����*  /
}
// �Ƹ˲��Գ���
void linearactuatorTest(void)
{

    // SeatLiftDrop();
//    SeatLiftDrop();
    // ThwartFB(); // ��Զ�̶�
    // OverallFB();
    // LegSpinFB();
    // LegExtension(); // ��Զ�̶�
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
    // ��������λ���ź�
    ActorLimitPara.A1_Downpos =1400;
    ActorLimitPara.A1_Uppos = 4100; // ǰ���Ѿ�������λ�ƣ����ǻ�е����ʱ��Լ��;

    ActorLimitPara.A2_Downpos = 2200;
    ActorLimitPara.A2_Uppos = 3000; // �����Ѿ�������λ�Ƴ��ȣ���е����ʱ��Լ��

    ActorLimitPara.A3_Downpos = 0; // ����λ��Լ��
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
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH1, T1_IN1);
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH2, T1_IN2);
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
        T1_IN1 = 200 * (1.0 - 0);
        T1_IN2 = 200 * (1.0 - acctemp);
        T2_IN1 = 200 * (1.0 - 0);
        T2_IN2 = 200 * (1.0 - Kp * acctemp);
        // ����״̬�µ� ��λ����
        if ( adcdata.lift_pos  > ActorLimitPara.B1_Uppos)
        {
            T1_IN1 = 200;
            T1_IN2 = 200;
            //��ֱ����������״̬�£������Ƹ�Ӧ��ֹͣ
            T2_IN1 = 200;
            T2_IN2 = 200;
        }

        if (adcdata.pedestal_pos > ActorLimitPara.B2_Uppos)
        {
            T2_IN1 = 200;
            T2_IN2 = 200;
        }
        // ���̾����Ÿ�B1(M)
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH1, T1_IN1);
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH2, T1_IN2);
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
        T1_IN1 = 200 * (1.0 - acctemp);
        T1_IN2 = 200 * (1.0 - 0);
        T2_IN1 = 200 * (1.0 - Kp * acctemp);
        T2_IN2 = 200 * (1.0 - 0);
        // ���̾����Ÿ�B1(M)
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH1, T1_IN1);
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH2, T1_IN2);
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
        T3_IN1 = 200 * (1.0 - 0);
        T3_IN2 = 200 * (1.0 - acctemp);
        /*ǰ���Ƹ�λ��Լ��*/
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
        /*�����Ƹ�λ��Լ��*/
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
        /*����ǰ/����Լ��*/
        // if ( adcdata.lift_pos  > ActorLimitPara.B1_Uppos)
        // {
        //     T1_IN1 = 200;
        //     T1_IN2 = 200;
        // }
        // ���̾����Ÿ�B1(M)
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
        /*�������Լ��*/
        if ( adcdata.lift_pos  > ActorLimitPara.B1_Uppos)
        {
            T1_IN1 = 200;
            T1_IN2 = 200;
        }    
        // ���̾����Ÿ�B1(M)
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH1, T1_IN1);
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH2, T1_IN2);
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
        T2_IN1 = 200 * (1.0 - 0);
        T2_IN2 = 200 * (1.0 - acctemp);
        /*����ǰ��Լ��*/
        if (adcdata.pedestal_pos > ActorLimitPara.B2_Uppos)
        {
            T2_IN1 = 200;
            T2_IN2 = 200;
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
        T2_IN1 = 200 * (1.0 - acctemp);
        T2_IN2 = 200 * (1.0 - 0);
        // ����ǶȳŸ�B2(L)
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH3, T2_IN1);
        __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH4, T2_IN2);

        break;

    default:
        break;
    
    }
}