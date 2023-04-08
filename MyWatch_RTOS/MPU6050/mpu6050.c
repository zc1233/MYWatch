#include <math.h>
#include "MPU6050.h"

const uint16_t i2c_timeout = 100;
const double Accel_Z_corrector = 14418.0;

uint32_t timer;

Kalman_t KalmanX = {
    .Q_angle = 0.001f,
    .Q_bias = 0.003f,
    .R_measure = 0.03f};

Kalman_t KalmanY = {
    .Q_angle = 0.001f,
    .Q_bias = 0.003f,
    .R_measure = 0.03f,
};

MPU6050_t angle = {
	  .Accel_X_RAW = 0,
    .Accel_Y_RAW = 0,
    .Accel_Z_RAW = 0,
    .Ax = 0.0,
    .Ay = 0.0,
    .Az = 0.0,

    .Gyro_X_RAW = 0,
    .Gyro_Y_RAW = 0,
    .Gyro_Z_RAW = 0,
    .Gx = 0.0,
    .Gy = 0.0,
    .Gz = 0.0,

    .Temperature = 0.0,

    .KalmanAngleX = 0.0,
    .KalmanAngleY = 0.0
};

//向MPU6050寄存器中写数据
uint8_t I2C_MPU6050_ByteWrite(uint8_t WriteAddr, uint8_t data)
{
		/* 第1步：发起I2C总线启动信号 */
	i2c_Start();
	
	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte((MPU6050_ADDR<<1)| I2C_WR);	/* 此处是写指令 */
	
	/* 第3步：等待ACK */
	if (i2c_WaitAck() != 0)
	{
 		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	
	/* 第4步：发送寄存器地址 */
	i2c_SendByte((uint8_t)WriteAddr);
	
  /* 第5步：等待ACK */
	if (i2c_WaitAck() != 0)
	{
	  goto cmd_fail;	/* EEPROM器件无应答 */
	}
	
	/* 第6步：开始写入数据 */
	i2c_SendByte(data);
	
	/* 第7步：等待ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	
  /* 命令执行成功，发送I2C总线停止信号 */
	i2c_Stop();
	return 1;
cmd_fail: /* 命令执行失败后，发送停止信号 */
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 0;
}


uint8_t I2C_MPU6050_DataRead(uint8_t ReadAddr)
{
	
	uint8_t data;
 
	/* 第1步：发起I2C总线启动信号 */
	i2c_Start();
 
	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte((MPU6050_ADDR<<1)|I2C_WR);	/* 此处是写指令 */
  
	/* 第3步：等待ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	
	/* 第4步：发送DS3231寄存器地址 */
	i2c_SendByte((uint8_t)ReadAddr);
	
	/* 第5步：等待ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}
	
	/* 第6步：产生第二次 I2C 起始信号 */  
  	i2c_Start();
	
	/* 第7步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	//i2c_SendByte(DS3231_ADDRESS | I2C_RD);	/* 此处是读指令 */
	i2c_SendByte((MPU6050_ADDR<<1)| I2C_RD);	/* 此处是读指令 */
	
	/* 第8步：发送ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM器件无应答 */
	}	
   
  	/* 第9步：读取数据 */
	data = i2c_ReadByte(); 
 
  	i2c_NAck();	
	i2c_Stop();
	return data;
	
  	cmd_fail: /* 命令执行失败后，发送停止信号 */
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	return 0;
}

void I2C_MPU6050_MultipleDataRead(uint8_t ReadAddr, uint8_t *arr, uint8_t size)
{
 	i2c_Start(); 
	i2c_SendByte((MPU6050_ADDR<<1)|0);//发送器件地址+写命令	
	if(i2c_WaitAck() != 0)	//等待应答
	{
		i2c_Stop();		 
		return;		
	}
    i2c_SendByte(ReadAddr);	//写寄存器地址
    i2c_WaitAck();		//等待应答
    i2c_Start();
	i2c_SendByte((MPU6050_ADDR<<1)|1);//发送器件地址+读命令	
    i2c_WaitAck();		//等待应答 
	while(size)
	{
		if(size==1){*arr=i2c_ReadByte();i2c_NAck();}//读数据,发送nACK 
		else {*arr=i2c_ReadByte();i2c_Ack();	}	//读数据,发送ACK  
		size--;
		arr++; 
	}    
   i2c_Stop();	//产生一个停止条件 
	return;	
}

