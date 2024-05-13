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
#include "./BSP/DAP21/hostdap21.h"

UART_HandleTypeDef g_dap21modbus_handler; /* Modbus���ƾ��(����) */

Data_DAP21 dap21Data; //������ȡ/д��ӻ��Ĵ����� ���ص�����

// �������ܵ�����

void ModbusDap21_UX_IRQHandler(void)
{
	uint8_t res;

	if ((__HAL_UART_GET_FLAG(&g_dap21modbus_handler, UART_FLAG_RXNE) != RESET)) /* ���յ����� */
	{
		HAL_UART_Receive(&g_dap21modbus_handler, &res, 1, 1000);

		if (modbus_dap21.reflag == 1) // �����ݰ����ڴ���
		{
			return;
		}
		modbus_dap21.rcbuf[modbus_dap21.recount++] = res;
		modbus_dap21.timout = 0;
		if (modbus_dap21.recount == 1) // �Ѿ��յ��˵ڶ����ַ�����
		{
			modbus_dap21.timrun = 1; // ����modbus��ʱ����ʱ
		}
	}
}

/**
 * @brief       Modbus��ʼ������
 *   @note      �ú�����Ҫ�ǳ�ʼ������
 * @param       baudrate: ������, �����Լ���Ҫ���ò�����ֵ
 * @retval      ��
 */
void Host_ModbusDap21UART2_init(uint32_t baudrate)
{
	/* IO �� ʱ������ */
	ModbusDap21_RE_GPIO_CLK_ENABLE(); /* ʹ�� Modbus_RE ��ʱ�� */
	ModbusDap21_TX_GPIO_CLK_ENABLE(); /* ʹ�� ����TX�� ʱ�� */
	ModbusDap21_RX_GPIO_CLK_ENABLE(); /* ʹ�� ����RX�� ʱ�� */
	ModbusDap21_UX_CLK_ENABLE();		 /* ʹ�� ���� ʱ�� */

	GPIO_InitTypeDef gpio_initure;
	gpio_initure.Pin = ModbusDap21_TX_GPIO_PIN;
	gpio_initure.Mode = GPIO_MODE_AF_PP;
	gpio_initure.Pull = GPIO_PULLUP;
	gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_initure.Alternate = GPIO_AF7_USART2;		   /* ����Ϊ����2 */
	HAL_GPIO_Init(ModbusDap21_TX_GPIO_PORT, &gpio_initure); /* ����TX �� ģʽ���� */

	gpio_initure.Pin = ModbusDap21_RX_GPIO_PIN;
	HAL_GPIO_Init(ModbusDap21_RX_GPIO_PORT, &gpio_initure); /* ����RX �� �������ó�����ģʽ */

	gpio_initure.Pin = ModbusDap21_RE_GPIO_PIN;
	gpio_initure.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_initure.Pull = GPIO_PULLUP;
	gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ModbusDap21_RE_GPIO_PORT, &gpio_initure); /* Modbus_RE �� ģʽ���� */

	/* USART ��ʼ������ */
	g_dap21modbus_handler.Instance = ModbusDap21_UX;				   /* ѡ��485��Ӧ�Ĵ��� */
	g_dap21modbus_handler.Init.BaudRate = baudrate;			   /* ������ */
	g_dap21modbus_handler.Init.WordLength = UART_WORDLENGTH_8B; /* �ֳ�Ϊ8λ���ݸ�ʽ */
	g_dap21modbus_handler.Init.StopBits = UART_STOPBITS_1;	   /* һ��ֹͣλ */
	g_dap21modbus_handler.Init.Parity = UART_PARITY_NONE;	   /* ����żУ��λ */
	g_dap21modbus_handler.Init.HwFlowCtl = UART_HWCONTROL_NONE; /* ��Ӳ������ */
	g_dap21modbus_handler.Init.Mode = UART_MODE_TX_RX;		   /* �շ�ģʽ */
	HAL_UART_Init(&g_dap21modbus_handler);					   /* ʹ�ܶ�Ӧ�Ĵ���, �������MSp */
	__HAL_UART_DISABLE_IT(&g_dap21modbus_handler, UART_IT_TC);

	/* ʹ�ܽ����ж� */
	__HAL_UART_ENABLE_IT(&g_dap21modbus_handler, UART_IT_RXNE); /* ���������ж� */
	HAL_NVIC_EnableIRQ(ModbusDap21_UX_IRQn);					   /* ʹ��USART1�ж� */
	HAL_NVIC_SetPriority(ModbusDap21_UX_IRQn, 3, 3);			   /* ��ռ���ȼ�3�������ȼ�3 */

	ModbusDap21_RE(0); /* Ĭ��Ϊ����ģʽ */
}

/**
 * @brief       Modbus����len���ֽ�
 * @param       buf     : �������׵�ַ
 * @param       len     : ���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ���� Modbus_REC_LEN ���ֽ�)
 * @retval      ��
 */
