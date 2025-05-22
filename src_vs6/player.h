#ifndef __PLAYERS__
#define __PLAYERS__

struct sMe
{
	cl_entity_s *pEnt;
	int Team;
	bool HudAlive;
	bool MsgAlive;
	int iIndex;
	int Fov;
	int MoveType;
	vec3_t Eye;
	Vector Forward;
	int WpnID;
	int LastWpnID;
	int Clip;
	float PunchAngle[3];
	float ViewAngles[3];
	float fTrigRadius;
	float fTrigDiff;
	Vector Origin;
	float Velocity[3];
	float speed;
	float weaponspeed;
	spread_info spread;
	Vector NoSpreadAng;
	Vector NoRecoilAng;
	int Flags;
	float FSpeed;
	Vector VVelocity;
	float FVSpeed;
	float FlHeight;
	Vector VOrigin;
	int UseHull;
	float m_flNextPrimaryAttack;
	float m_flNextAttack;
	int m_iInReload;
	bool infrozentime;
	float flFallSpeed;
	double FrameTime;
	float flDuckTimeMax;
	float fSpeed;
	bool bJumped;
	Vector vt1,vt2;
	Vector StartJumpPos;
	float flJumpDist;
	float flJumpMessTime;
	float flGroundAngle;
	int health;
	float AirAccelerate;
	float Waterlevel;
	float FAngleSpeed;
	Vector vAngles;
	float MaxSpeed;
};
extern sMe g_Local;

struct PlayerInfo
{
	//idhook
	hud_player_info_t entinfo;
	int entindex;
	void init( int _entindex) 
	{
		iHealth = 100;
		Team=0;
		entinfo.name="\\missing-name\\";
		entinfo.model="missing-model";
		entindex = _entindex;
	}
	PlayerInfo() { init(0); }
	//

	cl_entity_s *pEnt;
	int Team;
	bool Alive;
	bool IsValidEnt;
	bool IsValidTeam;
	bool IsVisible;
	bool Ducked;
	int iHealth;

	player_sound_t  previousSound;

	float AimFov;
	float AimDistance;
	bool AimVisible;
	Vector AimBone[53];
	Vector AimHitbox[21];
	Vector AimAngle;
	Vector Hud_AimOrigin;
	Vector Hud_AimOriginEye;
	Vector CL_AimOrigin;

	vec3_t VOrigin;
};
extern PlayerInfo g_Player[33];

extern bool CheckEntity(cl_entity_s *pEnt);
extern bool CheckEntity(int iIndex);
extern void g_LocalpEnt();
extern void g_LocaliIndex();
extern void g_LocalAlive();
extern float GetDist(float *from, float *to);
extern void VectorAnglesAimbot(const float *forward,float *angles);
extern void VectorAngles(const float *forward, float *angles);
extern bool IsKnife(int id);
extern bool IsExplosive(int id);
extern bool IsSecondary(int id);
extern bool IsShielded(int iIndex);
extern bool CheckTeam(int iIndex);
extern bool Visible(int iIndex);
extern void ApplySilentAngles(float *angles, usercmd_s *cmd, bool add_angle);
extern void g_LocalTeam();
extern void g_PlayerTeam();
extern void AntiAim(usercmd_s *cmd);
extern void ContinueFire(usercmd_s *cmd);
extern void FixupAngleDifference(usercmd_s *cmd);
extern char* GetWeaponName(int weaponmodel);
extern int Cstrike_SequenceInfo[];
extern int Cstrike_EGaitSequenceInfo[];
extern bool isEnemy(int i);
extern void Noflash();
extern void g_player();
extern float YawForVec(float* fwd);
extern void Chase();
extern bool CanAttack(void);
extern bool fileExists(const char* filename);
extern void State();
extern bool ValidEntity(cl_entity_s *pEnt);

//
extern void NoHUD();
extern void NoRefresh();
extern void QuakeGuns();
extern void SayHelloHook();
extern void ExcellentSettings();
extern float GetGunOffset(cl_entity_s* vm);

#endif