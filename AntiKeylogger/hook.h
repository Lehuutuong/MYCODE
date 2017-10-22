#ifndef _HOOK_H_
#define _HOOK_H_

#include "Constdef.h"

PSTRUCTA sub_40F4E0();
PSTRUCTA sub_40F640();
void sub_40F6A0(PSTRUCTA pStruct);
void sub_40E3E0(DWORD* pdw);
PVOID* sub_40E610(char* szDllName, char* szFuncName, DWORD* BaseAddress);
BOOL sub_40F760(PSTRUCTA pStruct, BOOL bFlag, DWORD* BaseAddress);
BOOL sub_40F400(PWSTR FullDllName, PWSTR BaseDllName, PDWORD BaseAddress, DWORD* pdw);
NTSTATUS NTAPI sub_40ED50(PWCHAR PathToFile, ULONG Flags, PUNICODE_STRING ModuleFileName, PHANDLE ModuleHandle);
DWORD sub_40EBC0(BOOL bFlag, PWSTR wsFileName, DWORD* arg_8);
HRESULT sub_40E6A0(DWORD* pdw, PWSTR wsFileName);
HRESULT sub_40EA30(PWSTR wsFileName, DWORD* pdw);
DWORD sub_406360(PWSTR wsFileName, DWORD dwMask, DWORD dwFlag);
DWORD sub_43F9A0(PWSTR strFile);
BOOL sub_40BAD0(PWSTR wsFileName);
void sub_40B800();

#endif