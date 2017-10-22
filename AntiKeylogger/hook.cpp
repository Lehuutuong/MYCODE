#include "StdAfx.h"
#include "Global.h"
#include "hook.h"
#include <Sfc.h>
#include <Softpub.h>
#include "ntstatus.h"
#include "Wintrust.h"
#include "hash&crypt.h"

PSTRUCTA pStruct_445E14 = NULL;
PWSTR wsz_445E18 = NULL, wsz_445EF4 = NULL;
BOOL bool_445D24 = FALSE, bool_4444C4 = FALSE;
DWORD dword_4443C0;

PSTRUCTA sub_40F4E0()
{
	PSTRUCTA psa1, psa2 = sub_40F640();
	char szDllName[] = "ntdll.dll";
	char szFuncName[] = "LdrLoadDll";

	while(psa2)
	{
		sub_40F6A0(psa2);
		psa2 = sub_40F640();
	}

	psa1 = pStruct_445E14;

	if(psa1)
	{
		while(psa1->ListA.Flink)
			psa1 = (PSTRUCTA)psa1->ListA.Flink;

		psa2 = (PSTRUCTA)RtlAllocateHeap(g_hProcessHeap, 8, 0x218);
		psa1->ListA.Flink = (PLIST_ENTRY)psa2;
		psa2->ListA.Blink = (PLIST_ENTRY)psa1;
		psa1 = (PSTRUCTA)psa1->ListA.Flink;
	}
	else
	{
		psa1 = (PSTRUCTA)RtlAllocateHeap(g_hProcessHeap, 8, 0x218);
		pStruct_445E14 = psa1;
	}

	memcpy(psa1->szDllName, szDllName, 10);
	memcpy(psa1->szFuncName, szFuncName, 11);
	psa1->ProcNew = (FARPROC)sub_40ED50;

	DWORD var_8[2];
	var_8[0] = 1;
	var_8[1] = (DWORD)psa1;

	sub_40E3E0(var_8);

	PPEB peb = GetPEB();

	PVOID* pbProc = sub_40E610(psa1->szDllName, psa1->szFuncName, (PDWORD)peb->ImageBaseAddress);
	if(pbProc == NULL)
		return psa1;

	if(*pbProc == psa1->ProcNew)
		return psa1;

	sub_40F760(psa1, TRUE, (PDWORD)peb->ImageBaseAddress);

	return psa1;
}

PSTRUCTA sub_40F640()
{
	PSTRUCTA psa = pStruct_445E14;
	if(psa == NULL)
		return NULL;

	while(psa)
	{
		if(strcmp(psa->szDllName, "ntdll.dll") == 0 && strcmp(psa->szFuncName, "LdrLoadDll") == 0)
			return psa;

		psa = (PSTRUCTA)psa->ListA.Flink;
	}

	return NULL;
}

void sub_40F6A0(PSTRUCTA pStruct)
{
	if(pStruct == NULL)
		return;

	DWORD var_8[2];
	PSTRUCTA psa1, psa2 = (PSTRUCTA)pStruct->ListA.Flink;
	if(psa2)
	{
		psa1 = (PSTRUCTA)pStruct->ListA.Blink;
		if(psa1)
		{
			psa1->ListA.Flink = (PLIST_ENTRY)psa2;
			pStruct->ListA.Flink->Blink = pStruct->ListA.Blink;
		}
		else
		{
			pStruct_445E14 = psa2;
			psa2->ListA.Blink = NULL;
		}
	}
	else
	{
		psa2 = (PSTRUCTA)pStruct->ListA.Blink;
		if(psa2)
			psa2->ListA.Flink = NULL;
	}

	if(pStruct == pStruct_445E14)
	{
		if(pStruct->ListA.Flink == NULL || pStruct->ListA.Blink == NULL)
			pStruct_445E14 = NULL;
	}

	var_8[0] = 0;
	var_8[1] = (DWORD)pStruct;
	sub_40E3E0(var_8);
	sub_40F760(pStruct, FALSE, (PDWORD)GetPEB()->ImageBaseAddress);
	RtlFreeHeap(g_hProcessHeap, 0, pStruct);
}

