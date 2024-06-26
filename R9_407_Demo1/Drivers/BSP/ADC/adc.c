/**
 ****************************************************************************************************
 * @file        adc.c
 * @author    lis
 * @version     V1.2
 * @date        2021-10-18
 * @brief       ADC 驱动代码
 * @license     复成医疗
 * note ：
 * 针对 R9系统的所有ADC 数据采集 ，
 *  一 、ADC1 采集10通道数据 包含
 * (1)  摇杆数据采集         PA2 PA3
 * (2)  抱闸 数据监测        PA4 PA5
 * (3)  底盘电机电流检测     PA6 PA7
 * (4)  电池电压            PC4
 * (5)  充电电流检测接口    PC5
 *
 * 二、   ADC3 数据采集  包含
 * (1) 推杆1~6  位置检测  PF5 PF3 PF4 PF6 PF8 PF7
 * (2) 推杆 1~6 的电流检测  PC2 PC3 PC0 PC1 PF9 PF10
 */
#include "./SYSTEM/delay/delay.h"
#include "./BSP/ADC/adc.h"

uint8_t g_adc1_dma_sta = 0; /* ADC1 DMA2传输状态标志, 0,未完成; 1, 已完成 */
uint8_t g_adc3_dma_sta = 0; /* ADC3 DMA2传输状态标志, 0,未完成; 1, 已完成 */

ADC_HandleTypeDef g_adc1_nch_dma_handle; /* 与DMA关联的ADC句柄 */
DMA_HandleTypeDef g_dma_nch_adc1_handle; /* 与ADC关联的DMA句柄 */

ADC_HandleTypeDef g_adc3_nch_dma_handle; /* 与DMA2关联的ADC句柄 */
DMA_HandleTypeDef g_dma_nch_adc3_handle; /* 与ADC3关联的DMA句柄 */

void adc_channel_set(ADC_HandleTypeDef *adc_handle, uint32_t ch, uint32_t rank, uint32_t stime)
{
    /* 配置对应ADC通道 */
    ADC_ChannelConfTypeDef adc_channel;
    adc_channel.Channel = ch;                        /* 设置ADCX对通道ch */
    adc_channel.Rank = rank;                         /* 设置采样序列 */
    adc_channel.SamplingTime = stime;                /* 设置采样时间 */
    HAL_ADC_ConfigChannel(adc_handle, &adc_channel); /* 初始化ADC通道 */
}

