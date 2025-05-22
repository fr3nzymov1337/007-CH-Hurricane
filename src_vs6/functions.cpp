#include "client.h"

void func_txt()
{
	string s;
	cmd.collectArguments(s);
	gSetHudMessage( s.c_str() );
}

void func_console_toggle()
{
	gConsole.active = !gConsole.active;
	cvar.Save();
}

void func_menu_toggle()
{
	MenuActive = !MenuActive;
	cvar.Save();
}

void func_bind()
{
	keyBindManager.addBind(cmd.argC(1),cmd.argC(2));
}

void func_unbind()
{
	keyBindManager.removeBind(cmd.argC(1));
}

void func_alias()
{
	const char*   name     = cmd.argC(1);
	string&       content  = cmd.argS(2);
	cmd.AddAlias(name,content);
}

void func_chatinput_toggle()
{
	if (gChatInput.Active)
		gChatInput.Active = 0;
	else if(!gChatInput.Active)
	{
		gChatInput.Init();
		gChatInput.mode = 1;
		gChatInput.Active = 1;		
	}
}

void func_chatinput2_toggle()
{
	if (gChatInput.Active)
		gChatInput.Active = 0;
	else if(!gChatInput.Active)
	{
		gChatInput.Init();
		gChatInput.mode = 2;
		gChatInput.Active = 1;		
	}
}

void func_exec()     
{
	ExecCfg( cmd.argC(1) ); 
}

void Init_Command_Interpreter()
{
	cmd.init();
	cvar.init();
	cvar.Load();

	#define REGISTER_COMMAND(name) cmd.AddCommand( #name, func_##name);
	REGISTER_COMMAND(alias)
	REGISTER_COMMAND(bind)
	REGISTER_COMMAND(unbind)
	REGISTER_COMMAND(txt)
	REGISTER_COMMAND(first_kill_mode)
	REGISTER_COMMAND(exec)
	REGISTER_COMMAND(addplayer)
	REGISTER_COMMAND(relistplayer)
	REGISTER_COMMAND(clearallplayer)
	REGISTER_COMMAND(player_toggle)
	REGISTER_COMMAND(console_toggle)
	REGISTER_COMMAND(menu_toggle)
	REGISTER_COMMAND(chatinput_toggle)
	REGISTER_COMMAND(chatinput2_toggle)
	REGISTER_COMMAND(way_load)
	REGISTER_COMMAND(way_clear)
	REGISTER_COMMAND(way_clear_all)
	REGISTER_COMMAND(way_record)
	REGISTER_COMMAND(way_record_stop)

	StrafeCommands();
}