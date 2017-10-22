#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#define NOT_SUPPORTED	0
#define WIN2K			1
#define WINXP			2

#include <Rpc.h>
#include "Constdef.h"

extern BYTE byte_4457C0;
extern PWSTR wsz_445E18, wsz_445EF4;
extern BOOL bool_IsOsWin2003, bool_445E2C, bool_445E1C, bool_DeviceNoAccess;
extern BOOL bool_445EE4, bool_IsUninstall, bool_445D24, bool_4444C4, bool_IsAutorun;
extern BOOL bool_445D14, bool_445868, bool_445D18, bool_445D20, bool_445D10, bool_445818;
extern DWORD dword_4457F0, dword_445E20, dword_4457EC, dword_4443C0, dword_44438C, dword_44586C;
extern UINT message_445828, message_44582C, message_445830, message_445834;
extern UINT message_445838, message_44583C, message_445840, message_445844;
extern UINT message_445848, message_44584C, message_445850, message_445854;
extern UINT message_445858, message_44585C, message_445860, message_445864;

extern PSTRUCTB pStruct_4457C4, pStruct_4457D0;
extern PSTRUCTC pStruct_445EE0, pStruct_445ED8;
extern OSVERSIONINFOW osvi_4455AC, osvi_444278;
extern RTL_CRITICAL_SECTION rcs_4457D4, rcs_4443D8, rcs_444398;
extern PSTRUCTA pStruct_4457B4, pStruct_445E14;
extern STRUCTJ struct_44425C;

extern HINSTANCE hLibUser32;
extern HANDLE hEvent_445E28, hEvent_445EDC, hEvent_445810, hEvent_445D04, hEvent_445D00;
extern HANDLE hEvent_Exit;
extern HANDLE g_hProcessHeap;
extern HANDLE g_hCurrentProcess;
extern HANDLE g_hDeviceFile;
extern HKEY hkey_445CF4, hkey_445CF8;
extern HWND hWnd_4457FC, btnCancel, hWndMain;

__inline PPEB GetPEB() {
	PPEB peb;
	__asm	mov eax, fs:[30h];
	__asm	mov peb, eax;
	return peb;
}

void sub_40ADF0(PWSTR* pwsCurDir);
BOOL sub_40ABF0();
BOOL sub_40AD50(PWSTR str);
int sub_40AAF0(PWSTR str, UINT uType, HWND hWndParent);
NTSTATUS sub_410FC0(UUID* Uuid);
NTSTATUS sub_411290(BYTE* arg_0, DWORD arg_4, DWORD* len);
BOOL sub_410F00(BYTE* buf, DWORD len);
void sub_410B70();
NTSTATUS sub_4106F0(PSTRUCTC structc);
BOOL sub_410F80(BYTE* buf, DWORD length);
BOOL SetAutoRun();
void RegisterMessages();
BOOLEAN sub_43EE70(PSTRUCTK psk);
void sub_40FE80(PDWORD pdwDrive);
BOOLEAN sub_43F2F0(PDWORD pdwDrive);
PWSTR sub_43F470(PWSTR wsPath);
void sub_43EF10(PSTRUCTK psk);
void sub_43EF70(PSTRUCTK psk);
DWORD sub_410D10();
DWORD sub_410D90(BYTE* pbBuffer, DWORD dwLen);
BOOLEAN sub_43EFF0(PSTRUCTK psk);
PWSTR sub_43F4D0(PWSTR str, PDWORD pdwDrive);
DWORD sub_410E10(BYTE* buf, DWORD len);
BOOLEAN sub_410B90();
BOOLEAN sub_410BF0();
DWORD sub_410E80();
BOOLEAN sub_43F270(PSTRUCTK psk);
BOOLEAN sub_43F150(PSTRUCTK psk);
BOOLEAN sub_410CD0(BYTE* buf, DWORD len);
DWORD GetOSVersion();

#endif // _GLOBAL_H_