#include "client.h"

char  gHudMessage[256]="";

StopTimer gHudTimer;

void gSetHudMessage(const char* message)
{
	strcpy(gHudMessage,message);
	gHudTimer.countdown(4);
}

void DrawHudText()
{
	int line_y = g_Screen.iHeight/2*2-180;
	int line_x = 20;
		
	if(cvar.misc_hud_info)
		if(gHudTimer.running())
			DrawConString(line_x,line_y,255,255,255, gHudMessage);
}