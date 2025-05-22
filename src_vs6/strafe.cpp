#include "client.h"

StrafeBot Strafebot;

static bool balance_bh = false;
#define PLAYER_MAX_SAFE_FALL_SPEED	580// approx 20 feet
static bool rand_strafe;
static float fMaxPspeed = 0.0;
float FrameCnt;
float FpsCnt;
float InterpFps;
float PreStrafe;
float JumpOff;

bool Strafe = false;
bool Fastrun = false;
bool Gstrafe = false;
bool StrafeBhop = false;
bool Jumpbug = false;

void func_strafe_on(){
	Strafe = true;}
void func_strafe_off(){
	Strafe = false;}
void func_jumpbug_on(){
	Jumpbug = true;}
void func_jumpbug_off(){
	Jumpbug = false;}
void func_bhop_on(){
	StrafeBhop = true;}
void func_bhop_off(){
	StrafeBhop = false;}
void func_gstrafe_on(){
	Gstrafe = true;}
void func_gstrafe_off(){
	Gstrafe = false;}
void func_fastrun_on(){
	Fastrun = true;}
void func_fastrun_off(){
	Fastrun = false;}


void func_Speeder1(){
	cvar.wavspeed = 0.1f;}
void func_Speeder2(){
	cvar.wavspeed = 0.2f;}
void func_Speeder3(){
	cvar.wavspeed = 0.3f;}
void func_Speeder4(){
	cvar.wavspeed = 0.4f;}
void func_Speeder5(){
	cvar.wavspeed = 0.5f;}
void func_Speeder6(){
	cvar.wavspeed = 0.6f;}
void func_Speeder7(){
	cvar.wavspeed = 0.7f;}
void func_Speeder8(){
	cvar.wavspeed = 0.8f;}
void func_Speeder9(){
	cvar.wavspeed = 0.9f;}
void func_Speeder10(){
	cvar.wavspeed = 1.0f;}

void StrafeCommands(){
	cmd.AddCommand( "strafe_on", func_strafe_on);
	cmd.AddCommand( "strafe_off", func_strafe_off);
	cmd.AddCommand( "bhop_on", func_bhop_on);
	cmd.AddCommand( "bhop_off", func_bhop_off);
	cmd.AddCommand( "gstrafe_on", func_gstrafe_on);
	cmd.AddCommand( "gstrafe_off", func_gstrafe_off);
	cmd.AddCommand( "jumpbug_on", func_jumpbug_on);
	cmd.AddCommand( "jumpbug_off", func_jumpbug_off);
	cmd.AddCommand( "fastrun_on", func_fastrun_on);
	cmd.AddCommand( "fastrun_off", func_fastrun_off);
	cmd.AddCommand( "speeder1", func_Speeder1);
	cmd.AddCommand( "speeder2", func_Speeder2);
	cmd.AddCommand( "speeder3", func_Speeder3);
	cmd.AddCommand( "speeder4", func_Speeder4);
	cmd.AddCommand( "speeder5", func_Speeder5);
	cmd.AddCommand( "speeder6", func_Speeder6);
	cmd.AddCommand( "speeder7", func_Speeder7);
	cmd.AddCommand( "speeder8", func_Speeder8);
	cmd.AddCommand( "speeder9", func_Speeder9);
	cmd.AddCommand( "speeder10", func_Speeder10);}

bool BindStrafe = true;
bool UnBindStrafe = false;
bool BindBhop = true;
bool UnBindBhop = false;
bool BindGstrafe = true;
bool UnBindGstrafe = false;
bool BindSlowDown = true;
bool UnBindSlowDown = false;
bool BindJumpBug = true;
bool UnBindJumpBug = false;
bool BindFastRun = true;
bool UnBindFastRun = false;
bool RunOnce = true;

