#include "client.h"

menu_entrys menuEntry[1000];
int menuSelect, menuItems, MenuActive;
float menutilte[64];

void TrapezoidalMenu(int x, int y, int w, int h);

int AddEntry(int n, char title[1000], float* value, float min, float max, float step)
{
	strcpy( menuEntry[n].title,title);
	menuEntry[n].value = value;
	menuEntry[n].min = min;
	menuEntry[n].max = max;
	menuEntry[n].step = step;
	return (n+1);
}

float start=0, stop=0, save=0;
float wayclr1 = 0, wayclr2 = 0, wayclr3 = 0, wayclr4 = 0, wayclr5 = 0, wayclr6 = 0, wayclr7 = 0, wayclr8 = 0, wayclr9 = 0, wayclr10 = 0, wayclrall = 0;

void WayMenu()
{
	if(start){
		cmd.exec("way_record");
		start = 0;}
	if(stop){
		cmd.exec("way_record_stop");
		stop = 0;}
	/*
	if(save){
		cmd.exec("way_save");
		save = 0;}
	*/
	if(wayclr1){
		cmd.exec("way_clear 1");
		wayclr1=0;
	}
	if(wayclr2){
		cmd.exec("way_clear 2");
		wayclr2=0;
	}
	if(wayclr3){
		cmd.exec("way_clear 3");
		wayclr3=0;
	}
	if(wayclr4){
		cmd.exec("way_clear 4");
		wayclr4=0;
	}
	if(wayclr5) {
		cmd.exec("way_clear 5");
		wayclr5=0;
	}
	if(wayclr6){
		cmd.exec("way_clear 6");
		wayclr6=0;
	}
	if(wayclr7){
		cmd.exec("way_clear 7");
		wayclr7=0;
	}
	if(wayclr8){
		cmd.exec("way_clear 8");
		wayclr8=0;
	}
	if(wayclr9){
		cmd.exec("way_clear 9");
		wayclr9=0;
	}
	if(wayclr10){
		cmd.exec("way_clear 10");
		wayclr10=0;
	}
	if(wayclrall) {
		cmd.exec("way_clear_all");
		wayclrall=0;
	}
}

