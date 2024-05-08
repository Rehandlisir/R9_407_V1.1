#include "./BSP/R9/mlx90393.h"
#include "./SYSTEM/delay/delay.h"

MLX90393Data mlxdata;


void MLX90393_IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();   //使能GPIOB时钟
    
    //PB8,9初始化设置
    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //快速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
    MLX90393_SDA=1;
    MLX90393_SCL=1;  
	
	
}

//产生MLX90393起始信号
void MLX90393_Start(void)
{
	MLX90393_SDA_OUT();     //sda线输出
	MLX90393_SDA=1;	  	  
	MLX90393_SCL=1;
	delay_us(4);
 	MLX90393_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	MLX90393_SCL=0;//钳住I2C总线，准备发送或接收数据 
	delay_us(2);
}	  
//产生MLX90393停止信号
void MLX90393_Stop(void)
{
	MLX90393_SDA_OUT();//sda线输出
	MLX90393_SCL=0;
	MLX90393_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	MLX90393_SCL=1; 	
	MLX90393_SDA=1;//发送I2C总线结束信号
    delay_us(4);
    MLX90393_NAck(); //停止后不需要产生应答
							   	
}
//等待应答信号到来 读一个Nack信号
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t MLX90393_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	unsigned char staturs;
	MLX90393_SDA_IN();      //SDA设置为输入   
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
	MLX90393_SCL=0;//时钟输出0 	
    return 0;
} 
//产生ACK应答 写一下 ACK信号  一般用于数据读取完成后再操作的
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
//不产生ACK应答		    
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
//MLX90393发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
uint8_t MLX90393_Send_Byte(uint8_t WriteByte)
{     
    uint8_t i;
    uint8_t staturs;
    MLX90393_SDA_OUT();  //Set-->SDA As OutPutMode
    MLX90393_SCL=0;//拉低时钟开始数据传输
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
    staturs = MLX90393_Wait_Ack();//写完数据后，要求等待 ACK信号
    return (staturs);
}	
		    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t MLX90393_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	MLX90393_SDA_IN();//SDA设置为输入
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
        MLX90393_NAck();//发送nACK
    else
        MLX90393_Ack(); //发送ACK   
    return receive;
}

