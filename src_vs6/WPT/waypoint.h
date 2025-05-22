/////
///// CBOBY V2 [Revision by ETK]
/////
///// By [boy_scout][boyscout_etk@hotmail.com]
/////
///// [2011][www.etalking.com.ar]
/////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

class WAYPOINT
{
public:
	float relativeSpeed();
	bool pathFree(float* from,float* to);
	float getDist(float *point);

	void aimAngleCalc(float *point,float *viewangles);
	float getAngle(float *point);
	float MoveAnglesSub(float *point);
	void MoveTo(float *point, float* forwardmove, float* sidemove);
	void runDirection(int cur_direction,int cur_group,struct usercmd_s *usercmd);
	void FindTarget(struct usercmd_s *usercmd);
	void DrawRadarsPoints();
	void DrawWayPoint();
	void DrawWayLine();
	void groupCountCalc();
	void getFileName();
	void SaveToFile();
	void LoadFromFile();
	void add(int index);
	void clear(int index);
	void clearAll();
	void record();

	//±‰¡ø
	int iLine;
	int iPoint;
	int direction;
	int beforeGroup;
	int target_point;
	float firstDist;
	float nextDist;
	bool FindNearPoint;
	bool isRunning;
    bool runContinue;
	bool isRecord;
	bool CalcRecordIndex;
protected:
	char file_name[256];
	char map_name[256];
	int group_count;
	int point_count[11];
public:
	WAYPOINT(){ target_point=-1; group_count=0;
	point_count[1]=0; point_count[2]=0; point_count[3]=0; point_count[4]=0;
	point_count[5]=0; point_count[6]=0; point_count[7]=0; point_count[8]=0;
	point_count[9]=0; point_count[10]=0;
	FindNearPoint=false; isRunning=false; iLine=-1; iPoint=-1;
	runContinue=false; direction=-1; beforeGroup=-1;isRecord=false;CalcRecordIndex=false;}
};

extern void func_way_load(void);
extern void func_way_save(void);
extern void func_way_clear();
extern void func_way_clear_all();
extern void func_way_record();
extern void func_way_record_stop();

extern WAYPOINT waypoint;
