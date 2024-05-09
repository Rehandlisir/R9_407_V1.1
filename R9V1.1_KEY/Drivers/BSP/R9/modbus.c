/**
 ****************************************************************************************************
 * @file        modbus.c
 * @author      R9�Ŷ�
 * @version     V1.0
 * @date        2021-10-23
 * @brief       Modbus ��������
 * @license    
 ****************************************************************************************************
 * @attention

 *
 ****************************************************************************************************
 */


#include "./SYSTEM/delay/delay.h"
#include "./BSP/R9/modbus.h"

UART_HandleTypeDef g_modbus_handler;     /* Modbus���ƾ��(����) */

uint16_t KeyReg[200] ={0
           };//reg����ǰ����õļĴ����ͼĴ������ݣ�Ҫ��ȡ�͸�д�Ĳ�������
 

void ModbusKey_UX_IRQHandler(void)
{
    uint8_t res;

    if ((__HAL_UART_GET_FLAG(&g_modbus_handler, UART_FLAG_RXNE) != RESET)) /* ���յ����� */
    {
        HAL_UART_Receive(&g_modbus_handler, &res, 1, 1000);

		if( modbus.reflag==1)  //�����ݰ����ڴ���
		{
		   return ;
		}		
		modbus.rcbuf[modbus.recount++] = res;
		modbus.timout = 0;
		if(modbus.recount == 1)  //�Ѿ��յ��˵ڶ����ַ�����
		{
		  modbus.timrun = 1;  //����modbus��ʱ����ʱ
		}		
		
		
    }
}



/**
 * @brief       Modbus��ʼ������
 *   @note      �ú�����Ҫ�ǳ�ʼ������
 * @param       baudrate: ������, �����Լ���Ҫ���ò�����ֵ
 * @retval      ��
 */
void ModbusKeyUART6_init(uint32_t baudrate)
{
    /* IO �� ʱ������ */
    Modbus_RE_GPIO_CLK_ENABLE(); /* ʹ�� Modbus_RE ��ʱ�� */
    Modbus_TX_GPIO_CLK_ENABLE(); /* ʹ�� ����TX�� ʱ�� */
    Modbus_RX_GPIO_CLK_ENABLE(); /* ʹ�� ����RX�� ʱ�� */
    Modbus_UX_CLK_ENABLE();      /* ʹ�� ���� ʱ�� */

    GPIO_InitTypeDef gpio_initure;
    gpio_initure.Pin = Modbus_TX_GPIO_PIN;
    gpio_initure.Mode = GPIO_MODE_AF_PP;
    gpio_initure.Pull = GPIO_PULLUP;
    gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_initure.Alternate = GPIO_AF8_USART6;               /* ����Ϊ����6 */
    HAL_GPIO_Init(Modbus_TX_GPIO_PORT, &gpio_initure);       /* ����TX �� ģʽ���� */

    gpio_initure.Pin = Modbus_RX_GPIO_PIN;
    HAL_GPIO_Init(Modbus_RX_GPIO_PORT, &gpio_initure);       /* ����RX �� �������ó�����ģʽ */

    gpio_initure.Pin = Modbus_RE_GPIO_PIN;
    gpio_initure.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_initure.Pull = GPIO_PULLUP;
    gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(Modbus_RE_GPIO_PORT, &gpio_initure);       /* Modbus_RE �� ģʽ���� */

    /* USART ��ʼ������ */
    g_modbus_handler.Instance = Modbus_UX;                    /* ѡ��485��Ӧ�Ĵ��� */
    g_modbus_handler.Init.BaudRate = baudrate;               /* ������ */
    g_modbus_handler.Init.WordLength = UART_WORDLENGTH_8B;   /* �ֳ�Ϊ8λ���ݸ�ʽ */
    g_modbus_handler.Init.StopBits = UART_STOPBITS_1;        /* һ��ֹͣλ */
    g_modbus_handler.Init.Parity = UART_PARITY_NONE;         /* ����żУ��λ */
    g_modbus_handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;   /* ��Ӳ������ */
    g_modbus_handler.Init.Mode = UART_MODE_TX_RX;            /* �շ�ģʽ */
    HAL_UART_Init(&g_modbus_handler);                        /* ʹ�ܶ�Ӧ�Ĵ���, �������MSp */
    __HAL_UART_DISABLE_IT(&g_modbus_handler, UART_IT_TC);


      /* ʹ�ܽ����ж� */
    __HAL_UART_ENABLE_IT(&g_modbus_handler, UART_IT_RXNE);   /* ���������ж� */
    HAL_NVIC_EnableIRQ(Modbus_UX_IRQn);                      /* ʹ��USART1�ж� */
    HAL_NVIC_SetPriority(Modbus_UX_IRQn, 3, 3);              /* ��ռ���ȼ�3�������ȼ�3 */


    Modbus_RE(0); /* Ĭ��Ϊ����ģʽ */
}

