
#pragma once

#include <wchar.h>
#include "io.h"
#include "stdio.h"
#include "psapi.h"
#include "shlwapi.h"
#include "shellapi.h"
#include "Tlhelp32.h"
#include "excpt.h"
#include "Winsock2.h"
#include <stdarg.h>
#include <varargs.h>

#pragma		comment(lib,"Ws2_32.lib")
#pragma		comment(lib,"user32.lib")

// 마크로정의
#define		HookedFunctionSize (sizeof(HookedFunctions) / sizeof(HookedFunctions[0])) 
#define		MAX_BREAK	10000
#define		HARDWARE	0
#define		SOFTWARE	1
#define     CODE		2
#define     DATA		3

// 형정의
typedef struct _BREAK
{
	DWORD	dwAddr;
	BYTE	bType;
	BYTE	bOrg;
} BREAK, *LPBREAK;

// AnyJack함수 정의
void			AJInitialize(HINSTANCE hModule, DWORD dwHandlerAddr);
BOOL			AJAddBreak(DWORD dwAddr, BYTE bType, BYTE SegTye = CODE);
BOOL			AJRemoveBreak(DWORD dwAddr);
void			AddBreakOnEntryPointPoker();
LONG CALLBACK 	VectoredHandlerPoker(struct _EXCEPTION_POINTERS *ExceptionInfo);
void			ResumeMainThreadPoker();
void			ResetHardwareBreak19821718();
DWORD			AlignPage19821718(DWORD dwAddr, DWORD dwAlign);
BOOL			AJRebuildPE(HANDLE hProcess, HMODULE hMod, WCHAR *szPath);
int				AJMessage(int nType, WCHAR *szFormat, ...);
BOOL			AJLog(BOOL bAddTime, WCHAR *szFormat, ...);
BOOL			AJLog(BOOL bAddTime, char *szFormat, ...);


// 내부변수
extern BOOL			g_bInit;
extern BYTE			g_bEntryByte;
extern DWORD		g_dwThreadID;
extern DWORD		g_dwEntryAddr;
extern DWORD		g_dwEntryProtect;
extern CONTEXT		*g_pContext;
extern DWORD		g_dwBreakAddr;
extern LPBREAK		g_pBreak;
extern int			g_nBreakNum;
extern DWORD		g_dwTick;
extern WCHAR		g_szLogFile[MAX_PATH];
extern WCHAR		g_szCaptureFile[MAX_PATH];
extern CONTEXT		Context;
extern DWORD		g_dwIDCaptureTimer;
extern HWND			g_HwndAI;
extern BOOL			g_bPickFlag;
extern HANDLE		g_hMod;
extern WCHAR		g_szLogMsg[1024];
extern FILE*		g_fp;


typedef struct _BREAKINFO
{
	DWORD	addr1;
	DWORD	addr2;
	BYTE	val1;
	BYTE	val2;
}BREAKINFO, *LPBREAKINFO;
extern BREAKINFO g_arrBreakInfo[40];