void InitMenu()
{
	int i = 0;

	i = AddEntry(i,"+ AIM", &menutilte[1], 0, 1, 1);
	if(menutilte[1])
	{
		/*
		i = AddEntry(i, "+ VEC Attack", &menutilte[13], 0, 1, 1);
		if(menutilte[13])
		{
			i = AddEntry(i, "     X Stand", &cvar.aim_x_stand, 1, 5, 1);
			i = AddEntry(i, "     Y Stand", &cvar.aim_y_stand, 1, 2, 1);
			i = AddEntry(i, "     Z Stand", &cvar.aim_z_stand, 1, 22, 1);
			i = AddEntry(i, "     X Running", &cvar.aim_x_running, 1, 5, 1);
			i = AddEntry(i, "     Y Running", &cvar.aim_y_running, 1, 2, 1);
			i = AddEntry(i, "     Z Running", &cvar.aim_z_running, 1, 22, 1);
			i = AddEntry(i, "     X Walking", &cvar.aim_x_walking, 1, 5, 1);
			i = AddEntry(i, "     Y Walking", &cvar.aim_y_walking, 1, 2, 1);
			i = AddEntry(i, "     Z Walking", &cvar.aim_z_walking, 1, 22, 1);
			i = AddEntry(i, "     X Duck", &cvar.aim_x_duck, 1, 5, 1);
			i = AddEntry(i, "     Y Duck", &cvar.aim_y_duck, 1, 2, 1);
			i = AddEntry(i, "     Z Duck", &cvar.aim_z_duck, 1, 25, 1);
			i = AddEntry(i, "     X Duckmove", &cvar.aim_x_duckmove, 1, 5, 1);
			i = AddEntry(i, "     Y Duckmove", &cvar.aim_y_duckmove, 1, 2, 1);
			i = AddEntry(i, "     Z Duckmove", &cvar.aim_z_duckmove, 1, 25, 1);
			i = AddEntry(i, "     X Jump", &cvar.aim_x_jump, 1, 2, 1);
			i = AddEntry(i, "     Y Jump", &cvar.aim_y_jump, 1, 5, 1);
			i = AddEntry(i, "     Z Jump", &cvar.aim_z_jump, 1, 13, 1);
		}
		*/
		i = AddEntry(i, "   Activate", &cvar.aim_activate, 0, 1, 1);
	    i = AddEntry(i, "   Auto", &cvar.aim_auto, 0, 1, 1);
		i = AddEntry(i, "   Mode",  &cvar.aim_mode, 1, 2, 1);
	    i = AddEntry(i, "   Auto FOV", &cvar.aim_auto_fov, 0, 360, 10);
	    i = AddEntry(i, "   Auto Punch", &cvar.aim_auto_punch, 0, 1, 1);
	    i = AddEntry(i, "   Color", &cvar.aim_color, 0, 1, 1);
		i = AddEntry(i, "   Team", &cvar.aim_team, 0, 1, 1);
	    i = AddEntry(i, "   Draw", &cvar.aim_drawaim, 0, 1, 1);
		i = AddEntry(i, "   Shield",  &cvar.aim_shield, 0, 1, 1);
	    i = AddEntry(i, "   Silent", &cvar.aim_silent, 0, 1, 1);
	    i = AddEntry(i, "   Psilent", &cvar.aim_psilent, 0, 1, 1);
	}
	i = AddEntry(i, "+ Trigger", &menutilte[2], 0, 1, 1);
	if(menutilte[2])
	{
		i = AddEntry(i, "   Activate",  &cvar.trigger_activate, 0, 1, 1);
		i = AddEntry(i, "   FOV",  &cvar.trigger_fov, 1, 60, 1);
	}
	i = AddEntry(i, "+ Knife", &menutilte[3], 0, 1, 1);
	if(menutilte[3])
	{
		i = AddEntry(i, "   Activate",  &cvar.knf_activate, 0, 1, 1);
		i = AddEntry(i, "   FOV",  &cvar.knf_fov, 1, 360, 1);
		i = AddEntry(i, "   Attack",  &cvar.knf_attack, 1, 2, 1);
	}
	i = AddEntry(i, "+ Spread", &menutilte[4], 0, 1, 1);
	if(menutilte[4])
	{
		i = AddEntry(i, "   Recoil",  &cvar.norecoil, 0, 1, 1);
		i = AddEntry(i, "   Visual",  &cvar.norecoil_visual, 0, 1, 1);
		i = AddEntry(i, "   Visible",  &cvar.norecoil_visible, 0, 1, 1);
		i = AddEntry(i, "   Value",  &cvar.norecoilvalue, 0, 3, 0.05f);
		i = AddEntry(i, "   Draw",  &cvar.norecoildraw, 0, 1, 1);
		i = AddEntry(i, "   Spread",  &cvar.nospread, 0, 1, 1);
		i = AddEntry(i, "   Visible",  &cvar.nospread_visible, 0, 1, 1);
		i = AddEntry(i, "   Draw",  &cvar.nospreaddraw, 0, 1, 1);
	}
	/*
	i = AddEntry(i, "+ Burst", &menutilte[5], 0, 1, 1);
	if(menutilte[5])
	{
		i = AddEntry(i, "   Ftime", &cvar.burst_ftime, 0, 1, 0.1f);
		i = AddEntry(i, "   Dtime", &cvar.burst_dtime, 0, 1, 0.1f);
		i = AddEntry(i, "   Sdelay", &cvar.burst_sdelay, 0, 1, 0.1f);
		i = AddEntry(i, "   Rdelay", &cvar.burst_rdelay, 0, 1, 0.1f);
	}
	
	i = AddEntry(i, "+ Burst Set", &menutilte[6], 0, 1, 1);
	if(menutilte[6])
	{
		i = AddEntry(i, "     Off",  &Offburst, 0, 1, 1);
		i = AddEntry(i, "     Short",  &Shortburst, 0, 1, 1);
		i = AddEntry(i, "     Medium",  &Mediumburst, 0, 1, 1);
		i = AddEntry(i, "     Fast",  &Fastburst, 0, 1, 1);
	}
	*/
	i = AddEntry(i,"+ Visual", &menutilte[14], 0, 1, 1);
	if(menutilte[14])
	{
		i = AddEntry(i, "+ ESP", &menutilte[88], 0, 1, 1);
		if(menutilte[88])
		{
			i = AddEntry(i, "   Box", &cvar.misc_box, 0, 1, 1);
			i = AddEntry(i, "   Back", &cvar.misc_esp_back, 0, 1, 1);
			i = AddEntry(i, "   Tint", &cvar.misc_tint, 0, 1, 0.05f);
			i = AddEntry(i, "   Team", &cvar.misc_esp_team, 0, 1, 1);
			i = AddEntry(i, "   Health", &cvar.misc_health, 0, 1, 1);
			i = AddEntry(i, "   Sound", &cvar.misc_soundesp, 0, 1, 1);
			i = AddEntry(i, "   Sound Time", &cvar.misc_soundtime, 0.25f, 3, 0.25f);
			i = AddEntry(i, "   Box W", &cvar.misc_box_width, 0.05f, 1, 0.05f);
			i = AddEntry(i, "   Box H", &cvar.misc_box_height, 0.05f, 1, 0.05f);
			i = AddEntry(i, "   Box Size+", &cvar.misc_box_size_up, 1, 50, 1);
			i = AddEntry(i, "   Box Size-", &cvar.misc_box_size_down, 1, 50, 1);
		}

		i = AddEntry(i, "   Name", &cvar.misc_name, 0, 1, 1);
		i = AddEntry(i, "   Weapon", &cvar.misc_weapon, 0, 1, 1);
		i = AddEntry(i, "   Sequence", &cvar.misc_sequence, 0, 1, 1);
		i = AddEntry(i, "   No Sky", &cvar.misc_nosky, 0, 1, 1);
		i = AddEntry(i, "   Lambert", &cvar.misc_lambert, 0, 2, 1);
		i = AddEntry(i, "   Nightmode", &cvar.misc_nightmode, 0, 1, 1);
		i = AddEntry(i, "   Wall", &cvar.misc_wall, 0, 1, 1);
		i = AddEntry(i, "   Wireframe", &cvar.misc_wirewall, 0, 1, 1);
	}

	i = AddEntry(i, "+ Kreedz", &menutilte[8], 0, 1, 1);
	if(menutilte[8])
	{
		i = AddEntry(i, "   Bhop", &cvar.kzbhop, 0, 1, 1);
		i = AddEntry(i, "   Bhop Duck", &cvar.kzbhop_autoduck, 0, 1, 1);
		i = AddEntry(i, "   Bhop Nsd", &cvar.kzbhop_nsd, 0, 1, 1);
		i = AddEntry(i, "   Fast Run", &cvar.kzfastrun, 0, 1, 1); 
		i = AddEntry(i, "   Fast Run Nsd", &cvar.kzfastrun_nsd, 0, 1, 1); 
		i = AddEntry(i, "   GStrafe", &cvar.kzgroundstrafe, 0, 1, 1);
		i = AddEntry(i, "   GS Bhop", &cvar.kzgstrafe_bhop, 0, 1, 1);
		i = AddEntry(i, "   GStrafe Nsd", &cvar.kzgstrafe_nsd, 0, 1, 1);
		i = AddEntry(i, "   GSStand Up", &cvar.kzgstrafe_standup, 0, 1, 1);
		i = AddEntry(i, "   Jump Bug", &cvar.kzjumpbug, 0, 1, 1);
		i = AddEntry(i, "   Jump BAuto", &cvar.kzjumpbugauto, 0, 1, 1);
		i = AddEntry(i, "   Jump BSlow", &cvar.kzjumpbugslowspeed, 0, 1, 1);
		i = AddEntry(i, "   Show KZ", &cvar.kzshow_kz, 0, 1, 1);
		i = AddEntry(i, "   Slow Down", &cvar.kzslowdown, 0, 1, 1);
		i = AddEntry(i, "   Strafe", &cvar.kzstrafehack, 0, 1, 1);
		i = AddEntry(i, "   Strafe Dir", &cvar.kzstrafeautodir, 0, 1, 1);
		i = AddEntry(i, "   Strafe Crazy", &cvar.kzstrafe_crazy, 0, 1, 1);
		i = AddEntry(i, "   Strafe Help", &cvar.kzstrafe_helper, 0, 1, 1);
		i = AddEntry(i, "   Strafe HBoost", &cvar.kzstrafe_helper_boost, 0, 1, 1);
		i = AddEntry(i, "   Strafe Inv", &cvar.kzstrafe_invisible, 0, 1, 1);
	}
	
	i = AddEntry(i,"+ Misc", &menutilte[7], 0, 1, 1);
	if(menutilte[7])
	{
		i = AddEntry(i, "+ AA Settings", &menutilte[15], 0, 1, 1);
		if(menutilte[15])
		{
			i = AddEntry(i, "   Spinbot", &cvar.misc_spin, 0, 2, 1);
			i = AddEntry(i, "   Pitch",  &cvar.misc_aa_pitch, 0, 4, 1);
			i = AddEntry(i, "   Yaw", &cvar.misc_aa_yaw, 0, 7, 1);
		}

		i = AddEntry(i, "   Quake Guns", &cvar.misc_quake, 0, 1, 1);
		i = AddEntry(i, "   Auto Pistol",  &cvar.misc_auto_pistol, 0, 1, 1);
		i = AddEntry(i, "   Chase", &cvar.misc_chasecam, 0, 1, 1);
		i = AddEntry(i, "   No HUD", &cvar.misc_nohud, 0, 1, 1);
	    i = AddEntry(i, "   No Flash", &cvar.misc_rem_flash, 0, 1, 1);
		i = AddEntry(i, "   No Refresh", &cvar.misc_norefresh, 0, 1, 1);
		i = AddEntry(i, "   Hook Say", &cvar.misc_sayid, 0, 1, 1);
	/*
	    i = AddEntry(i, "     Radar", &cvar.misc_radar, 0, 1, 1);
	    i = AddEntry(i, "     Radars", &cvar.misc_radars, 0, 300, 1);
	    i = AddEntry(i, "     Radarx", &cvar.misc_radarx, 90, 500, 1);
	    i = AddEntry(i, "     Radary", &cvar.misc_radary, 100, 500, 1);
		i = AddEntry(i, "     WavSpeed", &cvar.wavspeed, 1, 10, 1);
		i = AddEntry(i, "     Snap Time", &cvar.misc_snapshot_time, 1, 60, 1);
	*/
	}
	/*
	i = AddEntry(i, "+ Color", &menutilte[10], 0, 1, 1);
	if(menutilte[10])
	{
		i = AddEntry(i, "     Color: R", &cvar.color_red, 0, 1, 1);
		i = AddEntry(i, "     Color: G", &cvar.color_green, 0, 1, 1);
		i = AddEntry(i, "     Color: B", &cvar.color_blue, 0, 1, 1);
	}
	*/
	i = AddEntry(i, "+ Way", &menutilte[11], 0, 1, 1);
	if(menutilte[11])
	{
		i = AddEntry(i, "+ Settings", &menutilte[12], 0, 1, 1);
		if(menutilte[12])
		{
			i = AddEntry(i, "   Mode", &cvar.way_mode, 1, 2, 1);
			i = AddEntry(i, "   Draw", &cvar.way_draw, 0, 2, 1);
			i = AddEntry(i, "   Jump", &cvar.auto_jump, 0, 1, 1); 
			i = AddEntry(i, "   Visual", &cvar.way_drawvisuals, 0, 1, 1); 
			i = AddEntry(i, "   Direction", &cvar.auto_direction, 0, 1, 1);
		}

		i = AddEntry(i, "+ Record", &menutilte[20], 0, 1, 1);
		if(menutilte[20])
		{
			i = AddEntry(i, "   Start", &start, 0, 1, 1);
			i = AddEntry(i, "   Stop", &stop, 0, 1, 1);
		//	i = AddEntry(i, "   Save", &save, 0, 1, 1);
		}

		i = AddEntry(i, "+ Clear", &menutilte[21], 0, 1, 1);
		if(menutilte[21])
		{
				i = AddEntry(i, "   Clear 1",  &wayclr1, 0, 1, 1);
				i = AddEntry(i, "   Clear 2",  &wayclr2, 0, 1, 1);
				i = AddEntry(i, "   Clear 3",  &wayclr3, 0, 1, 1);
				i = AddEntry(i, "   Clear 4",  &wayclr4, 0, 1, 1);
				i = AddEntry(i, "   Clear 5",  &wayclr5, 0, 1, 1);
				i = AddEntry(i, "   Clear 6",  &wayclr6, 0, 1, 1);
				i = AddEntry(i, "   Clear 7",  &wayclr7, 0, 1, 1);
				i = AddEntry(i, "   Clear 8",  &wayclr8, 0, 1, 1);
				i = AddEntry(i, "   Clear 9",  &wayclr9, 0, 1, 1);
				i = AddEntry(i, "   Clear 10",  &wayclr10, 0, 1, 1);
				i = AddEntry(i, "   Clear All",  &wayclrall, 0, 1, 1);
		}
	}
	
	menuItems = i;
}