void sub_40E3E0(DWORD* pdw)
{
	PPEB peb = GetPEB();

	PLDR_MODULE plm1, plm2 = (PLDR_MODULE)peb->LoaderData->InLoadOrderModuleList.Flink;
	plm1 = (PLDR_MODULE)plm2->InLoadOrderModuleList.Flink;

	while(plm1 != plm2)
	{
		if(plm1->BaseAddress)
		{
			if(!sub_40F400(plm1->FullDllName.Buffer, plm1->BaseDllName.Buffer, (PDWORD)plm1->BaseAddress, pdw))
				break;
		}

		plm1 = (PLDR_MODULE)plm1->InLoadOrderModuleList.Flink;
	}

	return;
}

PVOID* sub_40E610(char* szDllName, char* szFuncName, DWORD* BaseAddress)
{
	if(BaseAddress == NULL)
		return NULL;

	DWORD _ebp, _edx = BaseAddress[0x0F];
	DWORD* _eax = (PDWORD)((PBYTE)BaseAddress + _edx);
	DWORD* _ecx = (DWORD*)_eax[0x1E];
	if(_ecx == NULL)
		return NULL;

	DWORD* _edi = (DWORD*)_eax[0x20];
	if(_edi == NULL)
		return NULL;

	_edi = (PDWORD)((PBYTE)BaseAddress + (DWORD)_edi);
	_eax = (DWORD*)_edi[3];
	
	while(_eax) {
		if(stricmp(szDllName, (char*)BaseAddress + (DWORD)_eax) == 0) {
			_ecx = (DWORD*)_edi[0];
			_ecx = (PDWORD)((PBYTE)BaseAddress + (DWORD)_ecx);
			_eax = (DWORD*)_ecx[0];
			_ebp = 0;
			while(_eax) {
				if(strcmp(szFuncName, (char*)BaseAddress + (DWORD)_eax + 2) == 0) {
					_ecx = (DWORD*)_edi[4];
					_eax = _ecx + _ebp;
					_eax = (PDWORD)((PBYTE)BaseAddress + (DWORD)_eax);
					return (PVOID*)_eax;
				}
				_ecx += 1;
				_ebp ++;
				_eax = (DWORD*)_ecx[0];
			}
		}
		_edi += 5;
		_eax = (DWORD*)_edi[3];
	}
	
	return NULL;	
}

BOOL sub_40F760(PSTRUCTA pStruct, BOOL bFlag, DWORD* BaseAddress)
{
	DWORD var_4 = 0;

	if(BaseAddress == NULL || pStruct == NULL)
		return FALSE;

	DWORD _ebp, _edx = BaseAddress[0x0F];
	DWORD* _eax = (PDWORD)((PBYTE)BaseAddress + _edx);
	DWORD* _ecx;
	DWORD* _edi = (DWORD*)_eax[0x20];
	if(_edi == 0) return FALSE;
	_edi = (PDWORD)((PBYTE)BaseAddress + (DWORD)_edi);
	_eax = (DWORD*)_edi[3];
	while(_eax) {
		if(stricmp(pStruct->szDllName, (char*)BaseAddress + (DWORD)_eax) == 0) {
			_ecx = (DWORD*)_edi[0];
			_ecx = (PDWORD)((PBYTE)BaseAddress + (DWORD)_ecx);
			_eax = (DWORD*)_ecx[0];
			_ebp = 0;
			while(_eax) {
				if(strcmp(pStruct->szFuncName, (char*)BaseAddress + (DWORD)_eax + 2) == 0) {
					_ecx = (DWORD*)_edi[4];
					_eax = _ecx + _ebp;
					_eax = (PDWORD)((PBYTE)BaseAddress + (DWORD)_eax);

					if(bFlag)
					{
						pStruct->ProcOld = (FARPROC)*_eax;
						return WriteProcessMemory(g_hCurrentProcess, _eax, &pStruct->ProcNew, 4, &var_4);
					}
					else
						return WriteProcessMemory(g_hCurrentProcess, _eax, &pStruct->ProcOld, 4, &var_4);					
				}
				_ecx += 1;
				_ebp ++;
				_eax = (DWORD*)_ecx[0];
			}
		}
		_edi += 5;
		_eax = (DWORD*)_edi[3];
	}
	
	return FALSE;	
}

