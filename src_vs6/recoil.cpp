#include "client.h"

#define USP_SILENCER (1 << 0)
#define GLOCK18_BURST (1 << 1)
#define M4A1_SILENCER (1 << 2)
#define ELITE_LEFT (1 << 4)
#define FAMAS_BURST (1 << 4)

static unsigned int glSeed = 0; 

unsigned int seed_table[ 256 ] =
{
	28985, 27138, 26457, 9451, 17764, 10909, 28790, 8716, 6361, 4853, 17798, 21977, 19643, 20662, 10834, 20103,
	27067, 28634, 18623, 25849, 8576, 26234, 23887, 18228, 32587, 4836, 3306, 1811, 3035, 24559, 18399, 315,
	26766, 907, 24102, 12370, 9674, 2972, 10472, 16492, 22683, 11529, 27968, 30406, 13213, 2319, 23620, 16823,
	10013, 23772, 21567, 1251, 19579, 20313, 18241, 30130, 8402, 20807, 27354, 7169, 21211, 17293, 5410, 19223,
	10255, 22480, 27388, 9946, 15628, 24389, 17308, 2370, 9530, 31683, 25927, 23567, 11694, 26397, 32602, 15031,
	18255, 17582, 1422, 28835, 23607, 12597, 20602, 10138, 5212, 1252, 10074, 23166, 19823, 31667, 5902, 24630,
	18948, 14330, 14950, 8939, 23540, 21311, 22428, 22391, 3583, 29004, 30498, 18714, 4278, 2437, 22430, 3439,
	28313, 23161, 25396, 13471, 19324, 15287, 2563, 18901, 13103, 16867, 9714, 14322, 15197, 26889, 19372, 26241,
	31925, 14640, 11497, 8941, 10056, 6451, 28656, 10737, 13874, 17356, 8281, 25937, 1661, 4850, 7448, 12744,
	21826, 5477, 10167, 16705, 26897, 8839, 30947, 27978, 27283, 24685, 32298, 3525, 12398, 28726, 9475, 10208,
	617, 13467, 22287, 2376, 6097, 26312, 2974, 9114, 21787, 28010, 4725, 15387, 3274, 10762, 31695, 17320,
	18324, 12441, 16801, 27376, 22464, 7500, 5666, 18144, 15314, 31914, 31627, 6495, 5226, 31203, 2331, 4668,
	12650, 18275, 351, 7268, 31319, 30119, 7600, 2905, 13826, 11343, 13053, 15583, 30055, 31093, 5067, 761,
	9685, 11070, 21369, 27155, 3663, 26542, 20169, 12161, 15411, 30401, 7580, 31784, 8985, 29367, 20989, 14203,
	29694, 21167, 10337, 1706, 28578, 887, 3373, 19477, 14382, 675, 7033, 15111, 26138, 12252, 30996, 21409,
	25678, 18555, 13256, 23316, 22407, 16727, 991, 9236, 5373, 29402, 6117, 15241, 27715, 19291, 19888, 19847
};

unsigned int U_Random( void ) 
{ 
	glSeed *= 69069; 
	glSeed += seed_table[ glSeed & 0xff ];
	return ( ++glSeed & 0x0fffffff ); 
} 

void U_Srand( unsigned int seed )
{
	glSeed = seed_table[ seed & 0xff ];
}

int UTIL_SharedRandomLong( unsigned int seed, int low, int high )
{
	unsigned int range;
	U_Srand( (int)seed + low + high );
	range = high - low + 1;
	if ( !(range - 1) )
	{
		return low;
	}
	else
	{
		int offset;
		int rnum;
		rnum = U_Random();
		offset = rnum % range;
		return (low + offset);
	}
}

float UTIL_SharedRandomFloat( unsigned int seed, float low, float high )
{
	unsigned int range;
	U_Srand( (int)seed + *(int *)&low + *(int *)&high );
	U_Random();
	U_Random();
	range = (unsigned int)(high - low);
	if ( !range )
	{
		return low;
	}
	else
	{
		int tensixrand;
		float offset;
		tensixrand = U_Random() & 65535;
		offset = (float)tensixrand / 65536.0f;
		return (low + offset * range );
	}
}

