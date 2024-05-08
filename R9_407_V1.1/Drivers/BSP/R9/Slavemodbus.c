/**
 ****************************************************************************************************
 * @file        slavemodbus.c
 * @author      R9�Ŷ�
 * @version     V1.0
 * @date        2021-10-23
 * @brief       Modbus ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ̽���� F407������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20211023
 * ��һ�η���
 *
 ****************************************************************************************************
 */


#include "./SYSTEM/delay/delay.h"
#include "./BSP/R9/Slavemodbus.h"

UART_HandleTypeDef g_slavemodbus_handler;     /* Modbus���ƾ��(����) */

uint16_t Reg[5000] ={0x0000
           };//reg����ǰ����õļĴ����ͼĴ������ݣ�Ҫ��ȡ�͸�д�Ĳ�������
 

void SlaveModbus_UX_IRQHandler(void)
{
    uint8_t res;

    if ((__HAL_UART_GET_FLAG(&g_slavemodbus_handler, UART_FLAG_RXNE) != RESET)) /* ���յ����� */
    {
        HAL_UART_Receive(&g_slavemodbus_handler, &res, 1, 1000);

		if( slavemodbus.reflag==1)  //�����ݰ����ڴ���
		{
		   return ;
		}		
		slavemodbus.rcbuf[slavemodbus.recount++] = res;
		slavemodbus.timout = 0;
		if(slavemodbus.recount == 1)  //�Ѿ��յ��˵ڶ����ַ�����
		{
		  slavemodbus.timrun = 1;  //����slavemodbus��ʱ����ʱ
		}		
		
		
    }
}



/**
 * @brief       SlaveModbus��ʼ������
 *   @note      �ú�����Ҫ�ǳ�ʼ������
 * @param       baudrate: ������, �����Լ���Ҫ���ò�����ֵ
 * @retval      ��
 */
void SlavemodbusUART2_init(uint32_t baudrate)
{
    /* IO �� ʱ������ */
    SlaveModbus_RE_GPIO_CLK_ENABLE(); /* ʹ�� SlaveModbus_RE ��ʱ�� */
    SlaveModbus_TX_GPIO_CLK_ENABLE(); /* ʹ�� ����TX�� ʱ�� */
    SlaveModbus_RX_GPIO_CLK_ENABLE(); /* ʹ�� ����RX�� ʱ�� */
    SlaveModbus_UX_CLK_ENABLE();      /* ʹ�� ���� ʱ�� */

    GPIO_InitTypeDef gpio_initure;
    gpio_initure.Pin = SlaveModbus_TX_GPIO_PIN;
    gpio_initure.Mode = GPIO_MODE_AF_PP;
    gpio_initure.Pull = GPIO_PULLUP;
    gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_initure.Alternate = GPIO_AF7_USART2;               /* ����Ϊ����2 */
    HAL_GPIO_Init(SlaveModbus_TX_GPIO_PORT, &gpio_initure);       /* ����TX �� ģʽ���� */

    gpio_initure.Pin = SlaveModbus_RX_GPIO_PIN;
    HAL_GPIO_Init(SlaveModbus_RX_GPIO_PORT, &gpio_initure);       /* ����RX �� �������ó�����ģʽ */

    gpio_initure.Pin = SlaveModbus_RE_GPIO_PIN;
    gpio_initure.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_initure.Pull = GPIO_PULLUP;
    gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SlaveModbus_RE_GPIO_PORT, &gpio_initure);       /* SlaveModbus_RE �� ģʽ���� */

    /* USART ��ʼ������ */
    g_slavemodbus_handler.Instance = SlaveModbus_UX;                    /* ѡ��485��Ӧ�Ĵ��� */
    g_slavemodbus_handler.Init.BaudRate = baudrate;               /* ������ */
    g_slavemodbus_handler.Init.WordLength = UART_WORDLENGTH_8B;   /* �ֳ�Ϊ8λ���ݸ�ʽ */
    g_slavemodbus_handler.Init.StopBits = UART_STOPBITS_1;        /* һ��ֹͣλ */
    g_slavemodbus_handler.Init.Parity = UART_PARITY_NONE;         /* ����żУ��λ */
    g_slavemodbus_handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;   /* ��Ӳ������ */
    g_slavemodbus_handler.Init.Mode = UART_MODE_TX_RX;            /* �շ�ģʽ */
    HAL_UART_Init(&g_slavemodbus_handler);                        /* ʹ�ܶ�Ӧ�Ĵ���, �������MSp */
    __HAL_UART_DISABLE_IT(&g_slavemodbus_handler, UART_IT_TC);


      /* ʹ�ܽ����ж� */
    __HAL_UART_ENABLE_IT(&g_slavemodbus_handler, UART_IT_RXNE);   /* ���������ж� */
    HAL_NVIC_EnableIRQ(SlaveModbus_UX_IRQn);                      /* ʹ��USART1�ж� */
    HAL_NVIC_SetPriority(SlaveModbus_UX_IRQn, 1, 2);              /* ��ռ���ȼ�3�������ȼ�3 */


    SlaveModbus_RE(0); /* Ĭ��Ϊ����ģʽ */
}

