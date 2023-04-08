#include "cartoon.h"

void Kunkun_dance(uint8_t x)
{
	OLED_ShowImage(0,0,128,64,Video_Images[x]);
}