/***************************************多通道ADC1 采集(DMA读取)程序*****************************************/
void adc1_nch_dma_init(uint32_t mar)
{
    /* ADC1-DMA2stream4配置 */
    ADC_ADC1_CHY_CLK_ENABLE();                     /* 使能ADC1时钟 */
    if ((uint32_t)ADC_ADC1_DMASx > (uint32_t)DMA2) /* 大于DMA1_Stream7, 则为DMA2 */
    {
        __HAL_RCC_DMA2_CLK_ENABLE(); /* DMA2时钟使能 */
    }
    else
    {
        __HAL_RCC_DMA1_CLK_ENABLE(); /*   */
    }

    g_dma_nch_adc1_handle.Instance = ADC_ADC1_DMASx;                          /* 设置DMA数据流 */
    g_dma_nch_adc1_handle.Init.Channel = DMA_CHANNEL_0;                       /* 设置DMA通道 */
    g_dma_nch_adc1_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;              /* DIR = 1 , 外设到存储器模式 */
    g_dma_nch_adc1_handle.Init.PeriphInc = DMA_PINC_DISABLE;                  /* 外设非增量模式 */
    g_dma_nch_adc1_handle.Init.MemInc = DMA_MINC_ENABLE;                      /* 存储器增量模式 */
    g_dma_nch_adc1_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; /* 外设数据长度:16位 */
    g_dma_nch_adc1_handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;    /* 存储器数据长度:16位 */
    g_dma_nch_adc1_handle.Init.Mode = DMA_NORMAL;                             /* 外设流控模式 */
    g_dma_nch_adc1_handle.Init.Priority = DMA_PRIORITY_MEDIUM;                /* 中等优先级 */
    HAL_DMA_Init(&g_dma_nch_adc1_handle);                                     /* 初始化DMA */
    HAL_DMA_Start(&g_dma_nch_adc1_handle, (uint32_t)&ADC_ADC1->DR, mar, 0);   /* 配置DMA传输参数 */

    g_adc1_nch_dma_handle.DMA_Handle = &g_dma_nch_adc1_handle; /* 设置ADC对应的DMA */
    g_adc1_nch_dma_handle.Instance = ADC_ADC1;
    g_adc1_nch_dma_handle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;            /* 4分频，ADCCLK = PCLK2/4 = 84/4 = 21Mhz */
    g_adc1_nch_dma_handle.Init.Resolution = ADC_RESOLUTION_12B;                      /* 12位模式 */
    g_adc1_nch_dma_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;                      /* 右对齐 */
    g_adc1_nch_dma_handle.Init.ScanConvMode = ENABLE;                                /* 扫描模式 */
    g_adc1_nch_dma_handle.Init.ContinuousConvMode = ENABLE;                          /* 连续转换模式，转换完成之后接着继续转换 */
    g_adc1_nch_dma_handle.Init.DiscontinuousConvMode = DISABLE;                      /* 禁止不连续采样模式 */
    g_adc1_nch_dma_handle.Init.NbrOfConversion = ADC1_CH_NUM;                        /* 使用转换通道数，需根据实际转换通道去设置 */
    g_adc1_nch_dma_handle.Init.NbrOfDiscConversion = 0;                              /* 不连续采样通道数为0 */
    g_adc1_nch_dma_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;                /* 软件触发 */
    g_adc1_nch_dma_handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE; /* 使用软件触发, 此位忽略 */
    g_adc1_nch_dma_handle.Init.DMAContinuousRequests = ENABLE;                       /* 开启DMA连续转换 */
    HAL_ADC_Init(&g_adc1_nch_dma_handle);                                            /* 初始化ADC */

    adc1_nch_dma_gpio_init(); /* GPIO 初始化 */
    /*摇杆 抱闸 底盘电机数据采集*/
    adc_channel_set(&g_adc1_nch_dma_handle, ADC_CHANNEL_2, 1, ADC_SAMPLETIME_480CYCLES); /* 设置采样规则序列1~6 */
    adc_channel_set(&g_adc1_nch_dma_handle, ADC_CHANNEL_3, 2, ADC_SAMPLETIME_480CYCLES);
    // adc_channel_set(&g_adc1_nch_dma_handle, ADC_CHANNEL_4, 3, ADC_SAMPLETIME_480CYCLES);
    // adc_channel_set(&g_adc1_nch_dma_handle, ADC_CHANNEL_5, 4, ADC_SAMPLETIME_480CYCLES);
    // adc_channel_set(&g_adc1_nch_dma_handle, ADC_CHANNEL_6, 5, ADC_SAMPLETIME_480CYCLES);
    // adc_channel_set(&g_adc1_nch_dma_handle, ADC_CHANNEL_7, 6, ADC_SAMPLETIME_480CYCLES);
    // adc_channel_set(&g_adc1_nch_dma_handle, ADC_CHANNEL_14, 7, ADC_SAMPLETIME_480CYCLES);
	// adc_channel_set(&g_adc1_nch_dma_handle, ADC_CHANNEL_15, 8, ADC_SAMPLETIME_480CYCLES);
	
    HAL_NVIC_SetPriority(ADC_ADC1_DMASx_IRQn, 2, 1);                   /* 设置DMA中断优先级为3，子优先级为3 */
    HAL_NVIC_EnableIRQ(ADC_ADC1_DMASx_IRQn);                           /* 使能DMA中断 */
    HAL_ADC_Start_DMA(&g_adc1_nch_dma_handle, &mar, sizeof(uint16_t)); /* 开始DMA数据传输 */
    __HAL_DMA_ENABLE_IT(&g_dma_nch_adc1_handle, DMA_IT_TC);            /* TCIE=1, 使能传输完成中断 */
}

