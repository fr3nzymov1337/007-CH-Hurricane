#include "client.h"

SCREENINFO g_Screen;

void GetScreenInfo()
{
	static bool FirstFrame = true;
	if (FirstFrame)
	{
		g_Screen.iSize = sizeof(SCREENINFO);
		gConsole.echo("&a ID: &w007 Client Hook [v3 - Hurricane] ");
		gConsole.echo(" ");
		gConsole.echo("&a Code by: &w007, boy_scout, GYJ, cboby, b2k5, -=[eVOL]=-");
		gConsole.echo(" ");
		gConsole.echo("&a Thanks to: &wcafeed28 &afor &wUCC HookMsg bypass method");
		gConsole.echo(" ");
		gConsole.echo("&a Compiled by: &wx F R 3 N Z Y M O V x &a[19.05.2025]");
		FirstFrame = false;
	}
	g_pEngine->pfnGetScreenInfo(&g_Screen);
}
bool WorldToScreen(float *pflOrigin, float *pflVecScreen)
{
	int iResult = g_Engine.pTriAPI->WorldToScreen(pflOrigin, pflVecScreen);
	if (pflVecScreen[0] < 1 && pflVecScreen[1] < 1 && pflVecScreen[0] > -1 && pflVecScreen[1] > -1 && !iResult)
	{
		pflVecScreen[0] = pflVecScreen[0] * (g_Screen.iWidth / 2) + (g_Screen.iWidth / 2);
		pflVecScreen[1] = -pflVecScreen[1] * (g_Screen.iHeight / 2) + (g_Screen.iHeight / 2);
		return true;
	}
	return false;
}

void DrawString(int x, int y, int r, int g, int b, const char *fmt, ...)
{
	va_list va_alist;
	char buf[256];
	va_start(va_alist, fmt);
	_vsnprintf(buf, sizeof(buf), fmt, va_alist);
	va_end(va_alist);
	g_Engine.pfnDrawSetTextColor((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f);
	g_Engine.pfnDrawConsoleString(x, y, buf);
}

void DrawConString (int x, int y, int r, int g, int b, const char *fmt, ... )
{
	va_list va_alist;
	char buf[256];
	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);
	g_Engine.pfnDrawSetTextColor(r/255.0f, g/255.0f, b/255.0f);
	g_Engine.pfnDrawConsoleString(x,y,buf);
}

void DrawGlowHudString(int x, int y, int r, int g, int b, const char* fmt, ... )
{
	va_list va_alist;
	char buf[256];
	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);
	DrawHudStringCenter(x-2,y,r,g,b,buf);
	DrawHudStringCenter(x+2,y,r,g,b,buf);
	DrawHudStringCenter(x,y+2,r,g,b,buf);
	DrawHudStringCenter(x,y-2,r,g,b,buf);
	DrawHudStringCenter(x-1,y-1,r,g,b,buf);
	DrawHudStringCenter(x+1,y+1,r,g,b,buf);
	DrawHudStringCenter(x+1,y-1,r,g,b,buf);
	DrawHudStringCenter(x-1,y+1,r,g,b,buf);
	DrawHudStringCenter(x,y,255,255,255,buf);
}

void DrawHudStringCenter(int x, int y, int r, int g, int b, const char *fmt, ...)
{
	va_list va_alist;
	char buf[256];

	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);
	
	int length, height;
	g_Engine.pfnDrawConsoleStringLen( buf, &length, &height );
	x = x - length/2;
	g_Engine.pfnDrawSetTextColor((float)r/255.0,(float)g/255.0,(float)b/255.0);
	g_Engine.pfnDrawConsoleString(x,y,buf);
}

void FillRGBA(GLfloat x, GLfloat y, int w, int h, UCHAR r, UCHAR g, UCHAR b, UCHAR a)
{
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4ub(r,g,b,a);
	glBegin(GL_QUADS);
	glVertex2f(x,y);
	glVertex2f(x+w,y);
	glVertex2f(x+w,y+h);
	glVertex2f(x,y+h);
	glEnd();
	glDisable(GL_BLEND);

	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
}

