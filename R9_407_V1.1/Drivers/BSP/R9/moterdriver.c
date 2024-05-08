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


void MoterdriveInit(void)
{
    MoterL_pwm_chy_init(100 - 1, 42 - 1);    //* 84 000 000 / 100*42      L 20khzƵ�ʵ�PWM ����*  /  
	MoterR_pwm_chy_init(200 - 1, 42 - 1);    //* 168 000 000 / 200*42     R  20khzƵ�ʵ�PWM ����*/
	MoterLift_pwm_chy_init(200 - 1, 42 - 1);    //* 168 000 000 / 200*42     T1  20khzƵ�ʵ�PWM ����*/
	MoterPedestal_Backboard_pwm_chy_init(200 - 1, 42 - 1);//                   T2&T3
	MoterLeg_pwm_chy_init(100 - 1, 42 - 1);               //                   T4&T5
    MoterSupport_pwm_chy_init(100 - 1, 42 - 1);   //* 84 000 000 / 100*42      T6  20khzƵ�ʵ�PWM ����*  /  
}
// �Ƹ˲��Գ���
void linearactuatorTest(void)
{
    // uint16_t T1_IN1 = 0;
    // uint16_t T1_IN2 = 0;
    // uint16_t T2_IN1 = 0;
    // uint16_t T2_IN2 = 0;
    // uint16_t T3_IN1 = 0;
    // uint16_t T3_IN2 = 0;
    // uint16_t T4_IN1 = 0;
    // uint16_t T4_IN2 = 0;
    // uint16_t T5_IN1 = 0;
    // uint16_t T5_IN2 = 0;
    // uint16_t T6_IN1 = 0;
    // uint16_t T6_IN2 = 0;	
    // static float acctemp=0,acct=0;
    // static uint8_t accdoneflage=0;
    // // if (key_scan5() == 1) 
    // if (KeyStateRecive[3] == SEAT_LIFT)
    // {
    //     // ���� ��ش��� 80ms    0.5~0.9 ռ�ձ�  ����
    //     if ((acct<100) && (accdoneflage ==0))
    //     {
    //         acct++;
    //         acctemp = 5.700000000000001e-10 * pow(acct,5)-1.425000000000000e-07*pow(acct,4)+9.500000000000000e-06*pow(acct,3);
    //     }
    //     else
    //     {
    //         accdoneflage = 1;
    //         acct =0;
    //     }
        
    //     T1_IN1 = 200 * (1.0 - 0);
    //     T1_IN2 = 200 * (1.0 -acctemp);	
    //     T2_IN1 = 200 * (1.0 - 0);
    //     T2_IN2 = 200 * (1.0 - 0.7);	
    //     T3_IN1 = 200 * (1.0 - 0);
    //     T3_IN2 = 200 * (1.0 - 0.7);	
    //     T4_IN1 = 100 * (1.0 - 0);
    //     T4_IN2 = 100 * (1.0 - 0.7);	
    //     T5_IN1 = 100 * (1.0 - 0);
    //     T5_IN2 = 100 * (1.0 - 0.7);	
    //     T6_IN1 = 100 * (1.0 - 0);
    //     T6_IN2 = 100 * (1.0 - 0.7);	
    //     __HAL_TIM_SET_COMPARE(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH1, T1_IN1);
    //     __HAL_TIM_SET_COMPARE(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH2, T1_IN2);

    //     __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH3, T2_IN1);
    //     __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH4, T2_IN2);
    //     __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH1, T3_IN1);
    //     __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH2, T3_IN2);

    //     __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH3, T4_IN1);
    //     __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH4, T4_IN2);			
    //     __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH1, T5_IN1);
    //     __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH2, T5_IN2);	

    //     __HAL_TIM_SET_COMPARE(&g_time12_pwm_chy_handle, GTIM_TIM12_PWM_CH1, T6_IN1);
    //     __HAL_TIM_SET_COMPARE(&g_time12_pwm_chy_handle, GTIM_TIM12_PWM_CH2, T6_IN2);

    // }

    // // else if (key_scan1() == 1)  
    // else if (KeyStateRecive[4] == SEAT_DROP )     
    // {
    //     if ((acct<100) && (accdoneflage ==0))
    //     {
    //         acct++;
    //         acctemp = 2.700000000000000e-10 * pow(acct,5)-6.750000000000000e-08*pow(acct,4)+4.500000000000000e-06*pow(acct,3)+0.5;
    //     }
    //     else
    //     {
    //         accdoneflage = 1;
    //         acct =0;
    //     } 
    //     T1_IN2 = 200 * (1.0 - 0);
    //     T1_IN1 = 200 * (1.0 - acctemp);	
    //     T2_IN2 = 200 * (1.0 - 0);
    //     T2_IN1 = 200 * (1.0 - 0.7);	
    //     T3_IN2 = 200 * (1.0 - 0);
    //     T3_IN1 = 200 * (1.0 - 0.7);	
    //     T4_IN2 = 100 * (1.0 - 0);
    //     T4_IN1 = 100 * (1.0 - 0.7);	
    //     T5_IN2 = 100 * (1.0 - 0);
    //     T5_IN1 = 100 * (1.0 - 0.7);	
    //     T6_IN2 = 100 * (1.0 - 0);
    //     T6_IN1 = 100 * (1.0 - 0.7);		
    //     __HAL_TIM_SET_COMPARE(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH1, T1_IN1);
    //     __HAL_TIM_SET_COMPARE(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH2, T1_IN2);

    //     __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH3, T2_IN1);
    //     __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH4, T2_IN2);
    //     __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH1, T3_IN1);
    //     __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH2, T3_IN2);

    //     __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH3, T4_IN1);
    //     __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH4, T4_IN2);			
    //     __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH1, T5_IN1);
    //     __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH2, T5_IN2);	

    //     __HAL_TIM_SET_COMPARE(&g_time12_pwm_chy_handle, GTIM_TIM12_PWM_CH1, T6_IN1);
    //     __HAL_TIM_SET_COMPARE(&g_time12_pwm_chy_handle, GTIM_TIM12_PWM_CH2, T6_IN2);
    // }	
    // // else if (key_scan1() == 0 && key_scan5() == 0)
    // else if (KeyStateRecive[3]==0 && KeyStateRecive[4]==0)
    // {
    //     acct = 0;
    //     accdoneflage = 0;
    //     acctemp = 0;
    //     T1_IN1 = 200 * (1.0 - 0); // �����Ƹ�
    //     T1_IN2 = 200 * (1.0 - 0);	
    //     T2_IN1 = 200 * (1.0 - 0); // ����
    //     T2_IN2 = 200 * (1.0 - 0);	
    //     T3_IN1 = 200 * (1.0 - 0); //����
    //     T3_IN2 = 200 * (1.0 - 0);	
    //     T4_IN1 = 100 * (1.0 - 0); // ��̤��ת
    //     T4_IN2 = 100 * (1.0 - 0);	
    //     T5_IN1 = 100 * (1.0 - 0); // ��̤�쳤
    //     T5_IN2 = 100 * (1.0 - 0);	
    //     T6_IN1 = 100 * (1.0 - 0); // ǰ֧����
    //     T6_IN2 = 100 * (1.0 - 0);	   
    //     __HAL_TIM_SET_COMPARE(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH1, T1_IN1);
    //     __HAL_TIM_SET_COMPARE(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH2, T1_IN2);

    //     __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH3, T2_IN1);
    //     __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH4, T2_IN2);
    //     __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH1, T3_IN1);
    //     __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH2, T3_IN2);

    //     __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH3, T4_IN1);
    //     __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH4, T4_IN2);			
    //     __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH1, T5_IN1);
    //     __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH2, T5_IN2);	

    //     __HAL_TIM_SET_COMPARE(&g_time12_pwm_chy_handle, GTIM_TIM12_PWM_CH1, T6_IN1);
    //     __HAL_TIM_SET_COMPARE(&g_time12_pwm_chy_handle, GTIM_TIM12_PWM_CH2, T6_IN2);

    // }
    // printf("acctemp:%d\t\n",T1_IN1);
    SeatLiftDrop();
}

 /*���ξ�������*/
 void SeatLiftDrop(void)
 {
    uint16_t T1_IN1 = 0;
    uint16_t T1_IN2 = 0;
    uint16_t T2_IN1 = 0;
    uint16_t T2_IN2 = 0;
    static float acctemp=0,acct=0;
    static uint8_t accdoneflage=0;

    if (KeyStateRecive[3] == SEAT_LIFT) //|| Reg[99] ==1  )
    {
      // �������� ��ش���
        if ((acct<100) && (accdoneflage ==0))
        {
            acct++;
            acctemp = 2.700000000000000e-10 * pow(acct,5)-6.750000000000000e-08*pow(acct,4)+4.500000000000000e-06*pow(acct,3)+0.5;
        }
        else
        {
            accdoneflage = 1;
            acct =0;
        }
        T1_IN1 = 200 * (1.0 - 0);
        T1_IN2 = 200 * (1.0 -acctemp);	
        T2_IN1 = 200 * (1.0 - 0);
        T2_IN2 = 200 * (1.0 - SEAT_LIFTDROPRATIO*acctemp);	
    }
    else if (KeyStateRecive[4] == SEAT_DROP )//|| Reg[99] ==2  )
    {
        // �½����� ��ش���

        if ((acct<100) && (accdoneflage ==0))
        {
            acct++;
             acctemp = 2.700000000000000e-10 * pow(acct,5)-6.750000000000000e-08*pow(acct,4)+4.500000000000000e-06*pow(acct,3)+0.5;
        }
        else
        {
            accdoneflage = 1;
            acct =0;
        }
        T1_IN1 = 200 * (1.0 - acctemp);
        T1_IN2 = 200 * (1.0 -0);	
        T2_IN1 = 200 * (1.0 - SEAT_LIFTDROPRATIO*acctemp);
        T2_IN2 = 200 * (1.0 - 0);	


    }
    else
    {
        acct = 0;
        accdoneflage = 0;
        acctemp = 0;
        T1_IN1 = 200 * (1.0 - 0);
        T1_IN2 = 200 * (1.0 -0);	
        T2_IN1 = 200 * (1.0 - 0);
        T2_IN2 = 200 * (1.0 - 0);	
    }
    __HAL_TIM_SET_COMPARE(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH1, T1_IN1);
    __HAL_TIM_SET_COMPARE(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH2, T1_IN2);

    __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH3, T2_IN1);
    __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH4, T2_IN2);

 }

 /*���ο�������*/	
 void BackresetFB(void)
 {
    uint16_t T3_IN1 = 0;
    uint16_t T3_IN2 = 0;
    static float acctemp=0,acct=0;
    static uint8_t accdoneflage=0;
    //����ǰ��
    if (KeyStateRecive[5] == BACKREST_FORWARD )// || Reg[100] ==1 )
    {
        if ((acct<50) && (accdoneflage ==0))
        {
            acct++;
            acctemp = 8.640000000000000e-09 * pow(acct,5)-1.080000000000000e-06*pow(acct,4)+3.600000000000000e-05*pow(acct,3)+0.5;
        }
        else
        {
            accdoneflage = 1;
            acct =0;
        }
        T3_IN2 = 200 * (1.0 - 0);
        T3_IN1 = 200 * (1.0 - acctemp);	


    }
    //��������
    else if (KeyStateRecive[6] == BACKREST_BACK )// || Reg[100] ==2 )
    {
        if ((acct<50) && (accdoneflage ==0))
        {
            acct++;
            acctemp = 8.640000000000000e-09 * pow(acct,5)-1.080000000000000e-06*pow(acct,4)+3.600000000000000e-05*pow(acct,3)+0.5;
        }
        else
        {
            accdoneflage = 1;
            acct =0;
        }
        T3_IN2 = 200 * (1.0 - acctemp);
        T3_IN1 = 200 * (1.0 - 0);	

    }
    //����ǰ����STOP
    else
    {
        acct = 0;
        accdoneflage = 0;
        acctemp = 0;  
        T3_IN2 = 0;
        T3_IN1 = 0;	      
    }
     __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH1, T3_IN1);
    __HAL_TIM_SET_COMPARE(&g_time8_pwm_chy_handle, GTIM_TIM8_PWM_CH2, T3_IN2);

 }

 /*�������*/	
