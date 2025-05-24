#include "client.h"

int x, y, ybox, ystep, boxradius, correct, ColorR, ColorG, ColorB;

float _h, box_height, box_width, ScreenTop[2], ScreenBot[2];

Vector Top, Bot;

bool Name(int i, int x, int y, ColorEntry *color)
{
	if(cvar.misc_name)
	{
		hud_player_info_t pinfo;
		g_pEngine->pfnGetPlayerInfo(i, &pinfo);
		char buf[1024];
		sprintf(buf,"%s",pinfo.name);
		DrawHudStringCenterEsp(x, y, color->r, color->g, color->b, cvar.misc_esp_back, cvar.misc_tint*255.0f, buf);
		return true;
	}
	return false;
}

bool Weapon(int i, int x, int y, ColorEntry *color)
{
	if(cvar.misc_weapon)
	{
		DrawHudStringCenterEsp(x, y, color->r, color->g, color->b, cvar.misc_esp_back, cvar.misc_tint*255.0f, "%s", GetWeaponName(g_pEngine->GetEntityByIndex(i)->curstate.weaponmodel)); 
		return true;
	}
	return false;
}

bool Sequence(int i, int x, int y, ColorEntry *color)
{
	if(cvar.misc_sequence)
	{
		int seqinfo = Cstrike_SequenceInfo[g_pEngine->GetEntityByIndex(i)->curstate.sequence];
		int atype = Cstrike_EGaitSequenceInfo[g_pEngine->GetEntityByIndex(i)->curstate.gaitsequence];

		if(atype == 0) 
			DrawHudStringCenterEsp(x, y, color->r, color->g, color->b, cvar.misc_esp_back, cvar.misc_tint*255.0f, "- STANDING -");				
		else if(atype == 1) 
			DrawHudStringCenterEsp(x, y, color->r, color->g, color->b, cvar.misc_esp_back, cvar.misc_tint*255.0f, "- CROUCHING -");
		else if(atype == 2) 
			DrawHudStringCenterEsp(x, y, color->r, color->g, color->b, cvar.misc_esp_back, cvar.misc_tint*255.0f, "- WALKING -");	
		else if(atype == 3) 
			DrawHudStringCenterEsp(x, y, color->r, color->g, color->b, cvar.misc_esp_back, cvar.misc_tint*255.0f, "- RUNNING -");	
		else if(atype == 4) 
			DrawHudStringCenterEsp(x, y, color->r, color->g, color->b, cvar.misc_esp_back, cvar.misc_tint*255.0f, "- CROUCH_RUN -");	
		else if(atype == 5) 
			DrawHudStringCenterEsp(x, y, color->r, color->g, color->b, cvar.misc_esp_back, cvar.misc_tint*255.0f, "- JUMPING -");
		else if(atype == 6) 
			DrawHudStringCenterEsp(x, y, color->r, color->g, color->b, cvar.misc_esp_back, cvar.misc_tint*255.0f, "- SWIMMING -");
		if(seqinfo == 99 || seqinfo == 100)
			DrawHudStringCenterEsp(x, y, color->r, color->g, color->b, cvar.misc_esp_back, cvar.misc_tint*255.0f, "- HIT -");
		else if(seqinfo == 8)
			DrawHudStringCenterEsp(x, y, color->r, color->g, color->b, cvar.misc_esp_back, cvar.misc_tint*255.0f, "- THROWING -");
		else if(seqinfo == 32)
			DrawHudStringCenterEsp(x, y, color->r, color->g, color->b, cvar.misc_esp_back, cvar.misc_tint*255.0f, "- SHIELD TO SIDE -");	
		else if(seqinfo == 64)
			DrawHudStringCenterEsp(x, y, color->r, color->g, color->b, cvar.misc_esp_back, cvar.misc_tint*255.0f, "- SHIELDED -");
		else if(seqinfo == 1)
			DrawHudStringCenterEsp(x, y, color->r, color->g, color->b, cvar.misc_esp_back, cvar.misc_tint*255.0f, "- SHOOTING -");
		else if(seqinfo == 2){
			DrawHudStringCenterEsp(x, y, color->r, color->g, color->b, cvar.misc_esp_back, cvar.misc_tint*255.0f, "- RELOADING -");	
			if(cvar.misc_tint*255.0f && cvar.misc_esp_back){
				FillRGBA(x-26, y+1, 53, 12, 0, 0, 0, cvar.misc_tint*255.0f);
				FillRGBA(x-25, y+2, (g_pEngine->GetEntityByIndex(i)->curstate.frame/255)*50, 10, 0, 255, 0, cvar.misc_tint*255.0f);}}
		else if(seqinfo == 5 || seqinfo == 16)
			DrawHudStringCenterEsp(x, y, color->r, color->g, color->b, cvar.misc_esp_back, cvar.misc_tint*255.0f, "- PLANTING C4 -");
		return true;
	}
	return false;
}

