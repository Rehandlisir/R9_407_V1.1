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
 *     ���� L  - gtim_tim5_pwm_chy_init - CH1 CH2 ----------
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
TIM_HandleTypeDef g_time5_pwm_chy_handle ;  /* ����L��� 1 �������*/
TIM_HandleTypeDef g_time9_pwm_chy_handle ;  /* ����R��� 2 �������*/
TIM_HandleTypeDef g_time1_pwm_chy_handle ;  /* �Ƹ�1 ��������  TIME1 �������*/
TIM_HandleTypeDef g_time8_pwm_chy_handle ;  /* �Ƹ�2 �����壩    �Ƹ�3 �������� TIME8 �������*/
TIM_HandleTypeDef g_time4_pwm_chy_handle ;  /* �Ƹ�4 ����̤��ת�� �Ƹ�5 ����̤�쳤��TIME1 �������*/
TIM_HandleTypeDef g_time12_pwm_chy_handle ;  /*�Ƹ�6 ��ǰ֧���֣� TIME12 �������*/
void MoterL_pwm_chy_init(uint16_t arr, uint16_t psc)  // ���ֵ��
{
    /*�����ֵ��*/
    TIM_OC_InitTypeDef tim5_oc_pwm_chy = {0};                       /* ��ʱ��5������ */
    g_time5_pwm_chy_handle.Instance = GTIM_TIM5_PWM;                 /* ��ʱ��x */
    g_time5_pwm_chy_handle.Init.Prescaler = psc;                      /* Ԥ��Ƶϵ�� */
    g_time5_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;    /* ��������ģʽ */
    g_time5_pwm_chy_handle.Init.Period = arr;                        /* �Զ���װ��ֵ */
    HAL_TIM_PWM_Init(&g_time5_pwm_chy_handle);                       /* ��ʼ��PWM */
    tim5_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                       /* ģʽѡ��PWM1 */
    tim5_oc_pwm_chy.Pulse = 0;                                /* ���ñȽ�ֵ,��ֵ����ȷ��ռ�ձ� */
    tim5_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;                                        /* ����Ƚϼ���Ϊ�� */
    HAL_TIM_PWM_ConfigChannel(&g_time5_pwm_chy_handle, &tim5_oc_pwm_chy, GTIM_TIM5_PWM_CH1); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time5_pwm_chy_handle, GTIM_TIM5_PWM_CH1);                           /* ������ӦPWMͨ�� */
	HAL_TIM_PWM_ConfigChannel(&g_time5_pwm_chy_handle, &tim5_oc_pwm_chy, GTIM_TIM5_PWM_CH2); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time5_pwm_chy_handle, GTIM_TIM5_PWM_CH2);                           /* ������ӦPWMͨ�� */
}
void  MoterR_pwm_chy_init(uint16_t arr, uint16_t psc) // ���ֵ��
{
    /*���ֵ��*/
    TIM_OC_InitTypeDef tim9_oc_pwm_chy = {0};                       /* ��ʱ��9������ */
    g_time9_pwm_chy_handle.Instance = GTIM_TIM9_PWM;                 /* ��ʱ��x */
    g_time9_pwm_chy_handle.Init.Prescaler = psc;                     /* Ԥ��Ƶϵ�� */
    g_time9_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;    /* ��������ģʽ */
    g_time9_pwm_chy_handle.Init.Period = arr;                        /* �Զ���װ��ֵ */
    HAL_TIM_PWM_Init(&g_time9_pwm_chy_handle);                       /* ��ʼ��PWM */
    tim9_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                       /* ģʽѡ��PWM1 */
    tim9_oc_pwm_chy.Pulse = 0;                                /* ���ñȽ�ֵ,��ֵ����ȷ��ռ�ձ� */
    tim9_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;                                        /* ����Ƚϼ���Ϊ�� */
    HAL_TIM_PWM_ConfigChannel(&g_time9_pwm_chy_handle, &tim9_oc_pwm_chy, GTIM_TIM9_PWM_CH1); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time9_pwm_chy_handle, GTIM_TIM9_PWM_CH1);                           /* ������ӦPWMͨ�� */
	HAL_TIM_PWM_ConfigChannel(&g_time9_pwm_chy_handle, &tim9_oc_pwm_chy, GTIM_TIM9_PWM_CH2); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time9_pwm_chy_handle, GTIM_TIM9_PWM_CH2);                           /* ������ӦPWMͨ�� */
}

