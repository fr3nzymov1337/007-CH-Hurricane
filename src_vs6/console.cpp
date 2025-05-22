#include "client.h"

Console gConsole;

void TrapezoidalConsole(int con_x, int con_y, int con_w, int con_h);

void DrawConsole()
{
	if( !gConsole.active ) return;

	int con_x = (g_Screen.iWidth/2)-200;
	int con_y = (g_Screen.iHeight/2)-150;
	int con_w = 400;
	int con_h = 300;

	{
		TrapezoidalConsole(con_x,con_y,con_w,con_h);
	}

	int x = con_x+3;
	int y = con_y+con_h-14;

	if(gConsole.blinkTimer.expired())
	{
		gConsole.blink = !gConsole.blink;
		if(gConsole.blink) { gConsole.blinkTimer.countdown(0.2); }
		else      { gConsole.blinkTimer.countdown(0.2); }
	}

	if(gConsole.blink)
	{
		int length, height, ch_length;
		char  save;
		save = gConsole.cursorpos[0]; gConsole.cursorpos[0]=0;
		g_Engine.pfnDrawConsoleStringLen( gConsole.editbuf, &length, &height );
		gConsole.cursorpos[0]=save;
		save = gConsole.cursorpos[1]; gConsole.cursorpos[1]=0;
		g_Engine.pfnDrawConsoleStringLen( gConsole.cursorpos, &ch_length, &height );
		gConsole.cursorpos[1]=save;
		if(!*gConsole.cursorpos) ch_length=5;
        FillRGBA(x+length,y,ch_length,height-2,cvar.color_red*255.0f,cvar.color_green*255.0f,cvar.color_blue*255.0f,255);
	}
	g_Engine.pfnDrawSetTextColor(255/255.0f, 255/255.0f, 255/255.0f);
	g_Engine.pfnDrawConsoleString(x,y-1,gConsole.editbuf);
	gConsole.lines.reset();
	for(;;)
	{
		y-=14;
		if(y<con_y) break;
		string& curLine = gConsole.lines.read(); 
		gConsole.lines.prev();
		gConsole.DrawConsoleLine(curLine,x,y);
	}
	
}

void Console::echo(const char* fmt, ... )
{
	va_list va_alist;
	char buf[384];
	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);
	lines.add( buf );
}

void Console::say(const char* text, const char* name, int team )
{
	if(team==1) { echo("&r%s :", name); echo(" %s",text); }
	else        { echo("&b%s :", name); echo(" %s",text); }
}

void Console::DrawConsoleLine( const string& str, int x, int y )
{
	const char* line = str.c_str();
	char  buf[256];
	char* bufpos;

	for(;;)
	{
		bufpos=buf;
		for(;;) { *bufpos=*line; if(!*line||*line=='&')break;  ++line; ++bufpos; };
		bufpos[0]=0;bufpos[1]=0;
		int length, height;
		g_Engine.pfnDrawConsoleStringLen( buf, &length, &height ); 
		g_Engine.pfnDrawSetTextColor(colorTags[curColorTag].onebased_r, colorTags[curColorTag].onebased_g, colorTags[curColorTag].onebased_b);
		g_Engine.pfnDrawConsoleString(x,y,buf);
		x+=length;
		if(*line=='&')
		{
			unsigned char ch = *++line - 'a';
			if(ch<26) curColorTag=ch;
			else         break;
			if(!*++line) break;
		}
		else 
		{
			break;
		}
	}
	curColorTag=0;
}

void Console::key(int ch)
{
	char* pos;
	{
		switch(ch)
		{
			case -1: // backspace
				if(cursorpos==editline) return;
				pos = --cursorpos;
				while(pos[0]) { pos[0]=pos[1]; ++pos; }
				return;
				
			case -2: // uparrow
				if(hist_direction!=DIR_BACK) { history.prev(); history.prev(); hist_direction=DIR_BACK;}
				strcpy(editline, history.read().c_str());
				cursorpos = editline + strlen(editline);
				history.prev();
				return;

			case -3: // downarrow
				if(hist_direction==DIR_BACK) { history.next(); history.next(); hist_direction=DIR_FORWARD;}
				strcpy(editline, history.read().c_str());
				cursorpos = editline + strlen(editline);
				history.next();
				return;
			
			case -4: // leftarrow
				if(cursorpos!=editline) --cursorpos;
				return;

			case -5: // leftarrow
				if(cursorpos!=(editline+strlen(editline))) ++cursorpos;
				return;

			case '\n':
				if( !strcmp(editline,"===") )
				{
					if(mode==MODE_EXECUTE) { mode=MODE_CHAT;    /*echo("&b*** &aCONSOLE: &wCHAT MODE &b***");*/ }
					else                   { mode=MODE_EXECUTE; /*echo("&b*** &aCONSOLE: &wEXEC MODE &b***");*/ }
				}
				else if(mode==MODE_EXECUTE)
				{
					echo    ( "&x%s",editbuf );
					if(editline[0])
					{
						cmd.exec( editline );
						history.add(editline);
						history.reset();
					}
				} 
				else if(mode==MODE_CHAT)
				{
					char* text = editline; while(*text==' ')++text;
					char buf[256];sprintf(buf,"say \"%s\"",text);
					g_Engine.pfnClientCmd(buf);
				}
				editline[0]=0;
				cursorpos = editline;
				return;
			default:
				if(strlen(editbuf)>(EDIT_MAX-4)) return;
				if(!cursorpos[0]) { cursorpos[0]=ch; ++cursorpos; cursorpos[0]=0; return; }
				pos = editbuf+strlen(editbuf)+1; 
				while(pos>cursorpos) { pos[0]=pos[-1]; --pos; }
				*cursorpos = ch;
				++cursorpos;
				return;
		}
	}
}

void Con_Echo(const char* fmt, ... )
{
	va_list va_alist;
	char buf[256];
	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);
	gConsole.echo("%s",buf);
}

void CB_KeyManager_ConType(char ch)
{
	gConsole.key(ch);
}

void OGC_ConsoleScrollUp(const char* message)
{
	gConsole.echo(message);
}

void OGCmessage(const char* fmt, ... )
{
	va_list va_alist;
	char buf[256];
	
	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);
	
	OGC_ConsoleScrollUp(buf); // echo + scroll up:
}