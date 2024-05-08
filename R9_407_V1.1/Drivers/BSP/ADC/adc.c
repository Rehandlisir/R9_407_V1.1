/**
 ****************************************************************************************************
 * @file        adc.c
 * @author    lis
 * @version     V1.2
 * @date        2021-10-18
 * @brief       ADC ��������
 * @license     ����ҽ��
 * note ��
 * ��� R9ϵͳ������ADC ���ݲɼ� ��
 *  һ ��ADC1 �ɼ�10ͨ������ ����
 * (1)  ҡ�����ݲɼ�         PA2 PA3
 * (2)  ��բ ���ݼ��        PA4 PA5
 * (3)  ���̵���������     PA6 PA7
 * (4)  ��ص�ѹ            PC5
 *
 * ����   ADC3 ���ݲɼ�  ����
 * (1) �Ƹ�1~6  λ�ü��  PF5 PF3 PF4 PF6 PF8 PF7
 * (2) �Ƹ� 1~6 �ĵ������  PC2 PC3 PC0 PC1 PF9 PF10
 */
#include "./SYSTEM/delay/delay.h"
#include "./BSP/ADC/adc.h"

uint8_t g_adc1_dma_sta = 0; /* ADC1 DMA2����״̬��־, 0,δ���; 1, ����� */
uint8_t g_adc3_dma_sta = 0; /* ADC3 DMA2����״̬��־, 0,δ���; 1, ����� */

ADC_HandleTypeDef g_adc1_nch_dma_handle; /* ��DMA������ADC��� */
DMA_HandleTypeDef g_dma_nch_adc1_handle; /* ��ADC������DMA��� */

ADC_HandleTypeDef g_adc3_nch_dma_handle; /* ��DMA2������ADC��� */
DMA_HandleTypeDef g_dma_nch_adc3_handle; /* ��ADC3������DMA��� */

void adc_channel_set(ADC_HandleTypeDef *adc_handle, uint32_t ch, uint32_t rank, uint32_t stime)
{
    /* ���ö�ӦADCͨ�� */
    ADC_ChannelConfTypeDef adc_channel;
    adc_channel.Channel = ch;                        /* ����ADCX��ͨ��ch */
    adc_channel.Rank = rank;                         /* ���ò������� */
    adc_channel.SamplingTime = stime;                /* ���ò���ʱ�� */
    HAL_ADC_ConfigChannel(adc_handle, &adc_channel); /* ��ʼ��ADCͨ�� */
}

