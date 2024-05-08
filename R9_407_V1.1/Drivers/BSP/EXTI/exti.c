// /**
//  ****************************************************************************************************
//  * @file        exti.c
//  * @author      Lisir
//  * @version     V1.0
//  * @date        2021-10-14
//  * @brief       外部中断 驱动代码
//  * @license     Copyright (c) 2024, 深圳复成医疗科技有限公司
//  ****************************************************************************************************
//  * @attention
//  *
//  * 实验平台:F407开发板
//  * none
//  * none
//  * none
//  * none
//  *
//  * 修改说明
//  * none
//  * 第一次发布
//  *
//  ****************************************************************************************************
//  */

// #include "./SYSTEM/sys/sys.h"
// #include "./SYSTEM/delay/delay.h"
// #include "./BSP/LED/led.h"
// #include "./BSP/BEEP/beep.h"
// #include "./BSP/KEY/key.h"
// #include "./BSP/EXTI/exti.h"


// /**
//  * @brief       KEY0 外部中断服务程序
//  * @param       无
//  * @retval      无
//  */
// void KEY0_INT_IRQHandler(void)
// {
//     HAL_GPIO_EXTI_IRQHandler(KEY0_INT_GPIO_PIN);         /* 调用中断处理公用函数 清除KEY0所在中断线 的中断标志位 */
//     __HAL_GPIO_EXTI_CLEAR_IT(KEY0_INT_GPIO_PIN);         /* HAL库默认先清中断再处理回调，退出时再清一次中断，避免按键抖动误触发 */
// }

// /**
//  * @brief       KEY1 外部中断服务程序
//  * @param       无
//  * @retval      无
//  */
// void KEY1_INT_IRQHandler(void)
// { 
//     HAL_GPIO_EXTI_IRQHandler(KEY1_INT_GPIO_PIN);         /* 调用中断处理公用函数 清除KEY1所在中断线 的中断标志位，中断下半部在HAL_GPIO_EXTI_Callback执行 */
//     __HAL_GPIO_EXTI_CLEAR_IT(KEY1_INT_GPIO_PIN);         /* HAL库默认先清中断再处理回调，退出时再清一次中断，避免按键抖动误触发 */
// }

// /**
//  * @brief       KEY2 外部中断服务程序
//  * @param       无
//  * @retval      无
//  */
// void KEY2_INT_IRQHandler(void)
// { 
//     HAL_GPIO_EXTI_IRQHandler(KEY2_INT_GPIO_PIN);        /* 调用中断处理公用函数 清除KEY2所在中断线 的中断标志位，中断下半部在HAL_GPIO_EXTI_Callback执行 */
//     __HAL_GPIO_EXTI_CLEAR_IT(KEY2_INT_GPIO_PIN);        /* HAL库默认先清中断再处理回调，退出时再清一次中断，避免按键抖动误触发 */
// }

// /**
//  * @brief       WK_UP 外部中断服务程序
//  * @param       无
//  * @retval      无
//  */
// void WKUP_INT_IRQHandler(void)
// { 
//     HAL_GPIO_EXTI_IRQHandler(WKUP_INT_GPIO_PIN);        /* 调用中断处理公用函数 清除KEY_UP所在中断线 的中断标志位，中断下半部在HAL_GPIO_EXTI_Callback执行 */
//     __HAL_GPIO_EXTI_CLEAR_IT(WKUP_INT_GPIO_PIN);        /* HAL库默认先清中断再处理回调，退出时再清一次中断，避免按键抖动误触发 */
// }

// /**
//  * @brief       中断服务程序中需要做的事情
//  *              在HAL库中所有的外部中断服务函数都会调用此函数
//  * @param       GPIO_Pin:中断引脚号
//  * @retval      无
//  */
// void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
// {
//     delay_ms(10);      /* 消抖 */
//     switch(GPIO_Pin)
//     {
//         case KEY0_INT_GPIO_PIN:
//             if (KEY0 == 0)
//             {
//                 LED0_TOGGLE();  /* LED0 状态取反 */ 

//             }
//             break;

//         case KEY1_INT_GPIO_PIN:
//             if (KEY1 == 0)
//             {
//                 LED1_TOGGLE();  /* LED1 状态取反 */ 
//             }
//             break;

//         case KEY2_INT_GPIO_PIN:
//             if (KEY2 == 0)
//             {
//                 LED1_TOGGLE();  /* LED1 状态取反 */
//                 LED0_TOGGLE();  /* LED0 状态取反 */ 
//             }
//             break;

//        case WKUP_INT_GPIO_PIN:
//            if (WK_UP == 1)
//            {
//                BEEP_TOGGLE();  /* 蜂鸣器状态取反 */ 
//            }
//            break;

//         default : break;
//     }
// }

// /**
//  * @brief       外部中断初始化程序
//  * @param       无
//  * @retval      无
//  */
// void extix_init(void)
// {
//     GPIO_InitTypeDef gpio_init_struct;
    
//     key_init();
//     gpio_init_struct.Pin = KEY0_INT_GPIO_PIN;
//     gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;            /* 下降沿触发 */
//     gpio_init_struct.Pull = GPIO_PULLUP;
//     HAL_GPIO_Init(KEY0_INT_GPIO_PORT, &gpio_init_struct);    /* KEY0配置为下降沿触发中断 */

//     gpio_init_struct.Pin = KEY1_INT_GPIO_PIN;
//     gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;            /* 下降沿触发 */
//     gpio_init_struct.Pull = GPIO_PULLUP;
//     HAL_GPIO_Init(KEY1_INT_GPIO_PORT, &gpio_init_struct);    /* KEY1配置为下降沿触发中断 */
    
//     gpio_init_struct.Pin = KEY2_INT_GPIO_PIN;
//     gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;            /* 下降沿触发 */
//     gpio_init_struct.Pull = GPIO_PULLUP;
//     HAL_GPIO_Init(KEY2_INT_GPIO_PORT, &gpio_init_struct);    /* KEY2配置为下降沿触发中断 */
    
//    gpio_init_struct.Pin = WKUP_INT_GPIO_PIN;
//    gpio_init_struct.Mode = GPIO_MODE_IT_RISING;             /* 上升沿触发 */
//    gpio_init_struct.Pull = GPIO_PULLDOWN;
//    HAL_GPIO_Init(WKUP_GPIO_PORT, &gpio_init_struct);        /* WKUP配置为上升沿触发中断 */

//     HAL_NVIC_SetPriority(KEY0_INT_IRQn, 0, 2);               /* 抢占0，子优先级2 */
//     HAL_NVIC_EnableIRQ(KEY0_INT_IRQn);                       /* 使能中断线4 */

//     HAL_NVIC_SetPriority(KEY1_INT_IRQn, 1, 2);                 /* 抢占1，子优先级2 */
//     HAL_NVIC_EnableIRQ(KEY1_INT_IRQn);                       /* 使能中断线3 */
    
//     HAL_NVIC_SetPriority(KEY2_INT_IRQn, 2, 2);               /* 抢占2，子优先级2 */
//     HAL_NVIC_EnableIRQ(KEY2_INT_IRQn);                       /* 使能中断线2 */

//     HAL_NVIC_SetPriority(WKUP_INT_IRQn, 3, 2);               /* 抢占3，子优先级2 */
//     HAL_NVIC_EnableIRQ(WKUP_INT_IRQn);                       /* 使能中断线0 */

// }












