#include "AnalogI2C.h"

/**
  * @brief  I2C I/O����
  * @param  None
  * @retval None
  */
void I2C_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	/* ʹ���� I2C �йص�ʱ�� */
  	__HAL_RCC_GPIOC_CLK_ENABLE();           	// ʹ��GPIOC�˿�ʱ��
	
	/* I2C_SCL��I2C_SDA*/
  GPIO_Initure.Mode=GPIO_MODE_OUTPUT_OD;  	//��©���
	GPIO_Initure.Pull=GPIO_PULLUP;          	//����
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;    	 	//����	
	GPIO_Initure.Pin=I2C_SCL_PIN|I2C_SDA_PIN; 						
  HAL_GPIO_Init(I2C_SCL_PORT,&GPIO_Initure);
  HAL_GPIO_WritePin(I2C_SCL_PORT,I2C_SDA_PIN|I2C_SCL_PIN,GPIO_PIN_SET);	//��ʼ������1
}
 
/**
  * @brief  I2C����λ�ӳ٣����400KHz
  * @param  None
  * @retval None
  */
void i2c_Delay(void)
{
	uint8_t i;
 
	/*��
	 	�����ʱ����ͨ���߼������ǲ��Եõ��ġ�
    ����������CPU��Ƶ72MHz ��MDK���뻷����1���Ż�
  
		ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz 
		ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us 
	 	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us 
	*/
	for (i = 0; i < 10; i++);
}
 
/**
  * @brief  I2C���������ź�
  * @param  None
  * @retval None
  */
void i2c_Start(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
	I2C_SDA_1();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_0();
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
}
 
/**
  * @brief  I2C����ֹͣ�ź�
  * @param  None
  * @retval None
  */
void i2c_Stop(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
	I2C_SDA_0();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_1();
}
 
/**
  * @brief  MCU��I2C�����豸����8bit����
  * @param   
  *		@arg 	_ucByte:���͵��ֽ�
	* @retval None
  */
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;
 
	/* �ȷ����ֽڵĸ�λbit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			I2C_SDA_1();
		}
		else
		{
			I2C_SDA_0();
		}
		i2c_Delay();
		I2C_SCL_1();
		i2c_Delay();	
		I2C_SCL_0();
		if (i == 7)
		{
			 I2C_SDA_1(); // �ͷ�����
		}
		_ucByte <<= 1;	/* ����һ��bit */
		i2c_Delay();
	}
}
 
/**
  * @brief  MCU��I2C�����豸��ȡ8bit����
  * @param  None
	* @retval ����������
  */
uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;
 
	/* ������1��bitΪ���ݵ�bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		I2C_SCL_1();
		i2c_Delay();
		if (I2C_SDA_READ())
		{
			value++;
		}
		I2C_SCL_0();
		i2c_Delay();
	}
	return value;
}
 
/**
  * @brief  MCU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
  * @param  None
	* @retval ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
  */
uint8_t i2c_WaitAck(void)
{
	uint8_t re;
 
	I2C_SDA_1();	/* CPU�ͷ�SDA���� */
	i2c_Delay();
	I2C_SCL_1();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	i2c_Delay();
	if (I2C_SDA_READ())	/* CPU��ȡSDA����״̬ */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	I2C_SCL_0();
	i2c_Delay();
	return re;
}
 
/**
  * @brief  MCU����һ��ACK�ź�
  * @param  None
	* @retval None
  */
void i2c_Ack(void)
{
	I2C_SDA_0();	/* CPU����SDA = 0 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU����1��ʱ�� */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
	I2C_SDA_1();	/* CPU�ͷ�SDA���� */
}
 
/**
  * @brief  MCU����1��NACK�ź�
  * @param  None
	* @retval None
  */
void i2c_NAck(void)
{
	I2C_SDA_1();	/* CPU����SDA = 1 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU����1��ʱ�� */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();	
}
 
/**
  * @brief  ���I2C�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ����
  * @param   
  *		@arg  _Address:�豸��I2C���ߵ�ַ
	* @retval ����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
  */
uint8_t i2c_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;
 
	I2C_GPIO_Config();		/* ����GPIO */
	i2c_Stop();
 
	
	i2c_Start();		/* ���������ź� */
 
	/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
	i2c_SendByte(_Address | I2C_WR);
	ucAck = i2c_WaitAck();	/* ����豸��ACKӦ�� */
 
	i2c_Stop();			/* ����ֹͣ�ź� */
 
	return ucAck;
}