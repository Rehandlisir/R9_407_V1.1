
#include "./BSP/Common/common.h"

AverageFilter filter_L;
AverageFilter filter_R;
int32_t Value_limit(int32_t min_value, int32_t current_value, int32_t max_value)
{

  if (current_value < min_value)
    return min_value;
  else if (current_value > max_value)
    return max_value;
  else
    return current_value;
}

float Value_limitf(float min_value, float current_value, float max_value)
{

  if (current_value < min_value)
    return min_value;
  else if (current_value > max_value)
    return max_value;
  else
    return current_value;
}

int32_t Value_Resetzero(int32_t min_value, int32_t current_value, int32_t max_value)
{
  if (current_value < min_value || current_value > max_value)
    return current_value;
  else
    return 0;
}

int32_t slopelimitx(int32_t value, int32_t increvalue)
{
  static int32_t out_last = 0; // 上一次值
  int32_t out;

  /***************** 如果第一次进入，则给 out_last 赋值 ******************/
  static char fisrt_flag = 1;
  if (fisrt_flag == 1)
  {
    fisrt_flag = 0;
    out_last = value;
  }
  /***************** 正向递增约束 ******************/
  if ((value - out_last) >= increvalue)
  {
    out = out_last + increvalue;
  }
  /***************** 负向向递减约束 ******************/
  else if ((value - out_last) <= (-increvalue))
  {
    out = out_last - increvalue;
  }
  /***************** 不满足增量约束条件直接输出 ******************/
  else
  {
    out = value;
  }
  out_last = out;

  return out;
}

int32_t slopelimity(int32_t value, int32_t increvalue)
{
  static int32_t out_last = 0; // 上一次值
  int32_t out;

  /***************** 如果第一次进入，则给 out_last 赋值 ******************/
  static char fisrt_flag = 1;
  if (fisrt_flag == 1)
  {
    fisrt_flag = 0;
    out_last = value;
  }
  /***************** 正向递增约束 ******************/
  if ((value - out_last) >= increvalue)
  {
    out = out_last + increvalue;
  }
  /***************** 负向向递减约束 ******************/
  else if ((value - out_last) <= (-increvalue))
  {
    out = out_last - increvalue;
  }
  /***************** 不满足增量约束条件直接输出 ******************/
  else
  {
    out = value;
  }
  out_last = out;

  return out;
}

double slopelimitLDuty(double value, double increvalue,double decreasvalue)
{
  static double out_last = 0; // 上一次值
  double out;

  /***************** 如果第一次进入，则给 out_last 赋值 ******************/
  static char fisrt_flag = 1;
  if (fisrt_flag == 1)
  {
    fisrt_flag = 0;
    out_last = value;
  }
  /***************** 正向递增约束 ******************/
  if ((value - out_last) >= increvalue)
  {
    out = out_last + increvalue;
  }
  /***************** 负向向递减约束 ******************/
  else if ((value - out_last) <= (-decreasvalue))
  {
    out = out_last - decreasvalue;
  }
  /***************** 不满足增量约束条件直接输出 ******************/
  else
  {
    out = value;
  }
  out_last = out;

  return out;
}

double slopelimitRDuty(double value, double increvalue,double decreasvalue)
{
  static double out_last = 0; // 上一次值
  double out;

  /***************** 如果第一次进入，则给 out_last 赋值 ******************/
  static char fisrt_flag = 1;
  if (fisrt_flag == 1)
  {
    fisrt_flag = 0;
    out_last = value;
  }
  /***************** 正向递增约束 ******************/
  if ((value - out_last) >= increvalue)
  {
    out = out_last + increvalue;
  }
  /***************** 负向向递减约束 ******************/
  else if ((value - out_last) <= (-decreasvalue))
  {
    out = out_last - decreasvalue;
  }
  /***************** 不满足增量约束条件直接输出 ******************/
  else
  {
    out = value;
  }
  out_last = out;

  return out;
}



// 初始化滤波器
void initializeFilter(AverageFilter *filter)
{
  for (int i = 0; i < WINDOW_SIZE; ++i)
  {
    filter->window[i] = 0;
  }
  filter->index = 0;
}

