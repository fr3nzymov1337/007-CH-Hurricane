
#include "client.h"

IdHook idhook;
typedef IdHook::Player Player;
typedef IdHook::PlayerEntry PlayerEntry;

static IdHook::Player* curPlayer = idhook.basePlayer;
bool player_active = false;
int selection=0, seekselection=0;

void TrapezoidalConsole(int con_x, int con_y, int con_w, int con_h);

void func_relistplayer() //ˢ�������б�
{
	idhook.RelistPlayer();
}

void func_clearallplayer() //��������רɱ/ר��ɱ����
{
	idhook.ClearPlayer();
}

void func_addplayer() //����רɱ/ר��ɱ����
{
	if(!cmd.argS(1).empty())
		idhook.AddPlayer(cmd.argI(1));
}

void player_describe_current()
{
	if(!player_active||!curPlayer) return;
	IdHook::PlayerEntry& entry = curPlayer->items[curPlayer->selection];
}

void func_player_toggle() //����רɱ/ר��ɱ�����б��˵�
{
	if(!player_active)
	{
		func_player_select();
	}
	else
	{
		selection=curPlayer->selection;
		seekselection=curPlayer->seekselection;
		player_active=false;
		cvar.Save();
	}
}

void func_player_select() //ѡ��
{
	if(!player_active)
	{
		idhook.init();
	}

	curPlayer = idhook.basePlayer;
	curPlayer->boundSelection();
	player_describe_current();

	if(player_active)
	{
		IdHook::PlayerEntry& entry = curPlayer->items[curPlayer->selection];
		if(entry.player)
		{
			curPlayer = entry.player;
			curPlayer->boundSelection();
			player_describe_current();
		}
		else
		{
			int i=curPlayer->selection;
			int j=curPlayer->seekselection;
			cmd.exec(const_cast<char*>(entry.content));
			player_active=false;
			func_player_select();
			curPlayer->selection=i;
			curPlayer->seekselection=j;
			curPlayer->boundSelection1();
			player_describe_current();
		}
	}
	else
	{
		curPlayer->selection=selection;
		curPlayer->seekselection=seekselection;
		curPlayer->boundSelection1();
		player_describe_current();
	}
	player_active=true;
}

void func_player_back() //���ϼ�
{
	if(!player_active || !curPlayer) return;
	curPlayer = curPlayer->parent;
	if(!curPlayer) 
	{ 
		curPlayer = idhook.basePlayer; 
		player_active=false; 
	}
	selection=curPlayer->selection;
	seekselection=curPlayer->seekselection;
	curPlayer->boundSelection();
	player_describe_current();
}

void func_player_up() //��������һ��
{
	if(!player_active || !curPlayer) return;
	--curPlayer->selection;
	curPlayer->boundSelection();
	player_describe_current();
}

void func_player_down() //��������һ��
{
	if(!player_active || !curPlayer) return;
	++curPlayer->selection;
	curPlayer->boundSelection();
	player_describe_current();
}

void ListIdHook() //�Զ�ˢ�²˵�
{
	if(player_active)
	{
		int i=curPlayer->selection;
		int j=curPlayer->seekselection;
		idhook.RelistPlayer();
		player_active=false;
		func_player_select();
		curPlayer->selection=i;
		curPlayer->seekselection=j;
		curPlayer->boundSelection1();
		player_describe_current();
	}
}

bool gPlayerActive()
{ 
	return player_active; 
}

void drawPlayer()
{
	if (!player_active)
		return;

	if (!curPlayer)
		curPlayer = idhook.basePlayer;

	vector<IdHook::PlayerEntry>& items = curPlayer->items;

	int player_y = 50;

	int x = g_Screen.iWidth/2 - g_Screen.iWidth/2.3/2;
	int y = player_y;
	int w = 40;

	enum{ TEXT_H = 16 };
	int title_h = TEXT_H-1; 
	int frame_y = player_y - title_h;
	int frame_h = items.size()*TEXT_H + title_h;

	for(int i=0;i<items.size();i++)
	{
		IdHook::PlayerEntry& item = items[i];
		va_list va_alist;
		char buf[256];
		va_start( va_alist, item.name );
		_vsnprintf( buf, sizeof( buf ), item.name, va_alist );
		va_end( va_alist );
		if(iStringLen("%s",buf) > w)
			w = iStringLen("%s",buf);
	}

	TrapezoidalConsole(x,/*frame_*/y,w,frame_h/*-11*/);

	int h1 = curPlayer->selection*TEXT_H;
	if (h1)
		y+=h1;

	tintArea( x+1,y+7,w-3,16,255,255,255,128);
	BorderMenu( x+1,y+7,w-3,16,0,0,0,255);
	
	y=player_y;
	x+=4;
	for(i=0;i<items.size();i++)
	{
		ColorEntry* clr;
		IdHook::PlayerEntry& item = items[i];
		
		if(strstr(item.name," T"))
			clr = colorList.get(0); // "T_alive"
		else if(strstr(item.name,"CT"))
			clr = colorList.get(1); // "CT_alive"
		else
			clr = colorList.get(4);

		g_Engine.pfnDrawSetTextColor(clr->onebased_r,clr->onebased_g,clr->onebased_b);
		g_Engine.pfnDrawConsoleString(x,y+7,const_cast<char*>(item.name));
		y+=TEXT_H;
	}
}

