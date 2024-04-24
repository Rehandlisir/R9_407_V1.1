  
//#include "./BSP/ADC/adc.h"
//#include "./BSP/R9/joystic.h"
//#include "./BSP/LED/led.h"
//#include "./BSP/LCD/lcd.h"
//#include "./SYSTEM/delay/delay.h"
//#include "./BSP/Common/common.h"

//#define ADC_DMA_BUF_SIZE        50 * 6     /* ADC DMA采集 BUF大小, 应等于ADC通道数的整数倍 */
//uint16_t g_adc_dma_buf[ADC_DMA_BUF_SIZE];   /* ADC DMA BUF */

//extern uint8_t g_adc1_dma_sta;               /* DMA传输状态标志, 0, 未完成; 1, 已完成 */

// int16_t  ADC_X;
// int16_t  ADC_Y;


// int16_t ADC_baseX;
// int16_t ADC_baseY;
// 


//void getjoysticInit(void)
//{
//	// delay_ms(500);    
//	adc1_nch_dma_init((uint32_t)&g_adc_dma_buf);
//	
//    adc1_nch_dma_enable(ADC_DMA_BUF_SIZE);   /* 启动ADC DMA多通道采集 */

//    joysticDatacalibration();
//	
//}


//uint8_t joysticDatacalibration(void)
//{
//	uint16_t i, j ,k;
//    uint16_t adcx;
//    uint32_t sum;
//    //delay_ms(1000);                          /*不应该在此处设置开机等待2 s                   */
//	for(k = 0; k < 1000; k++)               /* 转换 6000次                  */
//	{	/* 循环显示通道0~通道5的结果 */ 
//		for(j = 0; j < 6; j++)  /* 遍历6个通道 */
//		{
//			sum = 0; /* 清零 */
//			for (i = 0; i < ADC_DMA_BUF_SIZE / 6; i++)  /* 每个通道采集了10次数据,进行10次累加 */
//			{
//				sum += g_adc_dma_buf[(6 * i) + j];  /* 相同通道的转换数据累加 */
//			}
//			adcx =(uint16_t) (sum / (ADC_DMA_BUF_SIZE /6));    /* 取平均值 */
//	// 缓存基准值
//			if ( j == 0  )
//				ADC_baseX = adcx; 
//			
//			if ( j == 1  )
//				ADC_baseY = adcx;  	
//			
//		}
//		return  1 ;

//	}
//}

//void getjoysticData(void)
//{
//	uint16_t i, j;
//    uint16_t adcx;
//    uint32_t sum;
//    float temp;
//	            /* 循环显示通道0~通道5的结果 */
//	for(j = 0; j < 6; j++)  /* 遍历6个通道 */
//	{
//		sum = 0; /* 清零 */
//		for (i = 0; i < ADC_DMA_BUF_SIZE / 6; i++)  /* 每个通道采集了10次数据,进行10次累加 */
//		{
//			sum += g_adc_dma_buf[(6 * i) + j];  /* 相同通道的转换数据累加 */
//		}
//		adcx = sum / (ADC_DMA_BUF_SIZE / 6);    /* 取平均值 */
//		if (j == 0 || j == 1)
//		/* 显示结果 */
//		{

//            //gpio_init_struct.Pin =  GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 ; /* GPIOA2~7 */
//			if ( j == 0  )
//			
//				ADC_X = ADC_baseX-adcx; // X 数值清偏移
//				ADC_X = Value_limit(xadc_min ,Value_Resetzero(-xadc_Dim,ADC_X,xadc_Dim) ,xadc_max); // 设定耦合模糊区域及上下限值限制

//			if ( j == 1 )
//				ADC_Y = adcx-ADC_baseY ; // Y  数值清偏移
//				ADC_Y= Value_limit(yadc_min ,Value_Resetzero(-yadc_Dim,ADC_Y,yadc_Dim) ,yadc_max); // 设定耦合模糊区域及上下限数值限制
//			
//		}
//	 }

//		g_adc1_dma_sta = 0;  /* 清除DMA采集完成状态标志 */
//		adc1_nch_dma_enable(ADC_DMA_BUF_SIZE);   /* 启动下一次ADC DMA多通道采集 */


//}