/***************************************��ͨ��ADC1 �ɼ�(DMA��ȡ)����*****************************************/
void adc1_nch_dma_init(uint32_t mar)
{
    /* ADC1-DMA2stream4���� */
    ADC_ADC1_CHY_CLK_ENABLE();                     /* ʹ��ADC1ʱ�� */
    if ((uint32_t)ADC_ADC1_DMASx > (uint32_t)DMA2) /* ����DMA1_Stream7, ��ΪDMA2 */
    {
        __HAL_RCC_DMA2_CLK_ENABLE(); /* DMA2ʱ��ʹ�� */
    }
    else
    {
        __HAL_RCC_DMA1_CLK_ENABLE(); /*   */
    }

    g_dma_nch_adc1_handle.Instance = ADC_ADC1_DMASx;                          /* ����DMA������ */
    g_dma_nch_adc1_handle.Init.Channel = DMA_CHANNEL_0;                       /* ����DMAͨ�� */
    g_dma_nch_adc1_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;              /* DIR = 1 , ���赽�洢��ģʽ */
    g_dma_nch_adc1_handle.Init.PeriphInc = DMA_PINC_DISABLE;                  /* ���������ģʽ */
    g_dma_nch_adc1_handle.Init.MemInc = DMA_MINC_ENABLE;                      /* �洢������ģʽ */
    g_dma_nch_adc1_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; /* �������ݳ���:16λ */
    g_dma_nch_adc1_handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;    /* �洢�����ݳ���:16λ */
    g_dma_nch_adc1_handle.Init.Mode = DMA_NORMAL;                             /* ��������ģʽ */
    g_dma_nch_adc1_handle.Init.Priority = DMA_PRIORITY_MEDIUM;                /* �е����ȼ� */
    HAL_DMA_Init(&g_dma_nch_adc1_handle);                                     /* ��ʼ��DMA */
    HAL_DMA_Start(&g_dma_nch_adc1_handle, (uint32_t)&ADC_ADC1->DR, mar, 0);   /* ����DMA������� */

    g_adc1_nch_dma_handle.DMA_Handle = &g_dma_nch_adc1_handle; /* ����ADC��Ӧ��DMA */
    g_adc1_nch_dma_handle.Instance = ADC_ADC1;
    g_adc1_nch_dma_handle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;            /* 4��Ƶ��ADCCLK = PCLK2/4 = 84/4 = 21Mhz */
    g_adc1_nch_dma_handle.Init.Resolution = ADC_RESOLUTION_12B;                      /* 12λģʽ */
    g_adc1_nch_dma_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;                      /* �Ҷ��� */
    g_adc1_nch_dma_handle.Init.ScanConvMode = ENABLE;                                /* ɨ��ģʽ */
    g_adc1_nch_dma_handle.Init.ContinuousConvMode = ENABLE;                          /* ����ת��ģʽ��ת�����֮����ż���ת�� */
    g_adc1_nch_dma_handle.Init.DiscontinuousConvMode = DISABLE;                      /* ��ֹ����������ģʽ */
    g_adc1_nch_dma_handle.Init.NbrOfConversion = ADC1_CH_NUM;                        /* ʹ��ת��ͨ�����������ʵ��ת��ͨ��ȥ���� */
    g_adc1_nch_dma_handle.Init.NbrOfDiscConversion = 0;                              /* ����������ͨ����Ϊ0 */
    g_adc1_nch_dma_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;                /* ������� */
    g_adc1_nch_dma_handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE; /* ʹ���������, ��λ���� */
    g_adc1_nch_dma_handle.Init.DMAContinuousRequests = ENABLE;                       /* ����DMA����ת�� */
    HAL_ADC_Init(&g_adc1_nch_dma_handle);                                            /* ��ʼ��ADC */

    adc1_nch_dma_gpio_init(); /* GPIO ��ʼ�� */
    /*ҡ�� ��բ ���̵�����ݲɼ�*/
    adc_channel_set(&g_adc1_nch_dma_handle, ADC_CHANNEL_2, 1, ADC_SAMPLETIME_480CYCLES); /* ���ò�����������1~6 */
    adc_channel_set(&g_adc1_nch_dma_handle, ADC_CHANNEL_3, 2, ADC_SAMPLETIME_480CYCLES);
    adc_channel_set(&g_adc1_nch_dma_handle, ADC_CHANNEL_4, 3, ADC_SAMPLETIME_480CYCLES);
    adc_channel_set(&g_adc1_nch_dma_handle, ADC_CHANNEL_5, 4, ADC_SAMPLETIME_480CYCLES);
    adc_channel_set(&g_adc1_nch_dma_handle, ADC_CHANNEL_6, 5, ADC_SAMPLETIME_480CYCLES);
    adc_channel_set(&g_adc1_nch_dma_handle, ADC_CHANNEL_7, 6, ADC_SAMPLETIME_480CYCLES);
    adc_channel_set(&g_adc1_nch_dma_handle, ADC_CHANNEL_14, 7, ADC_SAMPLETIME_480CYCLES);

    HAL_NVIC_SetPriority(ADC_ADC1_DMASx_IRQn, 2, 1);                   /* ����DMA�ж����ȼ�Ϊ3�������ȼ�Ϊ3 */
    HAL_NVIC_EnableIRQ(ADC_ADC1_DMASx_IRQn);                           /* ʹ��DMA�ж� */
    HAL_ADC_Start_DMA(&g_adc1_nch_dma_handle, &mar, sizeof(uint16_t)); /* ��ʼDMA���ݴ��� */
    __HAL_DMA_ENABLE_IT(&g_dma_nch_adc1_handle, DMA_IT_TC);            /* TCIE=1, ʹ�ܴ�������ж� */
}

