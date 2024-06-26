
#ifndef __HOSTDAP21_H
#define __HOSTDAP21_H

#include "./SYSTEM/sys/sys.h"




/******************************************************************************************/
/* ModbusDap21 ?? ? ?? ?? 
 * ?????ModbusDap212?.
 * ??: ?????10????, ????UART1~UART7??????.
 */
#define ModbusDap21_RE_GPIO_PORT                  GPIOF
#define ModbusDap21_RE_GPIO_PIN                   GPIO_PIN_12
#define ModbusDap21_RE_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)   /*  */

#define ModbusDap21_TX_GPIO_PORT                  GPIOD
#define ModbusDap21_TX_GPIO_PIN                   GPIO_PIN_5
#define ModbusDap21_TX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /*  */

#define ModbusDap21_RX_GPIO_PORT                  GPIOD
#define ModbusDap21_RX_GPIO_PIN                   GPIO_PIN_6
#define ModbusDap21_RX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /* */

#define ModbusDap21_UX                            USART2
#define ModbusDap21_UX_IRQn                       USART2_IRQn
#define ModbusDap21_UX_IRQHandler              		USART2_IRQHandler
#define ModbusDap21_UX_CLK_ENABLE()               do{ __HAL_RCC_USART2_CLK_ENABLE(); }while(0)  /* USART3???? */


/******************************************************************************************/

/* ??ModbusDap21_RE?, ??ModbusDap21??/????
 * ModbusDap21_RE = 0, ??????
 * ModbusDap21_RE = 1, ??????
 */
#define ModbusDap21_RE(x)   do{ x ? \
                          HAL_GPIO_WritePin(ModbusDap21_RE_GPIO_PORT, ModbusDap21_RE_GPIO_PIN, GPIO_PIN_SET) : \
                          HAL_GPIO_WritePin(ModbusDap21_RE_GPIO_PORT, ModbusDap21_RE_GPIO_PIN, GPIO_PIN_RESET); \
                      }while(0)



 					  
/******************************************************************************************/					  
typedef struct 
{
	//???????
	uint8_t  myadd;        //???????
	uint8_t  rcbuf[100];   //modbus?????
	uint8_t  timout;       //modbus??????
	uint8_t  recount;      //modbus??????????
	uint8_t  timrun;       //modbus?????????
	uint8_t  reflag;       //modbus???????????
	uint8_t  sendbuf[100]; //modbus??????
	
	//????????
	uint8_t Host_Txbuf[8];	//modbus????	
	uint8_t slave_add;		//??????????(????????)
	uint8_t Host_send_flag;//?????????????
	int Host_Sendtime;//????????????
	uint8_t Host_time_flag;//????????,=1??????????
	uint8_t Host_End;//?????????	
	
}MODBUS_DAP21;



typedef struct 
{
	//???????
	uint8_t slav1data[2];
	uint8_t slav2data[2];
	uint8_t slav3data[2];
	uint8_t slav4data[2];
	uint16_t dyplength1;
	uint16_t dyplength2;
	uint16_t dyplength3;
	uint16_t dyplength4;
	
}Data_DAP21;

extern MODBUS_DAP21 modbus_dap21;
extern Data_DAP21 dap21Data;

//extern uint16_t Reg[];

extern uint16_t Dap21data[200];

void Host_ModbusDap21UART2_init(uint32_t baudrate);
void Host_ModbusDap21_send_data(uint8_t *buf, uint8_t len);
void Host_ModbusDap21_Init(void);
uint16_t  Host_ModbusDap21_CRC16( uint8_t *puchMsg, uint16_t usDataLen );
// Host
void HostDap21_Read03_slave(uint8_t slave,uint16_t StartAddr,uint16_t num);

void HostDap21_Func3(uint8_t slave);

void HOST_ModbusDap21RX(void);

void HostDap21_write06_slave(uint8_t slave,uint8_t fun,uint16_t StartAddr,uint16_t num);

void HostDap21_Func6(void);
#endif
 
