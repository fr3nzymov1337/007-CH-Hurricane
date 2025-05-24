#include "client.h"

float mainViewAngles[3];

void DrawRadarFrame()
{
	if(cvar.misc_radar)
	{
		g_Engine.pfnFillRGBA(g_Screen.iWidth/2,g_Screen.iHeight/2-cvar.misc_radars,1,2*cvar.misc_radars,0,255,0,cvar.misc_tint*255.0f);
		g_Engine.pfnFillRGBA(g_Screen.iWidth/2-cvar.misc_radars,g_Screen.iHeight/2,2*cvar.misc_radars,1,0,255,0,cvar.misc_tint*255.0f);
	}
}

static void calcRadarPoint(const float* origin, int& screenx, int& screeny, bool center)
{
	float dx  = origin[0] - g_Local.Eye[0];
	float dy  = origin[1] - g_Local.Eye[1];
    // rotate
	float yaw = mainViewAngles[1]*(3.1415/180.0);
	float sin_yaw       =  sin(yaw); 
	float minus_cos_yaw = -cos(yaw);

    float x =  dy*minus_cos_yaw + dx*sin_yaw;
	float y =  dx*minus_cos_yaw - dy*sin_yaw;
	
	float range = cvar.way_radardist;
	if(fabs(x)>range || fabs(y)>range)
	{ 
		// clipping
		if(y>x)
		{
			if(y>-x) {
				x = range*x/y;
				y = range;
			}  else  {
				y = -range*y/x; 
				x = -range; 
			}
		} else {
			if(y>-x) {
				y = range*y/x; 
				x = range; 
			}  else  {
				x = -range*x/y;
				y = -range;
			}
		}
	}
	screenx = g_Screen.iWidth/2+int(x/range*float(cvar.misc_radars));
	screeny = g_Screen.iHeight/2+int(y/range*float(cvar.misc_radars));
}

void DrawRadarPoint(const float* origin,int r,int g,int b,int w, int h, bool center)
{
	int screenx,screeny;

	if(center)
		calcRadarPoint(origin,screenx,screeny,center); 
	if(!center)
		calcRadarPoint(origin,screenx,screeny,center);

	FillRGBA(screenx-1,screeny-1,h,w,r,g,b,255);
	blackBorder(screenx-1,screeny-1,h,w+1,cvar.misc_tint*255.0f);
}