void Bind()
{
	if(cvar.kzstrafehack && BindStrafe){
		cmd.exec("bindstrafe");
		BindStrafe = false;
		UnBindStrafe = true;}
	if(!cvar.kzstrafehack && UnBindStrafe){
		cmd.exec("unbindstrafe");
		BindStrafe = true;
		UnBindStrafe = false;}
	if(cvar.kzbhop && BindBhop){
		cmd.exec("bindbhop");
		BindBhop = false;
		UnBindBhop = true;}
	if(!cvar.kzbhop && UnBindBhop){
		cmd.exec("unbindbhop");
		BindBhop = true;
		UnBindBhop = false;}
	if(cvar.kzgroundstrafe && BindGstrafe){
		cmd.exec("bindgstrafe");
		BindGstrafe = false;
		UnBindGstrafe = true;}
	if(!cvar.kzgroundstrafe && UnBindGstrafe){
		cmd.exec("unbindgstrafe");
		BindGstrafe = true;
		UnBindGstrafe = false;}
	if(cvar.kzslowdown && BindSlowDown){
		cmd.exec("bindslowdown");
		BindSlowDown = false;
		UnBindSlowDown = true;}
	if(!cvar.kzslowdown && UnBindSlowDown){
		cmd.exec("unbindslowdown");
		BindSlowDown = true;
		UnBindSlowDown = false;}
	if(cvar.kzjumpbug && BindJumpBug){
		cmd.exec("bindjumpbug");
		BindJumpBug = false;
		UnBindJumpBug = true;}
	if(!cvar.kzjumpbug && UnBindJumpBug){
		cmd.exec("unbindjumpbug");
		BindJumpBug = true;
		UnBindJumpBug = false;}
	if(cvar.kzfastrun && BindFastRun){
		cmd.exec("bindfastrun");
		BindFastRun = false;
		UnBindFastRun = true;}
	if(!cvar.kzfastrun && UnBindFastRun){
		cmd.exec("unbindfastrun");
		BindFastRun = true;
		UnBindFastRun = false;}
}

inline float EndSpeed(float StartSpeed,float gravity,float frametime,float distance)
{
	while(distance>0)
	{
		StartSpeed+=gravity*frametime;
		float dist=StartSpeed*frametime;
		distance-=dist;
	}
	return StartSpeed;
}

inline float interp(float s1,float s2,float s3,float f1,float f3)
{
	if(s2==s1)return f1;
	if(s2==s3)return f3;
	if(s3==s1)return f1;
	return f1+((s2-s1)/(s3-s1))*((f3-f1)/*/1*/);
}

float Damage()
{
	Vector start=g_Local.Origin;
	vec3_t vForward, vecEnd;
	float va[3];
	g_Engine.GetViewAngles(va);
	g_Engine.pfnAngleVectors(va, vForward, NULL, NULL);
	vecEnd[0] = start[0] + vForward[0] * 8192; vecEnd[1] = start[1] + vForward[1] * 8192; vecEnd[2] = start[2] + vForward[2] * 8192;
	pmtrace_t *trace = g_pEngine->PM_TraceLine(start, vecEnd, 1, 2, -1);
	float fDistance=((start.z)-(trace->endpos.z))-(g_Local.UseHull==0?(36):(18));
	float endSpeed=EndSpeed(g_Local.flFallSpeed,800,1/1000.0f,fDistance);
	if(interp(504.80001f,endSpeed,1000,1,100) > 0)
		return interp(504.80001f,endSpeed,1000,1,100);
	else return 0;
}

inline float EdgeDistance(){
#define TraceEdge(x,y){\
	Vector start=g_Local.Origin;\
	start[2]-=0.1f;\
	Vector end=start;\
	end[1]+=x*mind;\
	end[0]+=y*mind;\
	pmtrace_s* t1 = g_pEngine->PM_TraceLine(end,start,1,g_Local.UseHull,-1);\
	if(!(t1->startsolid))mind=(t1->endpos-start).Length2D();\
	}
	float mind=250;
	TraceEdge(-1,0);
	TraceEdge(1,0);
	TraceEdge(0,1);
	TraceEdge(0,-1);
	TraceEdge(-1,-1);
	TraceEdge(1,1);
	TraceEdge(1,-1);
	TraceEdge(-1,1);
	return mind;
}

