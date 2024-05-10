
#include "./BSP/ADC/adc.h"
#include "./BSP/R9/getadcdata.h"
#include "./BSP/LED/led.h"
// #include "./BSP/LCD/lcd.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/Common/common.h"
 /****************************************************************************************************
 * @attention
 * 针对 R9系统的所有ADC 数据采集 ，
 *  一 、ADC1 采集10通道数据 包含   
 * (1)  摇杆数据采集         PA2 PA3
 * (2)  抱闸 数据监测        PA4 PA5
 * (3)  底盘电机电流检测     PA6 PA7
 * (4) 电池电压 /充电状态检测            PC4 PC5
 

 * 二、   ADC3 数据采集  包含
 * (1) 推杆1~6  位置检测  PF5 PF3 PF4 PF6 PF8 PF7
 * (2) 推杆 1~6 的电流检测 PC2 PC3 PC0 PC1 PF9 PF10
 * 
 ****************************************************************************************************
 */
#define ADC1_DMA_BUF_SIZE        10 * 8     /* ADC1 DMA采集 BUF大小, 应等于ADC通道数的整数倍 */
uint16_t g_adc1_dma_buf[ADC1_DMA_BUF_SIZE];   /* ADC DMA BUF */

extern uint8_t g_adc1_dma_sta;               /* DMA传输状态标志, 0, 未完成; 1, 已完成 */

#define ADC3_DMA_BUF_SIZE        10 * 12     /* ADC3 DMA采集 BUF大小, 应等于ADC通道数的整数倍 */
uint16_t g_adc3_dma_buf[ADC3_DMA_BUF_SIZE];   /* ADC DMA BUF */

extern uint8_t g_adc3_dma_sta;               /* DMA传输状态标志, 0, 未完成; 1, 已完成 */

ADCDATA adcdata;

void getadcDataInit(void)
{
	Datareset();
	adc1_nch_dma_init((uint32_t)&g_adc1_dma_buf);
	
    adc1_nch_dma_enable(ADC1_DMA_BUF_SIZE);   /* 启动ADC1 DMA多通道采集 */

    adc3_nch_dma_init((uint32_t)&g_adc3_dma_buf);
	
    adc3_nch_dma_enable(ADC3_DMA_BUF_SIZE);   /* 启动ADC1 DMA多通道采集 */

    joysticDatacalibrationInit();
}

void getadcData(void)
{
    getadc1Data();
    getadc3Data();
}


void getadc1Data(void)
{
	uint16_t i, j;
    uint16_t adc1_alldata;
    uint32_t sum;
    float temp;
	            /* 循环显示通道1~通道8的结果 */
	for(j = 0; j < 8; j++)  /* 遍历8个通道 */
	{
		sum = 0; /* 清零 */
		for (i = 0; i < ADC1_DMA_BUF_SIZE / 8; i++)  /* 每个通道采集了10次数据,进行10次累加 */
		{
			sum += g_adc1_dma_buf[(8 * i) + j];  /* 相同通道的转换数据累加 */
		}
		adc1_alldata = sum / (ADC1_DMA_BUF_SIZE / 8);    /* 取平均值 */

		if ( j == 0  )   
		{
            adcdata.adc_x = 1800-adc1_alldata;//adcdata.adc_xbase - adc1_alldata;//
            adcdata.adc_x = Value_limit(xadc_min ,Value_Resetzero(-xadc_Dim,adcdata.adc_x,xadc_Dim) ,xadc_max); // 设定耦合模糊区域及上下限值限制
			
		}

		if ( j == 1 )
		{
            adcdata.adc_y = adc1_alldata- 1800;//adcdata.adc_ybase; //-1800 ;//
            adcdata.adc_y= Value_limit(yadc_min ,Value_Resetzero(-yadc_Dim,adcdata.adc_y,yadc_Dim) ,yadc_max); // 设定耦合模糊区域及上下限数值限制
		}
		// printf("%d,%d\r\n",adcdata.adc_x,adcdata.adc_y);
		if ( j == 2 )
        {
			adcdata.l_brakcurrent = adc1_alldata;
		}
		if ( j == 3 )
        {
			adcdata.r_brakcurrent = adc1_alldata;
		}
		if ( j == 4 )
        {
			adcdata.l_current = adc1_alldata;
		}   
		if ( j == 5 )
        {
			adcdata.r_current = adc1_alldata;  
		}   
		if ( j == 6 )
		{
			adcdata.bat_v = adc1_alldata;

		}  
		if (j == 7)
		{
			adcdata.chargeI_adc = adc1_alldata;
		}
			  
	 }
	g_adc1_dma_sta = 0;  /* 清除DMA采集完成状态标志 */
	adc1_nch_dma_enable(ADC1_DMA_BUF_SIZE);   /* 启动下一次ADC1 DMA多通道采集 */
    
		
}

