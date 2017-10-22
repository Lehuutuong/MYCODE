#include "StdAfx.h"
#include "Global.h"
#include "Comm.h"
#include "Constdef.h"
#include "ntdll.h"
#include "ntstatus.h"
#include "hash&crypt.h"
#include "Resource.h"

DWORD dword_4457F0 = 0, dword_445E20, dword_4457EC;
BOOL bool_445E2C = FALSE, bool_445E1C, bool_DeviceNoAccess = FALSE, bool_445EE4 = FALSE;
BOOL bool_IsUninstall = FALSE;
RTL_CRITICAL_SECTION rcs_4457D4;
HANDLE hEvent_445E28 = NULL, hEvent_445EDC = NULL;
HANDLE g_hProcessHeap = NULL;
HANDLE g_hCurrentProcess = NULL;
HANDLE g_hDeviceFile = NULL;
PSTRUCTC pStruct_445EE0 = NULL, pStruct_445ED8 = NULL;
HWND hWndMain = NULL;
BYTE byte_445578[0x10] = { 0x13, 0x5A, 0x03, 0xBC, 0xE0, 0x3A, 0xCC, 0x62, 
						   0x30, 0xC6, 0xF4, 0xC6, 0xAE, 0x8D, 0x1E, 0x12};

void sub_40FA20()
{
	hEvent_445E28 = OpenEventW(EVENT_ALL_ACCESS, FALSE, L"SHKeyPro_Forward");	// 0
	if(hEvent_445E28 == NULL)
	{
		hEvent_445E28 = CreateEventW(NULL, TRUE, FALSE, L"SHKeyPro_Forward");
		bool_445E2C = TRUE;
	}

	RtlInitializeCriticalSection(&rcs_4457D4);
	SetLastError(0);

	dword_445E20 = sub_410020();// 2f7835f3
	if(dword_445E20 != 0 && TEST(dword_445E20, 0x40A00) == 0)
	{
		if(sub_410100(&dword_4457EC) == FALSE)
			return;

		if(TEST(dword_4457EC, 0x10000000) == 0)// 70300
			bool_445E1C = sub_410B10();// 1

		return;
	}
	bool_DeviceNoAccess = (GetLastError() == ERROR_NOACCESS);
}

DWORD sub_410020()
{
	DWORD dwRet = 0, dwBytesReturned = 0;

	if(g_hDeviceFile != NULL)
	{
		CloseHandle(g_hDeviceFile);
		g_hDeviceFile = NULL;
	}

	g_hDeviceFile = LoadDriver();
	if(g_hDeviceFile == INVALID_HANDLE_VALUE) return dwRet;

	Sleep(2000);

	DeviceIoControl(g_hDeviceFile, 0x8400213C, NULL, 0, &dwRet, 4, &dwBytesReturned, 0);

	if(g_hDeviceFile != NULL)
		CloseHandle(g_hDeviceFile);
	g_hDeviceFile = NULL;

	return dwRet;
}