void Host_ModbusDap21_send_data(uint8_t *buf, uint8_t len)
{
	ModbusDap21_RE(1);										  /* ���뷢��ģʽ */
	HAL_UART_Transmit(&g_dap21modbus_handler, buf, len, 1000); /* ����5�������� */
														  //   g_Modbus_rx_cnt = 0;
	ModbusDap21_RE(0);										  /* �������ģʽ */
}
/******************************************************************************************/

MODBUS_DAP21 modbus_dap21; // �ṹ�����

// ��Ϊ�ӻ����ֵĴ���
//  Modbus��ʼ������
void Host_ModbusDap21_Init(void)
{
	Host_ModbusDap21UART2_init(115200);

	modbus_dap21.timrun = 0; // modbus��ʱ��ֹͣ����
}

/******************************************************************
����: CRC16У��
����:
���:
******************************************************************/
/* CRC ��λ�ֽ�ֵ�� */
const uint8_t Dap21_auchCRCHi[] = {
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
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40};
/* CRC��λ�ֽ�ֵ��*/
const uint8_t Dap21_auchCRCLo[] = {
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
	0x43, 0x83, 0x41, 0x81, 0x80, 0x40};
uint16_t Host_ModbusDap21_CRC16(uint8_t *puchMsg, uint16_t usDataLen)
{
	uint8_t uchCRCHi = 0xFF; // ��CRC�ֽڳ�ʼ��
	uint8_t uchCRCLo = 0xFF; // ��CRC �ֽڳ�ʼ��
	unsigned long uIndex;	 // CRCѭ���е�����

	while (usDataLen--) // ������Ϣ������
	{
		uIndex = uchCRCHi ^ *puchMsg++; // ����CRC
		uchCRCHi = uchCRCLo ^ Dap21_auchCRCHi[uIndex];
		uchCRCLo = Dap21_auchCRCLo[uIndex];
	}

	return (uchCRCHi << 8 | uchCRCLo);
}

// ���´�����Ϊ����ʱʹ��

// ����ѡ��ӻ�
// ����1�ӻ�������2��ʼ��ַ������3�Ĵ�������
void HostDap21_Read03_slave(uint8_t slave, uint16_t StartAddr, uint16_t num)
{
	int j;
	uint16_t crc; // �����CRCУ��λ
	modbus_dap21.slave_add = slave;
	modbus_dap21.Host_Txbuf[0] = slave;					 // ����Ҫƥ��Ĵӻ���ַ
	modbus_dap21.Host_Txbuf[1] = 0x03;					 // ������
	modbus_dap21.Host_Txbuf[2] = StartAddr / 256;			 // ��ʼ��ַ��λ
	modbus_dap21.Host_Txbuf[3] = StartAddr % 256;			 // ��ʼ��ַ  ��λ
	modbus_dap21.Host_Txbuf[4] = num / 256;				 // �Ĵ���������λ
	modbus_dap21.Host_Txbuf[5] = num % 256;				 // �Ĵ���������λ
	crc = Host_ModbusDap21_CRC16(&modbus_dap21.Host_Txbuf[0], 6); // ��ȡCRCУ��λ
	modbus_dap21.Host_Txbuf[6] = crc / 256;				 // �Ĵ���������λ
	modbus_dap21.Host_Txbuf[7] = crc % 256;				 // �Ĵ���������λ

	// ���������ݴ�����
	ModbusDap21_RE(1);
//	; // ʹ��485���ƶ�(��������)
//	//	HAL_Delay(100);
	for (j = 0; j < 8; j++)
	{
		Host_ModbusDap21_send_data(&modbus_dap21.Host_Txbuf[j], 1);
	}
	ModbusDap21_RE(0);			   // ʧ��485���ƶˣ���Ϊ���գ�
	modbus_dap21.Host_send_flag = 1; // ��ʾ�����������
}

// ���������ֽڸ���
// �����������ܵ�������
void HostDap21_Func3(uint8_t slave)
{
  if (slave ==0x01)
	{
		dap21Data.slav1data[0] = modbus_dap21.rcbuf[4];
		dap21Data.slav1data[1] = modbus_dap21.rcbuf[3];
		
		dap21Data.dyplength1 = ((*((uint8_t *)dap21Data.slav1data+ 1)<< 8))| *(uint8_t *)dap21Data.slav1data;

		modbus_dap21.Host_End = 1; // ���յ����ݴ������
	}
	if (slave ==0x02)
	{
		dap21Data.slav1data[0] = modbus_dap21.rcbuf[4];
		dap21Data.slav1data[1] = modbus_dap21.rcbuf[3];
		
		dap21Data.dyplength2 = ((*((uint8_t *)dap21Data.slav2data+ 1)<< 8))| *(uint8_t *)dap21Data.slav2data;

		modbus_dap21.Host_End = 1; // ���յ����ݴ������
	}
	
	if (slave ==0x03)
	{
		dap21Data.slav1data[0] = modbus_dap21.rcbuf[4];
		dap21Data.slav1data[1] = modbus_dap21.rcbuf[3];
		
		dap21Data.dyplength3 = ((*((uint8_t *)dap21Data.slav3data+ 1)<< 8))| *(uint8_t *)dap21Data.slav3data;

		modbus_dap21.Host_End = 1; // ���յ����ݴ������

	}
	if (slave ==0x04)
	{
		dap21Data.slav1data[0] = modbus_dap21.rcbuf[4];
		dap21Data.slav1data[1] = modbus_dap21.rcbuf[3];
		
		dap21Data.dyplength4 = ((*((uint8_t *)dap21Data.slav4data+ 1)<< 8))| *(uint8_t *)dap21Data.slav4data;

		modbus_dap21.Host_End = 1; // ���յ����ݴ������
	}

}

