/**
 ****************************************************************************************************
 * @file        can.h
 * @author      R9(V1.1)
 * @version     V1.1
 * @date        2023-06-06
 * @brief       CAN ��������
 * @license    R9
 ****************************************************************************************************
 * @attention

 * 1, �Ż�can_send_msg����, �������ͳ�ʱ�������
 ****************************************************************************************************
 */

#ifndef __CAN_H
#define __CAN_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* CAN ���� ���� */

#define CAN_RX_GPIO_PORT                GPIOD
#define CAN_RX_GPIO_PIN                 GPIO_PIN_0
#define CAN_RX_GPIO_CLK_ENABLE()        do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)     /* PA��ʱ��ʹ�� */

#define CAN_TX_GPIO_PORT                GPIOD
#define CAN_TX_GPIO_PIN                 GPIO_PIN_1
#define CAN_TX_GPIO_CLK_ENABLE()        do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)     /* PA��ʱ��ʹ�� */

/******************************************************************************************/

/* CAN����RX0�ж�ʹ�� */
#define CAN_RX0_INT_ENABLE              CAN1_RX0_IRQHandler

extern uint8_t CanKeybufReceive[8];
extern uint8_t CanLedCmdbufSend[8];
#define CANMASTERID 0x01
#define LED_GREEN 0
#define LED_YELLOW 1
#define LED_RED 2

/* �������� */
uint8_t can_receive_msg(uint32_t id, uint8_t *buf);                                     /* CAN��������, ��ѯ */
uint8_t can_send_msg(uint32_t id, uint8_t *msg, uint8_t len);                           /* CAN�������� */
uint8_t can_init(uint32_t tsjw,uint32_t tbs2,uint32_t tbs1,uint16_t brp,uint32_t mode); /* CAN��ʼ�� */
void CanKeyreceive(void);
void CanCmdled(uint8_t cmd1,uint8_t cmd2,uint8_t cmd3,uint8_t cmd4,uint8_t cmd5);
void CanReadexcute(void);

#endif








