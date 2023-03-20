#include "time.h"

//存储时分秒
uint8_t times[3] = {22,43,40};

//存储年月日和星期
uint8_t days[4] = {23,3,20,1};

//星期缩写
char *weeks[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

//每个月多少天
uint8_t Mons_Days[] = {31,28,31,30,31,30,31,31,30,31,30,31};

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint8_t led_flag = 0;
	if (htim == (&htim4))
	{
		led_flag++;
		if (led_flag == 100)
		{
			sec++;
			led_flag = 0;
		}
	}
}

//显示时分秒
void OLED_ShowTime(void)
{
	mins += sec / 60;
	sec = sec % 60;
	hrs += mins / 60;
	mins = mins % 60;
	day += hrs / 24;
	week += hrs / 24;
	hrs = hrs % 24;
	
	uint8_t w = 24;
	uint8_t h = 48;
	uint8_t fix = 4;
	OLED_ShowImage(fix,1,w,h,Get_TimeNum(hrs / 10));
	OLED_ShowImage(fix+w,1,w,h,Get_TimeNum(hrs % 10));
	sec % 2 == 0 ? OLED_ShowImage(fix+w*2,1,w,h,timeSplit) : OLED_ClearImage(fix+w*2,1,w,h);
	OLED_ShowImage(fix+w*3,1,w,h,Get_TimeNum(mins / 10));
	OLED_ShowImage(fix+w*4,1,w,h,Get_TimeNum(mins % 10));
}

//显示年月日和星期
void OLED_ShowDays(void)
{
	uint8_t mon_day = Mons_Days[mons];
	if (years % 4 == 0 & mons == 2)
		mon_day += 1;
	mons += day / (mon_day + 1);
	day = (day - 1) % mon_day + 1;
	years += mons / 13;
	mons = (mons -1) % 12 + 1;
	week = week % 7;
	
	char Days_Display[] = {years / 10 + '0', years % 10 + '0' , '/', mons / 10 + '0', mons % 10 + '0' , '/',day / 10 + '0', day % 10 + '0'};
	
	OLED_ShowString(0,0,Days_Display,8);
	OLED_ShowString(104,0,weeks[week],8);
}

uint8_t *Get_TimeNum(uint8_t x)
{
	switch (x)
	{
		case 0:
			return time0;
		case 1:
			return time1;
		case 2:
			return time2;
		case 3:
			return time3;
		case 4:
			return time4;
		case 5:
			return time5;
		case 6:
			return time6;
		case 7:
			return time7;
		case 8:
			return time8;
		case 9:
			return time9;
		default:
			return time9;
	}
}
	