void  MoterLift_pwm_chy_init(uint16_t arr, uint16_t psc) // �綯�Ƹ� 1 
{
    /*�Ƹ˵��1��������*/
    TIM_OC_InitTypeDef tim1_oc_pwm_chy = {0};                       /* ��ʱ��9������ */
    g_time1_pwm_chy_handle.Instance = GTIM_TIM1_PWM;                 /* ��ʱ��x */
    g_time1_pwm_chy_handle.Init.Prescaler = psc;                     /* Ԥ��Ƶϵ�� */
    g_time1_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;    /* ��������ģʽ */
    g_time1_pwm_chy_handle.Init.Period = arr;                        /* �Զ���װ��ֵ */
    HAL_TIM_PWM_Init(&g_time1_pwm_chy_handle);                       /* ��ʼ��PWM */
    tim1_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                       /* ģʽѡ��PWM1 */
    tim1_oc_pwm_chy.Pulse = 0;                                /* ���ñȽ�ֵ,��ֵ����ȷ��ռ�ձ� */
    tim1_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;                                        /* ����Ƚϼ���Ϊ�� */
    HAL_TIM_PWM_ConfigChannel(&g_time1_pwm_chy_handle, &tim1_oc_pwm_chy, GTIM_TIM1_PWM_CH1); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH1);                           /* ������ӦPWMͨ�� */
	HAL_TIM_PWM_ConfigChannel(&g_time1_pwm_chy_handle, &tim1_oc_pwm_chy, GTIM_TIM1_PWM_CH2); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH2);                           /* ������ӦPWMͨ�� */
}

void  MoterPedestal_Backboard_pwm_chy_init(uint16_t arr, uint16_t psc) // �綯�Ƹ� 2&3 
{
    /*�Ƹ˵��2�����壩*/
    TIM_OC_InitTypeDef tim8_oc_pwm_chy = {0};                       /* ��ʱ��9������ */
    g_time8_pwm_chy_handle.Instance = GTIM_TIM8_PWM;                 /* ��ʱ��x */
    g_time8_pwm_chy_handle.Init.Prescaler = psc;                     /* Ԥ��Ƶϵ�� */
    g_time8_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;    /* ��������ģʽ */
    g_time8_pwm_chy_handle.Init.Period = arr;                        /* �Զ���װ��ֵ */
    HAL_TIM_PWM_Init(&g_time8_pwm_chy_handle);                       /* ��ʼ��PWM */
    tim8_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                       /* ģʽѡ��PWM1 */
    tim8_oc_pwm_chy.Pulse = 0;                                /* ���ñȽ�ֵ,��ֵ����ȷ��ռ�ձ� */
    tim8_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;                                        /* ����Ƚϼ���Ϊ�� */
    HAL_TIM_PWM_ConfigChannel(&g_time8_pwm_chy_handle, &tim8_oc_pwm_chy, GTIM_TIM8_PWM_CH3); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH3);                           /* ������ӦPWMͨ�� */
	HAL_TIM_PWM_ConfigChannel(&g_time8_pwm_chy_handle, &tim8_oc_pwm_chy, GTIM_TIM8_PWM_CH4); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH4);                           /* ������ӦPWMͨ�� */
    /*�Ƹ˵��3��������*/
    HAL_TIM_PWM_ConfigChannel(&g_time8_pwm_chy_handle, &tim8_oc_pwm_chy, GTIM_TIM8_PWM_CH1); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH1);                           /* ������ӦPWMͨ�� */
	HAL_TIM_PWM_ConfigChannel(&g_time8_pwm_chy_handle, &tim8_oc_pwm_chy, GTIM_TIM8_PWM_CH2); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH2);                           /* ������ӦPWMͨ�� */

}