BOOL sub_40F400(PWSTR FullDllName, PWSTR BaseDllName, PDWORD BaseAddress, DWORD* pdw)
{
	if(BaseAddress == NULL)    
		return TRUE;

	DWORD _edx = BaseAddress[0x0F];
	DWORD* _eax = (PDWORD)((PBYTE)BaseAddress + _edx);
	DWORD* _ecx;
	DWORD* _edi = (DWORD*)_eax[0x20];
	DWORD _ebp;
	PSTRUCTA pStruct = (PSTRUCTA)pdw[1];

	if(_edi == 0) return TRUE;
	_edi = (PDWORD)((PBYTE)BaseAddress + (DWORD)_edi);
	_eax = (DWORD*)_edi[3];
	while(_eax) {
		if(stricmp(pStruct->szDllName, (char*)BaseAddress + (DWORD)_eax) == 0) {
			_ecx = (DWORD*)_edi[0];
			_ecx = (PDWORD)((PBYTE)BaseAddress + (DWORD)_ecx);
			_eax = (DWORD*)_ecx[0];
			_ebp = 0;
			while(_eax) {
				if(strcmp(pStruct->szFuncName, (char*)BaseAddress + (DWORD)_eax + 2) == 0) {
					_ecx = (DWORD*)_edi[4];
					_eax = _ecx + _ebp;
					_eax = (PDWORD)((PBYTE)BaseAddress + (DWORD)_eax);

					FARPROC proc = NULL;
					if(pdw[0] == 0) proc = pStruct->ProcOld;
					else proc = pStruct->ProcNew;
					if(_eax[0] != (DWORD)proc)
						sub_40F760(pStruct, pdw[0], (PDWORD)BaseAddress);

					return TRUE;
				}
				_ecx += 1;
				_ebp ++;
				_eax = (DWORD*)_ecx[0];
			}
		}
		_edi += 5;
		_eax = (DWORD*)_edi[3];
	}

	return TRUE;	
}