// 算术平均滤波函数
double filterValue(AverageFilter *filter, double input)
{
  // 更新缓存区
  filter->window[filter->index] = input;
  filter->index = (filter->index + 1) % WINDOW_SIZE;

  // 计算平均值
  double sum = 0.0;
  for (int i = 0; i < WINDOW_SIZE; ++i)
  {
    sum += filter->window[i];
  }
  double average = sum / WINDOW_SIZE;

  return average;
}

/**
 * @brief  cubic polynomial interpolation struct init.
 * @param  none
 * @retval none
 */
void mcl_cubic_poly_intpol_st_init(mcl_cubic_poly_intpol_st *cubic_poly_intpol, float intpol_ts)
{
  cubic_poly_intpol->intpol_completed = 0;
  cubic_poly_intpol->start = 0;
  cubic_poly_intpol->end = 0;
  cubic_poly_intpol->intpol_ts = intpol_ts;
  cubic_poly_intpol->intpol_time = 0;
  cubic_poly_intpol->total_intpol_time = 0;
  cubic_poly_intpol->next_value = 0;
}

/**
 * @brief  cubic polynomial interpolation.
 * @param  初始位置、末端位置已经知道、初始速度为0、末端速度为0、加速度为0
 * @retval none
 */
unsigned char mcl_cubic_poly_intpol(mcl_cubic_poly_intpol_st *cubic_poly_intpol, float init_value, float final_value, float time)
{
  if (time <= 0)
    return 0;

  if (cubic_poly_intpol->intpol_completed == 0)
  {

    cubic_poly_intpol->start = init_value;
    cubic_poly_intpol->end = final_value;
    cubic_poly_intpol->intpol_time = 0;
    cubic_poly_intpol->total_intpol_time = time;

    // compute interpolation coefficients
    cubic_poly_intpol->coefficients[0] = cubic_poly_intpol->start;
    cubic_poly_intpol->coefficients[1] = 0;
    cubic_poly_intpol->coefficients[2] = 3 * (cubic_poly_intpol->end - cubic_poly_intpol->start) / (cubic_poly_intpol->total_intpol_time * cubic_poly_intpol->total_intpol_time);
    cubic_poly_intpol->coefficients[3] = -2 * (cubic_poly_intpol->end - cubic_poly_intpol->start) / (cubic_poly_intpol->total_intpol_time * cubic_poly_intpol->total_intpol_time * cubic_poly_intpol->total_intpol_time);

    cubic_poly_intpol->intpol_completed = 1;
    return 1;
  }
  else
    return 0;
}

/**
 * @brief  passing path point cubic polynomial interpolation.
 * @param  none
 * @retval none
 */
unsigned char mcl_ppp_cubic_poly_intpol(mcl_cubic_poly_intpol_st *cubic_poly_intpol,float init_value, float final_value, float time, float init_velocity, float final_velocity)
{
  if (time <= 0)
    return 0;

  if (cubic_poly_intpol->intpol_completed == 0)
  {
    cubic_poly_intpol->start = init_value;
    cubic_poly_intpol->end = final_value;
    cubic_poly_intpol->intpol_time = 0;
    cubic_poly_intpol->total_intpol_time = time;

    // compute interpolation coefficients
    cubic_poly_intpol->coefficients[0] = cubic_poly_intpol->start;
    cubic_poly_intpol->coefficients[1] = init_velocity;
    cubic_poly_intpol->coefficients[2] = 3 * (cubic_poly_intpol->end - cubic_poly_intpol->start) / (cubic_poly_intpol->total_intpol_time * cubic_poly_intpol->total_intpol_time) - 2 * init_velocity / cubic_poly_intpol->total_intpol_time - final_velocity / cubic_poly_intpol->total_intpol_time;
    cubic_poly_intpol->coefficients[3] = -2 * (cubic_poly_intpol->end - cubic_poly_intpol->start) / (cubic_poly_intpol->total_intpol_time * cubic_poly_intpol->total_intpol_time * cubic_poly_intpol->total_intpol_time) + (init_velocity + final_velocity) / (cubic_poly_intpol->total_intpol_time * cubic_poly_intpol->total_intpol_time);
    cubic_poly_intpol->intpol_completed = 1;
    return 1;
  }
  else
    return 0;
}

/**
 * @brief  cubic polynomial interpolation updata value.
 * @param  none
 * @retval none
 */
