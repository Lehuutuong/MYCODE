#include "StdAfx.h"
#include "Global.h"
#include "Comm.h"
#include "ntstatus.h"
#include "Resource.h"

void sub_40ADF0(PWSTR* pwsCurDir)
{
	DWORD len;
	PPEB peb = GetPEB();
	PRTL_USER_PROCESS_PARAMETERS pRUPP = peb->ProcessParameters;

	if(pRUPP->ImagePathName.Buffer != NULL)
		len = wcslen(pRUPP->ImagePathName.Buffer);
	PWSTR str = (PWSTR)RtlAllocateHeap(g_hProcessHeap, HEAP_ZERO_MEMORY, 2 * (len + 1));
	wcscpy(str, pRUPP->ImagePathName.Buffer);
	PWSTR p = wcsrchr(str, '\\');
	p[0] = 0;
	SetCurrentDirectoryW(str);
	
	if(pwsCurDir == NULL)
		RtlFreeHeap(g_hProcessHeap, 0, str);
	else
		*pwsCurDir = str;
}

BOOL sub_40ABF0()
{
	HANDLE TokenHandle = NULL;
	DWORD ReturnLength = 0;
	LUID Luid;
	PTOKEN_PRIVILEGES TokenInformation;
	BOOL ret = FALSE;

	if(!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &TokenHandle))
		return FALSE;

	LookupPrivilegeValueW(NULL, L"SeLoadDriverPrivilege", &Luid);
	GetTokenInformation(TokenHandle, TokenPrivileges, NULL, 0, &ReturnLength);
	SetLastError(0);

	if(ReturnLength == 0)
	{
		CloseHandle(TokenHandle);
		return FALSE;
	}

	TokenInformation = (PTOKEN_PRIVILEGES)RtlAllocateHeap(g_hProcessHeap, HEAP_ZERO_MEMORY, ReturnLength);
	if(!GetTokenInformation(TokenHandle, TokenPrivileges, TokenInformation, ReturnLength, &ReturnLength) || 
		TokenInformation->PrivilegeCount == 0)
	{
		RtlFreeHeap(g_hProcessHeap, 0, TokenInformation);
		CloseHandle(TokenHandle);
		return ret;
	}

	for(int i = 0 ; i < (int)TokenInformation->PrivilegeCount ; i ++ )
	{
		if(memcmp(&Luid, &TokenInformation->Privileges[i].Luid, sizeof(LUID)) == 0)
		{
			if(TEST(TokenInformation->Privileges[i].Attributes, 3))
				ret = TRUE;
			break;
		}
	}

	RtlFreeHeap(g_hProcessHeap, 0, TokenInformation);
	CloseHandle(TokenHandle);
	return ret;
}

BOOL sub_40AD50(PWSTR str)
{
	HANDLE TokenHandle;
	LUID Luid;
	TOKEN_PRIVILEGES NewState;
	BOOL ret = FALSE;

	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &TokenHandle);
	if(!LookupPrivilegeValueW(NULL, str, &Luid))
	{
		CloseHandle(TokenHandle);
		return FALSE;
	}

	NewState.PrivilegeCount = 1;
	NewState.Privileges[0].Luid.LowPart = Luid.LowPart;
	NewState.Privileges[0].Luid.HighPart = Luid.HighPart;
	NewState.Privileges[0].Attributes = 2;
	ret = AdjustTokenPrivileges(TokenHandle, FALSE, &NewState, 0x10, NULL, NULL);

	CloseHandle(TokenHandle);
	return ret;
}

int sub_40AAF0(PWSTR str, UINT uType, HWND hWndParent)
{
//	HICON hIcon = sub_40AA60(0x20, 0x20);
	HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_TRAY_ACTIVE));
	DWORD dwStyle = (hWndParent == NULL) ? WS_OVERLAPPED : WS_CHILD;

	HWND hWnd = CreateWindowExW(0, L"#32770", NULL, dwStyle, 0, 0, 0, 0, hWndParent, NULL, NULL, NULL);

	HICON hOldIcon = (HICON)GetClassLongW(hWnd, GCL_HICON);
	SetClassLongW(hWnd, GCL_HICON, (LONG)hIcon);

	if(hWndParent)
		EnableWindow(hWndParent, FALSE);

	int ret = MessageBoxW(hWnd, str, L"AntiKeylogger", uType);

	if(hWndParent)
		EnableWindow(hWndParent, TRUE);

	SetClassLongW(hWnd, GCL_HICON, (LONG)hOldIcon);
	DestroyIcon(hIcon);
	DestroyWindow(hWnd);

	return ret;
}

