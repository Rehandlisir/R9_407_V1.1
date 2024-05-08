
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
     static double out_last = 0; //上一次值
     double out;

  /***************** 如果第一次进入，则给 out_last 赋值 ******************/
     static char fisrt_flag = 1;
     if (fisrt_flag == 1)
     {
        fisrt_flag = 0;
        out_last = value;
     }
 /***************** 正向递增约束 ******************/
    if ( (value - out_last) >= increvalue )
    {
      out = out_last + increvalue;
    }
  /***************** 负向向递减约束 ******************/   
    else if( (value - out_last) <= (- increvalue))
    {
      out = out_last - increvalue;
    }
  /***************** 不满足增量约束条件直接输出 ******************/  
    else
    {
      out = value ;
    }
      out_last = out;

      return out;
}

double slopelimity( double value,double increvalue)    
{
     static double out_last = 0; //上一次值
     double out;

  /***************** 如果第一次进入，则给 out_last 赋值 ******************/
     static char fisrt_flag = 1;
     if (fisrt_flag == 1)
     {
        fisrt_flag = 0;
        out_last = value;
     }
 /***************** 正向递增约束 ******************/
    if ( (value - out_last) >= increvalue )
    {
      out = out_last + increvalue;
    }
  /***************** 负向向递减约束 ******************/   
    else if( (value - out_last) <= (- increvalue))
    {
      out = out_last - increvalue;
    }
  /***************** 不满足增量约束条件直接输出 ******************/  
    else
    {
      out = value ;
    }
      out_last = out;

      return out;
}


 

 
// 初始化滤波器
void initializeFilter(AverageFilter* filter)
{
    for (int i = 0; i < WINDOW_SIZE; ++i)
	{
        filter->window[i] = 0;
    }
    filter->index = 0;
}
 
// 算术平均滤波函数
int filterValue(AverageFilter* filter, int input)
{
    // 更新缓存区
    filter->window[filter->index] = input;
    filter->index = (filter->index + 1) % WINDOW_SIZE;
 
    // 计算平均值
    int sum = 0;
    for (int i = 0; i < WINDOW_SIZE; ++i)
	{
        sum += filter->window[i];
    }
    int average = sum / WINDOW_SIZE;
 
    return average;
}

//int main() {
//    // 初始化滤波器
//    AverageFilter myFilter;
//    initializeFilter(&myFilter);
// 
//    // 使用示例
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
