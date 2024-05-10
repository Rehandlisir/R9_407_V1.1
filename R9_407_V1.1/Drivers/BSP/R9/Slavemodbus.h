
#ifndef __SLAVEMODBUS_H
#define __SLAVEMODBUS_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* SlaveSlaveModbus 引脚 和 串口 定义 
 * 默认是针对SlaveModbus2的.
 * 注意: 通过修改这10个宏定义, 可以支持UART1~UART7任意一个串口.
 */
#define SlaveModbus_RE_GPIO_PORT                  GPIOG
#define SlaveModbus_RE_GPIO_PIN                   GPIO_PIN_0
#define SlaveModbus_RE_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)   /* PD口时钟使能 */

#define SlaveModbus_TX_GPIO_PORT                  GPIOG
#define SlaveModbus_TX_GPIO_PIN                   GPIO_PIN_14
#define SlaveModbus_TX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define SlaveModbus_RX_GPIO_PORT                  GPIOG
#define SlaveModbus_RX_GPIO_PIN                   GPIO_PIN_9
#define SlaveModbus_RX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define SlaveModbus_UX                            USART6
#define SlaveModbus_UX_IRQn                       USART6_IRQn
#define SlaveModbus_UX_IRQHandler                 USART6_IRQHandler
#define SlaveModbus_UX_CLK_ENABLE()               do{ __HAL_RCC_USART6_CLK_ENABLE(); }while(0)  /* USART3 时钟使能 */


/******************************************************************************************/

/* 控制SlaveModbus_RE脚, 控制SlaveSlaveModbus发送/接收状态
 * SlaveModbus_RE = 0, 进入接收模式
 * SlaveModbus_RE = 1, 进入发送模式
 */
#define SlaveModbus_RE(x)   do{ x ? \
                          HAL_GPIO_WritePin(SlaveModbus_RE_GPIO_PORT, SlaveModbus_RE_GPIO_PIN, GPIO_PIN_SET) : \
                          HAL_GPIO_WritePin(SlaveModbus_RE_GPIO_PORT, SlaveModbus_RE_GPIO_PIN, GPIO_PIN_RESET); \
                      }while(0)

 					  


 					  
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
}SLAVEMODBUS;




extern SLAVEMODBUS slavemodbus;
extern uint16_t Reg[];
void SlavemodbusUART6_init(uint32_t baudrate);
void SlaveModbus_send_data(uint8_t *buf, uint8_t len);
void SlaveModbus_Init(void);
void SlaveModbus_Func3(void);//读寄存器数据
void SlaveModbus_Func6(void);//往1个寄存器中写入数据
void SlaveModbus_Func16(void);//往多个寄存器中写入数据
void SlaveModbus_Event(void);
uint16_t  SlaveModbus_CRC16( uint8_t *puchMsg, uint16_t usDataLen );
#endif







