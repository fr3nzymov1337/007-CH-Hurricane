#include "client.h"

sMe g_Local;
PlayerInfo g_Player[33];

bool CheckEntity(cl_entity_s *pEnt)
{
	sMe& l = g_Local;
	if ((pEnt->index != l.iIndex) && !(pEnt->curstate.effects & EF_NODRAW) && pEnt->player && (pEnt->model->name != 0 || pEnt->model->name != "") && !(pEnt->curstate.messagenum < g_Engine.GetLocalPlayer()->curstate.messagenum))
		return true;
	else
		return false;
}

bool CheckEntity(int iIndex)
{
	sMe& l = g_Local;
	cl_entity_s *pEnt = g_pEngine->GetEntityByIndex(iIndex);
	if ((pEnt->index != l.iIndex) && !(pEnt->curstate.effects & EF_NODRAW) && pEnt->player && (pEnt->model->name != 0 || pEnt->model->name != "") && !(pEnt->curstate.messagenum < g_Engine.GetLocalPlayer()->curstate.messagenum))
		return true;
	else
		return false;
}


bool ValidEntity(cl_entity_s *pEnt)
{
	if(pEnt->player && g_Local.iIndex != pEnt->index && pEnt->curstate.movetype != 6 && 
		 pEnt->curstate.movetype != 0 && !( pEnt->curstate.messagenum<g_Engine.GetLocalPlayer()->curstate.messagenum ) &&
		 !( g_Engine.GetLocalPlayer()->curstate.iuser1 == 4 && g_Engine.GetLocalPlayer()->curstate.iuser2 == pEnt->index ) )
	{
		return true;
	}

	return false;
}

bool CheckTeam(int iIndex)
{
	PlayerInfo& p = g_Player[iIndex];
	sMe& l = g_Local;

	if(cvar.aim_team == 0 && l.Team == p.Team)
		return false;

	return true;
}

bool Visible(int iIndex) 
{
	PlayerInfo& p = g_Player[iIndex];

	if(!p.AimVisible)
		return false; 

	return true;
}

void g_LocalpEnt()
{
	sMe& l = g_Local;
	l.pEnt = g_pEngine->GetLocalPlayer();
}

void g_LocaliIndex()
{
	sMe& l = g_Local;
	l.iIndex = l.pEnt->index;
}

void g_LocalAlive()
{
	sMe& l = g_Local;
	l.HudAlive = l.pEnt && !(l.pEnt->curstate.effects & EF_NODRAW) && l.pEnt->player && l.pEnt->curstate.movetype != 6 && l.pEnt->curstate.movetype != 0;
}

float GetDist(float *from, float *to) 
{ 
    float ang[3]; 
    ang[0] = to[0] - from[0]; 
    ang[1] = to[1] - from[1]; 
    ang[2] = to[2] - from[2]; 
    return (float)sqrt(ang[0] * ang[0] + ang[1] * ang[1] + ang[2] * ang[2]); 
}


