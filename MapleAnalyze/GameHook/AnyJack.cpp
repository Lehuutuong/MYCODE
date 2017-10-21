#include "stdafx.h"
#include "AnyJack.h"


typedef void (*_HandlerMap)(DWORD dwBreakAddr, DWORD dwCodeAddr);
_HandlerMap MyHandlerMap;
// 내부변수
BOOL		g_bInit;
BYTE		g_bEntryByte;
DWORD		g_dwThreadID;
DWORD		g_dwEntryAddr;
DWORD		g_dwEntryProtect;
CONTEXT		*g_pContext;
DWORD		g_dwBreakAddr;
LPBREAK		g_pBreak;
int			g_nBreakNum;
DWORD		g_dwTick;
WCHAR		g_szLogFile[MAX_PATH];
WCHAR		g_szCaptureFile[MAX_PATH];
CONTEXT		Context;
DWORD		g_dwIDCaptureTimer;
HWND		g_HwndAI;
BOOL		g_bPickFlag = FALSE;
HANDLE		g_hMod;
WCHAR		g_szLogMsg[1024];
FILE*		g_fp = NULL;
DWORD		g_dwHandlerMapFuncAddr;

BREAKINFO g_arrBreakInfo[40] = {
	//{0x498183, 0x498186, 0, 0},
	//{0x491650, 0x491652, 0, 0},
	//{0x491B3C, 0x491B3F, 0, 0},	
	//{0x491B1D, 0x491B20, 0, 0},
	//{0x507A7E, 0x507A85, 0, 0},

	{0,0,0,0},
};


void AJInitialize(HINSTANCE hModule, DWORD dwHandlerAddr)
{
	DWORD	bRet;
	g_hMod = hModule;
	g_pBreak = new BREAK[MAX_BREAK];
	g_nBreakNum = 0;
	g_dwHandlerMapFuncAddr = dwHandlerAddr;
	
	g_dwTick = GetTickCount();


	ZeroMemory(g_szLogMsg, sizeof(WCHAR) * 1024);
}

void ResumeMainThreadPoker()
{
	DWORD			dwID;
	HANDLE			hThreadSnap = INVALID_HANDLE_VALUE; 
	HANDLE			hThread;
	THREADENTRY32	te32; 

	dwID = GetCurrentProcessId();
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0); 
	if(hThreadSnap == INVALID_HANDLE_VALUE) 
		return;
	te32.dwSize = sizeof(THREADENTRY32); 
	if(!Thread32First(hThreadSnap, &te32)) 
	{
		CloseHandle(hThreadSnap);
		return;
	}
	do 
	{ 
		if(te32.th32OwnerProcessID != dwID)
			continue;
		if(te32.th32ThreadID == GetCurrentThreadId())
			continue;
		hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te32.th32ThreadID);
		ResumeThread(hThread);
		CloseHandle(hThread);
		break;
	}
	while(Thread32Next(hThreadSnap, &te32)); 
	CloseHandle(hThreadSnap);
}
void AddBreakOnEntryPointPoker()
{
	HMODULE					hMod;
	IMAGE_DOS_HEADER		*pDOSHdr;
	IMAGE_NT_HEADERS		*pNTHDr;

	hMod = GetModuleHandle(NULL);
	pDOSHdr = (IMAGE_DOS_HEADER *)hMod;
	pNTHDr = (IMAGE_NT_HEADERS *)((BYTE *)hMod+pDOSHdr->e_lfanew);
	g_dwEntryAddr = pNTHDr->OptionalHeader.AddressOfEntryPoint+(DWORD)hMod;
	g_bEntryByte = *((BYTE *)g_dwEntryAddr);
	VirtualProtect((LPVOID)g_dwEntryAddr, 1, PAGE_EXECUTE_READWRITE, &g_dwEntryProtect);
	*((BYTE *)g_dwEntryAddr) = 0xCC;
}
LONG CALLBACK  VectoredHandlerPoker(struct _EXCEPTION_POINTERS *ExceptionInfo)
{
	int		i, nBreak;
	AJLog(TRUE, L"exception %x", (int)ExceptionInfo->ExceptionRecord->ExceptionAddress);
	g_dwThreadID = GetCurrentThreadId();
	g_pContext = ExceptionInfo->ContextRecord;
	if(ExceptionInfo->ExceptionRecord->ExceptionCode == 0) // 초기화
	{
		if(g_dwHandlerMapFuncAddr != NULL)
		{
			*(DWORD*)&MyHandlerMap = (DWORD)g_dwHandlerMapFuncAddr;
			MyHandlerMap(0, 0);
		}
		
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	else if(ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP ||
		ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT)
	{

		nBreak = 0x0;
		// 우선 중단점이 등록된 중단점인가 본다.
		for(i = 0; i < g_nBreakNum; i++)
		{
			if(g_pBreak[i].dwAddr == (int)ExceptionInfo->ExceptionRecord->ExceptionAddress ||
				g_pBreak[i].dwAddr == g_pContext->Esi+0x170 ||
				g_pBreak[i].dwAddr == g_pContext->Edi+0x170 ||
				g_pBreak[i].dwAddr == g_pContext->Ecx+0x170 ||
				g_pBreak[i].dwAddr == g_pContext->Edx+0x170 ||
				g_pBreak[i].dwAddr == g_pContext->Eax+0x170)
				break;
		}
		if(i >= g_nBreakNum) // 등록된 중단점이 아니다.
			return EXCEPTION_CONTINUE_SEARCH;
		g_dwBreakAddr = g_pBreak[i].dwAddr;
//		g_dwBreakAddr = (int)ExceptionInfo->ExceptionRecord->ExceptionAddress;

	}
	else
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}

	memcpy(&Context.Edi, &g_pContext->Edi, sizeof(DWORD)*12);

	if(g_dwHandlerMapFuncAddr != NULL)
	{
		*(DWORD*)&MyHandlerMap = (DWORD)g_dwHandlerMapFuncAddr;
		MyHandlerMap(g_dwBreakAddr, (int)ExceptionInfo->ExceptionRecord->ExceptionAddress);
	}	
//	if(g_dwBreakAddr == (int)ExceptionInfo->ExceptionRecord->ExceptionAddress)
//		AJRemoveBreak(g_dwBreakAddr);
	return EXCEPTION_CONTINUE_EXECUTION;
}


