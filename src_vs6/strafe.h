#ifndef STRAFEBOT
#define STRAFEBOT

class StrafeBot
{
public:

	void StrafeHack(float frametime, struct usercmd_s *cmd,float visible,float crazy);
	void AutoDir(struct usercmd_s *cmd);
	void FastRun(struct usercmd_s *cmd);
	void GroundStrafe(struct usercmd_s *cmd);
	void Bhop(float frametime, struct usercmd_s *cmd);
	void JumpBug(float frametime, struct usercmd_s *cmd);
	void StrafeHelper(float frametime, struct usercmd_s *cmd);
};

extern void CLStrafeBot(float frametime, struct usercmd_s *cmd);
extern void HUDStrafeBot();
extern void StrafeCommands();

extern StrafeBot Strafebot;
#endif