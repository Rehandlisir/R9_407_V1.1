
#ifndef __HOSTMODBUS_H
#define __HOSTMODBUS_H

#include "./SYSTEM/sys/sys.h"




/******************************************************************************************/
/* Modbus 引脚 和 串口 定义 
 * 默认是针对Modbus2的.
 * 注意: 通过修改这10个宏定义, 可以支持UART1~UART7任意一个串口.
 */
#define Modbus_RE_GPIO_PORT                  GPIOF
#define Modbus_RE_GPIO_PIN                   GPIO_PIN_2
#define Modbus_RE_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)   /*  */

#define Modbus_TX_GPIO_PORT                  GPIOC
#define Modbus_TX_GPIO_PIN                   GPIO_PIN_12
#define Modbus_TX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /*  */

#define Modbus_RX_GPIO_PORT                  GPIOD
#define Modbus_RX_GPIO_PIN                   GPIO_PIN_2
#define Modbus_RX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /* */

#define Modbus_UX                            UART5
#define Modbus_UX_IRQn                       UART5_IRQn
#define ModbusKey_UX_IRQHandler              UART5_IRQHandler
#define Modbus_UX_CLK_ENABLE()               do{ __HAL_RCC_UART5_CLK_ENABLE(); }while(0)  /* USART3时钟使能 */


/******************************************************************************************/

/* 控制Modbus_RE脚, 控制Modbus发送/接收状态
 * Modbus_RE = 0, 进入接收模式
 * Modbus_RE = 1, 进入发送模式
 */
#define Modbus_RE(x)   do{ x ? \
                          HAL_GPIO_WritePin(Modbus_RE_GPIO_PORT, Modbus_RE_GPIO_PIN, GPIO_PIN_SET) : \
                          HAL_GPIO_WritePin(Modbus_RE_GPIO_PORT, Modbus_RE_GPIO_PIN, GPIO_PIN_RESET); \
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
	
	//作为主机添加部分
	uint8_t Host_Txbuf[8];	//modbus发送数组	
	uint8_t slave_add;		//要匹配的从机设备地址（做主机实验时使用）
	uint8_t Host_send_flag;//主机设备发送数据完毕标志位
	int Host_Sendtime;//发送完一帧数据后时间计数
	uint8_t Host_time_flag;//发送时间到标志位，=1表示到发送数据时间了
	uint8_t Host_End;//接收数据后处理完毕	
	
}MODBUS;

extern MODBUS modbus;
extern uint16_t KeyReg[];

extern uint16_t KeyStateRecive[200];

void Host_ModbusKeyUART5_init(uint32_t baudrate);
void Host_Modbuskey_send_data(uint8_t *buf, uint8_t len);
void Host_Modbuskey_Init(void);
uint16_t  Host_Modbuskey_CRC16( uint8_t *puchMsg, uint16_t usDataLen );
// Host
void Host_Read03_slave(uint8_t slave,uint16_t StartAddr,uint16_t num);

void Host_Func3(void);

void HOST_ModbusRX(void);

void Host_write06_slave(uint8_t slave,uint8_t fun,uint16_t StartAddr,uint16_t num);

void Host_Func6(void);
#endif
 
