#include "../client.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern SCREENINFO screeninfo;
extern bool oglSubtractive;
int HEALTH();
int ARMOR();
int CLIP();
int MONEY();
int AMMO();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct trapezoid_points
{
	int p1_x;
	int p1_y;
	int p2_x;
	int p2_y;
	int p3_x;
	int p3_y;
	int p4_x;
	int p4_y;
	///////////
	int p5_x;
	int p5_y;
	int p6_x;
	int p6_y;
	int p7_x;
	int p7_y;
};

struct rgba
{
	int r;
	int g; 
    int b; 
	int a;
};

struct blendcolor
{
	float red;
	float green;
	float blue;
	float upper_r; 
	float upper_g; 
	float upper_b;
	float lower_r; 
	float lower_g; 
	float lower_b; 
	float alpha;
};

struct blendcolor* InitBlendColors(int av_type, struct blendcolor* color2);
void DrawFlatBox(int x, int y, int width, int height, float br, float bg, float bb, float ba);
void DrawSmoothBox(int x, int y, int width, int height, float ubr, float ubg, float ubb, float lbr, float lbg, float lbb, float ba);
void Draw3DSmoothBox(int x, int y, int width, int height, float ubr, float ubg, float ubb, float lbr, float lbg, float lbb, float ba);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void RED(struct blendcolor &color2)
{
	color2.red=1.0;
    color2.green=0.0;
	color2.blue=0.0;
	color2.alpha=200;
	color2.upper_r=color2.red;
	color2.upper_g=color2.green;
	color2.upper_b=color2.blue;
	color2.lower_r=1;
	color2.lower_g=0;
	color2.lower_b=0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ORANGE(struct blendcolor &color2)
{
	color2.red=1.0;
    color2.green=0.7f;
	color2.blue=0.0;
	color2.alpha=200;
	color2.upper_r=color2.red;
	color2.upper_g=color2.green;
	color2.upper_b=color2.blue;
	color2.lower_r=1.0;
	color2.lower_g=0.7f;
	color2.lower_b=0.0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void YELLOW(struct blendcolor &color2)
{
	color2.red=1.0;
    color2.green=1.0;
	color2.blue=0.0;
	color2.alpha=200;
	color2.upper_r=color2.red;
	color2.upper_g=color2.green;
	color2.upper_b=color2.blue;
	color2.lower_r=1;
	color2.lower_g=1;
	color2.lower_b=0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LIGHTGREEN(struct blendcolor &color2)
{
	color2.red=0.0;
    color2.green=1.0;
	color2.blue=0.0;
	color2.alpha=200;
	color2.upper_r=color2.red;
	color2.upper_g=color2.green;
	color2.upper_b=color2.blue;
	color2.lower_r=0;
	color2.lower_g=1;
	color2.lower_b=0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GREEN(struct blendcolor &color2)
{
	color2.red=0.0;
    color2.green=0.7f;
	color2.blue=0.1f;
	color2.alpha=200;
	color2.upper_r=color2.red;
	color2.upper_g=color2.green;
	color2.upper_b=color2.blue;
	color2.lower_r=0;
	color2.lower_g=0.7f;
	color2.lower_b=0.1f;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void BLACK(struct blendcolor &color2)
{
	color2.red=0.0;
    color2.green=0.0;
	color2.blue=0.0;
	color2.alpha=200;
	color2.upper_r=color2.red;
	color2.upper_g=color2.green;
	color2.upper_b=color2.blue;
	color2.lower_r=0;
	color2.lower_g=0;
	color2.lower_b=0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct blendcolor* InitBlendColors(int av_type, struct blendcolor* color2)
{
	//struct blendcolor color2[10];

	switch(av_type)
	{
		case 1:{
			RED(color2[0]);
			BLACK(color2[1]);
			BLACK(color2[2]);
			BLACK(color2[3]);
			BLACK(color2[4]);
			BLACK(color2[5]);
			BLACK(color2[6]);
			BLACK(color2[7]);
			BLACK(color2[8]);
			BLACK(color2[9]);
		}break;
		case 2:{
			RED(color2[0]);
			RED(color2[1]);
			BLACK(color2[2]);
			BLACK(color2[3]);
			BLACK(color2[4]);
			BLACK(color2[5]);
			BLACK(color2[6]);
			BLACK(color2[7]);
			BLACK(color2[8]);
			BLACK(color2[9]);
		}break;
		case 3:{
	        RED(color2[0]);
			RED(color2[1]);
			ORANGE(color2[2]);
			BLACK(color2[3]);
			BLACK(color2[4]);
			BLACK(color2[5]);
			BLACK(color2[6]);
			BLACK(color2[7]);
			BLACK(color2[8]);
			BLACK(color2[9]);
		}break;
		case 4:{
		    RED(color2[0]);
			RED(color2[1]);
			ORANGE(color2[2]);
			YELLOW(color2[3]);
			BLACK(color2[4]);
			BLACK(color2[5]);
			BLACK(color2[6]);
			BLACK(color2[7]);
			BLACK(color2[8]);
			BLACK(color2[9]);
		}break;
	    case 5:{
	        RED(color2[0]);
			RED(color2[1]);
			ORANGE(color2[2]);
			YELLOW(color2[3]);
			YELLOW(color2[4]);
			BLACK(color2[5]);
			BLACK(color2[6]);
			BLACK(color2[7]);
			BLACK(color2[8]);
			BLACK(color2[9]);
		}break;
		case 6:{
		    RED(color2[0]);
			RED(color2[1]);
			ORANGE(color2[2]);
			YELLOW(color2[3]);
			YELLOW(color2[4]);
			LIGHTGREEN(color2[5]);
			BLACK(color2[6]);
			BLACK(color2[7]);
			BLACK(color2[8]);
			BLACK(color2[9]);
		}break;
		case 7:{
		    RED(color2[0]);
			RED(color2[1]);
			ORANGE(color2[2]);
			YELLOW(color2[3]);
			YELLOW(color2[4]);
			LIGHTGREEN(color2[5]);
			LIGHTGREEN(color2[6]);
			BLACK(color2[7]);
			BLACK(color2[8]);
			BLACK(color2[9]);
		}break;
		case 8:{
		    RED(color2[0]);
			RED(color2[1]);
			ORANGE(color2[2]);
			YELLOW(color2[3]);
			YELLOW(color2[4]);
			LIGHTGREEN(color2[5]);
			LIGHTGREEN(color2[6]);
			GREEN(color2[7]);
			BLACK(color2[8]);
			BLACK(color2[9]);
		}break;
		case 9:{
		    RED(color2[0]);
			RED(color2[1]);
			ORANGE(color2[2]);
			YELLOW(color2[3]);
			YELLOW(color2[4]);
			LIGHTGREEN(color2[5]);
			LIGHTGREEN(color2[6]);
			GREEN(color2[7]);
			GREEN(color2[8]);
			BLACK(color2[9]);
		}break;
		case 10:{
		    RED(color2[0]);
			RED(color2[1]);
			ORANGE(color2[2]);
			YELLOW(color2[3]);
			YELLOW(color2[4]);
			LIGHTGREEN(color2[5]);
			LIGHTGREEN(color2[6]);
			GREEN(color2[7]);
			GREEN(color2[8]);
			GREEN(color2[9]);
		}break;
		default:{
		    BLACK(color2[0]);
			BLACK(color2[1]);
			BLACK(color2[2]);
			BLACK(color2[3]);
			BLACK(color2[4]);
			BLACK(color2[5]);
			BLACK(color2[6]);
			BLACK(color2[7]);
			BLACK(color2[8]);
			BLACK(color2[9]);
		}break;
	}
	return color2;
}

struct trapezoid_points DrawDiagonal1(int x, int y, int w, int len, int box, struct rgba color1)
{
	struct trapezoid_points tpoints;
	int c=0;
	while(c<=len){

		if(c==0)
		{
			tpoints.p1_x=x-c;
			tpoints.p1_y=y+c;
			tpoints.p2_x=x+w-c;
			tpoints.p2_y=y+c;
		}

		if(c==len)
		{
			tpoints.p3_x=x-c;
			tpoints.p3_y=y+c;
			tpoints.p4_x=x+w-c;
			tpoints.p4_y=y+c;
		}

		oglSubtractive = true;
		FillRGBA( x-c,     y+c,      box,   box, color1.r,color1.g,color1.b,color1.a);
		oglSubtractive = false;
		c++;
	}
	return tpoints;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct trapezoid_points DrawDiagonal2(int x, int y, int w, int h, int len, int box, struct rgba color1, struct rgba color2)
{
	struct trapezoid_points tpoints;
	int c=0;
	while(c<=len){

		if(c!=0 && c != len)
		   FillRGBA( x+c,     y-c,       box,    box-h-1, color2.r, color2.g, color2.b, color2.a);

		if(c==0)
		{
			tpoints.p1_x=x+c;
			tpoints.p1_y=y-c;
			tpoints.p2_x=x+w+c;
			tpoints.p2_y=y-c;
		}

		if(c==len)
		{
			tpoints.p3_x=x+c;
			tpoints.p3_y=y-c;
			tpoints.p4_x=x+w+c;
			tpoints.p4_y=y-c;
		}

		oglSubtractive = true;
		FillRGBA( x+c,     y-c,      box,   box, color1.r,color1.g,color1.b,color1.a);
		oglSubtractive = false;
		c++;
	}
	return tpoints;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct trapezoid_points DrawDiagonal3(int x, int y, int w, int h, int len, int box, struct rgba color1, struct rgba color2)
{
	struct trapezoid_points tpoints;
	int c=0;
	while(c<=len){

		if(c!=0 && c != len)
		   FillRGBA( x+c,     y-c,       box+c,    box+c, color2.r, color2.g, color2.b, color2.a);

		if(c==0)
		{
			tpoints.p1_x=x+c;
			tpoints.p1_y=y-c;
			tpoints.p2_x=x+w+c;
			tpoints.p2_y=y-c;
		}

		if(c==len)
		{
			tpoints.p3_x=x+c;
			tpoints.p3_y=y-c;
			tpoints.p4_x=x+w+c;
			tpoints.p4_y=y-c;
		}

		oglSubtractive = true;
		FillRGBA( x+c,     y-c,      box,   box, color1.r,color1.g,color1.b,color1.a);
		oglSubtractive = false;
		c++;
	}
	return tpoints;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct trapezoid_points DrawCubeBox1(int x, int y, int w, int h, int box, int len, struct rgba color1, struct rgba color2)
{
	struct trapezoid_points tpoints1;
	struct trapezoid_points tpoints2;

	oglSubtractive = true;
	FillRGBA( x+len+1,       y-len,         w-1,    box, color1.r, color1.g, color1.b, color1.a);
	oglSubtractive = false;

	int i=0;
	while(i<=len)
	{
		if(i>=(len-1));
		else
		{
			FillRGBA( x+i+2,       y-i-1,         w-1,      box, color2.r, color2.g, color2.b, color2.a);//relleno
		}

		if(i==0)
		{
			tpoints1.p1_x=x+i;
			tpoints1.p1_y=y-i;
			tpoints1.p2_x=x+w+i;
			tpoints1.p2_y=y-i;
		}

		if(i==len)
		{
			tpoints1.p3_x=x+i;
			tpoints1.p3_y=y-i;
			tpoints1.p4_x=x+w+i;
			tpoints1.p4_y=y-i;
		}

		oglSubtractive = true;
		FillRGBA( x+i,       y-i,         box,    box, color1.r, color1.g, color1.b, color1.a);  //raya derecha
		FillRGBA( x+w+i,     y-i,         box,    box, color1.r, color1.g, color1.b, color1.a);  //raya izquierda
		oglSubtractive = false;
		i++;
	}

	oglSubtractive = true;
	int y_offs = h-1;
	FillRGBA( x+1,       y,         w-1,    box, color1.r, color1.g, color1.b, color1.a);
	oglSubtractive = false;

	/////////////////////

	oglSubtractive = true;
	FillRGBA( tpoints1.p1_x,       tpoints1.p1_y+1,         box,    h, color1.r, color1.g, color1.b, color1.a);/*| izquierda*/
	FillRGBA( tpoints1.p1_x+1,       tpoints1.p1_y+1+h,       w-1,    box, color1.r, color1.g, color1.b, color1.a);/*-- base*/
	FillRGBA( tpoints1.p1_x+w,       tpoints1.p1_y+1,       box,    h, color1.r, color1.g, color1.b, color1.a);/*| derecha*/
	oglSubtractive = false;

	FillRGBA( tpoints1.p1_x+1,       tpoints1.p1_y+1,       w-1,    h+1, color2.r, color2.g, color2.b, color2.a);//relleno

	tpoints1.p5_x=tpoints1.p1_x;
	tpoints1.p5_y=tpoints1.p1_y+h+1;
	tpoints1.p6_x=tpoints1.p1_x+w;
	tpoints1.p6_y=tpoints1.p1_y+h+1;

	////////////////////

	tpoints2 = DrawDiagonal2(tpoints1.p1_x+w, tpoints1.p1_y+1+h, w, h, len, box, color1, color2);

	oglSubtractive = true;
	FillRGBA( tpoints1.p4_x,       tpoints1.p4_y+1,       box,    h, color1.r, color1.g, color1.b, color1.a);/*| derecha exterior*/
	oglSubtractive = false;

	tpoints1.p7_x=tpoints1.p4_x;
	tpoints1.p7_y=tpoints1.p4_y+h+1;

	////////////////////

	return tpoints1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct trapezoid_points DrawCubeBox2(int x, int y, int w, int h, int box, int len, struct rgba color1, struct rgba color2)
{
	struct trapezoid_points tpoints1;
	struct trapezoid_points tpoints2;

	oglSubtractive = true;
	FillRGBA( x+len+1,       y-len,         w-1,    box, color1.r, color1.g, color1.b, color1.a);
	oglSubtractive = false;

	int i=0;
	while(i<=len)
	{
		if(i>=(len-1));
		else
		{
			FillRGBA( x+i+2,       y-i-1,         w-1,      box, color2.r, color2.g, color2.b, color2.a);//relleno
		}

		if(i==0)
		{
			tpoints1.p1_x=x+i;
			tpoints1.p1_y=y-i;
			tpoints1.p2_x=x+w+i;
			tpoints1.p2_y=y-i;
		}

		if(i==len)
		{
			tpoints1.p3_x=x+i;
			tpoints1.p3_y=y-i;
			tpoints1.p4_x=x+w+i;
			tpoints1.p4_y=y-i;
		}

		oglSubtractive = true;
		FillRGBA( x+i,       y-i,         box,    box, color1.r, color1.g, color1.b, color1.a);  //raya derecha
		FillRGBA( x+w+i,     y-i,         box,    box, color1.r, color1.g, color1.b, color1.a);  //raya izquierda
		oglSubtractive = false;
		i++;
	}

	oglSubtractive = true;
	int y_offs = h-1;
	FillRGBA( x+1,       y,         w-1,    box, color1.r, color1.g, color1.b, color1.a);
	oglSubtractive = false;

	/////////////////////

	oglSubtractive = true;
	FillRGBA( tpoints1.p1_x,       tpoints1.p1_y+1,         box,    h, color1.r, color1.g, color1.b, color1.a);/*| izquierda*/
	FillRGBA( tpoints1.p1_x+1,       tpoints1.p1_y+1+h,       w-1,    box, color1.r, color1.g, color1.b, color1.a);/*-- base*/
	FillRGBA( tpoints1.p1_x+w,       tpoints1.p1_y+1,       box,    h, color1.r, color1.g, color1.b, color1.a);/*| derecha*/
	oglSubtractive = false;

	//FillRGBA( tpoints1.p1_x+1,       tpoints1.p1_y+1,       w-1,    h+1, color2.r, color2.g, color2.b, color2.a);//relleno

	tpoints1.p5_x=tpoints1.p1_x;
	tpoints1.p5_y=tpoints1.p1_y+h+1;
	tpoints1.p6_x=tpoints1.p1_x+w;
	tpoints1.p6_y=tpoints1.p1_y+h+1;

	////////////////////

	tpoints2 = DrawDiagonal2(tpoints1.p1_x+w, tpoints1.p1_y+1+h, w, h, len, box, color1, color2);

	oglSubtractive = true;
	FillRGBA( tpoints1.p4_x,       tpoints1.p4_y+1,       box,    h, color1.r, color1.g, color1.b, color1.a);/*| derecha exterior*/
	oglSubtractive = false;

	tpoints1.p7_x=tpoints1.p4_x;
	tpoints1.p7_y=tpoints1.p4_y+h+1;

	////////////////////

	return tpoints1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawBox1(int x, int y, int w, int h, struct rgba color1, struct rgba color2)
{
	int box=1;
	oglSubtractive = true;
	FillRGBA( x,       y,       box,    h, color1.r, color1.g, color1.b, color1.a);/*| izquierda*/
	FillRGBA( x+1,     y,       w,    box, color1.r, color1.g, color1.b, color1.a);/*-- superior*/
	FillRGBA( x+w,     y+1,       box,    h, color1.r, color1.g, color1.b, color1.a);/*| derecha*/
	FillRGBA( x+1,     y+h,       w,    box, color1.r, color1.g, color1.b, color1.a);/*-- inferior*/

	FillRGBA( x+1,     y+1,       w-1,    h-1, color2.r, color2.g, color2.b, color2.a);//relleno
	oglSubtractive = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DrawBox2(int x, int y, int w, int h, struct rgba color1, struct blendcolor color2)
{
	int box=1;

	oglSubtractive = true;

	FillRGBA( x,       y,       box,    h, color1.r, color1.g, color1.b, color1.a);/*| izquierda*/
	FillRGBA( x+1,     y,       w,    box, color1.r, color1.g, color1.b, color1.a);/*-- superior*/
	FillRGBA( x+w,     y+1,       box,    h, color1.r, color1.g, color1.b, color1.a);/*| derecha*/
	FillRGBA( x+1,     y+h,       w,    box, color1.r, color1.g, color1.b, color1.a);/*-- inferior*/


	//FillRGBA( x+1,     y+1,       w-1,    h-1, color2.r, color2.g, color2.b, color2.a);//relleno

	Draw3DSmoothBox(x+1,     y+1,       w-1,    h-1, color2.upper_r,
																	   color2.upper_g,
																	   color2.upper_b,
																	   color2.lower_r,
																	   color2.lower_g,
																	   color2.lower_b,
																	   color2.alpha
	);

	oglSubtractive = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PrintWithFont(int x, int y, int r, int g, int b, const char *fmt, ... )
{
	va_list va_alist;
	char buf[256];

	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);
	gFont.drawString(false, x, y, r, g, b ,buf);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void HudTower1(int x, int y, int w, int h, int av, /*const*/ char* str)
{
	sMe& l = g_Local;

	if(!(l.HudAlive || l.MsgAlive))return;

	int box=1;
	int len=26;

	struct rgba color1;
//	color1.r=255;color1.g=255;color1.b=255;color1.a=255;
	color1.r=0;color1.g=60;color1.b=220;color1.a=230;
	struct rgba color2;
//	color2.r=0;color2.g=255;color2.b=0;color2.a=255;
	color2.r=0;color2.g=60;color2.b=220;color2.a=160;

	struct trapezoid_points tpoints1;
//	struct trapezoid_points tpoints2;
//	struct trapezoid_points tpoints3;
//	struct trapezoid_points tpoints4;

	/////////////////////////////////////////////////////////

	tpoints1=DrawCubeBox2(x,y,w,h,1,len,color1,color2);
//	PrintWithFont(tpoints1.p5_x,tpoints1.p5_y+2,0,255,255,str);
//	DrawConString(tpoints1.p5_x,tpoints1.p5_y+2,0,255,255,str);

	int r=10;
	gFont.drawString( true, tpoints1.p5_x+r,tpoints1.p5_y+2,0,255,255,str);


	struct rgba color3;
	color3.r=255;
	color3.g=255;
	color3.b=255;
	color3.a=255;
	///////////////////////
	//struct rgba color4;

	int av_type=av;

	//////////////////////////////////////////////
	struct blendcolor* color4=(struct blendcolor*)malloc(sizeof(struct blendcolor)*10);
	//struct blendcolor* color44=(struct blendcolor*)malloc(sizeof(struct blendcolor)*10);
	color4=InitBlendColors(av_type,color4);
	//////////////////////////////////////////////

	/////////////////////////

	int off=10;
	//for(int i=0; i<10; i++) DrawBox(tpoints1.p1_x+2, tpoints1.p1_y+(off*i)+2, 20, 10,color3,color4);

	/////////////////////////////////////////////////////////
	
	DrawBox2(tpoints1.p1_x+2, tpoints1.p1_y+(off*0)+2, 20, 10,color3,color4[9]);
	DrawBox2(tpoints1.p1_x+2, tpoints1.p1_y+(off*1)+2, 20, 10,color3,color4[8]);
	DrawBox2(tpoints1.p1_x+2, tpoints1.p1_y+(off*2)+2, 20, 10,color3,color4[7]);
	DrawBox2(tpoints1.p1_x+2, tpoints1.p1_y+(off*3)+2, 20, 10,color3,color4[6]);
	DrawBox2(tpoints1.p1_x+2, tpoints1.p1_y+(off*4)+2, 20, 10,color3,color4[5]);
	DrawBox2(tpoints1.p1_x+2, tpoints1.p1_y+(off*5)+2, 20, 10,color3,color4[4]);
	DrawBox2(tpoints1.p1_x+2, tpoints1.p1_y+(off*6)+2, 20, 10,color3,color4[3]);
	DrawBox2(tpoints1.p1_x+2, tpoints1.p1_y+(off*7)+2, 20, 10,color3,color4[2]);
	DrawBox2(tpoints1.p1_x+2, tpoints1.p1_y+(off*8)+2, 20, 10,color3,color4[1]);
	DrawBox2(tpoints1.p1_x+2, tpoints1.p1_y+(off*9)+2, 20, 10,color3,color4[0]);

	/////////////////////////////////////////////////////////
	free(color4);
	//free(color44);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void HudTower2(int x, int y, int w, int h, int av, /*const*/ char* str)
{
	sMe& l = g_Local;

	if(!(l.HudAlive || l.MsgAlive))return;

	int box=1;
	int len=26;

	struct rgba color1;
//	color1.r=255;color1.g=255;color1.b=255;color1.a=255;
	color1.r=0;color1.g=60;color1.b=220;color1.a=230;
	struct rgba color2;
//	color2.r=0;color2.g=255;color2.b=0;color2.a=255;
	color2.r=0;color2.g=60;color2.b=220;color2.a=160;

	struct trapezoid_points tpoints1;
//	struct trapezoid_points tpoints2;
//	struct trapezoid_points tpoints3;
//	struct trapezoid_points tpoints4;

	/////////////////////////////////////////////////////////

	tpoints1=DrawCubeBox2(x,y,w,h,1,len,color1,color2);
//	PrintWithFont(tpoints1.p5_x,tpoints1.p5_y+2,0,255,0,str);
//	DrawConString(tpoints1.p5_x,tpoints1.p5_y+2,0,255,0,str);

	int r=10;
	gFont.drawString( true, tpoints1.p5_x+r,tpoints1.p5_y+2,0,255,0,str);


	struct rgba color3;
	color3.r=255;
	color3.g=255;
	color3.b=255;
	color3.a=255;
	///////////////////////
	//struct rgba color4;

	int av_type=av;

	//////////////////////////////////////////////
	struct blendcolor* color4=(struct blendcolor*)malloc(sizeof(struct blendcolor)*10);
	//struct blendcolor* color44=(struct blendcolor*)malloc(sizeof(struct blendcolor)*10);
	color4=InitBlendColors(av_type,color4);
	//////////////////////////////////////////////

	/////////////////////////

	int off=20;
	//for(int i=0; i<10; i++) DrawBox(tpoints1.p1_x+(off*i)+2, tpoints1.p1_y+2, 20, 10,color3,color4);

	DrawBox2(tpoints1.p1_x+(off*0)+2, tpoints1.p1_y+2, 20, 10,color3,color4[0]);
	DrawBox2(tpoints1.p1_x+(off*1)+2, tpoints1.p1_y+2, 20, 10,color3,color4[1]);
	DrawBox2(tpoints1.p1_x+(off*2)+2, tpoints1.p1_y+2, 20, 10,color3,color4[2]);
	DrawBox2(tpoints1.p1_x+(off*3)+2, tpoints1.p1_y+2, 20, 10,color3,color4[3]);
	DrawBox2(tpoints1.p1_x+(off*4)+2, tpoints1.p1_y+2, 20, 10,color3,color4[4]);
	DrawBox2(tpoints1.p1_x+(off*5)+2, tpoints1.p1_y+2, 20, 10,color3,color4[5]);
	DrawBox2(tpoints1.p1_x+(off*6)+2, tpoints1.p1_y+2, 20, 10,color3,color4[6]);
	DrawBox2(tpoints1.p1_x+(off*7)+2, tpoints1.p1_y+2, 20, 10,color3,color4[7]);
	DrawBox2(tpoints1.p1_x+(off*8)+2, tpoints1.p1_y+2, 20, 10,color3,color4[8]);
	DrawBox2(tpoints1.p1_x+(off*9)+2, tpoints1.p1_y+2, 20, 10,color3,color4[9]);

	/////////////////////////////////////////////////////////
	free(color4);
	//free(color44);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