NTSTATUS NTAPI sub_40ED50(PWCHAR PathToFile, ULONG Flags, PUNICODE_STRING ModuleFileName, PHANDLE ModuleHandle)
{
    PWCHAR wsPath = NULL;
	int len = 0;
	DWORD var_24 = 0, var_1C = 1, var_10 = 1, var_18, *_eax;
	UNICODE_STRING usFile;
	PSTRUCTB pStruct = NULL;
	NTSTATUS ns;
	BOOL var_14;
	PWCHAR _ebp, _edi;
	PWSTR wsFile, _esi;
	HANDLE hFile;
	PSTRUCTA pStructA;
	PSTRUCTB pStructB;
	PPEB peb;

	if(PathToFile) {
		if(PathToFile[0])
			len = wcslen(PathToFile);
		len = (len + 1) * 2;
		wsPath = (PWCHAR)RtlAllocateHeap(g_hProcessHeap, 8, len);
		wcscpy(wsPath, PathToFile);
	}
	
	usFile.Length = ModuleFileName->Length;
	usFile.MaximumLength = ModuleFileName->MaximumLength;
	usFile.Buffer = NULL;
	if(usFile.MaximumLength) {
		usFile.Buffer = (PWSTR)RtlAllocateHeap(g_hProcessHeap, 8, usFile.MaximumLength);
		memcpy(usFile.Buffer, ModuleFileName->Buffer, usFile.MaximumLength);
	}
	
	if(PathToFile) {
		var_14 = (usFile.Buffer[1] == ':');
		PathToFile = var_14 ? usFile.Buffer : NULL;
		var_18 = usFile.Length / 2;
		if(var_14 == FALSE) {
			_edi = wsPath;
			_ebp = _edi;
			while(_edi[0]) {
				if(_ebp[0] == ';') {
					len = (((LONG)_ebp - (LONG)_edi) / 2 + 1) * 2;
					wsFile = (PWSTR)RtlAllocateHeap(g_hProcessHeap, 8, len + var_18 * 2 + 2);
					if(len > 2)
						memcpy(wsFile, _edi, len - 2);
					len = 0;
					if(wsFile && wsFile[0])
						len = wcslen(wsFile);
					if(wsFile[len - 1] != '\\') {
						len = wcslen(usFile.Buffer);
						if(usFile.Buffer[0] != '\\')
							wcscat(wsFile, L"\\");
					}
					wcscat(wsFile, usFile.Buffer);
					hFile = CreateFileW(wsFile, FILE_READ_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					if(hFile != INVALID_HANDLE_VALUE) {
						PathToFile = wsFile;
						CloseHandle(hFile);
						break;
					}
					RtlFreeHeap(g_hProcessHeap, 0, wsFile);
					_edi = _ebp + 1;
					if(_ebp[1] == '.')
						_edi ++;
					if(_edi[0] == ';')
						_edi ++;
					_ebp = _edi + 1;
				}
				_ebp ++;
				if(_ebp[0] == 0) break;
			}
		}
		if(PathToFile) {
			peb = GetPEB();
			if(wcsicmp(PathToFile, peb->ProcessParameters->ImagePathName.Buffer) == 0) var_1C = 1;
			else {
				pStruct = pStruct_4457D0;
				if(pStruct) {
					do {
						if(wcsicmp(pStruct->FullFileName, PathToFile) == 0) {
							if(var_14 == FALSE)
								RtlFreeHeap(g_hProcessHeap, 0, PathToFile);
							*ModuleHandle = NULL;
							if(wsPath)
								RtlFreeHeap(g_hProcessHeap, 0, wsPath);
							RtlFreeHeap(g_hProcessHeap, 0, usFile.Buffer);
							return STATUS_NO_SUCH_FILE;
						}
						pStruct = (PSTRUCTB)pStruct->ListB.Flink;						
					} while(pStruct != pStruct_4457D0);
				}
				pStruct = pStruct_4457C4;
				if(pStruct) {
					do {
						if(wcsicmp(pStruct->FullFileName, PathToFile) == 0) {
							pStruct->Unknown5 ++;
							var_10 = 0;
							goto loc_40F09D;
						}
						pStruct = (PSTRUCTB)pStruct->ListB.Flink;						
					} while(pStruct != pStruct_4457C4);
				}
				_esi = &usFile.Buffer[var_18];
				while(_esi[0] != '\\') {
					if(_esi == usFile.Buffer) break;
					_esi --;
				}
				if(_esi[0] == '\\')	_esi ++;
				
				if(pStruct_4457C4 == NULL)
				{
					pStruct = (PSTRUCTB)RtlAllocateHeap(g_hProcessHeap, 8, sizeof(STRUCTB));
					pStruct_4457C4 = pStruct;
					pStruct->ListB.Blink = (PLIST_ENTRY)pStruct;
					pStruct->ListB.Flink = (PLIST_ENTRY)pStruct;
				}
				else
				{
					pStruct = (PSTRUCTB)RtlAllocateHeap(g_hProcessHeap, 8, sizeof(STRUCTB));
					pStruct->ListB.Flink = (PLIST_ENTRY)pStruct_4457C4;
					pStruct->ListB.Blink = (PLIST_ENTRY)pStruct_4457C4->ListB.Blink;
					((PSTRUCTB)pStruct_4457C4->ListB.Blink)->ListB.Flink = (PLIST_ENTRY)pStruct;
					pStruct_4457C4->ListB.Blink = (PLIST_ENTRY)pStruct;
				}
				len = 0;
				pStruct->Unknown5 = 1;
				if(_esi && _esi[0])
					len = wcslen(_esi);
				pStruct->FileName = (PWSTR)RtlAllocateHeap(g_hProcessHeap, 8, (len + 1) * 2);
				wcscpy(pStruct->FileName, _esi);
				wsz_445E18 = pStruct->FileName;
				len = wcslen(PathToFile);
				pStruct->FullFileName = (PWSTR)RtlAllocateHeap(g_hProcessHeap, 8, (len + 1) * 2);
				wcscpy(pStruct->FullFileName, PathToFile);
				pStruct->IsFileProtected = SfcIsFileProtected(NULL, pStruct->FullFileName);
				if(!bool_IsOsWin2003)
					var_1C = sub_40EBC0(1, pStruct->FullFileName, NULL);
loc_40F09D:
				if(var_14 == FALSE)
					RtlFreeHeap(g_hProcessHeap, 0, PathToFile);
			}
		}
	}
	wsz_445E18 = NULL;
	if(var_1C) {
		ns = LdrLoadDll(wsPath, Flags, &usFile, ModuleHandle);
		if(var_10 != 0) {
			if(pStruct)
				pStruct->FileHandle = *ModuleHandle;
			if(*ModuleHandle != NULL) {
				pStructA = pStruct_445E14;
				while(pStructA) {
					_eax = (PDWORD)sub_40E610(pStructA->szDllName, pStructA->szFuncName, (PDWORD)*ModuleHandle);
					if(_eax != NULL && *_eax != (DWORD)pStructA->ProcNew)
						sub_40F760(pStructA, 1, (PDWORD)*ModuleHandle);
    				pStructA = (PSTRUCTA)pStructA->ListA.Flink;
				}
			}			
		}		
	}
	else {
		if(pStruct) {
			if(pStruct_4457D0 == NULL)
			{
				pStructB = (PSTRUCTB)RtlAllocateHeap(g_hProcessHeap, 8, sizeof(STRUCTB));
				pStruct_4457D0 = pStructB;
				pStructB->ListB.Flink = (PLIST_ENTRY)pStructB;
				pStructB->ListB.Blink = (PLIST_ENTRY)pStructB;
			}
			else
			{
				pStructB = (PSTRUCTB)RtlAllocateHeap(g_hProcessHeap, 8, sizeof(STRUCTB));
				pStructB->ListB.Flink = (PLIST_ENTRY)pStruct_4457D0;
				pStructB->ListB.Blink = pStruct_4457D0->ListB.Blink;
				pStruct_4457D0->ListB.Blink = (PLIST_ENTRY)pStructB;
				((PSTRUCTB)pStructB->ListB.Blink)->ListB.Flink = (PLIST_ENTRY)pStructB;
			}
			len = 0;
			if(pStruct->FullFileName && pStruct->FullFileName[0])
				len = wcslen(pStruct->FullFileName);
			pStructB->FullFileName = (PWSTR)RtlAllocateHeap(g_hProcessHeap, 8, (len + 1) * 2);
			wcscpy(pStructB->FullFileName, pStruct->FullFileName);
			if(pStruct != pStruct_4457C4) {
				((PSTRUCTB)pStruct->ListB.Flink)->ListB.Blink = pStruct->ListB.Blink;
				((PSTRUCTB)pStruct->ListB.Blink)->ListB.Flink = pStruct->ListB.Flink;
			}
			else if(pStruct_4457C4->ListB.Flink != (PLIST_ENTRY)pStruct_4457C4) {
				((PSTRUCTB)pStruct_4457C4->ListB.Flink)->ListB.Blink = pStruct_4457C4->ListB.Blink;
				((PSTRUCTB)pStruct_4457C4->ListB.Blink)->ListB.Flink = pStruct_4457C4->ListB.Flink;
				pStruct_4457C4 = (PSTRUCTB)pStruct_4457C4->ListB.Flink;
			}
			else
				pStruct_4457C4 = NULL;
			
			if(pStruct->FullFileName)
				RtlFreeHeap(g_hProcessHeap, 0, pStruct->FullFileName);
			if(pStruct->FileName)
				RtlFreeHeap(g_hProcessHeap, 0, pStruct->FileName);
			RtlFreeHeap(g_hProcessHeap, 0, pStruct);
		}
		*ModuleHandle = NULL;
		ns = STATUS_NO_SUCH_FILE;
	}

    if(wsPath)
		RtlFreeHeap(g_hProcessHeap, 0, wsPath);
	RtlFreeHeap(g_hProcessHeap, 0, usFile.Buffer);
	return ns;
}

DWORD sub_40EBC0(BOOL bFlag, PWSTR wsFileName, DWORD* arg_8)
{
	BOOL _ebp = FALSE, var_4 = FALSE;
	DWORD dwRet, var_8 = 0;

	var_4 = SfcIsFileProtected(NULL, wsFileName);
	dwRet = sub_40E6A0(arg_8, wsFileName);
	if(dwRet == 0 || dwRet == CERT_E_WRONG_USAGE || dwRet == E_NOTIMPL)
		_ebp = TRUE;
	else {
		dwRet = sub_40EA30(wsFileName, &var_8);
		if(dwRet == 0 || dwRet == CERT_E_WRONG_USAGE || dwRet == E_NOTIMPL) {
			_ebp = TRUE;
			if(arg_8) *arg_8 = var_8;
		}
	}
	if(bFlag == FALSE) return 2;
	if(_ebp == FALSE) {
		if(var_4) {
			if(TEST(byte_4457C0, 8) == 0) return 8;
			return sub_406360(wsFileName, 8, 0);
		}
		if(TEST(byte_4457C0, 4) == 0) return 4;
		return sub_406360(wsFileName, 4, 0);
	}
	if(TEST(byte_4457C0, 1) == 0) return 1;

	return sub_406360(wsFileName, 1, 0);// 1
}

HRESULT sub_40E6A0(DWORD* pdw, PWSTR wsFileName)
{
    HANDLE hFile;
	HCATADMIN hCatAdmin;
	HCATINFO hCatInfo;
	DWORD cbHash;
	BYTE pbHash[20];
	CATALOG_INFO sCatInfo;
	GUID ActionID;
	WINTRUST_DATA ActionData;
	DWORD var_440[0x110];
	WINTRUST_CATALOG_INFO CatalogInfo;
	WCHAR wszCatalogFilePath[0x106];
	HRESULT lStatus = ERROR_SUCCESS;
	
	if(pdw)
		*pdw = 0;
	hFile = CreateFileW(wsFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == NULL || hFile == INVALID_HANDLE_VALUE) return GetLastError();
	if(CryptCATAdminAcquireContext(&hCatAdmin, NULL, 0) == FALSE) {
		lStatus = GetLastError();
		CloseHandle(hFile);
		return lStatus;
	}
	cbHash = 20;
	if(CryptCATAdminCalcHashFromFileHandle(hFile, &cbHash, pbHash, 0) == FALSE) {
		lStatus = GetLastError();
		CryptCATAdminReleaseContext(hCatAdmin, 0);
		CloseHandle(hFile);
		return lStatus;
	}
	hCatInfo = CryptCATAdminEnumCatalogFromHash(hCatAdmin, pbHash, cbHash, 0, NULL);
	if(hCatInfo == NULL) {
		lStatus = GetLastError();
		if(lStatus == ERROR_SERVICE_NOT_ACTIVE) lStatus = ERROR_SUCCESS;
		CryptCATAdminReleaseContext(hCatAdmin, 0);
		CloseHandle(hFile);
		return lStatus;
	}
	if(pdw) {
		RtlZeroMemory(&sCatInfo, sizeof(CATALOG_INFO));
		sCatInfo.cbStruct = sizeof(CATALOG_INFO);
		if(CryptCATCatalogInfoFromContext(hCatInfo, &sCatInfo, 0) == TRUE) {
			ActionID.Data1 = 0xF750E6C3;
			ActionID.Data2 = 0x38EE;
			ActionID.Data3 = 0x11D1;
			ActionID.Data4[0] = 0x85;
			ActionID.Data4[1] = 0xE5;
			ActionID.Data4[2] = 0;
			ActionID.Data4[3] = 0xC0;
			ActionID.Data4[4] = 0x4F;
			ActionID.Data4[5] = 0xC2;
			ActionID.Data4[6] = 0x95;
			ActionID.Data4[7] = 0xEE;

			RtlZeroMemory(&ActionData, sizeof(WINTRUST_DATA));
			ActionData.cbStruct = sizeof(WINTRUST_DATA);
			RtlZeroMemory(var_440, 0x440);
			var_440[0] = 0x440;
			var_440[0x10A] = osvi_4455AC.dwMajorVersion;
			var_440[0x10C] = osvi_4455AC.dwMajorVersion;
			var_440[0x10B] = osvi_4455AC.dwMinorVersion;
			var_440[0x10D] = osvi_4455AC.dwMinorVersion;
			ActionData.pPolicyCallbackData = var_440;
			ActionData.dwUIChoice = WTD_UI_NONE;
			ActionData.fdwRevocationChecks = WTD_REVOKE_NONE;
			ActionData.dwUnionChoice = WTD_CHOICE_CATALOG;
			RtlZeroMemory(&CatalogInfo, sizeof(WINTRUST_CATALOG_INFO));
			CatalogInfo.cbStruct = sizeof(WINTRUST_CATALOG_INFO);
			CatalogInfo.pcwszCatalogFilePath = wszCatalogFilePath;
			CatalogInfo.pbCalculatedFileHash = pbHash;
			CatalogInfo.cbCalculatedFileHash = cbHash;
			ActionData.pCatalog = &CatalogInfo;
			ActionData.dwStateAction = WTD_STATEACTION_AUTO_CACHE;
			lStatus = WinVerifyTrust(NULL, &ActionID, &ActionData);
			if(lStatus == ERROR_SUCCESS || lStatus == CERT_E_WRONG_USAGE) {
				*pdw = var_440[0x109];
				lStatus = ERROR_SUCCESS;
			}
		}
	}
    CryptCATAdminReleaseCatalogContext(hCatAdmin, hCatInfo, 0);
	CryptCATAdminReleaseContext(hCatAdmin, 0);
	CloseHandle(hFile);
	return lStatus;
}

HRESULT sub_40EA30(PWSTR wsFileName, DWORD* pdw)
{
	GUID ActionID;
	WINTRUST_DATA ActionData;
	WINTRUST_FILE_INFO FileInfo;
	DWORD var_440[0x110];
	HRESULT lStatus;

	if(pdw)
	{
		ActionID.Data1 = 0xF750E6C3;
		ActionID.Data2 = 0x38EE;
		ActionID.Data3 = 0x11D1;
		ActionID.Data4[0] = 0x85;
		ActionID.Data4[1] = 0xE5;
		ActionID.Data4[2] = 0;
		ActionID.Data4[3] = 0xC0;
		ActionID.Data4[4] = 0x4F;
		ActionID.Data4[5] = 0xC2;
		ActionID.Data4[6] = 0x95;
		ActionID.Data4[7] = 0xEE;
	}
	else
	{
		ActionID.Data1 = 0xFC451C16;
		ActionID.Data2 = 0xAC75;
		ActionID.Data3 = 0x11D1;
		ActionID.Data4[0] = 0xB4;
		ActionID.Data4[1] = 0xB8;
		ActionID.Data4[2] = 0;
		ActionID.Data4[3] = 0xC0;
		ActionID.Data4[4] = 0x4F;
		ActionID.Data4[5] = 0xB6;
		ActionID.Data4[6] = 0x6E;
		ActionID.Data4[7] = 0xA0;
	}
	
	RtlZeroMemory(&ActionData, sizeof(WINTRUST_DATA));
	ActionData.cbStruct = sizeof(WINTRUST_DATA);
	RtlZeroMemory(var_440, 0x440);
	var_440[0] = 0x440;
	var_440[0x10A] = osvi_4455AC.dwMajorVersion;
	var_440[0x10C] = osvi_4455AC.dwMajorVersion;
	var_440[0x10B] = osvi_4455AC.dwMinorVersion;
	var_440[0x10D] = osvi_4455AC.dwMinorVersion;
	ActionData.pPolicyCallbackData = var_440;
	ActionData.dwUIChoice = WTD_UI_NONE;
	ActionData.fdwRevocationChecks = WTD_REVOKE_NONE;
	ActionData.dwUnionChoice = WTD_CHOICE_FILE;
	RtlZeroMemory(&FileInfo, sizeof(WINTRUST_FILE_INFO));
	FileInfo.cbStruct = sizeof(WINTRUST_FILE_INFO);
	FileInfo.pcwszFilePath = wsFileName;
	ActionData.pFile = &FileInfo;
	ActionData.dwStateAction = WTD_STATEACTION_AUTO_CACHE;
	lStatus = WinVerifyTrust(NULL, &ActionID, &ActionData);
	if(lStatus == ERROR_SUCCESS || lStatus == CERT_E_WRONG_USAGE) {
		if(pdw) *pdw = var_440[0x109];
		else if(var_440[0x109])
			CertFreeCertificateContext((PCCERT_CONTEXT)var_440[0x109]);
		lStatus = ERROR_SUCCESS;
	}
	return lStatus;
}

DWORD sub_406360(PWSTR wsFileName, DWORD dwMask, DWORD dwFlag)
{
    DWORD dwRet = 0;

	if(dwMask == 4) {
		if(wsz_445EF4 == NULL) return 0;
		dwRet = sub_43F9A0(wsFileName) ? 0 : 1;
		return dwRet;
	}
	
    if(dwMask == 8) {
		if(sub_40BAD0(wsFileName) == 0) return 1;
		ExitProcess(0);
		return 0;
    }
	
	if(dwMask == 1 || dwMask == 2) return 1;
	
	return 0;
}

DWORD sub_43F9A0(PWSTR strFile)
{
	DWORD ret = ERROR_INVALID_PARAMETER;
	HANDLE hFile;
	DWORD length, BytesRead, offset, var_4B4, var_4A0, i;
	BYTE *buf, *var_49C, *var_4B0, var_488[0x10], var_478[0x10], var_498[0x10] = {0};
	char szAuthKey[] = "AuthKey : ";
	char szKey[] = "ak7.3rt";
	BOOL bFind = FALSE;
	HASH_CTX var_468;
	RC4_CTX var_408;

	if(strFile == NULL)
		return ret;

	hFile = CreateFileW(strFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return GetLastError();

	length = GetFileSize(hFile, NULL);
	if(length == 0)
		goto loc_43FC86;

	buf = (PBYTE)VirtualAlloc(NULL, length, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if(buf == NULL)
		goto loc_43FC86;

	BytesRead = 0;
	if(!ReadFile(hFile, buf, length, &BytesRead, NULL))
		goto loc_43FC86;

	BytesRead = 0;

	for(offset = 0 ; offset < length - 10 ; offset ++) {
		if(memcmp(buf + offset, szAuthKey, 10) == 0) {
			bFind = TRUE;
			break;
		}
	}
	if(bFind == FALSE) {
		ret = ERROR_BAD_FORMAT;
		goto loc_43FC86;
	}
	offset += 10;
	length -= (offset + 0x10);
	BytesRead = offset;
	var_4B4 = offset - 1;
	var_4A0 = length;
	var_49C = (PBYTE)VirtualAlloc(NULL, var_4B4, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if(var_49C == NULL)
		goto loc_43FC86;

	var_4B0 = (PBYTE)VirtualAlloc(NULL, var_4A0, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if(var_4B0 == NULL)
		goto loc_43FC86;

	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	if(!ReadFile(hFile, var_49C, var_4B4, &var_4B4, NULL)) goto loc_43FC86;
	SetFilePointer(hFile, BytesRead + 0x10, NULL, FILE_BEGIN);
	if(!ReadFile(hFile, var_4B0, var_4A0, &var_4A0, NULL)) goto loc_43FC86;
	sub_41CE70(&var_468);
	sub_41CED0(&var_468, var_49C, var_4B4);
	sub_41D090(&var_468, var_488);

	sub_41CE70(&var_468);
	sub_41CED0(&var_468, var_4B0, var_4A0);
	sub_41D090(&var_468, var_478);

	for(i = 0 ; i < 0x10 ; i ++)
		var_488[i] *= (var_478[i] & 0x7F);

	sub_41DF60(&var_408, (PBYTE)szKey, 7);
	sub_41E170(&var_408, var_488, var_498, 0x10);
	if(memcmp(buf + BytesRead, var_498, 0x10) == 0)	ret = ERROR_SUCCESS;
	else ret = ERROR_BAD_FORMAT;

loc_43FC86:
	if(hFile)
		CloseHandle(hFile);
	if(buf)
		VirtualFree(buf, 0, MEM_RELEASE);
	if(var_49C)
		VirtualFree(var_49C, 0, MEM_RELEASE);
	if(var_4B0)
		VirtualFree(var_4B0, 0, MEM_RELEASE);

	return ret;
}

BOOL sub_40BAD0(PWSTR wsFileName)
{
	DWORD len = 0;
				
	if(bool_445D24)
		return FALSE;
	
	bool_445D24 = TRUE;
    sub_40B800();
	
	if(wsFileName == NULL)
	{
		bool_445D24 = FALSE;
		return bool_4444C4;
	}
    
	len = (DWORD)wsFileName;
	len *= 0x343FD;
	len += 0x269EC3;
	dword_4443C0 = len;
    	
/*	PWSTR wsz_444420 = (PWSTR)RtlAllocateHeap(g_hProcessHeap, 8, 0x2000);
    wsprintfW(wsz_444420, L"One or more system files necessary for correct functioning of %ws, have been replaced with unknown versions. \
						  The work of the program can be hampered, so we do recommend you to use the system command \"sfc /scannow\" \
						  before you go on launching %ws. Continue launching the program if you are sure there is no spyware on your computer.",
						  L"AntiKeylogger", L"AntiKeylogger");
	bool_4444C4 = (MessageBoxW(NULL, wsz_444420, L"AntiKeylogger", MB_OKCANCEL) == IDCANCEL);
	
	RtlFreeHeap(g_hProcessHeap, 0, wsz_444420);*/
	bool_4444C4 = FALSE;
	
	bool_445D24 = FALSE;
	return bool_4444C4;
}

void sub_40B800()
{
    // user32.dll, gdi32.dll, advapi32.dll의 일부 함수들을 얻는다.	
}