void  MoterLeg_pwm_chy_init(uint16_t arr, uint16_t psc) // �綯�Ƹ� 4&5 
{
   // �Ƹ�4 ����̤��ת��
    TIM_OC_InitTypeDef tim4_oc_pwm_chy = {0};                       /* ��ʱ��9������ */
    g_time4_pwm_chy_handle.Instance = GTIM_TIM4_PWM;                 /* ��ʱ��x */
    g_time4_pwm_chy_handle.Init.Prescaler = psc;                     /* Ԥ��Ƶϵ�� */
    g_time4_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;    /* ��������ģʽ */
    g_time4_pwm_chy_handle.Init.Period = arr;                        /* �Զ���װ��ֵ */
    HAL_TIM_PWM_Init(&g_time4_pwm_chy_handle);                       /* ��ʼ��PWM */
    tim4_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                       /* ģʽѡ��PWM1 */
    tim4_oc_pwm_chy.Pulse = 0;                                /* ���ñȽ�ֵ,��ֵ����ȷ��ռ�ձ� */
    tim4_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;                                        /* ����Ƚϼ���Ϊ�� */
    HAL_TIM_PWM_ConfigChannel(&g_time4_pwm_chy_handle, &tim4_oc_pwm_chy, GTIM_TIM4_PWM_CH3); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH3);                           /* ������ӦPWMͨ�� */
	HAL_TIM_PWM_ConfigChannel(&g_time4_pwm_chy_handle, &tim4_oc_pwm_chy, GTIM_TIM4_PWM_CH4); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH4);                           /* ������ӦPWMͨ�� */
    /* �Ƹ�5 ����̤�쳤��*/
    HAL_TIM_PWM_ConfigChannel(&g_time4_pwm_chy_handle, &tim4_oc_pwm_chy, GTIM_TIM4_PWM_CH1); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH1);                           /* ������ӦPWMͨ�� */
	HAL_TIM_PWM_ConfigChannel(&g_time4_pwm_chy_handle, &tim4_oc_pwm_chy, GTIM_TIM4_PWM_CH2); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH2);                           /* ������ӦPWMͨ�� */

}

void MoterSupport_pwm_chy_init(uint16_t arr, uint16_t psc) // �綯�Ƹ� 6 
{
    /*�Ƹ˵��6 ǰ֧��*/
    TIM_OC_InitTypeDef tim12_oc_pwm_chy = {0};                       /* ��ʱ��12������ */
    g_time12_pwm_chy_handle.Instance = GTIM_TIM12_PWM;                 /* ��ʱ��x */
    g_time12_pwm_chy_handle.Init.Prescaler = psc;                     /* Ԥ��Ƶϵ�� */
    g_time12_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;    /* ��������ģʽ */
    g_time12_pwm_chy_handle.Init.Period = arr;                        /* �Զ���װ��ֵ */
    HAL_TIM_PWM_Init(&g_time12_pwm_chy_handle);                       /* ��ʼ��PWM */
    tim12_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                       /* ģʽѡ��PWM1 */
    tim12_oc_pwm_chy.Pulse = 0;                                /* ���ñȽ�ֵ,��ֵ����ȷ��ռ�ձ� */
    tim12_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;                                        /* ����Ƚϼ���Ϊ�� */
    HAL_TIM_PWM_ConfigChannel(&g_time12_pwm_chy_handle, &tim12_oc_pwm_chy, GTIM_TIM12_PWM_CH1); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time12_pwm_chy_handle, GTIM_TIM12_PWM_CH1);                           /* ������ӦPWMͨ�� */
	HAL_TIM_PWM_ConfigChannel(&g_time12_pwm_chy_handle, &tim12_oc_pwm_chy, GTIM_TIM12_PWM_CH2); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_time12_pwm_chy_handle, GTIM_TIM12_PWM_CH2);                           /* ������ӦPWMͨ�� */
}

