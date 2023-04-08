#include "changeTime.h"

//时间数字（时分数字）
uint8_t ChangeNums[] = {0,0,0,0,3,10,7,0};

//每个月多少天
uint8_t Mons_Days[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};

uint8_t TimeNumEffe[] = {3,10,6,10};

void TimeNums_Init(void)
{
	ChangeNums[0] = hrs / 10;
	ChangeNums[1] = hrs % 10;
	ChangeNums[2] = mins / 10;
	ChangeNums[3] = mins % 10;
	ChangeNums[4] = years;
	ChangeNums[5] = mons;
	ChangeNums[6] = day;
	ChangeNums[7] = week;
}

//显示改变时间的界面
void ChangeTime_Motion(void)
{
	OLED_ShowTimes(0,ChangeNums[0],1);
	OLED_ShowTimes(1,ChangeNums[1],1);
	OLED_ShowTimes(2,1,1);
	for (uint8_t i = 2; i <= 8; i++)
		OLED_ShowTimes(i + 1,ChangeNums[i],1);
}

//位置上数字处理
void ChangeNumEffe(uint8_t place)
{
	if (place <= 3)
		ChangeNums[place] = ChangeNums[place] % TimeNumEffe[place];
	else
	{
		switch (place)
		{
			case 6:
			{
				uint8_t mon_day = Mons_Days[ChangeNums[5]];
				if (ChangeNums[4] % 4 == 0 & ChangeNums[5] == 2)
					mon_day += 1;
				ChangeNums[6] = (ChangeNums[6] - 1) % mon_day + 1;
				break;
			}
			case 5:
				ChangeNums[5] = (ChangeNums[5] - 1) % 12 + 1;
				break;
			case 4:
				ChangeNums[4] = ChangeNums[4] % 100;
				break;
			case 7:
				ChangeNums[7] = ChangeNums[7] % 7;
				break;
		}
	}
	
}