void CLStrafeBot(float frametime, struct usercmd_s *cmd)
{
	g_Local.FrameTime = frametime;

	if(cvar.kzstrafehack == 1) 
		Strafebot.StrafeHack(frametime,cmd,cvar.kzstrafe_invisible,cvar.kzstrafe_crazy);
	if(cvar.kzstrafeautodir)
		Strafebot.AutoDir(cmd);
	if(cvar.kzfastrun == 1)
		Strafebot.FastRun(cmd);
	if(cvar.kzgroundstrafe == 1)
		Strafebot.GroundStrafe(cmd);
	if(cvar.kzbhop == 1)
		Strafebot.Bhop(frametime, cmd);
		if((g_Engine.GetClientTime() - g_Local.flDuckTimeMax ) >= 0.8 && balance_bh == true)
		{
			g_Engine.pfnClientCmd("-duck");
			balance_bh = false;
		}
	if(cvar.kzjumpbug == 1)
		Strafebot.JumpBug(frametime, cmd);
	if(cvar.kzstrafe_helper)
		if( g_Local.fSpeed > 0 )
		{
			Strafebot.StrafeHelper(frametime,cmd);
		}
	if(cvar.kzfps_helper > 0)
		AdjustSpeed(((1/frametime)/cvar.kzfps_helper));
	if(g_Local.bJumped && (g_Local.Flags&FL_ONGROUND||g_Local.MoveType == 5))
	{
		Vector endpos=g_Local.Origin;
		endpos.z-=g_Local.UseHull==0?36.0:18.0;
		g_Local.vt2=endpos;
		if(endpos.z==g_Local.StartJumpPos.z)
		{
			Vector lj=endpos-g_Local.StartJumpPos;
			float dist=lj.Length()+32.0625f+0.003613;
			if(dist>=200)
			{
				InterpFps = FpsCnt / FrameCnt;
				FpsCnt = 0;
				FrameCnt = 0;
				g_Local.flJumpDist=dist;
				g_Local.flJumpMessTime=g_Engine.GetClientTime()+7.0;
			}
		}
		g_Local.bJumped=false;
	}
	if(!g_Local.bJumped && (g_Local.Flags&FL_ONGROUND) && cmd->buttons&IN_JUMP)
	{
		PreStrafe = g_Local.fSpeed;
		if(EdgeDistance() !=250)
			JumpOff = EdgeDistance();
		else JumpOff = 0;
		g_Local.StartJumpPos=g_Local.Origin;
		g_Local.StartJumpPos.z-=g_Local.UseHull==0?36.0:18.0;
		g_Local.vt1=g_Local.StartJumpPos;
		g_Local.bJumped=true;
	}
	Bind();
}

void HUDStrafeBot()
{
	if(g_Local.bJumped)
	{
		FrameCnt += 1;
		FpsCnt += (1/g_Local.FrameTime);
	}
	if(cvar.kzshow_kz)
	{
		static float Y = iStringHeight();
		if(g_Local.flJumpMessTime > g_Engine.GetClientTime())
		{
			float ft1[2];
			float ft2[2];
			gDrawFilledBoxAtLocation(g_Local.vt1,255,0,0,255,2);
			gDrawFilledBoxAtLocation(g_Local.vt2,255,0,0,255,2);
			DrawStippledVectorLine(g_Local.vt1,g_Local.vt2,2,4,0xAAAA,0,255,0,255);
			if(WorldToScreen(g_Local.vt1,ft1))
			{
				gFont.drawString( true, ft1[0], ft1[1]-14, 255, 255, 255, "Start");
			}
			if(WorldToScreen(g_Local.vt2,ft2))
			{
				gFont.drawString( true, ft2[0], ft2[1]-14, 255, 255, 255, "Length: %.3f", g_Local.flJumpDist);
			}
			gFont.drawString( true,g_Screen.iWidth / 2, g_Screen.iHeight * 0.22 + Y * 1.0, 255,0,255, "LongJump: %.3f", g_Local.flJumpDist);
			gFont.drawString( true,g_Screen.iWidth / 2, g_Screen.iHeight * 0.22 + Y * 2.0, 255,0,255, "PreStrafe: %.3f", PreStrafe);
			gFont.drawString( true,g_Screen.iWidth / 2, g_Screen.iHeight * 0.22 + Y * 3.0, 255,0,255, "JumpOff: %.3f", JumpOff);
			gFont.drawString( true,g_Screen.iWidth / 2, g_Screen.iHeight * 0.22 + Y * 4.0, 255,0,255, "Fps: %.3f", InterpFps);
		}
		if(g_Local.fSpeed == 0)
			fMaxPspeed = 0.0;
		if(g_Local.fSpeed > fMaxPspeed)
			fMaxPspeed = g_Local.fSpeed;
		gFont.drawString( true,g_Screen.iWidth / 2, g_Screen.iHeight * 0.22 + Y * 5.0, 255, 255, 255, "Speed: %.3f", g_Local.fSpeed );
		gFont.drawString( true,g_Screen.iWidth / 2, g_Screen.iHeight * 0.22 + Y * 6.0, 255, 255, 255, "SpeedMax: %.3f", fMaxPspeed );
		gFont.drawString( true,g_Screen.iWidth / 2, g_Screen.iHeight * 0.22 + Y * 7.0, 255, 255, 255, "Height: %.3f", g_Local.FlHeight );
		gFont.drawString( true,g_Screen.iWidth / 2, g_Screen.iHeight * 0.22 + Y * 8.0, 255, 255, 255, "Fallspeed: %.3f", g_Local.flFallSpeed );
		gFont.drawString( true,g_Screen.iWidth / 2, g_Screen.iHeight * 0.22 + Y * 9.0, 255, 255, 255, "GroundAngle: %.3f", g_Local.flGroundAngle );
		int color[3];
		color[0] = 255;
		color[1] = 255;
		color[2] = 255;
		if(Damage() >= g_Local.health)
		{
			color[0] = 255;
			color[1] = 0;
			color[2] = 0;
		}
		gFont.drawString( true,g_Screen.iWidth / 2, g_Screen.iHeight * 0.22 + Y * 10.0, color[0],color[1],color[2], "Damage: %.1f", Damage());
		gFont.drawString( true,g_Screen.iWidth / 2, g_Screen.iHeight * 0.22 + Y * 11.0, 255,0,EdgeDistance(), "Edge: %.3f", EdgeDistance());
	}
	else
		fMaxPspeed = 0.0;
}