uint8_t MPU6050_Init(void)
{ 
	uint8_t res;
 
	
	//MPU_AD0_CTRL=0;			//控制MPU6050的AD0脚为低电平,从机地址为:0X68
	I2C_GPIO_Config();
	//MPU_IIC_Init();//初始化IIC总线
	I2C_MPU6050_ByteWrite(MPU_PWR_MGMT1_REG,0X80);	//复位MPU6050
  HAL_Delay(100);
	I2C_MPU6050_ByteWrite(MPU_PWR_MGMT1_REG,0X00);	//唤醒MPU6050 
	I2C_MPU6050_ByteWrite(MPU_GYRO_CFG_REG,3<<3);      //陀螺仪传感器,±2000dps
	I2C_MPU6050_ByteWrite(MPU_ACCEL_CFG_REG,0<<3);				//加速度传感器,±2g
	MPU_Set_Rate(50);						//设置采样率50Hz
	I2C_MPU6050_ByteWrite(MPU_INT_EN_REG,0X00);	//关闭所有中断
	I2C_MPU6050_ByteWrite(MPU_USER_CTRL_REG,0X00);	//I2C主模式关闭
	I2C_MPU6050_ByteWrite(MPU_FIFO_EN_REG,0X00);	//关闭FIFO
	I2C_MPU6050_ByteWrite(MPU_INTBP_CFG_REG,0X80);	//INT引脚低电平有效
	res=I2C_MPU6050_DataRead(MPU_DEVICE_ID_REG);
	if(res==MPU6050_ADDR)//器件ID正确
	{
		I2C_MPU6050_ByteWrite(MPU_PWR_MGMT1_REG,0X01);	//设置CLKSEL,PLL X轴为参考
		I2C_MPU6050_ByteWrite(MPU_PWR_MGMT2_REG,0X00);	//加速度与陀螺仪都工作
		MPU_Set_Rate(50);						//设置采样率为50Hz
 	}else return 1;
	return 0;
}

//设置MPU6050的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)
//返回值:0,设置成功
//    其他,设置失败 
uint8_t MPU_Set_Rate(uint16_t rate)
{
	uint8_t data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=I2C_MPU6050_ByteWrite(MPU_SAMPLE_RATE_REG,data);	//设置数字低通滤波器
 	return MPU_Set_LPF(rate/2);	//自动设置LPF为采样率的一半
}

//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败 
uint8_t MPU_Set_LPF(uint16_t lpf)
{
	uint8_t data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return I2C_MPU6050_ByteWrite(MPU_CFG_REG,data);//设置数字低通滤波器  
}

void MPU6050_Read_Accel(MPU6050_t *DataStruct)
{
    uint8_t Rec_Data[6];

    // Read 6 BYTES of data starting from ACCEL_XOUT_H register
    I2C_MPU6050_MultipleDataRead(ACCEL_XOUT_H_REG, Rec_Data, 6);

    DataStruct->Accel_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data[1]);
    DataStruct->Accel_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data[3]);
    DataStruct->Accel_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data[5]);

    /*** convert the RAW values into acceleration in 'g'
         we have to divide according to the Full scale value set in FS_SEL
         I have configured FS_SEL = 0. So I am dividing by 16384.0
         for more details check ACCEL_CONFIG Register              ****/

    DataStruct->Ax = DataStruct->Accel_X_RAW / 16384.0;
    DataStruct->Ay = DataStruct->Accel_Y_RAW / 16384.0;
    DataStruct->Az = DataStruct->Accel_Z_RAW / Accel_Z_corrector;
}

void MPU6050_Read_Gyro(MPU6050_t *DataStruct)
{
    uint8_t Rec_Data[6];

    // Read 6 BYTES of data starting from GYRO_XOUT_H register

    I2C_MPU6050_MultipleDataRead(GYRO_XOUT_H_REG, Rec_Data, 6);

    DataStruct->Gyro_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data[1]);
    DataStruct->Gyro_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data[3]);
    DataStruct->Gyro_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data[5]);

    /*** convert the RAW values into dps (�/s)
         we have to divide according to the Full scale value set in FS_SEL
         I have configured FS_SEL = 0. So I am dividing by 131.0
         for more details check GYRO_CONFIG Register              ****/

    DataStruct->Gx = DataStruct->Gyro_X_RAW / 131.0;
    DataStruct->Gy = DataStruct->Gyro_Y_RAW / 131.0;
    DataStruct->Gz = DataStruct->Gyro_Z_RAW / 131.0;
}

