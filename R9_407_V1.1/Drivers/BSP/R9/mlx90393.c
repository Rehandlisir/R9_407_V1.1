#include "./BSP/R9/mlx90393.h"
#include "./SYSTEM/delay/delay.h"

MLX90393Data mlxdata;


void MLX90393_IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();   //ʹ��GPIOBʱ��
    
    //PB8,9��ʼ������
    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
    MLX90393_SDA=1;
    MLX90393_SCL=1;  
	
	
}

//����MLX90393��ʼ�ź�
void MLX90393_Start(void)
{
	MLX90393_SDA_OUT();     //sda�����
	MLX90393_SDA=1;	  	  
	MLX90393_SCL=1;
	delay_us(4);
 	MLX90393_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	MLX90393_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
	delay_us(2);
}	  
//����MLX90393ֹͣ�ź�
void MLX90393_Stop(void)
{
	MLX90393_SDA_OUT();//sda�����
	MLX90393_SCL=0;
	MLX90393_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	MLX90393_SCL=1; 	
	MLX90393_SDA=1;//����I2C���߽����ź�
    delay_us(4);
    MLX90393_NAck(); //ֹͣ����Ҫ����Ӧ��
							   	
}
//�ȴ�Ӧ���źŵ��� ��һ��Nack�ź�
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t MLX90393_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	unsigned char staturs;
	MLX90393_SDA_IN();      //SDA����Ϊ����   
    MLX90393_SDA=1;delay_us(1);
	MLX90393_SCL=1;delay_us(1);
    while (MLX90393_READ_SDA)
    {
        ucErrTime++;
        if (ucErrTime>250)
        {
          MLX90393_Stop();
          return 1;  
        }
    }
	MLX90393_SCL=0;//ʱ�����0 	
    return 0;
} 
//����ACKӦ�� дһ�� ACK�ź�  һ���������ݶ�ȡ��ɺ��ٲ�����
void MLX90393_Ack(void)
{
    MLX90393_SCL = 0;
	MLX90393_SDA_OUT();
	MLX90393_SDA=0;
	delay_us(2);
	MLX90393_SCL=1;
	delay_us(2);
	MLX90393_SCL=0;
	
}
//������ACKӦ��		    
void MLX90393_NAck(void)
{
    MLX90393_SCL=0;
	MLX90393_SDA_OUT();
	MLX90393_SDA=1;
    delay_us(2);
	MLX90393_SCL=1;
	delay_us(2);
    MLX90393_SCL=0;
	
	
}					 				     
//MLX90393����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
uint8_t MLX90393_Send_Byte(uint8_t WriteByte)
{     
    uint8_t i;
    uint8_t staturs;
    MLX90393_SDA_OUT();  //Set-->SDA As OutPutMode
    MLX90393_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(i = 0; i < 8; i++)
    {
        MLX90393_SDA = (WriteByte&0x80)>>7;
        WriteByte<<=1;
        delay_us(2);
        MLX90393_SCL = 1;
        delay_us(2);
        MLX90393_SCL = 0;
        delay_us(2);
    }
    staturs = MLX90393_Wait_Ack();//д�����ݺ�Ҫ��ȴ� ACK�ź�
    return (staturs);
}	
		    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t MLX90393_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	MLX90393_SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        MLX90393_SCL=0; 
        delay_us(2);
		MLX90393_SCL=1;
        receive<<=1;
        if(MLX90393_READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        MLX90393_NAck();//����nACK
    else
        MLX90393_Ack(); //����ACK   
    return receive;
}

//��90393дһ������ ucSlaveAdd Ϊд��ַ
unsigned char ucMLX90393WriteCommand(unsigned char ucWriteCmd, unsigned char ucSlaveAdd)
{
    unsigned char ucReadStatusData;//��ȡ������ ״̬��
    MLX90393_Start();
    MLX90393_Send_Byte(ucSlaveAdd);
    MLX90393_Send_Byte(ucWriteCmd);

    MLX90393_Start();
    ucSlaveAdd |= 0x01; //read data slave address
    MLX90393_Send_Byte(ucSlaveAdd);

    ucReadStatusData = MLX90393_Read_Byte(1); 
    
    /*IIC stop*/
    MLX90393_Stop();
    return (ucReadStatusData);
}

//�������ķ�ʽ д�Ĵ��� ucSlaveAdd Ϊд������ַ ucRegAddress Ϊ�Ĵ�����ַ
unsigned char ucMLX90393WriteReg(unsigned char ucRegAddress, unsigned short usWriteData, unsigned char ucSlaveAdd)
{
    unsigned char ucReadStatusData;//��ȡ������ ״̬��
    MLX90393_Start();
    MLX90393_Send_Byte(ucSlaveAdd);
    MLX90393_Send_Byte(MLX90393_WriteRegCommand);
    MLX90393_Send_Byte(usWriteData >> 8);
    MLX90393_Send_Byte(usWriteData);
    MLX90393_Send_Byte(ucRegAddress << 2);
    MLX90393_Start();
    ucSlaveAdd |= 0x01; //read data slave address
    MLX90393_Send_Byte(ucSlaveAdd);
    ucReadStatusData = MLX90393_Read_Byte(1);
   
    /*IIC stop*/
    MLX90393_Stop();
    return(ucReadStatusData);
}

