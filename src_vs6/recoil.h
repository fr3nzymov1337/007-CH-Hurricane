/************************************************************************************************************************************
*
*
* 007 HOOK
* Code By: 007 + boy_scout
* msn: david_bs@live.com
* (c)2011
* www.etalking.com.ar
*
*
************************************************************************************************************************************/

#ifndef RECOIL_H
#define RECOIL_H

//**********************************************************************************************************************************

struct spread_info
{
	unsigned int random_seed;
	int recoil;
	float gtime;
	float prevtime;
	float brokentime; // This is only supposed to be set to zero when you buy the sg550 or the g3sg1
						// not when you reload, switch weapons or pick up a weapon, this is do to the
						// cs bugs for these guns (valve fix your code please)
	float spreadvar;
	float recoiltime;
	bool firing;
	int WeaponState;
	int prcflags;
	float speed;
	float spread[3];
};

#define WEAPONLIST_P228		1
#define	WEAPONLIST_UNKNOWN1	2
#define	WEAPONLIST_SCOUT	3
#define	WEAPONLIST_HEGRENADE 4
#define	WEAPONLIST_XM1014 5
#define	WEAPONLIST_C4 6
#define	WEAPONLIST_MAC10 7
#define	WEAPONLIST_AUG 8
#define	WEAPONLIST_SMOKEGRENADE 9
#define	WEAPONLIST_ELITE 10
#define	WEAPONLIST_FIVESEVEN 11
#define	WEAPONLIST_UMP45 12
#define	WEAPONLIST_SG550 13
#define	WEAPONLIST_GALIL 14
#define	WEAPONLIST_FAMAS 15
#define	WEAPONLIST_USP 16
#define	WEAPONLIST_GLOCK18 17
#define	WEAPONLIST_AWP 18
#define	WEAPONLIST_MP5 19
#define	WEAPONLIST_M249 20
#define	WEAPONLIST_M3 21
#define	WEAPONLIST_M4A1 22
#define	WEAPONLIST_TMP 23
#define	WEAPONLIST_G3SG1 24
#define	WEAPONLIST_FLASHBANG 25
#define	WEAPONLIST_DEAGLE 26
#define	WEAPONLIST_SG552 27
#define	WEAPONLIST_AK47 28
#define	WEAPONLIST_KNIFE 29
#define	WEAPONLIST_P90 30


float GetVecSpread(float* velocity);
void GetSpreadXY(unsigned int seed, int future, float* velocity, float* vec);
void GetRecoilOffset(unsigned int seed, int future, float* inangles, float* velocity, float* outangles);
void DefaultSpreadVar(int weaponid);
void Recoil(struct usercmd_s* usercmd);
void Nospread(struct usercmd_s* usercmd);
void DrawSpread();
void DrawRecoil();
void Spread_HUD_PostRunCmd ( struct local_state_s *from, struct local_state_s *to, struct usercmd_s *usercmd, int runfuncs, double time, unsigned int random_seed );
//**********************************************************************************************************************************

#endif

//**********************************************************************************************************************************
