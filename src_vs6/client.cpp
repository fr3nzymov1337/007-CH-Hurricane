#include "client.h"

cl_clientfunc_t *g_pClient = NULL;
cl_enginefunc_t *g_pEngine = NULL;
engine_studio_api_t *g_pStudio = NULL;
CL_Move_t CL_Move_s = NULL;

cl_clientfunc_t g_Client;
cl_enginefunc_t g_Engine;
engine_studio_api_t g_Studio;

void HUD_Frame(double time)
{
	g_Client.HUD_Frame(time);

	GetScreenInfo();
	AttackHandling();
	Snapshot();

	if(DrawVisuals && (!cvar.rush || cvar.way_drawvisuals)) {	Noflash(); }
}

void HUD_Redraw(float time, int intermission)
{
	g_Client.HUD_Redraw(time, intermission);

	g_LocalpEnt();
	g_LocaliIndex();
	g_LocalAlive();
	g_player();
	Clienttime();
	State();

	if(DrawVisuals && (!cvar.rush || cvar.way_drawvisuals))
	{
		Chase();
		NoHUD();
		NoRefresh();
		DrawRadarFrame();
		waypoint.DrawRadarsPoints();
		PlayerEsp();
		DrawAim();
		DrawMenu();
		DrawSpread();
		DrawRecoil();
		HUDStrafeBot();
		DrawConsole();
		DrawHudText();
		DrawIdHook();
		DrawChat();
	}

	if(waypoint.isRecord) 	{	waypoint.record			();		}
	if(cvar.way_enable)		{ 	waypoint.DrawWayLine	();		}
	if(cvar.way_enable)		{	waypoint.DrawWayPoint	();		}
}

void HUD_PlayerMove(struct playermove_s *ppmove, int server)
{
	g_Client.HUD_PlayerMove(ppmove, server);

	sMe& l = g_Local;
	l.MoveType = ppmove->movetype;
	g_Engine.pEventAPI->EV_LocalPlayerViewheight(l.Eye);
	l.Eye = l.Eye + ppmove->origin;
	l.Origin = ppmove->origin;
	l.flFallSpeed = ppmove->flFallVelocity;
	l.Flags = ppmove->flags;
	l.FSpeed = sqrt(POW(ppmove->velocity[0]) + POW(ppmove->velocity[1]));	
	l.VVelocity = ppmove->velocity;
	VectorCopy(ppmove->angles,l.ViewAngles);
	VectorCopy(ppmove->velocity,l.Velocity);
	Vector vel = ppmove->velocity;
	l.FVSpeed = vel.Length();
	l.UseHull = ppmove->usehull;
	l.MaxSpeed = ppmove->maxspeed;
	l.FrameTime = ppmove->frametime;
	l.fSpeed = sqrt(POW(ppmove->velocity[0]) + POW(ppmove->velocity[1]));
	l.AirAccelerate = ppmove->movevars->airaccelerate;
	l.Waterlevel = ppmove->waterlevel;
	l.vAngles = ppmove->angles;
	l.FAngleSpeed = (g_Local.vAngles)[1]-(ppmove->angles)[1];while(g_Local.FAngleSpeed<-180){g_Local.FAngleSpeed+=360;}while(g_Local.FAngleSpeed>180){g_Local.FAngleSpeed-=360;}

	l.VOrigin = ppmove->origin;
	Vector vTemp1 = l.VOrigin;
	vTemp1[2] -= 8192;
	pmtrace_t *trace = g_pEngine->PM_TraceLine(l.VOrigin, vTemp1, 1, ppmove->usehull, -1); 
	l.FlHeight=abs(trace->endpos.z - l.VOrigin.z);
	
	if(g_Local.FlHeight <= 60) g_Local.flGroundAngle=acos(trace->plane.normal[2])/M_PI*180; 
	else g_Local.flGroundAngle = 0;

	Vector vTemp2=trace->endpos;
	pmtrace_t pTrace;
	g_pEngine->pEventAPI->EV_SetTraceHull( ppmove->usehull );
	g_pEngine->pEventAPI->EV_PlayerTrace( l.VOrigin, vTemp2, PM_GLASS_IGNORE | PM_STUDIO_BOX, l.iIndex, &pTrace );
	if( pTrace.fraction < 1.0f )
	{
		l.FlHeight=abs(pTrace.endpos.z - l.VOrigin.z);
		int ind=g_pEngine->pEventAPI->EV_IndexFromTrace(&pTrace);
		if(ind>0&&ind<g_Engine.GetMaxClients())
		{
			float dst=l.VOrigin.z-(l.UseHull==0?32:18)-g_Player[ind].VOrigin.z-l.FlHeight;
			if(dst<30)
			{
				l.FlHeight-=14.0;
			}
		}
	}
}