float mcl_cubic_poly_intpol_updata(mcl_cubic_poly_intpol_st *cubic_poly_intpol)
{
  if (cubic_poly_intpol->intpol_time <= cubic_poly_intpol->total_intpol_time)
  {
    cubic_poly_intpol->intpol_time += cubic_poly_intpol->intpol_ts;
    cubic_poly_intpol->next_value = cubic_poly_intpol->coefficients[0] + cubic_poly_intpol->coefficients[1] * cubic_poly_intpol->intpol_time + cubic_poly_intpol->coefficients[2] * cubic_poly_intpol->intpol_time * cubic_poly_intpol->intpol_time + cubic_poly_intpol->coefficients[3] * cubic_poly_intpol->intpol_time * cubic_poly_intpol->intpol_time * cubic_poly_intpol->intpol_time;
  }
  else
  {
    cubic_poly_intpol->next_value = cubic_poly_intpol->end;
    cubic_poly_intpol->intpol_completed = 0; // interpolation completed
  }
  return cubic_poly_intpol->next_value;
}

/**
 * @brief  line polynomial interpolation struct init.
 * @param  none
 * @retval none
 */
void mcl_line_poly_intpol_st_init(mcl_line_poly_intpol_st *line_poly_intpol, float intpol_ts)
{
  line_poly_intpol->intpol_completed = 0;
  line_poly_intpol->start = 0;
  line_poly_intpol->end = 0;
  line_poly_intpol->intpol_ts = intpol_ts;
  line_poly_intpol->intpol_time = 0;
  line_poly_intpol->total_intpol_time = 0;
  line_poly_intpol->next_value = 0;
}

/**
 * @brief  line polynomial interpolation.
 * @param  none
 * @retval none
 */
unsigned char mcl_line_poly_intpol(mcl_line_poly_intpol_st *line_poly_intpol, float init_value, float final_value, float time)
{
  if (time <= 0)
    return 0;

  if (line_poly_intpol->intpol_completed == 0)
  {
    line_poly_intpol->start = init_value;
    line_poly_intpol->end = final_value;
    line_poly_intpol->intpol_time = 0;
    line_poly_intpol->total_intpol_time = time;
    line_poly_intpol->intpol_completed = 1;
    return 1;
  }
  else
    return 0;
}

/**
 * @brief  Update the line interpolation value.
 * @param  line_poly_intpol: joint interpolation struct.
 * @retval 1 means updata intepolation data success, 0 means fail.
 */
float mcl_line_poly_intpol_updata(mcl_line_poly_intpol_st *line_poly_intpol)
{
  if ((line_poly_intpol->intpol_time < line_poly_intpol->total_intpol_time) && (line_poly_intpol->intpol_completed == 1))
  {
    line_poly_intpol->intpol_time += line_poly_intpol->intpol_ts;
    line_poly_intpol->next_value = line_poly_intpol->start + (line_poly_intpol->end - line_poly_intpol->start) * line_poly_intpol->intpol_time / line_poly_intpol->total_intpol_time;
  }
  else
  {
    line_poly_intpol->intpol_completed = 0; // interpolation completed
  }
  return line_poly_intpol->next_value;
}

/**
 * @brief  parabola polynomial interpolation struct init.
 * @param  none
 * @retval none
 */
void mcl_parabola_poly_intpol_st_init(mcl_parabola_poly_intpol_st *parabola_poly_intpol, float intpol_ts)
{
  parabola_poly_intpol->intpol_completed = 0;
  parabola_poly_intpol->peak = 0;
  parabola_poly_intpol->intpol_ts = intpol_ts;
  parabola_poly_intpol->intpol_time = 0;
  parabola_poly_intpol->total_time1 = 0;
  parabola_poly_intpol->total_time2 = 0;
  parabola_poly_intpol->peak_time = 0;
  parabola_poly_intpol->next_value = 0;
}

/**
 * @brief  parabola polynomial interpolation. from zero to peak
 * @param  none
 * @retval none
 */