/**
 * @brief       ��ͨ��ADC��gpio��ʼ������
 * @param       ��
 * @note        �˺����ᱻadc_nch_dma_init()����
 * @note        PA0-ADC_CHANNEL_0��PA1-ADC_CHANNEL_1��PA2-ADC_CHANNEL_2
                PA3-ADC_CHANNEL_3��PA4-ADC_CHANNEL_4��PA5-ADC_CHANNEL_5
* @retval       ��
 */

void adc1_nch_dma_gpio_init(void)
{
    GPIO_InitTypeDef gpioA_init_struct;
    __HAL_RCC_GPIOA_CLK_ENABLE(); /* ����GPIOA����ʱ�� */
    /* AD�ɼ�����ģʽ����,ģ������ */
    gpioA_init_struct.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7; /* GPIOA2~7 */
    gpioA_init_struct.Mode = GPIO_MODE_ANALOG;
    gpioA_init_struct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &gpioA_init_struct);

    GPIO_InitTypeDef gpioC_init_struct;
    __HAL_RCC_GPIOC_CLK_ENABLE(); /* ����GPIOC����ʱ�� */
    /* AD�ɼ�����ģʽ����,ģ������ */
    gpioC_init_struct.Pin = GPIO_PIN_4; /* GPIOA2~7 */
    gpioC_init_struct.Mode = GPIO_MODE_ANALOG;
    gpioC_init_struct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &gpioC_init_struct);
}

/**
 * @brief       ʹ��һ��ADC DMA����
 * @param       ndtr: DMA����Ĵ���
 * @retval      ��
 */
void adc1_nch_dma_enable(uint16_t ndtr)
{
    __HAL_ADC_DISABLE(&g_adc1_nch_dma_handle); /* �ȹر�ADC */

    __HAL_DMA_DISABLE(&g_dma_nch_adc1_handle);   /* �ر�DMA���� */
    g_dma_nch_adc1_handle.Instance->NDTR = ndtr; /* ����DMA���������� */
    __HAL_DMA_ENABLE(&g_dma_nch_adc1_handle);    /* ����DMA���� */

    __HAL_ADC_ENABLE(&g_adc1_nch_dma_handle); /* ��������ADC */
    ADC_ADC1->CR2 |= 1 << 30;                 /* ��������ת��ͨ�� */
}

/**
 * @brief       ADC DMA�ɼ��жϷ�����
 * @param       ��
 * @retval      ��
 */
void ADC_ADC1_DMASx_IRQHandler(void)
{
    if (ADC_ADC1_DMASx_IS_TC())
    {
        g_adc1_dma_sta = 1;      /* ���DMA������� */
        ADC_ADC1_DMASx_CLR_TC(); /* ���DMA2 ������4 ��������ж� */
    }
}

/***************************************��ͨ��ADC3 �ɼ�(DMA��ȡ)����*****************************************/

