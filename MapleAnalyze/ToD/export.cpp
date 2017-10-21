#include "stdafx.h"
#include "ToD.h"
#include "../_common/cmddef.h"
#include <time.h>
#include "ntdll.h"
#include "AnyJack.h"
#include "export.h"
#include "log.h"
#include <tlhelp32.h>

#include <psapi.h>

#pragma comment(lib, "psapi.lib")

//////////////////////////////////////////////////////////////////////////

void SetHardwareBreakOne(HANDLE hThread, BOOL bRemove, BOOL bData);
void SetHardwareBreakThread(DWORD dwThreadId, BOOL bRemove, BOOL bData);
void SetHardwareBreak(DWORD dwAddress1, DWORD dwAddress2/* = 0*/, DWORD dwAddress3/* = 0*/, DWORD dwAddress4/* = 0*/, BOOL bData);
void RemoveHardwareBreak();

DWORD g_dwHardwareBreakAddress[4] = {0};

DWORD   g_dwHWBreak;

void SetHardwareBreakOne(HANDLE hThread, BOOL bRemove, BOOL bData)
{
	CONTEXT context;
	RtlZeroMemory(&context, sizeof(context));

	GetThreadContext(hThread, &context);
	context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
	if (g_dwHardwareBreakAddress[0])
	{
		context.Dr0 = g_dwHardwareBreakAddress[0];
		if (!bRemove)
			context.Dr7 |= 0x3;
			//context.Dr7 = 0xFF;
		else
			context.Dr7 = 0;
		if (bData)
		{
			//context.Dr7 |= 0x10000;
			//context.Dr7 |= 0xFFFFFFFF; 
			context.Dr7 = (1<<17) + (1<<16) + 0x1; // 17:읽기, 16:쓰기
			//AJLog(TRUE, L"DATA-BREAK: addr=[%x]", context.Dr0);
			//PrintLogA("DATA-BREAK: addr=[%x]", context.Dr0);
		}
	}
	if (g_dwHardwareBreakAddress[1])
	{
		context.Dr1 = g_dwHardwareBreakAddress[1];
		if (!bRemove)
			context.Dr7 |= 0x3 << 2;
		else
			context.Dr7 = 0;
	}
	if (g_dwHardwareBreakAddress[2])
	{
		context.Dr2 = g_dwHardwareBreakAddress[2];
		if (!bRemove)
			context.Dr7 |= 0x3 << 4;
		else
			context.Dr7 = 0;
	}
	if (g_dwHardwareBreakAddress[3])
	{
		context.Dr3 = g_dwHardwareBreakAddress[3];
		if (!bRemove)
			context.Dr7 |= 0x3 << 6;
		else
			context.Dr7 = 0;
	}
	AJLog(TRUE, L"HW Break setting");
	if(SetThreadContext(hThread, &context))
	{
		AJLog(TRUE, L"HW Break setting");
	}
}

void SetHardwareBreakThread(DWORD dwThreadId, BOOL bRemove, BOOL bData)
{
	HANDLE hThread = OpenThread(MAXIMUM_ALLOWED, TRUE, dwThreadId);
	if (hThread)
	{
		SetHardwareBreakOne(hThread, bRemove, bData);
		CloseHandle(hThread);
	}
}

void SetHardwareBreak(DWORD dwAddress1, DWORD dwAddress2/* = 0*/, DWORD dwAddress3/* = 0*/, DWORD dwAddress4/* = 0*/, BOOL bData)
{
	g_dwHardwareBreakAddress[0] = dwAddress1;
	g_dwHardwareBreakAddress[1] = dwAddress2;
	g_dwHardwareBreakAddress[2] = dwAddress3;
	g_dwHardwareBreakAddress[3] = dwAddress4;

	DWORD dwProcessId = GetCurrentProcessId();
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	THREADENTRY32 te32;
	te32.dwSize = sizeof(THREADENTRY32);
	Thread32First(hSnapshot, &te32);
	do {
		if(te32.th32OwnerProcessID == dwProcessId)
		{
			SetHardwareBreakThread(te32.th32ThreadID, FALSE, bData);
		}
	} while(Thread32Next(hSnapshot, &te32));
}