void GetRecoil(int Id)
{
	switch(Id)
	{
	case WEAPONLIST_DEAGLE:
		cvar.norecoilvalue = 0.019f;
		break;
	case WEAPONLIST_ELITE:
		cvar.norecoilvalue = 0.029f;
		break;
	case WEAPONLIST_GALIL:
		cvar.norecoilvalue = 0.015f;
		break;
	case WEAPONLIST_FAMAS:
		cvar.norecoilvalue = 1.65f;
		break;
	case WEAPONLIST_FIVESEVEN:
		cvar.norecoilvalue = 0.0105f;
		break;
	case WEAPONLIST_GLOCK18:
		cvar.norecoilvalue = 0.015f;
		break;
	case WEAPONLIST_P228:
		cvar.norecoilvalue = 0.011f;
		break;
	case WEAPONLIST_G3SG1:
		cvar.norecoilvalue = 0.2f;
		break;
	case WEAPONLIST_SG550:
		cvar.norecoilvalue = 0.2f;
		break;
	case WEAPONLIST_USP:
		cvar.norecoilvalue = 0.015f;
		break;
	case WEAPONLIST_AK47:
		cvar.norecoilvalue = 1.923f;
		break;
	case WEAPONLIST_SG552:
		cvar.norecoilvalue = 1.67f;
		break;
	case WEAPONLIST_AUG:
		cvar.norecoilvalue = 1.62f;
		break;
	case WEAPONLIST_M249:
		cvar.norecoilvalue = 1.72f;
		break;
	case WEAPONLIST_M4A1:
		cvar.norecoilvalue = 1.65f;
		break;
	case WEAPONLIST_MP5:
		cvar.norecoilvalue = 1.6f;
		break;
	case WEAPONLIST_MAC10:
		cvar.norecoilvalue = 1.5f;
		break;
	case WEAPONLIST_P90:
		cvar.norecoilvalue = 1.5f;
		break;
	case WEAPONLIST_TMP:
		cvar.norecoilvalue = 1.5f;
		break;
	case WEAPONLIST_UMP45:
		cvar.norecoilvalue = 1.6f;
		break;
	case WEAPONLIST_AWP:
		cvar.norecoilvalue = 0.8f;
		break;
	case WEAPONLIST_SCOUT:
		cvar.norecoilvalue = 1.42f;
		break;
	default:
		break;
	}
}