void DrawMenu()
{
	if(!MenuActive) return;
	
	WayMenu();
	InitMenu();

	int w = 110;
	int h = 20;
	int x;
	int y = 60;

	x = g_Screen.iWidth-w-h;
	{
		TrapezoidalMenu(x,y,w,h);
	}
}

int MenuKeyEvent(int keynum)
{
	if( keynum == 128 || keynum == 240) //uparrow || mwheelup
	{
		if( menuSelect>0 ) menuSelect--;
		else menuSelect = menuItems - 1;
		return 0;
	}
	else if( keynum == 129 || keynum == 239) //downarrow || mwheeldown
	{
		if( menuSelect<menuItems-1 ) menuSelect++;
		else menuSelect = 0;
		return 0;
	}
	else if( keynum == 130 || keynum == 241 ) //leftarrow || leftbutton
	{
		if( menuEntry[menuSelect].value )
		{
			menuEntry[menuSelect].value[0] -= menuEntry[menuSelect].step;
			if( menuEntry[menuSelect].value[0] < menuEntry[menuSelect].min )
				menuEntry[menuSelect].value[0] = menuEntry[menuSelect].max;
		}
		return 0;
	}
	else if( keynum == 131 || keynum == 242 ) //rightarrow || rightbutton
	{
		if( menuEntry[menuSelect].value )
		{
			menuEntry[menuSelect].value[0] += menuEntry[menuSelect].step;
			if( menuEntry[menuSelect].value[0] > menuEntry[menuSelect].max )
				menuEntry[menuSelect].value[0] = menuEntry[menuSelect].min;
		}
		return 0;
	}
	return 1;
}
