#ifndef __changeTime_H
#define __changeTime_H

#include "AllInclude.h"
extern uint8_t ChangeNums[];
extern uint8_t DaysNums[];
extern uint8_t Mons_Days[];
extern uint8_t TimeNumEffe[];

void TimeNums_Init(void);

void ChangeTime_Motion(void);
void ChangeNumEffe(uint8_t place);
void Target_Flash(uint8_t x);

 #endif