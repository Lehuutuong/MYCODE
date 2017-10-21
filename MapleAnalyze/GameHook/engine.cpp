#include "stdafx.h"
#include "../_common/patch.h"
#include "../_common/log.h"
#include "engine.h"
#include "GameHook.h"

#include <stdio.h>
#include "AnyJack.h"
#include <psapi.h>

//#define USER_EXCEPTION_BREAK

#define  MAX_CALLSTACK_DEPTH	20

extern DWORD g_dwHardwareBreakAddress[4];

extern BOOL g_bUserKeySend;
extern DWORD g_KeyHwnd;
extern DWORD g_KeyMsg;
extern DWORD g_KeyParam1;
extern DWORD g_KeyParam2;
extern HANDLE g_hEhsvc;


void GetCallStackInfo(DWORD Ebp)
{
	DWORD dwEbp;
	dwEbp = Ebp;
	for (int  nIndex = 0; nIndex < MAX_CALLSTACK_DEPTH; nIndex++)   
	{
		AJLog(TRUE, L"%d th stack caller %x", nIndex, *(LPDWORD)(dwEbp+4));
		DWORD dwOldEbp = dwEbp;
		if (IsBadReadPtr((LPDWORD)(dwEbp), sizeof(DWORD))) 
			break;
		dwEbp = *(LPDWORD)dwEbp;
		if ((dwEbp % 4) != 0 || dwOldEbp > dwEbp) // 잘못된 값이 나올때까지 추적
		{   
			break;   
		}
	}
}
void GetStackInfo(DWORD Esp)
{
	DWORD dwEsp;
	for ( int i = 0; i < 20; i ++ )
	{
		if ( IsBadReadPtr((LPDWORD)(g_pContext->Esp+i*4), 4) == TRUE ) break;
		dwEsp = *(LPDWORD)(g_pContext->Esp+i*4);
		AJLog(TRUE, "%d th stack buff %x", i, dwEsp);
	}
}


void PrintMemoryInfo()
{
	HANDLE hProcess;
	PROCESS_MEMORY_COUNTERS pmc;

	// Print the process identifier.
	DWORD processID = GetCurrentProcessId();

	// Print information about the memory usage of the process.

	hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID );
	if (NULL == hProcess)
		return;

	if ( GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) )
	{
		//AJLog(TRUE,L"PeakWorkingSetSize: 0x%08X", pmc.PeakWorkingSetSize );
		AJLog(TRUE, L"WorkingSetSize: %d, PagefileUsage: %d", pmc.WorkingSetSize, pmc.PagefileUsage);		
		//AJLog(TRUE, L"PeakPagefileUsage: 0x%08X", pmc.PeakPagefileUsage );
	}

	CloseHandle( hProcess );
}

//0x10: VK_SHIFT
//0xA0: VK_LSHIFT

extern DWORD g_dwDIObj;