void RotateInvisible(float fixed_yaw, float fixed_pitch, usercmd_s *cmd)
{
	Vector viewforward, viewright, viewup, aimforward, aimright, aimup, vTemp;
	float newforward, newright, newup;
	float forward = cmd->forwardmove;
	float right = cmd->sidemove;
	float up = cmd->upmove;
	Vector tipo_real_va;
	VectorCopy(cmd->viewangles,tipo_real_va);

	g_Engine.pfnAngleVectors(Vector(0.0f, tipo_real_va.y, 0.0f), viewforward, viewright, viewup);
	tipo_real_va.y += fixed_yaw;
	g_Engine.pfnAngleVectors(Vector(0.0f, tipo_real_va.y, 0.0f), aimforward, aimright, aimup);

	newforward = DotProduct(forward * viewforward.Normalize(), aimforward) + DotProduct(right * viewright.Normalize(), aimforward) + DotProduct(up * viewup.Normalize(), aimforward);
	newright = DotProduct(forward * viewforward.Normalize(), aimright) + DotProduct(right * viewright.Normalize(), aimright) + DotProduct(up * viewup.Normalize(), aimright);
	newup = DotProduct(forward * viewforward.Normalize(), aimup) + DotProduct(right * viewright.Normalize(), aimup) + DotProduct(up * viewup.Normalize(), aimup);

	if (fixed_pitch>81 )
	{cmd->forwardmove = -newforward;}
	else 
	{cmd->forwardmove = newforward;}

	cmd->sidemove = newright;
	cmd->upmove = newup;
}

void StrafeBot::StrafeHack(float frametime, struct usercmd_s *cmd,float visible,float crazy)
{
	if(Strafe && !(g_Local.Flags & FL_ONGROUND) && (g_Local.MoveType!=5) && !(cmd->buttons&IN_ATTACK) && !(IsKnife(g_Local.WpnID) && (cmd->buttons & IN_ATTACK2)) && !(cmd->buttons&IN_USE))
	{
		float dir=0;

		int dir_value = cvar.kzstrafe_dir;
		
		if(dir_value == 1)dir = 0;
		else if(dir_value == 2)	dir = 90;
		else if(dir_value == 3)	dir = 180;
		else if(dir_value == 4)	dir = -90;

		if(g_Local.fSpeed < 15){	
			cmd->forwardmove=400;
			cmd->sidemove=0;}

		float va_real[3]={0,0,0};
		g_Engine.GetViewAngles(va_real);va_real[1]+=dir;
		float vspeed[3]={g_Local.VVelocity.x/g_Local.FVSpeed,g_Local.VVelocity.y/g_Local.FVSpeed,0.0f};
		float va_speed=YawForVec(vspeed);
		
		float adif=va_speed-va_real[1];
		while(adif<-180)adif+=360;
		while(adif>180)adif-=360;
		cmd->sidemove=(cvar.kzstrafe_sidemove)*(adif>0?1:-1);
		cmd->forwardmove=0;
		bool onlysidemove=(abs(adif)>=atan(cvar.kzstrafe_angle/g_Local.fSpeed)/M_PI*180);
		int aaddtova=0;
	
		if(visible == 0) RotateInvisible(-(adif),0,cmd);
		else cmd->viewangles[1] -= (-(adif)); 

		float fs=0;
		if(!onlysidemove)
		{
			static float lv=0;
			Vector fw=g_Local.Forward;fw[2]=0;fw=fw.Normalize();
			float vel=POW(fw[0]*g_Local.VVelocity[0])+POW(fw[1]*g_Local.VVelocity[1]);

			fs=lv;
			lv=sqrt(cvar.kzstrafe_speed * 100000 / vel);
			static float lastang=0;
			float ca=abs(adif);		
			lastang=ca;
		}

		if(visible == 0) cmd->forwardmove+=fs;
		else 
		{
			float ang = atan(fs/cmd->sidemove)/M_PI*180;
			cmd->viewangles.y+=ang;
		}

		if(crazy != 0)
		{
			static int _crazy = 1;
			_crazy *= (-1);
			cmd->viewangles.x = 89 * _crazy;
		}

		float sdmw=cmd->sidemove;
		float fdmw=cmd->forwardmove;
		if(cvar.kzstrafe_dir == 1)
		{
			cmd->forwardmove = fdmw;
			cmd->sidemove = sdmw;
		}
		else if(cvar.kzstrafe_dir == 2)
		{
			cmd->forwardmove = -sdmw;
			cmd->sidemove = fdmw;
		}
		else if(cvar.kzstrafe_dir == 3)
		{
			cmd->forwardmove = -fdmw;
			cmd->sidemove = -sdmw;
		}
		else if(cvar.kzstrafe_dir == 4)
		{
			cmd->forwardmove = sdmw;
			cmd->sidemove = -fdmw;
		}
	}
}

