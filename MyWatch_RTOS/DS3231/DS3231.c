#include "DS3231.h"

_calendar_obj calendar  = {23,3,27,0,17,39,0,0};

void I2C_DS3231_Init(void)
{
  I2C_GPIO_Config(); 
	
	/* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
	i2c_Stop();
}


//
/**
  * @brief	写一个字节到I2C DS3231中
  * @param   
  *		@arg data:要写入的字节
  *		@arg WriteAddr:写地址 
	* @retval 返回1，表示写入成功.
  */
uint32_t I2C_DS3231_ByteWrite(u8 WriteAddr, u8 data) 
{
	/* 第1步：发起I2C总线启动信号 */
	i2c_Start();
	
	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte(DS3231_ADDRESS| I2C_WR);	/* 此处是写指令 */
	
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
 
/**
  * @brief	从DS3231里面读取一个字节数据 
  * @param   
  *		@arg data:存放从DS3231读取的数据
  *		@arg ReadAddr:读取数据的DS3231的地址
	* @retval data:返回数据.
  */
uint8_t I2C_DS3231_DataRead(u8 ReadAddr)
{
	uint8_t data;
 
	/* 第1步：发起I2C总线启动信号 */
	i2c_Start();
 
	/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
	i2c_SendByte(DS3231_ADDRESS|I2C_WR);	/* 此处是写指令 */
  
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
	i2c_SendByte(DS3231_ADDRESS | I2C_RD);	/* 此处是读指令 */
	
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
 
/**
  * @brief	时间设置
  * @param   
  *		@arg 	分别输入 年 月 日 星期 时 分 秒
  * @retval 无
  */
void I2C_DS3231_SetTime(u8 y,u8 months,u8 d,u8 w,u8 h,u8 m,u8 s)
{
  u8 temp=0;
	
  temp=DEC_BCD(y);
  I2C_DS3231_ByteWrite(0x06,temp);
  temp=DEC_BCD(months);
  I2C_DS3231_ByteWrite(0x05,temp);
   
  temp=DEC_BCD(d);
  I2C_DS3231_ByteWrite(0x04,temp);
  
	temp=DEC_BCD(w);
  I2C_DS3231_ByteWrite(0x03,temp);
	
  temp=DEC_BCD(h);
  I2C_DS3231_ByteWrite(0x02,temp);
  
  temp=DEC_BCD(m);
  I2C_DS3231_ByteWrite(0x01,temp);
  
  temp=DEC_BCD(s);
  I2C_DS3231_ByteWrite(0x00,temp);
}	
 
/**
  * @brief	获取时间
  * @param   
  *		@arg pBuffer:存放从DS3231读取的数据的缓冲区指针
  *		@arg ReadAddr:读取数据的DS3231的地址
  *   @arg NumByteToWrite:要从DS3231读取的字节数
  */
void I2C_DS3231_getTime(void)
{
	calendar.now_year=I2C_DS3231_DataRead(0x06);  
	calendar.now_year=BCD_DEC(calendar.now_year);
 
	calendar.months=I2C_DS3231_DataRead(0x05); 
	calendar.months=BCD_DEC(calendar.months);
 
	calendar.dates=I2C_DS3231_DataRead(0x04);  
	calendar.dates=BCD_DEC(calendar.dates);
 
	calendar.now_week=I2C_DS3231_DataRead(0x03);  
	calendar.now_week=BCD_DEC(calendar.now_week);
 
	calendar.hour=I2C_DS3231_DataRead(0x02); 
	calendar.hour&=0x3f;                   
	calendar.hour=BCD_DEC(calendar.hour);
 
	calendar.minute=I2C_DS3231_DataRead(0x01);
	calendar.minute=BCD_DEC(calendar.minute);
 
 
	calendar.second=I2C_DS3231_DataRead(0x00);
	calendar.second=BCD_DEC(calendar.second);
}

/**
  * @brief	获取温度
  * @param  无
  * @retval 无
  */
void I2C_DS3231_getTemperature(void)
{
	I2C_DS3231_ByteWrite(DS3231_CONTROL, 0x20|0x05);
	calendar.now_temperature=I2C_DS3231_DataRead(DS3231_TEMPERATUREH);
}

//闹钟设置
void I2C_DS3231_SetAlarm(uint8_t h, uint8_t m)
{
	I2C_DS3231_ByteWrite(0x0E, 0x1D);
	I2C_DS3231_ByteWrite(0x07, 0);
	I2C_DS3231_ByteWrite(0x08, DEC_BCD(m));
	I2C_DS3231_ByteWrite(0x09, DEC_BCD(h));
	I2C_DS3231_ByteWrite(0x0A, 0x81);
}

//清除闹钟
void I2C_DS3231_ClearAlarm(void)
{
	I2C_DS3231_ByteWrite(0x0E, 0x1C);
}

//读取闹钟时间
void I2C_DS3231_ReadAlarm(uint8_t *arr)
{
	uint8_t num;
	
	num = I2C_DS3231_DataRead(0x09);
	arr[0] = num >> 4;
	arr[1] = num & 0x0f;
	
	num = I2C_DS3231_DataRead(0x08);
	arr[2] = num >> 4;
	arr[3] = num & 0x0f;
}
	
//BCD(8421)转DEC.
uint8_t BCD_DEC(u8 val)
{
	u8 i;
	i= val&0x0f;
	val >>= 4;
	val &= 0x0f;
	val *= 10;
	i += val;    
	return i;
}
 
//转BCD
uint8_t DEC_BCD(u8 val)
{
  u8 i,j,k;
  i=val/10;
  j=val%10;
  k=j+(i<<4);
  return k;
}

