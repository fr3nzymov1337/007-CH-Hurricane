#include "client.h"

PreS_DynamicSound_t PreS_DynamicSound_s;

void PreS_DynamicSound(int entid, DWORD unk0, char *szSoundFile, float *fOrigin, DWORD unk1, DWORD unk2, DWORD unk3, DWORD unk4)
{
	PlayerInfo& p = g_Player[entid];
	cl_entity_s *pEnt = g_pEngine->GetEntityByIndex(entid);
	if(entid>=0 && entid<=33 && entid)
	{
		if(cvar.misc_soundesp){
			if(pEnt->curstate.solid && pEnt->player)
				SoundCopy(p.previousSound,(float*)fOrigin);}
		
		if(pEnt->curstate.solid && pEnt->player && unk1 >= 0){
			if (strstr(szSoundFile, "player")) {
				if (strstr(szSoundFile, "bhit_helmet")) 
					g_Player[entid].iHealth -= 80;
				else if (strstr(szSoundFile, "bhit_kevlar"))
					g_Player[entid].iHealth -= 20;
				else if (strstr(szSoundFile, "bhit_flesh"))
					g_Player[entid].iHealth -= 30;
				else if (strstr(szSoundFile, "headshot")) 
					g_Player[entid].iHealth -= 80;
				else if (strstr(szSoundFile, "die") || strstr(szSoundFile, "death"))
					g_Player[entid].iHealth = 0;
				if (g_Player[entid].iHealth < 0)
					g_Player[entid].iHealth = 0;
			}
		}
	}
	PreS_DynamicSound_s(entid, unk0, szSoundFile, fOrigin, unk1, unk2, unk3, unk4);
}