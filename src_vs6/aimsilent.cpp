#include "client.h"

int iTargetID;
float fAimAngles[3], fOldFOV, fOldDistance;

float AimCalcFOV(Vector a,Vector b,float lena,float lenb)
{
	float l1=0.0f, l2=0.0f;
	if(lena>0)l1=lena;
	else l1=a.Length();
	if(lenb>0)l2=lenb;else l2=b.Length();
	float sc=a.x*b.x+a.y*b.y+a.z*b.z;
	return acos(sc/(l1*l2))*(180.0/M_PI);
}

void SortTargetByFov(int iIndex)
{
	PlayerInfo& p = g_Player[iIndex];

	if(p.AimFov <= 360.0)
	{
		if(p.AimFov<=fOldFOV || iTargetID == 0)
		{
			if(!cvar.aim_shield && IsShielded(iIndex)) 
				iTargetID = 0;

			else
			{
				fOldFOV = p.AimFov;
				iTargetID = iIndex;
			}
		}
	}

	else { iTargetID = 0; }
}

void SortTargetByDistance(int iIndex)
{
	PlayerInfo& p = g_Player[iIndex];

	if(p.AimDistance <= 8142.0)
	{
		if(p.AimDistance<=fOldDistance || iTargetID == 0)
		{
			if(!cvar.aim_shield && IsShielded(iIndex)) 
				iTargetID = 0;

			else
			{
				fOldDistance = p.AimDistance;
				iTargetID = iIndex;
			}
		}
	}

	else { iTargetID = 0; }
}

Vector vCalcOriginOffset(int iIndex)
{
	cl_entity_s *ent = g_pEngine->GetEntityByIndex(iIndex);

	Vector vAngles,vF,vR,vU,vOut;
	vAngles=Vector(0.0f,ent->angles[1],0.0f);

	g_Engine.pfnAngleVectors(vAngles,vF,vR,vU);

	if(ent->curstate.gaitsequence == 6) 
		vOut = ent->origin+vF*cvar.aim_x_jump+vR*cvar.aim_y_jump+vU*cvar.aim_z_jump;
	else if(ent->curstate.gaitsequence == 5) 
		vOut = ent->origin+vF*cvar.aim_x_duckmove+vR*cvar.aim_y_duckmove+vU*cvar.aim_z_duckmove;
	else if(ent->curstate.gaitsequence == 4) 
		vOut = ent->origin+vF*cvar.aim_x_running+vR*cvar.aim_y_running+vU*cvar.aim_z_running;
	else if(ent->curstate.gaitsequence == 3) 
		vOut = ent->origin+vF*cvar.aim_x_walking+vR*cvar.aim_y_walking+vU*cvar.aim_z_walking;
	else if(ent->curstate.gaitsequence == 2)
		vOut = ent->origin+vF*cvar.aim_x_duck+vR*cvar.aim_y_duck+vU*cvar.aim_z_duck;
	else
		vOut = ent->origin+vF*cvar.aim_x_stand+vR*cvar.aim_y_stand+vU*cvar.aim_z_stand;

	return vOut;
}

void DrawAim()
{
	sMe& l = g_Local;

	if(l.HudAlive && l.MsgAlive && !IsExplosive(l.WpnID))
	{
		if(cvar.aim_drawaim)
		{
			for(int i=0;i<33;i++)
			{
				PlayerInfo& p = g_Player[i];

				if(p.IsValidEnt && p.IsValidTeam && p.IsVisible)
				{
					float AimAngles[2];

					if(WorldToScreen(p.Hud_AimOrigin, AimAngles))
					{
						if(cvar.aim_activate)
							g_Engine.pfnFillRGBA((int)AimAngles[0]-1,(int)AimAngles[1] , 3 , 3 , 30 , 255 , 30 , cvar.misc_tint*255.0f );
						else
							g_Engine.pfnFillRGBA((int)AimAngles[0]-1,(int)AimAngles[1] , 3 , 3 , cvar.color_red*255.0f, cvar.color_green*255.0f, cvar.color_blue*255.0f , cvar.misc_tint*255.0f );

						blackBorder((int)AimAngles[0]-1,(int)AimAngles[1],3,4,cvar.misc_tint*255.0f);
					}
				}
			}
		}
	}
}

