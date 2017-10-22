#include "StdAfx.h"
#include "ntdll.h"
#include "global.h"
#include "uninstall.h"
#include <winerror.h>
#include "ntstatus.h"
#include "Comm.h"

BOOL bool_445D18 = FALSE;

DWORD sub_437FA0(HWND hWnd, BOOL bFlag)
{
//	HANDLE hEvent;
	NTSTATUS ns;
	PWSTR wsDir;
	HKEY hKey, hSubKey, hSubKey1;
	DWORD cbData, dwIndex = 0;
	STICKYKEYS StickyKeys;

//	hEvent = OpenEventW(EVENT_ALL_ACCESS, FALSE, L"{559EC6E0-E59C-4972-B004-05A5CC533B23}");
//	if(hEvent == NULL)
//		hEvent = CreateEventW(NULL, TRUE, FALSE, L"{559EC6E0-E59C-4972-B004-05A5CC533B23}");

	if(bFlag == TRUE) {
#ifdef _UNINSTALL_NOTIFY
		if(sub_40AAF0(L"Select Yes to uninstall AntiKeylogger. Select the No button to exit the uninstallation.", MB_SYSTEMMODAL | MB_YESNO, hWnd) != IDYES) {
//			CloseHandle(hEvent);
			if(hEvent_445D04)
				SetEvent(hEvent_445D04);
			return ERROR_CANCELLED;
		}
#endif	//	_UNINSTALL_NOTIFY
	}

	if(hEvent_445D04 == NULL)
		hEvent_445D04 = CreateEventW(NULL, TRUE, FALSE, L"SHKeyPro_Uninstall");
    
//	SetEvent(hEvent);
	Sleep(2000);
	bool_445D18 = TRUE;
	if(bool_445E1C == TRUE) {
		ns = sub_410C50();
		if(ns == STATUS_DEVICE_BUSY) {
//			sub_40AAF0(L"Warning: It is impossible to uninstall AntiKeylogger now, because the program is being used at the moment. \
//				To properly remove this program, switch to each user and log off before you continue.", MB_SYSTEMMODAL | MB_ICONEXCLAMATION | MB_OK, NULL);
			sub_40AAF0(L"Sorry, it is impossible to uninstall AntiKeylogger now, because the program is busy.", MB_SYSTEMMODAL | MB_ICONEXCLAMATION | MB_OK, NULL);
			return ERROR_SUCCESS;
		}
		if(ns == STATUS_TOO_MANY_LINKS) {
//			sub_40AAF0(L"It is impossible to uninstall the kernel of the AntiKeylogger at the moment.Try to do it a little bit later. \
//				If you see this message again, restart the system and try to uninstall the program.", MB_SYSTEMMODAL | MB_ICONEXCLAMATION | MB_OK, NULL);
			sub_40AAF0(L"Sorry,  uninstalling driver of the AntiKeylogger failed. Try again later.", MB_SYSTEMMODAL | MB_ICONEXCLAMATION | MB_OK, NULL);
			return ERROR_SUCCESS;
		}
		if(ns != ERROR_SUCCESS) 
			return ERROR_SUCCESS;
		sub_410B70();
	}
	PWSTR lpName = (PWSTR)VirtualAlloc(NULL, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
#ifdef _SELF_UNINSTALL
	if(!sub_437E20())
		sub_40AAF0(L"The \"uninstall.log\" file either hasn't been found or is damaged.", MB_OK | MB_ICONHAND | MB_SYSTEMMODAL, hWnd);
	sub_40ADF0(&wsDir);
	MoveFileExW(wsDir, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
	wcscat(wsDir, L"\\uninstall.log");
	DeleteFileW(wsDir);
	RtlFreeHeap(g_hProcessHeap, 0, wsDir);
#endif
	
	ns = sub_437AE0();
	
	while(RegEnumKeyW(HKEY_USERS, dwIndex, lpName, 0x500) == ERROR_SUCCESS) {
		if(RegOpenKeyW(HKEY_USERS, lpName, &hKey) == ERROR_SUCCESS) {
			if(RegOpenKeyW(hKey, L"Software", &hSubKey) == ERROR_SUCCESS) {
				if(RegOpenKeyW(hSubKey, L"SHKeyProtect", &hSubKey1) == ERROR_SUCCESS) {
					sub_43AC50(hSubKey, L"SHKeyProtect");
					RegCloseKey(hSubKey1);
				}
				RegCloseKey(hSubKey);
			}
			RegCloseKey(hKey);
		}
		dwIndex++;
	}

	cbData = 8;
	if(RegQueryValueExW(hkey_445CF8, L"StickyKeys", NULL, NULL, (BYTE*)&StickyKeys, &cbData) == ERROR_SUCCESS)
		SystemParametersInfoW(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &StickyKeys, 0);
	
	if(RegOpenKeyW(HKEY_LOCAL_MACHINE, L"Software", &hKey) == ERROR_SUCCESS)
	{
		if(RegOpenKeyW(hKey, L"SHKeyProtect", &hSubKey) == ERROR_SUCCESS)
		{
			sub_43AC50(hKey, L"SHKeyProtect");
			RegCloseKey(hSubKey);
		}
		RegCloseKey(hKey);
	}
	
	if(RegOpenKeyW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &hKey) == ERROR_SUCCESS)
	{
		RegDeleteValueW(hKey, L"AntiKeylogger");
		RegCloseKey(hKey);
	}
	
	wcscpy(lpName, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
	if(RegOpenKeyW(HKEY_LOCAL_MACHINE, lpName, &hKey) == ERROR_SUCCESS)
	{
		if(RegOpenKeyW(hKey, L"AntiKeylogger", &hSubKey) == ERROR_SUCCESS)
		{
			sub_43AC50(hKey, L"AntiKeylogger");
			RegCloseKey(hSubKey);
		}
		RegCloseKey(hKey);
	}

	if(RegOpenKeyW(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", &hKey) == ERROR_SUCCESS)
	{
		cbData = 0x1000;
		if(RegQueryValueExW(hKey, L"Common Programs", NULL, NULL, (BYTE*)lpName, &cbData) == ERROR_SUCCESS && cbData != 0)
		{
			wcscat(lpName, L"\\");
			wcscat(lpName, L"AntiKeylogger");
			sub_43A9C0(lpName);
		}

		cbData = 0x1000;
		if(RegQueryValueExW(hKey, L"Common Desktop", NULL, NULL, (BYTE*)lpName, &cbData) == ERROR_SUCCESS && cbData != 0)
		{
			wcscat(lpName, L"\\");
			wcscat(lpName, L"AntiKeylogger");
			wcscat(lpName, L".lnk");
			DeleteFileW(lpName);
		}
		RegCloseKey(hKey);
	}

	UninstallDriver();

#ifdef _SELF_UNINSTALL
	if(GetSystemDirectoryW(lpName, 0x1000))
	{
		wcscat(lpName, L"\\");
		wcscat(lpName, L"drivers\\SHKeyPro.sys");
		if(!DeleteFileW(lpName))
			MoveFileExW(lpName, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
	}
#endif

#ifdef _UNINSTALL_NOTIFY
	wcscpy(lpName, L"AntiKeylogger has been successfully uninstalled.");
	sub_40AAF0(lpName, MB_OK | MB_SYSTEMMODAL, NULL);
#endif	//	_UNINSTALL_NOTIFY
	VirtualFree(lpName, 0, MEM_RELEASE);

//	CloseHandle(hEvent);
	if(hEvent_445D04)
		SetEvent(hEvent_445D04);

	if(hEvent_Exit) SetEvent(hEvent_Exit);

	char szBatFile[MAX_PATH], szFileName[MAX_PATH];
	GetSystemDirectory(szBatFile, MAX_PATH);
	int nLen = strlen(szBatFile);
	if(szBatFile[nLen - 1] == '\\') szBatFile[nLen - 1] = '\0';
	strcat(szBatFile, "\\Temp.Bat");
	GetModuleFileName(NULL, szFileName, MAX_PATH);
	FILE *fp = fopen(szBatFile, "w");
	fprintf(fp, "@ECHO OFF\n");
	fprintf(fp, ":START\n");
	fprintf(fp, "DEL \"%s\"\n", szFileName);
	fprintf(fp, "IF EXIST \"%s\" GOTO START\n", szFileName);
	fprintf(fp, "DEL \"%s\"\n", szBatFile);
	fprintf(fp, "EXIT\n");
	fclose(fp);
	WinExec(szBatFile, SW_HIDE);

	return ERROR_SUCCESS;
}

DWORD sub_410C50()
{
	PSTRUCTD esi = (PSTRUCTD)RtlAllocateHeap(g_hProcessHeap, 8, 0x18);
	PSTRUCTD var_4;
	NTSTATUS ns;
	
	esi->Status = 0;
	esi->BufferBase = NULL;
	esi->BufferLength = 0;
	esi->Code = 3;
	if(!NT_SUCCESS(sub_410530(pStruct_445EE0, NULL, &var_4, esi))) return ERROR_SUCCESS;

	ns = *((PDWORD)var_4->BufferBase);
	if(var_4->BufferBase)
		RtlFreeHeap(g_hProcessHeap, 0, var_4->BufferBase);
	RtlFreeHeap(g_hProcessHeap, 0, var_4);

	return ns;    
}

BOOL sub_437E20()
{
	HANDLE hFile;
	DWORD len, dwAttrs;
	PBYTE buf, buf1;
	BOOL bFlag;
	WCHAR* p;

	sub_40ADF0(NULL);
	hFile = CreateFileW(L"uninstall.log", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE) return 0;

	len = GetFileSize(hFile, NULL);
	if(len <= 2) 
	{
		CloseHandle(hFile);
		return 0;
	}

	buf = (BYTE*)VirtualAlloc(NULL, len, 0x1000, 0x40);
	ReadFile(hFile, buf, len, &len, NULL);

	if(buf[0] != 0xFF && buf[1] != 0xFE)
	{
		VirtualFree(buf, 0, 0x8000);
		CloseHandle(hFile);
		return 0;
	}
	buf1 = buf + 2;

loc_437F20:
	p = wcsstr((PWSTR)buf1, L"\r\n");
	if(p) p[0] = 0;
	dwAttrs = GetFileAttributesW((PWSTR)buf1);
	if(TEST(dwAttrs, FILE_ATTRIBUTE_DIRECTORY))
		bFlag = RemoveDirectoryW((PWSTR)buf1);
	else
		bFlag = DeleteFileW((PWSTR)buf1);
	if(bFlag == FALSE)
        MoveFileExW((PWSTR)buf1, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
    if(p)
	{
		buf1 = (BYTE*)p + 4;
		goto loc_437F20;
	}

	VirtualFree(buf, 0, 0x8000);
	CloseHandle(hFile);

	return TRUE;
}

DWORD sub_437AE0()
{
	SC_HANDLE hSCManager, hService;
	DWORD dwRet;

	SetLastError(0);
	hSCManager = OpenSCManagerW(NULL, NULL, 0xF003F);
	if(hSCManager == NULL)
		return GetLastError();

	hService = OpenServiceW(hSCManager, L"SHKeyPro", SERVICE_ALL_ACCESS);
	if(hService == NULL)
	{
		CloseServiceHandle(hSCManager);
		return GetLastError();
	}

	dwRet = sub_437C30(hService);
	if(dwRet != 0)
	{
		CloseServiceHandle(hService);
		CloseServiceHandle(hSCManager);
		return dwRet;
	}

	if(!DeleteService(hService)) dwRet = GetLastError();

    CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);

	return dwRet;
}

DWORD sub_437C30(SC_HANDLE hService)
{
	SERVICE_STATUS status;
	DWORD var_28 = 3, dwResult;

	if(!ControlService(hService, SERVICE_CONTROL_STOP, &status))
		return GetLastError();

    dwResult = sub_437CE0(hService, &var_28);
	if(dwResult)
		return dwResult;

	if(var_28 != 1)
		return GetLastError();

    return 0;
}

DWORD sub_437CE0(SC_HANDLE hService, DWORD* pdw)
{
    SERVICE_STATUS status;
	DWORD esi, edi;

	if(!QueryServiceStatus(hService, &status))
		return GetLastError();

	esi = GetTickCount();
	edi = status.dwCheckPoint;

	while(status.dwCurrentState == *pdw) {
		if(status.dwWaitHint < 10000) status.dwWaitHint = 100000;
		else if(status.dwWaitHint > 100000) status.dwWaitHint = 100000;
		Sleep(status.dwWaitHint);
		if(!QueryServiceStatus(hService, &status))
			return GetLastError();

		if(status.dwCurrentState != *pdw)
		{
			*pdw = status.dwCurrentState;
			return 0;
		}

		if(status.dwCheckPoint > edi) {
			esi = GetTickCount();
			edi = status.dwCheckPoint;
		}
		else
			if(GetTickCount() - esi > status.dwWaitHint) return 0xFFFFFFFF;
	}
	
	*pdw = status.dwCurrentState;
	return 0;
}

DWORD sub_43AC50(HKEY hKey, PWSTR lpSubKey)
{
	HKEY hSubKey, hSubKey1;
	DWORD cbData = 0x1000;

	if(RegOpenKeyW(hKey, lpSubKey, &hSubKey)) return GetLastError();

	PWSTR lpName = (PWSTR)VirtualAlloc(NULL, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	while(RegEnumKeyW(hSubKey, 0, lpName, 0x1000) == ERROR_SUCCESS) {
		if(RegOpenKeyW(hSubKey, lpName, &hSubKey1) == ERROR_SUCCESS) {
			sub_43AC50(hSubKey, lpName);
			RegCloseKey(hSubKey1);
		}		
	}
	while(RegEnumValueW(hSubKey, 0, lpName, &cbData, NULL, NULL, NULL, NULL) ==ERROR_SUCCESS) {
		RegDeleteValueW(hSubKey, lpName);
		cbData = 0x1000;
	}
	VirtualFree(lpName, 0, MEM_RELEASE);
	RegCloseKey(hSubKey);
	if(RegDeleteKeyW(hKey, lpSubKey) != ERROR_SUCCESS) return GetLastError();
	
	return ERROR_SUCCESS;
}

void sub_43A9C0(PWSTR str)
{
	PPEB peb = GetPEB();
	PWSTR edi;
	WIN32_FIND_DATAW FindData;
	HANDLE esi;

	edi = (PWSTR)RtlAllocateHeap(g_hProcessHeap, 8, wcslen(peb->ProcessParameters->CurrentDirectoryPath.Buffer) * 2 + 2);
    wcscpy(edi, peb->ProcessParameters->CurrentDirectoryPath.Buffer);
	
	if(!SetCurrentDirectoryW(str))
	{
		RtlFreeHeap(g_hProcessHeap, 0, edi);
		return;
	}

	esi = FindFirstFileW(L"*.*", &FindData);
    if(esi == INVALID_HANDLE_VALUE)
	{
		SetCurrentDirectoryW(edi);
		if(!RemoveDirectoryW(str))
			MoveFileExW(edi, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
		RtlFreeHeap(g_hProcessHeap, 0, edi);
		return;
	}

	if(wcscmp(FindData.cFileName, L".") && wcscmp(FindData.cFileName, L"..")) {
		if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			SetFileAttributesW(FindData.cFileName, FILE_ATTRIBUTE_NORMAL);
			sub_43A9C0(FindData.cFileName);
		}
		else
		{
			SetFileAttributesW(FindData.cFileName, FILE_ATTRIBUTE_NORMAL);
			if(!DeleteFileW(FindData.cFileName))
				MoveFileExW(FindData.cFileName, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
		}
	}

	while(FindNextFileW(esi, &FindData))
	{
		if(wcscmp(FindData.cFileName, L".") == 0) continue;
		if(wcscmp(FindData.cFileName, L"..") == 0) continue;

		if(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			SetFileAttributesW(FindData.cFileName, FILE_ATTRIBUTE_NORMAL);
			sub_43A9C0(FindData.cFileName);
		}
		else
		{
			SetFileAttributesW(FindData.cFileName, FILE_ATTRIBUTE_NORMAL);
			if(!DeleteFileW(FindData.cFileName))
				MoveFileExW(FindData.cFileName, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
		}
	}

	FindClose(esi);
	SetCurrentDirectoryW(edi);
	if(!RemoveDirectoryW(str))
		MoveFileExW(edi, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
	RtlFreeHeap(g_hProcessHeap, 0, edi);
	return;
}

void UninstallDriver()
{
	ANSI_STRING aStr;
	
	RtlInitAnsiString(&aStr, "\\Registry\\Machine\\System\\CurrentControlSet\\Services\\SHKeyPro");
					
	UNICODE_STRING uStr;

	if(RtlAnsiStringToUnicodeString(&uStr, &aStr, TRUE) == STATUS_SUCCESS)
	{
		ZwUnloadDriver(&uStr);
		RtlFreeUnicodeString(&uStr);
	}

	RegDeleteKey(HKEY_LOCAL_MACHINE, "System\\CurrentControlSet\\Services\\SHKeyPro\\Enum");
	RegDeleteKey(HKEY_LOCAL_MACHINE, "System\\CurrentControlSet\\Services\\SHKeyPro");
}