//�������ķ�ʽ ���Ĵ���
unsigned short usMlx90393ReadReg(unsigned char RegAddress, unsigned char SlaveAdd)
{
    
    unsigned int Data;
    RegAddress = RegAddress << 2;
    MLX90393_Start();
    MLX90393_Send_Byte(SlaveAdd);
    MLX90393_Send_Byte(MLX90393_ReadRegCommand);
    MLX90393_Send_Byte(RegAddress);

    MLX90393_Start();
    SlaveAdd	|= 0x01; 			//read data slave address
    MLX90393_Send_Byte(SlaveAdd);

    mlxdata.statusByte = MLX90393_Read_Byte(1);
 
    Data = MLX90393_Read_Byte(1);
 
    Data = Data << 8;
    Data |= MLX90393_Read_Byte(1);
 

    /*IIC stop*/
    MLX90393_Stop();
    return (Data);
}


//�ж�״̬�Ĵ����Ƿ����
unsigned char ucCheckReadMlxStatusErro(unsigned char ucMlxStatusData, unsigned char ucCmdMode)
{
    if(0xFF == ucMlxStatusData)return 1;//���һ�����е�CLK�����SDA����ʱ��������ߵ�ƽ��˵������λ�Ǵ��
    else if(0x10 & ucMlxStatusData)return 1;//���״̬�Ĵ����д�����ڵ�
    if(MlxInReg_Mode == ucCmdMode)//����Ƕ��Ĵ������е�����
    {
        if(0x00 == ucMlxStatusData)return 0;
        else if(0x10 & ucMlxStatusData)return 1;
    }
    else
    {
        if(0x00 == ucMlxStatusData)return 1;//���һ�����е�CLK�����SDA����ʱ��������ߵ�ƽ��˵������λ�Ǵ��
    }
    return 0;
}

//д������״̬�Ĵ����Ƿ��ǳ���ģ������������ǿ�ƶ��д  ���ʱ��Ϊ ��MaxCmdErroTimes�����
void vCmdMlxAndWaiteErroTimes(unsigned char command, unsigned char SlaveAdd, unsigned char ucErroType)
{
    unsigned char ucReadCycCnt = 0;
    for(ucReadCycCnt = 0; ucReadCycCnt < MaxCmdErroTimes; ucReadCycCnt++)//�������������
    {
        mlxdata.statusByte = ucMLX90393WriteCommand(command, SlaveAdd);//дһ������
        if(ucCheckReadMlxStatusErro(mlxdata.statusByte, MlxInReg_Mode))
        {
             delay_ms(10);
        }
        else return;
    }
    if(ucReadCycCnt > 4) mlxdata.ucMlx90393ErroType = ucErroType;
}



//д�Ĵ��������״̬�Ĵ����Ƿ��ǳ���ģ������������ǿ�ƶ��д  ���ʱ��Ϊ ��MaxCmdErroTimes�����
void vCmdMlxWriteRegAndWaite(uint16_t RegAddress, unsigned short usWriteData , unsigned char SlaveAdd, unsigned char ucErroType)
{
    unsigned char ucReadCycCnt = 0;
    for(ucReadCycCnt = 0; ucReadCycCnt < MaxCmdErroTimes; ucReadCycCnt++)
    {
        mlxdata.statusByte = ucMLX90393WriteReg(RegAddress, usWriteData, SlaveAdd);
        if(ucCheckReadMlxStatusErro(mlxdata.statusByte, MlxInReg_Mode))
        {
				delay_ms(10);
        }
        else return;
    }
    if(ucReadCycCnt > 4) mlxdata.ucMlx90393ErroType = ucErroType;
}



