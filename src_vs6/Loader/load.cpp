#include <windows.h>
#include <stdio.h>
#include <fstream>
#include <string>

#include "ForceLib.h"
#include "load.h"

#include "utils/WinlicenseSDK.h"
#include "crc.h"
#include "xorstr.h"

#pragma pack(1)

using namespace std;

CCrc32Static aCRCChecker;
HINSTANCE mainInstance;

#include <direct.h> // for home directory

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char ogcdir [256];
string getOgcDirFile(const char* basename)
{
	if(strstr(basename,"..")){ return ":*?\\/<>\""; }
	string ret = ogcdir;
	return (ret+basename);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool fileExists(const char* filename)
{
	WIN32_FIND_DATA finddata;
	HANDLE handle = FindFirstFile(filename,&finddata);
	return (handle!=INVALID_HANDLE_VALUE);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool promptForFile( char* retFile, const char* tip, char* filter, char* title, bool save, HWND hWnd )
{
              OPENFILENAME ofn;
              memset(&ofn, 0, sizeof(OPENFILENAME));
              ofn.lStructSize = sizeof(OPENFILENAME);
              ofn.hwndOwner = hWnd;
              ofn.hInstance = mainInstance;
              ofn.nFilterIndex = 1;
              ofn.lpstrFile = retFile;
              ofn.nMaxFile = 256;
              ofn.lpstrInitialDir = tip;
              //====
              ofn.lpstrFilter = filter;
              ofn.lpstrTitle  = title;

              strcpy( retFile, tip );

              bool status;
              if( save )
              {
                  ofn.Flags = OFN_OVERWRITEPROMPT;
                  status =  (GetSaveFileName(&ofn)!=0);
              } else
              {
                  ofn.Flags = OFN_FILEMUSTEXIST;
                  status = (GetOpenFileName(&ofn)!=0);
              }
              return status;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

STARTUPINFO          SI;
PROCESS_INFORMATION  PI;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void removeTrailingCRLF(char* a)
{
	while(a[strlen(a)-1]=='\x0D' || a[strlen(a)-1]=='\x0A' ) a[strlen(a)-1]=0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char HomeDirectory[512];
char dllname[512];

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nShowCmd)
{
	///////

	bool errorflag=false;

	/////////////

	char ERROR_DLL1[128];
	char ERROR_DLL2[128];
	char MSG_CS1[128];
	char ERROR_INI1[128];
	char ERROR_EXE1[128];
	char ERROR_EXE2[128];
	char ERROR_HOOK1[128];
	char ERROR_HOOK2[128];
	char ERROR_HOOK3[128];
	char ERROR_HOOK4[128];

	{
	   strcpy(ERROR_DLL1,"Please make a new copy");
	   strcpy(ERROR_DLL2,"DLL is missing");
	   strcpy(MSG_CS1,  "CS Directory" );
	   strcpy(ERROR_INI1, "ini write error");
	   strcpy(ERROR_EXE1, "Injection failed");
	   strcpy(ERROR_EXE2, "Incorrect");
	   strcpy(ERROR_HOOK1, "Injection failed");
	   strcpy(ERROR_HOOK2, "[GYJ+007 Hook]");
	   strcpy(ERROR_HOOK3, "Injection failed");
	   strcpy(ERROR_HOOK4, "[GYJ+007 Hook]");
	}

    GetModuleFileName(0,ogcdir,255);
	char* position = ogcdir+strlen(ogcdir);
	while(position>=ogcdir && *position!='\\') --position;
	position[1]=0;

	CODEREPLACE_START
	char dll_name[] = {'0','0','7','.','d','l','l',0};
	CODEREPLACE_END

	CODEREPLACE_START
	if(!fileExists(dll_name))
	{
		CODEREPLACE_START
		char szFailMsg[512] = { 0 };

		errorflag=true;

		sprintf( szFailMsg, "DLL is missing\n\n\"%s\"", dll_name );

		MessageBox( NULL, szFailMsg, ERROR_DLL1, MB_ICONERROR | MB_OK );
		CODEREPLACE_END
		return -1;
	}
	CODEREPLACE_END

	//////////////////////////////////////////

	mainInstance = hInstance;
	char hookEXE  [400] = ""; 
	char hookDLL  [400] = ""; 
	char hookINI  [400] = "";
	char hlCmdLine[400] = "";
	char hlEXE    [400] = "";

	////////

	///////////////
	
	GetModuleFileName(0,hookEXE,390); 

	//////////////////////////////////////////

	int  len = strlen(hookEXE);
	strcpy(hookDLL,hookEXE); 
	strcpy(hookINI,hookEXE); 
	hookDLL[len-3]='d';hookDLL[len-2]='l';hookDLL[len-1]='l';
	hookINI[len-3]='i';hookINI[len-2]='n';hookINI[len-1]='i';

	if( !fileExists(hookDLL) ) {

		errorflag=true;
		MessageBox(0,ERROR_DLL1,ERROR_DLL2,MB_ICONERROR|MB_TOPMOST);
		return 1;
	}

	///////

/*	if(quit)
	{
		//MessageBox(NULL,"Bah! quit",NULL,NULL);
		errorflag=true;
		DWORD add1;
		__asm mov ebx, ExitProcess
		__asm mov add1, ebx
		__asm push 0
		__asm call ebx
	}*/

	//////////////////

	bool success = false;
	do{
		FILE* file = fopen(hookINI,"r");
		if(file)
		{
			fgets(hlEXE    ,390,file);hlEXE[398]=0;     removeTrailingCRLF(hlEXE);
			fclose(file);
		}

		if (!fileExists(hlEXE))
		{
			if(hlEXE)
			{
			   bool ok = promptForFile( hlEXE, "hl.exe.cstrike.exe",
				                               "hl.exe or cstrike.exe\0hl.exe;cstrike.exe\0",
				                               MSG_CS1, false, 0);
				if(!ok) { return 0; }
			}
			
			file = fopen(hookINI,"w");
			if(file)
			{
				fprintf(file,"%s\x0D\x0A",hlEXE);
				fclose(file);
			} else {
				MessageBox(0,hookINI,ERROR_INI1,0); 
	
				break;
			}
			if(fileExists(hlEXE)) success = true;
		}
		else
		{
			success = true;
		}

	} while(!success);

	if( !strcmpi(hlEXE,hookEXE) )
	{
		errorflag=true;
		MessageBox(0,ERROR_EXE1,ERROR_EXE2,MB_ICONEXCLAMATION);
		DeleteFile(hookINI);
		return 1;
	}

	//if(!strstr(lpCmdLine,"-game")) strcpy(hlCmdLine," -game cstrike");
	if(!strstr(lpCmdLine,/*-game*/XorStr<0x88,6,0xE34DA52E>("\xA5\xEE\xEB\xE6\xE9"+0xE34DA52E).s)) strcpy(hlCmdLine,/* -game cstrike */XorStr<0x03,16,0x4820B276>("\x23\x29\x62\x67\x6A\x6D\x29\x69\x78\x78\x7F\x67\x64\x75\x31"+0x4820B276).s);

	strcat(hlCmdLine,lpCmdLine);
	ZeroMemory(&SI,sizeof(STARTUPINFO));
	ZeroMemory(&PI,sizeof(PROCESS_INFORMATION));
	SI.cb = sizeof(STARTUPINFO);
	char hlDir[400];
	strcpy(hlDir,hlEXE);
	char* pos = hlDir+strlen(hlDir);
	while(pos>=hlDir && *pos!='\\') --pos;
	*pos = 0;
	char* hlBaseFileName = pos+1;
	SetCurrentDirectory(hlDir);

	//////////////

	if(strcmp(dll_name,/*007.dll*/XorStr<0xEA,8,0xE8F1EC64>("\xDA\xDB\xDB\xC3\x8A\x83\x9C"+0xE8F1EC64).s))
	{
		//MessageBox(NULL,"Bah! 007.dll",NULL,NULL);
		errorflag=true;
		DWORD add1;
		__asm mov ebx, ExitProcess
		__asm mov add1, ebx
		__asm push 0
		__asm call ebx
	}

	//////////////

	//errorflag=true;

	if(errorflag)
	{
		//MessageBox(NULL,"Bah! errorflag",NULL,NULL);

		//HINSTANCE x;
		//DWORD add0;
		//char kernel32L[128];
		//strcpy(kernel32L,/*kernel32.dll*/XorStr<0x76,13,0xE6A58BDD>("\x1D\x12\x0A\x17\x1F\x17\x4F\x4F\x50\x1B\xEC\xED"+0xE6A58BDD).s);
		//__asm mov ebx, LoadLibrary
		//__asm mov add0, ebx
		//__asm push kernel32L
		//__asm call add0
		//__asm mov x, eax
		///
		DWORD add1;
		typedef void (__cdecl* ExitProcess_t)(int);
		ExitProcess_t pExitProcess = (ExitProcess_t)GetProcAddress(GetModuleHandle(/*kernel32.dll*/XorStr<0x76,13,0xE6A58BDD>("\x1D\x12\x0A\x17\x1F\x17\x4F\x4F\x50\x1B\xEC\xED"+0xE6A58BDD).s),/*ExitProcess*/XorStr<0xB1,12,0xE8027CB5>("\xF4\xCA\xDA\xC0\xE5\xC4\xD8\xDB\xDC\xC9\xC8"+0xE8027CB5).s);
		__asm mov ebx, pExitProcess
		__asm mov add1, ebx
		__asm push 0
		__asm call add1
	}

	////

/*	char dll_name2[] = {'0','0','7','.','d','l','l',0};
	char ogcdir [256]="";
	GetModuleFileName(hInstance,ogcdir,255);
	char* position2 = ogcdir+strlen(ogcdir);
	while(position2>=ogcdir && *position2!='\\') --position2;
	position2[1]=0;*/

	char filepath[400];
	sprintf(filepath,"%s\\%s",ogcdir,dll_name);

	///////////////////

	void ListOfNOPs();
	ListOfNOPs();

//	MessageBox(0,ogcdir,dll_name2,0);


	/////////

	if (!CreateProcess(hlBaseFileName,hlCmdLine,NULL,NULL,FALSE,CREATE_SUSPENDED,NULL,NULL,&SI,&PI))
	{
		MessageBox(0,ERROR_HOOK1,ERROR_HOOK2,MB_ICONERROR|MB_TOPMOST);
		return -1;
	}
	
	if (!ForceLibrary(hookDLL,&PI))
	//if( !bInjectLibrary(PI.hProcess, filepath ))
	{
		MessageBox(0,ERROR_HOOK3,ERROR_HOOK4,MB_ICONERROR|MB_TOPMOST);
		TerminateProcess(PI.hProcess,-1);
		return -1;
	}


	
	ResumeThread(PI.hThread);
	CloseHandle(PI.hProcess);
	CloseHandle(PI.hThread);

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ListOfNOPs()
{
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop

	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop

	//
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	//
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	//
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop

	//
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop

	////////

	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop

	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop

//	__asm nop __asm nop __asm nop __asm nop __asm nop
//	__asm nop __asm nop __asm nop __asm nop __asm nop

//	__asm nop __asm nop __asm nop __asm nop __asm nop
//	__asm nop __asm nop __asm nop __asm nop __asm nop

	///////////
	
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop

	////////

	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop

	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop

	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop

	////////

	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop

	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop

	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop

	////////

	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop

	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop

	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop

	////////

	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop

	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
	__asm nop __asm nop __asm nop __asm nop __asm nop
}