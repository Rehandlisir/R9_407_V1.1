/**
 ****************************************************************************************************
 * @file        slavemodbus.c
 * @author      R9团队
 * @version     V1.0
 * @date        2021-10-23
 * @brief       Modbus 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 探索者 F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20211023
 * 第一次发布
 *
 ****************************************************************************************************
 */


#include "./SYSTEM/delay/delay.h"
#include "./BSP/R9/Slavemodbus.h"

UART_HandleTypeDef g_slavemodbus_handler;     /* Modbus控制句柄(串口) */

uint16_t g_slaveReg[5000] ={0x0000};//g_slaveReg是提前定义好的寄存器和寄存器数据，要读取和改写的部分内容
 

void SlaveModbus_UX_IRQHandler(void)
{
    uint8_t res;

    if ((__HAL_UART_GET_FLAG(&g_slavemodbus_handler, UART_FLAG_RXNE) != RESET)) /* 接收到数据 */
    {
        HAL_UART_Receive(&g_slavemodbus_handler, &res, 1, 1000);

		if( slavemodbus.reflag==1)  //有数据包正在处理
		{
		   return ;
		}		
		slavemodbus.rcbuf[slavemodbus.recount++] = res;
		slavemodbus.timout = 0;
		if(slavemodbus.recount == 1)  //已经收到了第二个字符数据
		{
		  slavemodbus.timrun = 1;  //开启slavemodbus定时器计时
		}		
		
		
    }
}



/**
 * @brief       SlaveModbus初始化函数
 *   @note      该函数主要是初始化串口
 * @param       baudrate: 波特率, 根据自己需要设置波特率值
 * @retval      无
 */
void SlavemodbusUART6_init(uint32_t baudrate)
{
    /* IO 及 时钟配置 */
    SlaveModbus_RE_GPIO_CLK_ENABLE(); /* 使能 SlaveModbus_RE 脚时钟 */
    SlaveModbus_TX_GPIO_CLK_ENABLE(); /* 使能 串口TX脚 时钟 */
    SlaveModbus_RX_GPIO_CLK_ENABLE(); /* 使能 串口RX脚 时钟 */
    SlaveModbus_UX_CLK_ENABLE();      /* 使能 串口 时钟 */

    GPIO_InitTypeDef gpio_initure;
    gpio_initure.Pin = SlaveModbus_TX_GPIO_PIN;
    gpio_initure.Mode = GPIO_MODE_AF_PP;
    gpio_initure.Pull = GPIO_PULLUP;
    gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_initure.Alternate = GPIO_AF8_USART6;               /* 复用为串口2 */
    HAL_GPIO_Init(SlaveModbus_TX_GPIO_PORT, &gpio_initure);       /* 串口TX 脚 模式设置 */

    gpio_initure.Pin = SlaveModbus_RX_GPIO_PIN;
    HAL_GPIO_Init(SlaveModbus_RX_GPIO_PORT, &gpio_initure);       /* 串口RX 脚 必须设置成输入模式 */

    gpio_initure.Pin = SlaveModbus_RE_GPIO_PIN;
    gpio_initure.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_initure.Pull = GPIO_PULLUP;
    gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SlaveModbus_RE_GPIO_PORT, &gpio_initure);       /* SlaveModbus_RE 脚 模式设置 */

    /* USART 初始化设置 */
    g_slavemodbus_handler.Instance = SlaveModbus_UX;                    /* 选择485对应的串口 */
    g_slavemodbus_handler.Init.BaudRate = baudrate;               /* 波特率 */
    g_slavemodbus_handler.Init.WordLength = UART_WORDLENGTH_8B;   /* 字长为8位数据格式 */
    g_slavemodbus_handler.Init.StopBits = UART_STOPBITS_1;        /* 一个停止位 */
    g_slavemodbus_handler.Init.Parity = UART_PARITY_NONE;         /* 无奇偶校验位 */
    g_slavemodbus_handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;   /* 无硬件流控 */
    g_slavemodbus_handler.Init.Mode = UART_MODE_TX_RX;            /* 收发模式 */
    HAL_UART_Init(&g_slavemodbus_handler);                        /* 使能对应的串口, 但会调用MSp */
    __HAL_UART_DISABLE_IT(&g_slavemodbus_handler, UART_IT_TC);


      /* 使能接收中断 */
    __HAL_UART_ENABLE_IT(&g_slavemodbus_handler, UART_IT_RXNE);   /* 开启接收中断 */
    HAL_NVIC_EnableIRQ(SlaveModbus_UX_IRQn);                      /* 使能USART1中断 */
    HAL_NVIC_SetPriority(SlaveModbus_UX_IRQn, 1, 2);              /* 抢占优先级3，子优先级3 */


    SlaveModbus_RE(0); /* 默认为接收模式 */
}