unsigned char mcl_parabola_poly_intpol(mcl_parabola_poly_intpol_st *parabola_poly_intpol, float peak, float time1, float time2, float peak_time)
{
  if (time1 <= 0)
    return 0;

  if (time2 <= 0)
    return 0;

  if (parabola_poly_intpol->intpol_completed == 0)
  {

    parabola_poly_intpol->peak = peak;
    parabola_poly_intpol->intpol_time = 0;
    parabola_poly_intpol->total_time1 = time1;
    parabola_poly_intpol->total_time2 = time2;
    parabola_poly_intpol->peak_time = peak_time;

    // compute interpolation coefficients, the acceleration coefficient
    parabola_poly_intpol->acc_coefficients[0] = 0;
    parabola_poly_intpol->acc_coefficients[1] = 0;
    parabola_poly_intpol->acc_coefficients[2] = 3 * parabola_poly_intpol->peak / (parabola_poly_intpol->total_time1 * parabola_poly_intpol->total_time1);
    parabola_poly_intpol->acc_coefficients[3] = -2 * parabola_poly_intpol->peak / (parabola_poly_intpol->total_time1 * parabola_poly_intpol->total_time1 * parabola_poly_intpol->total_time1);

    // compute interpolation coefficients, the deceleration coefficient
    parabola_poly_intpol->dec_coefficients[0] = parabola_poly_intpol->peak;
    parabola_poly_intpol->dec_coefficients[1] = 0;
    parabola_poly_intpol->dec_coefficients[2] = 3 * (0 - parabola_poly_intpol->peak) / (parabola_poly_intpol->total_time2 * parabola_poly_intpol->total_time2);
    parabola_poly_intpol->dec_coefficients[3] = -2 * (0 - parabola_poly_intpol->peak) / (parabola_poly_intpol->total_time2 * parabola_poly_intpol->total_time2 * parabola_poly_intpol->total_time2);

    parabola_poly_intpol->intpol_completed = 1;

    return 1;
  }
  else
    return 0;
}

/**
 * @brief  parabola polynomial interpolation. from any value to peak, and return to zero
 * @param  none
 * @retval none
 */
unsigned char mcl_parabola_poly_intpol_1(mcl_parabola_poly_intpol_st *parabola_poly_intpol, float start, float peak, float last, float time1, float time2, float peak_time)
{
  if (time1 <= 0)
    return 0;

  if (time2 <= 0)
    return 0;

  if (parabola_poly_intpol->intpol_completed == 0)
  {

    parabola_poly_intpol->peak = peak;
    parabola_poly_intpol->intpol_time = 0;
    parabola_poly_intpol->total_time1 = time1;
    parabola_poly_intpol->total_time2 = time2;
    parabola_poly_intpol->peak_time = peak_time;

    // compute interpolation coefficients, the acceleration coefficient
    parabola_poly_intpol->acc_coefficients[0] = start;
    parabola_poly_intpol->acc_coefficients[1] = 0;
    parabola_poly_intpol->acc_coefficients[2] = 3 * (parabola_poly_intpol->peak - start) / (parabola_poly_intpol->total_time1 * parabola_poly_intpol->total_time1);
    parabola_poly_intpol->acc_coefficients[3] = -2 * (parabola_poly_intpol->peak - start) / (parabola_poly_intpol->total_time1 * parabola_poly_intpol->total_time1 * parabola_poly_intpol->total_time1);

    // compute interpolation coefficients, the deceleration coefficient
    parabola_poly_intpol->dec_coefficients[0] = parabola_poly_intpol->peak;
    parabola_poly_intpol->dec_coefficients[1] = 0;
    parabola_poly_intpol->dec_coefficients[2] = 3 * (last - parabola_poly_intpol->peak) / (parabola_poly_intpol->total_time2 * parabola_poly_intpol->total_time2);
    parabola_poly_intpol->dec_coefficients[3] = -2 * (last - parabola_poly_intpol->peak) / (parabola_poly_intpol->total_time2 * parabola_poly_intpol->total_time2 * parabola_poly_intpol->total_time2);

    parabola_poly_intpol->intpol_completed = 1;

    return 1;
  }
  else
    return 0;
}

/**
 * @brief  parabola polynomial interpolation. from zero to peak
 * @param  none
 * @retval none
 */