void func_first_kill_mode()
{
	if(cvar.aim_id_mode==0.0)
		cvar.aim_id_mode=1.0; //��ɱ��Ұ�е�רɱ����
	else if(cvar.aim_id_mode==1.0)
		cvar.aim_id_mode=2.0; //ֻɱרɱ������������ɱ
	else
		cvar.aim_id_mode=0.0; //��ͨģʽ
}

void listPlayer(Player* pPlayer)
{
	PlayerEntry newEntry;

	sprintf(newEntry.content,"relistplayer");

	if(cvar.aim_id_mode == 0)
		sprintf(newEntry.name," + Attack All");
	if(cvar.aim_id_mode == 1)
		sprintf(newEntry.name," + Attack On First");
	if(cvar.aim_id_mode == 2)
		sprintf(newEntry.name," + Attack Only On");
	sprintf(newEntry.content,"first_kill_mode");
	newEntry.player = 0;
	pPlayer->items.push_back(newEntry);

	sprintf(newEntry.name," + Clear ID");
	sprintf(newEntry.content,"clearallplayer");
	newEntry.player = 0;
	pPlayer->items.push_back(newEntry);

	char *teamname;
	for(int ax=0;ax<33;ax++)
	{
		hud_player_info_t pinfo;
		g_pEngine->pfnGetPlayerInfo(ax, &pinfo);
		if(!pinfo.name ) { pinfo.name  = "\\missing-name\\"; }
		if(!pinfo.model) { pinfo.model = "unknown model";    } 
		if(strcmp(pinfo.name,"\\missing-name\\") && pinfo.name[0]!='\0' && g_Player[ax].Team>0 && g_Player[ax].Team==1)
		{
			teamname=" T";//T
			if(idhook.FirstKillPlayer[ax]==0)
				sprintf(newEntry.name,"            %s      %s   ",teamname,pinfo.name);
			else if(idhook.FirstKillPlayer[ax]==1)
				sprintf(newEntry.name," On>        %s      %s   ",teamname,pinfo.name);
			else
				sprintf(newEntry.name," Off>       %s      %s   ",teamname,pinfo.name);
			sprintf(newEntry.content,"addplayer %d",ax);
			newEntry.player = 0;
			pPlayer->items.push_back(newEntry);
		}
	}
	for(int x=0;x<33;x++)
	{
		hud_player_info_t pinfo;
		g_pEngine->pfnGetPlayerInfo(x, &pinfo);
		if(!pinfo.name ) { pinfo.name  = "\\missing-name\\"; }
		if(!pinfo.model) { pinfo.model = "unknown model";    } 
		if(strcmp(pinfo.name,"\\missing-name\\") && pinfo.name[0]!='\0' && g_Player[x].Team>0 && g_Player[x].Team==2)
		{
			teamname="CT";//CT
			if(idhook.FirstKillPlayer[x]==0)
				sprintf(newEntry.name,"            %s      %s   ",teamname,pinfo.name);
			else if(idhook.FirstKillPlayer[x]==1)
				sprintf(newEntry.name," On>        %s      %s   ",teamname,pinfo.name);
			else
				sprintf(newEntry.name," Off>       %s      %s   ",teamname,pinfo.name);
			sprintf(newEntry.content,"addplayer %d",x);
			newEntry.player = 0;
			pPlayer->items.push_back(newEntry);
		}
	}
}

void IdHook::init()
{
	if(basePlayer) delete basePlayer;
	basePlayer = new Player;
	basePlayer->name = " ";
	listPlayer(basePlayer);
}

void IdHook::AddPlayer(int ax)
{
	if(FirstKillPlayer[ax]==0)
		FirstKillPlayer[ax]=1;
	else if(FirstKillPlayer[ax]==1)
		FirstKillPlayer[ax]=2;
	else
		FirstKillPlayer[ax]=0;
}

void IdHook::ClearPlayer()
{
	for(int i=0;i<33;i++)
	{
		FirstKillPlayer[i]=0;
	}
}

void IdHook::RelistPlayer()
{
	for(int i=0;i<33;i++)
	{
		if(i>=33)
			return;
		if(g_Player[i].entinfo.name[0]=='\0')
			FirstKillPlayer[i]=0;
	}
}

void DrawIdHook()
{
	if(player_active)
		drawPlayer();
}