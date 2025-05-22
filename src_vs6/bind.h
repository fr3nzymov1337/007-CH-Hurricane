#ifndef BIND_H
#define BIND_H

class KeyBindManager
{
public:
	typedef void (*CallBack_ExecuteFunc)(const string& cmd);
	typedef void (*CallBack_ConTypeFunc)(char c);
	CallBack_ExecuteFunc CallBack_Execute;
	CallBack_ConTypeFunc CallBack_ConType;
	KeyBindManager() 
		: keyNames(256)
		, CallBack_Execute(NULL)
		, CallBack_ConType(NULL)
		, con_visible(NULL) 
		, messageMode(false)
	{}

	// hooking interface
	void notifyKeyEvent  ( int scancode,  bool down, bool repeat=false); 
	void notifyMouseEvent( char* keyname, bool down ); 
	bool keyBlocked    ( int scancode, bool down );
	bool keyBlocked    ( const char* name );
	int  remapScanCode ( int scancode ) { return keyRemapTable[scancode]; }

	// user interface
	void init( );
	void addBind   (char* key, char* value);
	void removeBind(char* key);
	void remapKey(char* from, char* to);
	bool messageMode;  // also a passthrough, resetted automatically
	int  (*con_visible)();

protected:
	void expandCommand(string& cmd,bool down);

	StringFinder myBind;
	vector<string> myBindExpand;
	vector<string> keyNames;

	int   keyRemapTable[256];
	
	typedef unsigned char byte;
	byte  shiftRemap[256];

};

extern void CB_KeyManager_Exec(const string& commandlist);
extern void HookCommands();
extern KeyBindManager keyBindManager;

#endif