void VectorAnglesAimbot(const float *forward,float *angles)
{
	float tmp, yaw, pitch;
	if (forward[1] == 0 && forward[0] == 0){
		yaw = 0;
		if (forward[2] > 0)
			pitch = 270;
		else
			pitch = 90;
	}else{
		yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
		if (yaw < 0)
			yaw += 360;
		tmp = sqrt (forward[0]*forward[0] + forward[1]*forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
		if (pitch < 0)
			pitch += 360;
	}
	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
	while(angles[0]<-89){angles[0]+=180;angles[1]+=180;}
	while(angles[0]>89){angles[0]-=180;angles[1]+=180;}
	while(angles[1]<-180){angles[1]+=360;}
	while(angles[1]>180){angles[1]-=360;}
}

void VectorAngles( const float *forward, float *angles )
{
	float tmp, yaw, pitch;
	if(forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if( forward[2] > 0 )
			pitch = 90.0f;
		else
			pitch = 270.0f;
	}else{
		yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
		if( yaw < 0 )
			yaw += 360.0f;
		tmp = sqrt( forward[0] * forward[0] + forward[1] * forward[1] );
		pitch = (atan2(forward[2], tmp) * 180 / M_PI);
	}
	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}

bool IsKnife(int id)
{
	if (id == WEAPONLIST_KNIFE)
		return true;

	return false;
}

bool IsExplosive(int id)
{
	if (id == WEAPONLIST_C4 || id == WEAPONLIST_FLASHBANG || id == WEAPONLIST_HEGRENADE|| id == WEAPONLIST_SMOKEGRENADE)
		return true;

	return false;
}

bool IsSecondary(int id)
{
	if (id == WEAPONLIST_USP || id == WEAPONLIST_DEAGLE || id == WEAPONLIST_GLOCK18 || id == WEAPONLIST_P228 || id == WEAPONLIST_ELITE || id == WEAPONLIST_FIVESEVEN)
		return true;

	return false;
}

bool IsShielded(int iIndex)
{
	int seqinfo = Cstrike_SequenceInfo[g_pEngine->GetEntityByIndex(iIndex)->curstate.sequence];

	if(seqinfo == 32 || seqinfo == 64)
		return true;

	return false;
}

void ApplySilentAngles(float *angles, usercmd_s *cmd, bool add_angle)
{
	sMe& l = g_Local;

	cl_entity_t *pLocal;
	Vector viewforward, viewright, viewup, aimforward, aimright, aimup, vTemp;
	float newforward, newright, newup;
	float forward = cmd->forwardmove;
	float right = cmd->sidemove;
	float up = cmd->upmove;

	pLocal = g_Engine.GetLocalPlayer();
	if (!pLocal) return;

	if (pLocal->curstate.movetype == MOVETYPE_WALK){ g_Engine.pfnAngleVectors(Vector(0.0f, cmd->viewangles.y, 0.0f), viewforward, viewright, viewup); }
	else{ g_Engine.pfnAngleVectors(cmd->viewangles, viewforward, viewright, viewup); }

	if (pLocal->curstate.movetype == MOVETYPE_WALK && !(l.MoveType == 5))
	{
		if (!add_angle){
			cmd->viewangles.x = angles[0];
			cmd->viewangles.y = angles[1];
		}else{
			cmd->viewangles.x += angles[0];
			cmd->viewangles.y += angles[1];
		}
	}

	if (pLocal->curstate.movetype == MOVETYPE_WALK){ g_Engine.pfnAngleVectors(Vector(0.0f, cmd->viewangles.y, 0.0f), aimforward, aimright, aimup); }
	else { g_Engine.pfnAngleVectors(cmd->viewangles, aimforward, aimright, aimup); }

	newforward = DotProduct(forward * viewforward.Normalize(), aimforward) + DotProduct(right * viewright.Normalize(), aimforward) + DotProduct(up * viewup.Normalize(), aimforward);
	newright = DotProduct(forward * viewforward.Normalize(), aimright) + DotProduct(right * viewright.Normalize(), aimright) + DotProduct(up * viewup.Normalize(), aimright);
	newup = DotProduct(forward * viewforward.Normalize(), aimup) + DotProduct(right * viewright.Normalize(), aimup) + DotProduct(up * viewup.Normalize(), aimup);

	if (angles[0]>81){ cmd->forwardmove = -newforward; }
	else { cmd->forwardmove = newforward; }

	cmd->sidemove = newright;
	cmd->upmove = newup;
}

void g_LocalTeam()
{
	for(int i=0;i<33;i++)
	{
		if(g_Local.Team == 0)
		{
			cl_entity_t *pLocal = g_Engine.GetLocalPlayer(); 
			hud_player_info_t pinfo;
			g_pEngine->pfnGetPlayerInfo(i, &pinfo);

			if(i == pLocal->index)
			{
				if( strstr ( pinfo.model, "arctic\0" )   || 
					strstr ( pinfo.model, "guerilla\0" ) || 
					strstr ( pinfo.model, "leet\0" )     || 
					strstr ( pinfo.model, "terror\0" )   || 
					strstr ( pinfo.model, "militia\0" ) )
				{
					g_Local.Team = 1;
				}
				else if( strstr ( pinfo.model, "gign\0" )      || 
						 strstr ( pinfo.model, "gsg9\0" )      || 
						 strstr ( pinfo.model, "sas\0" )       || 
						 strstr ( pinfo.model, "urban\0" )     || 
						 strstr ( pinfo.model, "vip\0" )       || 
						 strstr ( pinfo.model, "spetsnaz\0" ) )
				{
					g_Local.Team = 2;
				}
			}
		}
	}
}

void g_PlayerTeam()
{
	for(int i=0;i<33;i++)
	{
		if(g_Player[i].Team == 0)
		{
		cl_entity_s *ent = g_Engine.GetEntityByIndex(i);
		
		hud_player_info_t pinfo;
		g_pEngine->pfnGetPlayerInfo(ent->index, &pinfo);

		if( strstr ( pinfo.model, "arctic\0" )   || 
			strstr ( pinfo.model, "guerilla\0" ) || 
			strstr ( pinfo.model, "leet\0" )     || 
			strstr ( pinfo.model, "terror\0" )   || 
			strstr ( pinfo.model, "militia\0" ) ) 
			{
				g_Player[i].Team = 1;
			}
			else if( strstr ( pinfo.model, "gign\0" )      || 
					 strstr ( pinfo.model, "gsg9\0" )      || 
					 strstr ( pinfo.model, "sas\0" )       || 
					 strstr ( pinfo.model, "urban\0" )     || 
					 strstr ( pinfo.model, "vip\0" )       || 
					 strstr ( pinfo.model, "spetsnaz\0" ) ) 
			{
				g_Player[i].Team = 2;
			}
		}
	}
}

void ContinueFire( struct usercmd_s* usercmd )
{
	sMe& l = g_Local;
	if (usercmd->buttons&IN_ATTACK || 
		usercmd->buttons&IN_ATTACK2 && IsKnife(l.WpnID) ||
		!l.Clip && cvar.wavrspeed && iTargetID!=0 && cvar.wavaspeed>1){
		if(cvar.misc_auto_pistol && IsSecondary(l.WpnID) && l.Clip)
		{
			static bool flipFlop = true;
			if(flipFlop){ usercmd->buttons |= IN_ATTACK;    }
			else        { usercmd->buttons &= (~IN_ATTACK); }
			flipFlop = !flipFlop;
		}
		if(cvar.wavaspeed>1  && !IsExplosive(l.WpnID))
			cvar.wavspeed = cvar.wavaspeed;
	} 
	else 
	{
		if (cvar.wavaspeed>1)
			cvar.wavspeed = 1; 
	}
}

char* GetWeaponName(int weaponmodel) 
{ 
    static char weapon[50]; 
    weapon[0]=0; 

    model_s* mdl = g_pStudio->GetModelByIndex( weaponmodel );
    if( !mdl ){ return weapon; } 

    char* name = mdl->name;  if( !name )          { return weapon; } 
    int len = strlen(name);  if( len>48 || len<10){ return weapon; } 

    strcpy(weapon,name+9); len -=9; 
    if(len>4)weapon[len-4]=(char)0; 

    return weapon; 
}

int Cstrike_SequenceInfo[] = 
{
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0, // 0..9   
	0,	1,	2,	0,	1,	2,	0,	1,	2,	0, // 10..19 
	1,	2,	0,	1,	1,	2,	0,	1,	1,	2, // 20..29 
	0,	1,	2,	0,	1,	2,	0,	1,	2,	0, // 30..39 
	1,	2,	0,	1,	2,	0,	1,	2,	0,	1, // 40..49 
	2,	0,	1,	2,	0,	0,	0,	8,	0,	8, // 50..59 
	0, 16,	0, 16,	0,	0,	1,	1,	2,	0, // 60..69 
	1,	1,	2,	0,	1,	0,	1,	0,	1,	2, // 70..79 
	0,	1,	2, 	32, 40, 32, 40, 32, 32, 32, // 80..89
   	33, 64, 33, 34, 64, 65, 34, 32, 32, 4, // 90..99
	4,	4,	4,	4,	4,	4,	4,	4,	4,	4, // 100..109
	4                                      	// 110
};


int Cstrike_EGaitSequenceInfo[10] =
{
	0 , 0 , 1 , 2 , 3 , 4 , 5 , 5 , 6 , 6
};

bool isEnemy(int i) { return (g_Local.Team != g_Player[i].Team); }

void Noflash()
{
	if(cvar.misc_rem_flash)
	{
		static screenfade_t screenfade = { 0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0, 0 }; 
		g_Engine.pfnSetScreenFade( &screenfade ); 
	}
}

void g_player()
{
	for(int i=0;i<33;++i)
	{
		PlayerInfo& p = g_Player[i];
		sMe& l = g_Local;

		p.pEnt = g_pEngine->GetEntityByIndex(i);
		p.Alive = p.pEnt && !(p.pEnt->curstate.effects & EF_NODRAW) && p.pEnt->player && p.pEnt->curstate.movetype != 6 && p.pEnt->curstate.movetype != 0;
		p.IsValidEnt = CheckEntity(i) && p.Alive;
		p.IsValidTeam = CheckTeam(i);
		p.IsVisible = Visible(i);
		p.Ducked = ((p.pEnt->curstate.maxs[2] - p.pEnt->curstate.mins[2]) < 54 ? true : false);

		CalcHUDAngles(i);

		p.AimVisible = PathFree(l.Eye,p.Hud_AimOrigin);
		p.AimFov = AimCalcFOV(l.Forward,p.Hud_AimOriginEye);
		p.AimDistance = floor(sqrt(POW(abs(p.pEnt->origin.x - l.pEnt->origin.x)) + POW(abs(p.pEnt->origin.y - l.pEnt->origin.y)) + POW(abs(p.pEnt->origin.z - l.pEnt->origin.z)))-32);
	}
}

float YawForVec(float* fwd)
{
	if (fwd[1] == 0 && fwd[0] == 0)
	{
		return 0;
	}
	else
	{
		float yaw = (atan2(fwd[1], fwd[0]) * 180 / M_PI);
		if (yaw < 0)
			yaw += 360;
		return yaw;
	}
}

void Chase()
{
	sMe& l = g_Local;

	if(cvar.misc_chasecam && l.HudAlive && l.MsgAlive && DrawVisuals && (!cvar.rush || cvar.way_drawvisuals))
	{
		g_pEngine->pfnGetCvarPointer("chase_active")->value=1;
		g_pEngine->pfnGetCvarPointer("r_drawviewmodel")->value=0;
	}
	else 
	{
		g_pEngine->pfnGetCvarPointer("chase_active")->value=0;
		g_pEngine->pfnGetCvarPointer("r_drawviewmodel")->value=1;
	}
}

//////////////////////////////////////////////////////////////////////////

void NoRefresh()
{
	sMe& l = g_Local;

	if(cvar.misc_norefresh && cvar.rush && l.HudAlive && l.MsgAlive) { cmd.exec("#r_norefresh 1"); } 
	else { cmd.exec("#r_norefresh 0"); }
}

//////////////////////////////////////////////////////////////////////////

void NoHUD() 
{ 
	if(cvar.rush) { cmd.exec("#hud_saytext_time 0"); } else { cmd.exec("#hud_saytext_time 5"); } 

	if(cvar.misc_nohud && cvar.rush) { cmd.exec("#r_drawviewmodel 0"); } else if(!cvar.misc_chasecam) { cmd.exec("#r_drawviewmodel 1"); } 
}

//////////////////////////////////////////////////////////////////////////

void SayHelloHook()
{
	sMe& l = g_Local;

	if(cvar.misc_sayid && l.HudAlive && l.MsgAlive) 
	{
		char omghi[255]; 

		sprintf(omghi, "say \"    *** [007 Client Hook V3 - Hurricane] ***\"");

		g_Engine.pfnClientCmd(omghi);

	}
}
//////////////////////////////////////////////////////////////////////////

void FixupAngleDifference(usercmd_t *cmd)
{
	sMe& l = g_Local;

    cl_entity_t *pLocal;

    Vector viewforward, viewright, viewup, aimforward, aimright, aimup, vTemp;

    float newforward, newright, newup, fTime;
    float forward = cmd->forwardmove;
    float right = cmd->sidemove;
    float up = cmd->upmove;

    pLocal = g_Engine.GetLocalPlayer();

    if(!pLocal)
        return;

    if(pLocal->curstate.movetype == MOVETYPE_WALK)
        g_Engine.pfnAngleVectors(Vector(0.0f, cmd->viewangles.y, 0.0f), viewforward, viewright, viewup);
    else
        g_Engine.pfnAngleVectors(cmd->viewangles, viewforward, viewright, viewup);

    int iHasShiftHeld = GetAsyncKeyState(VK_LSHIFT);

    if(pLocal->curstate.movetype == MOVETYPE_WALK && !iHasShiftHeld && !(cmd->buttons & IN_ATTACK) && !(cmd->buttons & IN_USE))
    {
        fTime = g_Engine.GetClientTime();

		if(!(IsKnife(l.WpnID) || IsExplosive(l.WpnID)))
		{
			if(cvar.misc_spin == 2)
			{      	
				cmd->viewangles.x = cos(fTime * cvar.misc_spinspeed * 3.14f) * 360.0f;
      			cmd->viewangles.y = sin(fTime * cvar.misc_spinspeed * 3.14f) * 360.0f;
				cmd->viewangles.x = cos(fTime * cvar.misc_spinspeed * 3.14f) * 90.0f;
			}
			else if(cvar.misc_spin == 1)
				cmd->viewangles.y = fmod(fTime * 20 * 360.0f, 360.0f);
		}
    }

    if(pLocal->curstate.movetype == MOVETYPE_WALK)
        g_Engine.pfnAngleVectors(Vector(0.0f, cmd->viewangles.y, 0.0f), aimforward, aimright, aimup);
    else
        g_Engine.pfnAngleVectors(cmd->viewangles, aimforward, aimright, aimup);
	
	newforward = DotProduct(forward * viewforward.Normalize(), aimforward) + DotProduct(right * viewright.Normalize(), aimforward) + DotProduct(up * viewup.Normalize(), aimforward);
	newright = DotProduct(forward * viewforward.Normalize(), aimright) + DotProduct(right * viewright.Normalize(), aimright) + DotProduct(up * viewup.Normalize(), aimright);
	newup = DotProduct(forward * viewforward.Normalize(), aimup) + DotProduct(right * viewright.Normalize(), aimup) + DotProduct(up * viewup.Normalize(), aimup);

    cmd->forwardmove = newforward;
    cmd->sidemove = newright;
    cmd->upmove = newup;
}

//////////////////////////////////////////////////////////////////////////

void AntiAim(usercmd_t *cmd)
{
	sMe& l = g_Local;

	if(!(cmd->buttons&IN_ATTACK) && !(IsKnife(l.WpnID) || IsExplosive(l.WpnID) && (cmd->buttons & IN_ATTACK2)) && !(cmd->buttons&IN_USE))
    {
		Vector viewforward, viewright, viewup, aimforward, aimright, aimup, v;
		float newforward, newright, newup, fTime;
		float forward = cmd->forwardmove;
		float right = cmd->sidemove;
		float up = cmd->upmove;

		bool movetype_walk = (g_Engine.GetLocalPlayer()->curstate.movetype == MOVETYPE_WALK);
 
		if(movetype_walk)
		{
			v[0]=0.0f;
			v[1]=cmd->viewangles[1];
			v[2]=0.0f;
			g_Engine.pfnAngleVectors(v, viewforward, viewright, viewup);
		} 
		else 
			g_Engine.pfnAngleVectors(cmd->viewangles, viewforward, viewright, viewup);
 
		fTime = g_Engine.GetClientTime();

		if(cvar.misc_aa_pitch == 1)
			cmd->viewangles.x = 89.9f; // Down

		else if(cvar.misc_aa_pitch == 2)
			cmd->viewangles.x = -89.9f; // Up
		
		else if(cvar.misc_aa_pitch == 3)
			cmd->viewangles.x = 1620; // Fake Down
		
		else if(cvar.misc_aa_pitch == 4)
			cmd->viewangles.x = -1619; // Fake Up
		
		if(cvar.misc_aa_yaw == 1)
			cmd->viewangles.y = cmd->viewangles.y + 180; // Backwards

		else if(cvar.misc_aa_yaw == 2)
			cmd->viewangles.y = 90;	// Static Left

		else if(cvar.misc_aa_yaw == 3)
			cmd->viewangles.y = 270; // Static Right

		if(cvar.misc_aa_yaw == 4)
			cmd->viewangles.y = cmd->viewangles.y + 90; // Left

		else if(cvar.misc_aa_yaw == 5)
			cmd->viewangles.y = cmd->viewangles.y + 270; // Right

		else if(cvar.misc_aa_yaw == 6)
		{
			int random = rand() % 100;
 
			if (random < 98)
			// Look backwards
			cmd->viewangles.y -= 180;
 
			// Jitter
			if (random < 15)
			{
				float change = -70 + (rand() % (int)(140 + 1));
				cmd->viewangles.y += change;
			}
 
			if (random == 69)
			{
				float change = -90 + (rand() % (int)(180 + 1));
				cmd->viewangles.y += change;
			}
		}

		else if(cvar.misc_aa_yaw == 7)
		{
			//Jitter
			int random = 1 + (rand() % (int)(100 - 1 + 1));

			if(random < 10)
				cmd->viewangles.y+=180;
			
			if(random > 90)
				cmd->viewangles.y=cmd->viewangles.y;
		}
 
		//
		if(movetype_walk)
		{
			v[0]=0.0f;
			v[1]=cmd->viewangles[1];
			v[2]=0.0f;
			g_Engine.pfnAngleVectors(v, aimforward, aimright, aimup);
		} 
		else 
			g_Engine.pfnAngleVectors(cmd->viewangles, aimforward, aimright, aimup);
 
		//
		newforward = DotProduct(forward * viewforward.Normalize(), aimforward) + DotProduct(right * viewright.Normalize(), aimforward) + DotProduct(up * viewup.Normalize(), aimforward);
		newright = DotProduct(forward * viewforward.Normalize(), aimright) + DotProduct(right * viewright.Normalize(), aimright) + DotProduct(up * viewup.Normalize(), aimright);
		newup = DotProduct(forward * viewforward.Normalize(), aimup) + DotProduct(right * viewright.Normalize(), aimup) + DotProduct(up * viewup.Normalize(), aimup);
 
		//
		if(cvar.misc_aa_pitch == 3 || cvar.misc_aa_pitch == 4)
		{
			cmd->forwardmove = -newforward;
			cmd->sidemove = newright;
			cmd->upmove = newup;
		}
		else
		{
			cmd->forwardmove = newforward;
			cmd->sidemove = newright;
			cmd->upmove = newup;
		}
	}
}

//////////////////////////////////////////////////////////////////////////

float GetGunOffset(cl_entity_s* vm)
{
	if(!vm->model)
		return 0;

	char* gunname = vm->model->name;
	
	if(!gunname || !gunname[0])
		return 0;

	gunname += 9;

	#define CHECKGUNOFFSET(a,b) if(!strcmp(a,gunname)) return b;

	CHECKGUNOFFSET("glock18.mdl", -4.55f);
	CHECKGUNOFFSET("usp.mdl", -4.64f);
	CHECKGUNOFFSET("p228.mdl", -4.65f);
	CHECKGUNOFFSET("deagle.mdl", -4.71f);
	CHECKGUNOFFSET("fiveseven.mdl", -4.84f);
	
	CHECKGUNOFFSET("m3.mdl", -5.03f);
	CHECKGUNOFFSET("xm1014.mdl", -5.82f);

	CHECKGUNOFFSET("mac10.mdl", -5.05f);
	CHECKGUNOFFSET("tmp.mdl", -6.47f);
	CHECKGUNOFFSET("mp5.mdl", -5.53f);
	CHECKGUNOFFSET("ump45.mdl", -6.07f);
	CHECKGUNOFFSET("p90.mdl", -4.32f);

	CHECKGUNOFFSET("scout.mdl", -5.14f);
	CHECKGUNOFFSET("awp.mdl", -6.02f);

	CHECKGUNOFFSET("famas.mdl", -4.84f);
	CHECKGUNOFFSET("aug.mdl", -6.22f);
	CHECKGUNOFFSET("m4a1.mdl", -6.74f);
	CHECKGUNOFFSET("sg550.mdl", -7.11f);
	CHECKGUNOFFSET("ak47.mdl", -6.79f);

	CHECKGUNOFFSET("g3sg1.mdl", -6.19f);
	CHECKGUNOFFSET("sg552.mdl", -5.27f);
	CHECKGUNOFFSET("galil.mdl", -4.78f);

	CHECKGUNOFFSET("m249.mdl", -5.13f);

	return 0;
}

//////////////////////////////////////////////////////////////////////////

void QuakeGuns()
{
	cl_entity_s* vm = g_Engine.GetViewModel();

	if(!vm)
	return;

	float gunoffsetr = GetGunOffset(vm);

	if(gunoffsetr == 0)
	return;

	float* org = vm->origin;
	float* ang = vm->angles;

	VectorCopy(mainViewAngles, ang);

	vec3_t forward, right, up;
	g_Engine.pfnAngleVectors(mainViewAngles, forward, right, up);

	org[0] += forward[0] + up[0] + right[0]*gunoffsetr;
	org[1] += forward[1] + up[1] + right[1]*gunoffsetr;
	org[2] += forward[2] + up[2] + right[2]*gunoffsetr;
}

//////////////////////////////////////////////////////////////////////////

static int sprset_hsprite = 0;
static int sprset_r, sprset_g, sprset_b;

//////////////////////////////////////////////////////////////////////////

void SPR_DrawAdditive(int frame, int x, int y, const wrect_t *prc)
{
	if(cvar.rush) return;

	if(cvar.misc_hud) g_Engine.pfnSPR_Set(sprset_hsprite,sprset_r,sprset_g,sprset_b);
	g_Engine.pfnSPR_DrawAdditive(frame,x,y,prc);

}

//////////////////////////////////////////////////////////////////////////

void FillRGBA(int x, int y, int width, int height, int r, int g, int b, int a)
{
	if(cvar.rush) return;

	if(cvar.misc_hud) { g_Engine.pfnFillRGBA(x,y,width,height,cvar.color_red*255,cvar.color_green*255,cvar.color_blue*255,a); }
	else { g_Engine.pfnFillRGBA(x,y,width,height,r,g,b,a); }
}
	
//////////////////////////////////////////////////////////////////////////

int	DrawCharacter(int x, int y, int number, int r, int g, int b)
{
	if(b==0 && g>0 && cvar.misc_hud)
	{
		register double ratio = (double(r)/double(g));
		if(ratio>=1.5 && ratio<=1.7)
			return g_Engine.pfnDrawCharacter(x,y,number,cvar.color_red*r/255,cvar.color_green*r/255,cvar.color_blue*r/255);
	}

	return g_Engine.pfnDrawCharacter(x,y,number,r,g,b);
}

//////////////////////////////////////////////////////////////////////////

void SPR_Set(int hPic, int r, int g, int b)
{
	if(cvar.rush) return;

	if(cvar.misc_hud)
	{
		#define MAX_VALUE(a,b,c) ( a>b? (a>c?a:(b>c?b:c)) : (b>c?b:c) )

		sprset_hsprite=hPic;
		int intensity = MAX_VALUE(r,g,b);

		sprset_r = cvar.color_red*intensity;
		sprset_g = cvar.color_green*intensity;
		sprset_b = cvar.color_blue*intensity;
	}

	g_Engine.pfnSPR_Set (hPic,r,g,b);
}

//////////////////////////////////////////////////////////////////////////

void SPR_Draw(int frame, int x, int y, const wrect_t* prc) { if(cvar.rush) return; }

//////////////////////////////////////////////////////////////////////////

void ExcellentSettings()
{
	// FPS
	cmd.exec("#fps_max 1000"); 
	cmd.exec("#fps_modem 1000");
	cmd.exec("#developer 1");

	// NET
	cmd.exec("#rate 100000");
	cmd.exec("#cl_cmdrate 102");
	cmd.exec("#cl_updaterate 102");
	cmd.exec("#net_graph 0;#cl_showfps 1");

	// GL
	cmd.exec("#gamma 3");
	cmd.exec("#joystick 0");
	cmd.exec("#brightness 30");
	cmd.exec("#fastsprites 1");
	cmd.exec("#ex_interp 0.02");
	cmd.exec("#d_spriteskip 0");
	cmd.exec("#r_mmx 1");
	cmd.exec("#r_decals 0");
	cmd.exec("#r_dynamic 0");
	cmd.exec("#r_mirroralpha 0");
	cmd.exec("#gl_clear 1");
	cmd.exec("#gl_dither 1");
	cmd.exec("#gl_cull 1");
	cmd.exec("#gl_keeptjunctions 0");
	cmd.exec("#gl_picmip 1");
	cmd.exec("#gl_playermip 1");
	cmd.exec("#gl_palette_tex 0");
	cmd.exec("#gl_round_down 50"); 
	cmd.exec("#gl_texturemode \"GL_LINEAR_MIPMAP_NEAREST\"");
	cmd.exec("#gl_wateramp 0");
	cmd.exec("#max_shells 0");
	cmd.exec("#max_smokepuffs 0");

	// Game
	cmd.exec("#violence_ablood 0");
	cmd.exec("#violence_agibs 0");
	cmd.exec("#violence_hblood 0");
	cmd.exec("#violence_hgibs 0");
	cmd.exec("#hud_fastswitch 1");
	cmd.exec("#cl_minmodels 1");
	cmd.exec("#cl_allowdownload 1");
	cmd.exec("#cl_shadows 0");
	cmd.exec("#cl_weather 0");
	cmd.exec("#cl_bob 0;#cl_bobup 0");
	cmd.exec("#cl_crosshair_translucent 1");
	cmd.exec("#cl_crosshair_color \"0 0 0\"");
	/*
	cmd.exec("#cl_crosshair_size \"small\"");
	cmd.exec("#cl_crosshair_color \"50 250 50\"");
	*/
}

//////////////////////////////////////////////////////////////////////////

bool CanAttack(void)
{
	sMe& l = g_Local;

	if (l.m_flNextPrimaryAttack <= 0.01f && !l.m_iInReload && l.m_flNextAttack <= 0.01f)
		return true;

	return false;
}

bool fileExists(const char* filename)
{
	WIN32_FIND_DATA finddata;
	HANDLE handle = FindFirstFile(filename,&finddata);
	return (handle!=INVALID_HANDLE_VALUE);
}

void State()
{
	static bool previous_frozen_state=false;

	if(g_Local.speed > 10.0 && (g_Local.Flags & FL_ONGROUND))
		g_Local.infrozentime = false;

	if(previous_frozen_state && !g_Local.infrozentime)
		cmd.exec("buystuff");

	previous_frozen_state = g_Local.infrozentime;
}
