#include "Game_Elem.h"
#include "mpu6050.h"

#define size 4

//画小球
//x范围:0-124
//y范围:0-60
void OLED_ShowDot(uint8_t x, uint8_t y)
{
	uint8_t Start_Page = y / 8;
	_Bool page = (Start_Page * 8 + 8 - y < size);
	
	uint8_t num = 0;
	if (page)
	{
		OLED_Set_Pos(x, Start_Page);
		for (uint8_t k = 0; k < size; k++)
		{
			num = dot[k] << (y - Start_Page * 8);
			OLED_WR_DATA(num);
		}
		OLED_Set_Pos(x, Start_Page + 1);
		for (uint8_t k = 0; k < size; k++)
		{
			num = dot[k] >> (8 + Start_Page * 8 - y);
			OLED_WR_DATA(num);
		}
	}
	else
	{
		OLED_Set_Pos(x, Start_Page);
		for (uint8_t k = 0; k < size; k++)
		{
			num = dot[k] << (y - Start_Page * 8);
			OLED_WR_DATA(num);
		}
	}
}

//清除小球
void Clear_Dot(uint8_t x, uint8_t y)
{
		uint8_t Start_Page = y / 8;
	_Bool page = (Start_Page * 8 + 8 - y < size);
	
	if (page)
	{
		OLED_Set_Pos(x, Start_Page);
		for (uint8_t k = 0; k < size; k++)
			OLED_WR_DATA(0);
		OLED_Set_Pos(x, Start_Page + 1);
		for (uint8_t k = 0; k < size; k++)
			OLED_WR_DATA(0);
	}
	else
	{
		OLED_Set_Pos(x, Start_Page);
		for (uint8_t k = 0; k < size; k++)
			OLED_WR_DATA(0);
	}
}

//画中间那个圈
void OLED_ShowCir(void)
{
	uint8_t num = 0;
	OLED_Set_Pos(60, 3);
	for (uint8_t k = 0; k < 9; k++)
	{
		num = circle[k] << 4;
		OLED_WR_DATA(num);
	}
	OLED_Set_Pos(60, 4);
	for (uint8_t k = 9; k < 18; k++)
	{
		num = (circle[k] << 4) | ((0xF0 & circle[k - 9]) >> 4);
		OLED_WR_DATA(num);
	}
}

float k = -0.2;
int16_t x = 0;
int16_t y = 0;
_Bool My_Game(void)
{
	_Bool IsGameEnd = 0;
	int8_t vx = angle.KalmanAngleX * k;
	int8_t vy = angle.KalmanAngleY * k / 2;
	Clear_Dot(x,y);
	x -= vx;
	y += vy;
	
	if (x < 0) x = 0;
	else if (x > 124) x = 124;
	
	if (y < 0) y = 0;
	else if (y > 60) y = 60;
	
	OLED_ShowCir();
	OLED_ShowDot(x, y);
	
	if (x >= 60 && x <= 64 && y >= 28 && y <= 32 && vx*vx + vy*vy <= 4)
		IsGameEnd = 1;
	return IsGameEnd;
}

//初始化
void Game_Init(void)
{
	OLED_ShowCir();
	OLED_ShowDot(0, 0);
	x = 0;
	y = 0;
}
