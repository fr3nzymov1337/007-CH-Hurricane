#include "client.h"

ColorManager colorList;

void ColorManager::Init()
{
	add(255,000,000,255);   // 0 - Terrorist         - Red
	add(000,100,255,255);   // 1 - Counter Terrorist - Blue
	add(000,255,000,255);   // 2 - Aimbot Target     - Green
	add(255,000,255,255);   // 3 - Menu Title        - Pink
	add(255,255,255,255);   // 4 - Con Text          - White
	add(000,000,000,255);   // 5 - Con Text2         - Black
	add(000,200,200,255);   // 6 - Way 1			 - LtBlue 1
	add(000,255,255,255);   // 7 - Way 2			 - LtBlue 2
	add(000,000,000,255);	// 8 - Sky				 - Black
}

ColorEntry * ColorManager::get(int index)
{
	if(index<entrys.size())
		return &entrys[index];
	else
		return &entrys[0];
}

void ColorManager::add(int r, int g, int b, int a)
{
	ColorEntry tmp;
	tmp.r = r;
	tmp.g = g;
	tmp.b = b;
	tmp.a = a;
	tmp.onebased_r = r/255;
	tmp.onebased_g = g/255;
	tmp.onebased_b = b/255;
	tmp.onebased_a = a/255;
	entrys.push_back(tmp);
	currentIndex++;
}

ColorEntry * PlayerColor(int i)
{	
	PlayerInfo& p = g_Player[i];
	sMe& l = g_Local;
	if(cvar.aim_activate && cvar.aim_color && i == iTargetID && (l.HudAlive || l.MsgAlive) && !IsExplosive(l.WpnID))
		return colorList.get(2);
	else if(p.Team == 1)
		return colorList.get(0);
	else if(p.Team == 2)
		return colorList.get(1);
	else
		return colorList.get(4);
}