void StrafeBot::AutoDir(struct usercmd_s *cmd)	
{
	if(cmd->buttons&IN_MOVERIGHT){
		cvar.kzstrafe_dir = 2;}
	else if(cmd->buttons&IN_BACK){
		cvar.kzstrafe_dir = 3;}
	else if(cmd->buttons&IN_MOVELEFT){
		cvar.kzstrafe_dir = 4;}
	else
		cvar.kzstrafe_dir = 1;
}

void StrafeBot::FastRun(struct usercmd_s *cmd)
{
	if (Fastrun && g_Local.fSpeed && g_Local.flFallSpeed == 0 && !Gstrafe && g_Local.Flags&FL_ONGROUND)
	{
		static bool _FastRun = false;
		if ((cmd->buttons&IN_FORWARD && cmd->buttons&IN_MOVELEFT) || (cmd->buttons&IN_BACK && cmd->buttons&IN_MOVERIGHT)) {
			if (_FastRun)	{ _FastRun = false; cmd->sidemove -= 89.6f; cmd->forwardmove -= 89.6f; }
			else			{ _FastRun = true;  cmd->sidemove += 89.6f; cmd->forwardmove += 89.6f; }
		} else if ((cmd->buttons&IN_FORWARD && cmd->buttons&IN_MOVERIGHT) || (cmd->buttons&IN_BACK && cmd->buttons&IN_MOVELEFT)) {
			if (_FastRun)	{ _FastRun = false; cmd->sidemove -= 89.6f; cmd->forwardmove += 89.6f; }
			else			{ _FastRun = true;  cmd->sidemove += 89.6f; cmd->forwardmove -= 89.6f; }
		} else if(cmd->buttons&IN_FORWARD || cmd->buttons&IN_BACK) {
			if (_FastRun)	{ _FastRun = false; cmd->sidemove -= 126.6f; }
			else			{ _FastRun = true;  cmd->sidemove += 126.6f; }
		} else if (cmd->buttons&IN_MOVELEFT || cmd->buttons&IN_MOVERIGHT) {
			if (_FastRun)	{ _FastRun = false; cmd->forwardmove -= 126.6f; }
			else			{ _FastRun = true;  cmd->forwardmove += 126.6f; }
		}
	}
}

void StrafeBot::GroundStrafe(struct usercmd_s *cmd)
{
	if(Gstrafe && !Jumpbug)
	{
		static int gs_state = 0;
		if(cvar.kzgstrafe_standup && g_Local.FlHeight<cvar.kzgstrafe_standup)
		{
			if(cvar.kzgstrafe_nsd && (g_Local.flGroundAngle<5) && (g_Local.FlHeight<=0.000001f || g_Local.Flags&FL_ONGROUND))
			{
				AdjustSpeed(0.0001);
			}
			cmd->buttons |=IN_DUCK;
		}
		if(gs_state == 0 && g_Local.Flags&FL_ONGROUND)
		{
			if(cvar.kzgstrafe_nsd && (g_Local.flGroundAngle<5) && (g_Local.FlHeight<=0.000001f || g_Local.Flags&FL_ONGROUND))
			{
				AdjustSpeed(0.0001);
			}	
			cmd->buttons |=IN_DUCK;
			gs_state = 1;
		}
		else if(gs_state == 1)
		{
			if(cvar.kzgstrafe_nsd && (g_Local.flGroundAngle<5) && (g_Local.FlHeight<=0.000001f || g_Local.Flags&FL_ONGROUND))
			{
				AdjustSpeed(0.0001);
			}	
			if(cvar.kzgstrafe_bhop && g_Local.UseHull==0)
			{
				cmd->buttons|=IN_JUMP;
			}
			cmd->buttons &= ~IN_DUCK;
			gs_state = 0;
		}
	}
}

