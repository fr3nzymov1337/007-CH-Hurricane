#ifndef CONSOLE_H
#define CONSOLE_H

template < typename T, int N >

class WrapHistory
{
protected:
	void increase(int& idx) { ++idx; if(idx==N) idx=0;     }
	void decrease(int& idx) { --idx; if(idx<0 ) idx=(N-1); }
public:
	WrapHistory() : readpos(0), writepos(0) {}
	void add(const T& entry) 
	{ 
		increase(writepos);
		entries[writepos]=entry; 
		reset();
	}
	void reset(){ readpos = writepos; }
	T&   read (){ return entries[readpos]; }
	void prev (){ decrease(readpos); if(readpos==writepos) increase(readpos); }
	void next (){ if(readpos!=writepos) increase(readpos);  }
protected:
	int readpos;
	int writepos;
	T entries[N];
};

class Console
{
public:
	int  active;
	int  mode;   enum{ MODE_EXECUTE=0, MODE_CHAT=1 };
	void echo(const char *fmt, ... );
	void say (const char* text, const char* name, int team );
	void key (int ch);
	int scrollbar;
	WrapHistory<string,1000> lines;
	WrapHistory<string,1000> history;
	enum { DIR_BACK, DIR_FORWARD } hist_direction;
	enum { EDIT_MAX=160 };
	char  editbuf[EDIT_MAX];
	char* const editline;
	char* cursorpos;
	StopTimer blinkTimer;
	bool  blink;
	ColorEntry colorTags[26];
	int        curColorTag;  // ==0 <=> 'a'
	void DrawConsoleLine( const string& str, int x, int y );
	int curHeight;
public:
Console() : editline(editbuf+1) , hist_direction(DIR_BACK)
	{ 
		editbuf[0] = '>';
		editbuf[1] =  0 ;
		editbuf[EDIT_MAX-1] = -1;
		cursorpos = editline;
		curColorTag = 0;
		colorTags['a'-'a'] = *colorList.get(5);
		colorTags['w'-'a'] = *colorList.get(4);
		mode = MODE_EXECUTE;
	}
};

extern void Con_Echo(const char* fmt, ... );
extern void CB_KeyManager_ConType(char ch);
extern void OGCmessage(const char* fmt, ... );
extern void DrawConsole();
extern Console gConsole;

#endif
