/////
///// CBOBY V2 [Revision by ETK]
/////
///// By [boy_scout][boyscout_etk@hotmail.com]
/////
///// [2011][www.etalking.com.ar]
/////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// 									HURRICANE 0.1 WAYPOINT + AUTODIRECTION PORT by x F R 3 N Z Y M O V x 
/////												-=[CB]=- Hook -V9- by ***cboby+suyuke***
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>

#include "../client.h"
#include "autoadjust.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HURRICANE autodirection.cpp [by cs_007]
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AutoAdjust autoadjust;

#define U_PI 3.141592

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool AutoAdjust::MyFree(float* from,float* to)
{
	sMe& l = g_Local;

	pmtrace_t tr;
	g_Engine.pEventAPI->EV_SetTraceHull( 2 );
	g_Engine.pEventAPI->EV_PlayerTrace( from, to, PM_GLASS_IGNORE, l.iIndex, &tr );
	return ( tr.fraction == 1.0 ); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AutoAdjust::NextPos(float *nowpos,float *viewangles,float step,float *newpos)
{
	float dx,dy,dz,dxy;
	dx=dy=0;
        dz = 0;
        dxy = step;
	if( viewangles[1]>90 && viewangles[1]<=180 )dx = dxy * -cos((180.0-viewangles[1])*U_PI/180.0);
	else if( viewangles[1]>=0 && viewangles[1]<=90 )dx = dxy * cos(viewangles[1]*U_PI/180.0);
	else if( viewangles[1]>=-90 && viewangles[1]<0 )dx = dxy * cos((-viewangles[1])*U_PI/180.0);
	else if( viewangles[1]>=-180 && viewangles[1]<-90)dx = dxy * -cos((180.0+viewangles[1])*U_PI/180.0);
	if( viewangles[1]>90 && viewangles[1]<=180 )dy = dxy * sin((180.0-viewangles[1])*U_PI/180.0);
	else if( viewangles[1]>=0 && viewangles[1]<=90 )dy = dxy * sin(viewangles[1]*U_PI/180.0);
	else if( viewangles[1]>=-90 && viewangles[1]<0 )dy = dxy * -sin((-viewangles[1])*U_PI/180.0);
	else if( viewangles[1]>=-180 && viewangles[1]<-90)dy = dxy * -sin((180.0+viewangles[1])*U_PI/180.0);
	newpos[0]=nowpos[0]+dx;
	newpos[1]=nowpos[1]+dy;
	newpos[2]=nowpos[2]+dz;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AutoAdjust::SetAngle(float *viewangles)
{
	if( viewangles[0]>180 )viewangles[0]=viewangles[0]-360;
	else if( viewangles[0]<-180 )viewangles[0]=viewangles[0]+360;	
	if( viewangles[1]>180 )viewangles[1]=viewangles[1]-360;
	else if( viewangles[1]<-180 )viewangles[1]=viewangles[1]+360;
	if( viewangles[2]>180 )viewangles[2]=viewangles[2]-360;
	else if( viewangles[2]<-180 )viewangles[2]=viewangles[2]+360;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool AutoAdjust::IsMyfree(float step,float *viewangles)//头
{
	sMe& l = g_Local;

	l.pEnt = g_Engine.GetLocalPlayer();
	
	float src[3],src1[3],src2[3];
	float dst[3],dst1[3],dst2[3];
	float angles[3];
	bool free,free1,free2;
	src[0]=l.Eye[0];
	src[1]=l.Eye[1];
	src[2]=l.Eye[2];
	angles[0]=0;angles[2]=0;
	angles[1]=viewangles[1];
	SetAngle(angles);
	NextPos(src,angles,step,dst);
	angles[0]=0;angles[2]=0;
	angles[1]=viewangles[1]+90;
	SetAngle(angles);
	NextPos(src,angles,15,src1);
	angles[0]=0;angles[2]=0;
	angles[1]=viewangles[1];
	SetAngle(angles);
	NextPos(src1,angles,step,dst1);
	angles[0]=0;angles[2]=0;
	angles[1]=viewangles[1]-90;
	SetAngle(angles);
	NextPos(src,angles,15,src2);
	angles[0]=0;angles[2]=0;
	angles[1]=viewangles[1];
	SetAngle(angles);
	NextPos(src2,angles,step,dst2);
	free=MyFree(src,dst);
	free1=MyFree(src1,dst1);
	free2=MyFree(src2,dst2);
	if( free&free1&free2 )return true;
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool AutoAdjust::Isjumpfree(float step,float *viewangles)//脚
{
	sMe& l = g_Local;

	l.pEnt = g_Engine.GetLocalPlayer();

	float src[3],src1[3],src2[3];
	float dst[3],dst1[3],dst2[3];
	float angles[3];
	bool free,free1,free2;
	src[0]=l.Eye[0];
	src[1]=l.Eye[1];
	src[2]=l.Eye[2]-35;
	angles[0]=0;angles[2]=0;
	angles[1]=viewangles[1];
	SetAngle(angles);
	NextPos(src,angles,step,dst);
	angles[0]=0;angles[2]=0;
	angles[1]=viewangles[1]+90;
	SetAngle(angles);
	NextPos(src,angles,15,src1);
	angles[0]=0;angles[2]=0;
	angles[1]=viewangles[1];
	SetAngle(angles);
	NextPos(src1,angles,step,dst1);
	angles[0]=0;angles[2]=0;
	angles[1]=viewangles[1]-90;
	SetAngle(angles);
	NextPos(src,angles,15,src2);
	angles[0]=0;angles[2]=0;
	angles[1]=viewangles[1];
	SetAngle(angles);
	NextPos(src2,angles,step,dst2);
	free=MyFree(src,dst);
	free1=MyFree(src1,dst1);
	free2=MyFree(src2,dst2);
	if( free&free1&free2 )return true;
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool AutoAdjust::Jump(float *viewangles) //自动跳跃障碍
{
	float oldviews[3];
	int step=30;
	oldviews[0]=viewangles[0];
	oldviews[1]=viewangles[1];
	oldviews[2]=viewangles[2];
	if( IsMyfree(step,oldviews) )
	{
		if( !Isjumpfree(step,oldviews) )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool AutoAdjust::direction(float *viewangles) //自动避墙
{
	float oldviews[3],newview1[3],newview2[3];
	float da;
	bool free1,free2;
	int step=30;
	oldviews[0]=viewangles[0];
	oldviews[1]=viewangles[1];
	oldviews[2]=viewangles[2];
	if( IsMyfree(step,oldviews) )return false;
	for(da=15;da<=165;da=da+15)
	{
		newview1[0]=oldviews[0];
		newview1[1]=oldviews[1]+da;
		newview1[2]=oldviews[2];
		SetAngle(newview1);
		if( IsMyfree(step,newview1) ) free1=true; else free1=false;

		newview2[0]=oldviews[0];
		newview2[1]=oldviews[1]-da;
		newview2[2]=oldviews[2];
		SetAngle(newview2);
		if( IsMyfree(step,newview2) )free2=true; else free2=false;
		if( free1 && !free2 )goto aimnewview1;
		if( !free1 && free2 )goto aimnewview2;
		if( free1 && free2 )
				goto aimnewview1; 
	}
	viewangles[0]=oldviews[0];
	viewangles[1]=oldviews[1]-180;
	viewangles[2]=oldviews[2];
	SetAngle(viewangles);
	return true;
    aimnewview1:
	viewangles[0]=newview1[0];
	viewangles[1]=newview1[1];
	viewangles[2]=newview1[2];
	return true;
    aimnewview2:
	viewangles[0]=newview2[0];
	viewangles[1]=newview2[1];
	viewangles[2]=newview2[2];
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