//��90393���ϵ��ʼ������
void vSetUpMlx90393(void)
{
    
    #define IfMemoryRecallOkWaiteTime_ms 5//�����ͬ�߼�����������֮��ĵȴ���ʱ
    #define IfReadNextRegWaite_ms        2//ÿ����ͬ����֮���ʱ����
    unsigned char ucReadMlxRegNumIndex = 0;//��ȡоƬ���Ĵ���������
    unsigned char ucReadCycCnt = 0;//ѭ����������
    unsigned short usReg0_3DataArr[3] = {0};
    mlxdata.ucMlx90393ErroType = 0;//��� ����Ϣ

    MLX90393_IIC_Init(); // IIC ��ʼ��

    vCmdMlxAndWaiteErroTimes(MLX90393_ExitCommand, MLX90393slaveAdd_011, MlxErro_InCmdExitErro);//�������˳����в���

    delay_ms(1); //�ȴ� 1ms

    vCmdMlxAndWaiteErroTimes(MLX90393_ExitCommand, MLX90393slaveAdd_011, MlxErro_InCmdExitErro);//��λоƬ

    delay_ms(2); //�ȴ� 2ms 

    if((UseSetMlxReg0Data != usReg0_3DataArr[0]) || (UseSetMlxReg1Data != usReg0_3DataArr[1]) ||
            (UseSetMlxReg2Data != usReg0_3DataArr[2]))//���������Ԥ�ڲ�һ������������д��оƬ
    {
		delay_ms(1);
        vCmdMlxWriteRegAndWaite(0x00, UseSetMlxReg0Data, MLX90393slaveAdd_011, MlxErro_InWriteRegErro);   // д�Ĵ��� 0x00
        if(mlxdata.ucMlx90393ErroType)return;
        else
        {
			delay_ms(2);
            vCmdMlxWriteRegAndWaite(0x01, UseSetMlxReg1Data, MLX90393slaveAdd_011, MlxErro_InWriteRegErro);// д�Ĵ��� 0x01
        }
        if(mlxdata.ucMlx90393ErroType)return;
        else
        {
			delay_ms(2);
            vCmdMlxWriteRegAndWaite(0x02, UseSetMlxReg2Data, MLX90393slaveAdd_011, MlxErro_InWriteRegErro);// д�Ĵ��� 0x02
        }
        if(mlxdata.ucMlx90393ErroType)return;
        else
        {
			delay_ms(2);
            vCmdMlxAndWaiteErroTimes(MLX90393_MemStoreCommand, MLX90393slaveAdd_011, MlxErro_InStoreRegErro);
        }
		delay_ms(15);
    }
    vCmdMlxAndWaiteErroTimes(MLX90393_StartMeasurement, MLX90393slaveAdd_011, MlxErro_InCallMemoryCall);//���䴥��һ�β���
    if(mlxdata.ucMlx90393ErroType)return;
    else
	{mlxdata.ucMlx90393ErroType = FALSE;}
    //joysticBaseCal(100);
}


void vInMeasurementNormal(void) 
{
	unsigned char bufx[2], bufy[2];
	MLX90393_Start(); // д��ʼλ
	MLX90393_Send_Byte(MLX90393slaveAdd_011); //д�ӻ���ַ 
	
	MLX90393_Send_Byte(MLX90393_ReadMeausreCommand); //д-������ ֻ��X,Y����ֵ
	
	MLX90393_Start(); // ��д��ʼλ
	MLX90393_Send_Byte(MLX90393slaveAddAndRead_011); //������ ֻ��X,Y����ֵ
	
	mlxdata.statusByte = MLX90393_Read_Byte(1); // ��ȡ״̬λ������Ӧ��
	mlxdata.xhdata = MLX90393_Read_Byte(1) ;// ��ȡX ��8λ��λ������Ӧ��
	mlxdata.xldata = MLX90393_Read_Byte(1) ;// ��ȡX ��8λ��λ������Ӧ��
	mlxdata.yhdata = MLX90393_Read_Byte(1) ;// ��ȡY ��8λ��λ������Ӧ��
	mlxdata.yldata = MLX90393_Read_Byte(0) ;// ��ȡY ��8λ��λ��������Ӧ��
	
	
	
	//�Ͱ�λ�͸߰�λ���Ϊ 16λ ����ת�����
    bufx[0] = mlxdata.xldata;
    bufx[1] = mlxdata.xhdata;
    bufy[0] = mlxdata.yldata;
    bufy[1] = mlxdata.yhdata;	
    
    mlxdata.xdata = ((*((uint8_t *)bufx+ 1)<< 8))| *(uint8_t *)bufx;
	mlxdata.ydata = ((*((uint8_t *)bufy+ 1)<< 8))| *(uint8_t *)bufy;

    mlxdata.deltX = mlxdata.xdata ;//- mlxdata.basex;
    mlxdata.delty = mlxdata.ydata;// - mlxdata.basey;

	MLX90393_Stop();// ��ȡ��ɷ�ֹͣ�źţ�
	//��� ��Ҫ�ٴ�дһ�� ����ģʽ������
   if(ucCheckReadMlxStatusErro(mlxdata.statusByte, MlxInNoneReg_Mode))
    {
        mlxdata.usMlx90393StatusErroTimes++;//���״̬λ������ �ۼӴ���֡������
    }
    mlxdata.statusByte = ucMLX90393WriteCommand(MLX90393_StartMeasurement,MLX90393slaveAdd_011);//дһ�������
	
   if(ucCheckReadMlxStatusErro(mlxdata.statusByte, MlxInNoneReg_Mode))
    {
        mlxdata.usMlx90393StatusErroTimes++;//���״̬λ������ �ۼӴ���֡������
    }
    
}

void joysticBaseCal(uint16_t cnt)
{   
    uint16_t k;

    uint32_t sumx,sumy;
    joysticDatareset();
    for(k = 0; k < cnt; k++) 
    {
        vInMeasurementNormal();
        sumx +=  mlxdata.xdata;
        sumy += mlxdata.ydata;
        delay_ms(5);
    } 
    mlxdata.basex =  (sumx / cnt );
    mlxdata.basey =  (sumy/ cnt);
}

void joysticDatareset(void)
{
    mlxdata.basex = 0;
    mlxdata.basey = 0;
}