void FillArea(float x, float y, int w, int h, int r, int g, int b, int a)
{
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glShadeModel(GL_SMOOTH);
	glBegin(GL_TRIANGLE_STRIP);
	glColor4ub(r,g,b,a);
	glVertex2f(x,y + h);
	glVertex2f(x + w,y + h);
	glColor4ub(r,g,b,a);
	glVertex2f(x,y);
	glVertex2f(x + w,y);
	glEnd();
	
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glPopMatrix();
}

void DrawHudStringCenterEsp (int x, int y, int r, int g, int b, float back, float tint, const char *fmt, ... )
{
	va_list va_alist;
	char buf[256];

	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);
	
	int length, height;
	g_Engine.pfnDrawConsoleStringLen( buf, &length, &height );
	x = x - length/2;
	if(back)
	{
		FillRGBA( x, y+1, length+2, 1, cvar.color_red*255.0f, cvar.color_green*255.0f, cvar.color_blue*255.0f, tint/255*128);
		FillRGBA( x, y+2, length+2, 1, cvar.color_red*235.0f, cvar.color_green*235.0f, cvar.color_blue*235.0f, tint/255*128);
		FillRGBA( x, y+3, length+2, 1, cvar.color_red*215.0f, cvar.color_green*215.0f, cvar.color_blue*215.0f, tint/255*128);
		FillRGBA( x, y+4, length+2, 1, cvar.color_red*195.0f, cvar.color_green*195.0f, cvar.color_blue*195.0f, tint/255*128);
		FillRGBA( x, y+5, length+2, 1, cvar.color_red*175.0f, cvar.color_green*175.0f, cvar.color_blue*175.0f, tint/255*128);
		FillRGBA( x, y+6, length+2, 1, cvar.color_red*155.0f, cvar.color_green*155.0f, cvar.color_blue*155.0f, tint/255*128);
		FillRGBA( x, y+7, length+2, 1, cvar.color_red*135.0f, cvar.color_green*135.0f, cvar.color_blue*135.0f, tint/255*128);
		FillRGBA( x, y+8, length+2, 1, cvar.color_red*115.0f, cvar.color_green*115.0f, cvar.color_blue*115.0f, tint/255*128);
		FillRGBA( x, y+9, length+2, 1, cvar.color_red*95.0f, cvar.color_green*95.0f, cvar.color_blue*95.0f, tint/255*128);
		FillRGBA( x, y+10, length+2, 1, cvar.color_red*75.0f, cvar.color_green*75.0f, cvar.color_blue*75.0f, tint/255*128);
		FillRGBA( x, y+11, length+2, 1, cvar.color_red*55.0f, cvar.color_green*55.0f, cvar.color_blue*55.0f, tint/255*128);
		FillRGBA( x, y+12, length+2, 1, cvar.color_red*35.0f, cvar.color_green*35.0f, cvar.color_blue*35.0f, tint/255*128);
		FillRGBA( x, y+13, length+2, 1, cvar.color_red*15.0f, cvar.color_green*15.0f, cvar.color_blue*25.0f, tint/255*128);
		FillRGBA( x, y+14, length+2, 1, cvar.color_red*05.0f, cvar.color_green*05.0f, cvar.color_blue*05.0f, tint/255*128);

		FillRGBA( x-1, y, length+4, 1 ,cvar.color_red*255.0f/2, cvar.color_green*255.0f/2, cvar.color_blue*255.0f/2,tint/1.0f*0.55 );    //top
		FillRGBA( x-1, y+1, 1, 14 ,cvar.color_red*255.0f/2, cvar.color_green*255.0f/2, cvar.color_blue*255.0f/2,tint/1.0f*0.55 );	        //left
		FillRGBA( x-2+length+4, y, 1, 15 ,255/1.0f*0.22f, 255/1.0f*0.22f, 255/1.0f*0.22f,tint );	//right
		FillRGBA( x-1, y+15, length+4, 1 ,255/1.0f*0.22f, 255/1.0f*0.22f, 255/1.0f*0.22f, tint ); //bottom
	}
	g_Engine.pfnDrawSetTextColor((float)r/255.0,(float)g/255.0,(float)b/255.0);
	g_Engine.pfnDrawConsoleString(x,y,buf);
}

