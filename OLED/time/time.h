#ifndef __time_H
#define __time_H

#include "tim.h"
#include "oled.h"
#include "font.h"
#include "stdint.h"
#include "stdio.h"

extern uint8_t times[3];
extern uint8_t days[4];

#define hrs times[0]
#define sec times[2]
#define mins times[1]
#define years days[0]
#define mons days[1]
#define day days[2]
#define week days[3]

void OLED_ShowTime(void);
void OLED_ShowDays(void);
uint8_t *Get_TimeNum(uint8_t x);


#endif