/**
 * @brief       多通道ADC的gpio初始化函数
 * @param       无
 * @note        此函数会被adc_nch_dma_init()调用
 * @note        PA0-ADC_CHANNEL_0、PA1-ADC_CHANNEL_1、PA2-ADC_CHANNEL_2
                PA3-ADC_CHANNEL_3、PA4-ADC_CHANNEL_4、PA5-ADC_CHANNEL_5
* @retval       无
 */

void adc1_nch_dma_gpio_init(void)
{
    GPIO_InitTypeDef gpioA_init_struct;
    __HAL_RCC_GPIOA_CLK_ENABLE(); /* 开启GPIOA引脚时钟 */
    /* AD采集引脚模式设置,模拟输入 */
    gpioA_init_struct.Pin = GPIO_PIN_2 | GPIO_PIN_3;// | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7; /* GPIOA2~7 */
    gpioA_init_struct.Mode = GPIO_MODE_ANALOG;
    gpioA_init_struct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &gpioA_init_struct);

    // GPIO_InitTypeDef gpioC_init_struct;
    // __HAL_RCC_GPIOC_CLK_ENABLE(); /* 开启GPIOC引脚时钟 */
    // /* AD采集引脚模式设置,模拟输入 */
    // gpioC_init_struct.Pin = GPIO_PIN_4|GPIO_PIN_5; /* GPIOA2~7 */
    // gpioC_init_struct.Mode = GPIO_MODE_ANALOG;
    // gpioC_init_struct.Pull = GPIO_NOPULL;
    // HAL_GPIO_Init(GPIOC, &gpioC_init_struct);
}

/**
 * @brief       使能一次ADC DMA传输
 * @param       ndtr: DMA传输的次数
 * @retval      无
 */
void adc1_nch_dma_enable(uint16_t ndtr)
{
    __HAL_ADC_DISABLE(&g_adc1_nch_dma_handle); /* 先关闭ADC */

    __HAL_DMA_DISABLE(&g_dma_nch_adc1_handle);   /* 关闭DMA传输 */
    g_dma_nch_adc1_handle.Instance->NDTR = ndtr; /* 重设DMA传输数据量 */
    __HAL_DMA_ENABLE(&g_dma_nch_adc1_handle);    /* 开启DMA传输 */

    __HAL_ADC_ENABLE(&g_adc1_nch_dma_handle); /* 重新启动ADC */
    ADC_ADC1->CR2 |= 1 << 30;                 /* 启动规则转换通道 */
}

/**
 * @brief       ADC DMA采集中断服务函数
 * @param       无
 * @retval      无
 */
void ADC_ADC1_DMASx_IRQHandler(void)
{
    if (ADC_ADC1_DMASx_IS_TC())
    {
        g_adc1_dma_sta = 1;      /* 标记DMA传输完成 */
        ADC_ADC1_DMASx_CLR_TC(); /* 清除DMA2 数据流4 传输完成中断 */
    }
}

/***************************************多通道ADC3 采集(DMA读取)程序*****************************************/