NTSTATUS sub_410FC0(UUID* Uuid)
{
	PSTRUCTD structd = sub_4107C0(&Uuid, 4);
	if(structd == NULL)
		return STATUS_NO_MEMORY;

	PSTRUCTD var_4 = NULL;
	structd->Code = 0x0B;
	NTSTATUS ns = sub_410530(pStruct_445EE0, NULL, &var_4, structd);
	if(var_4 == NULL)
		return ns;

	if(var_4->BufferBase != NULL)
		RtlFreeHeap(GetProcessHeap(), 0, var_4->BufferBase);
	RtlFreeHeap(GetProcessHeap(), 0, var_4);
	return ns;
}

NTSTATUS sub_411290(BYTE* arg_0, DWORD arg_4, DWORD* len)
{
	DWORD var_8[2];
	PSTRUCTD var_C = NULL;

	var_8[0] = (DWORD)arg_0;
	var_8[1] = arg_4;
	PSTRUCTD structd = sub_4107C0(var_8, 8);
	structd->Code = 0x13;
	NTSTATUS ns = sub_410530(pStruct_445EE0, NULL, &var_C, structd);

	if(ns != STATUS_BUFFER_TOO_SMALL)
	{
		if(len)	*len = 0;
	}
	else
	{
		if(len)	*len = *((PDWORD)var_C->BufferBase);
	}

	if(var_C == NULL)
		return ns;

	if(var_C->BufferBase)
		RtlFreeHeap(GetProcessHeap(), 0, var_C->BufferBase);
	RtlFreeHeap(GetProcessHeap(), 0, var_C);
	return ns;
}

BOOL sub_410F00(BYTE* buf, DWORD len)
{
	DWORD var_8[2];
	PSTRUCTD var_C = NULL;
	BOOL bRet = FALSE;

	var_8[0] = (DWORD)buf;
	var_8[1] = len;
	PSTRUCTD structd = sub_4107C0(var_8, 8);
	if(structd == NULL)
		return bRet;

	structd->Code = 0x9;
	if(!NT_SUCCESS(sub_410530(pStruct_445EE0, NULL, &var_C, structd)))
		return bRet;

	bRet = (BOOL)var_C->Code;
	if(var_C->BufferBase)
		RtlFreeHeap(GetProcessHeap(), 0, var_C->BufferBase);
	RtlFreeHeap(GetProcessHeap(), 0, var_C);

	return bRet;
}

void sub_410B70()
{
	if(pStruct_445EE0)
		sub_4106F0(pStruct_445EE0);
}

NTSTATUS sub_4106F0(PSTRUCTC structc)
{
	NTSTATUS ns = ZwWaitForSingleObject(hEvent_445EDC, FALSE, 0);
	ULONG FreeSize;
	
	if(!NT_SUCCESS(ns))
		return ns;

	PSTRUCTC temp = pStruct_445ED8;
	if(temp) {
		do {
			if(temp == structc) goto loc_41073F;
			temp = (PSTRUCTC)temp->ListC.Flink;
		} while(temp != pStruct_445ED8);
	}
	ZwSetEvent(hEvent_445EDC, NULL);
	return STATUS_INVALID_HANDLE;

loc_41073F:
	ZwClose(structc->PortHandle);
	if(structc->ThreadHandle)
	{
		ZwTerminateThread(structc->ThreadHandle, STATUS_SUCCESS);
		ZwClose(structc->ThreadHandle);
	}

	ZwFreeVirtualMemory(NtCurrentProcess(), &structc->Buffer, &FreeSize, MEM_RELEASE);
	temp = NULL;
	if(structc->ListC.Flink != (PLIST_ENTRY)structc)
	{
		temp = (PSTRUCTC)structc->ListC.Flink;
		temp->ListC.Blink = structc->ListC.Blink;
		structc->ListC.Blink->Flink = (PLIST_ENTRY)temp;
	}
	RtlFreeHeap(GetProcessHeap(), 0, structc);
	pStruct_445ED8 = temp;
	ZwSetEvent(hEvent_445EDC, NULL);

	return STATUS_SUCCESS;
}

