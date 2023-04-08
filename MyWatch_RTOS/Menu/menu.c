#include "menu.h"
	
void Display_MenuLogo(uint8_t x)
{
	if (x <= 3)
	{
			OLED_ShowImage(32,0,64,64,MenuElement[x]);
	}
	else 
	{
		x == Menu_Num ? OLED_ShowImage(0,3,13,16,MenuElement[x]) : OLED_ShowImage(115,3,13,16,MenuElement[x]);
	}
}

void Menu_Static(void)
{
	Display_MenuLogo(Menu_Num);
	Display_MenuLogo(Menu_Num + 1);
	Display_Choice(1);
}