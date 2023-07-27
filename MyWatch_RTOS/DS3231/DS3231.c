#include "DS3231.h"

_calendar_obj calendar  = {23,3,27,0,17,39,0,0};

void I2C_DS3231_Init(void)
{
  I2C_GPIO_Config(); 
	
	/* ��һ��ֹͣ�ź�, ��λI2C�����ϵ������豸������ģʽ */
	i2c_Stop();
}


//
/**
  * @brief	дһ���ֽڵ�I2C DS3231��
  * @param   
  *		@arg data:Ҫд����ֽ�
  *		@arg WriteAddr:д��ַ 
	* @retval ����1����ʾд��ɹ�.
  */
uint32_t I2C_DS3231_ByteWrite(u8 WriteAddr, u8 data) 
{
	/* ��1��������I2C���������ź� */
	i2c_Start();
	
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	i2c_SendByte(DS3231_ADDRESS| I2C_WR);	/* �˴���дָ�� */
	
	/* ��3�����ȴ�ACK */
	if (i2c_WaitAck() != 0)
	{
 		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	
	/* ��4�������ͼĴ�����ַ */
	i2c_SendByte((uint8_t)WriteAddr);
	
  /* ��5�����ȴ�ACK */
	if (i2c_WaitAck() != 0)
	{
	  goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	
	/* ��6������ʼд������ */
	i2c_SendByte(data);
	
	/* ��7�����ȴ�ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	
  /* ����ִ�гɹ�������I2C����ֹͣ�ź� */
	i2c_Stop();
	return 1;
cmd_fail: /* ����ִ��ʧ�ܺ󣬷���ֹͣ�ź� */
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();
	return 0;
}
 
/**
  * @brief	��DS3231�����ȡһ���ֽ����� 
  * @param   
  *		@arg data:��Ŵ�DS3231��ȡ������
  *		@arg ReadAddr:��ȡ���ݵ�DS3231�ĵ�ַ
	* @retval data:��������.
  */
uint8_t I2C_DS3231_DataRead(u8 ReadAddr)
{
	uint8_t data;
 
	/* ��1��������I2C���������ź� */
	i2c_Start();
 
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	i2c_SendByte(DS3231_ADDRESS|I2C_WR);	/* �˴���дָ�� */
  
	/* ��3�����ȴ�ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	
	/* ��4��������DS3231�Ĵ�����ַ */
	i2c_SendByte((uint8_t)ReadAddr);
	
	/* ��5�����ȴ�ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	
	/* ��6���������ڶ��� I2C ��ʼ�ź� */  
  	i2c_Start();
	
	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	//i2c_SendByte(DS3231_ADDRESS | I2C_RD);	/* �˴��Ƕ�ָ�� */
	i2c_SendByte(DS3231_ADDRESS | I2C_RD);	/* �˴��Ƕ�ָ�� */
	
	/* ��8��������ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}	
   
  	/* ��9������ȡ���� */
	data = i2c_ReadByte(); 
 
  	i2c_NAck();	
	i2c_Stop();
	return data;
	
  	cmd_fail: /* ����ִ��ʧ�ܺ󣬷���ֹͣ�ź� */
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();
	return 0;
}
 
/**
  * @brief	ʱ������
  * @param   
  *		@arg 	�ֱ����� �� �� �� ���� ʱ �� ��
  * @retval ��
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
  * @brief	��ȡʱ��
  * @param   
  *		@arg pBuffer:��Ŵ�DS3231��ȡ�����ݵĻ�����ָ��
  *		@arg ReadAddr:��ȡ���ݵ�DS3231�ĵ�ַ
  *   @arg NumByteToWrite:Ҫ��DS3231��ȡ���ֽ���
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
  * @brief	��ȡ�¶�
  * @param  ��
  * @retval ��
  */
void I2C_DS3231_getTemperature(void)
{
	I2C_DS3231_ByteWrite(DS3231_CONTROL, 0x20|0x05);
	calendar.now_temperature=I2C_DS3231_DataRead(DS3231_TEMPERATUREH);
}

//��������
void I2C_DS3231_SetAlarm(uint8_t h, uint8_t m)
{
	I2C_DS3231_ByteWrite(0x0E, 0x1D);
	I2C_DS3231_ByteWrite(0x07, 0);
	I2C_DS3231_ByteWrite(0x08, DEC_BCD(m));
	I2C_DS3231_ByteWrite(0x09, DEC_BCD(h));
	I2C_DS3231_ByteWrite(0x0A, 0x81);
}

//�������
void I2C_DS3231_ClearAlarm(void)
{
	I2C_DS3231_ByteWrite(0x0E, 0x1C);
}

//��ȡ����ʱ��
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
	
//BCD(8421)תDEC.
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
 
//תBCD
uint8_t DEC_BCD(u8 val)
{
  u8 i,j,k;
  i=val/10;
  j=val%10;
  k=j+(i<<4);
  return k;
}