float GetVecSpread(float speed)
{
	sMe& l = g_Local;
	float spread;

	switch (l.WpnID)
	{
	case WEAPONLIST_DEAGLE:
		if (l.spread.prcflags & FL_ONGROUND)
		{
			if (speed)
				spread = 0.25 * (1.0f - l.spread.spreadvar);
			else if (l.spread.prcflags & FL_DUCKING)
				spread = 0.115 * (1.0f - l.spread.spreadvar);
			else
				spread = 0.13 * (1.0f - l.spread.spreadvar);
		}
		else
			spread = 1.5 * (1.0f - l.spread.spreadvar);

		break;
	case WEAPONLIST_ELITE:
		if (l.spread.prcflags & FL_ONGROUND)
		{
			if (speed)
				spread = 0.175 * (1.0f - l.spread.spreadvar);
			else if (l.spread.prcflags & FL_DUCKING)
				spread = 0.08 * (1.0f - l.spread.spreadvar);
			else
				spread = 0.1 * (1.0f - l.spread.spreadvar);
		}
		else
			spread = 1.3 * (1.0f - l.spread.spreadvar);

		break;
	case WEAPONLIST_GALIL:
		if (l.spread.prcflags & FL_ONGROUND)
		{
			if (speed > 140.0f)
				spread = l.spread.spreadvar * 0.07 + 0.04;
			else
				spread = l.spread.spreadvar * 0.0375;
		}
		else
			spread = l.spread.spreadvar * 0.4 + 0.03;

		break;
	case WEAPONLIST_FAMAS:
		if (l.spread.prcflags & FL_ONGROUND)
		{
			if (speed > 140.0f)
				spread = l.spread.spreadvar * 0.07 + 0.03;
			else
				spread = l.spread.spreadvar * 0.020;
		}
		else
			spread = l.spread.spreadvar * 0.4 + .04;

		break;
	case WEAPONLIST_FIVESEVEN:
		if (l.spread.prcflags & FL_ONGROUND)
		{
			if (speed)
				spread = 0.255 * (1.0f - l.spread.spreadvar);
			else if (l.spread.prcflags & FL_DUCKING)
				spread = 0.075 * (1.0f - l.spread.spreadvar);
			else
				spread = 0.15 * (1.0f - l.spread.spreadvar);
		}
		else
			spread = 1.5 * (1.0f - l.spread.spreadvar);

		break;
	case WEAPONLIST_GLOCK18:
		if (!(l.spread.WeaponState & GLOCK18_BURST))
		{
			if (l.spread.prcflags & FL_ONGROUND)
			{
				if (speed)
					spread = 0.165 * (1.0f - l.spread.spreadvar);
				else if (l.spread.prcflags & FL_DUCKING)
					spread = 0.075 * (1.0f - l.spread.spreadvar);
				else
					spread = 0.1 * (1.0f - l.spread.spreadvar);
			}
			else
				spread = 1.0f - l.spread.spreadvar;
		}
		else
		{
			if (l.spread.prcflags & FL_ONGROUND)
			{
				if (speed)
					spread = 0.185 * (1.0f - l.spread.spreadvar);
				else if (l.spread.prcflags & FL_DUCKING)
					spread = 0.095 * (1.0f - l.spread.spreadvar);
				else
					spread = 0.3 * (1.0f - l.spread.spreadvar);
			}
			else
				spread = 1.2 * (1.0f - l.spread.spreadvar);
		}

		break;
	case WEAPONLIST_P228:
		if (l.spread.prcflags & FL_ONGROUND)
		{
			if (speed)
				spread = 0.255 * (1.0f - l.spread.spreadvar);
			else if (l.spread.prcflags & FL_DUCKING)
				spread = 0.075 * (1.0f - l.spread.spreadvar);
			else
				spread = 0.15 * (1.0f - l.spread.spreadvar);
		}
		else
			spread = 1.5 * (1.0f - l.spread.spreadvar);

		break;
	case WEAPONLIST_G3SG1:
		if (l.spread.prcflags & FL_ONGROUND)
		{
			if (speed)
				spread = 0.15f;
			else if (l.spread.prcflags & FL_DUCKING)
				spread = 0.035 * (1.0f - l.spread.spreadvar);
			else
				spread = 0.055 * (1.0f - l.spread.spreadvar);
		}
		else
			spread = 0.45 * (1.0f - l.spread.spreadvar);

		if (!(l.Fov < 90.0f))
			spread += 0.025f;

		break;
	case WEAPONLIST_SG550:
		if (l.spread.prcflags & FL_ONGROUND)
		{
			if (speed)
				spread = 0.15f;
			else if (l.spread.prcflags & FL_DUCKING)
				spread = 0.04 * (1.0f - l.spread.spreadvar);
			else
				spread = 0.05 * (1.0f - l.spread.spreadvar);
		}
		else
			spread = 0.45 * (1.0f - l.spread.spreadvar);

		if (!(l.Fov < 90.0f))
			spread += 0.025f;

		break;
	case WEAPONLIST_USP:
		if (!(l.spread.WeaponState & USP_SILENCER))
		{
			if (l.spread.prcflags & FL_ONGROUND)
			{
				if (speed)
					spread = 0.225 * (1.0f - l.spread.spreadvar);
				else if (l.spread.prcflags & FL_DUCKING)
					spread = 0.08 * (1.0f - l.spread.spreadvar);
				else
					spread = 0.1 * (1.0f - l.spread.spreadvar);
			}
			else
				spread = 1.2 * (1.0f - l.spread.spreadvar);
		}
		else
		{
			if (l.spread.prcflags & FL_ONGROUND)
			{
				if (speed)
					spread = 0.25 * (1.0f - l.spread.spreadvar);
				else if (l.spread.prcflags & FL_DUCKING)
					spread = 0.125 * (1.0f - l.spread.spreadvar);
				else
					spread = 0.15 * (1.0f - l.spread.spreadvar);
			}
			else
				spread = 1.3 * (1.0f - l.spread.spreadvar);
		}

		break;
	case WEAPONLIST_AK47:
		if (l.spread.prcflags & FL_ONGROUND)
		{
			if (speed > 140.0f)
				spread = l.spread.spreadvar * 0.07 + 0.04;
			else
				spread = l.spread.spreadvar * 0.0275;
		}
		else
			spread = l.spread.spreadvar * 0.4 + .04;

		break;
	case WEAPONLIST_SG552:
		if (l.spread.prcflags & FL_ONGROUND)
		{
			if (speed > 140.0f)
				spread = l.spread.spreadvar * 0.07 + 0.035;
			else
				spread = l.spread.spreadvar * 0.02;
		}
		else
			spread = l.spread.spreadvar * 0.4 + .035;

		break;
	case WEAPONLIST_AUG:
		if (l.spread.prcflags & FL_ONGROUND)
		{
			if (speed > 140.0f)
				spread = l.spread.spreadvar * 0.07 + 0.035;
			else
				spread = l.spread.spreadvar * 0.02;
		}
		else
			spread = l.spread.spreadvar * 0.4 + .035;

		break;
	case WEAPONLIST_M249:
		if (l.spread.prcflags & FL_ONGROUND)
		{
			if (speed > 140.0f)
				spread = l.spread.spreadvar * 0.095 + 0.045;
			else
				spread = l.spread.spreadvar * 0.03;
		}
		else
			spread = l.spread.spreadvar * 0.5 + .045;

		break;
	case WEAPONLIST_M4A1:
		if (l.spread.prcflags & FL_ONGROUND)
		{
			if (!(l.spread.WeaponState & M4A1_SILENCER))
			{
				if (speed > 140.0f)
					spread = l.spread.spreadvar * 0.07 + 0.035;
				else
					spread = l.spread.spreadvar * 0.02;
			}
			else
			{
				if (speed > 140.0f)
					spread = l.spread.spreadvar * 0.07 + 0.035;
				else
					spread = l.spread.spreadvar * 0.025;
			}
		}
		else
			spread = l.spread.spreadvar * 0.4 + .035;

		break;
	case WEAPONLIST_MP5:
		if (l.spread.prcflags & FL_ONGROUND)
			spread = 0.04 * l.spread.spreadvar;
		else
			spread = 0.2 * l.spread.spreadvar;

		break;
	case WEAPONLIST_MAC10:
		if (l.spread.prcflags & FL_ONGROUND)
			spread = 0.03 * l.spread.spreadvar;
		else
			spread = 0.375 * l.spread.spreadvar;

		break;
	case WEAPONLIST_P90:
		if (l.spread.prcflags & FL_ONGROUND)
		{
			if (speed > 170.0f)
				spread = l.spread.spreadvar * 0.115;
			else
				spread = l.spread.spreadvar * 0.045;
		}
		else
			spread = l.spread.spreadvar * 0.3;

		break;
	case WEAPONLIST_TMP:
		if (l.spread.prcflags & FL_ONGROUND)
			spread = 0.03 * l.spread.spreadvar;
		else
			spread = 0.25 * l.spread.spreadvar;

		break;
	case WEAPONLIST_UMP45:
		if (l.spread.prcflags & FL_ONGROUND)
			spread = 0.04 * l.spread.spreadvar;
		else
			spread = 0.24 * l.spread.spreadvar;

		break;
	case WEAPONLIST_AWP:
		if (l.spread.prcflags & FL_ONGROUND)
		{
			if (speed < 10.0f)
			{
				if (l.spread.prcflags & FL_DUCKING)
					spread = 0;
				else
					spread = 0.001f;
			}
			else if (speed < 140.0f)
			{
				spread = 0.1f;
			}
			else
				spread = 0.25f;
		}
		else
			spread = 0.85f;

		if (!(l.Fov < 90.0f))
			spread += 0.08f;

		break;
	case WEAPONLIST_SCOUT:
		if (l.spread.prcflags & FL_ONGROUND)
		{
			if (speed < 170.0f)
			{
				if (l.spread.prcflags & FL_DUCKING)
					spread = 0;
				else
					spread = 0.007f;
			}
			else
				spread = 0.075f;
		}
		else
			spread = 0.2f;

		if (!(l.Fov < 90.0f))
			spread += 0.025f;

		break;
	default:
		spread = 0;
		break;
	}

	return spread;
}