/**
 * @brief       Modbus����len���ֽ�
 * @param       buf     : �������׵�ַ
 * @param       len     : ���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ���� Modbus_REC_LEN ���ֽ�)
 * @retval      ��
 */
void modbuskey_send_data(uint8_t *buf, uint8_t len)
{
    Modbus_RE(1);                                            /* ���뷢��ģʽ */
    HAL_UART_Transmit(&g_modbus_handler, buf, len, 1000);    /* ����6�������� */
//    g_Modbus_rx_cnt = 0;
    Modbus_RE(0); /* �������ģʽ */
}
/******************************************************************************************/

MODBUS modbus;//�ṹ�����


//��Ϊ�ӻ����ֵĴ���
// Modbus��ʼ������
void Modbuskey_Init(void)
{
  ModbusKeyUART6_init(115200);
  modbus.myadd = 0x11; //�ӻ��豸��ַΪ11
  modbus.timrun = 0;    //modbus��ʱ��ֹͣ����
  
}
// Modbus 3�Ź����뺯��
// Modbus ������ȡ�Ĵ���ֵ
void Modbuskey_Func3(void)
{
	uint16_t Regadd,Reglen,crc;
	uint8_t i,j;	
	//�õ�Ҫ��ȡ�Ĵ������׵�ַ
	Regadd = modbus.rcbuf[2]*256+modbus.rcbuf[3];//��ȡ���׵�ַ
	//�õ�Ҫ��ȡ�Ĵ��������ݳ���
	Reglen = modbus.rcbuf[4]*256+modbus.rcbuf[5];//��ȡ�ļĴ�������
	
	//���ͻ�Ӧ���ݰ�
	i = 0;
	modbus.sendbuf[i++] = modbus.myadd;      //ID�ţ����ͱ����豸��ַ
	modbus.sendbuf[i++] = 0x03;              //���͹�����
	modbus.sendbuf[i++] = ((Reglen*2)%256);   //�����ֽڸ���
	for(j=0;j<Reglen;j++)                    //��������
	{
		//reg����ǰ����õ�16λ���飨ģ�¼Ĵ�����
	  modbus.sendbuf[i++] = KeyReg[Regadd+j]/256;//��λ����
	  modbus.sendbuf[i++] = KeyReg[Regadd+j]%256;//��λ����
	}
	crc = Modbuskey_CRC16(modbus.sendbuf,i);    //����Ҫ�������ݵ�CRC
	modbus.sendbuf[i++] = crc/256;//У��λ��λ
	modbus.sendbuf[i++] = crc%256;//У��λ��λ
	//���ݰ�������
	// ��ʼ����Modbus����
	
	Modbus_RE(1); //���ǿ���485����
	
	for(j=0;j<i;j++)//��������
	{
	  modbuskey_send_data(&modbus.sendbuf[j],1);	
	}
	Modbus_RE(0);;//�����ǹر�485����
}


// Modbus 6�Ź����뺯��
// Modbus ����д��Ĵ���ֵ
void Modbuskey_Func6(void)  
{
	uint16_t Regadd;//��ַ16λ
	uint16_t val;//ֵ
	uint16_t i,crc,j;
	i=0;
	Regadd=modbus.rcbuf[2]*256+modbus.rcbuf[3];  //�õ�Ҫ�޸ĵĵ�ַ 
	val=modbus.rcbuf[4]*256+modbus.rcbuf[5];     //�޸ĺ��ֵ��Ҫд������ݣ�
	KeyReg[Regadd]=val;  //�޸ı��豸��Ӧ�ļĴ���
	
	//����Ϊ��Ӧ����
	modbus.sendbuf[i++]=modbus.myadd;//���豸��ַ
    modbus.sendbuf[i++]=0x06;        //������ 
    modbus.sendbuf[i++]=Regadd/256;//д��ĵ�ַ
	modbus.sendbuf[i++]=Regadd%256;
	modbus.sendbuf[i++]=val/256;//д�����ֵ
	modbus.sendbuf[i++]=val%256;
	crc=Modbuskey_CRC16(modbus.sendbuf,i);//��ȡcrcУ��λ
	modbus.sendbuf[i++]=crc/256;  //crcУ��λ�������
	modbus.sendbuf[i++]=crc%256;
	//���ݷ��Ͱ�������
	Modbus_RE(1);;//ʹ��485���ƶ�(��������)  
	for(j=0;j<i;j++)
	{
	 modbuskey_send_data(&modbus.sendbuf[j],1);
	}
	Modbus_RE(0);//ʧ��485���ƶˣ���Ϊ���գ�
}

