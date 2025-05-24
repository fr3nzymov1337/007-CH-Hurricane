#ifndef _CVARS_
#define _CVARS_


class CVARlist
{
public:
	void init();
	void Load();
	void Save();

    float aim_activate;
    float aim_team;
	float aim_mode;
	float aim_shield;
    float aim_silent;
    float aim_psilent;
    float aim_auto;
    float aim_auto_fov;
    float aim_knf_attack;
    float aim_auto_punch;
    float aim_color;
    float aim_drawaim;
    float aim_id_mode;
    float aim_x_duck;
    float aim_y_duck;
    float aim_z_duck;
    float aim_x_stand;
    float aim_y_stand;
    float aim_z_stand;
    float aim_x_jump;
    float aim_y_jump;
    float aim_z_jump;
    float aim_x_duckmove;
    float aim_y_duckmove;
    float aim_z_duckmove;
    float aim_x_running;
    float aim_y_running;
    float aim_z_running;
    float aim_x_walking;
    float aim_y_walking;
    float aim_z_walking;

    float knf_distattack;
    float knf_distattack2;
    float knf_attack;
    float knf_activate;
    float knf_fov;

    float trigger_activate;
    float trigger_fov;

    float norecoilvalue;
    float norecoil;
    float nospread;
    float nospreaddraw;
    float norecoildraw;
    float norecoil_visual;
    float norecoil_visible;
    float nospread_visible;

    float burst_ftime;
    float burst_dtime;
    float burst_sdelay;
    float burst_rdelay;

    float wavspeed;
    float wavaspeed;
    float wavrspeed;
	
    float misc_auto_pistol;
    float misc_rem_flash;
    float misc_name;
    float misc_weapon;
    float misc_esp_back;
    float misc_tint;
    float misc_esp_team;
    float misc_box;
    float misc_box_width;
    float misc_box_height;
    float misc_box_size_up;
    float misc_box_size_down;
    float misc_sequence;
    float misc_hud_info;
    float misc_soundesp;
    float misc_soundtime;
    float misc_snapshot_time;
    float misc_radar;
    float misc_radars;
	float misc_radarpoints;
    float misc_wall;
    float misc_chasecam;
	float misc_health;
	float misc_norefresh;
	float misc_spin;
	float misc_spinspeed;
	float misc_hud;
	float misc_nohud;
	float misc_nosky;
	float misc_lambert;
	float misc_quake;
	float misc_wirewall;
	float misc_nightmode;
	float misc_sayid;
	float misc_aa_pitch;
	float misc_aa_yaw;

    float kzstrafehack;
    float kzstrafeautodir;
    float kzgroundstrafe;
    float kzbhop;
    float kzjumpbug;
    float kzfastrun;
    float kzstrafe_helper;
    float kzslowdown;
    float kzshow_kz;
    float kzstrafe_dir;
    float kzstrafe_crazy;
    float kzstrafe_invisible;
    float kzstrafe_speed;
    float kzstrafe_sidemove;
    float kzstrafe_angle;
    float kzfastrun_nsd;
    float kzgstrafe_standup;
    float kzgstrafe_bhop;
    float kzgstrafe_nsd;
    float kzbhop_nsd;
    float kzbhop_autoduck;
    float kzbhop_cnt_rand_min;
    float kzbhop_cnt_rand_max;
    float kzbhop_cnt_rand;
    float kzbhop_cnt;
    float kzjumpbugauto;
    float kzjumpbugslow;
    float kzjumpbugslowspeed;
    float kzstrafe_helper_boost;
    float kzstrafe_helper_add_strafe;
    float kzstrafe_helper_max_strafe_rand;
    float kzstrafe_helper_max_strafe_min;
    float kzstrafe_helper_max_strafe_max;
    float kzstrafe_helper_max_strafe;
    float kzstrafe_helper_main;
    float kzstrafe_helper_move_rand;
    float kzstrafe_helper_move_rand_min;
    float kzstrafe_helper_move_rand_max;
    float kzstrafe_helper_move;
    float kzfps_helper;

	float auto_direction;	
	float auto_jump;
	float rush;
	float way_on;
	float way_enable;
	float way_draw;
	float way_mode;
	float record_dist;
	float way_fov;
	float way_maxdist;
	float way_mindist;
	float way_radardist;
	float way_drawvisuals;

    float color_red;
    float color_green;
    float color_blue;
};

extern CVARlist cvar;

#endif