void GetSpreadXY(unsigned int seed, int future, float* velocity, float* vec)
{
	sMe& l = g_Local;
	float vecspread;

	vec[0] = UTIL_SharedRandomFloat(seed + future, -0.5, 0.5) + UTIL_SharedRandomFloat(seed + 1 + future, -0.5, 0.5);

	vec[1] = UTIL_SharedRandomFloat(seed + 2 + future, -0.5, 0.5) + UTIL_SharedRandomFloat(seed + 3 + future, -0.5, 0.5);

	l.spread.speed = sqrt(velocity[0] * velocity[0] + velocity[1] * velocity[1]);
	
	vecspread = GetVecSpread(l.spread.speed);

	vec[0] *= vecspread;
	vec[1] *= vecspread;

	return;
}

void GetRecoilOffset(unsigned int seed, int future, float* inangles, float* velocity, float* outangles)
{
	float forward[3], right[3], up[3], vecDir[3];
	float view[3], dest[3], spread[2];
	
	g_Engine.pfnAngleVectors(inangles, forward, right, up);

	GetSpreadXY(seed, future, velocity, spread);

	vecDir[0] = forward[0] + spread[0] * right[0] + spread[1] * up[0];
	view[0] = 8192 * vecDir[0];

	vecDir[1] = forward[1] + spread[0] * right[1] + spread[1] * up[1];
	view[1] = 8192 * vecDir[1];

	vecDir[2] = forward[2] + spread[0] * right[2] + spread[1] * up[2];
	view[2] = 8192 * vecDir[2];

	VectorAngles(view, dest);
	dest[0] *= -1;

	outangles[0] = inangles[0] - dest[0];
	outangles[1] = inangles[1] - dest[1];
	outangles[2] = 0;
}

