
class IdHook
{
public:
	int FirstKillPlayer[33];
	void ClearPlayer();
	void RelistPlayer();
	void AddPlayer(int ax);
	struct Player;
	struct PlayerEntry{
		PlayerEntry() :player(0){}

		char name[256];
		char content[256];
		Player*  player;
	};

	struct Player {
		Player():parent(0),selection(0),seekselection(0){}
		void boundSelection()
		{ 
			// wrap around
			if(selection<0) { selection = items.size()-1; seekselection = 2;}
			else  if(selection >= (int)items.size()) { selection = 0; seekselection = 0;}
			if(selection==items.size()-1){seekselection = 2;}
			else if(selection==items.size()-2){seekselection = 1;}
			else {seekselection = 0;}
		}
		void boundSelection1()
		{ 
			// wrap around
			if(selection<0) { selection = 0; }
			else  if(selection >= (int)items.size()-3) 
			{ 
				if(seekselection == 0)
					selection = items.size()-3; 
				else if(seekselection == 1)
					selection = items.size()-2; 
				else if(seekselection == 2)
					selection = items.size()-1; 
			}
			else
			{
				if(seekselection == 1)
					selection = items.size()-2; 
				else if(seekselection == 2)
					selection = items.size()-1; 			}
		}

		Player*  parent;
		string name; 
		int    selection;
		int seekselection;
		vector<PlayerEntry> items;
	};

	IdHook():basePlayer(0){}
	void init();

	struct Player* basePlayer;
};

extern void func_relistplayer();
extern void func_clearallplayer();
extern void func_addplayer();
extern void func_player_toggle();
extern void func_player_select();
extern void func_player_back();
extern void func_player_up();
extern void func_player_down();
extern bool gPlayerActive();
extern void func_first_kill_mode();

extern void DrawIdHook();
extern void ListIdHook();

extern IdHook idhook;