void adc3_nch_dma_init(uint32_t mar)
{
    /* ADC3-DMA2stream1���� */
    ADC_ADC3_CHY_CLK_ENABLE();                     /* ʹ��ADC1ʱ�� */
    if ((uint32_t)ADC_ADC3_DMASx > (uint32_t)DMA2) /* ����DMA2_Stream7, ��ΪDMA2 */
    {
        __HAL_RCC_DMA2_CLK_ENABLE(); /* DMA2ʱ��ʹ�� */
    }
    else
    {
        __HAL_RCC_DMA1_CLK_ENABLE(); /*   */
    }

    g_dma_nch_adc3_handle.Instance = ADC_ADC3_DMASx;                          /* ����DMA������ */
    g_dma_nch_adc3_handle.Init.Channel = DMA_CHANNEL_2;                       /* ����DMAͨ�� */
    g_dma_nch_adc3_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;              /* DIR = 1 , ���赽�洢��ģʽ */
    g_dma_nch_adc3_handle.Init.PeriphInc = DMA_PINC_DISABLE;                  /* ���������ģʽ */
    g_dma_nch_adc3_handle.Init.MemInc = DMA_MINC_ENABLE;                      /* �洢������ģʽ */
    g_dma_nch_adc3_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; /* �������ݳ���:16λ */
    g_dma_nch_adc3_handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;    /* �洢�����ݳ���:16λ */
    g_dma_nch_adc3_handle.Init.Mode = DMA_NORMAL;                             /* ��������ģʽ */
    g_dma_nch_adc3_handle.Init.Priority = DMA_PRIORITY_MEDIUM;                /* �е����ȼ� */
    HAL_DMA_Init(&g_dma_nch_adc3_handle);                                     /* ��ʼ��DMA */
    HAL_DMA_Start(&g_dma_nch_adc3_handle, (uint32_t)&ADC_ADC3->DR, mar, 0);   /* ����DMA������� */

    g_adc3_nch_dma_handle.DMA_Handle = &g_dma_nch_adc3_handle; /* ����ADC��Ӧ��DMA */
    g_adc3_nch_dma_handle.Instance = ADC_ADC3;
    g_adc3_nch_dma_handle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;            /* 4��Ƶ��ADCCLK = PCLK2/4 = 84/4 = 21Mhz */
    g_adc3_nch_dma_handle.Init.Resolution = ADC_RESOLUTION_12B;                      /* 12λģʽ */
    g_adc3_nch_dma_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;                      /* �Ҷ��� */
    g_adc3_nch_dma_handle.Init.ScanConvMode = ENABLE;                                /* ɨ��ģʽ */
    g_adc3_nch_dma_handle.Init.ContinuousConvMode = ENABLE;                          /* ����ת��ģʽ��ת�����֮����ż���ת�� */
    g_adc3_nch_dma_handle.Init.DiscontinuousConvMode = DISABLE;                      /* ��ֹ����������ģʽ */
    g_adc3_nch_dma_handle.Init.NbrOfConversion = ADC3_CH_NUM;                        /* ʹ��ת��ͨ�����������ʵ��ת��ͨ��ȥ���� */
    g_adc3_nch_dma_handle.Init.NbrOfDiscConversion = 0;                              /* ����������ͨ����Ϊ0 */
    g_adc3_nch_dma_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;                /* ������� */
    g_adc3_nch_dma_handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE; /* ʹ���������, ��λ���� */
    g_adc3_nch_dma_handle.Init.DMAContinuousRequests = ENABLE;                       /* ����DMA����ת�� */
    HAL_ADC_Init(&g_adc3_nch_dma_handle);                                            /* ��ʼ��ADC */

    adc3_nch_dma_gpio_init();                                                             /* GPIO ��ʼ�� */
                                                                                          /* �Ƹ�1~6 λ����Ϣ*/
    adc_channel_set(&g_adc3_nch_dma_handle, ADC_CHANNEL_15, 1, ADC_SAMPLETIME_480CYCLES); /* ���ò�����������1~12 */
    adc_channel_set(&g_adc3_nch_dma_handle, ADC_CHANNEL_9, 2, ADC_SAMPLETIME_480CYCLES);
    adc_channel_set(&g_adc3_nch_dma_handle, ADC_CHANNEL_14, 3, ADC_SAMPLETIME_480CYCLES);
    adc_channel_set(&g_adc3_nch_dma_handle, ADC_CHANNEL_4, 4, ADC_SAMPLETIME_480CYCLES);
    adc_channel_set(&g_adc3_nch_dma_handle, ADC_CHANNEL_6, 5, ADC_SAMPLETIME_480CYCLES);
    adc_channel_set(&g_adc3_nch_dma_handle, ADC_CHANNEL_5, 6, ADC_SAMPLETIME_480CYCLES);
    /*�Ƹ�1~6 ������Ϣ*/
    adc_channel_set(&g_adc3_nch_dma_handle, ADC_CHANNEL_12, 7, ADC_SAMPLETIME_480CYCLES); /* ���ò�����������1~12 */
    adc_channel_set(&g_adc3_nch_dma_handle, ADC_CHANNEL_13, 8, ADC_SAMPLETIME_480CYCLES);
    adc_channel_set(&g_adc3_nch_dma_handle, ADC_CHANNEL_10, 9, ADC_SAMPLETIME_480CYCLES);
    adc_channel_set(&g_adc3_nch_dma_handle, ADC_CHANNEL_11, 10, ADC_SAMPLETIME_480CYCLES);
    adc_channel_set(&g_adc3_nch_dma_handle, ADC_CHANNEL_7, 11, ADC_SAMPLETIME_480CYCLES);
    adc_channel_set(&g_adc3_nch_dma_handle, ADC_CHANNEL_8, 12, ADC_SAMPLETIME_480CYCLES);

    HAL_NVIC_SetPriority(ADC_ADC3_DMASx_IRQn, 2, 2);                   /* ����DMA�ж����ȼ�Ϊ3�������ȼ�Ϊ3 */
    HAL_NVIC_EnableIRQ(ADC_ADC3_DMASx_IRQn);                           /* ʹ��DMA�ж� */
    HAL_ADC_Start_DMA(&g_adc3_nch_dma_handle, &mar, sizeof(uint16_t)); /* ��ʼDMA���ݴ��� */
    __HAL_DMA_ENABLE_IT(&g_dma_nch_adc3_handle, DMA_IT_TC);            /* TCIE=1, ʹ�ܴ�������ж� */
}

