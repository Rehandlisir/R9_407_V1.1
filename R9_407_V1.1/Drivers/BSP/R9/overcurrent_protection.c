   #include "./BSP/R9/overcurrent_protection.h"


void dcurrentpro_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    // D_MOTOR_PROTECT_GPIO_CLK_ENABLE();                                                     
    // gpio_init_struct.Pin = D_MOTOR_PROTECT_GPIO_PIN;                 
    // gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            
    // gpio_init_struct.Pull = GPIO_PULLUP;                   
    // gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          
    // HAL_GPIO_Init(D_MOTOR_PROTECT_POART, &gpio_init_struct);      
	// D_MOTOR_PROTECT(1);    // Ĭ�ϲ�����  

      
    D_MOTOR1_SLEEP_GPIO_CLK_ENABLE();                                                     
    gpio_init_struct.Pin = D_MOTOR1_SLEEP_GPIO_PIN;                 
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            
    gpio_init_struct.Pull = GPIO_PULLUP;                   
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          
    HAL_GPIO_Init(D_MOTOR1_SLEEP_POART, &gpio_init_struct);      
	D_MOTOR1_SLEEP(1);    // Ĭ������  

    D_MOTOR2_SLEEP_GPIO_CLK_ENABLET();                                                     
    gpio_init_struct.Pin = D_MOTOR2_SLEEP_GPIO_PIN;                 
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            
    gpio_init_struct.Pull = GPIO_PULLUP;                   
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          
    HAL_GPIO_Init(D_MOTOR2_SLEEP_POART, &gpio_init_struct);      
	D_MOTOR2_SLEEP(1);    // Ĭ������  


}

void tcurrentpro_init(void)
{
    // GPIO_InitTypeDef gpio_init_struct;          
    // T_MOTOR_PROTECT_CLK_ENABLE();                                
    // gpio_init_struct.Pin = T_MOTOR_PROTECT_GPIO_PIN;                   
    // gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;          
    // gpio_init_struct.Pull = GPIO_PULLUP;                    
    // gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;         
    // HAL_GPIO_Init(T_MOTOR_PROTECT_GPIO_PORT, &gpio_init_struct);                               
	// T_MOTOR_PROTECT(1);    // Ĭ�ϲ�����                                             
}
void currentproInit(void)
{
    dcurrentpro_init();                     /*����������ʼ��*/
	tcurrentpro_init();
}