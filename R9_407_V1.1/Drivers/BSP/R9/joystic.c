  
//#include "./BSP/ADC/adc.h"
//#include "./BSP/R9/joystic.h"
//#include "./BSP/LED/led.h"
//#include "./BSP/LCD/lcd.h"
//#include "./SYSTEM/delay/delay.h"
//#include "./BSP/Common/common.h"

//#define ADC_DMA_BUF_SIZE        50 * 6     /* ADC DMA�ɼ� BUF��С, Ӧ����ADCͨ������������ */
//uint16_t g_adc_dma_buf[ADC_DMA_BUF_SIZE];   /* ADC DMA BUF */

//extern uint8_t g_adc1_dma_sta;               /* DMA����״̬��־, 0, δ���; 1, ����� */

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
//    adc1_nch_dma_enable(ADC_DMA_BUF_SIZE);   /* ����ADC DMA��ͨ���ɼ� */

//    joysticDatacalibration();
//	
//}


//uint8_t joysticDatacalibration(void)
//{
//	uint16_t i, j ,k;
//    uint16_t adcx;
//    uint32_t sum;
//    //delay_ms(1000);                          /*��Ӧ���ڴ˴����ÿ����ȴ�2 s                   */
//	for(k = 0; k < 1000; k++)               /* ת�� 6000��                  */
//	{	/* ѭ����ʾͨ��0~ͨ��5�Ľ�� */ 
//		for(j = 0; j < 6; j++)  /* ����6��ͨ�� */
//		{
//			sum = 0; /* ���� */
//			for (i = 0; i < ADC_DMA_BUF_SIZE / 6; i++)  /* ÿ��ͨ���ɼ���10������,����10���ۼ� */
//			{
//				sum += g_adc_dma_buf[(6 * i) + j];  /* ��ͬͨ����ת�������ۼ� */
//			}
//			adcx =(uint16_t) (sum / (ADC_DMA_BUF_SIZE /6));    /* ȡƽ��ֵ */
//	// �����׼ֵ
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
//	            /* ѭ����ʾͨ��0~ͨ��5�Ľ�� */
//	for(j = 0; j < 6; j++)  /* ����6��ͨ�� */
//	{
//		sum = 0; /* ���� */
//		for (i = 0; i < ADC_DMA_BUF_SIZE / 6; i++)  /* ÿ��ͨ���ɼ���10������,����10���ۼ� */
//		{
//			sum += g_adc_dma_buf[(6 * i) + j];  /* ��ͬͨ����ת�������ۼ� */
//		}
//		adcx = sum / (ADC_DMA_BUF_SIZE / 6);    /* ȡƽ��ֵ */
//		if (j == 0 || j == 1)
//		/* ��ʾ��� */
//		{

//            //gpio_init_struct.Pin =  GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 ; /* GPIOA2~7 */
//			if ( j == 0  )
//			
//				ADC_X = ADC_baseX-adcx; // X ��ֵ��ƫ��
//				ADC_X = Value_limit(xadc_min ,Value_Resetzero(-xadc_Dim,ADC_X,xadc_Dim) ,xadc_max); // �趨���ģ������������ֵ����

//			if ( j == 1 )
//				ADC_Y = adcx-ADC_baseY ; // Y  ��ֵ��ƫ��
//				ADC_Y= Value_limit(yadc_min ,Value_Resetzero(-yadc_Dim,ADC_Y,yadc_Dim) ,yadc_max); // �趨���ģ��������������ֵ����
//			
//		}
//	 }

//		g_adc1_dma_sta = 0;  /* ���DMA�ɼ����״̬��־ */
//		adc1_nch_dma_enable(ADC_DMA_BUF_SIZE);   /* ������һ��ADC DMA��ͨ���ɼ� */


//}