/**
 * @brief       SlaveModbus发送len个字节
 * @param       buf     : 发送区首地址
 * @param       len     : 发送的字节数(为了和本代码的接收匹配,这里建议不要超过 SlaveModbus_REC_LEN 个字节)
 * @retval      无
 */
void SlaveModbus_send_data(uint8_t *buf, uint8_t len)
{
    SlaveModbus_RE(1);                                            /* 进入发送模式 */
    HAL_UART_Transmit(&g_slavemodbus_handler, buf, len, 1000);    /* 串口2发送数据 */
//    g_SlaveModbus_rx_cnt = 0;
    SlaveModbus_RE(0); /* 进入接收模式 */
}
/******************************************************************************************/

SLAVEMODBUS slavemodbus;//结构体变量


//作为从机部分的代码
// SlaveModbus初始化函数
void SlaveModbus_Init()
{
  SlavemodbusUART6_init(9600);
  slavemodbus.myadd = 0x01; //从机设备地址为1
  slavemodbus.timrun = 0;    //slavemodbus定时器停止计算
  
}
// Modbus 3号功能码函数
// Modbus 主机读取寄存器值
void SlaveModbus_Func3()
{
	uint16_t Regadd,Reglen,crc;
	uint8_t i,j;	
	//得到要读取寄存器的首地址
	Regadd = slavemodbus.rcbuf[2]*256+slavemodbus.rcbuf[3];//读取的首地址
	//得到要读取寄存器的数据长度
	Reglen = slavemodbus.rcbuf[4]*256+slavemodbus.rcbuf[5];//读取的寄存器个数
	//发送回应数据包
	i = 0;
	slavemodbus.sendbuf[i++] = slavemodbus.myadd;      //ID号：发送本机设备地址
	slavemodbus.sendbuf[i++] = 0x03;              //发送功能码
	slavemodbus.sendbuf[i++] = ((Reglen*2)%256);   //返回字节个数
	for(j=0;j<Reglen;j++)                    //返回数据
	{
		//g_slaveReg是提前定义好的16位数组（模仿寄存器）
	  slavemodbus.sendbuf[i++] = g_slaveReg[Regadd+j]/256;//高位数据
	  slavemodbus.sendbuf[i++] = g_slaveReg[Regadd+j]%256;//低位数据
	}
	crc = SlaveModbus_CRC16(slavemodbus.sendbuf,i);    //计算要返回数据的CRC
	slavemodbus.sendbuf[i++] = crc/256;//校验位高位
	slavemodbus.sendbuf[i++] = crc%256;//校验位低位
	//数据包打包完成
	// 开始返回Modbus数据
	
	SlaveModbus_RE(1); //这是开启485发送
	
	for(j=0;j<i;j++)//发送数据
	{
	  SlaveModbus_send_data(&slavemodbus.sendbuf[j],1);	
	}
	SlaveModbus_RE(0);;//这里是关闭485发送
}


// Modbus 6号功能码函数
// Modbus 主机写入寄存器值
void SlaveModbus_Func6()  
{
	uint16_t Regadd;//地址16位
	uint16_t val;//值
	uint16_t i,crc,j;
	i=0;
	Regadd=slavemodbus.rcbuf[2]*256+slavemodbus.rcbuf[3];  //得到要修改的地址 
	val=slavemodbus.rcbuf[4]*256+slavemodbus.rcbuf[5];     //修改后的值（要写入的数据）
	g_slaveReg[Regadd]=val;  //修改本设备相应的寄存器
	
	//以下为回应主机
	slavemodbus.sendbuf[i++]=slavemodbus.myadd;//本设备地址
    slavemodbus.sendbuf[i++]=0x06;        //功能码 
    slavemodbus.sendbuf[i++]=Regadd/256;//写入的地址
	slavemodbus.sendbuf[i++]=Regadd%256;
	slavemodbus.sendbuf[i++]=val/256;//写入的数值
	slavemodbus.sendbuf[i++]=val%256;
	crc=SlaveModbus_CRC16(slavemodbus.sendbuf,i);//获取crc校验位
	slavemodbus.sendbuf[i++]=crc/256;  //crc校验位加入包中
	slavemodbus.sendbuf[i++]=crc%256;
	//数据发送包打包完毕
	SlaveModbus_RE(1);//使能485控制端(启动发送)  
	for(j=0;j<i;j++)
	{
	 SlaveModbus_send_data(&slavemodbus.sendbuf[j],1);
	}
	SlaveModbus_RE(0);//失能485控制端（改为接收）
}

