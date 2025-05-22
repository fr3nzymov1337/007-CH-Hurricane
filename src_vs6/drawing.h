#ifndef __DRAWING__
#define __DRAWING__

extern bool WorldToScreen(float *pflOrigin, float *pflVecScreen);
extern void DrawString(int x, int y, int r, int g, int b, const char *fmt, ...);
extern void DrawHudStringCenter(int x, int y, int r, int g, int b, const char *fmt, ...);
extern void GetScreenInfo();
extern void DrawHudStringCenterEsp (int x, int y, int r, int g, int b, float back, float tint, const char *fmt, ... );
extern void FillRGBA(GLfloat x, GLfloat y, int w, int h, UCHAR r, UCHAR g, UCHAR b, UCHAR a);
extern void FillArea(float x, float y, int w, int h, int r, int g, int b, int a);
extern void blackBorder(int x,int y,int w, int h, int tint);
extern void Draw3DSmoothBoxAndSmoothHeaderWithFlatBorderRdar(int x, int y, int bw, int bh, int hh, float ubr, float ubg, float ubb, float lbr, float lbg, float lbb, float ba, float uhr, float uhg, float uhb, float lhr, float lhg, float lhb, float ha, float bor, float bog, float bob, float boa, int line);
extern void Draw3DSmoothBoxAndSmoothHeaderWithFlatBorder(int x, int y, int bw, int bh, int hh, float ubr, float ubg, float ubb, float lbr, float lbg, float lbb, float ba, float uhr, float uhg, float uhb, float lhr, float lhg, float lhb, float ha, float bor, float bog, float bob, float boa, int line);
extern void DrawConString (int x, int y, int r, int g, int b, const char *fmt, ... );
extern void DrawGlowHudString(int x, int y, int r, int g, int b, const char* fmt, ... );
extern void BorderMenu(int x,int y,int w, int h,int r,int g,int b,int a);
extern void DrawBoxAtScreenXY( int x, int y, int r, int g, int b, int alpha, int radius);
extern void tintArea(int x,int y,int w,int h,int r,int g,int b,int a);
extern void DrawStippledVectorLine(float *flSrc,float *flDestination,int width,GLint factor,GLushort pattern,ColorEntry *color);
extern void DrawStippledVectorLine(float *flSrc,float *flDestination, int width, GLint factor, GLushort pattern, int r, int g, int b, int tint);
extern int iStringLen( const char *fmt, ... );
extern int iStringHeight( void );
extern void gDrawFilledBoxAtLocation( float* origin, int r, int g, int b, int a, int radius);
extern void DrawLines(int x1,int y1,int x2,int y2,int width,ColorEntry *color,int tint);
extern void DrawLines(int x1,int y1,int x2,int y2,int width,int r,int g,int b,int tint);
extern void DrawVectorLine(float *flSrc,float *flDestination,int width,ColorEntry *color,int tint);
extern void Box(int x, int y, int w);
extern void RadarBox(int x, int y, int w);
extern void BeamDrawLine(float* from, float* to);

extern SCREENINFO g_Screen;

#endif