BOOL sub_410F80(BYTE* buf, DWORD length)
{
	PSTRUCTD structd = sub_4107C0(buf, length);
	if(structd == NULL) return FALSE;

	structd->Code = 0xA;
	if(!NT_SUCCESS(sub_410530(pStruct_445EE0, NULL, NULL, structd)))
		return FALSE;

	return TRUE;
}

BOOL SetAutoRun()
{
	HKEY hKey;
	PPEB peb = GetPEB();
	DWORD len;
	PWSTR str;

	if(bool_445D14 == FALSE) return FALSE;

	if(RegOpenKeyW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &hKey) != ERROR_SUCCESS)
		return FALSE;
	len = wcslen(peb->ProcessParameters->ImagePathName.Buffer);
	len += wcslen(L"/autorun");
	str = (PWSTR)RtlAllocateHeap(g_hProcessHeap, 8, len * 2 + 0x14);
	wcscpy(str, peb->ProcessParameters->ImagePathName.Buffer);
	wcscat(str, L" /autorun");
	len = wcslen(str) * 2 + 2;
	RegSetValueExW(hKey, L"AntiKeylogger", 0, REG_SZ, (BYTE*)str, len);
	RtlFreeHeap(g_hProcessHeap, 0, str);
	RegCloseKey(hKey);
	return TRUE;
}

void RegisterMessages()
{
	UUID Uuid;
	PWSTR StringUuid;
	UINT MessageId;
	
/*	UuidCreate(&Uuid);
	UuidToStringW(&Uuid, &StringUuid);
	MessageId = RegisterWindowMessageW(StringUuid);
	RpcMgmtStatsVectorFree((RPC_STATS_VECTOR**)&StringUuid);
	message_445828 = MessageId;*/

	UuidCreate(&Uuid);
	UuidToStringW(&Uuid, &StringUuid);
	MessageId = RegisterWindowMessageW(StringUuid);
	RpcMgmtStatsVectorFree((RPC_STATS_VECTOR**)&StringUuid);
	message_44582C = MessageId;

	UuidCreate(&Uuid);
	UuidToStringW(&Uuid, &StringUuid);
	MessageId = RegisterWindowMessageW(StringUuid);
	RpcMgmtStatsVectorFree((RPC_STATS_VECTOR**)&StringUuid);
	message_445830 = MessageId;
	
	UuidCreate(&Uuid);
	UuidToStringW(&Uuid, &StringUuid);
	MessageId = RegisterWindowMessageW(StringUuid);
	RpcMgmtStatsVectorFree((RPC_STATS_VECTOR**)&StringUuid);
	message_445834 = MessageId;

	UuidCreate(&Uuid);
	UuidToStringW(&Uuid, &StringUuid);
	MessageId = RegisterWindowMessageW(StringUuid);
	RpcMgmtStatsVectorFree((RPC_STATS_VECTOR**)&StringUuid);
	message_445838 = MessageId;

	UuidCreate(&Uuid);
	UuidToStringW(&Uuid, &StringUuid);
	MessageId = RegisterWindowMessageW(StringUuid);
	RpcMgmtStatsVectorFree((RPC_STATS_VECTOR**)&StringUuid);
	message_44583C = MessageId;

	UuidCreate(&Uuid);
	UuidToStringW(&Uuid, &StringUuid);
	MessageId = RegisterWindowMessageW(StringUuid);
	RpcMgmtStatsVectorFree((RPC_STATS_VECTOR**)&StringUuid);
	message_445844 = MessageId;

	UuidCreate(&Uuid);
	UuidToStringW(&Uuid, &StringUuid);
	MessageId = RegisterWindowMessageW(StringUuid);
	RpcMgmtStatsVectorFree((RPC_STATS_VECTOR**)&StringUuid);
	message_445848 = MessageId;

	UuidCreate(&Uuid);
	UuidToStringW(&Uuid, &StringUuid);
	MessageId = RegisterWindowMessageW(StringUuid);
	RpcMgmtStatsVectorFree((RPC_STATS_VECTOR**)&StringUuid);
	message_44584C = MessageId;

	UuidCreate(&Uuid);
	UuidToStringW(&Uuid, &StringUuid);
	MessageId = RegisterWindowMessageW(StringUuid);
	RpcMgmtStatsVectorFree((RPC_STATS_VECTOR**)&StringUuid);
	message_445850 = MessageId;
	
	UuidCreate(&Uuid);
	UuidToStringW(&Uuid, &StringUuid);
	MessageId = RegisterWindowMessageW(StringUuid);
	RpcMgmtStatsVectorFree((RPC_STATS_VECTOR**)&StringUuid);
	message_445854 = MessageId;

	UuidCreate(&Uuid);
	UuidToStringW(&Uuid, &StringUuid);
	MessageId = RegisterWindowMessageW(StringUuid);
	RpcMgmtStatsVectorFree((RPC_STATS_VECTOR**)&StringUuid);
	message_445858 = MessageId;

	UuidCreate(&Uuid);
	UuidToStringW(&Uuid, &StringUuid);
	MessageId = RegisterWindowMessageW(StringUuid);
	RpcMgmtStatsVectorFree((RPC_STATS_VECTOR**)&StringUuid);
	message_44585C = MessageId;

	UuidCreate(&Uuid);
	UuidToStringW(&Uuid, &StringUuid);
	MessageId = RegisterWindowMessageW(StringUuid);
	RpcMgmtStatsVectorFree((RPC_STATS_VECTOR**)&StringUuid);
	message_445860 = MessageId;
	
	UuidCreate(&Uuid);
	UuidToStringW(&Uuid, &StringUuid);
	MessageId = RegisterWindowMessageW(StringUuid);
	RpcMgmtStatsVectorFree((RPC_STATS_VECTOR**)&StringUuid);
	message_445864 = MessageId;

	message_445840 = RegisterWindowMessageW(L"TaskbarCreated");
}

