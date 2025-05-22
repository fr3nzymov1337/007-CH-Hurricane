/////
///// CBOBY V2 [Revision by ETK]
/////
///// By [boy_scout][boyscout_etk@hotmail.com]
/////
///// [2011][www.etalking.com.ar]
/////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// 									HURRICANE 0.1 WAYPOINT + AUTODIRECTION PORT BY x F R 3 N Z Y M O V x 
/////												-=[CB]=- Hook -V9- by ***cboby+suyuke***
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma warning(disable:4800)

#include "../client.h"
#include "waypoint.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern char hackdir[256];

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//HURRICANE v0.1 waypoint.cpp [by cs_007]
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PI 3.1416
#define POINT_MAX 999
#define GROUP_MAX 11

WAYPOINT waypoint;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct WAY
{
	float  point[3];
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

WAY ways[GROUP_MAX][POINT_MAX];

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float WAYPOINT::relativeSpeed()
{
	sMe& l = g_Local;

	float speed = sqrt(l.Velocity[0] * l.Velocity[0] + l.Velocity[1] * l.Velocity[1]);
	return speed;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool WAYPOINT::pathFree(float* from,float* to)
{
	sMe& l = g_Local;

	pmtrace_t tr; 
	g_Engine.pEventAPI->EV_SetTraceHull( 2 ); 
	g_Engine.pEventAPI->EV_PlayerTrace( from, to, PM_GLASS_IGNORE, l.iIndex, &tr ); 
	return  (tr.fraction == 1.0); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float WAYPOINT::getDist(float *point)
{	
	sMe& l = g_Local;

	register float a = point[0] - l.Eye[0];
	register float b = point[1] - l.Eye[1];
	register float c = point[2] - l.Eye[2];

	return sqrt(a*a + b*b + c*c);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WAYPOINT::aimAngleCalc(float *point,float *viewangles)
{
	sMe& l = g_Local;

	float src[3],dst[3];
	float dx,dy,b;

	src[0]=l.Origin[0];
	src[1]=l.Origin[1];

	dst[0]=point[0];
	dst[1]=point[1];

	dx = dst[0]-src[0];
	dy = dst[1]-src[1];

	if( dx==0&&dy==0 ){ b=0; }
	else if( dx==0 && dy>0  )b=90;
	else if( dx==0 && dy<0  )b=-90;
	else if( dx>0  && dy==0 )b=0;
	else if( dx<0  && dy==0 )b=180;
	else if( dx>0  && dy>0  )b=atan(dy/dx)*180.0/PI;
	else if( dx>0  && dy<0  )b=-atan(-dy/dx)*180.0/PI;
	else if( dx<0  && dy>0  )b=180-(atan(dy/-dx)*180.0/PI);
	else if( dx<0  && dy<0  )b=-(180-(atan(-dy/-dx)*180.0/PI));

	viewangles[0]=0;
	viewangles[1]=b;
	viewangles[2]=0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float WAYPOINT::getAngle(float *point)
{
	float angles[3];
	float angle;

	aimAngleCalc(point,angles);

	angles[0]=0;
	angles[2]=0;
//	angle=angles[1]-me.viewAngles[1];
	angle=angles[1]-mainViewAngles[1];

	if(angle>=180)angle-=360;
	if(angle<=-180)angle+=360;

        return angle;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float WAYPOINT::MoveAnglesSub(float *point)
{
	sMe& l = g_Local;

	float moveDir[2], moveAngles[3];
	float pointDir[2], pointAngles[3];
	float angles_sub;

	moveDir[0] = l.Velocity[0];
	moveDir[1] = l.Velocity[1];

	pointDir[0] = point[0] - l.Eye[0];
	pointDir[1] = point[1] - l.Eye[1];

	VectorAngles(moveDir,moveAngles);
	VectorAngles(pointDir,pointAngles);

	angles_sub = pointAngles[1] - moveAngles[1];

	if(angles_sub > 180.0) angles_sub -= 360.0;
	if(angles_sub < -180.0) angles_sub += 360.0;

	if(angles_sub < 0.0) angles_sub *= -1;

	return angles_sub;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WAYPOINT::MoveTo(float *point, float* forwardmove, float* sidemove)
{
	sMe& l = g_Local;

	float addvec[2];
	float dodgeDir[2];

	addvec[0] = l.Eye[0] - point[0];
	addvec[1] = l.Eye[1] - point[1];

	dodgeDir[0] = addvec[0];
	dodgeDir[1] = addvec[1];

	dodgeDir[2] = 0;

	float dodgeAngles[3];
	VectorAngles(dodgeDir,dodgeAngles);

//	float angle = dodgeAngles[1] - me.viewAngles[1];
	float angle = dodgeAngles[1] - mainViewAngles[1];
	while(angle<0)   { angle+=360; }
	while(angle>360) { angle-=360; }

	float forw = (float)cos(angle*(PI/180.0))*250.0f;
	float side = (float)-sin(angle*(PI/180.0))*250.0f;

	*forwardmove = -forw;
	*sidemove    = -side;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WAYPOINT::runDirection(int cur_direction,int cur_group,struct usercmd_s *usercmd)
{
	sMe& l = g_Local;

	if(cur_direction==1)
	{
		for(int i=0;i<point_count[cur_group];i++)
		{
			if(getDist(ways[cur_group][i].point)<cvar.way_maxdist)
			{
				target_point=i;	
			}
		}
		if(target_point!=-1)
		{
			if(target_point!=point_count[cur_group]-1)
			{
				MoveTo(ways[cur_group][target_point].point, &usercmd->forwardmove, &usercmd->sidemove);
				if( relativeSpeed() <= 20.0 )
				if( !pathFree(l.Eye,ways[cur_group][target_point].point) )
				{
					FindNearPoint=false;isRunning=false;runContinue=true;beforeGroup=-1;iLine=-1;iPoint=-1;
				}					
			}
			else 
			{
				FindNearPoint=false;isRunning=false;runContinue=true;beforeGroup=cur_group;iLine=-1;iPoint=-1;
			} 
		}
	}
	if(cur_direction==2)
	{
		for(int i=point_count[cur_group]-1;i>=0;i--)
		{
			if(getDist(ways[cur_group][i].point)<cvar.way_maxdist)
			{
				target_point=i;	
			}
		}
		if(target_point!=-1)
		{
			if(target_point!=0)
			{
				MoveTo(ways[cur_group][target_point].point, &usercmd->forwardmove, &usercmd->sidemove);
				if( relativeSpeed() <= 20.0 )
				if( !pathFree(l.Eye,ways[cur_group][target_point].point) )
				{
					FindNearPoint=false;isRunning=false;runContinue=true;beforeGroup=-1;iLine=-1;iPoint=-1;
				}
			}
			else 
			{
				FindNearPoint=false;isRunning=false;runContinue=true;beforeGroup=cur_group;iLine=-1;iPoint=-1;
			} 
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WAYPOINT::FindTarget(struct usercmd_s *usercmd)
{
	sMe& l = g_Local;

	if(!l.MsgAlive){ FindNearPoint=false; isRunning=false; runContinue=false; beforeGroup=-1; iLine=-1; iPoint=-1; return; }
	if(!cvar.rush || !cvar.way_on){ FindNearPoint=false; isRunning=false; runContinue=false; beforeGroup=-1; iLine=-1; iPoint=-1; return; }
	if(IsKnife(l.WpnID) && iTargetID != 0){ FindNearPoint=false; isRunning=false; runContinue=false; beforeGroup=-1; iLine=-1; iPoint=-1; return; }

	if(iTargetID != 0){ FindNearPoint=false; isRunning=false; runContinue=false; beforeGroup=-1; iLine=-1; iPoint=-1; return; }

	float nearDist=550.0f;
	float minAngles = 180.0f;

	if(!FindNearPoint && !isRunning)
	{
		if(!runContinue)
		{
			for(int i=1;i<=group_count;i++)
			{
				if(point_count[i]>0)
				{
					for(int j=0;j<point_count[i];j++)
					{
						if(pathFree(l.Eye,ways[i][j].point))
						if(getDist(ways[i][j].point)<nearDist && getDist(ways[i][j].point)>cvar.way_mindist)
						if(getAngle(ways[i][j].point)<cvar.way_fov && getAngle(ways[i][j].point)>-cvar.way_fov)
						{
							iLine=i;
							iPoint=j;
							nearDist=getDist(ways[iLine][iPoint].point);
						}
					}
				}
			}
			if(iPoint != 0 && iPoint != point_count[iLine]-1)
			{
				firstDist=getDist(ways[iLine][iPoint].point);
				nextDist=getDist(ways[iLine][iPoint+1].point);
			}
		}
		else
		{
			for(int i=1;i<=group_count;i++)
			{
				if(i!=beforeGroup && point_count[i]>0)
				{
					for(int j=0;j<point_count[i];j++)
					{
						if(cvar.way_mode == 1)
						{
							if(pathFree(l.Eye,ways[i][j].point))
							if(getDist(ways[i][j].point)<nearDist && getDist(ways[i][j].point)>cvar.way_mindist)
							{
								iLine=i;
								iPoint=j;
								nearDist=getDist(ways[iLine][iPoint].point);
							}
						}
						if(cvar.way_mode == 2)
						{
							if(pathFree(l.Eye,ways[i][j].point))
							if(getDist(ways[i][j].point)<nearDist)
							if(MoveAnglesSub(ways[i][j].point) < minAngles)
							{
								iLine=i;
								iPoint=j;
								minAngles=MoveAnglesSub(ways[iLine][iPoint].point);
							}
						}
					}
				}
			}
			if(iPoint != 0 && iPoint != point_count[iLine]-1)
			{
				firstDist=getDist(ways[iLine][iPoint].point);
				nextDist=getDist(ways[iLine][iPoint+1].point);
			}
		}
	}
	if(iLine!=-1 && iPoint!=-1 && !isRunning)
	{
		direction = -1;
		FindNearPoint = true;
		if(getDist(ways[iLine][iPoint].point) < cvar.way_mindist)
		{
			if(iPoint == 0)
			{
				direction=1;
			}
			else
			if(iPoint == point_count[iLine]-1)
			{
				direction=2;
			}
			else
			if(firstDist<nextDist)
			{
				direction=1;
			}
			else
			{
				direction=2;
			}
		}
		if(direction != -1)
		{
			isRunning=true;
		}
		MoveTo(ways[iLine][iPoint].point, &usercmd->forwardmove, &usercmd->sidemove);
	}
	if(isRunning)
	{
		runDirection(direction,iLine,usercmd);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WAYPOINT::DrawWayPoint()
{
	sMe& l = g_Local;

	if(!l.MsgAlive) return;

	for(int i=1;i<=group_count;i++)
	{
		if(point_count[i]>0)
		{
			if (cvar.way_draw == 1)
			{
				for(int j=0;j<point_count[i];j++)
				{
					if(getDist(ways[i][j].point)<500)
					{
						float vecScreen[2];
						float newPoint[3];

						newPoint[0]=ways[i][j].point[0];
						newPoint[1]=ways[i][j].point[1];
						newPoint[2]=ways[i][j].point[2]-20;
				
						if (WorldToScreen(newPoint,vecScreen))
						{
							int x = vecScreen[0];
							int y = vecScreen[1];
							DrawHudStringCenterEsp(x,y, 255, 255, 255, cvar.misc_esp_back*255.0f, cvar.misc_tint*255.0f, ".");

							if(j==0 || j==point_count[i]-1)
							{
								DrawHudStringCenterEsp(x, y+12, 255, 255, 255, cvar.misc_esp_back*255.0f, cvar.misc_tint*255.0f, "%d",i);
							}
						}
					}
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WAYPOINT::DrawWayLine()
{
	sMe& l = g_Local;

	if(!l.MsgAlive) return;

	float vecScreen[2];
	float from[3], to[3];

	if(cvar.way_draw==2)
	{
		for(int i=1;i<=group_count;i++)
		{
			if(point_count[i]>0)
			{
				for(int j=0;j<point_count[i]-1;j++)
				{
					if(j==0 || j==point_count[i]-2)
					{
						if (WorldToScreen(ways[i][j].point,vecScreen))
						{
							from[0]=ways[i][j].point[0];from[1]=ways[i][j].point[1];from[2]=ways[i][j].point[2]-40;
							to[0]=ways[i][j+1].point[0];to[1]=ways[i][j+1].point[1];to[2]=ways[i][j+1].point[2]-40;
							if(getDist(ways[i][j].point)<800) { BeamDrawLine(from,to); } 
						}
					}
					else
					{
						if (WorldToScreen(ways[i][j-1].point,vecScreen) || WorldToScreen(ways[i][j+1].point,vecScreen))
						{
							from[0]=ways[i][j].point[0];from[1]=ways[i][j].point[1];from[2]=ways[i][j].point[2]-40;
							to[0]=ways[i][j+1].point[0];to[1]=ways[i][j+1].point[1];to[2]=ways[i][j+1].point[2]-40;
							if(getDist(ways[i][j].point)<800) { BeamDrawLine(from,to); }
						}
					}
				}
			}
	   	}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WAYPOINT::DrawRadarsPoints()
{
	for(int i=1;i<=group_count;i++)
	{
		if(point_count[i]>0)
		{
			for(int j=0;j<point_count[i]-1;j++)
			{
				if(!(j%10))
				{
					if(cvar.misc_radar && cvar.misc_radarpoints)DrawRadarPoint(ways[i][j].point,255,255,255,3,3,false);
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WAYPOINT::groupCountCalc()
{
	int index_max = 0;
	for(int i=1;i<GROUP_MAX;i++)
	{
		if(point_count[i]>0)
		index_max = i;
	}
	group_count = index_max;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WAYPOINT::getFileName()
{	
	char tfilename[256];
	strcpy(map_name, g_Engine.pfnGetLevelName() + 5);
	map_name[strlen(map_name)-4] = 0;
	sprintf(tfilename, "wpt/%s.wpt", map_name);

	sprintf(file_name, "%s%s", hackdir, tfilename);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WAYPOINT::SaveToFile()
{
	getFileName();
	FILE *fp;
	fp = fopen(file_name,"w");
	if(fp==NULL)return;
	fprintf(fp,"// Way File(%s)\n",map_name);
	groupCountCalc();
	for(int i=1;i<=group_count;i++)
	{
		if(point_count[i] > 0)
		{
			for(int j=0;j<point_count[i];j++)
			{
				fprintf(fp,"P(%d)(%d):%f %f %f\n",i,j,ways[i][j].point[0],ways[i][j].point[1],ways[i][j].point[2]);
			}
		}
	}
	fclose(fp); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WAYPOINT::LoadFromFile(void)
{
	char buf[256];
	getFileName();
	for(int i=1;i<GROUP_MAX;i++)
	{
		point_count[i] = 0;
	}
	group_count = 0;
	FILE *fp;
	fp = fopen(file_name,"r");
	if(fp==NULL)return;

	while(fgets(buf,256,fp))
	{
		int tmp;

		if(buf[0]!='P')continue;

		for(int i=1;i<GROUP_MAX;i++)
		{
			if(buf[2] == i+'0')
			{
				sscanf(buf,"P(%d)(%d):%f %f %f",&i,&tmp,&ways[i][point_count[i]].point[0],&ways[i][point_count[i]].point[1],&ways[i][point_count[i]].point[2]);
				point_count[i]++;
			}
		}
	}

	fclose(fp);

	groupCountCalc();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WAYPOINT::add(int index)
{
	sMe& l = g_Local;

	ways[index][point_count[index]].point[0]= l.Eye[0];
	ways[index][point_count[index]].point[1] = l.Eye[1];
	ways[index][point_count[index]].point[2] = l.Eye[2];

	point_count[index]++;
	SaveToFile();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WAYPOINT::clear(int index)
{
	point_count[index] = 0;
	SaveToFile();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WAYPOINT::clearAll()
{
	for(int i=1;i<GROUP_MAX;i++)
	{
		point_count[i] = 0;
	}
	group_count = 0;
	SaveToFile();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WAYPOINT::record()
{
	sMe& l = g_Local;

	if(!cvar.way_on && !l.MsgAlive) return;
	static int recordIndex = 1;

	if(CalcRecordIndex)
	{
		for(int i=1;i<GROUP_MAX;i++)
		{
			if(point_count[i] == 0)
			{
				recordIndex = i;
				break;
			}
		}
	}
	CalcRecordIndex = false;

	if(point_count[recordIndex] == 0)
	{
		add(recordIndex);
	}
	else
	if(point_count[recordIndex]<POINT_MAX)
	{
		if(getDist(ways[recordIndex][point_count[recordIndex]-1].point)>cvar.record_dist && getDist(ways[recordIndex][point_count[recordIndex]-1].point)<2*cvar.record_dist)
		{
			add(recordIndex);
		}
	}
	else
	{
 		CalcRecordIndex = true;
		return;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void func_way_record() { waypoint.isRecord = true; waypoint.CalcRecordIndex = true; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void func_way_record_stop() { waypoint.isRecord = false; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void func_way_load(void) { waypoint.LoadFromFile(); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void func_way_save(void) { waypoint.SaveToFile(); }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void func_way_clear()
{
	if(cmd.argS(1).empty())
	{
		waypoint.clearAll();
		return;
	}
	int index = cmd.argI(1);
	waypoint.clear(index);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void func_way_clear_all()
{
	waypoint.clearAll();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