unsigned char mcl_parabola_poly_intpol_2(mcl_parabola_poly_intpol_st *parabola_poly_intpol, float peak, float time1)
{
  if (time1 <= 0)
    return 0;

  if (parabola_poly_intpol->intpol_completed == 0)
  {

    parabola_poly_intpol->peak = peak;
    parabola_poly_intpol->intpol_time = 0;
    parabola_poly_intpol->total_time1 = time1;

    // compute interpolation coefficients, the acceleration coefficient
    parabola_poly_intpol->acc_coefficients[0] = 0;
    parabola_poly_intpol->acc_coefficients[1] = 0;
    parabola_poly_intpol->acc_coefficients[2] = 3 * parabola_poly_intpol->peak / (parabola_poly_intpol->total_time1 * parabola_poly_intpol->total_time1);
    parabola_poly_intpol->acc_coefficients[3] = -2 * parabola_poly_intpol->peak / (parabola_poly_intpol->total_time1 * parabola_poly_intpol->total_time1 * parabola_poly_intpol->total_time1);

    parabola_poly_intpol->intpol_completed = 1;

    return 1;
  }
  else
    return 0;
}

unsigned char mcl_parabola_poly_intpol_3(mcl_parabola_poly_intpol_st *parabola_poly_intpol, float peak, float time2)
{
  if (time2 <= 0)
    return 0;

  if (parabola_poly_intpol->intpol_completed == 0)
  {

    parabola_poly_intpol->peak = peak;
    parabola_poly_intpol->intpol_time = 0;
    parabola_poly_intpol->total_time2 = time2;

    // compute interpolation coefficients, the deceleration coefficient
    parabola_poly_intpol->dec_coefficients[0] = parabola_poly_intpol->peak;
    parabola_poly_intpol->dec_coefficients[1] = 0;
    parabola_poly_intpol->dec_coefficients[2] = 3 * (0 - parabola_poly_intpol->peak) / (parabola_poly_intpol->total_time2 * parabola_poly_intpol->total_time2);
    parabola_poly_intpol->dec_coefficients[3] = -2 * (0 - parabola_poly_intpol->peak) / (parabola_poly_intpol->total_time2 * parabola_poly_intpol->total_time2 * parabola_poly_intpol->total_time2);

    parabola_poly_intpol->intpol_completed = 1;

    return 1;
  }
  else
    return 0;
}

// from init_value to peak_value
unsigned char mcl_parabola_poly_intpol_4(mcl_parabola_poly_intpol_st *parabola_poly_intpol, float init_value, float peak, float time1)
{
  if (time1 <= 0)
    return 0;

  if (parabola_poly_intpol->intpol_completed == 0)
  {

    parabola_poly_intpol->peak = peak;
    parabola_poly_intpol->intpol_time = 0;
    parabola_poly_intpol->total_time1 = time1;

    // compute interpolation coefficients, the acceleration coefficient
    parabola_poly_intpol->acc_coefficients[0] = init_value;
    parabola_poly_intpol->acc_coefficients[1] = 0;
    parabola_poly_intpol->acc_coefficients[2] = 3 * (parabola_poly_intpol->peak - init_value) / (parabola_poly_intpol->total_time1 * parabola_poly_intpol->total_time1);
    parabola_poly_intpol->acc_coefficients[3] = -2 * (parabola_poly_intpol->peak - init_value) / (parabola_poly_intpol->total_time1 * parabola_poly_intpol->total_time1 * parabola_poly_intpol->total_time1);

    parabola_poly_intpol->intpol_completed = 1;

    return 1;
  }
  else
    return 0;
}

/**
 * @brief  parabola polynomial interpolation updata value.
 * @param  none
 * @retval none
 */
