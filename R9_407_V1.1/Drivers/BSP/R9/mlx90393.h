#ifndef __MLX90393_H_
#define __MLX90393_H_

#include "./SYSTEM/sys/sys.h"

//IO��������
#define MLX90393_SDA_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9����ģʽ
#define MLX90393_SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9���ģʽ
//IO����
#define MLX90393_SCL   PBout(8) //SCL
#define MLX90393_SDA   PBout(9) //SDA
#define MLX90393_READ_SDA  PBin(9)  //����SDA
				  

// MLX90393	���ܵĵ�ַ					  
#define MLX90393slaveAddAndRead_011     0x19
#define MLX90393slaveAdd_011            0x18        //--

#define MLX90393slaveAddAndRead_012     0x21
#define MLX90393slaveAdd_012            0x20        //--

#define MLX90393slaveAddAndRead_013     0x29
#define MLX90393slaveAdd_013            0x28        //--

#define MLX90393slaveAddAndRead_014     0x31
#define MLX90393slaveAdd_014            0x30        //--		

// MLX 90393��ص�������
#define MLX90393_BurstReadCommand   0x16        //-yx-
#define MLX90393_WeekUpCommand      0x26        //-yx-
#define MLX90393_SingleReadCommand  0x36        //-yx-
#define MLX90393_ReadMeausreCommand 0x46        //-yx-
#define MLX90393_ReadRegCommand     0x50
#define MLX90393_WriteRegCommand    0x60
#define MLX90393_ExitCommand        0x80
#define MLX90393_MemRecallCommand   0xd0
#define MLX90393_MemStoreCommand    0xe0
#define MLX90393_ResetCommand       0xf0
#define MLX90393_StartMeasurement   0x36        //-yx-




// other

#define UseSetMlxReg0Data           0X801C

#define UseSetMlxReg1Data           0xE180
#define UseSetMlxReg2Data           0x01E5

#define MaxCmdErroTimes   10//??????????
#define IfMlxStatusByteErroWaiteNextCmdTime_ms   10 //???MLX ?????????????????????????????????????

#define MlxErro_InCallMemoryCall   0x01
#define MlxErro_InReadRegErro      0x02
#define MlxErro_InCmdExitErro      0x03
#define MlxErro_InWriteRegErro     0x04
#define MlxErro_InStoreRegErro     0x05
#define MlxErro_CheckUseFulDataErro 0x06
#define MlxInReg_Mode              1
#define MlxInNoneReg_Mode          0
#define MlxCycOprtationIcDelayTime 100//?????????????IC????????

#define MlxErro_StatusByteErro     0xFF

#define FALSE                      0
#define TRUE                       1


union uniRecUsefulMemoryData__
{
    unsigned short usRecUsefulMemoryData[10];
    struct strMlxAdjust__
    {
        unsigned short usMiddleX : 16;
        unsigned short usMiddleY : 16;
        unsigned short usLeftX : 16;
        unsigned short usLeftY : 16;
        unsigned short usRightX : 16;
        unsigned short usRightY : 16;
        unsigned short usUpX : 16;
        unsigned short usUpY : 16;
        unsigned short usDownX : 16;
        unsigned short usDownY : 16;
    } strMlxAdjust;
};




typedef  struct
{
	union uniRecUsefulMemoryData__  uniRecUsefulMemoryData;
	uint8_t xldata;
	uint8_t xhdata;
	uint8_t yldata;
	uint8_t yhdata;
	uint8_t zldata;
	uint8_t zhdata;	
	uint8_t tldata;
	uint8_t thdata;	
    uint8_t statusByte;	
	uint16_t xdata;
	uint16_t ydata;
	uint16_t zdata;
	uint16_t tdata;
    uint16_t basex;
    uint16_t basey;
    int16_t  deltX;
    int16_t  delty;
	unsigned char ucMlx90393ErroType;
	unsigned short usMlx90393StatusErroTimes;
	
} MLX90393Data;
//MLX90393���в�������

void MLX90393_IIC_Init(void);                //��ʼ��MLX90393��IO��				 
void MLX90393_Start(void);				//����MLX90393��ʼ�ź�
void MLX90393_Stop(void);	  			//����MLX90393ֹͣ�ź�
uint8_t MLX90393_Send_Byte(uint8_t WriteByte);
uint8_t MLX90393_Read_Byte(unsigned char ack);//MLX90393��ȡһ���ֽ�
uint8_t MLX90393_Wait_Ack(void); 				//MLX90393�ȴ�ACK�ź�
void MLX90393_Ack(void);					//MLX90393����ACK�ź�
void MLX90393_NAck(void);				//MLX90393������ACK�ź�						  
void vInMeasurementNormal(void) ;
unsigned char ucMLX90393WriteCommand(unsigned char ucWriteCmd, unsigned char ucSlaveAdd);
unsigned short usMlx90393ReadReg(unsigned char RegAddress, unsigned char SlaveAdd);
unsigned char ucMLX90393WriteReg(unsigned char ucRegAddress, unsigned short usWriteData, unsigned char ucSlaveAdd);
unsigned char ucCheckReadMlxStatusErro(unsigned char ucMlxStatusData, unsigned char ucCmdMode);
void vCmdMlxWriteRegAndWaite(uint16_t RegAddress, unsigned short usWriteData , unsigned char SlaveAdd, unsigned char ucErroType);
void vCmdMlxAndWaiteErroTimes(unsigned char command, unsigned char SlaveAdd, unsigned char ucErroType);
void vSetUpMlx90393(void);
void joysticBaseCal(uint16_t cnt);
void joysticDatareset(void);
#endif