/**
 * @brief       ��ʱ���ײ�������ʱ��ʹ�ܣ���������
                �˺����ᱻHAL_TIM_PWM_Init()����
 * @param       htim:��ʱ�����
 * @retval      ��
 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == GTIM_TIM5_PWM)
    {   // ����
        GPIO_InitTypeDef gpio_init_structA;
		GTIM_TIM5_PWM_CHY_GPIOA_CLK_ENABLE();                            /* ����ͨ��y��GPIOʱ�� */
        GTIM_TIM5_PWM_CHY_CLK_ENABLE();                                 /* ʹ�ܶ�ʱ��ʱ�� */
        gpio_init_structA.Pin = GTIM_TIM5_PWM_CHY_GPIO_PIN0 | GTIM_TIM5_PWM_CHY_GPIO_PIN1;              /* ͨ��1 2��CPIO�� */
        gpio_init_structA.Mode = GPIO_MODE_AF_PP;                        /* ����������� */
        gpio_init_structA.Pull = GPIO_PULLUP;                            /* ���� */
        gpio_init_structA.Speed = GPIO_SPEED_FREQ_HIGH;                  /* ���� */
        gpio_init_structA.Alternate = GTIM_TIM5_PWM_CHY_GPIO_AF;         /* IO��REMAP����, �Ƿ��Ҫ�鿴ͷ�ļ����õ�˵��! */
        HAL_GPIO_Init(GTIM_TIM5_PWM_CHY_GPIO_PORTA, &gpio_init_structA);

    }
    if (htim->Instance == GTIM_TIM9_PWM)
    {   // ����
        GPIO_InitTypeDef gpio_init_structE;
		GTIM_TIM9_PWM_CHY_GPIOE_CLK_ENABLE();                            /* ����ͨ��y��GPIOʱ�� */
        GTIM_TIM9_PWM_CHY_CLK_ENABLE();                                 /* ʹ�ܶ�ʱ��ʱ�� */
        gpio_init_structE.Pin = GTIM_TIM9_PWM_CHY_GPIO_PIN5 | GTIM_TIM9_PWM_CHY_GPIO_PIN6;              /* ͨ��1 2��CPIO�� */
        gpio_init_structE.Mode = GPIO_MODE_AF_PP;                        /* ����������� */
        gpio_init_structE.Pull = GPIO_PULLUP;                            /* ���� */
        gpio_init_structE.Speed = GPIO_SPEED_FREQ_HIGH;                  /* ���� */
        gpio_init_structE.Alternate = GTIM_TIM9_PWM_CHY_GPIO_AF;         /* IO��REMAP����, �Ƿ��Ҫ�鿴ͷ�ļ����õ�˵��! */
        HAL_GPIO_Init(GTIM_TIM9_PWM_CHY_GPIO_PORTE, &gpio_init_structE);
		
    }

    if (htim->Instance == GTIM_TIM1_PWM)
    {
        // �Ƹ�1
        GPIO_InitTypeDef gpio_init_structA;
		GTIM_TIM1_PWM_CHY_GPIOA_CLK_ENABLE();                            /* ����ͨ��y��GPIOʱ�� */
        GTIM_TIM1_PWM_CHY_CLK_ENABLE();                                 /* ʹ�ܶ�ʱ��ʱ�� */
        gpio_init_structA.Pin = GTIM_TIM1_PWM_CHY_GPIO_PIN8 | GTIM_TIM1_PWM_CHY_GPIO_PIN9;              /* ͨ��1 2��CPIO�� */
        gpio_init_structA.Mode = GPIO_MODE_AF_PP;                        /* ����������� */
        gpio_init_structA.Pull = GPIO_PULLUP;                            /* ���� */
        gpio_init_structA.Speed = GPIO_SPEED_FREQ_HIGH;                  /* ���� */
        gpio_init_structA.Alternate = GTIM_TIM1_PWM_CHY_GPIO_AF;         /* IO��REMAP����, �Ƿ��Ҫ�鿴ͷ�ļ����õ�˵��! */
        HAL_GPIO_Init(GTIM_TIM1_PWM_CHY_GPIO_PORTA, &gpio_init_structA);
		
    }

    if (htim->Instance == GTIM_TIM8_PWM)
    {
        // �Ƹ�2&3
        GPIO_InitTypeDef gpio_init_structC;
		GTIM_TIM8_PWM_CHY_GPIOC_CLK_ENABLE();                            /* ����ͨ��y��GPIOʱ�� */
        GTIM_TIM8_PWM_CHY_CLK_ENABLE();                                 /* ʹ�ܶ�ʱ��ʱ�� */
        gpio_init_structC.Pin = GTIM_TIM8_PWM_CHY_GPIO_PIN8 | GTIM_TIM8_PWM_CHY_GPIO_PIN9|GTIM_TIM8_PWM_CHY_GPIO_PIN6|GTIM_TIM8_PWM_CHY_GPIO_PIN7;              /* ͨ��1 2��CPIO�� */
        gpio_init_structC.Mode = GPIO_MODE_AF_PP;                        /* ����������� */
        gpio_init_structC.Pull = GPIO_PULLUP;                            /* ���� */
        gpio_init_structC.Speed = GPIO_SPEED_FREQ_HIGH;                  /* ���� */
        gpio_init_structC.Alternate = GTIM_TIM8_PWM_CHY_GPIO_AF;         /* IO��REMAP����, �Ƿ��Ҫ�鿴ͷ�ļ����õ�˵��! */
        HAL_GPIO_Init(GTIM_TIM8_PWM_CHY_GPIO_PORTC, &gpio_init_structC);
		
    }


    if (htim->Instance == GTIM_TIM4_PWM)
    {
        // �Ƹ�4&5
        GPIO_InitTypeDef gpio_init_structD;
		GTIM_TIM4_PWM_CHY_GPIOD_CLK_ENABLE();                            /* ����ͨ��y��GPIOʱ�� */
        GTIM_TIM4_PWM_CHY_CLK_ENABLE();                                 /* ʹ�ܶ�ʱ��ʱ�� */
        gpio_init_structD.Pin = GTIM_TIM4_PWM_CHY_GPIO_PIN12 | GTIM_TIM4_PWM_CHY_GPIO_PIN13|GTIM_TIM4_PWM_CHY_GPIO_PIN14|GTIM_TIM4_PWM_CHY_GPIO_PIN15;              /* ͨ��1 2��CPIO�� */
        gpio_init_structD.Mode = GPIO_MODE_AF_PP;                        /* ����������� */
        gpio_init_structD.Pull = GPIO_PULLUP;                            /* ���� */
        gpio_init_structD.Speed = GPIO_SPEED_FREQ_HIGH;                  /* ���� */
        gpio_init_structD.Alternate = GTIM_TIM4_PWM_CHY_GPIO_AF;         /* IO��REMAP����, �Ƿ��Ҫ�鿴ͷ�ļ����õ�˵��! */
        HAL_GPIO_Init(GTIM_TIM4_PWM_CHY_GPIO_PORTD, &gpio_init_structD);
		
    }

    if (htim->Instance == GTIM_TIM12_PWM)
    {
        // �Ƹ�6
        GPIO_InitTypeDef gpio_init_structB;
		GTIM_TIM12_PWM_CHY_GPIOB_CLK_ENABLE();                            /* ����ͨ��y��GPIOʱ�� */
        GTIM_TIM12_PWM_CHY_CLK_ENABLE();                                 /* ʹ�ܶ�ʱ��ʱ�� */
        gpio_init_structB.Pin = GTIM_TIM12_PWM_CHY_GPIO_PIN14 | GTIM_TIM12_PWM_CHY_GPIO_PIN15;              /* ͨ��1 2��CPIO�� */
        gpio_init_structB.Mode = GPIO_MODE_AF_PP;                        /* ����������� */
        gpio_init_structB.Pull = GPIO_PULLUP;                            /* ���� */
        gpio_init_structB.Speed = GPIO_SPEED_FREQ_HIGH;                  /* ���� */
        gpio_init_structB.Alternate = GTIM_TIM12_PWM_CHY_GPIO_AF;         /* IO��REMAP����, �Ƿ��Ҫ�鿴ͷ�ļ����õ�˵��! */
        HAL_GPIO_Init(GTIM_TIM12_PWM_CHY_GPIO_PORTB, &gpio_init_structB);
		
    }
    
}



	