void CL_CreateMove(float frametime, struct usercmd_s * cmd, int active)
{
	sMe& l = g_Local;

	g_Client.CL_CreateMove(frametime, cmd, active);

	AdjustSpeed(cvar.wavspeed);
	waypoint.FindTarget(cmd);
	ListIdHook();
	Aimbot(cmd);
	CLStrafeBot(frametime, cmd);
	ContinueFire(cmd);
	Recoil(cmd);
	Nospread(cmd);

	if(DrawVisuals) { opengl_ClearBackground(); }

	if(iTargetID == 0) { FixupAngleDifference(cmd); AntiAim(cmd); }

	if(cvar.auto_direction && l.HudAlive && l.MsgAlive && !IsExplosive(l.WpnID) && iTargetID == 0 && autoadjust.direction(cmd->viewangles))
		g_Engine.SetViewAngles(l.ViewAngles);

	if(cvar.auto_jump && l.HudAlive && l.MsgAlive && cvar.way_on && autoadjust.Jump(cmd->viewangles))
		cmd->buttons |= IN_JUMP;
}

void HUD_PostRunCmd(struct local_state_s *from, struct local_state_s *to, struct usercmd_s *cmd, int runfuncs, double time, unsigned int random_seed)
{
	g_Client.HUD_PostRunCmd(from, to, cmd, runfuncs, time, random_seed);

	Spread_HUD_PostRunCmd(from, to, cmd, runfuncs, time, random_seed);
}

int HUD_UpdateClientData(client_data_t *pcldata, float flTime) { return g_Client.HUD_UpdateClientData(pcldata, flTime); }

void V_CalcRefdef(struct ref_params_s *pparams)
{
	g_Client.V_CalcRefdef(pparams);

	sMe& l = g_Local;

	g_Local.health = pparams->health;

	VectorCopy(pparams->forward, l.Forward)
	VectorCopy(pparams->punchangle, l.PunchAngle);

	l.NoRecoilAng[0] = cvar.norecoilvalue * pparams->punchangle[0];
	l.NoRecoilAng[1] = cvar.norecoilvalue * pparams->punchangle[1];
	
	if(cvar.norecoil_visual) { VectorClear(pparams->punchangle); }

	if(cvar.norecoil_visible)
	{
		pparams->punchangle[0] -= l.NoRecoilAng[0];
		pparams->punchangle[1] -= l.NoRecoilAng[1];
	}
	
	if(cvar.nospread && cvar.nospread_visible && pparams->cmd->buttons & IN_ATTACK)
	{
		pparams->punchangle[0] += l.NoSpreadAng[0]; 
		pparams->punchangle[1] += l.NoSpreadAng[1];
	}

	if(pparams->nextView == 0) { VectorCopy(pparams->viewangles,mainViewAngles); }

	if(cvar.misc_quake == 1) { QuakeGuns(); }
		
}