void SlowDownSleep(struct usercmd_s *cmd, float fNeedSpeed)
{
	float redspeed = g_Local.fSpeed - fNeedSpeed;
	float degspeed = atan2(g_Local.VVelocity[1], g_Local.VVelocity[0]);

	if(degspeed < 0)
		degspeed += (M_PI * 2.0f);
	if(redspeed < 30.0f)
		redspeed = (300.0f / g_Local.AirAccelerate);

	cmd->forwardmove = -redspeed * cos(cmd->viewangles.y * M_PI / 180.0f - degspeed);
	cmd->sidemove = -redspeed * sin(cmd->viewangles.y * M_PI / 180.0f - degspeed);
}

void StrafeBot::Bhop(float frametime, struct usercmd_s *cmd)
{
	static bool bhop_standup_state = false;
	static bool lastFramePressedJump=false;
	static bool JumpInNextFrame=false;
	static int inAirBhopCnt=0;
	bool isJumped=false;

	if ( g_Local.FlHeight <= 20 && bhop_standup_state == true && StrafeBhop !=0 && cvar.kzbhop_autoduck )
	{
		bhop_standup_state = false;
		g_Engine.pfnClientCmd("-duck");
	}
	
	if ( bhop_standup_state == true && (g_Local.MoveType == 5))
	{
		g_Engine.pfnClientCmd("-duck");
	}

	if(JumpInNextFrame)
	{
		JumpInNextFrame=false;
		cmd->buttons|=IN_JUMP;
		goto bhopfuncend;
	}

	if(StrafeBhop !=0 && !Gstrafe)
	{
		if ( cvar.kzbhop_autoduck )
		{
			if ( g_Local.FlHeight >= 30 && g_Local.FlHeight <= 64 && bhop_standup_state == false && g_Local.MoveType != 5)
			{
				g_Engine.pfnClientCmd("+duck");
				bhop_standup_state = true;
				g_Local.flDuckTimeMax = g_Engine.GetClientTime();
				balance_bh = true;
			}
		}

		cmd->buttons &= ~IN_JUMP;
		if( ((!lastFramePressedJump)|| g_Local.Flags&FL_ONGROUND || g_Local.Waterlevel >= 2 || g_Local.MoveType==5 || g_Local.FlHeight<=2)/* && !jumpbug*/)
		{
			if(true)
			{
				if(cvar.kzbhop_nsd)
				{
					if((int)g_Local.fSpeed <= 299 && !Strafe)
					{
						AdjustSpeed(0.0001);
					}
				}
				static int bhop_jump_number=0;
				bhop_jump_number++;
				if(bhop_jump_number>=2)
				{
					bhop_jump_number=0;
					JumpInNextFrame=true; 
					goto bhopfuncend;
				}
			}

			if(cvar.kzbhop_cnt_rand_min > cvar.kzbhop_cnt_rand_max)
			{
				cvar.kzbhop_cnt_rand_min = cvar.kzbhop_cnt_rand_max;
			}

			if(cvar.kzbhop_cnt_rand_min <= 1)
			{
				cvar.kzbhop_cnt_rand_min = 1;
			}

			if(cvar.kzbhop_cnt_rand_min > 20)
			{
				cvar.kzbhop_cnt_rand_min = 20;
			}

			if(cvar.kzbhop_cnt_rand_max > 20)
			{
				cvar.kzbhop_cnt_rand_max = 20;
			}

			if(cvar.kzbhop_cnt_rand_max <= 1)
			{
				cvar.kzbhop_cnt_rand_max = 1;
			}

			if(cvar.kzbhop_cnt_rand)
				inAirBhopCnt = (int)g_Engine.pfnRandomFloat((int)cvar.kzbhop_cnt_rand_min,(int)cvar.kzbhop_cnt_rand_max);
			else
				inAirBhopCnt = cvar.kzbhop_cnt;

			isJumped=true;
			cmd->buttons |= IN_JUMP;
		} 
	}
	if(!isJumped)
	{
		if(inAirBhopCnt>0)
		{
			if(inAirBhopCnt%2==0) 
			{
				cmd->buttons |= IN_JUMP;
			}
			else cmd->buttons &= ~IN_JUMP;
			inAirBhopCnt--;
		}
	}

    bhopfuncend:

	lastFramePressedJump = cmd->buttons&IN_JUMP;
}