void blackBorder(int x,int y,int w, int h, int tint)
{
	FillRGBA( x-1, y-1, w+2, 1 ,0,0,0,tint ); 
	FillRGBA( x-1, y, 1, h-1 ,0,0,0,tint );	
	FillRGBA( x+w, y, 1, h-1 ,0,0,0,tint );	
	FillRGBA( x-1, y+h-1, w+2, 1 ,0,0,0,tint ); 
}

void DrawFlatBox(int x, int y, int width, int height, float br, float bg, float bb, float ba)
{
	glDisable(GL_TEXTURE_2D);
	glShadeModel(GL_FLAT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(br,bg,bb,ba);
	glBegin(GL_QUADS);
	glVertex2i(x,y);
	glVertex2i(x+width,y);
	glVertex2i(x+width,y+height);
	glVertex2i(x,y+height);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void DrawSmoothBox(int x, int y, int width, int height, float ubr, float ubg, float ubb, float lbr, float lbg, float lbb, float ba)
{
	glDisable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUADS);
	glColor4f(ubr,ubg,ubb,ba);
	glVertex2i(x,y);
	glColor4f(ubr,ubg,ubb,ba);
	glVertex2i(x+width,y);
	glColor4f(lbr,lbg,lbb,ba);
	glVertex2i(x+width,y+height);
	glColor4f(lbr,lbg,lbb,ba);
	glVertex2i(x,y+height);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void Draw3DSmoothBox(int x, int y, int width, int height, float ubr, float ubg, float ubb, float lbr, float lbg, float lbb, float ba)
{
	DrawSmoothBox(x,y,width,height,ubr,ubg,ubb,lbr,lbg,lbb,ba);
	DrawFlatBox(x,y,width,1,0.22f,0.22f,0.22f,0.22f);
	DrawFlatBox(x+width,y,-1,height,0.22f,0.22f,0.22f,0.78f);
	DrawFlatBox(x+width,y+height,-width,-1,0.22f,0.22f,0.22f,0.78f);
	DrawFlatBox(x,y+height,1,-height,0.22f,0.22f,0.22f,0.22f);
}

void DrawFlatBorder(int x, int y, int width, int height, float bor, float bog, float bob, float boa, int line)
{
	//I know i can do this with GL_LINES and GL_LINE_LOOP, but y should i xD
	DrawFlatBox(x,y,width,line,bor,bog,bob,boa);
	DrawFlatBox(x+width,y,-line,height,bor,bog,bob,boa);
	DrawFlatBox(x+width,y+height,-width,-line,bor,bog,bob,boa);
	DrawFlatBox(x,y+height,line,-height,bor,bog,bob,boa);
}

void Draw3DSmoothBoxAndSmoothHeaderWithFlatBorder(int x, int y, int bw, int bh, int hh, float ubr, float ubg, float ubb, float lbr, float lbg, float lbb, float ba, float uhr, float uhg, float uhb, float lhr, float lhg, float lhb, float ha, float bor, float bog, float bob, float boa, int line)
{
	Draw3DSmoothBox(x+line,y+line,bw-(line*2),hh-(line*2),uhr,uhg,uhb,lhr,lhg,lhb,ha);
	DrawFlatBorder(x,y,bw,hh,bor,bog,bob,boa,line);
	Draw3DSmoothBox(x+line,y+hh+line,bw-(line*2),bh-(line*2),ubr,ubg,ubb,lbr,lbg,lbb,ba);
	DrawFlatBorder(x,y+hh,bw,bh,bor,bog,bob,boa,line);
}

void Draw3DSmoothBoxAndSmoothHeaderWithFlatBorderRdar(int x, int y, int bw, int bh, int hh, float ubr, float ubg, float ubb, float lbr, float lbg, float lbb, float ba, float uhr, float uhg, float uhb, float lhr, float lhg, float lhb, float ha, float bor, float bog, float bob, float boa, int line)
{
	Draw3DSmoothBox(x+line,y+line,bw-(line*2),hh-(line*2),uhr,uhg,uhb,lhr,lhg,lhb,ha);
	DrawFlatBorder(x,y,bw,hh,bor,bog,bob,boa,line);
}

void BorderMenu(int x,int y,int w, int h,int r,int g,int b,int a)
{
	FillRGBA( x+1, y-1, w, 1 ,r,g,b,0.22f*255.0f ); //top
	FillRGBA( x, y-1, 1, h+1 ,r,g,b,0.22f*255.0f );	//left
	FillRGBA( x+w, y, 1, h-1 ,r,g,b,0.78f*255.0f );	//right
	FillRGBA( x+1, y+h-1, w, 1 ,r,g,b,0.78f*255.0f ); //bottom
}

void DrawBoxAtScreenXY( int x, int y, int r, int g, int b, int alpha, int radius=1)
{
	int radius1 = radius<<1;
	FillRGBA(x-radius+1, y-radius, radius1-1,1,r,g,b,alpha);
	FillRGBA(x-radius, y-radius, 1,radius1,r,g,b,alpha);
	FillRGBA(x-radius, y+radius, radius1,1,r,g,b,alpha);
	FillRGBA(x+radius, y-radius, 1,radius1+1,r,g,b,alpha);
}

void tintArea(int x,int y,int w,int h,int r,int g,int b,int a)
{
	FillRGBA(x,y,w,h,r,g,b,a);
}

void DrawStippledLines(int x1,int y1,int x2,int y2,int width,GLint factor,GLushort pattern,ColorEntry *color)
{
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glColor4ub(color->r,color->g,color->b,color->a);
	glLineWidth((float)width);
	glLineStipple(factor, pattern);
	glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINES);
	glVertex2i(x1,y1);
	glVertex2i(x2,y2);
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_LINE_STIPPLE);
}