int HUD_Key_Event(int down , int keynum , const char *pszCurrentBinding)
{
	int ret = g_Client.HUD_Key_Event(down , keynum , pszCurrentBinding);

	if (MenuActive && down )
	{
		ret = MenuKeyEvent(keynum);
		if (!ret)
			return 0;		
	}

	if (keynum >= 239 && keynum <= 244)
	{
		switch (keynum) {
		case 239:
			keyBindManager.notifyMouseEvent("mwheeldown", (down == 1));
			break;
		case 240:
			keyBindManager.notifyMouseEvent("mwheelup", (down == 1));
			break;
		case 241:
			keyBindManager.notifyMouseEvent("mouse1", (down == 1));
			break;
		case 242:
			keyBindManager.notifyMouseEvent("mouse2", (down == 1));
			break;
		case 243:
			keyBindManager.notifyMouseEvent("mouse3", (down == 1));
			break;
		case 244:
			keyBindManager.notifyMouseEvent("mouse4", (down == 1));
			break;
		case 245:
			keyBindManager.notifyMouseEvent("mouse5", (down == 1));
			break;
		}
	}
	else
	{
		if (down)
			keyBindManager.notifyKeyEvent(keynum, true, false);
		else
			keyBindManager.notifyKeyEvent(keynum, false, false);
	}
	
	ret = (keyBindManager.keyBlocked(keynum, (down) ? true : false)) ? 0 : 1;

	if(gPlayerActive() && (keynum >= 239 && keynum <= 245))
		ret = 0;

	return ret;
}

void AtMapChange()
{
	sMe& l = g_Local;

	l.spread.prcflags = 0;
	l.spread.random_seed = 0;
	l.spread.recoil = 0;
	l.spread.gtime = 0;
	l.spread.prevtime = 0;
	l.spread.brokentime = 0;
	l.spread.spreadvar = 0;
	l.spread.recoiltime = 0;
	l.spread.firing = false;

	gHudTimer.setExpired();
}

char currentMap[100];

void AtRoundStart(void)
{
	sMe& l = g_Local;

	if( strcmp(currentMap,g_Engine.pfnGetLevelName()) )
	{
		strcpy(currentMap,g_Engine.pfnGetLevelName());
		AtMapChange();
	}

	if(l.WpnID == WEAPONLIST_SG550 || l.WpnID == WEAPONLIST_G3SG1)
		l.spread.brokentime = 0; 

	waypoint.LoadFromFile();

	if(cvar.misc_sayid && l.HudAlive && l.MsgAlive) { SayHelloHook(); }

	l.infrozentime=true;
}

void bSendpacket(bool status) 
{
	static bool bsendpacket_status = true;
	static DWORD NULLTIME = NULL;

	if(status && !bsendpacket_status)
	{
		bsendpacket_status = true;
		*(DWORD*)(c_Offset.dwSendPacketPointer) = c_Offset.dwSendPacketBackup;
	}
	
	if(!status && bsendpacket_status)
	{
		bsendpacket_status = false;
		*(DWORD*)(c_Offset.dwSendPacketPointer) = (DWORD)&NULLTIME;
	}
}

void CL_Move()
{
	bSendpacket(true);

	CL_Move_s();
}

void AdjustSpeed(double speed)
{
	static double LastSpeed = 1;

	if (speed != LastSpeed)
	{
		*(double*)c_Offset.dwSpeedPointer = (speed * 1000);

		LastSpeed = speed;
	}
}

void HookFunction()
{
	g_pClient->HUD_Frame = HUD_Frame;
	g_pClient->HUD_Redraw = HUD_Redraw;
	g_pClient->CL_CreateMove = CL_CreateMove;
	g_pClient->HUD_PlayerMove = HUD_PlayerMove;
	g_pClient->V_CalcRefdef = V_CalcRefdef;
	g_pClient->HUD_PostRunCmd = HUD_PostRunCmd;
	g_pClient->HUD_UpdateClientData = HUD_UpdateClientData;
	g_pClient->HUD_Key_Event = HUD_Key_Event;
}