BOOL	AJAddBreak(DWORD dwAddr, BYTE bType, BYTE SegTye)
{
	int			i;
	DWORD		*pDR;

	if(g_nBreakNum >= MAX_BREAK)
		return FALSE;

	// 먼저 이전의 중단점을 해제한다.
	AJRemoveBreak(dwAddr);
	if(bType == HARDWARE) // Hardware 중단점
	{
		pDR = &g_pContext->Dr0;
		for(i = 0; i < 4; i++)
		{
			if(pDR[i] == dwAddr) // 이미전에 중단점이 설정되여있으면
			{
				return TRUE;
			}
		}
		for(i = 0; i < 4; i++)
		{
			if(!pDR[i])
				break;
		}
		if(i < 4)
		{
			pDR[i] = dwAddr;
			g_pContext->Dr7 |= 3 <<(i*2);
			if (SegTye == DATA)
			{
				g_pContext->Dr7 |= 0x10000 << (i*4);
			}

			// 중단점을 등록한다.
			g_pBreak[g_nBreakNum].dwAddr = dwAddr;
			g_pBreak[g_nBreakNum].bType = HARDWARE;
			g_nBreakNum++;
		}
	}
	else // Software 중단점
	{
		BYTE	bOrg, bTemp = 0xCC;
		DWORD	dwOldProtect;

		memcpy(&bOrg, (BYTE *)dwAddr, 1);
		if(bOrg == 0xCC) // 이미전에 중단점이 설정되여있으면
		{
			return TRUE;
		}
		if(!VirtualProtect((LPVOID)dwAddr, 1, PAGE_EXECUTE_READWRITE, &dwOldProtect))
		{
			return FALSE;
		}
		memcpy((BYTE *)dwAddr, &bTemp, 1);
		VirtualProtect((LPVOID)dwAddr, 1, dwOldProtect, &dwOldProtect);
		//OutputDebugString("SB Success");
		// 중단점을 등록한다.
		g_pBreak[g_nBreakNum].dwAddr = dwAddr;
		g_pBreak[g_nBreakNum].bType = SOFTWARE;
		g_pBreak[g_nBreakNum].bOrg = bOrg;
		g_nBreakNum++;
	}
	return TRUE;
}

