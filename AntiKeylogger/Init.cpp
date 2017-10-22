#include "StdAfx.h"
#include "Constdef.h"
#include "Global.h"
#include <winnt.h>
#include "Comm.h"
#include "hook.h"
#include <CommCtrl.h>
#include "ntstatus.h"
#include "Trans.h"
#include "password.h"
#include "uninstall.h"
#include "Resource.h"

HINSTANCE hLibUser32 = NULL;
BYTE byte_4457C0 = 0xFF;
BOOL bool_IsOsWin2003 = FALSE, bool_IsAutorun = FALSE, bool_445D14 = FALSE;
PSTRUCTB pStruct_4457C4 = NULL;
PSTRUCTB pStruct_4457D0 = NULL;
OSVERSIONINFOW osvi_4455AC, osvi_444278;
PSTRUCTA pStruct_4457B4 = NULL;
RTL_CRITICAL_SECTION rcs_4443D8, rcs_444398;
HANDLE hEvent_445810 = NULL, hEvent_445D04 = NULL, hEvent_445D00 = NULL;
HKEY hkey_445CF4 = NULL, hkey_445CF8 = NULL;
DWORD dword_44438C = 0;
STRUCTJ struct_44425C;

BOOL Start()
{
	ULONGLONG dwlConditionMask = 0;
	OSVERSIONINFOEXW osviex;
	PWSTR str, p;
	PPEB peb;
	PRTL_USER_PROCESS_PARAMETERS pRUPP;
	BOOL ret;
	HKEY hKey, hSubkey, hSubkey1 = 0;
	REGSAM samDesired;
	BOOL bInstall = FALSE;
	DWORD len = 4, dat[2] = {0};
	UUID Uuid;
	BYTE* buf;
	STICKYKEYS param;

	hEvent_Exit = OpenEventW(EVENT_ALL_ACCESS, FALSE, L"SHKeyPro_Exit");
	if(hEvent_Exit == NULL)
		hEvent_Exit = CreateEventW(NULL, TRUE, FALSE, L"SHKeyPro_Exit");

	ZeroMemory(&osvi_4455AC, sizeof(OSVERSIONINFOW));
	osvi_4455AC.dwOSVersionInfoSize = sizeof(OSVERSIONINFOW);
	GetVersionExW(&osvi_4455AC);

	dwlConditionMask = VerSetConditionMask(dwlConditionMask, VER_MAJORVERSION, VER_EQUAL);
	dwlConditionMask = VerSetConditionMask(dwlConditionMask, VER_MINORVERSION, VER_EQUAL);
	dwlConditionMask = VerSetConditionMask(dwlConditionMask, VER_BUILDNUMBER, VER_EQUAL);
	dwlConditionMask = VerSetConditionMask(dwlConditionMask, VER_SERVICEPACKMAJOR, VER_EQUAL);
	dwlConditionMask = VerSetConditionMask(dwlConditionMask, VER_SERVICEPACKMINOR, VER_EQUAL);

	ZeroMemory(&osviex, sizeof(OSVERSIONINFOEXW));
	osviex.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);
	osviex.dwMajorVersion = 5;
	osviex.dwMinorVersion = 2;
	osviex.dwBuildNumber = 0x0ece;
	osviex.wServicePackMajor = 0;
	osviex.wServicePackMinor = 0;
	bool_IsOsWin2003 = VerifyVersionInfoW(&osviex, VER_MAJORVERSION | VER_MINORVERSION | VER_BUILDNUMBER | VER_SERVICEPACKMAJOR | VER_SERVICEPACKMINOR, dwlConditionMask);

	g_hCurrentProcess = GetCurrentProcess();
	g_hProcessHeap = GetProcessHeap();
	hLibUser32 = LoadLibraryW(L"user32.dll");

	sub_40FA20();

	pStruct_4457B4 = sub_40F4E0();
	InitCommonControls();
	RtlInitializeCriticalSection(&rcs_4443D8);
	hEvent_445810 = CreateEventW(NULL, FALSE, TRUE, NULL);
	Sleep(2000);
	sub_40ADF0(NULL);
	memcpy(&osvi_444278, &osvi_4455AC, sizeof(OSVERSIONINFOW));
	RtlInitializeCriticalSection(&rcs_444398);
	str = (PWSTR)VirtualAlloc(NULL, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	peb = GetPEB();
	pRUPP = peb->ProcessParameters;
	if(pRUPP->CommandLine.Buffer[0])
		wcscpy(str, pRUPP->CommandLine.Buffer);
	wcslwr(str);
	p = wcsstr(str, L"/uninstall");
	if(p) bool_IsUninstall = TRUE;
	p = wcsstr(str, L"/autorun");
	if(p) bool_IsAutorun = TRUE;
	VirtualFree(str, 0, MEM_RELEASE);

	ret = sub_40ABF0();
	if(ret == FALSE)
	{
		sub_40AD50(L"SeLoadDriverPrivilege");
		ret = sub_40ABF0();
	}
	bool_445D14 = ret;

	if(bool_IsUninstall) {
		samDesired = bool_445D14 ? KEY_ALL_ACCESS : KEY_READ;
		if(RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"Software", 0, samDesired, &hKey) == ERROR_SUCCESS) {
			if(RegOpenKeyExW(hKey, L"SHKeyProtect", 0, samDesired, &hSubkey) == ERROR_SUCCESS) {
				if(RegOpenKeyExW(hSubkey, L"AntiKeylogger", 0, samDesired, &hSubkey1) == ERROR_SUCCESS) {
					if(RegQueryValueExW(hSubkey1, L"Install", 0, NULL, (PBYTE)&bInstall, &len) == ERROR_SUCCESS) {
						if(bInstall == TRUE) {
							RegCloseKey(hSubkey1);
							RegCloseKey(hSubkey);
							RegCloseKey(hKey);
							goto loc_40A56B;
						}
						if(bool_445D14 == TRUE) RegDeleteValueW(hSubkey1, L"Install");
					}
					RegCloseKey(hSubkey1);
				}
				RegCloseKey(hSubkey);
			}
			RegCloseKey(hKey);
		}
	}
	
	if(bool_IsUninstall) {
		if(bool_445D14 == FALSE) {
			sub_40AAF0(L"Uninstall failed, because you have no administrative privilege.", MB_OK | MB_ICONEXCLAMATION | MB_SYSTEMMODAL, NULL);
			goto loc_40A56B;
		}
		hEvent_445D04 = OpenEventW(EVENT_ALL_ACCESS, FALSE, L"SHKeyPro_Uninstall");
		if(hEvent_445D04 != NULL) {
			CloseHandle(hEvent_445D04);
			hEvent_445D04 = NULL;
			goto loc_40A56B;
		}
		hEvent_445D04 = CreateEventW(NULL, TRUE, TRUE, L"SHKeyPro_Uninstall");
	}
	hEvent_445D00 = OpenEventW(EVENT_ALL_ACCESS, FALSE, L"SHKeyPro_Window");
	if(bool_IsUninstall == FALSE) {
		if(hEvent_445D00) {
			if(bool_IsAutorun == FALSE)
				SetEvent(hEvent_445D00);
			goto loc_40A56B;
		}
		else {
			hEvent_445D00 = CreateEventW(NULL, TRUE, FALSE, L"SHKeyPro_Window");
		}
	}

	if(RegOpenKeyW(HKEY_CURRENT_USER, L"Software", &hKey) != ERROR_SUCCESS) goto loc_40A56B;
	if(RegOpenKeyW(hKey, L"SHKeyProtect", &hSubkey) != ERROR_SUCCESS) {
		if(RegCreateKeyW(hKey, L"SHKeyProtect", &hSubkey) != ERROR_SUCCESS) {
			RegCloseKey(hKey);
			goto loc_40A56B;
		}	
	}
	RegCloseKey(hKey);
	if(hSubkey) {
		if(RegOpenKeyW(hSubkey, L"AntiKeylogger", &hkey_445CF4) != ERROR_SUCCESS) {
			if(RegCreateKeyW(hSubkey, L"AntiKeylogger", &hkey_445CF4) != ERROR_SUCCESS) {
				RegCloseKey(hSubkey);
				goto loc_40A56B;
			}
		}
		RegCloseKey(hSubkey);
	}
	samDesired = bool_445D14 ? KEY_ALL_ACCESS : KEY_READ;
	if(RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"Software", 0, samDesired, &hKey) != ERROR_SUCCESS) goto loc_40A56B;
	if(RegOpenKeyExW(hKey, L"SHKeyProtect", 0, samDesired, &hSubkey) != ERROR_SUCCESS) {
		if(bool_445D14 == TRUE) {
			if(RegCreateKeyW(hKey, L"SHKeyProtect", &hSubkey) != ERROR_SUCCESS) {
				RegCloseKey(hKey);
				goto loc_40A56B;
			}
		}	
	}
	if(hSubkey) {
		if(RegCreateKeyW(hSubkey, L"AntiKeylogger", &hkey_445CF8) != ERROR_SUCCESS) {
			RegCloseKey(hKey);
			RegCloseKey(hSubkey);
			goto loc_40A56B;
		}
		RegCloseKey(hSubkey);
	}
	RegCloseKey(hKey);

	if(bool_445D14 == TRUE && bool_IsUninstall == FALSE) {
		if(RegQueryValueExW(hkey_445CF8, L"Path", 0, NULL, NULL, NULL) != ERROR_SUCCESS) {
			peb = GetPEB();
			pRUPP = peb->ProcessParameters;
			int len = wcslen(pRUPP->CurrentDirectoryPath.Buffer);
			if(RegSetValueExW(hkey_445CF8, L"Path", 0, REG_SZ, (PBYTE)pRUPP->CurrentDirectoryPath.Buffer, 2 * (len + 1)) != ERROR_SUCCESS) 
				goto loc_40A56B;
		}
	}
		
	dword_44438C = 2;
	RtlZeroMemory(&Uuid, sizeof(UUID));
	if(bool_445E1C == FALSE || sub_410FC0(&Uuid) != ERROR_SUCCESS) 
		UuidCreate(&Uuid);
	len = 0;
	if(dword_445E20 != 0) {
		if(sub_411290(NULL, 0, &len) == STATUS_BUFFER_TOO_SMALL) {
			buf = (BYTE*)VirtualAlloc(NULL, len, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
			sub_411290(buf, len, NULL);
			peb = GetPEB();
			dword_44438C = sub_40E0E0(buf, len, &struct_44425C);
			if(dword_44438C != 2) {
				char* szData1 = struct_44425C.szData1;
				char* szData2 = struct_44425C.szData2;
				struct_44425C.szData1 = (char*)RtlAllocateHeap(g_hProcessHeap, HEAP_ZERO_MEMORY, strlen(struct_44425C.szData1) + 1);
				struct_44425C.szData2 = (char*)RtlAllocateHeap(g_hProcessHeap, HEAP_ZERO_MEMORY, strlen(struct_44425C.szData2) + 1);
				memcpy(struct_44425C.szData1, szData1, strlen(szData1) + 1);
				memcpy(struct_44425C.szData2, szData2, strlen(szData2) + 1);
			}
			VirtualFree(buf, 0, MEM_RELEASE);
		}
	}
	if(dword_44438C == 2) {
		BYTE *lpResLock;
		HRSRC hResLoad, hRes = FindResource(NULL, (LPCSTR)IDR_DATA, "BIN");
		if (hRes) {
			if(hResLoad = (HRSRC)LoadResource(NULL, hRes)) {
				if(lpResLock = (BYTE*)LockResource(hResLoad)) {
					dword_44438C = sub_40E0E0(lpResLock, 0x2C8, &struct_44425C);
				}
			}
		}		
	}
	
	if(bool_IsUninstall == FALSE) {
		if(bool_445E1C == FALSE) {
			if(TEST(dword_445E20, 0x40000)) {
				sub_40AAF0(L"Some incompatible application is detected on this computer.", MB_OK | MB_ICONHAND | MB_SYSTEMMODAL, NULL);
				SetEvent(hEvent_445E28);
				goto loc_40A56B;
			}
			if(bool_DeviceNoAccess == TRUE) {
				sub_40AAF0(L"Sorry, a kernel debugger is installed in the system, our program can not start.", MB_OK | MB_ICONHAND | MB_SYSTEMMODAL, NULL);
				SetEvent(hEvent_445E28);
				goto loc_40A56B;
			}
			str = (PWSTR)RtlAllocateHeap(g_hProcessHeap, HEAP_ZERO_MEMORY, 2 * wcslen(L"Driver is not installed. Please reinstall the program.") + 0xC8);
			wcscpy(str, L"Driver is not installed. Please reinstall the program.");
#ifdef _DEBUG
			wsprintfW(str + wcslen(str), L"[code: 0x%08X]", dword_445E20);
#endif
			sub_40AAF0(str, MB_OK | MB_ICONHAND | MB_SYSTEMMODAL, NULL);
			RtlFreeHeap(g_hProcessHeap, 0, str);
			goto loc_40A56B;
		}

		if(bool_445D14 == TRUE) {
			if(RegQueryValueExW(hkey_445CF8, L"StickyKeys", 0, NULL, NULL, NULL) != ERROR_SUCCESS) {
				param.cbSize = sizeof(STICKYKEYS);
				SystemParametersInfoW(SPI_GETSTICKYKEYS, sizeof(STICKYKEYS), &param, 0);
				RegSetValueExW(hkey_445CF8, L"StickyKeys", 0, REG_BINARY, (BYTE*)&param, sizeof(STICKYKEYS));
			}	
		}
		param.cbSize = sizeof(STICKYKEYS);
		SystemParametersInfoW(SPI_GETSTICKYKEYS, sizeof(STICKYKEYS), &param, 0);
		param.dwFlags = 0;
		SystemParametersInfoW(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &param, 0);
		
		sub_410F00((BYTE*)dat, 8);
		if(TEST(dat[0], 0x100000)) hKey = hkey_445CF8;
		else hKey = hkey_445CF4;
		return TRUE;
	}

	if(bool_445E2C == TRUE)
		SetEvent(hEvent_445E28);
	else
		WaitForSingleObject(hEvent_445E28, INFINITE);

	dat[0] = 0; dat[1] = 0;
	if(sub_410F00((BYTE*)dat, 8) && TEST(dat[0], 0x200000))
	{
		DWORD var_5C;
		if(sub_401040(L"Enter your password:", &var_5C, NULL))
			goto loc_40A56B;
		if(var_5C != dat[1])
			goto loc_40A56B;
	}
	sub_437FA0(NULL, TRUE);

loc_40A56B:
	return FALSE;
}