void Aimbot(usercmd_s *cmd)
{
	iTargetID = 0;

	sMe& l = g_Local;

	if((l.HudAlive || l.MsgAlive) && !IsExplosive(l.WpnID))
	{
		if(!cvar.aim_activate)
		{	
			for(int i=0;i<33;i++)
			{
				PlayerInfo& p = g_Player[i];

				if(p.IsValidEnt && p.IsValidTeam && p.IsVisible)
				{
					if(IsKnife(l.WpnID) || cvar.aim_mode == 1)
							SortTargetByDistance(i);
						else if(cvar.aim_mode == 2)
							SortTargetByFov(i);
					
					if(iTargetID != 0 && l.Clip) 
					{
						KnifeBot(cmd, i);

						TriggerBot(cmd);
					}
				}
			}
		}

		if(cvar.aim_activate)
		{
			for(int i=0;i<33;i++)
			{
				if(idhook.FirstKillPlayer[i]==1 || cvar.aim_id_mode == 0)
				{
					PlayerInfo& p = g_Player[i];

					if(p.IsValidEnt && p.IsValidTeam && p.IsVisible)
					{
						if(IsKnife(l.WpnID) || cvar.aim_mode == 1)
								SortTargetByDistance(i);
							else if(cvar.aim_mode == 2)
								SortTargetByFov(i);
			
						if(iTargetID != 0 && l.Clip) 
						{
							if(cvar.aim_psilent?CanAttack():1)
							{
								KnifeBot(cmd, i);

								TriggerBot(cmd);
					
								AimAuto(cmd, i);

								ApplyAngles(cmd, i);
							}
						}
					}
				}
			}

			if(cvar.aim_id_mode!=2 && iTargetID == 0)
			{
				// Fucking idhook needs two loops
				for(int i=0;i<33;i++)
				{
					if(idhook.FirstKillPlayer[i]<2)
					{
						PlayerInfo& p = g_Player[i];

						if(p.IsValidEnt && p.IsValidTeam && p.IsVisible)
						{
							if(IsKnife(l.WpnID) || cvar.aim_mode==1)
									SortTargetByDistance(i);
								else if(cvar.aim_mode==2)
									SortTargetByFov(i);
								
							if(iTargetID != 0 && l.Clip)
							{
								if(cvar.aim_psilent?CanAttack():1)
								{
									KnifeBot(cmd, i);

									TriggerBot(cmd);
					
									AimAuto(cmd, i);

									ApplyAngles(cmd, i);
								}
							}
						}
					}
				}
			}
		}
	}
}

void CalcCLAngles(int i)
{
	PlayerInfo& p = g_Player[i];
	sMe& l = g_Local;
	p.CL_AimOrigin = vCalcOriginOffset(iTargetID)-l.Eye;
}

void CalcHUDAngles(int i)
{
	PlayerInfo& p = g_Player[i];
	sMe& l = g_Local;
	p.Hud_AimOrigin = vCalcOriginOffset(i);
	p.Hud_AimOriginEye = vCalcOriginOffset(i)-l.Eye;
}

void TriggerBot(usercmd_s *cmd)
{
	for(int i=0;i<33;i++)
	{
		PlayerInfo& p = g_Player[i];
		sMe& l = g_Local;

		if(cvar.trigger_activate && !IsKnife(l.WpnID))
		{
			float diff[3], radius, distance, boxradius;
			vec3_t vecEnd, up, right, forward, EntViewOrg, playerAngles, taimangles, faimangles;

			faimangles[0] = cmd->viewangles[0] + l.PunchAngle[0];
			faimangles[1] = cmd->viewangles[1] + l.PunchAngle[1];

			CalcCLAngles(i);

			VectorCopy(p.CL_AimOrigin,EntViewOrg);
			VectorAngles(EntViewOrg,taimangles);

			taimangles[0] *= -1;
			if (taimangles[1]>180) taimangles[1]-=360;
	
			diff[0] = faimangles[0] - taimangles[0];
			diff[1] = faimangles[1] - taimangles[1];

			if (diff[1]>180) diff[1]-=360;
			if (diff[1]>45 || diff[1]<-45) continue;

			diff[2] = sqrt(diff[0] * diff[0] + diff[1] * diff[1]);

			distance = sqrt(EntViewOrg[0] * EntViewOrg[0] + EntViewOrg[1] * EntViewOrg[1] + EntViewOrg[2] * EntViewOrg[2]);
		
			boxradius = cvar.trigger_fov;
			radius = (float) ((atan2( boxradius, distance) * 180 / M_PI));
			l.fTrigRadius = radius;
			l.fTrigDiff = diff[2];
					
			if (diff[2] < radius)
				cmd->buttons |= IN_ATTACK;
		}
	}
}

void KnifeBot(usercmd_s *cmd, int i)
{
	PlayerInfo& p = g_Player[i];
	sMe& l = g_Local;

	if(cvar.knf_activate && IsKnife(l.WpnID))
	{
		float distance;

		if(cvar.knf_attack == 1)
			distance = (float)cvar.knf_distattack;
		else if (cvar.knf_attack == 2)
			distance = (float)cvar.knf_distattack2;
			
		cl_entity_s* ent = g_pEngine->GetEntityByIndex(i);

		if(VectorDistance(l.Origin,ent->origin)<=distance)
		{
			if(p.AimFov<=cvar.knf_fov)
			{ 
				if(cvar.knf_attack == 1)
					cmd->buttons |= IN_ATTACK;
				else if(cvar.knf_attack == 2)
					cmd->buttons |= IN_ATTACK2;
			}
		}
	}
}

