#ifndef __SOUNDHOOK_H__
#define __SOUNDHOOK_H__

struct player_sound_t
{
	vec3_t vOrigin;
	DWORD dwTime;
};

typedef void(*PreS_DynamicSound_t)(int, DWORD, char *, float *, DWORD, DWORD, DWORD, DWORD);
extern PreS_DynamicSound_t PreS_DynamicSound_s;
void PreS_DynamicSound(int entid, DWORD unk0, char *szSoundFile, float *fOrigin, DWORD unk1, DWORD unk2, DWORD unk3, DWORD unk4);

#endif