void End()
{
	if(dword_44438C == 1)
	{
        if(struct_44425C.szData1)
			RtlFreeHeap(g_hProcessHeap, 0, struct_44425C.szData1);
        if(struct_44425C.szData2)
            RtlFreeHeap(g_hProcessHeap, 0, struct_44425C.szData2);    
	}

	if(hEvent_445D04)
	{
		WaitForSingleObject(hEvent_445D04, INFINITE);
		CloseHandle(hEvent_445D04);
	}
	if(hEvent_445D00)
		CloseHandle(hEvent_445D00);
	if(hkey_445CF4)
		RegCloseKey(hkey_445CF4);
	if(hkey_445CF8)
		RegCloseKey(hkey_445CF8);
	RtlDeleteCriticalSection(&rcs_444398);
	CloseHandle(hEvent_445810);
	RtlDeleteCriticalSection(&rcs_4443D8);
	RtlDeleteCriticalSection(&rcs_4457D4);
	CloseHandle(hEvent_445E28);
	sub_40F6A0(pStruct_4457B4);
	
	if(pStruct_445E14 != NULL)
	{
		PSTRUCTA psa = pStruct_445E14;
		while(psa->ListA.Flink)
			psa = (PSTRUCTA)psa->ListA.Flink;
		psa = (PSTRUCTA)psa->ListA.Blink;
		while(psa)
		{
			RtlFreeHeap(g_hProcessHeap, 0, psa->ListA.Flink);
			psa = (PSTRUCTA)psa->ListA.Blink;
		}
		RtlFreeHeap(g_hProcessHeap, 0, pStruct_445E14);
		pStruct_445E14 = NULL;
	}

	PSTRUCTB pStructB = pStruct_4457C4, psb;
	if(pStructB)
	{
		do {
			RtlFreeHeap(g_hProcessHeap, 0, pStructB->FullFileName);
			RtlFreeHeap(g_hProcessHeap, 0, pStructB->FileName);
			psb = (PSTRUCTB)pStructB->ListB.Flink;
			RtlFreeHeap(g_hProcessHeap, 0, pStructB);
			pStructB = psb;			
		} while(pStructB != pStruct_4457C4);
		pStruct_4457C4 = NULL;
	}

	pStructB = pStruct_4457D0;
	if(pStructB)
	{
		do {
			RtlFreeHeap(g_hProcessHeap, 0, pStructB->FullFileName);
			RtlFreeHeap(g_hProcessHeap, 0, pStructB->FileName);
			psb = (PSTRUCTB)pStructB->ListB.Flink;
			RtlFreeHeap(g_hProcessHeap, 0, pStructB);
			pStructB = psb;	
		} while(pStructB != pStruct_4457D0);
		pStruct_4457D0 = NULL;
	}

	if(hLibUser32) FreeLibrary(hLibUser32);
	NtTerminateProcess(NtCurrentProcess(), 0);
    Beep(0xBEEB, 0xDEAD);
}