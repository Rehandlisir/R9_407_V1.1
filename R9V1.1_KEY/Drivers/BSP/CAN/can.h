/**
 ****************************************************************************************************
 * @file        can.h
 * @author      R9(V1.1)
 * @version     V1.1
 * @date        2023-06-06
 * @brief       CAN 驱动代码
 * @license    R9
 ****************************************************************************************************
 * @attention

 * 1, 优化can_send_msg函数, 新增发送超时处理机制
 ****************************************************************************************************
 */

#ifndef __CAN_H
#define __CAN_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* CAN 引脚 定义 */

#define CAN_RX_GPIO_PORT                GPIOB
#define CAN_RX_GPIO_PIN                 GPIO_PIN_8
#define CAN_RX_GPIO_CLK_ENABLE()        do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)     /* PA口时钟使能 */

#define CAN_TX_GPIO_PORT                GPIOB
#define CAN_TX_GPIO_PIN                 GPIO_PIN_9
#define CAN_TX_GPIO_CLK_ENABLE()        do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)     /* PA口时钟使能 */

/******************************************************************************************/

/* CAN接收RX0中断使能 */
#define CAN_RX0_INT_ENABLE              CAN1_RX0_IRQHandler
extern uint8_t CanKeybufSendSend[8];
extern uint8_t CanLedCmdbufReceive[8];
/* 函数声明 */
uint8_t can_receive_msg(uint32_t id, uint8_t *buf);                                     /* CAN接收数据, 查询 */
uint8_t can_send_msg(uint32_t id, uint8_t *msg, uint8_t len);                           /* CAN发送数据 */
uint8_t can_init(uint32_t tsjw,uint32_t tbs2,uint32_t tbs1,uint16_t brp,uint32_t mode); /* CAN初始化 */

#endif