BOOLEAN sub_43EE70(PSTRUCTK psk)
{
	sub_40FE80(&psk->dwDriveCount2C);
	if(!sub_43F2F0(&psk->dwDriveCount2C)) return FALSE;
	sub_43EF10(psk);
	if(psk->dw18 == 0) RtlEnterCriticalSection(&psk->rcs00);
	psk->dwLength20 = sub_410D10();
	if(psk->dwLength20) {
		psk->pbBuffer1C = (BYTE*)RtlAllocateHeap(g_hProcessHeap, HEAP_ZERO_MEMORY, psk->dwLength20);
		if(sub_410D90(psk->pbBuffer1C, psk->dwLength20)) {
			if(!sub_43EFF0(psk)) {
				if(psk->dw18 == 0) RtlLeaveCriticalSection(&psk->rcs00);
				sub_43EF10(psk);
				return FALSE;
			}
		}
	}
	if(psk->dw18 == 0) RtlLeaveCriticalSection(&psk->rcs00);
	return TRUE;
}

void sub_40FE80(PDWORD pdwDrive)
{
	PDWORD pdwRealBuffer, pdwBuffer = (PDWORD)pdwDrive[1];
	DWORD dwCount;
	if(pdwBuffer) {
		dwCount = *(pdwBuffer - 1);
		pdwRealBuffer = pdwBuffer - 1;
		pdwBuffer += (dwCount * 4 + 2);
		while(dwCount > 0) {
			pdwBuffer -= 4;
			if(*pdwBuffer) RtlFreeHeap(g_hProcessHeap, 0, (BYTE*)(*pdwBuffer));
			dwCount --;
		}
		RtlFreeHeap(g_hProcessHeap, 0, pdwRealBuffer);
	}
	pdwDrive[0] = 0;
	pdwDrive[1] = 0;
}