/**
 * @brief       SlaveModbus����len���ֽ�
 * @param       buf     : �������׵�ַ
 * @param       len     : ���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ���� SlaveModbus_REC_LEN ���ֽ�)
 * @retval      ��
 */
void SlaveModbus_send_data(uint8_t *buf, uint8_t len)
{
    SlaveModbus_RE(1);                                            /* ���뷢��ģʽ */
    HAL_UART_Transmit(&g_slavemodbus_handler, buf, len, 1000);    /* ����2�������� */
//    g_SlaveModbus_rx_cnt = 0;
    SlaveModbus_RE(0); /* �������ģʽ */
}
/******************************************************************************************/

SLAVEMODBUS slavemodbus;//�ṹ�����


//��Ϊ�ӻ����ֵĴ���
// SlaveModbus��ʼ������
void SlaveModbus_Init()
{
  SlavemodbusUART2_init(9600);
  slavemodbus.myadd = 0x01; //�ӻ��豸��ַΪ1
  slavemodbus.timrun = 0;    //slavemodbus��ʱ��ֹͣ����
  
}
// Modbus 3�Ź����뺯��
// Modbus ������ȡ�Ĵ���ֵ
void SlaveModbus_Func3()
{
	uint16_t Regadd,Reglen,crc;
	uint8_t i,j;	
	//�õ�Ҫ��ȡ�Ĵ������׵�ַ
	Regadd = slavemodbus.rcbuf[2]*256+slavemodbus.rcbuf[3];//��ȡ���׵�ַ
	//�õ�Ҫ��ȡ�Ĵ��������ݳ���
	Reglen = slavemodbus.rcbuf[4]*256+slavemodbus.rcbuf[5];//��ȡ�ļĴ�������
	//���ͻ�Ӧ���ݰ�
	i = 0;
	slavemodbus.sendbuf[i++] = slavemodbus.myadd;      //ID�ţ����ͱ����豸��ַ
	slavemodbus.sendbuf[i++] = 0x03;              //���͹�����
	slavemodbus.sendbuf[i++] = ((Reglen*2)%256);   //�����ֽڸ���
	for(j=0;j<Reglen;j++)                    //��������
	{
		//reg����ǰ����õ�16λ���飨ģ�¼Ĵ�����
	  slavemodbus.sendbuf[i++] = Reg[Regadd+j]/256;//��λ����
	  slavemodbus.sendbuf[i++] = Reg[Regadd+j]%256;//��λ����
	}
	crc = SlaveModbus_CRC16(slavemodbus.sendbuf,i);    //����Ҫ�������ݵ�CRC
	slavemodbus.sendbuf[i++] = crc/256;//У��λ��λ
	slavemodbus.sendbuf[i++] = crc%256;//У��λ��λ
	//���ݰ�������
	// ��ʼ����Modbus����
	
	SlaveModbus_RE(1); //���ǿ���485����
	
	for(j=0;j<i;j++)//��������
	{
	  SlaveModbus_send_data(&slavemodbus.sendbuf[j],1);	
	}
	SlaveModbus_RE(0);;//�����ǹر�485����
}


// Modbus 6�Ź����뺯��
// Modbus ����д��Ĵ���ֵ
void SlaveModbus_Func6()  
{
	uint16_t Regadd;//��ַ16λ
	uint16_t val;//ֵ
	uint16_t i,crc,j;
	i=0;
	Regadd=slavemodbus.rcbuf[2]*256+slavemodbus.rcbuf[3];  //�õ�Ҫ�޸ĵĵ�ַ 
	val=slavemodbus.rcbuf[4]*256+slavemodbus.rcbuf[5];     //�޸ĺ��ֵ��Ҫд������ݣ�
	Reg[Regadd]=val;  //�޸ı��豸��Ӧ�ļĴ���
	
	//����Ϊ��Ӧ����
	slavemodbus.sendbuf[i++]=slavemodbus.myadd;//���豸��ַ
    slavemodbus.sendbuf[i++]=0x06;        //������ 
    slavemodbus.sendbuf[i++]=Regadd/256;//д��ĵ�ַ
	slavemodbus.sendbuf[i++]=Regadd%256;
	slavemodbus.sendbuf[i++]=val/256;//д�����ֵ
	slavemodbus.sendbuf[i++]=val%256;
	crc=SlaveModbus_CRC16(slavemodbus.sendbuf,i);//��ȡcrcУ��λ
	slavemodbus.sendbuf[i++]=crc/256;  //crcУ��λ�������
	slavemodbus.sendbuf[i++]=crc%256;
	//���ݷ��Ͱ�������
	SlaveModbus_RE(1);//ʹ��485���ƶ�(��������)  
	for(j=0;j<i;j++)
	{
	 SlaveModbus_send_data(&slavemodbus.sendbuf[j],1);
	}
	SlaveModbus_RE(0);//ʧ��485���ƶˣ���Ϊ���գ�
}