void ThwartFB(void)
{
    uint16_t T2_IN1 = 0;
    uint16_t T2_IN2 = 0;
    static float acctemp=0,acct=0;
    static uint8_t accdoneflage=0;

    //����ǰ��
    if (0)//Reg[104] ==1 )
    {
        if ((acct<50) && (accdoneflage ==0))
        {
            acct++;
            acctemp = 8.640000000000000e-09 * pow(acct,5)-1.080000000000000e-06*pow(acct,4)+3.600000000000000e-05*pow(acct,3)+0.5;
        }
        else
        {
            accdoneflage = 1;
            acct =0;
        }
        T2_IN1 = 200 * (1.0 - 0);
        T2_IN2 = 200 * (1.0 - acctemp);	


    }
    //�������
    else if (0)//( Reg[104] ==2 )
    {
        if ((acct<50) && (accdoneflage ==0))
        {
            acct++;
            acctemp = 8.640000000000000e-09 * pow(acct,5)-1.080000000000000e-06*pow(acct,4)+3.600000000000000e-05*pow(acct,3)+0.5;
        }
        else
        {
            accdoneflage = 1;
            acct =0;
        }
        T2_IN1 = 200 * (1.0 - acctemp);
        T2_IN2 = 200 * (1.0 - 0);	

    }
    //����ǰ����STOP
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
/*����ǰ����*/
void OverallFB(void)
{
    uint16_t T1_IN1 = 0;
    uint16_t T1_IN2 = 0;
    static float acctemp=0,acct=0;
    static uint8_t accdoneflage=0;
    // ����ǰ��
    if (KeyStateRecive[7] == ALL_FORWARD)// || Reg[101] == 1 )
    {
       if ((acct<50) && (accdoneflage ==0))
        {
            acct++;
            acctemp = 8.640000000000000e-09 * pow(acct,5)-1.080000000000000e-06*pow(acct,4)+3.600000000000000e-05*pow(acct,3)+0.5;
        }
        else
        {
            accdoneflage = 1;
            acct =0;
        }
        T1_IN1 = 200 * (1.0 - 0);
        T1_IN2 = 200 * (1.0 - acctemp);	
    }

    // �������
    else if (KeyStateRecive[8] ==ALL_BACK)// || Reg[101] == 2  )
    {

        if ((acct<50) && (accdoneflage ==0))
        {
            acct++;
            acctemp = 8.640000000000000e-09 * pow(acct,5)-1.080000000000000e-06*pow(acct,4)+3.600000000000000e-05*pow(acct,3)+0.5;
        }
        else
        {
            accdoneflage = 1;
            acct =0;
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
    __HAL_TIM_SET_COMPARE(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH1, T1_IN1);
    __HAL_TIM_SET_COMPARE(&g_time1_pwm_chy_handle, GTIM_TIM1_PWM_CH2, T1_IN2);
}

/*����������ת����*/
void LegSpinFB(void)
{
    uint16_t T4_IN1 = 0;
    uint16_t T4_IN2 = 0;
    uint16_t T5_IN1 = 0;
    uint16_t T5_IN2 = 0;
    static float acctemp=0,acct=0;
    static uint8_t accdoneflage=0;

    if (KeyStateRecive[9] == LEG_TOPSPIN)// || Reg[102] ==1  )
    {
      // ��������
        if ((acct<50) && (accdoneflage ==0))
        {
            acct++;
            acctemp = 8.640000000000000e-09 * pow(acct,5)-1.080000000000000e-06*pow(acct,4)+3.600000000000000e-05*pow(acct,3)+0.5;
        }
        else
        {
            accdoneflage = 1;
            acct =0;
        }

        T4_IN1 = 100 * (1.0 - 0);
        T4_IN2 = 100 * (1.0 - acctemp);	
        T5_IN1 = 100 * (1.0 - 0);
        T5_IN2 = 100 * (1.0 - acctemp);	
    }
    else if (KeyStateRecive[10] == LEG_BACKSPIN)// ||Reg[102] ==2  )
    {
        // ��������

        if ((acct<50) && (accdoneflage ==0))
        {
            acct++;
            acctemp = 8.640000000000000e-09 * pow(acct,5)-1.080000000000000e-06*pow(acct,4)+3.600000000000000e-05*pow(acct,3)+0.5;
        }
        else
        {
            accdoneflage = 1;
            acct =0;
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
        T4_IN2 = 100 * (1.0 -0);	
        T5_IN1 = 100 * (1.0 - 0);
        T5_IN2 = 100 * (1.0 - 0);	
    }
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH3, T4_IN1);
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH4, T4_IN2);			
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH1, T5_IN1);
        __HAL_TIM_SET_COMPARE(&g_time4_pwm_chy_handle, GTIM_TIM4_PWM_CH2, T5_IN2);	

}



void OverallStandSit()
{
    static float acctemp=0,acct=0;
    static uint8_t accdoneflage=0;
    NONE;
}