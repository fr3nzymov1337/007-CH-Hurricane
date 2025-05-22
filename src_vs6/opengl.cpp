#include "client.h"

typedef void (APIENTRY *glBegin_t)(GLenum);
typedef void (APIENTRY *glVertex3f_t)(GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY *glVertex3fv_t)(const GLfloat *v);
typedef void (APIENTRY *glColor4f_t)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef BOOL (APIENTRY *wglSwapBuffers_t)(HDC  hdc);
typedef void (APIENTRY *glClear_t)(GLbitfield mask);
typedef void (APIENTRY *glViewport_t)(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (APIENTRY *glBlendFunc_t)(GLenum,GLenum);
typedef void (APIENTRY *glFrustum_t)(GLdouble left,GLdouble right,GLdouble bottom,GLdouble top,GLdouble zNear,GLdouble zFar);

glBegin_t pglBegin = NULL;
glVertex3f_t pglVertex3f = NULL;
glVertex3fv_t pglVertex3fv = NULL;
glColor4f_t pglColor4f = NULL;
wglSwapBuffers_t pwglSwapBuffers = NULL;
glClear_t pglClear = NULL;
glViewport_t pglViewport = NULL;
glBlendFunc_t pglBlendFunc = NULL;
glFrustum_t pglFrustum = NULL;

bool bDrawingSky = false;
bool oglSubtractive = false;

void opengl_ClearBackground()
{
	if(&glClearColor && &glClear)
	{
		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}

void APIENTRY Hooked_glBlendFunc(GLenum sfactor, GLenum dfactor)
{
	(*pglBlendFunc)(sfactor,dfactor);
}

void APIENTRY Hooked_glBegin(GLenum mode)
{
	cl_entity_s *pEnt=g_Studio.GetCurrentEntity();

	if(DrawVisuals && !cvar.rush)
	{
		if(pEnt && pEnt->player && pEnt->curstate.solid || pEnt && !pEnt->player && pEnt->model && pEnt->model->name && strstr(pEnt->model->name , "w_" ))
			if(mode == GL_TRIANGLE_STRIP || mode == GL_TRIANGLE_FAN) { if(cvar.misc_wall)glDepthRange(0, 0.5); }
		else
			if(cvar.misc_wall)glDisable(GL_DEPTH_TEST);
	}

	if(cvar.misc_nightmode == 1 && mode == GL_POLYGON) { glColor4f(0.35f, 0.35f, 0.35f, 0.35f);	}

	if(cvar.misc_wirewall == 1 && mode == GL_POLYGON)
	{
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		glLineWidth(1);
		glColor3f(cvar.color_red, cvar.color_green, cvar.color_blue);
	}
	else
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	if (mode == GL_QUADS && cvar.misc_nosky == 1)
		bDrawingSky=true;
	else
		bDrawingSky=false;

	(*pglBegin)(mode);
}

void APIENTRY Hooked_glVertex3f(GLfloat x,GLfloat y,GLfloat z)
{
	if(cvar.misc_nohud)
		if(cvar.rush)
			return;

	if(cvar.misc_lambert == 1) 
		glColor3f(1.0f,1.0f,1.0f);
	else if(cvar.misc_lambert == 2) 
		glColor3f(0.0f,0.0f,0.0f);

	(*pglVertex3f)(x, y, z);
}

void APIENTRY Hooked_glVertex3fv(GLfloat *v)
{
	if (bDrawingSky==true && cvar.misc_nosky == 1 && v[2]>3000)
		return;

	(*pglVertex3fv)(v);
}

void APIENTRY Hooked_glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	(*pglColor4f)(red, green, blue, alpha);
}

BOOL APIENTRY Hooked_wglSwapBuffers(HDC hdc)
{
	return(*pwglSwapBuffers)(hdc);
}

void APIENTRY Hooked_glClear(GLbitfield mask) 
{
	if ((mask==GL_DEPTH_BUFFER_BIT) && cvar.misc_nosky == 1)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
	}

	(*pglClear)(mask);
}

void APIENTRY Hooked_glFrustum(GLdouble left,GLdouble right,GLdouble bottom,GLdouble top,GLdouble zNear,GLdouble zFar)
{
	(*pglFrustum)(left,right,bottom,top,zNear,zFar);
}

void APIENTRY Hooked_glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	static bool FirstFrame = true;
	if (FirstFrame)
	{
		if(!g_Engine.Con_IsVisible())
			g_Engine.pfnClientCmd("toggleconsole");

		g_Engine.Con_Printf("#\t\t\t\t\t\t\t 0000    0000   777777\n" );
		g_Engine.Con_Printf("#\t\t\t\t\t\t\t00  00  00  00        77\n" );
		g_Engine.Con_Printf("#\t\t\t\t\t\t\t00  00  00  00       77\n" );
		g_Engine.Con_Printf("#\t\t\t\t\t\t\t00  00  00  00      77\n" );
		g_Engine.Con_Printf("#\t\t\t\t\t\t\t 0000    0000      77\n\n" );

		g_Engine.Con_Printf("#\t\t\t\t\t[007 Client Hook V3 - Hurricane]\n\n" );

		g_Engine.Con_Printf("#\t\t\t\t\t\t\t\tJ = Cheat Mode\n" );
		g_Engine.Con_Printf("#\t\t\t\t\t\t\t\tINS = Cheat Menu\n" );
		g_Engine.Con_Printf("#\t\t\t\t\t\t\t\tDEL = Cheat Console\n\n" );

		g_Engine.Con_Printf("#\t\t\t\t\tCompiled by: x F R 3 N Z Y M O V x\n" );

		ExcellentSettings();

		FirstFrame = false;
	}

	(*pglViewport)(x, y, width, height);
}


void HookOpenGL()
{
	HMODULE hmOpenGL = GetModuleHandle ("opengl32.dll");
	pglBegin = (glBegin_t)DetourFunction((LPBYTE)GetProcAddress(hmOpenGL, "glBegin"), (LPBYTE)&Hooked_glBegin);
	pglColor4f = (glColor4f_t)DetourFunction((LPBYTE)GetProcAddress(hmOpenGL, "glColor4f"), (LPBYTE)&Hooked_glColor4f);
	pglVertex3f = (glVertex3f_t)DetourFunction((LPBYTE)GetProcAddress(hmOpenGL, "glVertex3f"), (LPBYTE)&Hooked_glVertex3f);
	pglVertex3fv = (glVertex3fv_t)DetourFunction((LPBYTE)GetProcAddress(hmOpenGL, "glVertex3fv"), (LPBYTE)&Hooked_glVertex3fv);
	pglClear = (glClear_t)DetourFunction((LPBYTE)GetProcAddress(hmOpenGL, "glClear"), (LPBYTE)&Hooked_glClear);
	pwglSwapBuffers = (wglSwapBuffers_t)DetourFunction((LPBYTE)GetProcAddress(hmOpenGL, "wglSwapBuffers"), (LPBYTE)&Hooked_wglSwapBuffers);
	pglViewport = (glViewport_t)DetourFunction((LPBYTE)GetProcAddress(hmOpenGL, "glViewport"), (LPBYTE)&Hooked_glViewport);
	pglBlendFunc = (glBlendFunc_t)DetourFunction((LPBYTE)GetProcAddress(hmOpenGL, "glBlendFunc"), (LPBYTE)&Hooked_glBlendFunc);
	pglFrustum = (glFrustum_t)DetourFunction((LPBYTE)GetProcAddress(hmOpenGL, "glFrustum"), (LPBYTE)&Hooked_glFrustum);
}
