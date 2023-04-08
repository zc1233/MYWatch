#include "Alarm.h"

void Alarm_Init(uint8_t *arr)
{
	OLED_ShowTimes(0,arr[0],1);
	OLED_ShowTimes(1,arr[1],1);
	OLED_ShowTimes(2,1,1);
	OLED_ShowTimes(3,arr[2],1);
	OLED_ShowTimes(4,arr[3],1);
}