void DefaultSpreadVar(int weaponid)
{
	sMe& l = g_Local;

	switch(weaponid)
	{
	case WEAPONLIST_DEAGLE:
		l.spread.spreadvar = 0.9f;
		l.weaponspeed  = 250.0f-2.0f;
		break;
	case WEAPONLIST_P228:
		l.spread.spreadvar = 0.9f;
		l.weaponspeed  = 250.0f-2.0f;
		break;
	case WEAPONLIST_FIVESEVEN:
		l.spread.spreadvar = 0.9f;
		l.weaponspeed  = 250.0f-2.0f;
		break;
	case WEAPONLIST_ELITE:
		l.spread.spreadvar = 0.8f;
		l.weaponspeed  = 250.0f-2.0f;
		break;
	case WEAPONLIST_GLOCK18:
		l.spread.spreadvar = 0.9f;
		l.weaponspeed  = 250.0f-2.0f;
		break;
	case WEAPONLIST_USP:
		l.spread.spreadvar = 0.9f;
		l.weaponspeed  = 250.0f-2.0f;
		break;
	case WEAPONLIST_TMP:
		l.spread.spreadvar = 0.2f;
		l.weaponspeed  = 250.0f-2.0f;
		break;
	case WEAPONLIST_MAC10:
		l.spread.spreadvar = 0.15f;
		l.weaponspeed  = 250.0f-2.0f;
		break;
	case WEAPONLIST_P90:
		l.spread.spreadvar = 0.15f;
		l.weaponspeed  = 245.0f-2.0f;
		break;
	case WEAPONLIST_MP5:
		l.spread.spreadvar = 0;// -------------------> 0
		l.weaponspeed  = 250.0f-2.0f;
		break;
	case WEAPONLIST_UMP45:
		l.spread.spreadvar = 0;// -------------------> 0
		l.weaponspeed  = 250.0f-2.0f;
		break;
	case WEAPONLIST_M3:
		l.spread.spreadvar = 0;// -------------------> 0
		l.weaponspeed  = 230.0f-2.0f;
		break;
	case WEAPONLIST_XM1014:
		l.spread.spreadvar = 0;// -------------------> 0
		l.weaponspeed = 240.0f-2.0f;
		break;
	case WEAPONLIST_FAMAS:
		l.spread.spreadvar = 0.2f;// -------------------> 0
		l.weaponspeed  = 240.0f-2.0f;
		break;
	case WEAPONLIST_GALIL:
		l.spread.spreadvar = 0.2f;// -------------------> 0
		l.weaponspeed  = 240.0f-2.0f;
		break;
	case WEAPONLIST_AUG:
		l.spread.spreadvar = 0.3f;
		l.weaponspeed  = 240.0f-2.0f;
		break;
	case WEAPONLIST_SG552:
		l.spread.spreadvar = 0.2f;
		l.weaponspeed  = 235.0f-2.0f;
		break;
	case WEAPONLIST_M4A1:
		l.spread.spreadvar = 0.2f;
		l.weaponspeed  = 230.0f-2.0f;
		break;
	case WEAPONLIST_AK47:
		l.spread.spreadvar = 0.2f;
		l.weaponspeed  = 221.0f-2.0f;
		break;
	case WEAPONLIST_AWP:
		l.spread.spreadvar = 0;// -------------------> 0
		l.weaponspeed  = 210.0f-2.0f;
		break;
	case WEAPONLIST_SCOUT:
		l.spread.spreadvar = 0;// -------------------> 0
		l.weaponspeed  = 260.0f-2.0f;
		break;
	case WEAPONLIST_SG550:
		l.spread.spreadvar = 0.2f;
		l.weaponspeed  = 210.0f-2.0f;
		break;
	case WEAPONLIST_G3SG1:
		l.spread.spreadvar = 0.2f;
		l.weaponspeed  = 210.0f-2.0f;
		break;
	case WEAPONLIST_M249:
		l.spread.spreadvar = 0.2f;
		l.weaponspeed  = 220.0f-2.0f;
		break;
	default:
		l.spread.spreadvar = 0;
		l.weaponspeed  = 0;
		break;
    }
}

