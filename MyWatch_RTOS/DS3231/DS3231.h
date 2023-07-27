#ifndef __DS3231_H
#define __DS3231_H

#include "AnalogI2C.h"
 
#ifdef _I2C_DS3231_PUBLIC_
#define PEXT 
#else
#define PEXT extern
#endif

#define u8 uint8_t 
#define u32 uint32_t
#define u16 uint16_t
 
/* Exported types ------------------------------------------------------------*/
typedef struct
{
	u8  now_year;
	u8  months;
	u8  dates;
	u8  now_week;
	u8  hour;
	u8  minute;
	u8  second;
	u16 now_temperature;
}_calendar_obj;	
//�����ṹ��
extern _calendar_obj calendar;
 

/* �����ַֻҪ��STM32��ҵ�I2C������ַ��һ������ */
#define 						I2Cx_OWN_ADDRESS7     										 0X0A   
/* DS3231 ��ַ���� */
#define 						DS3231_ADDRESS 														 0xD0   
 
/* DS3231�Ĵ�����ַ */
																					
#define							DS3231_SECOND															 0x00    //��
#define 						DS3231_MINUTE      												 0x01    //��
#define 						DS3231_HOUR        												 0x02    //ʱ
#define 						DS3231_WEEK         											 0x03    //����
#define 						DS3231_DAY          											 0x04    //��
#define 						DS3231_MONTH                      		     0x05    //��
#define             DS3231_YEAR        						             0x06    //�� 	

/* ����1 */          	
#define             DS3231_SALARM1ECOND                        0x07    //��
#define 						DS3231_ALARM1MINUTE                        0x08    //��
#define             DS3231_ALARM1HOUR                          0x09    //ʱ
#define 						DS3231_ALARM1WEEK  												 0x0A    //����/��

/* ����2 */
#define 						DS3231_ALARM2MINUTE 											 0x0b    //��
#define 						DS3231_ALARM2HOUR                          0x0c    //ʱ
#define 						DS3231_ALARM2WEEK                          0x0d    //����/��
 
#define 						DS3231_CONTROL                             0x0e    //���ƼĴ���
#define 						DS3231_STATUS                              0x0f    //״̬�Ĵ���
#define 						BSY                 											 2       //æ
#define 						OSF                												 7       //����ֹͣ��־
#define 						DS3231_XTAL         											 0x10    //�����ϻ��Ĵ���
#define 						DS3231_TEMPERATUREH 											 0x11    //�¶ȼĴ������ֽ�(8λ)
#define 						DS3231_TEMPERATUREL 											 0x12    //�¶ȼĴ������ֽ�(��2λ) 																				


 
/* Exported functions ------------------------------------------------------- */																				
PEXT void I2C_DS3231_Init(void);	
PEXT uint32_t I2C_DS3231_ByteWrite(u8 WriteAddr, u8 data); 	
PEXT uint8_t I2C_DS3231_DataRead(u8 ReadAddr);																			
PEXT uint8_t BCD_DEC(u8 val);		
PEXT uint8_t DEC_BCD(u8 val);

//�꣬�£��գ����ڣ�Сʱ�����ӣ���
PEXT void I2C_DS3231_SetTime(u8 y,u8 ,u8 d,u8 w,u8 h,u8 m,u8 s);
 
PEXT void I2C_DS3231_getTime(void);
PEXT void I2C_DS3231_getTemperature(void);
PEXT void I2C_DS3231_SetAlarm(uint8_t h, uint8_t m);
PEXT void I2C_DS3231_ClearAlarm(void);
PEXT void I2C_DS3231_ReadAlarm(uint8_t *arr);
 
#undef PEXT
#endif

