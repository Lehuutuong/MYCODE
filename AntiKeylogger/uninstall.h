#ifndef _UNINSTALL_H_
#define _UNINSTALL_H_

#include <winsvc.h>

DWORD sub_437FA0(HWND hWnd, BOOL bFlag);
DWORD sub_410C50();
BOOL sub_437E20();
DWORD sub_437AE0();
DWORD sub_437C30(SC_HANDLE hService);
DWORD sub_437CE0(SC_HANDLE hService, DWORD* pdw);
DWORD sub_43AC50(HKEY hKey, PWSTR lpSubKey);
void sub_43A9C0(PWSTR str);
void UninstallDriver();

#endif