void DrawStippledVectorLine(float *flSrc,float *flDestination,int width,GLint factor,GLushort pattern,ColorEntry *color)
{
	vec3_t vScreenSrc,vScreenDest;
	if(!WorldToScreen(flSrc,vScreenSrc) | !WorldToScreen(flDestination,vScreenDest))
		return;
	DrawStippledLines((int)vScreenSrc[0],(int)vScreenSrc[1],(int)vScreenDest[0],(int)vScreenDest[1],width,factor,pattern,color);
}

void DrawStippledLines(int x1,int y1,int x2,int y2,int width,GLint factor,GLushort pattern, int r, int g, int b, int tint)
{
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glColor4ub(r,g,b,tint);
	glLineWidth((float)width);
	if(factor)glLineStipple(factor, pattern);
	if(factor)glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINES);
	glVertex2i(x1,y1);
	glVertex2i(x2,y2);
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	if(factor)glDisable(GL_LINE_STIPPLE);
}

void DrawStippledVectorLine(float *flSrc,float *flDestination, int width, GLint factor, GLushort pattern, int r, int g, int b, int tint)
{
	vec3_t vScreenSrc,vScreenDest;
	if(!WorldToScreen(flSrc,vScreenSrc) | !WorldToScreen(flDestination,vScreenDest))
		return;
	DrawStippledLines((int)vScreenSrc[0],(int)vScreenSrc[1],(int)vScreenDest[0],(int)vScreenDest[1],width,factor,pattern,r,g,b,tint);
}

void BeamDrawLine(float* from, float* to)
{
	sMe& l = g_Local;

	int beamindex = g_Engine.pEventAPI->EV_FindModelIndex("sprites/laserbeam.spr");
	g_Engine.pEfxAPI->R_BeamPoints(from,to,beamindex,l.FrameTime*10.0f,2.0f,0,10,0,0,0,cvar.color_red, cvar.color_green, cvar.color_blue);
}

