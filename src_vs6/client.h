#ifndef __CLIENT__
#define __CLIENT__

#pragma once

#pragma warning(disable:4800)
#pragma warning(disable:4715)
#pragma warning(disable:4410)
#pragma warning(disable:4409)

#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <cstdlib>
#include <ctime>
#include <crtdbg.h>
#include <map>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <windows.h>
#include <mmsystem.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <tlhelp32.h>
#include <iostream>

using namespace std;

extern char hackdir [256];

#include "ValveSDK\common\interface.h"
#include "ValveSDK\VGUI\VGUI_Frame.h"
#include "ValveSDK\VGUI\VGUI_Panel.h"
#include "ValveSDK\GameUI\IVGuiModuleLoader.h"
#include "ValveSDK\GameUI\IRunGameEngine.h"
#include "ValveSDK\GameUI\IGameUI.h"
#include "ValveSDK\GameUI\IGameConsole.h"
#include "ValveSDK\VGUI2\IHTML.h"
#include "ValveSDK\VGUI2\IPanel.h"
#include "ValveSDK\VGUI2\ISurface.h"
#include "ValveSDK\VGUI2\IEngineVGui.h"

#include "SDK\MISC\PARSEMSG.H"
#include "SDK\engine\wrect.h"
#include "SDK\engine\cl_dll.h"
#include "SDK\engine\cdll_int.h"
#include "SDK\engine\const.h"
#include "SDK\engine\entity_types.h"
#include "SDK\engine\pmtrace.h"
#include "SDK\engine\pm_defs.h"
#include "SDK\engine\pm_movevars.h"
#include "SDK\engine\r_efx.h"
#include "SDK\engine\studio.h"
#include "SDK\engine\studio_event.h"
#include "SDK\engine\triangleapi.h"
#include "SDK\engine\eiface.h"
#include "SDK\engine\util_vector.h"
#include "SDK\common\com_model.h"
#include "SDK\common\event_api.h"
#include "SDK\common\net_api.h"
#include "SDK\common\ref_params.h"
#include "SDK\common\screenfade.h"
#include "SDK\misc\r_studioint.h"
#include "SDK\misc\sprites.h"
#include "SDK\engine\pm_materials.h"

#include "detours.h"
#include "opengl.h"
#include "usermsg.h"
#include "offset.h"
#include "soundesp.h"
#include "recoil.h"
#include "player.h"
#include "color.h"
#include "drawing.h"
#include "esp.h"
#include "cvar.h"
#include "aimsilent.h"
#include "menu.h"
#include "snapshot.h"
#include "radar.h"
#include "strafe.h"
#include "stringfinder.h"
#include "interpreter.h"
#include "functions.h"
#include "idhook.h"
#include "time.h"
#include "console.h"
#include "bind.h"
#include "hudmessage.h"
#include "ChineseCodeLib.h"
#include "Chat.h"
#include "attack.h"
#include "WPT/autoadjust.h"
#include "WPT/waypoint.h"
#include "defs.h"
#include "trace.h"
#include "font.h"

extern void HookFunction();
extern void AtRoundStart(void);
extern void CL_Move();
extern void bSendpacket(bool status);
extern void AdjustSpeed(double speed);

#define POW(x) ((x)*(x))
#define VectorDistance(a,b) sqrt(POW((a)[0]-(b)[0])+POW((a)[1]-(b)[1])+POW((a)[2]-(b)[2]))
#define SoundCopy(a,origin) {a.vOrigin=origin; a.dwTime=GetTickCount();}
#define SoundClear(a) {a.vOrigin[0]=0; a.vOrigin[1]=0; a.vOrigin[2]=0; a.dwTime=0;}
#define M_PI 3.14159265358979323846f
#define VectorTransform(a,b,c){(c)[0]=DotProduct((a),(b)[0])+(b)[0][3];(c)[1]=DotProduct((a),(b)[1])+(b)[1][3];(c)[2]=DotProduct((a),(b)[2])+(b)[2][3];}
#define VectorLength(a) sqrt(POW((a)[0])+POW((a)[1])+POW((a)[2]))