BOOL	AJRemoveBreak(DWORD dwAddr)
{
	int			i, nTemp, nBreak;
	DWORD		*pDR;

	// 순환을 돌면서 중단점을 찾아낸다.
	for(i = 0; i < g_nBreakNum; i++)
	{
		if(g_pBreak[i].dwAddr == dwAddr)
			break;
	}
	if(i >= g_nBreakNum)
		return FALSE;
	nBreak = i;
	if(g_pBreak[i].bType == HARDWARE) // 하드웨어
	{
		pDR = &g_pContext->Dr0;
		for(i = 0; i < 4; i++)
		{
			if(pDR[i] == (DWORD)dwAddr)
				break;
		}
		if(i < 4)
		{
			pDR[i] = 0;
			nTemp = 3<<(i*2);
			nTemp ^= 0xFFFFFFFF;
			g_pContext->Dr7 &= nTemp;
		}
	}
	else // 소프트웨어
	{
		DWORD	dwOldProtect;
		BYTE	bTemp;

		memcpy(&bTemp, (BYTE *)dwAddr, 1);
		if(bTemp == 0xCC) // 중단점이 설정되여있는 경우에만 해제한다.
		{
			if(!VirtualProtect((LPVOID)dwAddr, 1, PAGE_EXECUTE_READWRITE, &dwOldProtect))
			{
				return FALSE;
			}
			memcpy((BYTE *)dwAddr, &g_pBreak[i].bOrg, 1);
			VirtualProtect((LPVOID)dwAddr, 1, dwOldProtect, &dwOldProtect);
		}
	}

	// 중단점정보를 없앤다.
	for(i = nBreak; i < g_nBreakNum-1; i++)
	{
		memcpy(&g_pBreak[i], &g_pBreak[i+1], sizeof(BREAK));
	}
	g_nBreakNum--;
	return TRUE;

}



void ResetHardwareBreak19821718()
{
	CONTEXT		context;
	HANDLE		hThread;
	DWORD		dwThreadID;
	int			i, nBreak = 0;
	DWORD		*pDR;

	dwThreadID = GetCurrentThreadId();
	hThread = OpenThread(THREAD_GET_CONTEXT|THREAD_SET_CONTEXT, FALSE, dwThreadID);
	if(hThread == NULL)
		return;
	context.ContextFlags = CONTEXT_ALL;
	GetThreadContext(hThread, &context);

	pDR = &context.Dr0;
	context.Dr7 = 0;
	for(i = 0; i < 4; i++)
		pDR[i] = 0;
	for(i = 0; i < g_nBreakNum; i++)
	{
		if(g_pBreak[i].bType == SOFTWARE)
			continue;
		pDR[nBreak] = g_pBreak[i].dwAddr;
		context.Dr7 |= 3<<(nBreak*2);
	}
	SetThreadContext(hThread, &context);
	CloseHandle(hThread);
}

DWORD	AlignPage19821718(DWORD dwAddr, DWORD dwAlign)
{
	return ((dwAddr+dwAlign-1)/dwAlign)*dwAlign;
}

BOOL	AJRebuildPE(HANDLE hProcess, HMODULE hMod, WCHAR *szPath)
{
	BYTE					*pbHeader, *pbBuffer, *pbTemp;
	IMAGE_DOS_HEADER		*pDOSHdr;
	IMAGE_NT_HEADERS		*pNTHdr;
	IMAGE_SECTION_HEADER	*pSectionHdr;
	IMAGE_DATA_DIRECTORY	*pDataDirectory;
	BOOL					bIsCenter;
	DWORD					dwTemp, dwOld;
	FILE					*fp;
	WCHAR					szModule[MAX_PATH];
	int						i,j;

	// 먼저 헤더부들을 읽어들인다.
	GetModuleFileNameW(hMod, szModule, MAX_PATH);
	fp = _wfopen(szModule, L"rb");
	dwTemp = (DWORD)filelength(fileno(fp));
	pbHeader = new BYTE[dwTemp];
	fread(pbHeader, dwTemp, 1, fp);
	fclose(fp);

	pDOSHdr = (PIMAGE_DOS_HEADER)pbHeader;
	pNTHdr = (PIMAGE_NT_HEADERS)(pbHeader+pDOSHdr->e_lfanew);
	pSectionHdr = (PIMAGE_SECTION_HEADER)(pNTHdr+1);

	pbBuffer = new BYTE[pNTHdr->OptionalHeader.SizeOfImage];
	ZeroMemory(pbBuffer, pNTHdr->OptionalHeader.SizeOfImage);

	// 섹션에 들어가지 않는 디렉토리들을 보관한다.
	pDataDirectory = pNTHdr->OptionalHeader.DataDirectory;
	for( i = 0 ; i< 16; i++)
	{
		if(!pDataDirectory[i].VirtualAddress)
			continue;
		//i번째 디렉토리가 섹션안에 놓이는가를 검사한다.
		bIsCenter = FALSE;
		for(j = 0 ; j < pNTHdr->FileHeader.NumberOfSections; j++)
		{
			if( pSectionHdr[j].VirtualAddress <= pDataDirectory[i].VirtualAddress )
				if( pSectionHdr[j].Misc.VirtualSize >= pDataDirectory[i].Size )
				{
					bIsCenter = TRUE;
					break;
				}
		}
		//어느 섹션안에도 놓여있지 않으면 그 디렉토리를 저장한다.
		if(!bIsCenter)
		{
			pbTemp = new BYTE[pDataDirectory[i].Size];
			VirtualProtect((BYTE *)hMod+pDataDirectory[i].VirtualAddress, pDataDirectory[i].Size, PAGE_EXECUTE_READWRITE, &dwOld);
			memcpy(pbTemp, (BYTE *)hMod+pDataDirectory[i].VirtualAddress, pDataDirectory[i].Size);
			memcpy(pbBuffer+pDataDirectory[i].VirtualAddress, pbTemp, pDataDirectory[i].Size);
			delete [] pbTemp;
		}
	}

	// 섹션들을 보관한다.
	DWORD		dwMax = pNTHdr->OptionalHeader.SizeOfHeaders;

	for(i = 0; i < pNTHdr->FileHeader.NumberOfSections; i++)
	{
		DWORD	dwSize;

		dwMax = AlignPage19821718(dwMax, pNTHdr->OptionalHeader.FileAlignment);
		dwSize = max(pSectionHdr[i].Misc.VirtualSize, pSectionHdr[i].SizeOfRawData);
		pbTemp = new BYTE[dwSize];
		VirtualProtect((BYTE *)hMod+pSectionHdr[i].VirtualAddress, dwSize, PAGE_EXECUTE_READWRITE, &dwOld);
		memcpy(pbTemp, (BYTE *)hMod+pSectionHdr[i].VirtualAddress, dwSize);
		memcpy(pbBuffer+dwMax, pbTemp, dwSize);
		pSectionHdr[i].PointerToRawData = dwMax;
		pSectionHdr[i].SizeOfRawData = dwSize;
		dwMax += dwSize;
	}

	dwMax = AlignPage19821718(dwMax, pNTHdr->OptionalHeader.FileAlignment);
	// 헤더들을 보관한다.
	memcpy(pbBuffer, pDOSHdr, pNTHdr->OptionalHeader.SizeOfHeaders);

	fp = _wfopen(szPath, L"wb");
	if(fp)
	{
		fwrite(pbBuffer, dwMax, 1, fp);
		fclose(fp);
	}
	delete [] pbBuffer;
	delete [] pbHeader;
	return TRUE;
}