int iStringLen( const char *fmt, ... )
{
	va_list va_alist;
	char buf[256];
	va_start( va_alist, fmt );
	_vsnprintf( buf, sizeof( buf ), fmt, va_alist );
	va_end( va_alist );
	int iWidth, iHeight;
	g_Engine.pfnDrawConsoleStringLen( buf, &iWidth, &iHeight );
	return iWidth;
}

int iStringHeight( void )
{
	int iWidth, iHeight;
	g_Engine.pfnDrawConsoleStringLen( "F", &iWidth, &iHeight );
	return iHeight;
}

void gDrawFilledBoxAtLocation( float* origin, int r, int g, int b, int a, int radius)
{
	float vecScreen[2]; 
	if( !WorldToScreen(origin, vecScreen) ) { return; }
	int radius2 = radius<<1;
	tintArea(vecScreen[0]-radius,vecScreen[1]-radius,radius2,radius2,r,g,b,a);
}

void DrawLines(int x1,int y1,int x2,int y2,int width,int r,int g,int b,int tint)
{
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glColor4ub(r,g,b,tint);
	glLineWidth((float)width);
	glBegin(GL_LINES);
	glVertex2i(x1,y1);
	glVertex2i(x2,y2);
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void DrawLines(int x1,int y1,int x2,int y2,int width,ColorEntry *color,int tint)
{
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glColor4ub(color->r,color->g,color->b,tint);
	glLineWidth((float)width);
	glBegin(GL_LINES);
	glVertex2i(x1,y1);
	glVertex2i(x2,y2);
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void DrawVectorLine(float *flSrc,float *flDestination,int width,ColorEntry *color,int tint)
{
	vec3_t vScreenSrc,vScreenDest;
	if(!WorldToScreen(flSrc,vScreenSrc) | !WorldToScreen(flDestination,vScreenDest))
		return;
	DrawLines((int)vScreenSrc[0],(int)vScreenSrc[1],(int)vScreenDest[0],(int)vScreenDest[1],width,color,tint);
}

void Box(int x, int y, int w)
{
	FillRGBA(x+w-18,y-14,1,15,0,0,0,255); //left box
	FillRGBA(x+w-4,y-14,1,15,0,0,0,255); //right box
	FillRGBA(x+w-17,y-14,13,1,0,0,0,255); //up box
	FillRGBA(x+w-17,y,13,1,0,0,0,255); //down box

	FillRGBA(x+w-17,y-13,13,1,245,0,0,255); //fill top box
	FillRGBA(x+w-17,y-12,13,1,235,0,0,255); //down box
	FillRGBA(x+w-17,y-11,13,1,225,0,0,255); //down box
	FillRGBA(x+w-17,y-10,13,1,215,0,0,255); //down box
	FillRGBA(x+w-17,y-9,13,1,205,0,0,255); //down box
	FillRGBA(x+w-17,y-8,13,1,195,0,0,255); //down box
	FillRGBA(x+w-17,y-7,13,1,185,0,0,255); //down box
	FillRGBA(x+w-17,y-6,13,1,175,0,0,255); //down box
	FillRGBA(x+w-17,y-5,13,1,165,0,0,255); //down box
	FillRGBA(x+w-17,y-4,13,1,155,0,0,255); //down box
	FillRGBA(x+w-17,y-3,13,1,145,0,0,255); //down box
	FillRGBA(x+w-17,y-2,13,1,135,0,0,255); //down box
	FillRGBA(x+w-17,y-1,13,1,125,0,0,255); //down box
		
	FillRGBA(x+w-7,y-11,1,1,0,0,0,255); //up X box
	FillRGBA(x+w-15,y-11,1,1,0,0,0,255); //up X box
	FillRGBA(x+w-8,y-10,1,1,0,0,0,255); //up X box
	FillRGBA(x+w-14,y-10,1,1,0,0,0,255); //up X box
	FillRGBA(x+w-9,y-9,1,1,0,0,0,255); //up X box
	FillRGBA(x+w-13,y-9,1,1,0,0,0,255); //up X box
	FillRGBA(x+w-10,y-8,1,1,0,0,0,255); //up X box
	FillRGBA(x+w-12,y-8,1,1,0,0,0,255); //up X box
	FillRGBA(x+w-11,y-7,1,1,0,0,0,255); //middle X box
	FillRGBA(x+w-10,y-6,1,1,0,0,0,255); //down X box
	FillRGBA(x+w-12,y-6,1,1,0,0,0,255); //down X box
	FillRGBA(x+w-9,y-5,1,1,0,0,0,255); //down X box
	FillRGBA(x+w-13,y-5,1,1,0,0,0,255); //down X box
	FillRGBA(x+w-8,y-4,1,1,0,0,0,255); //down X box
	FillRGBA(x+w-14,y-4,1,1,0,0,0,255); //down X box
	FillRGBA(x+w-7,y-3,1,1,0,0,0,255); //down X box
	FillRGBA(x+w-15,y-3,1,1,0,0,0,255); //down X box
}

void RadarBox(int x, int y, int w)
{
	FillRGBA(x+w-16,y-w-18,1,15,0,0,0,255); //left box
	FillRGBA(x+w-2,y-w-18,1,15,0,0,0,255); //right box
	FillRGBA(x+w-15,y-w-18,13,1,0,0,0,255); //up box
	FillRGBA(x+w-15,y-w-4,13,1,0,0,0,255); //down box

	FillRGBA(x+w-15,y-w-17,13,1,245,0,0,255); //fill top box
	FillRGBA(x+w-15,y-w-16,13,1,235,0,0,255); //down box
	FillRGBA(x+w-15,y-w-15,13,1,225,0,0,255); //down box
	FillRGBA(x+w-15,y-w-14,13,1,215,0,0,255); //down box
	FillRGBA(x+w-15,y-w-13,13,1,205,0,0,255); //down box
	FillRGBA(x+w-15,y-w-12,13,1,195,0,0,255); //down box
	FillRGBA(x+w-15,y-w-11,13,1,185,0,0,255); //down box
	FillRGBA(x+w-15,y-w-10,13,1,175,0,0,255); //down box
	FillRGBA(x+w-15,y-w-9,13,1,165,0,0,255); //down box
	FillRGBA(x+w-15,y-w-8,13,1,155,0,0,255); //down box
	FillRGBA(x+w-15,y-w-7,13,1,145,0,0,255); //down box
	FillRGBA(x+w-15,y-w-6,13,1,135,0,0,255); //down box
	FillRGBA(x+w-15,y-w-5,13,1,125,0,0,255); //down box
	
	FillRGBA(x+w-5,y-w-15,1,1,0,0,0,255); //up X box
	FillRGBA(x+w-13,y-w-15,1,1,0,0,0,255); //up X box
	FillRGBA(x+w-6,y-w-14,1,1,0,0,0,255); //up X box
	FillRGBA(x+w-12,y-w-14,1,1,0,0,0,255); //up X box
	FillRGBA(x+w-7,y-w-13,1,1,0,0,0,255); //up X box
	FillRGBA(x+w-11,y-w-13,1,1,0,0,0,255); //up X box
	FillRGBA(x+w-8,y-w-12,1,1,0,0,0,255); //up X box
	FillRGBA(x+w-10,y-w-12,1,1,0,0,0,255); //up X box
	FillRGBA(x+w-9,y-w-11,1,1,0,0,0,255); //middle X box
	FillRGBA(x+w-8,y-w-10,1,1,0,0,0,255); //down X box
	FillRGBA(x+w-10,y-w-10,1,1,0,0,0,255); //down X box
	FillRGBA(x+w-7,y-w-9,1,1,0,0,0,255); //down X box
	FillRGBA(x+w-11,y-w-9,1,1,0,0,0,255); //down X box
	FillRGBA(x+w-6,y-w-8,1,1,0,0,0,255); //down X box
	FillRGBA(x+w-12,y-w-8,1,1,0,0,0,255); //down X box
	FillRGBA(x+w-5,y-w-7,1,1,0,0,0,255); //down X box
	FillRGBA(x+w-13,y-w-7,1,1,0,0,0,255); //down X box
}