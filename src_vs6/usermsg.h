#ifndef __USERMSG__
#define __USERMSG__

pfnUserMsgHook HookUserMsg(char *szMsgName, pfnUserMsgHook pfn);

typedef struct TUserMsg
{
	int number;
	int size;
	char name[16];
	struct TUserMsg* next;
	pfnUserMsgHook pfn;
} *PUserMsg;

extern int ResetHUD(const char *pszName, int iSize, void *pbuf);
extern int SetFOV(const char *pszName, int iSize, void *pbuf);
extern int TeamInfo(const char *pszName, int iSize, void *pbuf);
extern int NVGToggle(const char *pszName, int iSize, void *pbuf);
extern int CurWeapon(const char *pszName, int iSize, void *pbuf);
extern int DeathMsg(const char *pszName, int iSize, void *pbuf);
extern int AmmoX(const char *pszName, int iSize, void *pbuf);
extern int WeaponList(const char *pszName, int iSize, void *pbuf);
extern int ScoreAttrib(const char *pszName, int iSize, void *pbuf);
extern pfnUserMsgHook pResetHUD;
extern pfnUserMsgHook pSetFOV;
extern pfnUserMsgHook pTeamInfo;
extern pfnUserMsgHook pCurWeapon;
extern pfnUserMsgHook pDeathMsg;
extern pfnUserMsgHook pWeaponList;
extern pfnUserMsgHook pAmmoX;
extern pfnUserMsgHook pScoreAttrib;

extern void HookUserMessages();

extern PUserMsg pUserMsgBase;

#endif