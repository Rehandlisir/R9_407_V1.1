
#ifndef __MODBUS_H
#define __MODBUS_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* Modbus 引脚 和 串口 定义 
 * 默认是针对Modbus2的.
 * 注意: 通过修改这10个宏定义, 可以支持UART1~UART7任意一个串口.
 */
#define Modbus_RE_GPIO_PORT                  GPIOF
#define Modbus_RE_GPIO_PIN                   GPIO_PIN_12
#define Modbus_RE_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)   /* PD口时钟使能 */

#define Modbus_TX_GPIO_PORT                  GPIOD
#define Modbus_TX_GPIO_PIN                   GPIO_PIN_5
#define Modbus_TX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define Modbus_RX_GPIO_PORT                  GPIOD
#define Modbus_RX_GPIO_PIN                   GPIO_PIN_6
#define Modbus_RX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define Modbus_UX                            USART2
#define Modbus_UX_IRQn                       USART2_IRQn
#define Modbus_UX_IRQHandler                 USART2_IRQHandler
#define Modbus_UX_CLK_ENABLE()               do{ __HAL_RCC_USART2_CLK_ENABLE(); }while(0)  /* USART3 时钟使能 */


/******************************************************************************************/

/* 控制Modbus_RE脚, 控制Modbus发送/接收状态
 * Modbus_RE = 0, 进入接收模式
 * Modbus_RE = 1, 进入发送模式
 */
#define Modbus_RE(x)   do{ x ? \
                          HAL_GPIO_WritePin(Modbus_RE_GPIO_PORT, Modbus_RE_GPIO_PIN, GPIO_PIN_SET) : \
                          HAL_GPIO_WritePin(Modbus_RE_GPIO_PORT, Modbus_RE_GPIO_PIN, GPIO_PIN_RESET); \
                      }while(0)

 					  
void modbusUART2_init( uint32_t baudrate);  /* Modbus初始化 */
void modbus_send_data(uint8_t *buf, uint8_t len);    /* Modbus发送数据 */

 					  
/******************************************************************************************/					  
typedef struct 
{
	//作为从机时使用
	uint8_t  myadd;        //本设备从机地址
	uint8_t  rcbuf[100];   //modbus接受缓冲区
	uint8_t  timout;       //modbus数据持续时间
	uint8_t  recount;      //modbus端口接收到的数据个数
	uint8_t  timrun;       //modbus定时器是否计时标志
	uint8_t  reflag;       //modbus一帧数据接受完成标志位
	uint8_t  sendbuf[100]; //modbus接发送缓冲区
}MODBUS;




extern MODBUS modbus;
extern uint16_t Reg[];
void Modbus_Init(void);
void Modbus_Func3(void);//读寄存器数据
void Modbus_Func6(void);//往1个寄存器中写入数据
void Modbus_Func16(void);//往多个寄存器中写入数据
void Modbus_Event(void);
uint16_t  Modbus_CRC16( uint8_t *puchMsg, uint16_t usDataLen );
#endif







