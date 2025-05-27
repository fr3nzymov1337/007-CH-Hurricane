#include "client.h"

PUserMsg pUserMsgBase;

PUserMsg UserMsgByName(char* szMsgName)
{
	PUserMsg Ptr = NULL;
	Ptr = pUserMsgBase;
	while (Ptr->next)
	{
		if (!strcmp(Ptr->name, szMsgName))
			return Ptr;
		Ptr = Ptr->next;
	}
	Ptr->pfn = 0;
	return Ptr;
}

pfnUserMsgHook HookUserMsg(char *szMsgName, pfnUserMsgHook pfn)
{
	PUserMsg Ptr = NULL;
	pfnUserMsgHook Original = NULL;
	Ptr = UserMsgByName(szMsgName);
	if (Ptr->pfn != 0) {
		Original = Ptr->pfn;
		Ptr->pfn = pfn;
		return Original;
	}
	else
	{
		char Message[256];
		strcpy(Message, "ERROR: Couldn't find ");
		strcat(Message, szMsgName);
		strcat(Message, " message.");
		c_Offset.Error(Message);
	}
}

void HookUserMessages()
{
#define HOOK_MSG(n) \
	p##n = HookUserMsg(#n, ##n);

	HOOK_MSG(ResetHUD);
	HOOK_MSG(SetFOV);
	HOOK_MSG(TeamInfo);
	HOOK_MSG(CurWeapon);
	HOOK_MSG(DeathMsg);
	HOOK_MSG(AmmoX);
	HOOK_MSG(WeaponList);
	HOOK_MSG(ScoreAttrib);
}

pfnUserMsgHook pResetHUD;
pfnUserMsgHook pSetFOV;
pfnUserMsgHook pTeamInfo;
pfnUserMsgHook pCurWeapon;
pfnUserMsgHook pDeathMsg;
pfnUserMsgHook pWeaponList;
pfnUserMsgHook pAmmoX;
pfnUserMsgHook pScoreAttrib;


int ScoreAttrib(const char *pszName, int iSize, void *pbuf)
{
	g_LocalpEnt();
	g_LocaliIndex();

	BEGIN_READ( pbuf, iSize );

    int idx  = READ_BYTE();
    int info = READ_BYTE();

	if(idx > 32)
		return false;

	if(info > 32)
		return false;

	if(idx == g_Local.iIndex)
		g_Local.MsgAlive = ((info&1)==0);

	return pScoreAttrib(pszName,iSize,pbuf);
}

int ResetHUD(const char *pszName, int iSize, void *pbuf)
{
	AtRoundStart();

	for(int i=0;i<=33;i++)
	{
		if (i == g_Local.iIndex)
			continue;

		g_Player[i].Alive = false;
		g_Player[i].iHealth = 100;
	}

	return pResetHUD(pszName, iSize, pbuf);
}


int DeathMsg(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ(pbuf, iSize);
	
	int killer = READ_BYTE();
	int victim = READ_BYTE();
	int headshot = READ_BYTE();

	char* weaponName = READ_STRING();

	char sayidprefix[] = { '0','0', '7', 0 };

	if(killer > 32)
		return false;

	if(victim > 32)
		return false;

	if(headshot > 32)
		return false;

	if(victim != g_Local.iIndex) 
	{
		g_Player[victim].Alive = false;
		g_Player[victim].iHealth = 100;
		SoundClear(g_Player[victim].previousSound);
	}

	if(cvar.misc_sayid && killer == g_Local.iIndex && killer != victim) 
	{
		char SayText[255]; 

		if(headshot) { sprintf(SayText, "say \"[%s CLIENT HOOK V3 - HURRICANE] || [[[ *** HEADSHOT by %s ***]]]\"", sayidprefix, weaponName); }
		else { sprintf(SayText, "say \"[%s CLIENT HOOK V3 - HURRICANE] || [[[ *** KILLED by %s ***]]]\"",  sayidprefix, weaponName); }

		g_Engine.pfnClientCmd(SayText);
	}

	return pDeathMsg(pszName, iSize, pbuf);
}

int SetFOV(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ(pbuf, iSize);
	g_Local.Fov = READ_BYTE();
	if (!g_Local.Fov)
		g_Local.Fov = 90;

	return pSetFOV(pszName, iSize, pbuf);
}

int TeamInfo(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ(pbuf, iSize);

	int iIndex = READ_BYTE();

	if(iIndex > 32)
		return false;

	PlayerInfo& p = g_Player[iIndex];

	sMe& l = g_Local;

	char *szTeam = READ_STRING();
	_strlwr(szTeam);	

	if (!strcmp(szTeam, "terrorist"))
	{
		p.Team = 1;
		if (iIndex == l.iIndex) { l.Team = 1; }
	}
	else if (!strcmp(szTeam, "ct"))
	{
		p.Team = 2;
		if (iIndex == l.iIndex) { l.Team = 2; }
	}
	else
	{
		p.Team = 0;
		if (iIndex == l.iIndex) { l.Team = 0; }
	}
	
	return pTeamInfo(pszName, iSize, pbuf);

}

int WeaponList(const char *pszName, int iSize, void *pbuf)
{
	return (*pWeaponList)(pszName, iSize, pbuf);
}

int AmmoX(const char *pszName, int iSize, void *pbuf)
{
	return (*pAmmoX)(pszName, iSize, pbuf);
}

int CurWeapon(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ(pbuf, iSize);
	int iState = READ_BYTE();
	int iWeaponID = READ_CHAR();
	int iClip = READ_CHAR();
	if (iState)
	{
		g_Local.Clip = iClip;
		g_Local.WpnID = iWeaponID;
	}
	return pCurWeapon(pszName, iSize, pbuf);
}