void RemoveHardwareBreak()
{
	g_dwHardwareBreakAddress[0] = 0;
	g_dwHardwareBreakAddress[1] = 0;
	g_dwHardwareBreakAddress[2] = 0;
	g_dwHardwareBreakAddress[3] = 0x12341234;

	DWORD dwProcessId = GetCurrentProcessId();
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	THREADENTRY32 te32;
	te32.dwSize = sizeof(THREADENTRY32);
	Thread32First(hSnapshot, &te32);
	do {
		if(te32.th32OwnerProcessID == dwProcessId)
		{
			SetHardwareBreakThread(te32.th32ThreadID, TRUE, FALSE);
		}
	} while(Thread32Next(hSnapshot, &te32));
}


#define  MAX_CALLSTACK_DEPTH	20

void GetCallStackInfo(DWORD Ebp)
{
	DWORD dwEbp;
	dwEbp = Ebp;
	AJLog(TRUE, L"recv stack caller EBP = %x", Ebp);
	for (int  nIndex = 0; nIndex < MAX_CALLSTACK_DEPTH; nIndex++)   
	{
		if (IsBadReadPtr((LPDWORD)(dwEbp+4), sizeof(DWORD))) 
			continue;

		AJLog(TRUE, L"%d th recv stack caller %x", nIndex, *(LPDWORD)(dwEbp+4));
		DWORD dwOldEbp = dwEbp;
		if (IsBadReadPtr((LPDWORD)(dwEbp), sizeof(DWORD))) 
			break;
		dwEbp = *(LPDWORD)dwEbp;
		if ((dwEbp % 4) != 0 || dwOldEbp > dwEbp) break; 
	}
}

#include "HookApi.h"
#pragma comment(lib, "HookApi.lib")

INLINE_HOOK_INFO	g_HookInfo1, g_HookInfo2;
DWORD				g_Nakerecv_Call, g_Nakesend_Call;
int					g_nRecvlen, g_nSendlen;
DWORD				g_dwRecvBuf, g_dwSendBuf;
DWORD				g_dwEsp;
DWORD				g_dwEbp;
char				g_szLog[MAX_PATH];
int __declspec(naked) WSAAPI MyNakeRecv()
{
	__asm
	{
		mov g_dwEsp, esp;
		mov ebx, [esp + 0x0C];
		mov g_dwRecvBuf, ebx;
		mov g_nRecvlen,eax;		
		mov g_dwEbp, ebp;
		pushfd;
		pushad;
	}

	//PrintLogA("DirectInput8Create return addr = %x", *(PDWORD)(g_dwEsp));
	//AJLog(TRUE, L"MyNakeRecv return addr = %x", *(PDWORD)(g_dwEbp+4));
	//GetCallStackInfo(g_dwEbp);
	
//	sprintf(g_szLog, "RECV: %d - %x", g_nRecvlen, *(PDWORD)(g_dwEbp+4));
//	OutputDebugStringA(g_szLog);
	//if (g_nRecvlen > 0) PrintBufToFile((PBYTE)g_dwRecvBuf, g_nRecvlen, NULL);
	AJLog(TRUE, L"engine -- !!! recv Hooked caller = %x", *(PDWORD)g_dwEsp);
	GetCallStackInfo(g_dwEbp);
	__asm
	{
		popad;
		popfd;
		jmp [g_Nakerecv_Call];
	}
}

int g_i;
int __declspec(naked) WSAAPI MyNakeSend()
{
	_asm
	{
		mov g_dwEsp, esp;
		pushfd;
		pushad;
		mov ebx, [ebp + 0x0C];
		mov g_dwSendBuf, ebx;
		mov g_nSendlen, eax;
		mov g_dwEbp, ebp;
	}
	
	//if ( g_nSendlen == 13 )
//	if ( g_nSendlen == 9 || g_nSendlen == 11)
	if ( g_nSendlen == 10)
	{
		//PrintBufToFile((PBYTE)g_dwSendBuf, g_nSendlen, "C:\\DNF\\DNFSend.txt");
		//PrintHexToFile((PBYTE)g_dwSendBuf, g_nSendlen, "C:\\DNF\\DNFSend.dat");
//		PrintBufToFile((PBYTE)g_dwSendBuf, g_nSendlen, "C:\\DNF\\MPSSend-1.txt");
 //		sprintf(g_szLog, "SEND: %d - %x", g_nSendlen, *(PDWORD)(g_dwEbp+4));
//    		OutputDebugStringA(g_szLog);
//   		for (g_i = 0; g_i < 100; g_i++)
//   		{
//    			sprintf(g_szLog, "stack: %d - %x", g_i, *(PDWORD)(g_dwEsp+4*g_i));
//    			OutputDebugStringA(g_szLog);
 //  		}

	}
	AJLog(TRUE, L"engine -- !!! Send Hooked caller = %x", *(PDWORD)g_dwEsp);
	GetCallStackInfo(g_dwEbp);
	

	__asm
	{
		popad;
		popfd;
		jmp [g_Nakesend_Call];
	}
}