HANDLE LoadDriver()
{
	wchar_t *str = L"\\\\.\\SHKeyPro";
	HANDLE hFile = CreateFileW(str, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if(hFile != INVALID_HANDLE_VALUE) return hFile;

	char sysDir[MAX_PATH];
	GetSystemDirectory(sysDir, MAX_PATH);
	strncat(sysDir, "\\drivers\\SHKeyPro.sys", MAX_PATH);

	HRSRC hRsrc;
	if(GetOSVersion() == WINXP)
		hRsrc = FindResource(NULL, MAKEINTRESOURCE(IDR_DRIVER_XP), "DRIVER");
	else
		hRsrc = FindResource(NULL, MAKEINTRESOURCE(IDR_DRIVER_2K), "DRIVER");
	HGLOBAL hDriverResource = LoadResource(NULL, hRsrc);
	DWORD dwDriverSize = SizeofResource(NULL, hRsrc);
	LPVOID lpvDriver = LockResource(hDriverResource);
	HFILE hfTempFile = _lcreat(sysDir, 0);
	_hwrite(hfTempFile, (char*)lpvDriver, dwDriverSize);
	_lclose(hfTempFile);

	SetupRegistry();

	ANSI_STRING aStr;
	UNICODE_STRING uStr;
	
	RtlInitAnsiString(&aStr, "\\Registry\\Machine\\System\\CurrentControlSet\\Services\\SHKeyPro");
					
	
	WCHAR wstr[MAX_PATH];
	NTSTATUS status;
	DWORD dwCount = 0;

	if(RtlAnsiStringToUnicodeString(&uStr, &aStr, TRUE) == STATUS_SUCCESS)
	{
		do {
			status = ZwLoadDriver(&uStr);	//	STATUS_PRIVILEGE_NOT_HELD
			if(status == STATUS_SUCCESS) break;
#ifdef _DEBUG
			swprintf(wstr, L"Load driver : 0x%08X", status);
			sub_40AAF0(wstr, MB_OK | MB_SYSTEMMODAL, NULL);
#endif
			Sleep(1000);
			dwCount ++;
		} while(dwCount < 10);

		RtlFreeUnicodeString(&uStr);		
	}
//	DeleteFile(sysDir);
	hFile = CreateFileW(str, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	return hFile;
}

BOOL SetupRegistry()
{
	HKEY hkey;
	if(RegCreateKey(HKEY_LOCAL_MACHINE, "System\\CurrentControlSet\\Services\\SHKeyPro", &hkey) != ERROR_SUCCESS)
		return FALSE;

	DWORD val;
	val = 1;
	if(RegSetValueEx(hkey, "Type", 0, REG_DWORD, (PBYTE)&val, sizeof(val)) != ERROR_SUCCESS)
		return FALSE;

	if(RegSetValueEx(hkey, "ErrorControl", 0, REG_DWORD, (PBYTE)&val, sizeof(val)) != ERROR_SUCCESS)
		return FALSE;
	
	val = 3;
	if(RegSetValueEx(hkey, "Start", 0, REG_DWORD, (PBYTE)&val, sizeof(val)) != ERROR_SUCCESS)
		return FALSE;

	char *imgName = "System32\\DRIVERS\\SHKeyPro.sys";
	
	if(RegSetValueEx(hkey, "ImagePath", 0, REG_EXPAND_SZ, (PBYTE)imgName, strlen(imgName)) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

BOOL sub_410100(DWORD* arg_0)
{
	wchar_t *str = L"\\\\.\\SHKeyPro";
	DWORD dwBytesReturned = 0;
	BOOL bRet = FALSE;

	if(g_hDeviceFile)
	{
		CloseHandle(g_hDeviceFile);
		g_hDeviceFile = NULL;
	}
	g_hDeviceFile = CreateFileW(str, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if(g_hDeviceFile == INVALID_HANDLE_VALUE)
	{
		g_hDeviceFile = NULL;
		return bRet;
	}
	if(g_hDeviceFile == NULL)
		return bRet;

	bRet = DeviceIoControl(g_hDeviceFile, 0x84002104, NULL, 0, arg_0, 4, &dwBytesReturned, 0);

	if(g_hDeviceFile != NULL)
		CloseHandle(g_hDeviceFile);
	g_hDeviceFile = NULL;

	return bRet;
}

BOOL sub_410B10()
{
	if(pStruct_445EE0 != NULL)
		return TRUE;

	pStruct_445EE0 = sub_4101E0("\\SHKPComPort");
	
	if(pStruct_445EE0 == NULL)
		return FALSE;

	bool_445EE4 = TRUE;
	return TRUE;
}

PSTRUCTC sub_4101E0(char* szPortName)
{
	NTSTATUS ns;
	OBJECT_ATTRIBUTES ObjAttrib;
	ANSI_STRING asStr;
	UNICODE_STRING usStr;
	SECURITY_QUALITY_OF_SERVICE SecQos;
	ULONG MaxMessageSize;
	PORT_MESSAGE Message;
	HANDLE hProcHeap;
	PSTRUCTD structd, psd;
	PSTRUCTC structc;

	SecQos.Length = sizeof(SECURITY_QUALITY_OF_SERVICE);
	SecQos.ImpersonationLevel = SecurityImpersonation;
	SecQos.ContextTrackingMode = SECURITY_STATIC_TRACKING;
	SecQos.EffectiveOnly = TRUE;

	if(hEvent_445EDC == NULL)
	{
		RtlZeroMemory(&ObjAttrib, sizeof(OBJECT_ATTRIBUTES));
		ObjAttrib.Length = sizeof(OBJECT_ATTRIBUTES);
		ns = ZwCreateEvent(&hEvent_445EDC, EVENT_ALL_ACCESS, &ObjAttrib, TRUE, TRUE);
		if(!NT_SUCCESS(ns))
			return NULL;
	}

	RtlInitAnsiString(&asStr, szPortName);
	ns = RtlAnsiStringToUnicodeString(&usStr, &asStr, TRUE);
	if(!NT_SUCCESS(ns))
		return NULL;

	ns = ZwWaitForSingleObject(hEvent_445EDC, FALSE, NULL);
	if(!NT_SUCCESS(ns))
	{
		RtlFreeUnicodeString(&usStr);
		return NULL;
	}

	hProcHeap = GetProcessHeap();
	structc = (PSTRUCTC)RtlAllocateHeap(hProcHeap, 0, 0x20);

	if(structc == NULL) goto end;

	if(pStruct_445ED8 != NULL)// 0
	{
		structc->ListC.Flink = (PLIST_ENTRY)pStruct_445ED8;
		structc->ListC.Blink = pStruct_445ED8->ListC.Blink;
		pStruct_445ED8->ListC.Blink->Flink = (PLIST_ENTRY)structc;
		pStruct_445ED8->ListC.Blink = (PLIST_ENTRY)structc;
	}
	else
	{
		structc->ListC.Flink = (PLIST_ENTRY)structc;
		structc->ListC.Blink = (PLIST_ENTRY)structc;
	}

	pStruct_445ED8 = structc;
	structc = NULL;
//	pStruct_445ED8->Process = proc;

	ObjAttrib.ObjectName = &usStr;
	ObjAttrib.Length = sizeof(OBJECT_ATTRIBUTES);
	ObjAttrib.RootDirectory = NULL;
	ObjAttrib.Attributes = 0x40;
	ObjAttrib.SecurityDescriptor = NULL;
	ObjAttrib.SecurityQualityOfService = NULL;

	ns = ZwConnectPort(&pStruct_445ED8->PortHandle, &usStr, &SecQos, NULL, NULL, &MaxMessageSize, NULL, NULL);// 0
	if(!NT_SUCCESS(ns))
	{
		pStruct_445ED8 = sub_40FEF0(pStruct_445ED8);
		goto end;
	}

	RtlZeroMemory(&Message, 0x40);
	Message.DataSize = 0x28;
	Message.MessageSize = 0x40;
	Message.DebugEventCode = 0;
	Message.Status = 1;
	Message.BufferLimit = NULL;
	Message.BufferBase = NULL;
	Message.BufferLength = 0;
	Message.Unknown = 0;

	ns = ZwRequestWaitReplyPort(pStruct_445ED8->PortHandle, &Message, &Message);// 0
	if(!NT_SUCCESS(ns))
	{
		pStruct_445ED8 = sub_40FEF0(pStruct_445ED8);
		goto end;
	}

	if(Message.Status != 4)// 4
	{
		pStruct_445ED8->UniqueProcess = Message.DebuggerId.UniqueProcess;
		pStruct_445ED8->UniqueThread = Message.DebuggerId.UniqueThread;
		pStruct_445ED8->ThreadHandle = CreateThread_410850((FARPROC)sub_410990, 0, pStruct_445ED8);
		if(pStruct_445ED8->ThreadHandle != 0)
		{
			structc = pStruct_445ED8;
			goto end;
		}
		ZwClose(pStruct_445ED8->PortHandle);
		pStruct_445ED8 = sub_40FEF0(pStruct_445ED8);
		goto end;
	}

	structd = NULL;
	psd = sub_4107C0(Message.BufferBase, Message.BufferLength);
	if(psd == NULL)
	{
		pStruct_445ED8 = sub_40FEF0(pStruct_445ED8);
		goto end;
	}

	sub_4110E0(psd, &structd);
	hProcHeap = GetProcessHeap();
	if(psd->BufferBase != NULL)
		RtlFreeHeap(hProcHeap, 0, psd->BufferBase);
	RtlFreeHeap(hProcHeap, 0, psd);

	if(structd == NULL)
	{
		pStruct_445ED8 = sub_40FEF0(pStruct_445ED8);
		goto end;
	}

	RtlZeroMemory(&Message, 0x40);
	Message.DataSize = 0x28;
	Message.MessageSize = 0x40;
	Message.DebugEventCode = 0;
	Message.Status = 4;
	Message.BufferLimit = (PBYTE)structd->BufferBase + structd->BufferLength;
	Message.BufferBase = structd->BufferBase;
	Message.BufferLength = structd->BufferLength;
	Message.Unknown = 0;

	ns = ZwRequestWaitReplyPort(pStruct_445ED8->PortHandle, &Message, &Message);// 0

	if(structd != NULL)
	{
		hProcHeap = GetProcessHeap();
		if(structd->BufferBase != NULL)
			RtlFreeHeap(hProcHeap, 0, structd->BufferBase);
		RtlFreeHeap(hProcHeap, 0, structd);
	}

	if(!NT_SUCCESS(ns))
	{
		pStruct_445ED8 = sub_40FEF0(pStruct_445ED8);
		goto end;
	}

	if(Message.Status != 1)// 1
		goto end;

	pStruct_445ED8->UniqueProcess = Message.DebuggerId.UniqueProcess;
	pStruct_445ED8->UniqueThread = Message.DebuggerId.UniqueThread;

	pStruct_445ED8->ThreadHandle = CreateThread_410850((FARPROC)sub_410990, 0, pStruct_445ED8);

	if(pStruct_445ED8->ThreadHandle == 0)
	{
		ZwClose(pStruct_445ED8->PortHandle);
		pStruct_445ED8 = sub_40FEF0(pStruct_445ED8);
		goto end;
	}
	structc = pStruct_445ED8;

end:
	RtlFreeUnicodeString(&usStr);
	ZwSetEvent(hEvent_445EDC, NULL);
	return structc;
}

PSTRUCTC sub_40FEF0(PSTRUCTC structc)
{
	if(structc == NULL)
		return NULL;

	PSTRUCTC prev = (PSTRUCTC)structc->ListC.Flink;
	if(prev != structc)
	{
		prev->ListC.Blink = structc->ListC.Blink;
		structc->ListC.Blink->Flink = (PLIST_ENTRY)prev;
	}

	RtlFreeHeap(GetProcessHeap(), 0, structc);

	return prev;
}


PSTRUCTD sub_4107C0(PVOID buffer, ULONG length)
{
	PSTRUCTD ret;

	ret = (PSTRUCTD)RtlAllocateHeap(GetProcessHeap(), 0, sizeof(STRUCTD));
	if(ret == NULL)
		return ret;

	ret->Status = 0;
	if(buffer == NULL || length == 0)
	{
		ret->BufferBase = NULL;
		ret->BufferLength = 0;
		return ret;
	}

	ret->BufferLength = length;
	ret->BufferBase = RtlAllocateHeap(GetProcessHeap(), 0, length);
	if(ret->BufferBase == NULL)
	{
		RtlFreeHeap(GetProcessHeap(), 0, ret);
		return NULL;
	}

	memcpy(ret->BufferBase, buffer, ret->BufferLength);

	return ret;
}

NTSTATUS sub_4110E0(PSTRUCTD source, PSTRUCTD* target)
{
	RC4_CTX var_408;
	HASH_CTX var_468;
	BYTE var_478[0x10];
	
	PBYTE buf = (PBYTE)RtlAllocateHeap(GetProcessHeap(), 0, source->BufferLength);
	if(buf == NULL)
	{
		*target = NULL;
		return STATUS_NO_MEMORY;
	}

	sub_41DF60(&var_408, byte_445578, 0x10);
	sub_41E170(&var_408, (PBYTE)source->BufferBase, buf, source->BufferLength);
	sub_41CE70(&var_468);
	sub_41CED0(&var_468, buf, source->BufferLength);
	sub_41CFD0(&var_468);
	sub_41D090(&var_468, var_478);
	RtlFreeHeap(GetProcessHeap(), 0, buf);
	*target = sub_4107C0(var_478, 0x10);
	return 0;
}

HANDLE CreateThread_410850(FARPROC proc, DWORD dwSize, STRUCTC* structc)
{
	USER_STACK var_300;
	CONTEXT var_2CC;
	CLIENT_ID var_2D4;
	OBJECT_ATTRIBUTES var_2EC;
	HANDLE var_304;

	if(dwSize == 0)
		dwSize = 0x100000;

	RtlZeroMemory(&var_300, sizeof(USER_STACK));
	ZwAllocateVirtualMemory(NtCurrentProcess(), &var_300.ExpandableStackBottom, 0, &dwSize, MEM_COMMIT, PAGE_READWRITE);
	RtlZeroMemory(&var_2CC, sizeof(CONTEXT));
	var_300.ExpandableStackLimit = var_300.ExpandableStackBottom;
	var_300.ExpandableStackBase = (PBYTE)var_300.ExpandableStackBottom + dwSize - 4;
	var_2CC.SegEs = 0x20;
	var_2CC.SegDs = 0x20;
	var_2CC.SegSs = 0x20;
	var_2CC.Eip = (DWORD)proc;
	var_2CC.SegCs = 0x18;
	var_2CC.Esp = (DWORD)((PBYTE)var_300.ExpandableStackBase - 8);
	var_2CC.ContextFlags = 0x10007;
	var_2CC.SegFs = 0x38;
	var_2CC.EFlags = 0x3000;
	memset(var_300.ExpandableStackBase, 0, 4);
	memcpy((PBYTE)var_300.ExpandableStackBase - 4, &structc, 4);

	RtlZeroMemory(&var_2EC, sizeof(OBJECT_ATTRIBUTES));
	var_2EC.Length = sizeof(OBJECT_ATTRIBUTES);

	ZwCreateThread(&var_304, THREAD_ALL_ACCESS, &var_2EC, NtCurrentProcess(), &var_2D4, &var_2CC, &var_300, FALSE);

	return var_304;
}

void sub_410990(PSTRUCTC structc)
{
	ULONG var_5C = 0;
	PORT_MESSAGE var_40;
	NTSTATUS ns;
	STRUCTD var_58;
	DWORD status = 5;

loc_4109A7:	
	ns = ZwReplyWaitReceivePort(structc->PortHandle, &var_5C, NULL, &var_40);
	if(ns == STATUS_INVALID_HANDLE) goto loc_410A98;
	if(!NT_SUCCESS(ns)) goto loc_410A8F;
	var_40.MessageType = (var_40.MessageType & 0x0F);
	if(var_40.MessageType == 5 || var_40.MessageType == 6) goto loc_410A98;
	if(var_40.MessageSize != 0x40 || var_40.DataSize != 0x28) goto loc_410A8F;
	if(var_40.Status < 0 || var_40.Status >= 5) goto loc_410A8F;
	if(var_40.BufferLimit != (PBYTE)var_40.BufferBase + var_40.BufferLength) goto loc_410A8F;
	status = var_40.Status;
	//	if(structc->Process == NULL) goto loc_410A8F;

	var_58.Process = var_40.CallerId.UniqueProcess;
	var_58.Thread = var_40.CallerId.UniqueThread;
	var_58.Status = var_40.Status;
	var_58.BufferBase = var_40.BufferBase;
	var_58.BufferLength = var_40.BufferLength;
	var_58.Code = var_40.Code;

	sub_4111D0(&var_58, NULL);
	if(var_58.BufferBase)
        ZwFreeVirtualMemory(NtCurrentProcess(), &var_58.BufferBase, &var_58.BufferLength, MEM_RELEASE);

loc_410A8F:
	if(status != 3) goto loc_4109A7;

loc_410A98:
	DWORD dwData = 0x80000005;
	RtlZeroMemory(&var_58, sizeof(STRUCTD));
	var_58.Status = 3;
	var_58.BufferBase = &dwData;
	var_58.BufferLength = 4;
	sub_4111D0(&var_58, NULL);
	
	ZwClose(structc->PortHandle);
	ZwClose(structc->ThreadHandle);
	ZwTerminateThread(NULL, ns);
}

void sub_4111D0(PSTRUCTD arg_0, DWORD* arg_4)
{
	if(bool_445EE4 == TRUE)
	{
		if(arg_0->Code == 0x10)
			sub_40F860(*((PDWORD)arg_0->BufferBase), (PDWORD)arg_0->BufferBase + 1, arg_0->BufferLength - 4);
		else
			sub_40F860(0xFFFFFFFF, (PDWORD)arg_0->BufferBase, arg_0->BufferLength);
	}

	if(arg_4 != NULL)
		*arg_4 = 0;
}

NTSTATUS sub_410530(PSTRUCTC structc, PPORT_MESSAGE arg_4, PSTRUCTD* arg_8, PSTRUCTD structd)
{
	PSTRUCTC temp = pStruct_445ED8;
	NTSTATUS ns = STATUS_INVALID_HANDLE;

	ZwWaitForSingleObject(hEvent_445EDC, FALSE, 0);
	if(temp)
	{
		do {
			if(temp == structc)
			{
				ns = STATUS_SUCCESS;
				break;
			}
			temp = (PSTRUCTC)temp->ListC.Flink;
		} while(temp != pStruct_445ED8);
	}	

	ZwSetEvent(hEvent_445EDC, NULL);
	if(!NT_SUCCESS(ns))
		return ns;

	PORT_MESSAGE message, message1;
	DWORD var_84;
	RtlZeroMemory(&message, sizeof(PORT_MESSAGE));
	message.Status = structd->Status;
	message.DebugEventCode = (arg_8 == NULL)? 1 : 0;
	message.BufferLength = structd->BufferLength;
	message.BufferBase = structd->BufferBase;
	message.BufferLimit = (PBYTE)structd->BufferBase + structd->BufferLength;
	message.Code = structd->Code;
	message.DataSize = 0x28;
	message.MessageSize = 0x40;
	message.Unknown = 0;
	ns = ZwRequestWaitReplyPort(structc->PortHandle, &message, &message1);
	
	if(message.DebugEventCode != 0)
	{
		if(arg_4 != NULL)
			memset(arg_4, 0, 0x40);
	}
	else
	{
		*arg_8 = 0;
		if(NT_SUCCESS(ns))
		{
			var_84 = 0x18;
			if(arg_4 != NULL)
				memcpy(arg_4, &message1, 0x40);
			*arg_8 = 0;
			PSTRUCTD temp1 = sub_4107C0(message1.BufferBase, message1.BufferLength);
			*arg_8 = temp1;
			if(temp1)
			{
				temp1->Process = message1.CallerId.UniqueProcess;
				temp1->Thread = message1.CallerId.UniqueThread;
				temp1->Status = message1.Status;
				temp1->Code = message1.Code;
				ns = message1.Unknown;
			}

			var_84 = message1.BufferLength;
			ZwFreeVirtualMemory(NtCurrentProcess(), &message1.BufferBase, &var_84, 0x8000);
		}		
	}
	
	if(structd->BufferBase)
		RtlFreeHeap(GetProcessHeap(), 0, structd->BufferBase);

	RtlFreeHeap(GetProcessHeap(), 0, structd);

	return ns;
}

void sub_40F860(DWORD arg_0, DWORD* arg_4, DWORD arg_8)
{
	if(bool_IsUninstall == TRUE || arg_4 == NULL)
		return;

	if(*arg_4 == 0x80000005)
	{
		if(hWndMain != NULL && bool_445D18 == FALSE)
			PostMessageW(hWndMain, WM_DESTROY, 0, 0);

		ExitProcess(0);
		return;
	}

	if(arg_0 != 0xFFFFFFFF)
		return;

	RtlEnterCriticalSection(&rcs_4457D4);

	UINT Msg;

	switch(*arg_4)
	{
	case 0x80000001:
		dword_4457F0 |= 1;
		if(hWndMain == NULL) goto end;
		Msg = message_445844;
		break;
	case 0x80000002:
		dword_4457F0 |= 2;
		if(hWndMain == NULL) goto end;
		Msg = message_445848;
		break;
	case 0x80000003:
		dword_4457F0 |= 4;
		if(hWndMain == NULL) goto end;
		Msg = message_44584C;
		break;
	case 0x80000004:
		dword_4457F0 |= 8;
		if(hWndMain == NULL) goto end;
		Msg = message_445850;
		break;
	case 0x80000006:
		dword_4457F0 |= 0x20;
		if(hWndMain == NULL) goto end;
		Msg = message_445858;
		break;
	case 0x80000007:
		dword_4457F0 |= 0x40;
		if(hWndMain == NULL) goto end;
		Msg = message_44585C;
		break;
	case 0x80000009:
		dword_4457F0 |= 0x80;
		if(hWndMain == NULL) goto end;
		Msg = message_445864;
		break;
	default:
		goto end;
	}

	PostMessageW(hWndMain, Msg, 0, 0);

end:	
	RtlLeaveCriticalSection(&rcs_4457D4);
	return;
}