void AimAuto(usercmd_s *cmd, int i)
{
    PlayerInfo& p = g_Player[i];
	sMe& l = g_Local;

	if(cvar.aim_auto)
	{
		if(p.AimFov<=cvar.aim_auto_fov)
		{ 
			if(IsKnife(l.WpnID))
			{
				if(cvar.aim_knf_attack == 1)
					cmd->buttons |= IN_ATTACK;
				else if(cvar.aim_knf_attack == 2)
					cmd->buttons |= IN_ATTACK2;	 
			}else{
				if(bAttacking)
				{
					if(cvar.aim_auto_punch)
					{
						if(VectorLength(l.PunchAngle) == 0)
							cmd->buttons |= IN_ATTACK;
					}
					else
						cmd->buttons |= IN_ATTACK;
				}
			}
		}
	}
}

void ApplyAngles(usercmd_s *cmd, int i)
{
	PlayerInfo& p = g_Player[i];
	sMe& l = g_Local;

	CalcCLAngles(i);

	VectorAnglesAimbot(p.CL_AimOrigin,fAimAngles);
	
	if((cmd->buttons&IN_ATTACK || cmd->buttons&IN_ATTACK2 && IsKnife(l.WpnID)))
	{
		if(cvar.aim_silent)
		{
			ApplySilentAngles(fAimAngles, cmd, false);
			if(cvar.aim_psilent)bSendpacket(false);
		}
		else
		{
			cmd->viewangles = fAimAngles;
			g_Engine.SetViewAngles(fAimAngles);
		}
	}
}

int PathFree(float* from,float* to) 
{ 
	if( !from || !to ) { return false; }

	int pathtest; 

	pmtrace_t tr; 

	sMe& l = g_Local;

	g_Engine.pEventAPI->EV_SetTraceHull( 2 ); 
	g_Engine.pEventAPI->EV_PlayerTrace( from, to, PM_GLASS_IGNORE | PM_STUDIO_BOX, l.iIndex, &tr ); 

	pathtest = (tr.fraction == 1.0); 

	return pathtest; 
}

// broken autowall (fps issue)
/*
int CorrectGun() 
{ 
    sMe& l = g_Local;

	if (l.WpnID == WEAPONLIST_SG550 || 
		l.WpnID == WEAPONLIST_G3SG1 || 
		l.WpnID == WEAPONLIST_SCOUT || 
		l.WpnID == WEAPONLIST_AWP) 
		return 3; 
	if (l.WpnID == WEAPONLIST_AUG || 
		l.WpnID == WEAPONLIST_DEAGLE || 
		l.WpnID == WEAPONLIST_AK47) 
		return 2; 
	return 0; 
}

int CanPenetrate( float *start, float *end, int power ) 
{
	pmtrace_t pmtrace;
	pmtrace_t* tr = (pmtrace_t*) &pmtrace;
	
	float view[3];
	float dir[3];
	
	view[0] = end[0] - start[0];
	view[1] = end[1] - start[1];
	view[2] = end[2] - start[2];
	
	float length = VectorLength(view);
	
	dir[0] = view[0] / length;
	dir[1] = view[1] / length;
	dir[2] = view[2] / length;
	
	float position[3];
	position[0] = start[0];
	position[1] = start[1];
	position[2] = start[2];
	tr->startsolid = true;
	
	while( power )
	{ 
		if( !tr->startsolid )
			power--;
		tr = g_pEngine->PM_TraceLine( position, end, PM_TRACELINE_PHYSENTSONLY, 2, -1);
		
		if( tr->fraction==1.0f )
			return 1;
		if( tr->allsolid )
			return 0;
		
		position[0] = tr->endpos[0] + dir[0] * 8.0f;
		position[1] = tr->endpos[1] + dir[1] * 8.0f;
		position[2] = tr->endpos[2] + dir[2] * 8.0f;
	}
	return 0;
}

int PathFree(float* from,float* to) 
{ 
	if( !from || !to ) { return false; }
	int pathtest; 

	pmtrace_t tr; 

	sMe& l = g_Local;

	g_Engine.pEventAPI->EV_SetTraceHull( 2 ); 
	g_Engine.pEventAPI->EV_PlayerTrace( from, to, PM_GLASS_IGNORE | PM_STUDIO_BOX, -1, &tr ); 

	pathtest = (tr.fraction == 1.0); 

	if (!pathtest && cvar.aim_activate) 
		pathtest = CanPenetrate(from, to, CorrectGun());

	return pathtest; 
}
*/