void HookRecvFunc()
{
	HMODULE hMod = LoadLibrary(L"ws2_32.dll");
	FARPROC proc = GetProcAddress(hMod, "recv");
	Hook_Api((DWORD)proc,(DWORD)MyNakeRecv, &g_HookInfo1, &g_Nakerecv_Call); // eng : 0x71AB67FD
	AJLog(TRUE, L"recv hook proc address %x", (DWORD)proc);
}

void HookSendFunc()
{
	HMODULE hMod = LoadLibrary(L"ws2_32.dll");
	FARPROC proc = GetProcAddress(hMod, "send");
	Hook_Api((DWORD)proc, (DWORD)MyNakeSend, &g_HookInfo2, &g_Nakesend_Call);
	AJLog(TRUE, L"send hook proc address %x", (DWORD)proc);
}

///////////////////////

// 다이얼로그와의 통신을 위한 글로벌이벤트, 공유메모리 변수 -
#define	SHARE_MEM_SIZE		0x10000

HANDLE g_hSendEvent			= NULL;
HANDLE g_hRecvEvent			= NULL;
HANDLE g_hSendAnswerEvent	= NULL;
HANDLE g_hRecvAnswerEvent	= NULL;

HANDLE g_hMapFile			= NULL;
BYTE * g_pbMapBuf			= NULL;

SHARE_INFO g_ShareInfo;
// -/
BYTE	g_newbytes1[5]		= {0};
BYTE	g_storebytes1[5]	= {0};
DWORD	g_dwRealSize		= 5;


HOOK_API_INFO haiHackPatchFunc1;
HOOK_API_INFO haiHackPatchFunc4;
// 
// typedef ATOM (WINAPI* _RegisterClassExA)( IN CONST WNDCLASSEXA * lpwcx);
// _RegisterClassExA pRegisterClassExA = NULL;
// ATOM WINAPI MyRegisterClassExA( IN CONST WNDCLASSEXA * lpwcx );


typedef
NTSTATUS
(NTAPI
 *_NtQueryInformationProcess)(
 IN HANDLE			hProcess,
 IN PROCESSINFOCLASS	ProcessInformationClass,
 OUT PVOID			pProcessInformation,
 IN ULONG			uProcessInformationLength,
 OUT PULONG			puReturnLength OPTIONAL
 );


