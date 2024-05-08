
#include "./BSP/Common/common.h"

double Value_limit(double min_value ,double current_value ,double max_value)
{
	
	if (current_value<min_value)
		return min_value;
	else if (current_value>max_value)
		return max_value;
	else
		return current_value;
}



int32_t  Value_Resetzero(int32_t min_value ,int32_t current_value ,int32_t max_value)
{
	if (current_value<min_value ||  current_value >  max_value)
		return current_value;
	else
		return 0;
}

double slopelimitx( double value,double increvalue)    
{
     static double out_last = 0; //��һ��ֵ
     double out;

  /***************** �����һ�ν��룬��� out_last ��ֵ ******************/
     static char fisrt_flag = 1;
     if (fisrt_flag == 1)
     {
        fisrt_flag = 0;
        out_last = value;
     }
 /***************** �������Լ�� ******************/
    if ( (value - out_last) >= increvalue )
    {
      out = out_last + increvalue;
    }
  /***************** ������ݼ�Լ�� ******************/   
    else if( (value - out_last) <= (- increvalue))
    {
      out = out_last - increvalue;
    }
  /***************** ����������Լ������ֱ����� ******************/  
    else
    {
      out = value ;
    }
      out_last = out;

      return out;
}

double slopelimity( double value,double increvalue)    
{
     static double out_last = 0; //��һ��ֵ
     double out;

  /***************** �����һ�ν��룬��� out_last ��ֵ ******************/
     static char fisrt_flag = 1;
     if (fisrt_flag == 1)
     {
        fisrt_flag = 0;
        out_last = value;
     }
 /***************** �������Լ�� ******************/
    if ( (value - out_last) >= increvalue )
    {
      out = out_last + increvalue;
    }
  /***************** ������ݼ�Լ�� ******************/   
    else if( (value - out_last) <= (- increvalue))
    {
      out = out_last - increvalue;
    }
  /***************** ����������Լ������ֱ����� ******************/  
    else
    {
      out = value ;
    }
      out_last = out;

      return out;
}


 

 
// ��ʼ���˲���
void initializeFilter(AverageFilter* filter)
{
    for (int i = 0; i < WINDOW_SIZE; ++i)
	{
        filter->window[i] = 0;
    }
    filter->index = 0;
}
 
// ����ƽ���˲�����
int filterValue(AverageFilter* filter, int input)
{
    // ���»�����
    filter->window[filter->index] = input;
    filter->index = (filter->index + 1) % WINDOW_SIZE;
 
    // ����ƽ��ֵ
    int sum = 0;
    for (int i = 0; i < WINDOW_SIZE; ++i)
	{
        sum += filter->window[i];
    }
    int average = sum / WINDOW_SIZE;
 
    return average;
}

//int main() {
//    // ��ʼ���˲���
//    AverageFilter myFilter;
//    initializeFilter(&myFilter);
// 
//    // ʹ��ʾ��
//    int inputValues[] = {10, 15, 20, 18, 22, 25, 17};
//    int numValues = sizeof(inputValues) / sizeof(inputValues[0]);
// 
//    printf("Input Values:\tFiltered Values:\n");
// 
//    for (int i = 0; i < numValues; ++i) {
//        int filteredValue = filterValue(&myFilter, inputValues[i]);
//        printf("%d\t\t%d\n", inputValues[i], filteredValue);
//    }
// 
//    return 0;
//}