double _my_abs(double n) 
{
	if (n >= 0)return n; //if positive, return without ant change
	else return 0 - n; //if negative, return a positive version
}

void StrafeBot::JumpBug(float frametime, struct usercmd_s *cmd)
{
	static int state=0;

	bool autojb=false;
	if(cvar.kzjumpbugauto !=0 && g_Local.flFallSpeed>=PLAYER_MAX_SAFE_FALL_SPEED)
		if(g_Local.FlHeight-(g_Local.flFallSpeed*frametime/cvar.wavspeed *15)<=0)
			autojb=true;

	if((Jumpbug||autojb)&& g_Local.flFallSpeed>0)
	{
		bool curveang=false;
		float fpheight=0;
		if(g_Local.flGroundAngle>1)
		{
			curveang=true;
			Vector vTemp = g_Local.Origin;
			vTemp[2] -= 8192;
			pmtrace_t *trace =  g_pEngine->PM_TraceLine(g_Local.Origin, vTemp, 1, 2, -1);
			fpheight=abs(g_Local.Origin.z-trace->endpos.z-(g_Local.UseHull==1?18.0f:36.0f));
		}
		else fpheight=g_Local.FlHeight;
		

		static float last_h=0.0f;
		float cur_frame_zdist=abs((g_Local.flFallSpeed+(800*frametime))*frametime);
		cmd->buttons|=IN_DUCK;
		cmd->buttons&=~IN_JUMP;
		switch(state)
		{
		case 1:
			cmd->buttons&=~IN_DUCK;
			cmd->buttons|=IN_JUMP;
			state=2;
			break;
		case 2:
			state=0;
			break;
		default:
			if(_my_abs(fpheight-cur_frame_zdist*1.5)<=(20.0)&&cur_frame_zdist>0.0f)
			{
				float needspd=_my_abs(fpheight-(19.0));
				float scale=abs(needspd/cur_frame_zdist);
				AdjustSpeed(scale);
				state=1;
			}
			break;
		}
		last_h=fpheight;
	}
	else state=0;
}