void HideDll(LPWSTR lpDllName)
{
	HMODULE hModule = GetModuleHandle(lpDllName);

	_NtQueryInformationProcess NtQueryInformationProcess = (_NtQueryInformationProcess)
		GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtQueryInformationProcess");

	PROCESS_BASIC_INFORMATION processInfo = {0};

	DWORD dwReturnLength = 0;
	NtQueryInformationProcess(GetCurrentProcess(), ProcessBasicInformation, &processInfo, sizeof(processInfo), &dwReturnLength);

	PLIST_ENTRY ListHead, Current;

	PLDR_DATA_TABLE_ENTRY pstEntry = NULL;

	ListHead = &( processInfo.PebBaseAddress->Ldr->InLoadOrderModuleList);
	Current = ListHead->Flink;
	while ( Current != ListHead)
	{
		pstEntry = CONTAINING_RECORD( Current, LDR_DATA_TABLE_ENTRY, InLoadOrderModuleList);
		if ( pstEntry->DllBase == hModule)
		{
			pstEntry->InLoadOrderModuleList.Flink->Blink = pstEntry->InLoadOrderModuleList.Blink;
			pstEntry->InLoadOrderModuleList.Blink->Flink = pstEntry->InLoadOrderModuleList.Flink;
			break;
		}
		Current = pstEntry->InLoadOrderModuleList.Flink;
	}

	ListHead = &( processInfo.PebBaseAddress->Ldr->InMemoryOrderModuleList);
	Current = ListHead->Flink;
	while ( Current != ListHead)
	{
		pstEntry = CONTAINING_RECORD( Current, LDR_DATA_TABLE_ENTRY, InMemoryOrderModuleList);
		if ( pstEntry->DllBase == hModule)
		{
			pstEntry->InMemoryOrderModuleList.Flink->Blink = pstEntry->InMemoryOrderModuleList.Blink;
			pstEntry->InMemoryOrderModuleList.Blink->Flink = pstEntry->InMemoryOrderModuleList.Flink;
			break;
		}
		Current = pstEntry->InMemoryOrderModuleList.Flink;
	}

	ListHead = &( processInfo.PebBaseAddress->Ldr->InInitializationOrderModuleList);
	Current = ListHead->Flink;
	while ( Current != ListHead)
	{
		pstEntry = CONTAINING_RECORD( Current, LDR_DATA_TABLE_ENTRY, InInitializationOrderModuleList);
		if ( pstEntry->DllBase == hModule)
		{
			pstEntry->InInitializationOrderModuleList.Flink->Blink = pstEntry->InInitializationOrderModuleList.Blink;
			pstEntry->InInitializationOrderModuleList.Blink->Flink = pstEntry->InInitializationOrderModuleList.Flink;
			break;
		}
		Current = pstEntry->InInitializationOrderModuleList.Flink;
	}
}

//ATOM WINAPI MyRegisterClassExA( IN CONST WNDCLASSEXA * lpwcx )
DWORD StartCommThread()
{
	ATOM wRet = TRUE;
	int err = 0;
	DWORD dwThreadID = 0;
	DWORD dwOldProtect = 0;
	//	PrintLogA("Enter MyRegisterClassExA");
	AJLog(TRUE, L"StartCommThread Start");
	
	InstallVEH();
/*
	g_hSendEvent		= OpenEvent(EVENT_ALL_ACCESS, TRUE, L"Global\\MyRecvEvent");
	g_hRecvEvent		= OpenEvent(EVENT_ALL_ACCESS, TRUE, L"Global\\MySendEvent");	
	g_hSendAnswerEvent	= OpenEvent(EVENT_ALL_ACCESS, TRUE, L"Global\\MyRecvAnserEvent");	
	g_hRecvAnswerEvent	= OpenEvent(EVENT_ALL_ACCESS, TRUE, L"Global\\MySendAnswerEvent");

	g_hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 // do not inherit the name
		L"MAPFILE");               // name of mapping object 

	if (g_hMapFile == NULL) return 0;

	g_pbMapBuf = (BYTE*)MapViewOfFile(g_hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS,						// read/write permission
		0, 
		0, 
		SHARE_MEM_SIZE);           

	if (g_pbMapBuf == NULL) return 0;

	g_hBasicThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RecvThread, NULL, 0, &dwThreadID);
	if (g_hBasicThread == NULL)
	{
		PrintLogA("Engine -> Create REcvThread Failed");
		return 0;
	}

	SendCommand(CMD_READY);
*/

	// 0322 send, recv 후킹
	//HookRecvFunc();
	//HookSendFunc();

	//HMODULE hMod = LoadLibrary(L"ws2_32.dll");
	DWORD PID = GetCurrentProcessId();
	WCHAR szExeName[MAX_PATH];
	GetProcessNameFromPID(szExeName, PID);
	AJLog(TRUE, L"Process Name = %s", szExeName);
	
	if (!wcsstr(szExeName, L"lin.bin"))
	{
		HMODULE hMod = LoadLibrary(L"kernel32.dll");
		HMODULE hMod1 = LoadLibrary(L"KERNELBASE.dll");
		g_dwHardwareBreakAddress[0] = (DWORD)GetProcAddress(hMod, "CreateThread");
	//	g_dwHardwareBreakAddress[1] = (DWORD)GetProcAddress(hMod1, "CreateRemoteThreadEx");
		g_dwHardwareBreakAddress[2] = (DWORD)GetProcAddress(hMod, "WriteProcessMemory");
//		g_dwHardwareBreakAddress[3] = (DWORD)GetProcAddress(hMod, "CreateThread");
		
		AJLog(TRUE, L"Hook module = %x,  proc adress = %x",(DWORD)hMod, g_dwHardwareBreakAddress[0]);
		AJLog(TRUE, L"Hook module = %x,  proc adress = %x",(DWORD)hMod, g_dwHardwareBreakAddress[1]);
		AJLog(TRUE, L"Hook module = %x,  proc adress = %x",(DWORD)hMod, g_dwHardwareBreakAddress[2]);
		AJLog(TRUE, L"Hook module = %x,  proc adress = %x",(DWORD)hMod, g_dwHardwareBreakAddress[3]);
		SetHardwareBreak(g_dwHardwareBreakAddress[0], 0, g_dwHardwareBreakAddress[2], 0, FALSE);

		
	
		
	/*
			proc = GetProcAddress(hMod, "CreateRemoteThreadEx");
				if ((DWORD)proc)
				{
					g_dwHWBreak = (DWORD)proc;
					AJLog(TRUE, L"Hook module = %x,  proc adress = %x",(DWORD)hMod, g_dwHWBreak);
					SetHardwareBreak(g_dwHWBreak,  0, 0,0, FALSE);
				}*/
		
		
		
		
	}



//	GetAllModArrInCurProc();


	AJLog(TRUE, L"StartCommThread End");

	return wRet;
}


