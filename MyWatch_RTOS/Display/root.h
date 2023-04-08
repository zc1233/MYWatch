#ifndef __root_H
#define __root_H

#include "AllInclude.h"
#include "i2c.h"
#include "DS3231.h"

#define hrs calendar.hour
#define sec calendar.second
#define mins calendar.minute
#define years calendar.now_year
#define mons calendar.months
#define day calendar.dates
#define week calendar.now_week
#define temperature calendar.now_temperature

void OLED_ShowTimes(uint8_t place, uint8_t num, _Bool state);
void Root_Motion(void);

#endif