void Recoil(struct usercmd_s* usercmd)
{
	sMe& l = g_Local;

	if(cvar.norecoil && !IsKnife(l.WpnID) && !IsExplosive(l.WpnID) && (l.HudAlive || l.MsgAlive))
	{
		if(cvar.aim_psilent?CanAttack():1)
		{
			usercmd->viewangles[0] -= (l.PunchAngle[0] * cvar.norecoilvalue);
			usercmd->viewangles[1] -= (l.PunchAngle[1] * cvar.norecoilvalue);
			usercmd->viewangles[2] -= (l.PunchAngle[2] * cvar.norecoilvalue);
		}
	}
}

void Nospread(struct usercmd_s* usercmd)
{
	sMe& l = g_Local;
	float offset[3];

	GetRecoilOffset(l.spread.random_seed, 1, usercmd->viewangles, l.Velocity, offset);
	VectorCopy(offset, l.NoSpreadAng);
	if(cvar.nospread && (usercmd->buttons & IN_ATTACK) && !IsKnife(l.WpnID) && !IsExplosive(l.WpnID) && (l.HudAlive || l.MsgAlive))
	{
		if(cvar.aim_psilent?CanAttack():1)
		{
			ApplySilentAngles(offset, usercmd, true);
		}
	}
}

void DrawSpread()
{
	sMe& l = g_Local;
	if(cvar.nospreaddraw && cvar.nospread)
	{
		if(!IsKnife(l.WpnID) && !IsExplosive(l.WpnID) && (l.HudAlive || l.MsgAlive))
		{
			float xx = l.NoSpreadAng[1];
			float yy = l.NoSpreadAng[0];

			float x = (g_Screen.iWidth / 2.0f) - (xx * 10);
			float y = (g_Screen.iHeight / 2.0f) + (yy * 10);

			g_Engine.pfnFillRGBA(x-0.5, y, 3, 3, cvar.color_red*255.0f, cvar.color_green*255.0f, cvar.color_blue*255.0f, cvar.misc_tint*255.0f);
			blackBorder(x-0.5,y,3,4,cvar.misc_tint*255.0f);
		}
	}
}