float mcl_parabola_poly_intpol_updata(mcl_parabola_poly_intpol_st *parabola_poly_intpol)
{
  if (parabola_poly_intpol->intpol_time <= parabola_poly_intpol->total_time1)
  {
    parabola_poly_intpol->intpol_time += parabola_poly_intpol->intpol_ts;
    parabola_poly_intpol->next_value = parabola_poly_intpol->acc_coefficients[0] + parabola_poly_intpol->acc_coefficients[1] * parabola_poly_intpol->intpol_time + parabola_poly_intpol->acc_coefficients[2] * parabola_poly_intpol->intpol_time * parabola_poly_intpol->intpol_time + parabola_poly_intpol->acc_coefficients[3] * parabola_poly_intpol->intpol_time * parabola_poly_intpol->intpol_time * parabola_poly_intpol->intpol_time;
  }
  else if (parabola_poly_intpol->intpol_time > parabola_poly_intpol->total_time1 && parabola_poly_intpol->intpol_time <= (parabola_poly_intpol->total_time1 + parabola_poly_intpol->peak_time))
  {
    parabola_poly_intpol->intpol_time += parabola_poly_intpol->intpol_ts;
    parabola_poly_intpol->next_value = parabola_poly_intpol->peak;
  }
  else if (parabola_poly_intpol->intpol_time > (parabola_poly_intpol->total_time1 + parabola_poly_intpol->peak_time) && parabola_poly_intpol->intpol_time <= (parabola_poly_intpol->total_time1 + parabola_poly_intpol->peak_time + parabola_poly_intpol->total_time2))
  {

    parabola_poly_intpol->intpol_time += parabola_poly_intpol->intpol_ts;
    parabola_poly_intpol->intpol_time2 = parabola_poly_intpol->intpol_time - parabola_poly_intpol->total_time1 - parabola_poly_intpol->peak_time;
    parabola_poly_intpol->next_value = parabola_poly_intpol->dec_coefficients[0] + parabola_poly_intpol->dec_coefficients[1] * parabola_poly_intpol->intpol_time2 + parabola_poly_intpol->dec_coefficients[2] * parabola_poly_intpol->intpol_time2 * parabola_poly_intpol->intpol_time2 + parabola_poly_intpol->dec_coefficients[3] * parabola_poly_intpol->intpol_time2 * parabola_poly_intpol->intpol_time2 * parabola_poly_intpol->intpol_time2;
  }
  else
  {
    //		parabola_poly_intpol->next_value = parabola_poly_intpol->peak;
    parabola_poly_intpol->intpol_completed = 0; // interpolation completed
  }
  return parabola_poly_intpol->next_value;
}

/**
 * @brief  parabola polynomial interpolation updata value.
 * @param  none
 * @retval none
 */
float mcl_parabola_poly_intpol_updata_2(mcl_parabola_poly_intpol_st *parabola_poly_intpol)
{
  if (parabola_poly_intpol->intpol_time <= parabola_poly_intpol->total_time1)
  {
    parabola_poly_intpol->intpol_time += parabola_poly_intpol->intpol_ts;
    parabola_poly_intpol->next_value = parabola_poly_intpol->acc_coefficients[0] + parabola_poly_intpol->acc_coefficients[1] * parabola_poly_intpol->intpol_time + parabola_poly_intpol->acc_coefficients[2] * parabola_poly_intpol->intpol_time * parabola_poly_intpol->intpol_time + parabola_poly_intpol->acc_coefficients[3] * parabola_poly_intpol->intpol_time * parabola_poly_intpol->intpol_time * parabola_poly_intpol->intpol_time;
  }
  else
  {
    //		parabola_poly_intpol->next_value = parabola_poly_intpol->peak;
    parabola_poly_intpol->intpol_completed = 0; // interpolation completed
  }
  return parabola_poly_intpol->next_value;
}

float mcl_parabola_poly_intpol_updata_3(mcl_parabola_poly_intpol_st *parabola_poly_intpol)
{
  if (parabola_poly_intpol->intpol_time <= parabola_poly_intpol->total_time2)
  {
    parabola_poly_intpol->intpol_time += parabola_poly_intpol->intpol_ts;
    parabola_poly_intpol->intpol_time2 = parabola_poly_intpol->intpol_time;
    parabola_poly_intpol->next_value = parabola_poly_intpol->dec_coefficients[0] + parabola_poly_intpol->dec_coefficients[1] * parabola_poly_intpol->intpol_time2 + parabola_poly_intpol->dec_coefficients[2] * parabola_poly_intpol->intpol_time2 * parabola_poly_intpol->intpol_time2 + parabola_poly_intpol->dec_coefficients[3] * parabola_poly_intpol->intpol_time2 * parabola_poly_intpol->intpol_time2 * parabola_poly_intpol->intpol_time2;
  }
  else
  {
    //		parabola_poly_intpol->next_value = parabola_poly_intpol->peak;
    parabola_poly_intpol->intpol_completed = 0; // interpolation completed
  }
  return parabola_poly_intpol->next_value;
}

/****************************** END OF FILE ***********************************/