//这是往多个寄存器器中写入数据
//功能码0x10指令即十进制16
void SlaveModbus_Func16()
{
		uint16_t Regadd;//地址16位
		uint16_t Reglen;
		uint16_t i,crc,j;
		
		Regadd=slavemodbus.rcbuf[2]*256+slavemodbus.rcbuf[3];  //要修改内容的起始地址
		Reglen = slavemodbus.rcbuf[4]*256+slavemodbus.rcbuf[5];//读取的寄存器个数
		for(i=0;i<Reglen;i++)//往寄存器中写入数据
		{
			//接收数组的第七位开始是数据
			g_slaveReg[Regadd+i]=slavemodbus.rcbuf[7+i*2]*256+slavemodbus.rcbuf[8+i*2];//对寄存器一次写入数据
		}
		//写入数据完毕，接下来需要进行打包回复数据了
		
		//以下为回应主机内容
		//内容=接收数组的前6位+两位的校验位
		slavemodbus.sendbuf[0]=slavemodbus.rcbuf[0];//本设备地址
		slavemodbus.sendbuf[1]=slavemodbus.rcbuf[1];  //功能码 
		slavemodbus.sendbuf[2]=slavemodbus.rcbuf[2];//写入的地址
		slavemodbus.sendbuf[3]=slavemodbus.rcbuf[3];
		slavemodbus.sendbuf[4]=slavemodbus.rcbuf[4];
		slavemodbus.sendbuf[5]=slavemodbus.rcbuf[5];
		crc=SlaveModbus_CRC16(slavemodbus.sendbuf,6);//获取crc校验位
		slavemodbus.sendbuf[6]=crc/256;  //crc校验位加入包中
		slavemodbus.sendbuf[7]=crc%256;
		//数据发送包打包完毕
		
		SlaveModbus_RE(1);;//使能485控制端(启动发送)  
		for(j=0;j<8;j++)
		{
			SlaveModbus_send_data(&slavemodbus.sendbuf[j],1);
		}
		SlaveModbus_RE(0);//失能485控制端（改为接收）
}


// Modbus事件处理函数
void SlaveModbus_Event()
{
	uint16_t crc,rccrc;//crc和接收到的crc
	//没有收到数据包
  if(slavemodbus.reflag == 0)  //如果接收未完成则返回空
	{
	   return;
	}
	//收到数据包(接收完成)
	//通过读到的数据帧计算CRC
	//参数1是数组首地址，参数2是要计算的长度（除了CRC校验位其余全算）
	crc = SlaveModbus_CRC16(&slavemodbus.rcbuf[0],slavemodbus.recount-2); //获取CRC校验位
	// 读取数据帧的CRC
	rccrc = slavemodbus.rcbuf[slavemodbus.recount-2]*256+slavemodbus.rcbuf[slavemodbus.recount-1];//计算读取的CRC校验位
	//等价于下面这条语句
	//rccrc=slavemodbus.rcbuf[slavemodbus.recount-1]|(((uint16_t)slavemodbus.rcbuf[slavemodbus.recount-2])<<8);//获取接收到的CRC
	if(crc == rccrc) //CRC检验成功 开始分析包
	{	
	   if(slavemodbus.rcbuf[0] == slavemodbus.myadd)  // 检查地址是否时自己的地址
		 {
		   switch(slavemodbus.rcbuf[1])   //分析slavemodbus功能码
			 {
			   case 0:             break;
				 case 1:             break;
				 case 2:             break;
				 case 3:      SlaveModbus_Func3();      break;//这是读取寄存器的数据
				 case 4:             break;
				 case 5:             break;
				 case 6:      SlaveModbus_Func6();      break;//这是写入单个寄存器数据
				 case 7:             break;
				 case 8:             break;
				 case 9:             break;
				 case 16:     SlaveModbus_Func16(); 			break;//写入多个寄存器数据
			 }
		 }
		 else if(slavemodbus.rcbuf[0] == 0) //广播地址不予回应
		 {
		    
		 }	 
	}	
	 slavemodbus.recount = 0;//接收计数清零
   slavemodbus.reflag = 0; //接收标志清零
}

/* CRC 高位字节值表 */
const uint8_t auchCRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
} ;
/* CRC低位字节值表*/
const uint8_t auchCRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
} ;


/******************************************************************
功能: CRC16校验
输入:
输出:
******************************************************************/
uint16_t  SlaveModbus_CRC16( uint8_t *puchMsg, uint16_t usDataLen )
{
    uint8_t uchCRCHi = 0xFF ; // 高CRC字节初始化
    uint8_t uchCRCLo = 0xFF ; // 低CRC 字节初始化
    unsigned long uIndex ; 		// CRC循环中的索引

    while ( usDataLen-- ) 	// 传输消息缓冲区
    {
        uIndex = uchCRCHi ^ *puchMsg++ ; 	// 计算CRC
        uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
        uchCRCLo = auchCRCLo[uIndex] ;
    }

    return ( uchCRCHi << 8 | uchCRCLo )  ;
}