//����������Ĵ�������д������
//������0x10ָ�ʮ����16
void Modbuskey_Func16(void)
{
		uint16_t Regadd;//��ַ16λ
		uint16_t Reglen;
		uint16_t i,crc,j;
		
		Regadd=modbus.rcbuf[2]*256+modbus.rcbuf[3];  //Ҫ�޸����ݵ���ʼ��ַ
		Reglen = modbus.rcbuf[4]*256+modbus.rcbuf[5];//��ȡ�ļĴ�������
		for(i=0;i<Reglen;i++)//���Ĵ�����д������
		{
			//��������ĵ���λ��ʼ������
			KeyReg[Regadd+i]=modbus.rcbuf[7+i*2]*256+modbus.rcbuf[8+i*2];//�ԼĴ���һ��д������
		}
		//д��������ϣ���������Ҫ���д���ظ�������
		
		//����Ϊ��Ӧ��������
		//����=���������ǰ6λ+��λ��У��λ
		modbus.sendbuf[0]=modbus.rcbuf[0];//���豸��ַ
		modbus.sendbuf[1]=modbus.rcbuf[1];  //������ 
		modbus.sendbuf[2]=modbus.rcbuf[2];//д��ĵ�ַ
		modbus.sendbuf[3]=modbus.rcbuf[3];
		modbus.sendbuf[4]=modbus.rcbuf[4];
		modbus.sendbuf[5]=modbus.rcbuf[5];
		crc=Modbuskey_CRC16(modbus.sendbuf,6);//��ȡcrcУ��λ
		modbus.sendbuf[6]=crc/256;  //crcУ��λ�������
		modbus.sendbuf[7]=crc%256;
		//���ݷ��Ͱ�������
		
		Modbus_RE(1);;//ʹ��485���ƶ�(��������)  
		for(j=0;j<8;j++)
		{
			modbuskey_send_data(&modbus.sendbuf[j],1);
		}
		Modbus_RE(0);//ʧ��485���ƶˣ���Ϊ���գ�
}


// Modbus�¼�������
void Modbuskey_Event(void)
{
	uint16_t crc,rccrc;//crc�ͽ��յ���crc
	//û���յ����ݰ�
  if(modbus.reflag == 0)  //�������δ����򷵻ؿ�
	{
	   return;
	}
	//�յ����ݰ�(�������)
	//ͨ������������֡����CRC
	//����1�������׵�ַ������2��Ҫ����ĳ��ȣ�����CRCУ��λ����ȫ�㣩
	crc = Modbuskey_CRC16(&modbus.rcbuf[0],modbus.recount-2); //��ȡCRCУ��λ
	// ��ȡ����֡��CRC
	rccrc = modbus.rcbuf[modbus.recount-2]*256+modbus.rcbuf[modbus.recount-1];//�����ȡ��CRCУ��λ
	//�ȼ��������������
	//rccrc=modbus.rcbuf[modbus.recount-1]|(((uint16_t)modbus.rcbuf[modbus.recount-2])<<8);//��ȡ���յ���CRC
	if(crc == rccrc) //CRC����ɹ� ��ʼ������
	{	
	   if(modbus.rcbuf[0] == modbus.myadd)  // ����ַ�Ƿ�ʱ�Լ��ĵ�ַ
		 {
		   switch(modbus.rcbuf[1])   //����modbus������
			 {
			   case 0:             break;
				 case 1:             break;
				 case 2:             break;
				 case 3:      Modbuskey_Func3();      break;//���Ƕ�ȡ�Ĵ���������
				 case 4:             break;
				 case 5:             break;
				 case 6:      Modbuskey_Func6();      break;//����д�뵥���Ĵ�������
				 case 7:             break;
				 case 8:             break;
				 case 9:             break;
				 case 16:     Modbuskey_Func16(); 			break;//д�����Ĵ�������
			 }
		 }
		 else if(modbus.rcbuf[0] == 0) //�㲥��ַ�����Ӧ
		 {
		    
		 }	 
	}	
	 modbus.recount = 0;//���ռ�������
   modbus.reflag = 0; //���ձ�־����
}


/******************************************************************
����: CRC16У��
����:
���:
******************************************************************/
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
uint16_t  Modbuskey_CRC16( uint8_t *puchMsg, uint16_t usDataLen )
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

    return ( uchCRCHi << 8 | uchCRCLo ) ;
}