BOOLEAN sub_43F2F0(PDWORD pdwDrive)
{
	DWORD dwDriveMask, *pdwBuffer, i, j;
	WCHAR wsDrive[3] = L" :";
	WCHAR wsPath[MAX_PATH];
	
	dwDriveMask = GetLogicalDrives();
	for(i = 0; i < 32; i ++) {
		if(TEST(dwDriveMask, (1 << i))) pdwDrive[0] ++;
	}
	if(pdwDrive[0] == 0) return FALSE;
	pdwBuffer = (PDWORD)RtlAllocateHeap(g_hProcessHeap, HEAP_ZERO_MEMORY, pdwDrive[0] * 16 + 4);
	pdwBuffer[0] = pdwDrive[0];
	pdwBuffer += 1;
	pdwDrive[1] = (DWORD)pdwBuffer;
	for(i = 0; i < pdwDrive[0]; i ++) {
		wcscpy((PWSTR)pdwBuffer, wsDrive);
		pdwBuffer[2] = 0;
		pdwBuffer[3] = 0;
		pdwBuffer += 4;
	}
	pdwBuffer = (PDWORD)pdwDrive[1];
	i = 0; j = 0;
	do {
		if(TEST(dwDriveMask, (1 << i))) {
			wsDrive[0] = WCHAR(i + 'A');
			if(QueryDosDeviceW(wsDrive, wsPath, MAX_PATH) > 0) {
				*((PWORD)pdwBuffer) = wsDrive[0];
				pdwBuffer[3] = wcslen(wsPath);
				pdwBuffer[2] = (DWORD)sub_43F470(wsPath);
				pdwBuffer += 4;
			}
			j ++;
		}
		i ++;
	} while(j < pdwDrive[0]);
	return TRUE;
}

PWSTR sub_43F470(PWSTR wsPath)
{
	DWORD dwLen;
	PWSTR wsPathName;

	dwLen = wcslen(wsPath);
	wsPathName = (PWSTR)RtlAllocateHeap(g_hProcessHeap, HEAP_ZERO_MEMORY, 2 * dwLen + 2);
	wcscpy(wsPathName, wsPath);
	return wsPathName;
}

void sub_43EF10(PSTRUCTK psk)
{
	if(psk->dw18 == 0) RtlEnterCriticalSection(&psk->rcs00);
	psk->dw18 = 1;
	psk->dwLength20 = 0;
	if(psk->pbBuffer1C) {
		RtlFreeHeap(g_hProcessHeap, 0, psk->pbBuffer1C);
		psk->pbBuffer1C = NULL;
	}
	sub_43EF70(psk);
	psk->dw18 = 0;
	RtlLeaveCriticalSection(&psk->rcs00);
}

void sub_43EF70(PSTRUCTK psk)
{
	if(psk->pdwNewBuffer24 == NULL) return;
	for(DWORD i = 0; i < psk->dwCount28; i ++){
		RtlFreeHeap(g_hProcessHeap, 0, (PVOID)(psk->pdwNewBuffer24[i * 4 + 1]));
		RtlFreeHeap(g_hProcessHeap, 0, (PVOID)(psk->pdwNewBuffer24[i * 4 + 2]));
	}
	RtlFreeHeap(g_hProcessHeap, 0, psk->pdwNewBuffer24);
	psk->pdwNewBuffer24 = NULL;
	return;
}

DWORD sub_410D10()
{
	DWORD dwRet;
	PSTRUCTD var_4, structd = (PSTRUCTD)RtlAllocateHeap(g_hProcessHeap, HEAP_ZERO_MEMORY, 0x18);
	
	structd->Code = 0x05;
	if(!NT_SUCCESS(sub_410530(pStruct_445EE0, NULL, &var_4, structd))) return 0;
	dwRet = *((PDWORD)var_4->BufferBase);
	if(var_4->BufferBase) RtlFreeHeap(g_hProcessHeap, 0, var_4->BufferBase);
	RtlFreeHeap(g_hProcessHeap, 0, var_4);
	
	return dwRet;
}

DWORD sub_410D90(BYTE* pbBuffer, DWORD dwLen)
{
    DWORD dwRet, var_8[2];
	var_8[0] = (DWORD)pbBuffer;
	var_8[1] = dwLen;
	PSTRUCTD var_C, pstructd = sub_4107C0(var_8, 8);
    
	pstructd->Code = 0x06;
	if(!NT_SUCCESS(sub_410530(pStruct_445EE0, NULL, &var_C, pstructd))) return 0;
	dwRet = var_C->Code;
	if(var_C->BufferBase) RtlFreeHeap(g_hProcessHeap, 0, var_C->BufferBase);
	RtlFreeHeap(g_hProcessHeap, 0, var_C);

	return dwRet;
}

