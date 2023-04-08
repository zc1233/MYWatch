#ifndef __display_H__
#define __display_H__

#include "stdio.h"
#include "stdint.h"
#include "oled.h"

#include "Game_Font.h"

void OLED_ShowDot(uint8_t x, uint8_t y);
void OLED_ShowCir(void);
void Clear_Dot(uint8_t x, uint8_t y);
void Game_Init(void);
_Bool My_Game(void);

#endif