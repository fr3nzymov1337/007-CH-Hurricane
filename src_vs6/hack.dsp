# Microsoft Developer Studio Project File - Name="hack" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=hack - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "hack.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "hack.mak" CFG="hack - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "hack - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HACK_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HACK_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib winmm.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib opengl32.lib /nologo /dll /machine:I386 /nodefaultlib:"libc.lib" /out:"007\007.dll"
# SUBTRACT LINK32 /pdb:none /nodefaultlib
# Begin Target

# Name "hack - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\GUI\trapezoidal_console.cpp
# End Source File
# Begin Source File

SOURCE=.\GUI\trapezoidal_draw.cpp
# End Source File
# Begin Source File

SOURCE=.\GUI\trapezoidal_menu.cpp
# End Source File
# Begin Source File

SOURCE=.\WPT\autoadjust.cpp
# End Source File
# Begin Source File

SOURCE=.\WPT\autoadjust.h
# End Source File
# Begin Source File

SOURCE=.\WPT\waypoint.cpp
# End Source File
# Begin Source File

SOURCE=.\WPT\waypoint.h
# End Source File
# Begin Source File


SOURCE=.\aimsilent.cpp
# End Source File
# Begin Source File

SOURCE=.\aimsilent.h
# End Source File
# Begin Source File

SOURCE=.\attack.cpp
# End Source File
# Begin Source File

SOURCE=.\attack.h
# End Source File
# Begin Source File

SOURCE=.\bind.cpp
# End Source File
# Begin Source File

SOURCE=.\bind.h
# End Source File
# Begin Source File

SOURCE=.\Chat.cpp
# End Source File
# Begin Source File

SOURCE=.\Chat.h
# End Source File
# Begin Source File

SOURCE=.\ChineseCodeLib.cpp
# End Source File
# Begin Source File

SOURCE=.\ChineseCodeLib.h
# End Source File
# Begin Source File

SOURCE=.\client.cpp
# End Source File
# Begin Source File

SOURCE=.\client.h
# End Source File
# Begin Source File

SOURCE=.\color.cpp
# End Source File
# Begin Source File

SOURCE=.\color.h
# End Source File
# Begin Source File

SOURCE=.\console.cpp
# End Source File
# Begin Source File

SOURCE=.\console.h
# End Source File
# Begin Source File

SOURCE=.\cvar.cpp
# End Source File
# Begin Source File

SOURCE=.\cvar.h
# End Source File
# Begin Source File

SOURCE=.\drawing.cpp
# End Source File
# Begin Source File

SOURCE=.\drawing.h
# End Source File
# Begin Source File

SOURCE=.\esp.cpp
# End Source File
# Begin Source File

SOURCE=.\esp.h
# End Source File
# Begin Source File

SOURCE=.\font.cpp
# End Source File
# Begin Source File

SOURCE=.\font.h
# End Source File
# Begin Source File

SOURCE=.\functions.cpp
# End Source File
# Begin Source File

SOURCE=.\functions.h
# End Source File
# Begin Source File

SOURCE=.\hudmessage.cpp
# End Source File
# Begin Source File

SOURCE=.\hudmessage.h
# End Source File
# Begin Source File

SOURCE=.\idhook.cpp
# End Source File
# Begin Source File

SOURCE=.\idhook.h
# End Source File
# Begin Source File

SOURCE=.\ValveSDK\common\interface.cpp
# End Source File
# Begin Source File

SOURCE=.\interpreter.cpp
# End Source File
# Begin Source File

SOURCE=.\interpreter.h
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\menu.cpp
# End Source File
# Begin Source File

SOURCE=.\menu.h
# End Source File
# Begin Source File

SOURCE=.\offset.cpp
# End Source File
# Begin Source File

SOURCE=.\offset.h
# End Source File
# Begin Source File

SOURCE=.\opengl.cpp
# End Source File
# Begin Source File

SOURCE=.\opengl.h
# End Source File
# Begin Source File

SOURCE=.\SDK\MISC\PARSEMSG.CPP
# End Source File
# Begin Source File

SOURCE=.\SDK\MISC\PARSEMSG.H
# End Source File
# Begin Source File

SOURCE=.\player.cpp
# End Source File
# Begin Source File

SOURCE=.\player.h
# End Source File
# Begin Source File

SOURCE=.\radar.cpp
# End Source File
# Begin Source File

SOURCE=.\radar.h
# End Source File
# Begin Source File

SOURCE=.\recoil.cpp
# End Source File
# Begin Source File

SOURCE=.\recoil.h
# End Source File
# Begin Source File

SOURCE=.\snapshot.cpp
# End Source File
# Begin Source File

SOURCE=.\snapshot.h
# End Source File
# Begin Source File

SOURCE=.\soundesp.cpp
# End Source File
# Begin Source File

SOURCE=.\soundesp.h
# End Source File
# Begin Source File

SOURCE=.\strafe.cpp
# End Source File
# Begin Source File

SOURCE=.\strafe.h
# End Source File
# Begin Source File

SOURCE=.\stringfinder.cpp
# End Source File
# Begin Source File

SOURCE=.\stringfinder.h
# End Source File
# Begin Source File

SOURCE=.\time.cpp
# End Source File
# Begin Source File

SOURCE=.\time.h
# End Source File
# Begin Source File

SOURCE=.\trace.cpp
# End Source File
# Begin Source File

SOURCE=.\trace.h
# End Source File
# Begin Source File

SOURCE=.\usermsg.cpp
# End Source File
# Begin Source File

SOURCE=.\usermsg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter ""
# End Group
# End Target
# End Project