void Radar(Vector origin, ColorEntry *color)
{
	if(cvar.misc_radar)
		DrawRadarPoint(origin, color->r, color->g, color->b, 3, 3, true);
}

void Box(ColorEntry *color)
{
	if(cvar.misc_box)
	{
		if(cvar.misc_esp_back)
		{
			ColorEntry *black = colorList.get(5);
			DrawLines(ScreenTop[0]-box_width+1,ScreenTop[1],ScreenTop[0]+box_width-2,ScreenTop[1],3,black,cvar.misc_tint*255.0f); //bot
			DrawLines(ScreenTop[0]-box_width+1,ScreenTop[1]+box_height,ScreenTop[0]+box_width-2,ScreenTop[1]+box_height,3,black,cvar.misc_tint*255.0f); //top
			DrawLines(ScreenTop[0]-box_width,ScreenTop[1],ScreenTop[0]-box_width,ScreenTop[1]+box_height,3,black,cvar.misc_tint*255.0f); //right
			DrawLines(ScreenTop[0]+box_width,ScreenTop[1],ScreenTop[0]+box_width,ScreenTop[1]+box_height,3,black,cvar.misc_tint*255.0f);//left
		}
		DrawLines(ScreenTop[0]-box_width,ScreenTop[1],ScreenTop[0]+box_width-1,ScreenTop[1],1,color,cvar.misc_tint*255.0f);// bot
		DrawLines(ScreenTop[0]-box_width,ScreenTop[1]+box_height,ScreenTop[0]+box_width,ScreenTop[1]+box_height,1,color,cvar.misc_tint*255.0f);	//top
		DrawLines(ScreenTop[0]-box_width,ScreenTop[1],ScreenTop[0]-box_width,ScreenTop[1]+box_height,1,color,cvar.misc_tint*255.0f); //right
		DrawLines(ScreenTop[0]+box_width,ScreenTop[1],ScreenTop[0]+box_width,ScreenTop[1]+box_height,1,color,cvar.misc_tint*255.0f);//left
	}
}

void GetScreen(Vector Player)
{
	Top = Vector(Player.x, Player.y, Player.z - (cvar.misc_box_size_up*cvar.misc_box_height+4));
	Bot = Vector(Player.x, Player.y, Player.z + cvar.misc_box_size_down);
}

void CalculateScreen(int i)
{
	PlayerInfo& p = g_Player[i];
	_h = ScreenBot[1] - ScreenTop[1];
	box_height = p.Ducked ? _h : _h * cvar.misc_box_height;
	box_width = box_height * cvar.misc_box_width;

	x = ScreenTop[0];
	y = ScreenTop[1] - 17 + box_height;
	ystep = 16;
	y-=2;
}

void Ystep()
{
	y -= ystep;
}

void HealthBar(int i)
{
	if(cvar.misc_health)
	{
		float health_width = box_width - 4;

		unsigned int hp = g_Player[i].iHealth;

		if (hp < 0)
			hp = 0;
		else if (hp > 100)
			hp = 100;

		byte Red = 255 - (hp * 2.55);
		byte Green = hp * 2.55;

		float health_height = (box_height / 100) * hp;

		if(cvar.misc_esp_back)
			DrawLines(ScreenTop[0] + health_width,ScreenTop[1]+2,ScreenTop[0]+health_width,ScreenTop[1]+health_height-1,3, 0, 0, 0,cvar.misc_tint*255.0f);
		DrawLines(ScreenTop[0] + health_width,ScreenTop[1]+1,ScreenTop[0]+health_width,ScreenTop[1]+health_height,1, Red, Green, 0,cvar.misc_tint*255.0f);
	}
}

void DrawEsp(int i, Vector Player)
{
	if(isEnemy(i) || cvar.misc_esp_team)
	{
		ColorEntry *color = PlayerColor(i);
		
		Radar(Player, color);

		GetScreen(Player);

		if(WorldToScreen(Top, ScreenTop) && WorldToScreen(Bot, ScreenBot))
		{
			CalculateScreen(i);

			HealthBar(i);

			Box(color);
			
			if(Name(i, x, y, color))
				Ystep();

			if(Weapon(i, x, y, color))
				Ystep();

			if(Sequence(i, x, y, color))
				Ystep();
		}
	}
}

void PlayerEsp()
{
	for(int i=0;i<33;i++)
	{
		PlayerInfo& p = g_Player[i];
		sMe& l = g_Local;

		if(cvar.rush && (cvar.misc_nohud || cvar.misc_norefresh)) return;

		if(i == l.iIndex || p.IsValidEnt || p.previousSound.dwTime + cvar.misc_soundtime*1000 <= GetTickCount()){
			SoundClear(p.previousSound);}
		else
			DrawEsp(i, p.previousSound.vOrigin);
		
		if(p.IsValidEnt)
			DrawEsp(i, p.pEnt->origin);
	}
}