void adc3_nch_dma_init(uint32_t mar)
{
    /* ADC3-DMA2stream1配置 */
    ADC_ADC3_CHY_CLK_ENABLE();                     /* 使能ADC1时钟 */
    if ((uint32_t)ADC_ADC3_DMASx > (uint32_t)DMA2) /* 大于DMA2_Stream7, 则为DMA2 */
    {
        __HAL_RCC_DMA2_CLK_ENABLE(); /* DMA2时钟使能 */
    }
    else
    {
        __HAL_RCC_DMA1_CLK_ENABLE(); /*   */
    }

    g_dma_nch_adc3_handle.Instance = ADC_ADC3_DMASx;                          /* 设置DMA数据流 */
    g_dma_nch_adc3_handle.Init.Channel = DMA_CHANNEL_2;                       /* 设置DMA通道 */
    g_dma_nch_adc3_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;              /* DIR = 1 , 外设到存储器模式 */
    g_dma_nch_adc3_handle.Init.PeriphInc = DMA_PINC_DISABLE;                  /* 外设非增量模式 */
    g_dma_nch_adc3_handle.Init.MemInc = DMA_MINC_ENABLE;                      /* 存储器增量模式 */
    g_dma_nch_adc3_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; /* 外设数据长度:16位 */
    g_dma_nch_adc3_handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;    /* 存储器数据长度:16位 */
    g_dma_nch_adc3_handle.Init.Mode = DMA_NORMAL;                             /* 外设流控模式 */
    g_dma_nch_adc3_handle.Init.Priority = DMA_PRIORITY_MEDIUM;                /* 中等优先级 */
    HAL_DMA_Init(&g_dma_nch_adc3_handle);                                     /* 初始化DMA */
    HAL_DMA_Start(&g_dma_nch_adc3_handle, (uint32_t)&ADC_ADC3->DR, mar, 0);   /* 配置DMA传输参数 */

    g_adc3_nch_dma_handle.DMA_Handle = &g_dma_nch_adc3_handle; /* 设置ADC对应的DMA */
    g_adc3_nch_dma_handle.Instance = ADC_ADC3;
    g_adc3_nch_dma_handle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;            /* 4分频，ADCCLK = PCLK2/4 = 84/4 = 21Mhz */
    g_adc3_nch_dma_handle.Init.Resolution = ADC_RESOLUTION_12B;                      /* 12位模式 */
    g_adc3_nch_dma_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;                      /* 右对齐 */
    g_adc3_nch_dma_handle.Init.ScanConvMode = ENABLE;                                /* 扫描模式 */
    g_adc3_nch_dma_handle.Init.ContinuousConvMode = ENABLE;                          /* 连续转换模式，转换完成之后接着继续转换 */
    g_adc3_nch_dma_handle.Init.DiscontinuousConvMode = DISABLE;                      /* 禁止不连续采样模式 */
    g_adc3_nch_dma_handle.Init.NbrOfConversion = ADC3_CH_NUM;                        /* 使用转换通道数，需根据实际转换通道去设置 */
    g_adc3_nch_dma_handle.Init.NbrOfDiscConversion = 0;                              /* 不连续采样通道数为0 */
    g_adc3_nch_dma_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;                /* 软件触发 */
    g_adc3_nch_dma_handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE; /* 使用软件触发, 此位忽略 */
    g_adc3_nch_dma_handle.Init.DMAContinuousRequests = ENABLE;                       /* 开启DMA连续转换 */
    HAL_ADC_Init(&g_adc3_nch_dma_handle);                                            /* 初始化ADC */

    adc3_nch_dma_gpio_init();                                                             /* GPIO 初始化 */
                                                                                          /* 推杆1~6 位置信息*/
    adc_channel_set(&g_adc3_nch_dma_handle, ADC_CHANNEL_15, 1, ADC_SAMPLETIME_480CYCLES); /* 设置采样规则序列1~12 */
    adc_channel_set(&g_adc3_nch_dma_handle, ADC_CHANNEL_9, 2, ADC_SAMPLETIME_480CYCLES);
    adc_channel_set(&g_adc3_nch_dma_handle, ADC_CHANNEL_14, 3, ADC_SAMPLETIME_480CYCLES);
    adc_channel_set(&g_adc3_nch_dma_handle, ADC_CHANNEL_4, 4, ADC_SAMPLETIME_480CYCLES);
    adc_channel_set(&g_adc3_nch_dma_handle, ADC_CHANNEL_6, 5, ADC_SAMPLETIME_480CYCLES);
    adc_channel_set(&g_adc3_nch_dma_handle, ADC_CHANNEL_5, 6, ADC_SAMPLETIME_480CYCLES);
    /*推杆1~6 电流信息*/
    adc_channel_set(&g_adc3_nch_dma_handle, ADC_CHANNEL_12, 7, ADC_SAMPLETIME_480CYCLES); /* 设置采样规则序列1~12 */
    adc_channel_set(&g_adc3_nch_dma_handle, ADC_CHANNEL_13, 8, ADC_SAMPLETIME_480CYCLES);
    adc_channel_set(&g_adc3_nch_dma_handle, ADC_CHANNEL_10, 9, ADC_SAMPLETIME_480CYCLES);
    adc_channel_set(&g_adc3_nch_dma_handle, ADC_CHANNEL_11, 10, ADC_SAMPLETIME_480CYCLES);
    adc_channel_set(&g_adc3_nch_dma_handle, ADC_CHANNEL_7, 11, ADC_SAMPLETIME_480CYCLES);
    adc_channel_set(&g_adc3_nch_dma_handle, ADC_CHANNEL_8, 12, ADC_SAMPLETIME_480CYCLES);

    HAL_NVIC_SetPriority(ADC_ADC3_DMASx_IRQn, 2, 2);                   /* 设置DMA中断优先级为3，子优先级为3 */
    HAL_NVIC_EnableIRQ(ADC_ADC3_DMASx_IRQn);                           /* 使能DMA中断 */
    HAL_ADC_Start_DMA(&g_adc3_nch_dma_handle, &mar, sizeof(uint16_t)); /* 开始DMA数据传输 */
    __HAL_DMA_ENABLE_IT(&g_dma_nch_adc3_handle, DMA_IT_TC);            /* TCIE=1, 使能传输完成中断 */
}