typedef struct cl_clientfuncs_s
{
	int(*Initialize) (cl_enginefunc_t *pEnginefuncs, int iVersion);
	int(*HUD_Init) (void);
	int(*HUD_VidInit) (void);
	void(*HUD_Redraw) (float time, int intermission);
	int(*HUD_UpdateClientData) (client_data_t *pcldata, float flTime);
	int(*HUD_Reset) (void);
	void(*HUD_PlayerMove) (struct playermove_s *ppmove, int server);
	void(*HUD_PlayerMoveInit) (struct playermove_s *ppmove);
	char(*HUD_PlayerMoveTexture) (char *name);
	void(*IN_ActivateMouse) (void);
	void(*IN_DeactivateMouse) (void);
	void(*IN_MouseEvent) (int mstate);
	void(*IN_ClearStates) (void);
	void(*IN_Accumulate) (void);
	void(*CL_CreateMove) (float frametime, struct usercmd_s *cmd, int active);
	int(*CL_IsThirdPerson) (void);
	void(*CL_CameraOffset) (float *ofs);
	struct kbutton_s *(*KB_Find) (const char *name);
	void(*CAM_Think) (void);
	void(*V_CalcRefdef) (struct ref_params_s *pparams);
	int(*HUD_AddEntity) (int type, struct cl_entity_s *ent, const char *modelname);
	void(*HUD_CreateEntities) (void);
	void(*HUD_DrawNormalTriangles) (void);
	void(*HUD_DrawTransparentTriangles) (void);
	void(*HUD_StudioEvent) (const struct mstudioevent_s *event, const struct cl_entity_s *entity);
	void(*HUD_PostRunCmd) (struct local_state_s *from, struct local_state_s *to, struct usercmd_s *cmd, int runfuncs, double time, unsigned int random_seed);
	void(*HUD_Shutdown) (void);
	void(*HUD_TxferLocalOverrides) (struct entity_state_s *state, const struct clientdata_s *client);
	void(*HUD_ProcessPlayerState) (struct entity_state_s *dst, const struct entity_state_s *src);
	void(*HUD_TxferPredictionData) (struct entity_state_s *ps, const struct entity_state_s *pps, struct clientdata_s *pcd, const struct clientdata_s *ppcd, struct weapon_data_s *wd, const struct weapon_data_s *pwd);
	void(*Demo_ReadBuffer) (int size, unsigned char *buffer);
	int(*HUD_ConnectionlessPacket) (struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size);
	int(*HUD_GetHullBounds) (int hullnumber, float *mins, float *maxs);
	void(*HUD_Frame) (double time);
	int(*HUD_Key_Event) (int down, int keynum, const char *pszCurrentBinding);
	void(*HUD_TempEntUpdate) (double frametime, double client_time, double cl_gravity, struct tempent_s **ppTempEntFree, struct tempent_s **ppTempEntActive, int(*Callback_AddVisibleEntity)(struct cl_entity_s *pEntity), void(*Callback_TempEntPlaySound)(struct tempent_s *pTemp, float damp));
	struct cl_entity_s *(*HUD_GetUserEntity) (int index);
	int(*HUD_VoiceStatus) (int entindex, qboolean bTalking);
	int(*HUD_DirectorMessage) (unsigned char command, unsigned int firstObject, unsigned int secondObject, unsigned int flags);
	int(*HUD_GetStudioModelInterface) (int version, struct r_studio_interface_s **ppinterface, struct engine_studio_api_s *pstudio);
	void(*HUD_CHATINPUTPOSITION_FUNCTION) (int *x, int *y);
	int(*HUD_GETPLAYERTEAM_FUNCTION) (int iplayer);
	void(*CLIENTFACTORY) (void);
} cl_clientfunc_t; 

extern cl_clientfunc_t *g_pClient;
extern cl_enginefunc_t *g_pEngine;
extern engine_studio_api_t *g_pStudio;
typedef void(*CL_Move_t)();
extern CL_Move_t CL_Move_s;

extern cl_clientfunc_t g_Client;
extern cl_enginefunc_t g_Engine;
extern engine_studio_api_t g_Studio;

#endif