void MPU6050_Read_Temp(MPU6050_t *DataStruct)
{
    uint8_t Rec_Data[2];
    int16_t temp;

    // Read 2 BYTES of data starting from TEMP_OUT_H_REG register

    I2C_MPU6050_MultipleDataRead(TEMP_OUT_H_REG, Rec_Data, 2);

    temp = (int16_t)(Rec_Data[0] << 8 | Rec_Data[1]);
    DataStruct->Temperature = (float)((int16_t)temp / (float)340.0 + (float)36.53);
}

void MPU6050_Read_All(MPU6050_t *DataStruct)
{
    uint8_t Rec_Data[14];
    int16_t temp;

    // Read 14 BYTES of data starting from ACCEL_XOUT_H register

    I2C_MPU6050_MultipleDataRead(ACCEL_XOUT_H_REG, Rec_Data, 14);

    DataStruct->Accel_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data[1]);
    DataStruct->Accel_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data[3]);
    DataStruct->Accel_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data[5]);
    temp = (int16_t)(Rec_Data[6] << 8 | Rec_Data[7]);
    DataStruct->Gyro_X_RAW = (int16_t)(Rec_Data[8] << 8 | Rec_Data[9]);
    DataStruct->Gyro_Y_RAW = (int16_t)(Rec_Data[10] << 8 | Rec_Data[11]);
    DataStruct->Gyro_Z_RAW = (int16_t)(Rec_Data[12] << 8 | Rec_Data[13]);

    DataStruct->Ax = DataStruct->Accel_X_RAW / 16384.0;
    DataStruct->Ay = DataStruct->Accel_Y_RAW / 16384.0;
    DataStruct->Az = DataStruct->Accel_Z_RAW / Accel_Z_corrector;
    DataStruct->Temperature = (float)((int16_t)temp / (float)340.0 + (float)36.53);
    DataStruct->Gx = DataStruct->Gyro_X_RAW / 131.0;
    DataStruct->Gy = DataStruct->Gyro_Y_RAW / 131.0;
    DataStruct->Gz = DataStruct->Gyro_Z_RAW / 131.0;

    // Kalman angle solve
    double dt = (double)(HAL_GetTick() - timer) / 1000;
    timer = HAL_GetTick();
    double roll;
    double roll_sqrt = sqrt(
        DataStruct->Accel_X_RAW * DataStruct->Accel_X_RAW + DataStruct->Accel_Z_RAW * DataStruct->Accel_Z_RAW);
    if (roll_sqrt != 0.0)
    {
        roll = atan(DataStruct->Accel_Y_RAW / roll_sqrt) * RAD_TO_DEG;
    }
    else
    {
        roll = 0.0;
    }
    double pitch = atan2(-DataStruct->Accel_X_RAW, DataStruct->Accel_Z_RAW) * RAD_TO_DEG;
    if ((pitch < -90 && DataStruct->KalmanAngleY > 90) || (pitch > 90 && DataStruct->KalmanAngleY < -90))
    {
        KalmanY.angle = pitch;
        DataStruct->KalmanAngleY = pitch;
    }
    else
    {
        DataStruct->KalmanAngleY = Kalman_getAngle(&KalmanY, pitch, DataStruct->Gy, dt);
    }
    if (fabs(DataStruct->KalmanAngleY) > 90)
        DataStruct->Gx = -DataStruct->Gx;
    DataStruct->KalmanAngleX = Kalman_getAngle(&KalmanX, roll, DataStruct->Gx, dt);
}

double Kalman_getAngle(Kalman_t *Kalman, double newAngle, double newRate, double dt)
{
    double rate = newRate - Kalman->bias;
    Kalman->angle += dt * rate;

    Kalman->P[0][0] += dt * (dt * Kalman->P[1][1] - Kalman->P[0][1] - Kalman->P[1][0] + Kalman->Q_angle);
    Kalman->P[0][1] -= dt * Kalman->P[1][1];
    Kalman->P[1][0] -= dt * Kalman->P[1][1];
    Kalman->P[1][1] += Kalman->Q_bias * dt;

    double S = Kalman->P[0][0] + Kalman->R_measure;
    double K[2];
    K[0] = Kalman->P[0][0] / S;
    K[1] = Kalman->P[1][0] / S;

    double y = newAngle - Kalman->angle;
    Kalman->angle += K[0] * y;
    Kalman->bias += K[1] * y;

    double P00_temp = Kalman->P[0][0];
    double P01_temp = Kalman->P[0][1];

    Kalman->P[0][0] -= K[0] * P00_temp;
    Kalman->P[0][1] -= K[0] * P01_temp;
    Kalman->P[1][0] -= K[1] * P00_temp;
    Kalman->P[1][1] -= K[1] * P01_temp;

    return Kalman->angle;
};