/**
 * @brief       多通道ADC的gpio初始化函数
 * @param       无
 * @note        此函数会被adc_nch_dma_init()调用
 * @note        PA0-ADC_CHANNEL_0、PA1-ADC_CHANNEL_1、PA2-ADC_CHANNEL_2
                PA3-ADC_CHANNEL_3、PA4-ADC_CHANNEL_4、PA5-ADC_CHANNEL_5
* @retval       无
 */

void adc3_nch_dma_gpio_init(void)
{
    GPIO_InitTypeDef gpioF_init_struct;
    __HAL_RCC_GPIOF_CLK_ENABLE(); /* 开启GPIOA引脚时钟 */
    /* AD采集引脚模式设置,模拟输入 */
    gpioF_init_struct.Pin = GPIO_PIN_5 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_8 | GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_10; /* GPIOA2~7 */
    gpioF_init_struct.Mode = GPIO_MODE_ANALOG;
    gpioF_init_struct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOF, &gpioF_init_struct);

    GPIO_InitTypeDef gpioC_init_struct;
    __HAL_RCC_GPIOC_CLK_ENABLE(); /* 开启GPIOC引脚时钟 */
    /* AD采集引脚模式设置,模拟输入 */
    gpioC_init_struct.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_0 | GPIO_PIN_1; /* GPIOC0~3 */
    gpioC_init_struct.Mode = GPIO_MODE_ANALOG;
    gpioC_init_struct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &gpioC_init_struct);
}

/**
 * @brief       使能一次ADC DMA传输
 * @param       ndtr: DMA传输的次数
 * @retval      无
 */
void adc3_nch_dma_enable(uint16_t ndtr)
{  
    __HAL_ADC_DISABLE(&g_adc3_nch_dma_handle); /* 先关闭ADC */

    __HAL_DMA_DISABLE(&g_dma_nch_adc3_handle);   /* 关闭DMA传输 */
    g_dma_nch_adc3_handle.Instance->NDTR = ndtr; /* 重设DMA传输数据量 */
    __HAL_DMA_ENABLE(&g_dma_nch_adc3_handle);    /* 开启DMA传输 */

    __HAL_ADC_ENABLE(&g_adc3_nch_dma_handle); /* 重新启动ADC */
    ADC_ADC3->CR2 |= 1 << 30;                 /* 启动规则转换通道 */
}

/**
 * @brief       ADC DMA采集中断服务函数
 * @param       无
 * @retval      无
 */
void ADC_ADC3_DMASx_IRQHandler(void)
{
    if (ADC_ADC3_DMASx_IS_TC())
    {
        g_adc3_dma_sta = 1;      /* 标记DMA传输完成 */
        ADC_ADC3_DMASx_CLR_TC(); /* 清除DMA2 数据流4 传输完成中断 */
    }
}