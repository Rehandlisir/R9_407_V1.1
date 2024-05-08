///**
// ****************************************************************************************************
// * @file        rs485.h

// ****************************************************************************************************
// * @attention
// *

// *
// ****************************************************************************************************
// */

//#ifndef __RS485_H
//#define __RS485_H

//#include "./SYSTEM/sys/sys.h"


///******************************************************************************************/
///* RS485 ���� �� ���� ���� 
// * Ĭ�������RS4852��.
// * ע��: ͨ���޸���10���궨��, ����֧��UART1~UART7����һ������.
// */
//#define RS485_RE_GPIO_PORT                  GPIOF
//#define RS485_RE_GPIO_PIN                   GPIO_PIN_12
//#define RS485_RE_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)   /* PD��ʱ��ʹ�� */

//#define RS485_TX_GPIO_PORT                  GPIOD
//#define RS485_TX_GPIO_PIN                   GPIO_PIN_5
//#define RS485_TX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */

//#define RS485_RX_GPIO_PORT                  GPIOD
//#define RS485_RX_GPIO_PIN                   GPIO_PIN_6
//#define RS485_RX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */

//#define RS485_UX                            USART2
//#define RS485_UX_IRQn                       USART2_IRQn
//#define RS485_UX_IRQHandler                 USART2_IRQHandler
//#define RS485_UX_CLK_ENABLE()               do{ __HAL_RCC_USART2_CLK_ENABLE(); }while(0)  /* USART2 ʱ��ʹ�� */


//#define ID               0x01       // ��������ַ
//#define READ             0x03       // ��������
//#define REGISTERADRR1    0x01
//#define REGISTERADRR2    0x01       // ʵʱֵ
//#define REGISTERnum1     0x00       // �Ĵ�������
//#define REGISTERnum2     0x01       // �Ĵ�������
//#define CHECK1           0xD4       // У��λ1
//#define CHECK2           0x36       // У��λ2


///******************************************************************************************/

///* ����RS485_RE��, ����RS485����/����״̬
// * RS485_RE = 0, �������ģʽ
// * RS485_RE = 1, ���뷢��ģʽ
// */
// 
//#define RS485_RE(x)   do{ x ? \
//                          HAL_GPIO_WritePin(RS485_RE_GPIO_PORT, RS485_RE_GPIO_PIN, GPIO_PIN_SET) : \
//                          HAL_GPIO_WritePin(RS485_RE_GPIO_PORT, RS485_RE_GPIO_PIN, GPIO_PIN_RESET); \
//                      }while(0)


//#define RS485_REC_LEN               7          /* �����������ֽ��� 64 */
//#define RS485_EN_RX                 1           /* ʹ�ܣ�1��/��ֹ��0��RS485���� */


//extern uint8_t g_RS485_rx_buf[RS485_REC_LEN];   /* ���ջ���,���RS485_REC_LEN���ֽ� */
//extern uint8_t g_RS485_rx_cnt;                  /* �������ݳ��� */


//void rs485_init( uint32_t baudrate);  /* RS485��ʼ�� */
//void rs485_send_data(uint8_t *buf, uint8_t len);    /* RS485�������� */
//void rs485_receive_data(uint8_t *buf, uint8_t *len);/* RS485�������� */
//#endif







