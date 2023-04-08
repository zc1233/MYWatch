#include "Timing.h"

uint8_t Timing_Num[] = {0,0};

//计时动态界面
void Timing_Motion(void)
{
	OLED_ShowTimes(0,Timing_Num[0]/10,1);
	OLED_ShowTimes(1,Timing_Num[0]%10,1);
	
	OLED_ShowTimes(3,Timing_Num[1]/10,1);
	OLED_ShowTimes(4,Timing_Num[1]%10,1);
}

//计时归零
void Timing_Init(void)
{
	Timing_Num[0] = 0;
	Timing_Num[1] = 0;
}