void StrafeBot::StrafeHelper(float frametime, struct usercmd_s *cmd)
{
	if(g_Local.fSpeed > 0)
	{
		static int lastdir=0;
		static int strafe_counter=0;
		static int strafe_num=0;
		if(g_Local.Flags&FL_ONGROUND||g_Local.MoveType==5||!(g_Local.Waterlevel < 2)||(g_Local.flGroundAngle>45.9&&g_Local.FlHeight<50))
		{
			strafe_counter=0;
			lastdir=0;
			if(cvar.kzstrafe_helper_boost)
					Strafe=false;
			rand_strafe = true;
		}
		else
		{
			if(cvar.kzstrafe_helper_boost)
					Strafe=true;
	
			cmd->buttons&=~IN_BACK;
			cmd->buttons&=~IN_FORWARD;

			if(!(cmd->buttons&IN_MOVERIGHT) && !cvar.kzstrafe_helper_add_strafe)
				cmd->buttons&=~IN_MOVERIGHT;

			if(!(cmd->buttons&IN_MOVELEFT) && !cvar.kzstrafe_helper_add_strafe)
				cmd->buttons&=~IN_MOVELEFT;			
			
			if(cvar.kzstrafe_helper_max_strafe_rand && cvar.kzstrafe_helper_add_strafe)
			{
				if(cvar.kzstrafe_helper_max_strafe_min > cvar.kzstrafe_helper_max_strafe_max)
				{
					cvar.kzstrafe_helper_max_strafe_min = cvar.kzstrafe_helper_max_strafe_max;
				}
				if(cvar.kzstrafe_helper_max_strafe_min <= 1 )
				{
					cvar.kzstrafe_helper_max_strafe_min = 1;
				}
				if(rand_strafe == true )
				{
					cvar.kzstrafe_helper_max_strafe = g_Engine.pfnRandomFloat(cvar.kzstrafe_helper_max_strafe_min,cvar.kzstrafe_helper_max_strafe_max);
					rand_strafe = false;
				}
			}

			static int nobuttons=0;
			if(lastdir!=-1&&g_Local.FAngleSpeed<0)
			{
				lastdir=-1;
				strafe_counter++;
				strafe_num=0;
				nobuttons=3;
			}
			if(lastdir!=1&&g_Local.FAngleSpeed>0)
			{
				lastdir=1;
				strafe_counter++;
				strafe_num=0;
				nobuttons=3;
			}
			strafe_num++;

			if(strafe_counter<=(cvar.kzstrafe_helper_max_strafe) && cvar.kzstrafe_helper_add_strafe)
			{
				if(cvar.kzstrafe_dir == 1)
				{
					if(g_Local.FAngleSpeed<0){cmd->buttons|=IN_MOVELEFT;cmd->buttons&=~IN_MOVERIGHT;}//mouse left - left
					if(g_Local.FAngleSpeed>0){cmd->buttons|=IN_MOVERIGHT;cmd->buttons&=~IN_MOVELEFT;}//mouse right - right
				}
				else if(cvar.kzstrafe_dir == 2)
				{
					if(g_Local.FAngleSpeed<0){cmd->buttons|=IN_FORWARD;cmd->buttons&=~IN_BACK;}//mouse left - forward
					if(g_Local.FAngleSpeed>0){cmd->buttons|=IN_BACK;cmd->buttons&=~IN_FORWARD;}//mouse right - back
					
				}
				else if(cvar.kzstrafe_dir == 3)
				{
					if(g_Local.FAngleSpeed<0){cmd->buttons|=IN_MOVERIGHT;cmd->buttons&=~IN_MOVELEFT;}//mouse left - right
					if(g_Local.FAngleSpeed>0){cmd->buttons|=IN_MOVELEFT;cmd->buttons&=~IN_MOVERIGHT;}//mouse right - left
				}
				else if(cvar.kzstrafe_dir == 4)
				{
					if(g_Local.FAngleSpeed<0){cmd->buttons|=IN_BACK;cmd->buttons&=~IN_FORWARD;}//mouse left - back
					if(g_Local.FAngleSpeed>0){cmd->buttons|=IN_FORWARD;cmd->buttons&=~IN_BACK;}//mouse right - forward
				}
			}
			float sidespeed=0;
			float forwardspeed=0;

			if(cvar.kzstrafe_helper_move_rand && cvar.kzstrafe_helper_add_strafe)
			{
				if(cvar.kzstrafe_helper_move_rand_min > cvar.kzstrafe_helper_move_rand_max)
				{
					cvar.kzstrafe_helper_move_rand_min = cvar.kzstrafe_helper_move_rand_max;
				}

				if(cvar.kzstrafe_helper_move_rand_max > 400)
				{
					cvar.kzstrafe_helper_move_rand_max = 400;
				}

				if(cvar.kzstrafe_helper_move_rand_max <= 1)
				{
					cvar.kzstrafe_helper_move_rand_max = 1;
				}

				if(cvar.kzstrafe_helper_move_rand_min < 50)
				{
					cvar.kzstrafe_helper_move_rand_min = 50;
				}

				if(cvar.kzstrafe_helper_move_rand_min > 400)
				{
					cvar.kzstrafe_helper_move_rand_min = 400;
				}
				cvar.kzstrafe_helper_move = g_Engine.pfnRandomFloat(cvar.kzstrafe_helper_move_rand_min,cvar.kzstrafe_helper_move_rand_max);
			}

			double ms = g_Local.fSpeed/g_Local.MaxSpeed;
			sidespeed += cvar.kzstrafe_helper_move*(g_Local.FAngleSpeed<0?-1:1);
			forwardspeed = cvar.kzstrafe_helper_main/ms;

			if(g_Local.FAngleSpeed>0.0f||g_Local.FAngleSpeed<0.0f)
			{
				if(cvar.kzstrafe_dir == 1)
				{
					cmd->forwardmove=forwardspeed;
					cmd->sidemove=sidespeed;
				}
				else if(cvar.kzstrafe_dir == 2)
				{
					cmd->forwardmove=-forwardspeed;
					cmd->sidemove=-sidespeed;
				}
				else if(cvar.kzstrafe_dir == 3)
				{
					cmd->forwardmove=-forwardspeed;
					cmd->sidemove=-sidespeed;
				}
				else if(cvar.kzstrafe_dir == 4)
				{
					cmd->forwardmove=sidespeed;
					cmd->sidemove=-forwardspeed;
				}
			}
		}
	}
}