//向90393写一个命令 ucSlaveAdd 为写地址
unsigned char ucMLX90393WriteCommand(unsigned char ucWriteCmd, unsigned char ucSlaveAdd)
{
    unsigned char ucReadStatusData;//读取出来的 状态字
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

//用阻塞的方式 写寄存器 ucSlaveAdd 为写器件地址 ucRegAddress 为寄存器地址
unsigned char ucMLX90393WriteReg(unsigned char ucRegAddress, unsigned short usWriteData, unsigned char ucSlaveAdd)
{
    unsigned char ucReadStatusData;//读取出来的 状态字
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

//用阻塞的方式 读寄存器
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


//判断状态寄存器是否出错
unsigned char ucCheckReadMlxStatusErro(unsigned char ucMlxStatusData, unsigned char ucCmdMode)
{
    if(0xFF == ucMlxStatusData)return 1;//如果一个所有的CLK周期里，SDA被长时间地拉到高电平，说明数据位是错的
    else if(0x10 & ucMlxStatusData)return 1;//如果状态寄存器有错误存在的
    if(MlxInReg_Mode == ucCmdMode)//如果是读寄存器里中的数据
    {
        if(0x00 == ucMlxStatusData)return 0;
        else if(0x10 & ucMlxStatusData)return 1;
    }
    else
    {
        if(0x00 == ucMlxStatusData)return 1;//如果一个所有的CLK周期里，SDA被长时间地拉到高电平，说明数据位是错的
    }
    return 0;
}

//写命令并检测状态寄存器是否是出错的，如果出错允许强制多次写  最多时间为 由MaxCmdErroTimes定义的
void vCmdMlxAndWaiteErroTimes(unsigned char command, unsigned char SlaveAdd, unsigned char ucErroType)
{
    unsigned char ucReadCycCnt = 0;
    for(ucReadCycCnt = 0; ucReadCycCnt < MaxCmdErroTimes; ucReadCycCnt++)//最大允许错误计数
    {
        mlxdata.statusByte = ucMLX90393WriteCommand(command, SlaveAdd);//写一次数据
        if(ucCheckReadMlxStatusErro(mlxdata.statusByte, MlxInReg_Mode))
        {
             delay_ms(10);
        }
        else return;
    }
    if(ucReadCycCnt > 4) mlxdata.ucMlx90393ErroType = ucErroType;
}



//写寄存器并检测状态寄存器是否是出错的，如果出错允许强制多次写  最多时间为 由MaxCmdErroTimes定义的
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



//对90393的上电初始化工作
void vSetUpMlx90393(void)
{
    
    #define IfMemoryRecallOkWaiteTime_ms 5//如果不同逻辑操作的命令之间的等待延时
    #define IfReadNextRegWaite_ms        2//每个相同命令之间的时间间隔
    unsigned char ucReadMlxRegNumIndex = 0;//读取芯片，寄存器计数器
    unsigned char ucReadCycCnt = 0;//循环读计数器
    unsigned short usReg0_3DataArr[3] = {0};
    mlxdata.ucMlx90393ErroType = 0;//清故 障信息

    MLX90393_IIC_Init(); // IIC 初始化

    vCmdMlxAndWaiteErroTimes(MLX90393_ExitCommand, MLX90393slaveAdd_011, MlxErro_InCmdExitErro);//让其先退出所有操做

    delay_ms(1); //等待 1ms

    vCmdMlxAndWaiteErroTimes(MLX90393_ExitCommand, MLX90393slaveAdd_011, MlxErro_InCmdExitErro);//复位芯片

    delay_ms(2); //等待 2ms 

    if((UseSetMlxReg0Data != usReg0_3DataArr[0]) || (UseSetMlxReg1Data != usReg0_3DataArr[1]) ||
            (UseSetMlxReg2Data != usReg0_3DataArr[2]))//如果数据与预期不一样，就让其烧写下芯片
    {
		delay_ms(1);
        vCmdMlxWriteRegAndWaite(0x00, UseSetMlxReg0Data, MLX90393slaveAdd_011, MlxErro_InWriteRegErro);   // 写寄存器 0x00
        if(mlxdata.ucMlx90393ErroType)return;
        else
        {
			delay_ms(2);
            vCmdMlxWriteRegAndWaite(0x01, UseSetMlxReg1Data, MLX90393slaveAdd_011, MlxErro_InWriteRegErro);// 写寄存器 0x01
        }
        if(mlxdata.ucMlx90393ErroType)return;
        else
        {
			delay_ms(2);
            vCmdMlxWriteRegAndWaite(0x02, UseSetMlxReg2Data, MLX90393slaveAdd_011, MlxErro_InWriteRegErro);// 写寄存器 0x02
        }
        if(mlxdata.ucMlx90393ErroType)return;
        else
        {
			delay_ms(2);
            vCmdMlxAndWaiteErroTimes(MLX90393_MemStoreCommand, MLX90393slaveAdd_011, MlxErro_InStoreRegErro);
        }
		delay_ms(15);
    }
    vCmdMlxAndWaiteErroTimes(MLX90393_StartMeasurement, MLX90393slaveAdd_011, MlxErro_InCallMemoryCall);//让其触发一次采样
    if(mlxdata.ucMlx90393ErroType)return;
    else
	{mlxdata.ucMlx90393ErroType = FALSE;}
    //joysticBaseCal(100);
}


void vInMeasurementNormal(void) 
{
	unsigned char bufx[2], bufy[2];
	MLX90393_Start(); // 写起始位
	MLX90393_Send_Byte(MLX90393slaveAdd_011); //写从机地址 
	
	MLX90393_Send_Byte(MLX90393_ReadMeausreCommand); //写-读命令 只读X,Y的数值
	
	MLX90393_Start(); // 重写起始位
	MLX90393_Send_Byte(MLX90393slaveAddAndRead_011); //读操作 只读X,Y的数值
	
	mlxdata.statusByte = MLX90393_Read_Byte(1); // 读取状态位，主机应答
	mlxdata.xhdata = MLX90393_Read_Byte(1) ;// 读取X 的8位高位，主机应答
	mlxdata.xldata = MLX90393_Read_Byte(1) ;// 读取X 的8位低位，主机应答
	mlxdata.yhdata = MLX90393_Read_Byte(1) ;// 读取Y 的8位高位，主机应答
	mlxdata.yldata = MLX90393_Read_Byte(0) ;// 读取Y 的8位低位，主机非应答
	
	
	
	//低八位和高八位组合为 16位 数据转换输出
    bufx[0] = mlxdata.xldata;
    bufx[1] = mlxdata.xhdata;
    bufy[0] = mlxdata.yldata;
    bufy[1] = mlxdata.yhdata;	
    
    mlxdata.xdata = ((*((uint8_t *)bufx+ 1)<< 8))| *(uint8_t *)bufx;
	mlxdata.ydata = ((*((uint8_t *)bufy+ 1)<< 8))| *(uint8_t *)bufy;

    mlxdata.deltX = mlxdata.xdata ;//- mlxdata.basex;
    mlxdata.delty = mlxdata.ydata;// - mlxdata.basey;

	MLX90393_Stop();// 读取完成发停止信号；
	//最后 需要再次写一次 工作模式的命令
   if(ucCheckReadMlxStatusErro(mlxdata.statusByte, MlxInNoneReg_Mode))
    {
        mlxdata.usMlx90393StatusErroTimes++;//如果状态位有问题 累加错误帧计数器
    }
    mlxdata.statusByte = ucMLX90393WriteCommand(MLX90393_StartMeasurement,MLX90393slaveAdd_011);//写一个读命令，
	
   if(ucCheckReadMlxStatusErro(mlxdata.statusByte, MlxInNoneReg_Mode))
    {
        mlxdata.usMlx90393StatusErroTimes++;//如果状态位有问题 累加错误帧计数器
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