DWORD g_dwTTT = 0;
DWORD g_nChangeNum = 0;
BOOL g_bDataBreak = FALSE;
BOOL g_bFindFirstBreak = FALSE;
BYTE g_BotName[13] = {0}, g_BotName1[13] = {0};
LONG WINAPI VectoredHandler_Engine(struct _EXCEPTION_POINTERS *ExceptionInfo)
{
	DWORD	nCode, ExceptionAddr;	
	DWORD	dwTemp1, dwTemp2, dwecx, dwebp, dwCall;
	DWORD	dwEax, dwEbx, dwEcx, dwEdx, dwEsi, dwEdi, dwEsp, dwEbp, dwEip; 
	DWORD	Param1, Param2;
	int		i, j;
	LPMSG	lpMsg = NULL;
	LONG	nRet = EXCEPTION_CONTINUE_SEARCH;
	DWORD	dwThreadId;

	ExceptionAddr	= (DWORD)ExceptionInfo->ExceptionRecord->ExceptionAddress;
	nCode			= ExceptionInfo->ExceptionRecord->ExceptionCode;	
	g_pContext		= ExceptionInfo->ContextRecord;

	//AJLog(TRUE, L"Exception Addr = %x, Code = %x", ExceptionAddr, nCode);
	if (nCode == 0)
	{
		AJLog(TRUE, "하브걸림");
// 		g_pContext->Dr0 = 0x11A2D70;
// 		dwTemp1 = 0x171FE90;
// 		dwTemp1 = *(PDWORD)dwTemp1;
		AJLog(TRUE, L"하브 걸자: module = %x", g_hEhsvc);
		g_pContext->Dr0 = (DWORD)g_hEhsvc + 0x1812F;
		g_pContext->Dr7 = 0xFF;
// 		g_pContext->Dr7 = (1 << 16) + 1;
// 		SetHardwareBreak(0x7C82168C, 0, 0, 0x12341234, FALSE);
		return EXCEPTION_CONTINUE_EXECUTION;
	}

	if (nCode == EXCEPTION_ACCESS_VIOLATION)
	{
		if (ExceptionAddr > 0)
		{
			AJLog(TRUE, L"Access Violation Addr = %x", ExceptionAddr);
			//for (int i = 0; i < 0x500; i ++)
			//	AJLog(TRUE, L"[%d] %x", i, *(DWORD*)(g_pContext->Esp+4*i));
		}
	}
	if (nCode == EXCEPTION_SINGLE_STEP || nCode == EXCEPTION_BREAKPOINT)
	{
		if (ExceptionAddr == 0x5509D0)
		{
			g_pContext->Esp -= 4;
			*(PDWORD)(g_pContext->Esp) = g_pContext->Ebp;
			dwTemp1 = *(PDWORD)(g_pContext->Esp + 8);
			dwTemp2	= *(WORD*)*(PDWORD)(dwTemp1+4);
			AJLog(TRUE, L"하브주소 = %x, caller = %x, OpCode = %x", ExceptionAddr, *(PDWORD)(g_pContext->Esp + 4), dwTemp2);
			g_pContext->Eip += 1;
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		else if (ExceptionAddr == 0x7C82168C)
		{
			AJLog(TRUE, L"하브주소 = %x, caller = %x", ExceptionAddr, *(PDWORD)(g_pContext->Esp));
			g_pContext->Edi += 2;
		}
		else if (ExceptionAddr == (DWORD)g_hEhsvc + 0xC5C4C)
		{
			AJLog(TRUE, "하브걸림, 주소 = %x, %x", g_pContext->Eip,  *(PDWORD)(g_pContext->Ebp + 0x10));
			GetCallStackInfo(g_pContext->Ebp);
// 			PrintBufToFile((PBYTE)(*(PDWORD)(g_pContext->Ebp + 0xC)), *(PDWORD)(g_pContext->Ebp + 0x10), "c:\\hshield.log");
			g_pContext->Esp -= 4;
			*(PDWORD)(g_pContext->Esp) = *(PDWORD)(g_pContext->Eax);
			g_pContext->Eip += 2;
		}
		else if (g_pContext->Eip == (DWORD)g_hEhsvc + 0x60B6A)
		{
			AJLog(TRUE, "하브걸림, 주소 = %x, %d", g_pContext->Eip,  g_pContext->Eax);
// 			GetCallStackInfo(g_pContext->Ebp);
			g_pContext->Esp -= 4;
			*(PDWORD)(g_pContext->Esp) = g_pContext->Eax;
			g_pContext->Eip += 1;
		}
		else if (g_pContext->Eip == (DWORD)g_hEhsvc + 0x1812F)
		{
			g_pContext->Esp -= 4;
			*(PDWORD)(g_pContext->Esp) = g_pContext->Edx;
			AJLog(TRUE, "하브걸림, 주소 = %x, %s", g_pContext->Eip, g_pContext->Edx);
			g_pContext->Eip += 1;
		}
		else
		{
			AJLog(TRUE, L"데이터 브릭 걸림: 주소 = %x", ExceptionAddr);
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		
// 		if (g_bDataBreak) AJLog(TRUE, L"DD = %x", ExceptionAddr);
// 		for (i = 0; i < 40; i ++)
// 		{
// 			if (g_arrBreakInfo[i].addr1 == 0) continue;
// 			if (g_arrBreakInfo[i].addr1 == ExceptionAddr)
// 			{
// 				if (i == 0) g_bFindFirstBreak = TRUE;
// 				//if (g_bFindFirstBreak) {
// 					AJLog(TRUE, L"EA[%ds]=%x,eax=%x,ebx=%x,ecx=%x,edx=%x,ebp=%x,esi=%x,edi=%x,esp=%x,esp[0]=%x,esp[4]=%x,esp[8]=%x,esp[c]=%x,esp[10]=%x",
// 						i+1,ExceptionAddr, g_pContext->Eax,g_pContext->Ebx,g_pContext->Ecx,g_pContext->Edx,g_pContext->Ebp,g_pContext->Esi,g_pContext->Edi,g_pContext->Esp,
// 						*(DWORD*)g_pContext->Esp,*(DWORD*)(g_pContext->Esp+4),*(DWORD*)(g_pContext->Esp+8),*(DWORD*)(g_pContext->Esp+0xC),*(DWORD*)(g_pContext->Esp+0x10));
// 										
// 										
// 				//}
// 				//AJLog(TRUE, L"ThreadId = %x", GetCurrentThreadId());
// 				//if (ExceptionAddr == 0x736230) {
// 				//	AJLog(TRUE, L"len=%d",  *(DWORD*)(g_pContext->Edx+0x8));
// 				//}				
// 				switch (i)
// 				{
// 				case 0:
// 					{
// 					}
// 					break;				
// 				default:
// 					break;
// 				}
// 				*(BYTE*)g_arrBreakInfo[i].addr1 = g_arrBreakInfo[i].val1;				
// 				*(BYTE*)g_arrBreakInfo[i].addr2 = 0xCC;
// 			}
// 			else if (g_arrBreakInfo[i].addr2 == ExceptionAddr)
// 			{
// 				//AJLog(TRUE, L"EA[%de]=%x,eax=%x,ebx=%x,ecx=%x,edx=%x,ebp=%x,esi=%x,edi=%x,esp=%x,esp[0]=%x,esp[4]=%x,esp[8]=%x,esp[c]=%x,esp[10]=%x",
// 				//	i+1,ExceptionAddr, g_pContext->Eax,g_pContext->Ebx,g_pContext->Ecx,g_pContext->Edx,g_pContext->Ebp,g_pContext->Esi,g_pContext->Edi,g_pContext->Esp,
// 				//	*(DWORD*)g_pContext->Esp,*(DWORD*)(g_pContext->Esp+4),*(DWORD*)(g_pContext->Esp+8),*(DWORD*)(g_pContext->Esp+0xC),*(DWORD*)(g_pContext->Esp+0x10));
// 
// 				//if (ExceptionAddr == 0x491B20) {}
// 				*(BYTE*)g_arrBreakInfo[i].addr2 = g_arrBreakInfo[i].val2;
// 				*(BYTE*)g_arrBreakInfo[i].addr1 = 0xCC;
// 
// 				if (i == 5) g_bFindFirstBreak = FALSE;
// 			}			
// 		}

		//if (i == 40)
		//{
			//if (ExceptionAddr != 0xC485F0)
			//AJLog(TRUE, L"[SBEX]exception addr = %x, code=%x", ExceptionAddr, nCode);
			//if (g_dwHardwareBreakAddress[0] > 0 && g_dwHardwareBreakAddress[0] == ExceptionAddr)
			//{
			//	RemoveHardwareBreak();
			//}
			//return EXCEPTION_CONTINUE_EXECUTION;
		//}

		return EXCEPTION_CONTINUE_EXECUTION;
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

void InstallVEH()
{
	g_hVectoredHandler = AddVectoredExceptionHandler(1, VectoredHandler_Engine);
	AJLog(TRUE, L"VectoredHandler_Engine = %x", g_hVectoredHandler);
	if ( g_hVectoredHandler == NULL )
	{
		AJLog(TRUE, L"InstallVEH failed.");
		return;
	}

	AJLog(TRUE, L"InstallVEH succeessed in Engine.");
}