BOOLEAN sub_43EFF0(PSTRUCTK psk)
{
	DWORD dwLen, dwCount = 0, i;
	BYTE* buf = psk->pbBuffer1C;
	BYTE* tmp;
	PWSTR str, str1;

	dwLen = *((DWORD*)buf);
	while(dwLen) {
		buf += dwLen;
		dwLen = *((DWORD*)buf);
		dwCount ++;
	}
	buf = (BYTE*)RtlAllocateHeap(g_hProcessHeap, HEAP_ZERO_MEMORY, dwCount * 16);
	psk->pdwNewBuffer24 = (PDWORD)buf;
	for(i = 0; i < dwCount; i ++) {
		buf[0] = 0;
		buf += 16;
	}
	buf = psk->pbBuffer1C;
	dwLen = *((DWORD*)buf);
	dwCount = 0;
	while(dwLen) {
		tmp = (BYTE*)(psk->pdwNewBuffer24 + dwCount * 4);
		tmp[1] = *((DWORD*)(buf + 4)) ? 1 : 0;
		tmp[2] = ((*((DWORD*)(buf + 8)) & 0x20000) == 0x20000) ? 1 : 0;
		str = (PWSTR)(buf + 0x20);
		dwLen = wcslen(str);
		str1 = (PWSTR)RtlAllocateHeap(g_hProcessHeap, HEAP_ZERO_MEMORY, dwLen * 2 + 2);
		psk->pdwNewBuffer24[dwCount * 4 + 2] = (DWORD)str1;
		wcscpy(str1, str);
		psk->pdwNewBuffer24[dwCount * 4 + 1] = (DWORD)sub_43F4D0(str1, &psk->dwDriveCount2C);
		psk->pdwNewBuffer24[dwCount * 4 + 3] = *((DWORD*)(buf + 0xC));
		buf += *((DWORD*)buf);
		dwLen = *((DWORD*)buf);
		dwCount ++;
	}
	psk->dwCount28 = dwCount;
	psk->dwLength20 = 0;
	if(psk->pbBuffer1C) {
		RtlFreeHeap(g_hProcessHeap, 0, psk->pbBuffer1C);
		psk->pbBuffer1C = NULL;
	}
	return TRUE;
}

PWSTR sub_43F4D0(PWSTR str, PDWORD pdwDrive)
{
	DWORD i, dwLen = 0, *pdwBuffer;
	WCHAR wsDrive[0x400];
	if(str) dwLen = wcslen(str);
	pdwBuffer = (PDWORD)pdwDrive[1];
	for(i = 0; i < pdwDrive[0]; i ++) {
		if(dwLen >= pdwBuffer[i * 4 + 3]) {
			if(wcsnicmp((PWSTR)pdwBuffer[i * 4 + 2], str, pdwBuffer[i * 4 + 3]) == 0) {
				wsprintfW(wsDrive, L"%s%s", (PWSTR)&pdwBuffer[i * 4], str + pdwBuffer[i * 4 + 3]);
				return sub_43F470(wsDrive);
			}
		}
	}
	if(dwLen <= 0x18) return NULL;
	if(wcsnicmp(L"\\Device\\LanmanRedirector", str, 0x18) != 0) return NULL;
	i = 0x18;
	if(str[0x19] == ';') {
		for(i = 0x1A; i < dwLen; i ++) {
			if(str[i] == '\\') break;
		}
	}
	wsprintfW(wsDrive, L"\\%s", str + i);
	return sub_43F470(wsDrive);
}

DWORD sub_410E10(BYTE* buf, DWORD len)
{
	PSTRUCTD var_4 = NULL;
	DWORD dwRet = 0;

	PSTRUCTD structd = sub_4107C0(buf, len);
	if(structd == NULL)
		return dwRet;

	structd->Code = 0x7;
	if(!NT_SUCCESS(sub_410530(pStruct_445EE0, NULL, &var_4, structd)))
		return dwRet;

	dwRet = *((DWORD*)var_4->BufferBase);
	if(var_4->BufferBase)
		RtlFreeHeap(GetProcessHeap(), 0, var_4->BufferBase);
	RtlFreeHeap(GetProcessHeap(), 0, var_4);

	return dwRet;
}

