#ifndef __OLED_H
#define __OLED_H

#include "stdint.h"
#include "stdio.h"

//根据I2C所在引脚进行宏定义，方便后续操作
#define   GPIOx_OLED_PORT               GPIOB
#define   OLED_SCK_PIN                  GPIO_PIN_6
#define   OLED_SCK_ON()                 HAL_GPIO_WritePin(GPIOx_OLED_PORT, OLED_SCK_PIN, GPIO_PIN_SET)
#define   OLED_SCK_OFF()                HAL_GPIO_WritePin(GPIOx_OLED_PORT, OLED_SCK_PIN, GPIO_PIN_RESET)
#define   OLED_SCK_TOGGLE()             HAL_GPIO_TogglePin(GPIOx_OLED_PORT, OLED_SCK_PIN)
#define   OLED_SDA_PIN                  GPIO_PIN_7
#define   OLED_SDA_ON()                 HAL_GPIO_WritePin(GPIOx_OLED_PORT, OLED_SDA_PIN, GPIO_PIN_SET)
#define   OLED_SDA_OFF()                HAL_GPIO_WritePin(GPIOx_OLED_PORT, OLED_SDA_PIN, GPIO_PIN_RESET)
#define   OLED_SDA_TOGGLE()             HAL_GPIO_TogglePin(GPIOx_OLED_PORT, OLED_SDA_PIN)
///

void WriteCmd(void);
void OLED_WR_CMD(uint8_t cmd);
void OLED_WR_DATA(uint8_t data);
void OLED_Init(void);
void OLED_Clear(void);

void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Set_Pos(uint8_t x, uint8_t y);
void OLED_On(void);
void OLED_ShowNum(uint8_t x,uint8_t y,unsigned int num,uint8_t len,uint8_t size2);

void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size);
void OLED_ClearChar(uint8_t x,uint8_t y,uint8_t Char_Size);

void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size);
void OLED_ClearStr(uint8_t x,uint8_t y,uint8_t len,uint8_t Char_Size);

void OLED_ShowImage(uint8_t x,uint8_t y,uint8_t w,uint8_t h,const uint8_t *Image);
void OLED_ClearImage(uint8_t x,uint8_t y,uint8_t w,uint8_t h);
//void OLED_ShowImag


#endif