void getadc3Data(void)
{
	uint16_t i, j;
    uint16_t adc3_alldata;
    uint32_t sum;
	for(j = 0; j < 12; j++)  /* 遍历6个通道 */
	{
		sum = 0; /* 清零 */
		for (i = 0; i < ADC3_DMA_BUF_SIZE / 12; i++)  /* 每个通道采集了10次数据,进行10次累加 */
		{
			sum += g_adc3_dma_buf[(12 * i) + j];  /* 相同通道的转换数据累加 */
		}
		adc3_alldata = sum / (ADC3_DMA_BUF_SIZE / 12);    /* 取平均值 */

		if ( j == 0  )   
            adcdata.lift_pos = adc3_alldata;
		if ( j == 1 )
            adcdata.pedestal_pos = adc3_alldata; 
		if ( j == 2 )
            adcdata.backboard_pos = adc3_alldata;
		if ( j == 3 )
            adcdata.legangle_pos = adc3_alldata;
		if ( j == 4 )
            adcdata.leglength_pos = adc3_alldata;
		if ( j == 5 )
            adcdata.support_pos = adc3_alldata;      
		if ( j == 6 )
            adcdata.lift_current = adc3_alldata;      
		if ( j == 7 )
            adcdata.pedestal_current = adc3_alldata;      
		if ( j == 8 )
            adcdata.backboard_current = adc3_alldata;  
		if ( j == 9 )
            adcdata.legangle_current = adc3_alldata;    
		if ( j == 10 )
            adcdata.leglength_current = adc3_alldata;  
		if ( j == 11 )
            adcdata.support_current = adc3_alldata;                  
	 }
	g_adc3_dma_sta = 0;  /* 清除DMA采集完成状态标志 */
	adc3_nch_dma_enable(ADC3_DMA_BUF_SIZE);   /* 启动下一次ADC1 DMA多通道采集 */

}

void joysticDatacalibrationInit(void)
{
	uint16_t i, j ,k;
    uint16_t adc_current;
    uint32_t sum;
	for(k = 0; k < 1000; k++)               /* 转换 700次                  */
	{	/* 循环显示通道0~通道5的结果 */ 
		for(j = 0; j < 7; j++)  /* 遍历6个通道 */
		{
			sum = 0; /* 清零 */
			for (i = 0; i < ADC1_DMA_BUF_SIZE / 7; i++)  /* 每个通道采集了10次数据,进行10次累加 */
			{
				sum += g_adc1_dma_buf[(7 * i) + j];  /* 相同通道的转换数据累加 */
			}
			adc_current =(uint16_t) (sum / (ADC1_DMA_BUF_SIZE /7));    /* 取平均值 */
	// 缓存基准值
			if ( j == 0  )
				adcdata.adc_xbase = adc_current; 
			
			if ( j == 1  )
				adcdata.adc_ybase= adc_current;  	
			
		}
	}
    g_adc1_dma_sta = 0;  /* 清除DMA采集完成状态标志 */
	adc1_nch_dma_enable(ADC1_DMA_BUF_SIZE);   /* 启动下一次ADC1 DMA多通道采集 */
}

void Datareset(void)
{
	adcdata.adc_x =0;
	adcdata.adc_y =0;
	adcdata.adc_xbase =0;
	adcdata.adc_ybase =0;
	adcdata.l_current =0;
	adcdata.r_current =0;
	adcdata.lift_current =0;
	adcdata.pedestal_current =0;
	adcdata.backboard_current =0;	
	adcdata.legangle_current =0;
	adcdata.leglength_current =0;
	adcdata.support_current =0;
	adcdata.lift_pos =0;
	adcdata.pedestal_pos =0;
	adcdata.backboard_pos =0;
	adcdata.legangle_pos =0;
	adcdata.leglength_pos =0;
	adcdata.support_pos =0;	
//	adcdata.l_brakcurrent = 0;
//	adcdata.r_brakcurrent = 0;
//	adcdata.l_brakcurrent = 0;
//	adcdata.r_brakcurrent = 0;
	
}