//����������Ĵ�������д������
//������0x10ָ�ʮ����16
void SlaveModbus_Func16()
{
		uint16_t Regadd;//��ַ16λ
		uint16_t Reglen;
		uint16_t i,crc,j;
		
		Regadd=slavemodbus.rcbuf[2]*256+slavemodbus.rcbuf[3];  //Ҫ�޸����ݵ���ʼ��ַ
		Reglen = slavemodbus.rcbuf[4]*256+slavemodbus.rcbuf[5];//��ȡ�ļĴ�������
		for(i=0;i<Reglen;i++)//���Ĵ�����д������
		{
			//��������ĵ���λ��ʼ������
			Reg[Regadd+i]=slavemodbus.rcbuf[7+i*2]*256+slavemodbus.rcbuf[8+i*2];//�ԼĴ���һ��д������
		}
		//д��������ϣ���������Ҫ���д���ظ�������
		
		//����Ϊ��Ӧ��������
		//����=���������ǰ6λ+��λ��У��λ
		slavemodbus.sendbuf[0]=slavemodbus.rcbuf[0];//���豸��ַ
		slavemodbus.sendbuf[1]=slavemodbus.rcbuf[1];  //������ 
		slavemodbus.sendbuf[2]=slavemodbus.rcbuf[2];//д��ĵ�ַ
		slavemodbus.sendbuf[3]=slavemodbus.rcbuf[3];
		slavemodbus.sendbuf[4]=slavemodbus.rcbuf[4];
		slavemodbus.sendbuf[5]=slavemodbus.rcbuf[5];
		crc=SlaveModbus_CRC16(slavemodbus.sendbuf,6);//��ȡcrcУ��λ
		slavemodbus.sendbuf[6]=crc/256;  //crcУ��λ�������
		slavemodbus.sendbuf[7]=crc%256;
		//���ݷ��Ͱ�������
		
		SlaveModbus_RE(1);;//ʹ��485���ƶ�(��������)  
		for(j=0;j<8;j++)
		{
			SlaveModbus_send_data(&slavemodbus.sendbuf[j],1);
		}
		SlaveModbus_RE(0);//ʧ��485���ƶˣ���Ϊ���գ�
}


// Modbus�¼�������
void SlaveModbus_Event()
{
	uint16_t crc,rccrc;//crc�ͽ��յ���crc
	//û���յ����ݰ�
  if(slavemodbus.reflag == 0)  //�������δ����򷵻ؿ�
	{
	   return;
	}
	//�յ����ݰ�(�������)
	//ͨ������������֡����CRC
	//����1�������׵�ַ������2��Ҫ����ĳ��ȣ�����CRCУ��λ����ȫ�㣩
	crc = SlaveModbus_CRC16(&slavemodbus.rcbuf[0],slavemodbus.recount-2); //��ȡCRCУ��λ
	// ��ȡ����֡��CRC
	rccrc = slavemodbus.rcbuf[slavemodbus.recount-2]*256+slavemodbus.rcbuf[slavemodbus.recount-1];//�����ȡ��CRCУ��λ
	//�ȼ��������������
	//rccrc=slavemodbus.rcbuf[slavemodbus.recount-1]|(((uint16_t)slavemodbus.rcbuf[slavemodbus.recount-2])<<8);//��ȡ���յ���CRC
	if(crc == rccrc) //CRC����ɹ� ��ʼ������
	{	
	   if(slavemodbus.rcbuf[0] == slavemodbus.myadd)  // ����ַ�Ƿ�ʱ�Լ��ĵ�ַ
		 {
		   switch(slavemodbus.rcbuf[1])   //����slavemodbus������
			 {
			   case 0:             break;
				 case 1:             break;
				 case 2:             break;
				 case 3:      SlaveModbus_Func3();      break;//���Ƕ�ȡ�Ĵ���������
				 case 4:             break;
				 case 5:             break;
				 case 6:      SlaveModbus_Func6();      break;//����д�뵥���Ĵ�������
				 case 7:             break;
				 case 8:             break;
				 case 9:             break;
				 case 16:     SlaveModbus_Func16(); 			break;//д�����Ĵ�������
			 }
		 }
		 else if(slavemodbus.rcbuf[0] == 0) //�㲥��ַ�����Ӧ
		 {
		    
		 }	 
	}	
	 slavemodbus.recount = 0;//���ռ�������
   slavemodbus.reflag = 0; //���ձ�־����
}

/* CRC ��λ�ֽ�ֵ�� */
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
/* CRC��λ�ֽ�ֵ��*/
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
����: CRC16У��
����:
���:
******************************************************************/
uint16_t  SlaveModbus_CRC16( uint8_t *puchMsg, uint16_t usDataLen )
{
    uint8_t uchCRCHi = 0xFF ; // ��CRC�ֽڳ�ʼ��
    uint8_t uchCRCLo = 0xFF ; // ��CRC �ֽڳ�ʼ��
    unsigned long uIndex ; 		// CRCѭ���е�����

    while ( usDataLen-- ) 	// ������Ϣ������
    {
        uIndex = uchCRCHi ^ *puchMsg++ ; 	// ����CRC
        uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
        uchCRCLo = auchCRCLo[uIndex] ;
    }

    return ( uchCRCHi << 8 | uchCRCLo )  ;
}





