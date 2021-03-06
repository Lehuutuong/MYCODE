// ToD.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//

#include "stdafx.h"
#include "ToD.h"
#include <Iphlpapi.h >
#include <time.h>
#include <math.h>

#include "AnyJack.h"
#include "export.h"


DWORD g_dwGameProcessID = 0;
HANDLE g_hBasicThread;
GENERALREGISTER  g_GeneralRegister;
BOOL  g_bSaveRegister = FALSE;
DWORD g_dwVTIndex;


LONG WINAPI VectoredHandler(struct _EXCEPTION_POINTERS *ExceptionInfo)
{
	DWORD nCode = ExceptionInfo->ExceptionRecord->ExceptionCode;
	LONG nRet = EXCEPTION_CONTINUE_SEARCH;
	DWORD Index, dwRet, obj, NextIP, MonVTAddr;
	int  Hp, DecHP, DecMP, i, dwHP;
	DWORD	ExceptionAddr;
	DWORD dwTemp, dwTemp1, dwTemp2, dwecx, dwebp, dwCall;
	DWORD dwEax, dwEbx, dwEcx, dwEdx, dwEsi, dwEdi, dwEsp, dwEbp, dwEip; 
	DWORD Param1, Param2;
	g_pContext = ExceptionInfo->ContextRecord;

	ExceptionAddr = (DWORD)ExceptionInfo->ExceptionRecord->ExceptionAddress;

	if ( nCode == 0 )
	{
		AJLog(TRUE, L"Enter code 0 in VectoredHandler");
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	//AJLog(TRUE, L"!!! BreakAddress= %x, Code = %x", ExceptionAddr, nCode);
	if ( nCode == EXCEPTION_BREAKPOINT || nCode == EXCEPTION_SINGLE_STEP)
	{
		if (ExceptionAddr == g_dwHardwareBreakAddress[0])
		{
			//			AJLog(TRUE, L"!!! Break Point %x caller = %x appname = %s, cmdline = %s, flag = %x", ExceptionAddr, 
			//				*(PDWORD)g_pContext->Esp,*(PDWORD)(g_pContext->Esp+4), *(PDWORD)(g_pContext->Esp+8), *(PDWORD)(g_pContext->Esp+0x18));
			AJLog(TRUE, L"!!! Break Point %x caller = %x", ExceptionAddr, *(PDWORD)(g_pContext->Esp));
			GetCallStackInfo(g_pContext->Ebp);
			//GetAllModArrInCurProc();
			// 			__asm{
			// 				pushad
			// 			}
			///DumpModule(L"tmp1089.dll", L"e:\\tmp1089.dll.dump");
			// 			__asm
			// 			{
			// 				popad
			// 			}
			//  			g_pContext->Ecx = *(PDWORD)(g_pContext->Ebp - 0x1c);
			g_pContext->Edi = g_pContext->Edi;

			g_pContext->Eip += 2;
			return EXCEPTION_CONTINUE_EXECUTION;
		}

		if (ExceptionAddr == g_dwHardwareBreakAddress[1])
		{
			//			AJLog(TRUE, L"!!! Break Point %x caller = %x appname = %s, cmdline = %s, flag = %x", ExceptionAddr, 
			//				*(PDWORD)g_pContext->Esp,*(PDWORD)(g_pContext->Esp+4), *(PDWORD)(g_pContext->Esp+8), *(PDWORD)(g_pContext->Esp+0x18));
			AJLog(TRUE, L"!!! Break Point %x caller = %x", ExceptionAddr, *(PDWORD)(g_pContext->Esp));
			GetCallStackInfo(g_pContext->Ebp);
			//GetAllModArrInCurProc();
			// 			__asm{
			// 				pushad
			// 			}
			///DumpModule(L"tmp1089.dll", L"e:\\tmp1089.dll.dump");
			// 			__asm
			// 			{
			// 				popad
			// 			}
			//  			g_pContext->Ecx = *(PDWORD)(g_pContext->Ebp - 0x1c);
			g_pContext->Edi = g_pContext->Edi;

			g_pContext->Eip += 2;
			return EXCEPTION_CONTINUE_EXECUTION;
		}

		if (ExceptionAddr == g_dwHardwareBreakAddress[2])
		{
			//			AJLog(TRUE, L"!!! Break Point %x caller = %x appname = %s, cmdline = %s, flag = %x", ExceptionAddr, 
			//				*(PDWORD)g_pContext->Esp,*(PDWORD)(g_pContext->Esp+4), *(PDWORD)(g_pContext->Esp+8), *(PDWORD)(g_pContext->Esp+0x18));
			AJLog(TRUE, L"!!! Break Point %x caller = %x", ExceptionAddr, *(PDWORD)(g_pContext->Esp));
			GetCallStackInfo(g_pContext->Ebp);
			//GetAllModArrInCurProc();
			// 			__asm{
			// 				pushad
			// 			}
			///DumpModule(L"tmp1089.dll", L"e:\\tmp1089.dll.dump");
			// 			__asm
			// 			{
			// 				popad
			// 			}
			//  			g_pContext->Ecx = *(PDWORD)(g_pContext->Ebp - 0x1c);
			g_pContext->Edi = g_pContext->Edi;

			g_pContext->Eip += 2;
			return EXCEPTION_CONTINUE_EXECUTION;
		}

		if (ExceptionAddr == g_dwHardwareBreakAddress[3])
		{
			//			AJLog(TRUE, L"!!! Break Point %x caller = %x appname = %s, cmdline = %s, flag = %x", ExceptionAddr, 
			//				*(PDWORD)g_pContext->Esp,*(PDWORD)(g_pContext->Esp+4), *(PDWORD)(g_pContext->Esp+8), *(PDWORD)(g_pContext->Esp+0x18));
			AJLog(TRUE, L"!!! Break Point %x caller = %x", ExceptionAddr, *(PDWORD)(g_pContext->Esp));
			GetCallStackInfo(g_pContext->Ebp);
			//GetAllModArrInCurProc();
			// 			__asm{
			// 				pushad
			// 			}
			///DumpModule(L"tmp1089.dll", L"e:\\tmp1089.dll.dump");
			// 			__asm
			// 			{
			// 				popad
			// 			}
			//  			g_pContext->Ecx = *(PDWORD)(g_pContext->Ebp - 0x1c);
			g_pContext->Edi = g_pContext->Edi;

			g_pContext->Eip += 2;
			return EXCEPTION_CONTINUE_EXECUTION;
		}		
	}
	if ( nCode == EXCEPTION_ACCESS_VIOLATION)
	{
		//AJLog(TRUE, L"EXCEPTION_ACCESS_VIOLATION %x", ExceptionAddr);
		if (g_bSaveRegister)
		{
			__asm
			{
				mov eax, g_GeneralRegister.Eax;
				mov dwEax, eax;
				mov eax, g_GeneralRegister.Ebx;
				mov dwEbx, eax;
				mov eax, g_GeneralRegister.Ecx;
				mov dwEcx, eax;
				mov eax, g_GeneralRegister.Edx;
				mov dwEdx, eax;
				mov eax, g_GeneralRegister.Esi;
				mov dwEsi, eax;
				mov eax, g_GeneralRegister.Edi;
				mov dwEdi, eax;
				mov eax, g_GeneralRegister.Esp;
				mov dwEsp, eax;
				mov eax, g_GeneralRegister.Ebp;
				mov dwEbp, eax;
				mov eax, g_GeneralRegister.Eip;
				mov dwEip, eax;
			}

			g_pContext->Eax = dwEax;
			g_pContext->Ebx = dwEbx;
			g_pContext->Ecx = dwEcx;
			g_pContext->Edx = dwEdx;
			g_pContext->Esi = dwEsi;
			g_pContext->Edi = dwEdi;
			g_pContext->Esp = dwEsp;
			g_pContext->Ebp = dwEbp;
			g_pContext->Eip = dwEip + 0x18D; // 15C
			return EXCEPTION_CONTINUE_EXECUTION;
		}
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

void InstallVEH()
{
	PVOID pRet = AddVectoredExceptionHandler(1, VectoredHandler);
	if ( pRet == NULL )
	{
		PrintLogA("InstallVEH failed.");

		return;
	}
	RaiseException(0, 0, 0, 0);

	AJLog(TRUE, L"InstallVEH succeessed.");
}


void DumpGame(char * pszFilePath)
{
	if ( pszFilePath == NULL ) return;
	BYTE pOrigHeader[0x400];
	char szOrigPath[200];
	GetModuleFileNameA(NULL, szOrigPath, 200);

	FILE * fp = fopen(szOrigPath, "rb");
	fread(pOrigHeader, 1, 0x400, fp);
	fclose(fp);

	IMAGE_DOS_HEADER		*pDosHdr;
	IMAGE_NT_HEADERS		*pNTHdr;
	IMAGE_SECTION_HEADER	*pSectionHdr;

	pDosHdr = (IMAGE_DOS_HEADER *)pOrigHeader;
	pNTHdr = (IMAGE_NT_HEADERS *)(pOrigHeader + pDosHdr->e_lfanew);
	pNTHdr->OptionalHeader.FileAlignment = pNTHdr->OptionalHeader.SectionAlignment;
	int nCnt = pNTHdr->FileHeader.NumberOfSections;
	pSectionHdr = (IMAGE_SECTION_HEADER *)(pNTHdr + 1);

	for ( int i = 0 ; i < nCnt ; i ++ )
	{
		BYTE * pIn = (PBYTE)&pSectionHdr[i]; pIn += 8;
		memcpy(pIn + 8, pIn, 8);
	}

	fp = fopen(pszFilePath, "wb");
	fwrite(pOrigHeader, 1, 0x400, fp);
	fwrite((PBYTE)0x400400, 1, pNTHdr->OptionalHeader.SizeOfImage - 0x400, fp);
	fclose(fp);
}



DWORD DumpModule(LPTSTR lpModuleName, LPTSTR lpFileName)
{

	WCHAR str[256];
	//	HMODULE hModule = (HMODULE)0x24000000;
	HMODULE hModule = GetModuleHandle(lpModuleName);
	if(hModule)
	{
		
		DWORD dwOffset = *(PDWORD)((PBYTE)hModule + 0x3C);
		DWORD ImageSize = *(PDWORD)((PBYTE)hModule + dwOffset + 0x50);
		
		BYTE* Buffer = new BYTE [ImageSize];
		if(Buffer)
		{
			memcpy(Buffer, hModule, ImageSize);
			HANDLE hDump = CreateFile(lpFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
			if(hDump != INVALID_HANDLE_VALUE)
			{
				PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)((ULONG)Buffer + ((PIMAGE_DOS_HEADER)Buffer)->e_lfanew);
				pNtHeader->OptionalHeader.ImageBase = (DWORD)hModule; // 추가 
				PIMAGE_SECTION_HEADER pSecHeader = IMAGE_FIRST_SECTION(pNtHeader);				

				for(WORD i = 0; i < pNtHeader->FileHeader.NumberOfSections; i ++)
				{
					pSecHeader[i].PointerToRawData = pSecHeader[i].VirtualAddress;
					pSecHeader[i].SizeOfRawData = pSecHeader[i].Misc.VirtualSize;
				}
				WriteFile(hDump, Buffer, ImageSize, &ImageSize, NULL);
				CloseHandle(hDump);
			}
			delete Buffer;
		}
	}
	
	return (DWORD)hModule;
}