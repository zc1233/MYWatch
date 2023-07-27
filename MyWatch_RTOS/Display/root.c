#include "root.h"

//星期缩写
unsigned char *weeks[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

//root界面变化部分
void Root_Motion(void)
{
	OLED_ShowTimes(0,hrs/10,1);
	OLED_ShowTimes(1,hrs%10,1);
	
	OLED_ShowTimes(2,10,sec % 2);
	
	OLED_ShowTimes(3,mins/10,1);
	OLED_ShowTimes(4,mins%10,1);
	
	OLED_ShowTimes(5,years,1);
	OLED_ShowTimes(6,mons,1);
	OLED_ShowTimes(7,day,1);
	OLED_ShowTimes(8,week,1);
}

//年月日时显示
//位置 数字 显示与否
void OLED_ShowTimes(uint8_t place, uint8_t num, _Bool state)
{
	place = place % 9;
	if (place <= 4 && place != 2)
	{
		state ? OLED_ShowImage(4 + 24 * place,1,24,48,timeNumDisplay[num]) : OLED_ClearImage(4 + 24 * place,1,24,48);
		return;
	}
	
	unsigned char c[] = {num / 10 + '0',num % 10 + '0' , '/', '\0'};
	
	switch (place)
	{
		case 2:
			state ? OLED_ShowImage(4 + 24 * place,1,24,48,timeNumDisplay[10]) : OLED_ClearImage(4 + 24 * place,1,24,48);
			break;
		case 5:
			state ? OLED_ShowString(0,0,c,8):OLED_ClearStr(0,0,2,8);
			break;
		case 6:
			state ? OLED_ShowString(24,0,c,8):OLED_ClearStr(24,0,2,8);
			break;
		case 7:
			state ? OLED_ShowNum(48,0,num,2,8):OLED_ClearStr(48,0,2,8);
			break;
		case 8:
			state ? OLED_ShowString(104,0,weeks[num],8):OLED_ClearStr(104,0,3,8);
			break;
	}
}