// �������մӻ�����Ϣ���д���
void HOST_ModbusDap21RX()
{
	uint16_t crc, rccrc; // ����crc�ͽ��յ���crc

	//	printf("0000");
	if (modbus_dap21.reflag == 0) // �������δ����򷵻ؿ�
	{
		//		printf("55555");
		return;
	}
	// �������ݽ���

	// �������г��������λCRCУ��λ����ȫ�㣩
	crc = Host_ModbusDap21_CRC16(&modbus_dap21.rcbuf[0], modbus_dap21.recount - 2);					   // ��ȡCRCУ��λ
	rccrc = modbus_dap21.rcbuf[modbus_dap21.recount - 2] * 256 + modbus_dap21.rcbuf[modbus_dap21.recount - 1]; // �����ȡ��CRCУ��λ

	if (crc == rccrc) // CRC����ɹ� ��ʼ������
	{
		if (modbus_dap21.rcbuf[0] == modbus_dap21.slave_add) // ����ַ���Ƕ�Ӧ�ӻ���������
		{
			if (modbus_dap21.rcbuf[1] == 3) // ������ʱ03

				HostDap21_Func3(modbus_dap21.slave_add); // ���Ƕ�ȡ�Ĵ�������Ч����λ���м���
		}
	}
	modbus_dap21.recount = 0; // ���ռ�������
	modbus_dap21.reflag = 0;	// ���ձ�־����
}

// ��һ���Ĵ�����д���ݵĲ�������
void HostDap21_write06_slave(uint8_t slave, uint8_t fun, uint16_t StartAddr, uint16_t num)
{
	uint16_t crc, j;						// �����CRCУ��λ
	modbus_dap21.slave_add = slave;				// �ӻ���ַ��ֵһ�£���������
	modbus_dap21.Host_Txbuf[0] = slave;			// ����Ҫƥ��Ĵӻ���ַ
	modbus_dap21.Host_Txbuf[1] = fun;				// ������
	modbus_dap21.Host_Txbuf[2] = StartAddr / 256; // ��ʼ��ַ��λ
	modbus_dap21.Host_Txbuf[3] = StartAddr % 256; // ��ʼ��ַ��λ
	modbus_dap21.Host_Txbuf[4] = num / 256;
	modbus_dap21.Host_Txbuf[5] = num % 256;
	crc = Host_ModbusDap21_CRC16(&modbus_dap21.Host_Txbuf[0], 6); // ��ȡCRCУ��λ
	modbus_dap21.Host_Txbuf[6] = crc / 256;				 // �Ĵ���������λ
	modbus_dap21.Host_Txbuf[7] = crc % 256;				 // �Ĵ���������λ

	// �������ݰ�װ���
	//Modbus_RE(1); // ʹ��485���ƶ�(��������)
//	delay_ms(5);
	for (j = 0; j < 8; j++)
	{
		Host_ModbusDap21_send_data(&modbus_dap21.Host_Txbuf[j], 1);
	}
//	delay_ms(5);
//	Modbus_RE(0);						   // ʧ��485���ƶˣ���Ϊ���գ�
	modbus_dap21.Host_send_flag = 1; // ��ʾ�����������
}

// �ӻ���������
void HostDap21_Func6()
{
	int crc, rccrc;
	crc = Host_ModbusDap21_CRC16(&modbus_dap21.rcbuf[0], 6);		 // ��ȡCRCУ��λ
	rccrc = modbus_dap21.rcbuf[6] * 256 + modbus_dap21.rcbuf[7]; // �����ȡ��CRCУ��λ
	if (crc == rccrc)								 // CRC����ɹ� ��ʼ������
	{
		if (modbus_dap21.rcbuf[0] == modbus_dap21.slave_add) // ����ַ���Ƕ�Ӧ�ӻ���������
		{
			if (modbus_dap21.rcbuf[1] == 6) // ������ʱ06
			{

				;// printf("��ַΪ %d �Ĵӻ��Ĵ��� %d ��д������ %d \r\n ", modbus.rcbuf[0], modbus.rcbuf[3] + (modbus.rcbuf[2]) * 256, modbus.rcbuf[5] + (modbus.rcbuf[4]) * 256);
				// printf("Host_06 write data right!\r\n");
			}
		}
	}

	modbus_dap21.Host_End = 1; // ���յ����ݴ������
}