void DrawRecoil()
{
	sMe& l = g_Local;
	if(cvar.norecoildraw && cvar.norecoil)
	{
		if(!IsKnife(l.WpnID) && !IsExplosive(l.WpnID) && (l.HudAlive || l.MsgAlive))
		{
			float xx = l.NoRecoilAng[1];
			float yy = l.NoRecoilAng[0];

			float x = (g_Screen.iWidth / 2.0f) - (xx * 10);
			float y = (g_Screen.iHeight / 2.0f) + (yy * 10);

			g_Engine.pfnFillRGBA(x-0.5, y, 3, 3, 0, 255, 0, cvar.misc_tint*255.0f);
			blackBorder(x-0.5,y,3,4,cvar.misc_tint*255.0f);
		}
	}
}

void Spread_HUD_PostRunCmd ( struct local_state_s *from, struct local_state_s *to, struct usercmd_s *usercmd, int runfuncs, double time, unsigned int random_seed )
{
	sMe& l = g_Local;

	int Id;

	if (runfuncs)
	{
		l.Fov = to->client.fov;
		l.Clip = to->weapondata[to->client.m_iId].m_iClip;
		l.WpnID=to->client.m_iId;
		l.m_flNextPrimaryAttack = to->weapondata[to->client.m_iId].m_flNextPrimaryAttack;
		l.m_iInReload = (to->weapondata[to->client.m_iId].m_fInReload || !to->weapondata[to->client.m_iId].m_iClip);
		l.m_flNextAttack = to->client.m_flNextAttack;
		l.speed=to->client.velocity.Length();

		l.spread.random_seed = random_seed;
		l.spread.gtime = time;
		l.spread.prcflags = to->client.flags;
		l.speed=to->client.velocity.Length();
		
		Id = to->client.m_iId;

		if (Id >= 0 && Id < MAX_WEAPONS)
			l.spread.WeaponState = to->weapondata[Id].m_iWeaponState;

		if (!(usercmd->buttons & (IN_ATTACK | IN_ATTACK2)))
		{
			if (l.spread.firing)
			{
				l.spread.firing = false;

				if (l.spread.recoil > 15)
					l.spread.recoil = 15;

				l.spread.recoiltime = time + 0.4f;
			}

			if (l.WpnID >= WEAPONLIST_USP && l.WpnID <= WEAPONLIST_FIVESEVEN)
			{
				l.spread.recoil = 0;
			}
			else if (l.spread.recoil > 0)
			{
				if (l.spread.recoiltime <= time)
				{
					l.spread.recoiltime = l.spread.recoiltime + 0.0225f;
					l.spread.recoil--;
				}
			}
		}
		if (Id >= 0 && Id < MAX_WEAPONS && to->weapondata[Id].m_fInReload)
		{
			l.spread.recoil = 0;
			l.spread.prevtime = 0;
			DefaultSpreadVar(Id);
			l.spread.recoiltime = time;
			l.spread.firing = false;
		}
		
		if (l.LastWpnID != l.WpnID)
		{
			l.LastWpnID = l.WpnID;

			l.spread.recoil = 0;
			l.spread.prevtime = 0;
			DefaultSpreadVar(Id);
			l.spread.recoiltime = time;
			l.spread.firing = true;

			if(l.HudAlive || l.MsgAlive)
			{
				char signal[256];
				sprintf(signal,"txt |[ %s ]|",GetWeaponName(g_pEngine->GetLocalPlayer()->curstate.weaponmodel));
				cmd.exec(signal);
				GetRecoil(Id);
			}
		}
	}
}