/**
 * @brief       ��ͨ��ADC��gpio��ʼ������
 * @param       ��
 * @note        �˺����ᱻadc_nch_dma_init()����
 * @note        PA0-ADC_CHANNEL_0��PA1-ADC_CHANNEL_1��PA2-ADC_CHANNEL_2
                PA3-ADC_CHANNEL_3��PA4-ADC_CHANNEL_4��PA5-ADC_CHANNEL_5
* @retval       ��
 */

void adc3_nch_dma_gpio_init(void)
{
    GPIO_InitTypeDef gpioF_init_struct;
    __HAL_RCC_GPIOF_CLK_ENABLE(); /* ����GPIOA����ʱ�� */
    /* AD�ɼ�����ģʽ����,ģ������ */
    gpioF_init_struct.Pin = GPIO_PIN_5 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_8 | GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_10; /* GPIOA2~7 */
    gpioF_init_struct.Mode = GPIO_MODE_ANALOG;
    gpioF_init_struct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOF, &gpioF_init_struct);

    GPIO_InitTypeDef gpioC_init_struct;
    __HAL_RCC_GPIOC_CLK_ENABLE(); /* ����GPIOC����ʱ�� */
    /* AD�ɼ�����ģʽ����,ģ������ */
    gpioC_init_struct.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_0 | GPIO_PIN_1; /* GPIOC0~3 */
    gpioC_init_struct.Mode = GPIO_MODE_ANALOG;
    gpioC_init_struct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &gpioC_init_struct);
}

/**
 * @brief       ʹ��һ��ADC DMA����
 * @param       ndtr: DMA����Ĵ���
 * @retval      ��
 */
void adc3_nch_dma_enable(uint16_t ndtr)
{  
    __HAL_ADC_DISABLE(&g_adc3_nch_dma_handle); /* �ȹر�ADC */

    __HAL_DMA_DISABLE(&g_dma_nch_adc3_handle);   /* �ر�DMA���� */
    g_dma_nch_adc3_handle.Instance->NDTR = ndtr; /* ����DMA���������� */
    __HAL_DMA_ENABLE(&g_dma_nch_adc3_handle);    /* ����DMA���� */

    __HAL_ADC_ENABLE(&g_adc3_nch_dma_handle); /* ��������ADC */
    ADC_ADC3->CR2 |= 1 << 30;                 /* ��������ת��ͨ�� */
}

/**
 * @brief       ADC DMA�ɼ��жϷ�����
 * @param       ��
 * @retval      ��
 */
void ADC_ADC3_DMASx_IRQHandler(void)
{
    if (ADC_ADC3_DMASx_IS_TC())
    {
        g_adc3_dma_sta = 1;      /* ���DMA������� */
        ADC_ADC3_DMASx_CLR_TC(); /* ���DMA2 ������4 ��������ж� */
    }
}