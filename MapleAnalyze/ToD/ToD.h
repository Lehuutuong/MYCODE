#ifndef __TOD_H__
#define __TOD_H__


#pragma once

#include <stdio.h>
#include <io.h>
#include <algorithm>
#include <vector>
using namespace std;
#include "..\_common\CmdDef.h"


struct	GENERALREGISTER
{
	DWORD	Eax;
	DWORD	Ecx;
	DWORD	Edx;
	DWORD	Ebx;
	DWORD	Esp;
	DWORD	Ebp;
	DWORD	Esi;
	DWORD	Edi;
	DWORD	Eip;
};


typedef struct _HOOK_API_CODE{
	BYTE code1;
	DWORD address;
	BYTE code2;
}HOOK_API_CODE, *PHOOK_API_CODE; //6

typedef struct _HOOK_API_INFO {
	FARPROC	OrgProc;
	FARPROC	HookProc; // 4
	HOOK_API_CODE hookBytes;//8
	BYTE	SaveBytes[6];//0x0e
} HOOK_API_INFO, *PHOOK_API_INFO; // 0x14


extern DWORD			g_dwGameProcessID;
extern HANDLE			g_hBasicThread;

extern GENERALREGISTER  g_GeneralRegister;
extern BOOL				g_bSaveRegister;


extern HANDLE g_hSendEvent;
extern HANDLE g_hRecvEvent;
extern HANDLE g_hSendAnswerEvent;
extern HANDLE g_hRecvAnswerEvent;

extern HANDLE g_hMapFile;
extern BYTE * g_pbMapBuf;

extern SHARE_INFO g_ShareInfo;

extern DWORD   g_dwHWBreak;
extern DWORD   g_dwVTIndex;
extern DWORD   g_dwBreakEcx;
extern DWORD g_dwHardwareBreakAddress[4];
//	Development Functions
void DumpGame(char * pszFilePath); 
DWORD DumpModule(LPTSTR lpModuleName, LPTSTR lpFileName);

//////////////////////////////////////////////////////////////////////////

void		InstallVEH();
DWORD		RecvThread(LPVOID pParam);
void		SendCommand(int nCommand);
BOOL		RefreshAutoInfo(BOOL bWrite = TRUE);



#endif