int WINAPI ST()
{
	DWORD dwThreadID;
	WCHAR	szModuleName[MAX_PATH];
	AJInitialize(NULL, NULL);
	wcscpy(g_szLogFile, L"e:\\enginelog.txt");
	AJLog(TRUE, L"Start_Engine");

	g_dwGameProcessID = GetCurrentProcessId();	
	WCHAR	szDllPath[MAX_PATH] = L"engine.dll";
	
	PrintLog(L"Injection Dll Name %s", szDllPath);

	StartCommThread();

// 	HookLibGetFuncs();
// 
 	
// 
// 	// AJ 로그파일 경로
	
	return 0;
}


DWORD ModuleList[2048] = {0};

LPVOID g_AllModuleAddr[0x1000];

int g_nModuleCount;


void ReadyForModules(void)
{
	HANDLE hSnapShot = NULL;
	int focus = 0;
	RtlZeroMemory(ModuleList, sizeof(ModuleList));
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
	if(hSnapShot != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 me;
		DWORD ret;
		RtlZeroMemory(&me, sizeof(MODULEENTRY32));
		me.dwSize = sizeof(MODULEENTRY32);
		ret = Module32First(hSnapShot, &me);
		while(ret)
		{
			if((focus + 1) >= 2048) break;
			ModuleList[focus++] = (DWORD)me.hModule;
			ret = Module32Next(hSnapShot, &me);
		}
		CloseHandle(hSnapShot);
	}
}
void GetAllModArrInCurProc()
{
	LPVOID CurrentPtr = NULL;
	SYSTEM_INFO SysInfo;
	MEMORY_BASIC_INFORMATION mbi;
	DWORD Dummy = 0L;
	// --- Initialize
	RtlZeroMemory(&SysInfo, sizeof(SYSTEM_INFO));
	RtlZeroMemory(&mbi, sizeof(MEMORY_BASIC_INFORMATION));
	// --- Get System Information
	GetSystemInfo(&SysInfo);
	CurrentPtr = SysInfo.lpMinimumApplicationAddress;
	ReadyForModules();


	int n = 0;
	do
	{
		if(!VirtualQuery(CurrentPtr, &mbi, sizeof(MEMORY_BASIC_INFORMATION))) break;
		//PrintLog(L"Address %x %x %x", (DWORD)CurrentPtr, mbi.BaseAddress, mbi.RegionSize);
		//		if(!IsBadReadPtr(CurrentPtr, 2) &&
		//			*(short *)CurrentPtr == IMAGE_DOS_SIGNATURE && 
		//			mbi.Type == MEM_IMAGE)
		{
			// --- 모듈을 찾았다.
			//if(!IsInModuleList(mbi.BaseAddress))
			{				
				WCHAR FileName[MAX_PATH] = {0};

				if(GetMappedFileNameW(GetCurrentProcess(), mbi.BaseAddress, FileName, MAX_PATH)
					== 0) // via Psapi
				{
					//PrintLog(L"- Module FileName: [Query ModuleName is Failed]\n");
					DWORD dwProtect;
					LPVOID peAddr = CurrentPtr;
					AJLog(TRUE, L"General Unnamed Module base address %x size %x", mbi.BaseAddress, mbi.RegionSize, mbi.AllocationProtect);
					if(VirtualProtect(peAddr, 0x4, PAGE_EXECUTE_READWRITE, &dwProtect))
					{						
						
						if (*(PDWORD)peAddr == 0x905a4d && mbi.RegionSize == 0x1000)
						{
							AJLog(TRUE, L"\n Find Hidden module %x\n", (ULONG)peAddr);
							/*WCHAR DumpFileName[MAX_PATH];
							wsprintf(DumpFileName, L"e:\\%x.dump", (ULONG)peAddr); 
							// unprotect
							DWORD dwOffset = *(PDWORD)((PBYTE)peAddr + 0x3C);
							DWORD ImageSize = *(PDWORD)((PBYTE)peAddr + dwOffset + 0x50);
							DWORD Totalsize = 0;
							DWORD dwOldProtect[10];
							int sectionNumber = 0;
							DWORD dumpsize = 0;
							do
							{
								if(VirtualProtect(CurrentPtr, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &dwOldProtect[sectionNumber]))
								{
									AJLog(TRUE, L"\n %d th section change protect success size %x \n", sectionNumber, mbi.RegionSize);
									dumpsize += mbi.RegionSize;
								}
								else AJLog(TRUE, L"\n %d th section change protect failed size %x  \n", sectionNumber, mbi.RegionSize);
								
								CurrentPtr = (PVOID)((ULONG)CurrentPtr  + mbi.RegionSize);
								if(!VirtualQuery(CurrentPtr, &mbi, sizeof(MEMORY_BASIC_INFORMATION)))
								{
									AJLog(TRUE, L"VirtualQuery failed");
									break;
								}

								Totalsize += mbi.RegionSize;
								sectionNumber++;
							}while(Totalsize < ImageSize);
							AJLog(TRUE, L"\n section count %x \n", sectionNumber);
							AJLog(TRUE, L"\n module Size %x image size %x \n", mbi.RegionSize, ImageSize);
							DumpModule((HMODULE)peAddr, DumpFileName, dumpsize);*/
							
						}
						VirtualProtect(peAddr, 0x4, dwProtect, &dwProtect);

					}	
				}
				else 
				{
					//strHiddenModulePath.Add(FileName);
					AJLog(TRUE, L"General named Module name %s base address %x size %x", FileName, mbi.BaseAddress, mbi.RegionSize);
// 					if (wcsstr(FileName, L"lin.bin") > 0 && (DWORD)mbi.BaseAddress > 0x1700000)
// 					{
// 						WCHAR strOutput[MAX_PATH];
// 						wsprintf(strOutput, L"d:\\dump_%x.dump", n);
// 						AJLog(TRUE, L"Dump Engine %s value %x",strOutput, *(PDWORD)mbi.BaseAddress);						
// 						n++;
// 						//DumpHiddenModel(mbi.BaseAddress, mbi.RegionSize, strOutput);
// 					}
				}
			}
		}

		// --- Find Next Memory
		CurrentPtr = (PVOID)((ULONG)CurrentPtr  + mbi.RegionSize);
	} while((ULONG)CurrentPtr <= (ULONG)SysInfo.lpMaximumApplicationAddress &&
		mbi.RegionSize != 0L);
}


BOOL GetProcessNameFromPID(LPWSTR lpProcessName, DWORD PID)
{
	HANDLE         hProcessSnap = NULL;
	PROCESSENTRY32 pe32         = {0};
	BOOL		   bRet = FALSE;	

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return (DWORD)INVALID_HANDLE_VALUE;
	}
	pe32.dwSize = sizeof(PROCESSENTRY32);
	int nRunnigNum = 0;
	if (Process32First(hProcessSnap, &pe32))
	{
		DWORD	Code = 0;
		do
		{			
			if (pe32.th32ProcessID == PID)
			{				
				wcscpy(lpProcessName, pe32.szExeFile);								
				bRet = TRUE;				
				//PrintLog("동일프로세스 찿음");
				break;
			}			
		} while (Process32Next(hProcessSnap, &pe32));
	}

	CloseHandle (hProcessSnap);


}