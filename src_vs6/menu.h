#ifndef MENU_H
#define MENU_H

struct menu_entrys
{
	char title[1000];
	float* value;
	float min;
	float max;
	float step;
};

extern int menuSelect, menuItems, MenuActive;
extern float menutilte[64], MaxFps;

extern void DrawMenu();
extern int MenuKeyEvent(int keynum);

#endif