BOOLEAN sub_410B90()
{
	PSTRUCTD psd = (PSTRUCTD)RtlAllocateHeap(g_hProcessHeap, 0, 0x18);
	
	psd->Status = 0;
	psd->BufferBase = NULL;
	psd->BufferLength = 0;
	psd->Code = 0x1;
	return NT_SUCCESS(sub_410530(pStruct_445EE0, NULL, NULL, psd));
}

BOOLEAN sub_410BF0()
{
	PSTRUCTD psd = (PSTRUCTD)RtlAllocateHeap(g_hProcessHeap, 0, 0x18);
	
	psd->Status = 0;
	psd->BufferBase = NULL;
	psd->BufferLength = 0;
	psd->Code = 0x2;
	return NT_SUCCESS(sub_410530(pStruct_445EE0, NULL, NULL, psd));
}

DWORD sub_410E80()
{
	PSTRUCTD var_4, psd = (PSTRUCTD)RtlAllocateHeap(g_hProcessHeap, 0, 0x18);
	DWORD dwRet;
	
	psd->Status = 0;
	psd->BufferBase = NULL;
	psd->BufferLength = 0;
	psd->Code = 0x8;
	if(!NT_SUCCESS(sub_410530(pStruct_445EE0, NULL, &var_4, psd))) return 0;
	dwRet = *((DWORD*)var_4->BufferBase);
	if(var_4->BufferBase)
		RtlFreeHeap(GetProcessHeap(), 0, var_4->BufferBase);
	RtlFreeHeap(GetProcessHeap(), 0, var_4);
	return dwRet;
}

BOOLEAN sub_43F270(PSTRUCTK psk)
{
	if(psk->dw18 == 0) RtlEnterCriticalSection(&psk->rcs00);
	if(psk->dwCount28 > 0 && psk->pdwNewBuffer24 != NULL) {
		sub_43F150(psk);
		sub_410CD0(psk->pbBuffer1C, psk->dwLength20);
	}
	if(psk->dw18 == 0) RtlLeaveCriticalSection(&psk->rcs00);
	return TRUE;
}

BOOLEAN sub_43F150(PSTRUCTK psk)
{
	DWORD dwLen = 0;
	BYTE *buf, *tmp;
	PWSTR str;
	for(int i = 0; i < psk->dwCount28; i ++) {
		str = (PWSTR)psk->pdwNewBuffer24[i * 4 + 2];
		dwLen += (wcslen(str) * 2 + 0x22);
	}
	dwLen += 4;
	buf = (BYTE*)RtlAllocateHeap(g_hProcessHeap, HEAP_ZERO_MEMORY, dwLen);
	psk->pbBuffer1C = buf;
	psk->dwLength20 = dwLen;
	RtlZeroMemory(psk->pbBuffer1C, dwLen);
	for(i = 0; i < psk->dwCount28; i ++) {
		tmp = (BYTE*)&psk->pdwNewBuffer24[i * 4];
		*((DWORD*)(buf + 4)) = tmp[1];
		if(tmp[0]) *((DWORD*)(buf + 4)) = tmp[1] + 1;
		str = (PWSTR)psk->pdwNewBuffer24[i * 4 + 2];
		dwLen = wcslen(str) * 2;
		memcpy(buf + 0x20, str, dwLen);
		*((DWORD*)buf) = dwLen + 0x22;
		buf += (dwLen + 0x22);
	}

	return TRUE;
}

BOOLEAN sub_410CD0(BYTE* buf, DWORD len)
{
	PSTRUCTD structd = sub_4107C0(buf, len);
	if(structd == NULL)
		return FALSE;

	structd->Code = 0x4;
	return NT_SUCCESS(sub_410530(pStruct_445EE0, NULL, NULL, structd));
}

DWORD GetOSVersion()
{
	DWORD dwVersion = NOT_SUPPORTED;
	OSVERSIONINFO ov;
	ov.dwOSVersionInfoSize = sizeof(ov);
	GetVersionEx(&ov);
	if(ov.dwMajorVersion == 5) {
		if(ov.dwMinorVersion == 0)
			dwVersion = WIN2K;
		else if(ov.dwMinorVersion == 1)
			dwVersion = WINXP;
	}
	return dwVersion;
}