BOOL	AJLog(BOOL bAddTime, WCHAR *szFormat, ...)
{
	FILE	*fp;	
	WCHAR	szLog[1024];
	va_list	arg;

	va_start(arg, szFormat);
	vswprintf(szLog, szFormat, arg);
	va_end(arg);
	
	char	aszLog[MAX_PATH] = {0};
	SYSTEMTIME st; GetLocalTime(&st);
	sprintf(aszLog, "%d-%d-%d-%d - ", st.wHour, st.wMinute, st.wSecond,st.wMilliseconds);

	fp = _wfopen(g_szLogFile, L"a+");
	if(!fp)	return FALSE;

	char *writeLog = new char[wcslen(szLog)*2+1];
	WideCharToMultiByte(CP_ACP, 0, szLog, wcslen(szLog)+1, writeLog, wcslen(szLog)*2, 0, 0);
	
	fprintf(fp, "%s%s\n", aszLog, writeLog);
	fclose(fp);
	delete [] writeLog;

	return TRUE;
}
BOOL	AJLog(BOOL bAddTime, char *szFormat, ...)
{
	FILE	*fp;	
	char	szLog[1024];
	va_list	arg;

	va_start(arg, szFormat);
	vsprintf(szLog, szFormat, arg);
	va_end(arg);

	char	aszLog[MAX_PATH] = {0};
	SYSTEMTIME st; GetLocalTime(&st);
	sprintf(aszLog, "%d-%d-%d-%d - ", st.wHour, st.wMinute, st.wSecond,st.wMilliseconds);

	fp = _wfopen(g_szLogFile, L"a+");
	if(!fp)	return FALSE;

	char *writeLog = new char[strlen(szLog)];
	//WideCharToMultiByte(CP_ACP, 0, szLog, wcslen(szLog)+1, writeLog, wcslen(szLog)*2, 0, 0);
	strcpy(writeLog, szLog);

	fprintf(fp, "%s%s\n", aszLog, writeLog);
	fclose(fp);
	delete [] writeLog;

	return TRUE;
}


int	AJMessage(int nType, WCHAR *szFormat, ...)
{
	WCHAR	szMsg[1024];
	va_list	arg;

	va_start(arg, szFormat);
	vswprintf(szMsg, szFormat, arg);
	va_end(arg);
	return MessageBoxW(NULL, szMsg, L"AnyJack", nType);
}
