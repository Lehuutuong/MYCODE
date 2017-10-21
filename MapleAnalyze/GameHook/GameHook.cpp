// GameHook.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <stdio.h>
#include "GameHook.h"
#include "../_common/patch.h"
#include "engine.h"
#include "AnyJack.h"
#include <math.h>
#include "d3d9.h"

#include <winsvc.h>
#include <winioctl.h>
#include "GameInfo.h"


#include "HookApi_K.h"
#pragma comment(lib, "HookApi.lib")


HANDLE g_hkbfDevice = NULL;


#define IOCTL_POSTMSG_DATA		CTL_CODE(FILE_DEVICE_UNKNOWN, 5, METHOD_BUFFERED, FILE_ANY_ACCESS)
HANDLE  g_hMsDevice = NULL;

DWORD	g_GameThreadId = NULL;
HWND	g_GameHwnd = NULL;
DWORD	g_dwDIObj = 0;
DWORD	g_GameProcessID = NULL;

POINT	g_ptTelePortalStart_Bugi = {648, 1132};	// 부활하는 기억 텔레포탈위치
POINT	g_ptTelePortalDest_Bugi = {-236, 111};

DWORD	g_dwBotID;
BYTE	g_HookBytes[10];		
BYTE	g_SaveBytes[10];
DWORD	g_dwHookAddress;
DWORD	g_dwOffset;
DWORD	g_dwAttack;
POINT	g_ptBotPos;

HANDLE	g_hEhsvc = NULL;
BOOL	g_bTextureCapture = FALSE;

typedef SHORT	(*_GetAsyncKeyState)(int vKey); // user32.dll 함수 후킹
typedef HWND	(*_GetFocus)(void);
typedef SHORT  (*_GetKeyState)(int nVirtKey);
typedef BOOL  (*_GetKeyboardState)(PBYTE lpKeyState);
typedef BOOL (*_DeviceIoControl)(HANDLE hDevice,DWORD dwIoControlCode,LPVOID lpInBuffer,DWORD nInBufferSize,LPVOID lpOutBuffer,DWORD nOutBufferSize,LPDWORD lpBytesReturned,LPOVERLAPPED lpOverlapped);

typedef BOOL (*_PostMessageA)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
typedef int (*_closesocket)(SOCKET s);
typedef int (*_setsockopt)(SOCKET s, int level, int optname, const char *optval, int optlen);
typedef char* (*_inet_ntoa)(struct   in_addr in);

typedef DWORD (*_GetQueueStatus)(UINT flags);
typedef BOOL (*_GetInputState)(void);
typedef BOOL (*_GetMessage)(LPMSG lpMsg,HWND hWnd,UINT wMsgFilterMin,UINT wMsgFilterMax);
typedef HWND (*_GetForegroundWindow)();

typedef int (*_GetWindowTextA)(HWND hWnd,LPCSTR lpString,int nMaxCount);
					   
typedef BOOL (*_GetWindowRect)(HWND hWnd,LPRECT lpRect);



SHORT	MyGetAsyncKeyState(int vKey);
SHORT	MyGetKeyState(  int nVirtKey );
HWND	MyGetFocus();
BOOL	MyGetKeyboardState(PBYTE lpKeyState);
BOOL	MyDeviceIoControl(HANDLE hDevice,DWORD dwIoControlCode,LPVOID lpInBuffer,DWORD nInBufferSize,LPVOID lpOutBuffer,DWORD nOutBufferSize,LPDWORD lpBytesReturned,LPOVERLAPPED lpOverlapped);
DWORD	MyGetQueueStatus(UINT flags);
BOOL	MyGetInputState(void);
BOOL	MyGetMessage(LPMSG lpMsg,HWND hWnd,UINT wMsgFilterMin,UINT wMsgFilterMax);
BOOL	MyGetWindowRect(HWND hWnd, LPRECT lpRect);



BOOL GetMobPosition(); 

void SeBotPosition(DWORD X, DWORD Y);
void ChangeMobStatus();
DWORD GetCurrentHP();
DWORD GetEmployeeShopID(CString strShopName);
DWORD GetOtherCharacterShopID(CString strShopName, DWORD &dwTmp);
void GetMobPosFromMobProp(DWORD dwProp, POINT &pt);



void GetModuleList(DWORD dwPID);
DWORD GetProcessID(LPCTSTR pszProcessName);
BOOL GetProcessModule(DWORD dwPID, LPCTSTR pszProcessName);
BOOL DisplayModuleInProcess(DWORD dwPID);



//BOOL ResetTeleportInfo(POINT srcPt, POINT dstPt);

DWORD g_dwBotJobId;
void GetSkillInventoryInfo(DWORD* pSkillIdArray, DWORD* pLevelArray);
DWORD GetSkillLevel(DWORD dwSkillId);

void _get_mob_x(DWORD dwEsi);
void _get_mob_y(DWORD dwEsi);

PVOID	g_hVectoredHandler;
HMODULE			g_ImageBase = NULL;
BOOL			g_bMobMori = FALSE;
BOOL			g_bMobAnran = FALSE;
BOOL			g_bMobZomAndRight = FALSE;
BOOL			g_bHookSend = FALSE;
BOOL			g_bHookRecv = FALSE;
BOOL			g_bItemMove = FALSE;
BOOL			g_bMoveMob = FALSE;
BOOL			g_bAttackSpeed = FALSE;
BOOL			g_bItemToggle = FALSE;
BOOL			g_bMobSpeed = FALSE;
BOOL			g_bAllMobAttack = FALSE;
BOOL			g_bMujok = FALSE;
BOOL			g_bRegenSamePos = FALSE;
BOOL			g_bItemFilter = FALSE;
BOOL			g_bHookItemPick = FALSE;
BOOL			g_bUnRaming = FALSE;
BOOL			g_bSkillRemove = FALSE;
BOOL			g_bDemiji = FALSE;
BOOL			g_bXYTem = FALSE;
BOOL			g_bPosHook = FALSE;
BOOL			g_bCritical = FALSE;
BOOL			g_bPickNoLimit = FALSE;
BOOL			g_bPickItemWithPacket = FALSE;
BOOL			g_bSuddenMove = FALSE;
BOOL			g_bMouseClick = FALSE;
BOOL			g_bBloodStop = FALSE;
BOOL			g_bUseHackStance = FALSE;
BOOL			g_bUseHackRemoveBGD = FALSE;
BOOL			g_bRemoveMob = FALSE;
BOOL			g_bSonGongRaming = FALSE;
BOOL			g_bAllMobLeft = FALSE;
BOOL			g_bSpeedPickup = FALSE;
BOOL			g_bLogTest = FALSE;
BOOL			g_bIceAttack = FALSE;
BOOL			g_bNoLimitAttack = FALSE;
BOOL			g_bMob = FALSE;
BOOL			g_bAllMobRight = FALSE;
BOOL			g_bRemoveLeak = FALSE;
BOOL			g_bMotionRemove = FALSE;
BOOL			g_bEffectRemove = FALSE;
BOOL			g_bSiluetRemove = FALSE;
BOOL			g_bDupe = FALSE;
BOOL			g_bPacketCapture = FALSE;
BOOL			g_bMugong = FALSE;
BOOL			g_bMobStance = FALSE;
BOOL			g_bMobNoResponse = FALSE;
BOOL			g_bSkipAddr = FALSE;
BOOL			g_bSoftBreak = FALSE;
BOOL			g_bChangeMobProp = FALSE;
BOOL			g_bTelePortalHook = FALSE;
BOOL			g_bSpeedUp = FALSE;






BOOL g_bUserKeySend = FALSE;
DWORD g_KeyHwnd;
DWORD g_KeyMsg;
DWORD g_KeyParam1;
DWORD g_KeyParam2;




BOOL g_bInjectEngine = FALSE;
#define USER_KBD_MSG	WM_USER+0x100
UINT PostUserMsgThread(LPVOID pVoid)
{
	g_bInjectEngine = TRUE;
	while (g_bInjectEngine)
	{
		PostMessage(g_GameHwnd, USER_KBD_MSG, 0, 0);
		//PrintLogA("#");
		Sleep(10);
	}
	return 0;
}


DWORD g_dwHardwareBreakAddress[4] = {0};

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
			//context.Dr7 |= 0x3;
			context.Dr7 = 0xFF;
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

	SetThreadContext(hThread, &context);
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

typedef struct _HOOK_API_CODE{
	BYTE code1;
	DWORD address;
	BYTE code2;
}HOOK_API_CODE, *PHOOK_API_CODE; //6

typedef struct _HOOK_API_INFO {
	FARPROC	OrgProc;
	FARPROC	HookProc; // 4
	HOOK_API_CODE hookBytes;//8
	BYTE	SaveBytes[6];//0x0e
} HOOK_API_INFO, *PHOOK_API_INFO; // 0x14

typedef struct _HOOK_FUNC_CODE
{
	BYTE code;
	DWORD offset;
}HOOK_FUNC_CODE, *PHOOK_FUNC_CODE;

typedef struct _HOOK_FUNC_INFO
{
	DWORD OrgProc;
	DWORD HookProc; // 4
	HOOK_FUNC_CODE  hookBytes; //8
	BYTE SaveBytes[8]; //0x0d
}HOOK_FUNC_INFO, *PHOOK_FUNC_INFO;

HOOK_API_INFO haiHackPatchFunc1;
HOOK_FUNC_INFO hfiHackPatchFunc2;
HOOK_FUNC_INFO hfiHackPatchFunc3;
HOOK_FUNC_INFO hfiHackPatchFunc4;
HOOK_FUNC_INFO	hfiHackPatchFunc5;
HOOK_API_INFO	haiHackMolMolFunc;


HOOK_API_INFO 	haiD3d9CreateTexture;
HOOK_API_INFO 	haiD3d9DrawPrimitiveUP; 


HOOK_API_INFO				haiPostMessage;
HOOK_API_INFO				haiclosesocket;
HOOK_API_INFO				haisetsockopt;
HOOK_API_INFO				haiinet_ntoa;

HOOK_API_INFO				haiDeviceIoControl;
HOOK_API_INFO				haiGetQueueStatus;
HOOK_API_INFO				haiGetInputState;
HOOK_API_INFO				haiGetMessage;
HOOK_API_INFO				haiGetForegroundWindow;
HOOK_API_INFO				haiGetWindowTextA;



HOOK_API_INFO				haiGetAsyncKeyState;
HOOK_API_INFO 				haiGetFocus;
HOOK_API_INFO 				haiGetKeyState;
HOOK_API_INFO				haiGetKeyboardState;
HOOK_API_INFO				haiDirectInput8Create;

HOOK_API_INFO				haiGetWindowRect;


DWORD GetProcedureAddress(HMODULE hModule, LPSTR lpFuncName);

typedef enum D3DTRANSFORMSTATETYPE {
	D3DTS_VIEW          = 2,
	D3DTS_PROJECTION    = 3,
	D3DTS_TEXTURE0      = 16,
	D3DTS_TEXTURE1      = 17,
	D3DTS_TEXTURE2      = 18,
	D3DTS_TEXTURE3      = 19,
	D3DTS_TEXTURE4      = 20,
	D3DTS_TEXTURE5      = 21,
	D3DTS_TEXTURE6      = 22,
	D3DTS_TEXTURE7      = 23,
	D3DTS_FORCE_DWORD   = 0x7fffffff 
} D3DTRANSFORMSTATETYPE, *LPD3DTRANSFORMSTATETYPE;

typedef struct _D3DMATRIX {
	union 
	{
		struct 
		{
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;
		};
		float m[4][4];
	};
} D3DMATRIX;

typedef DWORD (*_D3d9_CreateTexture)(LPDWORD arg_0, DWORD arg_4, DWORD arg_8, DWORD arg_C, DWORD arg_10, DWORD arg_14, DWORD arg_18, DWORD arg_1C, DWORD arg_20);
typedef DWORD (*_D3d9_DrawPrimitiveUP)(LPDWORD arg_0, DWORD arg_4, DWORD arg_8, DWORD arg_C, DWORD arg_10);
typedef HRESULT (*_D3d9_GetTexture)(LPDWORD arg_0, DWORD arg_4, LPDWORD* arg_C);

typedef DWORD (*_D3d9_Release_8)(LPDWORD arg_0);
typedef DWORD (*_D3D9_LockRect)(LPDWORD pIDirect3DSurface9, D3DLOCKED_RECT *pLockedRect, RECT *pRect, DWORD Flags); // CMipSurface::LockRect(_D3DLOCKED_RECT *,tagRECT const *,ulong)
typedef DWORD (*_D3D9_UnlockRect)(LPDWORD pIDirect3DSurface9);
typedef DWORD (*_D3D9_GetLevelDesc)(LPDWORD pMipmap, DWORD arg_4, D3DSURFACE_DESC* pD3DSURFACE_DESC); // LONG CMipMap::GetLevelDesc(uint,_D3DSURFACE_DESC *)
typedef DWORD (*_D3D9_GetSurfaceLevel)(LPDWORD pMipmap, DWORD arg_4, LPDWORD* ppIDirect3DSurface9); // LONG CMipMap::GetSurfaceLevel(uint,IDirect3DSurface9 * *)


HOOK_API_INFO 	haiMy867130;
typedef DWORD (*_My867130Func)(DWORD arg_0, DWORD arg_4, DWORD arg_8, DWORD arg_C, DWORD arg_10, DWORD arg_14);


DWORD g_jmpMobSpeedNext;
DWORD g_jmpMobMoriNext;
DWORD g_jmpMobAnran;
DWORD g_sendShopNext;
DWORD g_dwPickedItemIndex = 0;
BOOL  g_bPick = FALSE;

DWORD RolXor(DWORD arg_0, DWORD arg_4, DWORD arg_8)
{
	DWORD eax;
	eax = _rotl(arg_4, 5);
	eax = eax ^ arg_0;
	return eax;
}

DWORD g_dwCaller, g_dwTemp;
PBYTE g_pSendBuf = NULL;
DWORD g_dwEax, g_dwEbp, g_dwEcx, g_dwEsp, g_dwEsi, g_dwEbx, g_dwEdi;
DWORD g_JmpAddr;
DWORD g_nSendLen, g_dwSendX, g_dwSendY;
DWORD g_dwNoSendPacketJmp = 0x4F76AD;
DWORD g_dwTmp;
WORD g_wSegNum = 0, w_i;
DWORD g_dwMobId;
DWORD g_dwBotId = 0;
int g_nMobX, g_nMobY;
WORD g_wNewX, g_wNewY, g_wOldX, g_wOldY;

DWORD g_dwDecryptCall;
DWORD dwPktAddr;
BYTE *g_pRecvBuf; 

void __declspec(naked) HOOKRECV()
{
	__asm
	{
		pushad;
		mov g_dwEcx, ecx;
	}

	g_dwDecryptCall = HOOK_DECRYPT_CALL;
//	PrintLogA("recv Buffer %x", g_dwEcx);
	__asm
	{
		popad;
		call g_dwDecryptCall;
		pushad;
	}

	g_JmpAddr = HOOK_RECV_FUNC+5;
	g_dwTemp = *(WORD*)(*(PDWORD)(g_dwEcx+8)+4);
	g_pRecvBuf = (BYTE*)(*(PDWORD)(g_dwEcx+8)+4);

	//if (!(g_dwTemp == 0x18e||g_dwTemp==0x191||g_dwTemp==0x192||g_dwTemp==0x193||g_dwTemp==0x195||g_dwTemp==0x196||g_dwTemp==0x8f
	//	||g_dwTemp==0xc||g_dwTemp==0x119 ||g_dwTemp==0x1b||g_dwTemp==0x198||g_dwTemp==0x199||g_dwTemp==0x197||g_dwTemp==0x1c9
	//	))
	//{
 	PrintLogA("--Recv Opcode = %x Bytes = %x, %x", *(WORD*)(*(PDWORD)(g_dwEcx+8)+4), *(PDWORD)(g_dwEcx+0x14), *(BYTE*)(*(PDWORD)(g_dwEcx+8)+6));
 		//if (g_dwTemp == 0x19B) 
		//{
			PrintBufToFile((BYTE*)(*(PDWORD)(g_dwEcx+8)+4), *(PDWORD)(g_dwEcx+0x0C) - *(PDWORD)(g_dwEcx+0x18), "c:\\MPSRecv.txt");
		//}
	//}

	__asm
	{
		popad;
		jmp g_JmpAddr;
	}
}

// 무적
DWORD dw_NONENEMMYJMP;
DWORD dw_ORGJMP;
DWORD dw_NONENEMMYTIME = 0;
DWORD dw_NONENEMMYNEXT;
DWORD g_dwMuGokIntensy = 300;

void __declspec(naked) NONENEMMYPATCHFNC()
{
  	__asm
  	{
  		mov g_dwEsp, esp;
  		pushad;
  	}

	_asm
	{
		popad;
		mov ebx, dw_NONENEMMYJMP;
		inc dw_NONENEMMYTIME;
		push ebx;
		mov ebx, g_dwMuGokIntensy;
		cmp dw_NONENEMMYTIME,ebx;
		pop ebx;
		jl RETURN;
		mov dw_NONENEMMYTIME,0;
		jmp NXT;
RETURN:
		jmp dw_NONENEMMYJMP;
NXT:
		jmp dw_NONENEMMYNEXT;
	}
}

// DWORD sub_c61f00 = HOOK_SONKONGRAMING_FUNC;
// DWORD dwJmpSonkongRaming = HOOK_SONKONGRAMING_ADDR_1+9;//0xC8E207;
// DWORD dwBotObj = MEMADDR_BOTPOSITION;
// void __declspec(naked) HOOKSONKONGRAMING()
// {
// 	__asm
// 	{
// 		call sub_c61f00;
// 		//mov     edi, eax
// 		pushad;
// 		mov edx, dwBotObj;
// 		mov edx,[edx]; //A1 ? ? ? ? 85 C0 74 12 8D 
// 		mov edx,[edx+058e4h];//4ee4 //89 ? ? ? 00 00 8d ?? ?? ?? ?? 8d ?? ?? ?? 00 00 6a 03 c6 ?? ?? ?? ?? 89 ?? e8 
// 		mov edx,[edx+0ch];
// 		mov [esi+02c0h],edx;
// 		popad;
// 		jmp dwJmpSonkongRaming;
// 	}
// }
// 
// // 고속0601
// DWORD dw_MOBMOLINEXT_LEFT1 = HACK_ALLMOBMOLI_LEFT + 5;
// DWORD dw_MOBMOLINEXT_LEFT2 = HACK_ALLMOBMOLI_LEFT + 0x18;
// void __declspec(naked) HOOKLEFTMOLI()
// {
// 	__asm
// 	{
// 		mov eax,[edx+34h];
// 		call eax;
// 		mov g_dwEax, eax;
// 		pushad;
// 	}
// 
// 	if (g_dwEax)
// 	{
// 		__asm
// 		{
// 			popad;
// 			mov     edx, [eax+0Ch];
// 			lea     ecx, [eax+0Ch];
// 			mov     eax, [edx+38h];
// 			call    eax
// 				mov		g_dwEax, eax;
// 			pushad;
// 		}
// 		
// 		{
// 			PrintLogA("<<<");
// 			__asm
// 			{
// 				popad;
// 				mov [esi+013ch],0ffffffffh;
// 				mov [esi+2F8h],5; 
// 				jmp dw_MOBMOLINEXT_LEFT2;
// 			}
// 		}
// 		//__asm
// 		//{
// 		//	popad;			
// 		//	jmp dw_MOBMOLINEXT_LEFT2;
// 		//}
// 	}
// 	else
// 	{
// 		__asm
// 		{
// 			popad;			
// 			jmp dw_MOBMOLINEXT_LEFT1
// 		}
// 	}
// }
// 
// DWORD g_dwMotionRemoveJmpAddr1 = 0xC4D38C;
// DWORD g_dwMotionRemoveJmpAddr2 = 0xC4CF6A;
// void __declspec(naked)HOOKMOTIONREMOVE()
// {
// 	__asm
// 	{
// 		mov g_dwEax, eax;
// 		mov g_dwEbx, ebx;
// 		pushad;
// 	}
// 
// 	PrintLogA("eax = %x, ebx = %x", g_dwEax, g_dwEbx);
// 
// 	if (g_dwEax == g_dwEbx)
// 	{
// 		__asm
// 		{
// 			popad;
// 			jmp g_dwMotionRemoveJmpAddr1;
// 		}
// 	}
// 	else
// 	{
// 		__asm
// 		{
// 			popad;
// 			jmp g_dwMotionRemoveJmpAddr2;
// 		}
// 	}	
// }

DWORD	sub_4ADAB0 = 0x4ADAB0;
void _get_mob_x(DWORD dwEsi)
{
	WORD	wX;
	__asm{
		pushad;
		mov esi, dwEsi;
		mov eax, [esi + 0x234];
		push eax;
		lea edi, [esi + 0x230];
		push edi;
		call sub_4ADAB0;
		mov wX, ax;
		add esp, 8;
		popad;
	}

	g_nMobX = (int)((short)wX);	
}

void _get_mob_y(DWORD dwEsi)
{
	WORD wY;
	__asm{
		pushad;
		mov esi, dwEsi;
		mov eax, [esi + 0x23c];
		push eax;
		lea edi, [esi + 0x238];
		push edi;
		call sub_4ADAB0;
		mov wY, ax;
		add esp, 8;
		popad;
	}

	g_nMobY = (int)((short)wY);
}

DWORD g_dwObjId = 0;
void GetPacketObjID(DWORD dwProp)
{
	// 	PrintLogA("GetMobPosFromMobProp0 = %x", dwProp);
	DWORD dwAddr = MEMADDR_BOTPOSITION;
	DWORD ebx;

	DWORD dwMemAddr, ebp, eax, edi;
	DWORD dwBaseAddr;
	DWORD dwMobEdi, dwMobPropObj;

	POINT errpt = {-1000, -1000};

	dwMemAddr = MEMADDR_MOBPOSITION;
	if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) 
	{			
		return;
	}
	// 	PrintLogA("GetMobPosFromMobProp1");
	dwMemAddr = *(LPDWORD)dwMemAddr;

	dwMemAddr += MEMADDR_MOBPOSITION_OFFSET3;

	if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) 
	{			
		return ;
	}
	ebp = *(LPDWORD)dwMemAddr;

	if (ebp == 0 || ebp == 0x10)
	{
		return ;
	}

	dwMemAddr += 0x10;
	dwBaseAddr = dwMemAddr;
	// 	PrintLogA("GetMobPosFromMobProp2");

	tagPOINT point;
	do 
	{
		eax = dwBaseAddr;
		if (IsBadReadPtr((LPVOID)eax, 4))
		{				
			return ;
		}
		dwMemAddr = ebp + 4;
		ebp -= 12;

		if (IsBadReadPtr((LPVOID)ebp, 4))
		{
			return ;
		}

		ebp = *(LPDWORD)ebp;
		ebp += 0x10;

		if (IsBadReadPtr((LPVOID)dwMemAddr, 4))
		{				
			return ;
		}

		edi = *(LPDWORD)dwMemAddr;

		dwMobEdi = edi;


		DWORD dwMobPropObj = *(DWORD*)(dwMobEdi+HACK_MOB_STATUS_OFFSET1)-HACK_MOB_STATUS_OFFSET2;		
		// PrintLogA("GetMobPosFromMobProp3 = %x", dwMobPropObj);
		if (dwMobPropObj == dwProp)
		{
			//PrintLogA("MOBID = %x", *(PDWORD)(dwMobEdi + 0x1C4));
			//g_dwObjId = ;
			return ;
		}
	} while (ebp != 0x10);

	return ;
}

// 
// DWORD	Platform = 0, RunFlag = 0, g_SSDupeJmpAddr = MEMADDR_HACK_SS_MAVAK+6;
// int		g_nDupeX, g_nDupeY;
// DWORD	g_dwDupeEsi;
// WORD	g_wDupeX, g_wDupeY;
// POINT	g_ptDupeMob;
// 
// void __declspec(naked) HOOKSSDUPE()
// {
// 	__asm
// 	{
// 		mov g_dwDupeEsi, esi;
// 		pushad;		
// 	}
// 
// 	GetMobPosFromMobProp(g_dwDupeEsi, g_ptDupeMob);
// 	//PrintLogA("듀프몹, %d, %d", g_ptDupeMob.x, g_ptDupeMob.y);
// 
// 	//GetBotPosition(g_ptBotPos);
// 
// 	//PrintLogA("dupe :  %d, %d", g_nDupeX, g_nDupeY);
// 
// 	if (g_ptDupeMob.x >= 750 && g_ptDupeMob.x <= 1200 && g_ptDupeMob.y >= 1000 && g_ptDupeMob.y <= 1200)
// 	/*if (g_ptDupeMob.x == -1000
// 		|| g_ptDupeMob.y == -1000
// 		|| (g_ptBotPos.y > 950 && g_ptDupeMob.y < 950) 
// 		|| (g_ptBotPos.y < 950 && g_ptDupeMob.y > 950)
// 		)*/
// 	{
// 		PrintLogA("듀프적용제외: 위치 %d, %d", g_ptDupeMob.x, g_ptDupeMob.y);
// 		__asm
// 		{
// 			popad;
// 			
// 			mov [esi+0F4h],ebx;
// 			jmp g_SSDupeJmpAddr;
// 		}
// 	}
// 
// 	__asm
// 	{
// 		popad;
// 		pushfd;
// 		push eax;
// 		push ecx;
// 		mov eax, MEMADDR_BOTPOSITION;
// 		mov eax,[eax];
// 		test eax,eax;
// 		je NullPlat;
// 
// 		mov eax,[eax+05a38h];
// 		je NullPlat;
// 
// 		lea ecx,[eax-0ch];
// 		test ecx,ecx;
// 		je NullPlat;
// 
// 		mov eax,[ecx+0F4h];
// 		test eax,eax;
// 		je NullPlat;
// 
// 		cmp [RunFlag],01;
// 		je DoVac;
// 
// 		mov [Platform],eax;
// 		inc [RunFlag];
// 		jmp DoVac
// 
// DoVac:
// 		mov [esi+02f8h],01;
// 		cmp esi,ecx;
// 		je Normal;
// 
// 		mov ebx,[Platform];
// 		mov [esi+0F4h],ebx;
// 		jmp Normal;
// 
// NullPlat:
// 		mov [Platform],00;
// 		mov [RunFlag],00;
// 		jmp Normal;
// 
// Normal:
// 		pop ecx;
// 		pop eax;
// 		popfd;
// 		mov [esi+0F4h],ebx;
// 		jmp g_SSDupeJmpAddr;
// 	}
// }
// 
// #define MEMADDR_HACK_MUGONG
// DWORD g_dwMuGongJmpAddr = MEMADDR_HACK_MUGONG + 5;
// DWORD sub_EC9B00 = 0xECB9B0;
// void __declspec(naked)HOOKMUGONG()
// {
// 	__asm
// 	{
// 		call sub_EC9B00;
// 		jmp CSX;
// CSX:
// 		push	eax;
// 		push	ebx;
// 		mov		ebx, MEMADDR_BOTPOSITION;
// 		mov		eax,[ebx] //39 05 ? ? ? ? 0f 95 ? c3 cc cc cc cc
// 		cmp		dword ptr [eax+06c08h],30; //89 ?? ?? ?? 00 00 ?? 64 00 00 00 89 ?? ?? ?? 00 00 89 ?? ?? ?? 00 00 89 ?? ?? ?? 00 00 89
// 		mov		[eax+06c18h],0;
// 		jnl		CSXReturn;
// 		pop		ebx;
// 		pop		eax;
// 		jmp		g_dwMuGongJmpAddr;
// CSXReturn:
// 		pop		ebx;
// 		pop		eax;
// 		add		[edi-04],16;
// 		jmp		g_dwMuGongJmpAddr;
// 	}
// }

// DWORD g_dwHookTelePortalJmpAddr = HOOK_TELEPORTAL_ADDR + 5;
// DWORD g_dwTelePortalMovePos;
// DWORD g_dwTeleTmp;
// void __declspec(naked) HOOKTELEPORTAL()
// {
// 	__asm
// 	{
// 		push eax;
// 		mov ecx, esi;
// 		call edx;
// 		mov g_dwTeleTmp, eax;
// 		pushad;		
// 	}
// 
// 	PrintLogA("텔레포탈이동시작위치: %d, %d", ((POINT*)g_dwTeleTmp)->x, ((POINT*)g_dwTeleTmp)->y);
// 	g_dwTelePortalMovePos = g_dwTeleTmp;
// 
// 	/*if (g_dwTemp == 0xa7
// 		&& g_bAutoState == STATE_HUNT_HUNTPLACE
// 		&& (g_nCurrentMapId == MOB_CONTROL_MAPID_3 || g_nCurrentMapId == MOB_CONTROL_MAPID_11)
// 		&& g_bSuddenMove)
// 	{
// 		//PrintLogA("--Send Opcode = %x Bytes %x Caller = %x",*(WORD*)*(PDWORD)(g_dwEax+4), *(PDWORD)(g_dwEax+8), *(PDWORD)g_dwCaller);		
// 		//PrintBufToFile((BYTE*)*(PDWORD)(g_dwEax+4), *(PDWORD)(g_dwEax+8), "c:\\MPSSend.txt");
// 		GetBotPosition(&g_ptBot_send);
// 		if (abs(g_ptBot_send.x - g_ptSuddenMoveStart.x) < 20 && abs(g_ptBot_send.y - g_ptSuddenMoveStart.y) < 20)
// 		{
// 			//DebugMessageA(1, "텔레포탈로 이동");
// 			if (IsBadWritePtr((LPVOID)(g_pPacktBuf+9), 4) == FALSE)
// 			{
// 				*(WORD*)(g_pPacktBuf+9)		= (WORD)g_ptTelePortalDest.x;
// 				*(WORD*)(g_pPacktBuf+11)	= (WORD)g_ptTelePortalDest.y;
// 				ResetTeleportInfo(g_ptSuddenMoveDest, g_ptTelePortalStart);
// 			}
// 			else
// 			{
// 				//DebugMessageA(1, "############### err-1 ###############");
// 			}			
// 		}
// 	}*/
// 
// 	__asm
// 	{
// 		popad;
// 		mov eax, g_dwTelePortalMovePos;
// 		jmp g_dwHookTelePortalJmpAddr;
// 	}
// }

// DWORD g_dwSKillTimeJmpAddr = HOOK_SKILLTIME_ADDR + 6;
// DWORD g_curSkilId = 0, g_curSkillTime;;
// DWORD g_dwI;
// void __declspec(naked) HOOKSKILLTIME()
// {
// 	__asm
// 	{
// 		mov g_dwEsi, esi;
// 		pushad;
// 	}
// 	if (g_curSkilId != *(DWORD*)(g_dwEsi+0x3c4))
// 	{
// 		g_curSkilId = *(DWORD*)(g_dwEsi+0x3c4);
// 		g_curSkillTime = GetTickCount();
// 		PrintLogA("SKILL = %x, esi=%x, esi_1=%x", *(DWORD*)(g_dwEsi+0x3c4), g_dwEsi, *(DWORD*)MEMADDR_BOTPOSITION + 0x90);		
// 	}
// 	else if (g_curSkilId == 0x11d)
// 	{
// 		if (GetTickCount() - g_curSkillTime > 1000)
// 		{
// 			*(DWORD*)(g_dwEsi+0x3c4) = 0xFFFFFFFF;
// 			*(DWORD*)(g_dwEsi+0x3B0) = 0xFFFFFFFF;
// 		}
// 	}
// 	
// 	__asm
// 	{
// 		popad;
// 		mov eax, [esi+0348h];
// 		jmp g_dwSKillTimeJmpAddr;
// 	}
// }
// 
DWORD g_dwSendPacketEbp;
DWORD g_dwSendPacketJmp = HOOK_SENDPACKET_ADDR + 5;
DWORD g_dwEax_send,g_dwCaller_send, g_pSendPackLen;
PBYTE g_pSendPacktBuf;
WORD g_wSendOpcode;
void __declspec(naked)HOOKSENDPACKET()
{
	__asm
	{
		push ebp;
		mov ebp, esp;
		push 0FFFFFFFFh;
		mov g_dwSendPacketEbp, ebp;
		pushad;
	}

	g_dwEax_send	= g_dwSendPacketEbp + 8;
	g_dwCaller_send = g_dwSendPacketEbp + 4;
	g_dwEax_send	= *(PDWORD)g_dwEax_send;	 

	g_pSendPacktBuf		= (PBYTE)(*(PDWORD)(g_dwEax_send+4));
	g_pSendPackLen	= *(PDWORD)(g_dwEax_send+8);
	g_wSendOpcode		= *(WORD*)*(PDWORD)(g_dwEax_send+4);

	if (g_wSendOpcode == 0xc6)
	{
		PrintLogA("[send] op=%x, len=%d, caller=%x", g_wSendOpcode, g_pSendPackLen,*(DWORD*)g_dwCaller_send);	
		PrintBufToFile(g_pSendPacktBuf, g_pSendPackLen, "c:\\MPSSend.txt");	
	}

	__asm
	{
		popad;
		jmp g_dwSendPacketJmp;
	}
}

void __cdecl HookHackAddr()
{
	DWORD ret;
	DWORD OldProtect, NewProtect;
	BYTE* lpMemAddr;
	BYTE var_8[2] = {0};
	DWORD FN_PATCHFUNC;
	int ebx = 0;

 	if (g_bHookSend)
 	{
 		*(BYTE*)HOOK_SENDPACKET_ADDR = 0xE9;
 		*(DWORD*)(HOOK_SENDPACKET_ADDR+1) = (DWORD)HOOKSENDPACKET - HOOK_SENDPACKET_ADDR - 5;
 	}	

	if (g_bSoftBreak)
	{
		for (int i = 0; i < 40; i ++)
		{
			if (g_arrBreakInfo[i].addr1 != 0) *(BYTE*)g_arrBreakInfo[i].addr1 = 0xCC;
			if (g_arrBreakInfo[i].addr2 != 0) *(BYTE*)g_arrBreakInfo[i].addr2 = 0xCC;
		}
	}

	//if (g_bTelePortalHook)
	//{
	//	VirtualProtect((LPVOID)HOOK_TELEPORTAL_ADDR, 5, 0x40, &OldProtect);
	//	*(BYTE*)HOOK_TELEPORTAL_ADDR = 0xE9;
	//	*(DWORD*)(HOOK_TELEPORTAL_ADDR+1) = (DWORD)HOOKTELEPORTAL - HOOK_TELEPORTAL_ADDR - 5;
	//	VirtualProtect((LPVOID)HOOK_TELEPORTAL_ADDR, 5, OldProtect, &OldProtect);
	//}

	if (g_bHookRecv)
	{
		VirtualProtect((LPVOID)HOOK_RECV_FUNC, 5, 0x40, &OldProtect);
		*(BYTE*)HOOK_RECV_FUNC = 0xE9;
		*(DWORD*)(HOOK_RECV_FUNC+1) = (DWORD)HOOKRECV - HOOK_RECV_FUNC - 5;
		VirtualProtect((LPVOID)HOOK_RECV_FUNC, 5, OldProtect, &OldProtect);
	}

	

	if (g_bMujok)
	{
		VirtualProtect((LPVOID)MEMADDR_HACKTIMENONENEMMY, 6, 0x40, &OldProtect);
		*(BYTE*)(MEMADDR_HACKTIMENONENEMMY) = 0x90;
		*(BYTE*)(MEMADDR_HACKTIMENONENEMMY+1) = 0xE9;
		*(DWORD*)(MEMADDR_HACKTIMENONENEMMY+2) = (DWORD)NONENEMMYPATCHFNC - MEMADDR_HACKTIMENONENEMMY - 5;
		VirtualProtect((LPVOID)MEMADDR_HACKTIMENONENEMMY, 6, OldProtect, &NewProtect);
	}

// 	if (g_bSonGongRaming)
// 	{
// 		VirtualProtect((LPVOID)HACK_SONJE_ATTACK_ADDR, 2, 0x40, &OldProtect);
// 		*(WORD*)HACK_SONJE_ATTACK_ADDR = 0x840F;
// 		VirtualProtect((LPVOID)HACK_SONJE_ATTACK_ADDR, 2, OldProtect, &OldProtect);
// 
// 		VirtualProtect((LPVOID)HACK_SONJE_RAMING_ADDR, 2, 0x40, &OldProtect);
// 		*(WORD*)HACK_SONJE_RAMING_ADDR = 0x86DC;
// 		VirtualProtect((LPVOID)HACK_SONJE_RAMING_ADDR, 2, OldProtect, &OldProtect);
// 	}

// 	if (g_bAllMobLeft)
// 	{
// 		VirtualProtect((LPVOID)HACK_ALLMOBMOLI_LEFT, 5, 0x40, &OldProtect);
// 		*(BYTE*)HACK_ALLMOBMOLI_LEFT = 0xE9;
// 		*(DWORD*)(HACK_ALLMOBMOLI_LEFT+1) = (DWORD)HOOKLEFTMOLI - HACK_ALLMOBMOLI_LEFT - 5;
// 		VirtualProtect((LPVOID)HACK_ALLMOBMOLI_LEFT, 5, OldProtect, &OldProtect);	
// 	}

	if (g_bEffectRemove)
	{
		VirtualProtect((LPVOID)(HACK_EFFECT_REMOVE_ADDR+7), 2, 0x40, &OldProtect);
		*(WORD*)(HACK_EFFECT_REMOVE_ADDR+7) = 0x9090;
		VirtualProtect((LPVOID)(HACK_EFFECT_REMOVE_ADDR+7), 2, OldProtect, &OldProtect);

		VirtualProtect((LPVOID)(HACK_EFFECT_REMOVE_ADDR+0xD), 2, 0x40, &OldProtect);
		*(WORD*)(HACK_EFFECT_REMOVE_ADDR+0xD) = 0xE990;
		VirtualProtect((LPVOID)(HACK_EFFECT_REMOVE_ADDR+0xD), 2, OldProtect, &OldProtect);
	}

	if (g_bSiluetRemove)
	{
		VirtualProtect((LPVOID)HACK_SILUET_REMOVE_ADDR, 2, 0x40, &OldProtect);
		*(WORD*)HACK_SILUET_REMOVE_ADDR = 0xE990;
		VirtualProtect((LPVOID)HACK_SILUET_REMOVE_ADDR, 2, OldProtect, &OldProtect);
	}

// 	if (g_bDupe)
// 	{
// 		VirtualProtect((LPVOID)MEMADDR_HACK_SS_MAVAK, 6, 0x40, &OldProtect);
// 		*(BYTE*)MEMADDR_HACK_SS_MAVAK		= 0xE9;
// 		*(DWORD*)(MEMADDR_HACK_SS_MAVAK+1)	= (DWORD) HOOKSSDUPE - MEMADDR_HACK_SS_MAVAK - 5;
// 		*(BYTE*)(MEMADDR_HACK_SS_MAVAK+5)	=  0x90;
// 		VirtualProtect((LPVOID)MEMADDR_HACK_SS_MAVAK, 6, OldProtect, &OldProtect);
// 	}
// 	if (g_bMobStance)
// 	{
// 		VirtualProtect((LPVOID)HOOK_MOBSTANCE, 2, 0x40, &OldProtect);
// 		*(WORD*)HOOK_MOBSTANCE = 0x9090;
// 		VirtualProtect((LPVOID)HOOK_MOBSTANCE, 2, OldProtect, &OldProtect);
// 	}
// 
// 	if (g_bMobNoResponse)
// 	{
// 		VirtualProtect((LPVOID)HACK_MOB_NORESPONSE, 1, 0x40, &OldProtect);
// 		*(BYTE*)HACK_MOB_NORESPONSE = 0x78;
// 		VirtualProtect((LPVOID)HACK_MOB_NORESPONSE, 1, OldProtect, &OldProtect);
// 	}
}

void __cdecl UnHookHackAddr()
{	
	DWORD ret;
	DWORD OldProtect, NewProtect;
	BYTE* lpMemAddr;
	BYTE var_8[2] = {0};
	DWORD FN_PATCHFUNC;
	int ebx = 0;

 	if (g_bHookSend)
 	{
 		*(BYTE*)HOOK_SENDPACKET_ADDR		= 0x55;
 		*(DWORD*)(HOOK_SENDPACKET_ADDR+1)	= 0xFF6AEC8B;
 	}

	if (g_bSoftBreak)
	{
		for (int i = 0; i < 40; i ++)
		{
			if (g_arrBreakInfo[i].addr1 != 0) *(BYTE*)g_arrBreakInfo[i].addr1 = g_arrBreakInfo[i].val1;
			if (g_arrBreakInfo[i].addr2 != 0) *(BYTE*)g_arrBreakInfo[i].addr2 = g_arrBreakInfo[i].val2;
		}
	}

	

	//if (g_bTelePortalHook)
	//{
	//	VirtualProtect((LPVOID)HOOK_TELEPORTAL_ADDR, 5, 0x40, &OldProtect);
	//	*(BYTE*)(HOOK_TELEPORTAL_ADDR) =  0x50;
	//	*(DWORD*)(HOOK_TELEPORTAL_ADDR+1) = 0xD2FFCE8B;
	//	VirtualProtect((LPVOID)HOOK_TELEPORTAL_ADDR, 5, OldProtect, &OldProtect);
	//}

	if (g_bHookRecv)
	{
		VirtualProtect((LPVOID)HOOK_RECV_FUNC, 5, 0x40, &OldProtect);
		*(PDWORD)HOOK_RECV_FUNC = HOOK_RECV_VAL;
		*(PBYTE)(HOOK_RECV_FUNC+4) = 0x0;
		VirtualProtect((LPVOID)HOOK_RECV_FUNC, 5, OldProtect, &NewProtect);
	}	

	

	if (g_bMujok)
	{
		if (VirtualProtect((LPVOID)MEMADDR_HACKTIMENONENEMMY, 6, 0x40, &OldProtect))
		{
			*(BYTE*)(MEMADDR_HACKTIMENONENEMMY)		= 0x0F;
			*(BYTE*)(MEMADDR_HACKTIMENONENEMMY+1)	= 0x85;
			*(DWORD*)(MEMADDR_HACKTIMENONENEMMY+2)	= dw_ORGJMP;
			VirtualProtect((LPVOID)MEMADDR_HACKTIMENONENEMMY, 6, OldProtect, &NewProtect);
		}
	}

// 	if (g_bSonGongRaming)
// 	{
// 		VirtualProtect((LPVOID)HACK_SONJE_ATTACK_ADDR, 2, 0x40, &OldProtect);
// 		*(WORD*)HACK_SONJE_ATTACK_ADDR = 0x850F;
// 		VirtualProtect((LPVOID)HACK_SONJE_ATTACK_ADDR, 2, OldProtect, &OldProtect);
// 
// 		VirtualProtect((LPVOID)HACK_SONJE_RAMING_ADDR, 2, 0x40, &OldProtect);
// 		*(WORD*)HACK_SONJE_RAMING_ADDR = 0x86DD;
// 		VirtualProtect((LPVOID)HACK_SONJE_RAMING_ADDR, 2, OldProtect, &OldProtect);
// 	}

// 	if (g_bAllMobLeft)
// 	{
// 		VirtualProtect((LPVOID)HACK_ALLMOBMOLI_LEFT, 5, 0x40, &OldProtect);
// 		*(PDWORD)HACK_ALLMOBMOLI_LEFT		= HACK_ALLMOBMOLI_VAL1;
// 		*(PBYTE)(HACK_ALLMOBMOLI_LEFT+4)	= 0xD0;
// 		VirtualProtect((LPVOID)HACK_ALLMOBMOLI_LEFT, 5, OldProtect, &NewProtect);
// 	}

	if (g_bEffectRemove)
	{
		VirtualProtect((LPVOID)(HACK_EFFECT_REMOVE_ADDR+7), 2, 0x40, &OldProtect);
		*(WORD*)(HACK_EFFECT_REMOVE_ADDR+7) = 0x0A74;
		VirtualProtect((LPVOID)(HACK_EFFECT_REMOVE_ADDR+7), 2, OldProtect, &OldProtect);

		VirtualProtect((LPVOID)(HACK_EFFECT_REMOVE_ADDR+0xD), 2, 0x40, &OldProtect);
		*(WORD*)(HACK_EFFECT_REMOVE_ADDR+0xD) = 0x850F;
		VirtualProtect((LPVOID)(HACK_EFFECT_REMOVE_ADDR+0xD), 2, OldProtect, &OldProtect);
	}

	if (g_bSiluetRemove)
	{
		VirtualProtect((LPVOID)HACK_SILUET_REMOVE_ADDR, 2, 0x40, &OldProtect);
		*(WORD*)HACK_SILUET_REMOVE_ADDR = 0x870F;
		VirtualProtect((LPVOID)HACK_SILUET_REMOVE_ADDR, 2, OldProtect, &OldProtect);
	}

// 	if (g_bDupe)
// 	{
// 		VirtualProtect((LPVOID)MEMADDR_HACK_SS_MAVAK, 6, 0x40, &OldProtect);
// 		*(WORD*)MEMADDR_HACK_SS_MAVAK		= 0x9E89;
// 		*(DWORD*)(MEMADDR_HACK_SS_MAVAK+2)	= 0xF4;
// 		VirtualProtect((LPVOID)MEMADDR_HACK_SS_MAVAK, 6, OldProtect, &OldProtect);
// 	}

// 	if (g_bMobStance)
// 	{
// 		//VirtualProtect((LPVOID)HOOK_MOBSTANCE, 2, 0x40, &OldProtect);
// 		*(WORD*)HOOK_MOBSTANCE = 0x3374;
// 		//VirtualProtect((LPVOID)HOOK_MOBSTANCE, 2, OldProtect, &OldProtect);
// 	}

// 	if (g_bMobNoResponse)
// 	{
// 		VirtualProtect((LPVOID)HACK_MOB_NORESPONSE, 1, 0x40, &OldProtect);
// 		*(BYTE*)HACK_MOB_NORESPONSE = 0x79;
// 		VirtualProtect((LPVOID)HACK_MOB_NORESPONSE, 1, OldProtect, &OldProtect);
// 	}
}


void __cdecl MyHackPatchFunc1(DWORD a1, DWORD a2, DWORD a3, DWORD a4, DWORD a5, DWORD a6)
{
	DWORD ret;
	DWORD OldProtect, NewProtect;
	BYTE* lpMemAddr;
	BYTE var_8[2] = {0};
	DWORD FN_PATCHFUNC;
	int ebx = 0;
	DWORD dwEbp;

	//DebugMessageA(1, "MyHackPatchFunc1 enter");
	UnHookHackAddr();

	// -- 핵패치2 (ORI)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc2.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		memcpy(lpMemAddr, hfiHackPatchFunc2.SaveBytes, 8);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);
	}

	// -- 핵패치3 (ORI)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc3.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		memcpy(lpMemAddr, hfiHackPatchFunc3.SaveBytes, 8);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);
	}

	// -- 핵패치4 (ORI)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc4.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		memcpy(lpMemAddr, hfiHackPatchFunc4.SaveBytes, 8);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);
	}

	// -- 핵패치5 (ORI)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc5.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		memcpy(lpMemAddr, hfiHackPatchFunc5.SaveBytes, 8);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);
	}

	// -- 핵패치1 (ORI - HOOK)
	if (VirtualProtect((LPVOID)MEMADDR_HACKPATCHFUNC1, 6, 0x40, &OldProtect))
	{
		memcpy((LPBYTE)MEMADDR_HACKPATCHFUNC1, haiHackPatchFunc1.SaveBytes, 6);	
		VirtualProtect((LPVOID)MEMADDR_HACKPATCHFUNC1, 6, OldProtect, &NewProtect);
	}

	FN_PATCHFUNC = (DWORD)MEMADDR_HACKPATCHFUNC1;
	if (VirtualProtect((LPVOID)MEMADDR_HACKPATCHFUNC1, 6, 0x10, &OldProtect))
	{
		__asm
		{
			push a6;
			push a5;
			push a4;
			push a3;
			push a2;
			push a1;
			call FN_PATCHFUNC;
			add esp, 0x18;
			mov ret, eax;
		}
		VirtualProtect((LPVOID)MEMADDR_HACKPATCHFUNC1, 6, OldProtect, &NewProtect);
	}

	if (VirtualProtect((LPVOID)MEMADDR_HACKPATCHFUNC1, 6, 0x40, &OldProtect))
	{
		memcpy((LPBYTE)MEMADDR_HACKPATCHFUNC1, (LPBYTE)(&haiHackPatchFunc1.hookBytes), 6);	
		VirtualProtect((LPVOID)MEMADDR_HACKPATCHFUNC1, 6, OldProtect, &NewProtect);
	}

	// -- 핵패치2 (HOOK)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc2.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		var_8[0] = 0x58;  var_8[1] = 0x51;
		*(WORD*)lpMemAddr = *(WORD*)var_8;
		*(BYTE*)(lpMemAddr+2) = 0x50;
		memcpy(lpMemAddr+3, (LPBYTE)(&hfiHackPatchFunc2.hookBytes), 5);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);	
	}

	// -- 핵패치3 (HOOK)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc3.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		var_8[0] = 0x58;  var_8[1] = 0x51;
		*(WORD*)lpMemAddr = *(WORD*)var_8;
		*(BYTE*)(lpMemAddr+2) = 0x50;
		memcpy(lpMemAddr+3, (LPBYTE)(&hfiHackPatchFunc3.hookBytes), 5);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);	
	}

	// -- 핵패치4 (HOOK)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc4.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		var_8[0] = 0x58;  var_8[1] = 0x51;
		*(WORD*)lpMemAddr = *(WORD*)var_8;
		*(BYTE*)(lpMemAddr+2) = 0x50;
		memcpy(lpMemAddr+3, (LPBYTE)(&hfiHackPatchFunc4.hookBytes), 5);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);	
	}

	// 	// -- 핵패치5 (HOOK)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc5.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		var_8[0] = 0x58;  var_8[1] = 0x51;
		*(WORD*)lpMemAddr = *(WORD*)var_8;
		*(BYTE*)(lpMemAddr+2) = 0x50;
		memcpy(lpMemAddr+3, (LPBYTE)(&hfiHackPatchFunc5.hookBytes), 5);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);	
	}

	HookHackAddr();

	//	PrintLogA("MyHackPatchFunc1 End");
	_asm
	{
		mov eax, ret;
	}	
}

void __stdcall MyHackPatchFunc2(DWORD a0, DWORD a1, DWORD a2, DWORD a3, DWORD a4)
{
	DWORD ret;
	DWORD lpHackFunc2;
	BYTE var_8[2] = {0};
	DWORD NewProtect,OldProtect;
	BYTE *lpMemAddr;

	//DebugMessageA(1, "MyHackPatchFunc2 enter");

	UnHookHackAddr();

	// -- 핵패치1 (ORI)
	if (VirtualProtect((LPVOID)MEMADDR_HACKPATCHFUNC1, 6, 0x40, &NewProtect))
	{
		memcpy((LPBYTE)MEMADDR_HACKPATCHFUNC1, haiHackPatchFunc1.SaveBytes, 6);	
		VirtualProtect((LPVOID)MEMADDR_HACKPATCHFUNC1, 6, NewProtect, &OldProtect);
	}
	// 
	// -- 핵패치3 (ORI)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc3.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		memcpy(lpMemAddr, hfiHackPatchFunc3.SaveBytes, 8);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);
	}

	// -- 핵패치4 (ORI)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc4.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		memcpy(lpMemAddr, hfiHackPatchFunc4.SaveBytes, 8);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);
	}
	// -- 핵패치5 (ORI)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc5.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		memcpy(lpMemAddr, hfiHackPatchFunc5.SaveBytes, 8);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);
	}

	// -- 핵패치2 (ORI - HOOK)
	lpHackFunc2 = hfiHackPatchFunc2.OrgProc;
	if (VirtualProtect((LPVOID)lpHackFunc2, 8, 0x40, &NewProtect))
	{
		memcpy((LPBYTE)lpHackFunc2, hfiHackPatchFunc2.SaveBytes, 8);
		_asm
		{
			push a4;
			push a3;
			push a2;
			push a1;
			mov ecx, a0;
			call lpHackFunc2;
			mov ret, eax;
		}

		var_8[0] = 0x58;
		var_8[1] = 0x51;
		*(WORD*)lpHackFunc2 = *(WORD*)var_8;
		*(BYTE*)(lpHackFunc2+2) = 0x50;
		memcpy((LPBYTE)(lpHackFunc2+3), (LPBYTE)(&hfiHackPatchFunc2.hookBytes), 5);
		VirtualProtect((LPVOID)lpHackFunc2, 8, NewProtect, &OldProtect);
	}
	// -- 핵패치1 (HOOK)
	if (VirtualProtect((LPVOID)MEMADDR_HACKPATCHFUNC1, 6, 0x40, &NewProtect))
	{
		memcpy((LPBYTE)MEMADDR_HACKPATCHFUNC1, (LPBYTE)(&haiHackPatchFunc1.hookBytes), 6);	
		VirtualProtect((LPVOID)MEMADDR_HACKPATCHFUNC1, 6, NewProtect, &OldProtect);
	}
	// -- 핵패치3 (HOOK)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc3.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		var_8[0] = 0x58;  var_8[1] = 0x51;
		*(WORD*)lpMemAddr = *(WORD*)var_8;
		*(BYTE*)(lpMemAddr+2) = 0x50;
		memcpy(lpMemAddr+3, (LPBYTE)(&hfiHackPatchFunc3.hookBytes), 5);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);	
	}
	// -- 핵패치4 (HOOK)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc4.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		var_8[0] = 0x58;  var_8[1] = 0x51;
		*(WORD*)lpMemAddr = *(WORD*)var_8;
		*(BYTE*)(lpMemAddr+2) = 0x50;
		memcpy(lpMemAddr+3, (LPBYTE)(&hfiHackPatchFunc4.hookBytes), 5);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);	
	}

	// 	// -- 핵패치5 (HOOK)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc5.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		var_8[0] = 0x58;  var_8[1] = 0x51;
		*(WORD*)lpMemAddr = *(WORD*)var_8;
		*(BYTE*)(lpMemAddr+2) = 0x50;
		memcpy(lpMemAddr+3, (LPBYTE)(&hfiHackPatchFunc5.hookBytes), 5);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);	
	}

	HookHackAddr();
	_asm
	{
		mov eax, ret
	}
}

void __stdcall MyHackPatchFunc3(DWORD a0, DWORD a1, DWORD a2, DWORD a3)
{
	//DebugMessageA(1, "MyHackPatchFunc3 enter");
	DWORD ret;
	DWORD lpHackFunc3;
	BYTE var_8[2] = {0};
	DWORD NewProtect,OldProtect;
	BYTE *lpMemAddr;

	//DebugMessageA(3, "MyHackPatchFunc3 enter");

	UnHookHackAddr();

	// -- 핵패치1 (ORI)
	if (VirtualProtect((LPVOID)MEMADDR_HACKPATCHFUNC1, 6, 0x40, &NewProtect))
	{
		memcpy((LPBYTE)MEMADDR_HACKPATCHFUNC1, haiHackPatchFunc1.SaveBytes, 6);	
		VirtualProtect((LPVOID)MEMADDR_HACKPATCHFUNC1, 6, NewProtect, &OldProtect);
	}

	// -- 핵패치2 (ORI)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc2.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		memcpy(lpMemAddr, hfiHackPatchFunc2.SaveBytes, 8);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);
	}

	// -- 핵패치4 (ORI)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc4.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		memcpy(lpMemAddr, hfiHackPatchFunc4.SaveBytes, 8);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);
	}

	// -- 핵패치5 (ORI)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc5.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		memcpy(lpMemAddr, hfiHackPatchFunc5.SaveBytes, 8);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);
	}

	// -- 핵패치3 (ORI - HOOK)
	lpHackFunc3 = hfiHackPatchFunc3.OrgProc;
	if (VirtualProtect((LPVOID)lpHackFunc3, 8, 0x40, &NewProtect))
	{
		memcpy((LPBYTE)lpHackFunc3, hfiHackPatchFunc3.SaveBytes, 8);
		_asm
		{		
			push a3;
			push a2;
			push a1;
			mov ecx, a0;
			call lpHackFunc3;
		}

		var_8[0] = 0x58;
		var_8[1] = 0x51;
		*(WORD*)lpHackFunc3 = *(WORD*)var_8;
		*(BYTE*)(lpHackFunc3+2) = 0x50;
		memcpy((LPBYTE)(lpHackFunc3+3), (LPBYTE)(&hfiHackPatchFunc3.hookBytes), 5);
		VirtualProtect((LPVOID)lpHackFunc3, 8, NewProtect, &OldProtect);
	}
	// -- 핵패치1 (HOOK)
	if (VirtualProtect((LPVOID)MEMADDR_HACKPATCHFUNC1, 6, 0x40, &NewProtect))
	{
		memcpy((LPBYTE)MEMADDR_HACKPATCHFUNC1, (LPBYTE)(&haiHackPatchFunc1.hookBytes), 6);	
		VirtualProtect((LPVOID)MEMADDR_HACKPATCHFUNC1, 6, NewProtect, &OldProtect);
	}

	// -- 핵패치2 (HOOK)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc2.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		var_8[0] = 0x58;  var_8[1] = 0x51;
		*(WORD*)lpMemAddr = *(WORD*)var_8;
		*(BYTE*)(lpMemAddr+2) = 0x50;
		memcpy(lpMemAddr+3, (LPBYTE)(&hfiHackPatchFunc2.hookBytes), 5);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);	
	}

	// -- 핵패치4 (HOOK)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc4.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		var_8[0] = 0x58;  var_8[1] = 0x51;
		*(WORD*)lpMemAddr = *(WORD*)var_8;
		*(BYTE*)(lpMemAddr+2) = 0x50;
		memcpy(lpMemAddr+3, (LPBYTE)(&hfiHackPatchFunc4.hookBytes), 5);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);	
	}

	// -- 핵패치5 (HOOK)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc5.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		var_8[0] = 0x58;  var_8[1] = 0x51;
		*(WORD*)lpMemAddr = *(WORD*)var_8;
		*(BYTE*)(lpMemAddr+2) = 0x50;
		memcpy(lpMemAddr+3, (LPBYTE)(&hfiHackPatchFunc5.hookBytes), 5);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);	
	}

	HookHackAddr();
	//DebugMessageA(1, "MyHackPatchFunc3 end");
}


void __stdcall MyHackPatchFunc4(DWORD a0, DWORD a1, DWORD a2, DWORD a3)
{
	DWORD ret;
	DWORD lpHackFunc4;
	BYTE var_8[2] = {0};
	DWORD NewProtect,OldProtect;
	BYTE *lpMemAddr;

	//DebugMessageA(1, "MyHackPatchFunc4 enter");

	UnHookHackAddr();

	// -- 핵패치1 (ORI)
	if (VirtualProtect((LPVOID)MEMADDR_HACKPATCHFUNC1, 6, 0x40, &NewProtect))
	{
		memcpy((LPBYTE)MEMADDR_HACKPATCHFUNC1, haiHackPatchFunc1.SaveBytes, 6);	
		VirtualProtect((LPVOID)MEMADDR_HACKPATCHFUNC1, 6, NewProtect, &OldProtect);
	}
	// 
	// -- 핵패치2 (ORI)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc2.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		memcpy(lpMemAddr, hfiHackPatchFunc2.SaveBytes, 8);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);
	}
	// -- 핵패치3 (ORI)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc3.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		memcpy(lpMemAddr, hfiHackPatchFunc3.SaveBytes, 8);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);
	}

	// -- 핵패치5 (ORI)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc5.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		memcpy(lpMemAddr, hfiHackPatchFunc5.SaveBytes, 8);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);
	}

	// -- 핵패치4 (ORI - HOOK)
	lpHackFunc4 = hfiHackPatchFunc4.OrgProc;
	if (VirtualProtect((LPVOID)lpHackFunc4, 8, 0x40, &NewProtect))
	{
		memcpy((LPBYTE)lpHackFunc4, hfiHackPatchFunc4.SaveBytes, 8);
		_asm
		{
			push a3;
			push a2;
			push a1;
			mov ecx, a0;
			call lpHackFunc4;
			;add esp, 0Ch;
		}

		var_8[0] = 0x58;
		var_8[1] = 0x51;
		*(WORD*)lpHackFunc4 = *(WORD*)var_8;
		*(BYTE*)(lpHackFunc4+2) = 0x50;
		memcpy((LPBYTE)(lpHackFunc4+3), (LPBYTE)(&hfiHackPatchFunc4.hookBytes), 5);
		VirtualProtect((LPVOID)lpHackFunc4, 8, NewProtect, &OldProtect);
	}
	// -- 핵패치1 (HOOK)
	if (VirtualProtect((LPVOID)MEMADDR_HACKPATCHFUNC1, 6, 0x40, &NewProtect))
	{
		memcpy((LPBYTE)MEMADDR_HACKPATCHFUNC1, (LPBYTE)(&haiHackPatchFunc1.hookBytes), 6);	
		VirtualProtect((LPVOID)MEMADDR_HACKPATCHFUNC1, 6, NewProtect, &OldProtect);
	}
	// -- 핵패치3 (HOOK)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc3.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		var_8[0] = 0x58;  var_8[1] = 0x51;
		*(WORD*)lpMemAddr = *(WORD*)var_8;
		*(BYTE*)(lpMemAddr+2) = 0x50;
		memcpy(lpMemAddr+3, (LPBYTE)(&hfiHackPatchFunc3.hookBytes), 5);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);	
	}

	// -- 핵패치2 (HOOK)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc2.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		var_8[0] = 0x58;  var_8[1] = 0x51;
		*(WORD*)lpMemAddr = *(WORD*)var_8;
		*(BYTE*)(lpMemAddr+2) = 0x50;
		memcpy(lpMemAddr+3, (LPBYTE)(&hfiHackPatchFunc2.hookBytes), 5);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);	
	}

	// -- 핵패치5 (HOOK)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc5.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		var_8[0] = 0x58;  var_8[1] = 0x51;
		*(WORD*)lpMemAddr = *(WORD*)var_8;
		*(BYTE*)(lpMemAddr+2) = 0x50;
		memcpy(lpMemAddr+3, (LPBYTE)(&hfiHackPatchFunc5.hookBytes), 5);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);	
	}


	HookHackAddr();

}



void __stdcall MyHackPatchFunc5(DWORD a0, DWORD a1)
{

	DWORD ret;
	DWORD lpHackFunc5;
	BYTE var_8[2] = {0};
	DWORD NewProtect,OldProtect;
	BYTE *lpMemAddr;

	// PrintLogA("MyHackPatchFunc2 Start");
	UnHookHackAddr();

	// -- 핵패치1 (ORI)
	if (VirtualProtect((LPVOID)MEMADDR_HACKPATCHFUNC1, 6, 0x40, &NewProtect))
	{
		memcpy((LPBYTE)MEMADDR_HACKPATCHFUNC1, haiHackPatchFunc1.SaveBytes, 6);	
		VirtualProtect((LPVOID)MEMADDR_HACKPATCHFUNC1, 6, NewProtect, &OldProtect);
	}
	// 
	// -- 핵패치2 (ORI)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc2.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		memcpy(lpMemAddr, hfiHackPatchFunc2.SaveBytes, 8);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);
	}
	// -- 핵패치3 (ORI)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc3.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		memcpy(lpMemAddr, hfiHackPatchFunc3.SaveBytes, 8);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);
	}
	// -- 핵패치4 (ORI)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc4.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		memcpy(lpMemAddr, hfiHackPatchFunc4.SaveBytes, 8);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);
	}	
	// -- 핵패치5 (ORI - HOOK)
	lpHackFunc5 = hfiHackPatchFunc5.OrgProc;
	if (VirtualProtect((LPVOID)lpHackFunc5, 8, 0x40, &NewProtect))
	{
		memcpy((LPBYTE)lpHackFunc5, hfiHackPatchFunc5.SaveBytes, 8);
		_asm
		{
			push a1;
			mov ecx, a0;
			call lpHackFunc5;
		}

		var_8[0] = 0x58;
		var_8[1] = 0x51;
		*(WORD*)lpHackFunc5 = *(WORD*)var_8;
		*(BYTE*)(lpHackFunc5+2) = 0x50;
		memcpy((LPBYTE)(lpHackFunc5+3), (LPBYTE)(&hfiHackPatchFunc5.hookBytes), 5);
		VirtualProtect((LPVOID)lpHackFunc5, 8, NewProtect, &OldProtect);
	}
	// -- 핵패치1 (HOOK)
	if (VirtualProtect((LPVOID)MEMADDR_HACKPATCHFUNC1, 6, 0x40, &NewProtect))
	{
		memcpy((LPBYTE)MEMADDR_HACKPATCHFUNC1, (LPBYTE)(&haiHackPatchFunc1.hookBytes), 6);	
		VirtualProtect((LPVOID)MEMADDR_HACKPATCHFUNC1, 6, NewProtect, &OldProtect);
	}
	// -- 핵패치3 (HOOK)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc3.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		var_8[0] = 0x58;  var_8[1] = 0x51;
		*(WORD*)lpMemAddr = *(WORD*)var_8;
		*(BYTE*)(lpMemAddr+2) = 0x50;
		memcpy(lpMemAddr+3, (LPBYTE)(&hfiHackPatchFunc3.hookBytes), 5);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);	
	}

	// -- 핵패치2 (HOOK)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc2.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		var_8[0] = 0x58;  var_8[1] = 0x51;
		*(WORD*)lpMemAddr = *(WORD*)var_8;
		*(BYTE*)(lpMemAddr+2) = 0x50;
		memcpy(lpMemAddr+3, (LPBYTE)(&hfiHackPatchFunc2.hookBytes), 5);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);	
	}
	// -- 핵패치4 (HOOK)
	lpMemAddr = (BYTE*)(hfiHackPatchFunc4.OrgProc);
	if (VirtualProtect(lpMemAddr, 8, 0x40, &OldProtect))
	{
		var_8[0] = 0x58;  var_8[1] = 0x51;
		*(WORD*)lpMemAddr = *(WORD*)var_8;
		*(BYTE*)(lpMemAddr+2) = 0x50;
		memcpy(lpMemAddr+3, (LPBYTE)(&hfiHackPatchFunc4.hookBytes), 5);
		VirtualProtect(lpMemAddr, 8, OldProtect, &NewProtect);	
	}	
	HookHackAddr();
}


void InitForHackPatch1()
{
	DWORD OldProtect, NewProtect;
	VirtualProtect((LPVOID)MEMADDR_HACKPATCHFUNC1, 6, 0x40, &OldProtect);
	haiHackPatchFunc1.OrgProc = (FARPROC)MEMADDR_HACKPATCHFUNC1;
	haiHackPatchFunc1.HookProc = (FARPROC)MyHackPatchFunc1;
	memcpy(haiHackPatchFunc1.SaveBytes, (LPBYTE)MEMADDR_HACKPATCHFUNC1, 6);
	haiHackPatchFunc1.hookBytes.code1 = 0x68;
	haiHackPatchFunc1.hookBytes.address = (DWORD)MyHackPatchFunc1; 
	haiHackPatchFunc1.hookBytes.code2 = 0xC3;
	memcpy((LPBYTE)MEMADDR_HACKPATCHFUNC1, (LPBYTE)(&haiHackPatchFunc1.hookBytes), 6);
	VirtualProtect((LPVOID)MEMADDR_HACKPATCHFUNC1, 6, OldProtect, &NewProtect); 
}

void InitForHackPatch2()
{
	BYTE var_8[2] = {0};
	DWORD lpHackPatchFunc2;
	DWORD OldProtect, NewProtect;

	hfiHackPatchFunc2.OrgProc = MEMADDR_HACKPATCHFUNC2;
	lpHackPatchFunc2 = hfiHackPatchFunc2.OrgProc;

	VirtualProtect((LPVOID)lpHackPatchFunc2, 8, 0x40, &OldProtect);
	hfiHackPatchFunc2.HookProc = (DWORD)/*sub_51103A0;*/MyHackPatchFunc2;
	hfiHackPatchFunc2.hookBytes.code = 0xE9; // jmp
	hfiHackPatchFunc2.hookBytes.offset = (DWORD)MyHackPatchFunc2/*sub_51103A0*/ - lpHackPatchFunc2 - 8;	
	memcpy(hfiHackPatchFunc2.SaveBytes,(LPBYTE)lpHackPatchFunc2, 8);

	var_8[0] = 0x58;
	var_8[1] = 0x51;
	*(WORD*)lpHackPatchFunc2 = *(WORD*)var_8;
	*(BYTE*)(lpHackPatchFunc2+2) = 0x50;
	memcpy((LPBYTE)(lpHackPatchFunc2+3), (LPBYTE)(&hfiHackPatchFunc2.hookBytes), 5);

	VirtualProtect((LPVOID)lpHackPatchFunc2, 8, OldProtect, &NewProtect);
}

void InitForHackPatch3()
{
	BYTE	var_8[2] = {0};
	DWORD	lpHackPatchFunc3;
	DWORD	OldProtect, NewProtect;

	hfiHackPatchFunc3.OrgProc = MEMADDR_HACKPATCHFUNC3;
	lpHackPatchFunc3 = hfiHackPatchFunc3.OrgProc;

	VirtualProtect((LPVOID)lpHackPatchFunc3, 8, 0x40, &OldProtect);
	hfiHackPatchFunc3.HookProc = (DWORD) MyHackPatchFunc3;
	hfiHackPatchFunc3.hookBytes.code = 0xE9; //jmp
	hfiHackPatchFunc3.hookBytes.offset = (DWORD)MyHackPatchFunc3 - lpHackPatchFunc3 - 8;
	memcpy(hfiHackPatchFunc3.SaveBytes, (LPBYTE)lpHackPatchFunc3, 8);

	var_8[0] = 0x58;
	var_8[1] = 0x51;
	*(WORD*)lpHackPatchFunc3 = *(WORD*)var_8;
	*(BYTE*)(lpHackPatchFunc3 + 2) = 0x50;
	memcpy((LPBYTE)(lpHackPatchFunc3+3), (LPBYTE)(&hfiHackPatchFunc3.hookBytes), 5);

	VirtualProtect((LPVOID)lpHackPatchFunc3, 8, OldProtect, &NewProtect);
}


void InitForHackPatch4()
{
	BYTE var_8[2] = {0};
	DWORD lpHackPatchFunc4;
	DWORD OldProtect, NewProtect;

	hfiHackPatchFunc4.OrgProc = MEMADDR_HACKPATCHFUNC4;
	lpHackPatchFunc4 = hfiHackPatchFunc4.OrgProc;

	VirtualProtect((LPVOID)lpHackPatchFunc4, 8, 0x40, &OldProtect);
	hfiHackPatchFunc4.HookProc = (DWORD)/*sub_51103A0;*/MyHackPatchFunc4;
	hfiHackPatchFunc4.hookBytes.code = 0xE9; // jmp
	hfiHackPatchFunc4.hookBytes.offset = (DWORD)MyHackPatchFunc4/*sub_51103A0*/ - lpHackPatchFunc4 - 8;	
	memcpy(hfiHackPatchFunc4.SaveBytes,(LPBYTE)lpHackPatchFunc4, 8);

	var_8[0] = 0x58;
	var_8[1] = 0x51;
	*(WORD*)lpHackPatchFunc4 = *(WORD*)var_8;
	*(BYTE*)(lpHackPatchFunc4+2) = 0x50;
	memcpy((LPBYTE)(lpHackPatchFunc4+3), (LPBYTE)(&hfiHackPatchFunc4.hookBytes), 5);

	VirtualProtect((LPVOID)lpHackPatchFunc4, 8, OldProtect, &NewProtect);
}

void InitForHackPatch5()
{
	BYTE var_8[2] = {0};
	DWORD lpHackPatchFunc5;
	DWORD OldProtect, NewProtect;

	hfiHackPatchFunc5.OrgProc = MEMADDR_HACKPATCHFUNC5;
	lpHackPatchFunc5 = hfiHackPatchFunc5.OrgProc;

	VirtualProtect((LPVOID)lpHackPatchFunc5, 8, 0x40, &OldProtect);
	hfiHackPatchFunc5.HookProc = (DWORD)MyHackPatchFunc5;
	hfiHackPatchFunc5.hookBytes.code = 0xE9; // jmp
	hfiHackPatchFunc5.hookBytes.offset = (DWORD)MyHackPatchFunc5 - lpHackPatchFunc5 - 8;	
	memcpy(hfiHackPatchFunc5.SaveBytes,(LPBYTE)lpHackPatchFunc5, 8);

	var_8[0] = 0x58;
	var_8[1] = 0x51;
	*(WORD*)lpHackPatchFunc5 = *(WORD*)var_8;
	*(BYTE*)(lpHackPatchFunc5+2) = 0x50;
	memcpy((LPBYTE)(lpHackPatchFunc5+3), (LPBYTE)(&hfiHackPatchFunc5.hookBytes), 5);

	VirtualProtect((LPVOID)lpHackPatchFunc5, 8, OldProtect, &NewProtect);
}

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:	
		g_ImageBase = (HMODULE)hModule;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}

    return TRUE;
}


// 120405 캐릭네임으로부터 게임아이템교환네임 얻기
DWORD GetOtherCharacterExChangeID( CString strShopName )
{
	DWORD dwMemAddr, dwTemp, ecx = 0;
	DWORD dwPtrAddr;
	POINT pt;

	dwMemAddr = MEMADDR_OTHERCHARICINFO;
	if (IsBadReadPtr((LPVOID)dwMemAddr, 4) == TRUE)
	{
		return 0;
	}
	dwMemAddr = *(LPDWORD)dwMemAddr;
	dwMemAddr = dwMemAddr + MEMADDR_OTHERCHARICINFO_OFFSET1;
	if (IsBadReadPtr((LPVOID)dwMemAddr, 4) ==  FALSE)
	{
		dwMemAddr = *(LPDWORD)dwMemAddr;
		dwPtrAddr = dwMemAddr;

		while (dwPtrAddr)
		{
			dwMemAddr = dwPtrAddr;

			dwMemAddr = dwMemAddr + 4;
			if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) { break; }
			dwMemAddr = *(LPDWORD)dwMemAddr;

			dwMemAddr = dwMemAddr + 16;
			if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) { break; }
			dwTemp = *(LPDWORD)dwMemAddr;

			dwMemAddr = dwTemp + MEMADDR_OTHERCHARICINFO_OFFSET2 - 4;
			if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) { break; }

			ecx = *(PDWORD)dwMemAddr;

			dwMemAddr = dwTemp + MEMADDR_OTHERCHARICINFO_OFFSET2;
			if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) { break; }
			dwMemAddr = *(LPDWORD)(dwMemAddr);

			if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) { break; }

			if (stricmp((char*)dwMemAddr, strShopName.GetBuffer(0)) == 0)
			{
				dwMemAddr = dwTemp + MEMADDR_EXCHANGECHARINFO;//0x22e8;
				if (!IsBadReadPtr((LPVOID)dwMemAddr, 4))
				{				 
					ecx = *(PDWORD)dwMemAddr;
					return ecx;
				}
			}

			if (IsBadReadPtr((LPVOID)dwPtrAddr, 4)) { break; }

			if (IsBadReadPtr((LPVOID)(dwPtrAddr - 12), 4)) { break; }
			dwMemAddr = *(LPDWORD)(dwPtrAddr - 12);
			if (dwMemAddr == 0) break;

			dwPtrAddr = dwMemAddr + 0x10;
		}
	}
	return 0;
}
// 
// // 120405 게임아이템교환창 열기
// void SendExChangePacket( CString strCharName )
// {
// 	DWORD dwExChangeID = GetOtherCharacterExChangeID(strCharName);
// 	// 	
// 	BYTE var_2C[0x2C];
// 	DWORD var_4;
// 	DWORD sub_71E650 = ADDR_SEND_OPCODE, sub_415860 = ADDR_SEND_BYTE, sub_4158B0 = ADDR_SEND_DWORD, sub_4D0C00 = ADDR_SEND_FUNC,
// 		sub_71E5B0 = ADDR_SEND_EXCHANGE;
// 
// 	BYTE var_38[0x38], var_48[0x48];
// 	DWORD sub_GetVersion = 0x7C81126A;// kernel32:GetVersion
// 	DWORD sub_FD9234 = 0xFD9234;	
// 
// 	if (dwExChangeID)
// 	{
// 		__asm
// 		{
// 			pushad;
// 			mov		edi, sub_FD9234;
// 			mov		edi, [edi];
// 			xor		ebx, ebx;
// 			push    122h;
// 			lea     ecx, var_38;
// 			call    sub_71E650;
// 			mov     ecx, MEMADDR_PREPROCESSCLASS_STARTBOT;
// 			mov		ecx, [ecx];
// 			cmp     [ecx+60h], ebx;
// 			lea     ecx, var_38;
// 			setnz   dl;
// 			movzx   eax, dl;
// 			push    eax;
// 			mov     var_4, ebx;
// 			call    sub_415860;
// 			mov     cl, [edi+4Dh];
// 			mov     byte ptr [var_48], cl;
// 			lea     edx, var_48;
// 			mov		edx, [edx];
// 			push    edx;
// 			lea     ecx, var_38;
// 			call    sub_415860;
// 			mov     al, [edi+4Ch];
// 			mov     byte ptr [var_48], al;
// 			lea     ecx, var_48;
// 			mov		ecx, [ecx];
// 			push    ecx;
// 			lea     ecx, var_38;
// 			call    sub_415860;
// 			call    sub_GetVersion;
// 			cmp     eax, 80000000h;
// 			setnb   dl;
// 			movzx   eax, dl;
// 			push    eax;
// 			lea     ecx, var_38;
// 			call    sub_415860;
// 			lea     ecx, var_38;
// 			push    ecx;
// 			mov     ecx, ADDR_SOCKET_OBJ;
// 			mov		ecx, [ecx];
// 			call    sub_4D0C00;
// 
// 			push    PACKET_OPCODE;
// 			lea     ecx, var_2C;
// 			call    sub_71E650;
// 			push    6;
// 			lea     ecx, var_2C;
// 			mov     var_4, 4;
// 			call    sub_415860;				
// 			push    3;
// 			lea     ecx, var_2C;
// 			call    sub_415860;
// 			push    0;
// 			lea     ecx, var_2C;
// 			call    sub_415860;
// 			lea     ecx, var_2C;
// 			push    ecx;
// 			mov     ecx, ADDR_SOCKET_OBJ;
// 			mov		ecx, [ecx];
// 			call    sub_4D0C00;
// 
// 			push    PACKET_OPCODE;
// 			lea     ecx, var_2C;
// 			call    sub_71E5B0;
// 			push    0Bh;
// 			lea     ecx, var_2C;
// 			call    sub_415860;
// 			mov     esi, dwExChangeID;
// 			push    esi;
// 			lea     ecx, var_2C;
// 			call    sub_4158B0;
// 			lea     ecx, var_2C;
// 			push    ecx;
// 			mov     ecx, ADDR_SOCKET_OBJ;
// 			mov		ecx, [ecx];
// 			call    sub_4D0C00;
// 			popad;
// 		}
// 	}
// }
// 
// void SendMoveItemPacket(int nTab, int nMySlot, int nCount, int nExChangeSlot)
// {
// 	BYTE var_1C[0x1C];
// 	DWORD var_4;
// 	DWORD sub_71E650 = ADDR_SEND_OPCODE, sub_415860 = ADDR_SEND_BYTE, sub_433880 = ADDR_SEND_WORD, sub_4D0C00 = ADDR_SEND_FUNC;
// 	//PrintLogA("SendMoveItemPacket");
// 	// 875de8
// 
// 	BYTE var_38[0x38], var_48[0x48];
// 	DWORD sub_GetVersion = 0x7C81126A;// kernel32:GetVersion
// 	DWORD sub_FD9234 = 0xFD9234;
// 
// 
// 	__asm
// 	{
// 		pushad;
// 		mov		edi, sub_FD9234;
// 		mov		edi, [edi];
// 		xor		ebx, ebx;
// 		push    122h;
// 		lea     ecx, var_38;
// 		call    sub_71E650;
// 		mov     ecx, MEMADDR_PREPROCESSCLASS_STARTBOT;
// 		mov		ecx, [ecx];
// 		cmp     [ecx+60h], ebx;
// 		lea     ecx, var_38;
// 		setnz   dl;
// 		movzx   eax, dl;
// 		push    eax;
// 		mov     var_4, ebx;
// 		call    sub_415860;
// 		mov     cl, [edi+4Dh];
// 		mov     byte ptr [var_48], cl;
// 		lea     edx, var_48;
// 		mov		edx, [edx];
// 		push    edx;
// 		lea     ecx, var_38;
// 		call    sub_415860;
// 		mov     al, [edi+4Ch];
// 		mov     byte ptr [var_48], al;
// 		lea     ecx, var_48;
// 		mov		ecx, [ecx];
// 		push    ecx;
// 		lea     ecx, var_38;
// 		call    sub_415860;
// 		call    sub_GetVersion;
// 		cmp     eax, 80000000h;
// 		setnb   dl;
// 		movzx   eax, dl;
// 		push    eax;
// 		lea     ecx, var_38;
// 		call    sub_415860;
// 		lea     ecx, var_38;
// 		push    ecx;
// 		mov     ecx, ADDR_SOCKET_OBJ;
// 		mov		ecx, [ecx];
// 		call    sub_4D0C00;
// 
// 
// 		push    PACKET_OPCODE;
// 		lea     ecx, var_1C;
// 		call    sub_71E650;
// 		mov		ebx, 0;
// 		push    ebx;
// 		lea     ecx, var_1C;
// 		mov     var_4, 5;
// 		call    sub_415860;
// 		mov     eax, nTab;
// 		push    eax;
// 		lea     ecx, var_1C;
// 		call    sub_415860;
// 		mov     ecx, nMySlot;
// 		push    ecx;
// 		lea     ecx, var_1C;
// 		call    sub_433880;
// 		mov		eax, nCount;
// 		push    eax;
// 		lea     ecx, var_1C;
// 		call    sub_433880;
// 		mov		eax, nExChangeSlot;
// 		push    eax;
// 		lea     ecx, var_1C;
// 		call    sub_415860;
// 		lea     ecx, var_1C;
// 		push    ecx;
// 		mov     ecx, ADDR_SOCKET_OBJ;
// 		mov		ecx, [ecx];
// 		call    sub_4D0C00;
// 		popad;
// 	}
// }
// 
// void SendExChangeMesoPacket(DWORD dwGold)
// {
// 	BYTE var_1C[0x1C];
// 	DWORD var_4;
// 	DWORD sub_71E650 = ADDR_SEND_OPCODE, sub_415860 = ADDR_SEND_BYTE, sub_4158B0 = ADDR_SEND_DWORD, sub_4D0C00 = ADDR_SEND_FUNC;
// 	//PrintLogA("SendExChangeMesoPacket");
// 
// 	BYTE var_38[0x38], var_48[0x48];
// 	DWORD sub_GetVersion = 0x7C81126A;// kernel32:GetVersion
// 	DWORD sub_FD9234 = 0xFD9234;
// 
// 
// 	__asm
// 	{
// 		pushad;
// 		mov		edi, sub_FD9234;
// 		mov		edi, [edi];
// 		xor		ebx, ebx;
// 		push    122h;
// 		lea     ecx, var_38;
// 		call    sub_71E650;
// 		mov     ecx, MEMADDR_PREPROCESSCLASS_STARTBOT;
// 		mov		ecx, [ecx];
// 		cmp     [ecx+60h], ebx;
// 		lea     ecx, var_38;
// 		setnz   dl;
// 		movzx   eax, dl;
// 		push    eax;
// 		mov     var_4, ebx;
// 		call    sub_415860;
// 		mov     cl, [edi+4Dh];
// 		mov     byte ptr [var_48], cl;
// 		lea     edx, var_48;
// 		mov		edx, [edx];
// 		push    edx;
// 		lea     ecx, var_38;
// 		call    sub_415860;
// 		mov     al, [edi+4Ch];
// 		mov     byte ptr [var_48], al;
// 		lea     ecx, var_48;
// 		mov		ecx, [ecx];
// 		push    ecx;
// 		lea     ecx, var_38;
// 		call    sub_415860;
// 		call    sub_GetVersion;
// 		cmp     eax, 80000000h;
// 		setnb   dl;
// 		movzx   eax, dl;
// 		push    eax;
// 		lea     ecx, var_38;
// 		call    sub_415860;
// 		lea     ecx, var_38;
// 		push    ecx;
// 		mov     ecx, ADDR_SOCKET_OBJ;
// 		mov		ecx, [ecx];
// 		call    sub_4D0C00;
// 
// 
// 		push    PACKET_OPCODE;
// 		lea     ecx, var_1C;
// 		call    sub_71E650;
// 		push    1;
// 		lea     ecx, var_1C;
// 		mov     var_4, 4;
// 		call    sub_415860;
// 		mov     eax, dwGold;
// 		push    eax;
// 		lea     ecx, var_1C;
// 		call    sub_4158B0;
// 		lea     ecx, var_1C;
// 		push    ecx;
// 		mov     ecx, ADDR_SOCKET_OBJ;
// 		mov		ecx, [ecx];
// 		call    sub_4D0C00;
// 		popad;
// 	}
// }
// 
// void  PickItem(DWORD XPos, DWORD YPos, DWORD ItemObject, DWORD ItemIndex)
// {
// 	
// 	BYTE var_1C[0x2C], var_20;
// 	DWORD var_4;
// 	DWORD sub_71E650 = ADDR_SEND_OPCODE, sub_415860 = ADDR_SEND_BYTE, sub_4158B0 = ADDR_SEND_DWORD, sub_4D0C00 = ADDR_SEND_FUNC,
// 		sub_435270 = ADDR_SEND_WORD;
// 	DWORD sub_44F080 = 0x44F860, sub_BF3CC0 = 0xC6EDF0;
// 	DWORD dwEdi = *(PDWORD)(ItemObject+0x20), dwEax;
// 	DWORD sub_64FE90 = 0x679CE0, dwTmp = 0;
// 	DWORD ADDR_ITEM_PROPERTY = 0x1146990;
// 	__asm
// 	{
// 		pushad;
// 		call    sub_44F080;
// 		mov		dwEax, eax;
// 		popad;
// 	}
// 	dwTmp = (DWORD)*(PBYTE)(dwEax+0x1A0);
// 	//PrintLogA("dwTmp = %x", dwTmp);
// 	__asm
// 	{
// 		pushad;
// 		mov		esi, ItemObject;
// 		mov     ecx, [esi+34h];
// 		push    ecx;
// 		mov     ecx, ADDR_ITEM_PROPERTY;
// 		mov		ecx, [ecx];
// 		call    sub_64FE90;
// 		mov		dwEax, eax;
// 		popad;
// 	}
// 	if (dwEdi == ItemIndex) return;
// 	PrintLogA("ItemObject = %x Eax %x Edi = %x ItemID = %x", ItemObject, dwEax, dwEdi, ItemIndex);
// 	
// 
// 	__asm
// 	{
// 		pushad;
// 		push    166h;
// 		lea     ecx, var_1C;
// 		call    sub_71E650;
// 		call    sub_44F080;
// 		mov     eax, dwTmp;
// 		push    eax
// 		lea     ecx, var_1C;
// 		call    sub_415860;
// 		call    sub_BF3CC0;
// 		push    eax
// 		lea     ecx, var_1C;
// 		call    sub_4158B0;
// 		mov		ecx, XPos;
// 		push    ecx;
// 		lea     ecx, var_1C;
// 		call    sub_435270;
// 		mov		edx, YPos;
// 		push    edx;
// 		lea     ecx, var_1C;
// 		call    sub_435270;
// 
// 		mov		edi, dwEdi;
// 		push    edi
// 		lea     ecx, var_1C;
// 		call    sub_4158B0;
// 
// 		mov     eax, dwEax;
// 		;push    eax;
// 		push    0h;
// 		lea     ecx, var_1C;
// 		call    sub_4158B0;
// 		lea     ecx, var_1C;
// 		push    ecx;
// 		mov     ecx, ADDR_SOCKET_OBJ;
// 		mov		ecx, [ecx];
// 		call    sub_4D0C00;
// 		popad;
// 	}
// 
// }
void SendExChangeFunc(char *szName)
{
	DWORD sub_44DB60 = 0x44DB60, sub_5821E0 = 0x5821E0;
	DWORD var_8C;
	var_8C = (DWORD)&szName[0];
	_asm
	{
		pushad;
		lea		edx, var_8C;
		xor		ebx, ebx
		push	ebx;
		push	edx;
		call    sub_44DB60;
		mov     ecx, eax;
		call    sub_5821E0;
		popad;
	}
}
char* GetWorldChannelName()
{
	PrintLogA("GetWorldChannelName start");
	DWORD esi, eax, ebx;
	DWORD dwMemAddr;

	dwMemAddr = MEMADDR_WORLDCHANNELNAME3;
	if (IsBadReadPtr((LPVOID)dwMemAddr, 4) == TRUE) 
	{
		PrintLogA("GetWorldChnnel rtn(1)");
		return NULL;
	}
	PrintLogA("1");

	ebx = MEMADDR_WORLDCHANNELNAME1;
	if (IsBadReadPtr((LPVOID)ebx, 4) == TRUE) 
	{
		PrintLogA("GetWorldChnnel rtn(2)");
		return NULL;
	}
	ebx = *(LPDWORD)ebx;

	PrintLogA("2: %x", ebx);

	esi = MEMADDR_WORLDCHANNELNAME2;
	if (IsBadReadPtr((LPVOID)esi, 4) == TRUE) 
	{
		PrintLogA("GetWorldChnnel rtn(3)");
		return NULL;
	}
	eax = *(LPDWORD)esi;

	PrintLogA("3: %x", eax);

	if (IsBadReadPtr((LPVOID)(eax+ebx*4), 4) == TRUE) 
	{
		PrintLogA("GetWorldChnnel rtn(4)");
		return NULL;
	}
	esi = *(LPDWORD)(eax + ebx * 4);

	PrintLogA("4: %x", esi);

	if (IsBadReadPtr((LPVOID)esi, 4) == TRUE) 
	{
		PrintLogA("GetWorldChnnel rtn(5)");
		return NULL; 
	}

	PrintLogA("채널네임: %s", (char*)esi);
	return (char*)esi;
}

int GetMapIdForBot()
{
	DWORD dwMemAddr;
	DWORD dwMapId = 0;
	dwMemAddr = MEMADDR_MAPID;
	if (IsBadReadPtr((LPVOID)dwMemAddr, 4) == FALSE)
	{
		// loc_10009B25
		dwMemAddr = *(LPDWORD)dwMemAddr;
		dwMemAddr += MEMADDR_MAPID_OFFSET;
		if (IsBadReadPtr((LPVOID)dwMemAddr, 4) == FALSE) 
		{
			dwMapId = *(LPDWORD)dwMemAddr;
		}
	}

	return (int)dwMapId;
}

BOOL ChangeAPICodes(PHOOK_API_INFO pHookApiInfo, FARPROC lpNewFunc)
{
	DWORD dwOldProtect;
	pHookApiInfo->HookProc = lpNewFunc;
	VirtualProtect(pHookApiInfo->OrgProc, 6, PAGE_EXECUTE_READWRITE, &dwOldProtect);

	pHookApiInfo->hookBytes.code1 = 0x68;
	pHookApiInfo->hookBytes.address = (DWORD)lpNewFunc;
	pHookApiInfo->hookBytes.code2 = 0xc3;
	memcpy(pHookApiInfo->SaveBytes, (LPBYTE)pHookApiInfo->OrgProc, 6);

	memcpy((LPBYTE)pHookApiInfo->OrgProc, (LPBYTE)(&pHookApiInfo->hookBytes), 6);

	VirtualProtect(pHookApiInfo->OrgProc, 6, dwOldProtect, &dwOldProtect);

	return TRUE;
}

BOOL RestoreAPICodes(PHOOK_API_INFO pApiInfo)
{
	DWORD dwOldProtect;

	if(pApiInfo == NULL) return FALSE;
	if(pApiInfo->OrgProc == NULL) return TRUE;

	if(!VirtualProtect(pApiInfo->OrgProc, 6, PAGE_EXECUTE_READWRITE, &dwOldProtect)) return FALSE;

	RtlCopyMemory(pApiInfo->OrgProc, pApiInfo->SaveBytes, 6);

	if(!VirtualProtect(pApiInfo->OrgProc, 6, dwOldProtect, &dwOldProtect)) return FALSE;

	return TRUE;
}

DWORD dword_12FE750 = 0x12FE750;
DWORD loc_FF6448 = 0xFF6448;
DWORD sub_80A000 = 0x80A000;
DWORD dword_1300284 = 0x1300284;
DWORD sub_4F7550 = 0x4F7550;
DWORD sub_402020 = 0x402020;
DWORD dword_13107B8 = 0x13107B8;


void DonjonEntrance_194()
{
	__asm
	{
		pushad
		push    0FFFFFFFFh;
		push    offset loc_FF6448;
		mov     eax, fs:0;
		push    eax;
		sub     esp, 10h;
		mov		eax, dword_12FE750;
		mov     eax, [eax];
		xor     eax, esp;
		push    eax;
		lea     eax, [esp+014h];
		mov     fs:0, eax
		push    194h
		lea     ecx, [esp+08h]
		call    sub_80A000
		mov		ecx , dword_1300284;
		mov     ecx, [ecx];
		lea     eax, [esp+04h]
		push    eax
		mov     [esp+020h], 0
		call    sub_4F7550
		mov     eax, [esp+08h]
		mov     [esp+01ch], 0FFFFFFFFh
		test    eax, eax
		jz      short loc_E8AE17
		add     eax, 0FFFFFFFCh
		push    eax
		mov     ecx, offset dword_13107B8
		call    sub_402020

loc_E8AE17:
		mov     ecx, [esp+014h]
		mov     fs:0, ecx
		pop     ecx
		add     esp, 1Ch
		popad;
		retn
	}
}

DWORD loc_FF6478 = 0xFF6478;
DWORD dword_1300434 = 0x1300434;
DWORD sub_415B70  = 0x415B70;

void DonjonEntrance_E0()
{
	__asm
	{
		push    0FFFFFFFFh;
		push    offset loc_FF6478;
		mov     eax, fs:0;
		push    eax;
		sub     esp, 10h;
		push    esi;
		mov		eax, dword_12FE750;
		mov     eax, [eax];
		xor     eax, esp
		push    eax
		lea     eax, [esp+018h]
		mov     fs:0, eax
		mov		esi, dword_1300434;
		mov     esi, [esi]
		test    esi, esi
		jz      short loc_E8AEB0
		push    0E0h
		lea     ecx, [esp+0ch]
		call    sub_80A000
		movzx   eax, byte ptr [esi+78h]
		push    eax
		lea     ecx, [esp+0ch]
		mov     [esp+024h], 0
		call    sub_415B70
		lea     ecx, [esp+08h]
		push    ecx
		mov		ecx, dword_1300284;
		mov     ecx, [ecx]
		call    sub_4F7550
		mov     eax, [esp+0ch]
		mov     [esp+020h], 0FFFFFFFFh
		test    eax, eax
		jz      short loc_E8AEB0
		add     eax, 0FFFFFFFCh
		push    eax
		mov     ecx, offset dword_13107B8
		call    sub_402020

loc_E8AEB0:                            

		mov     ecx, [esp+018h]
		mov     fs:0, ecx
		pop     ecx
		pop     esi
		add     esp, 1Ch
	}
	return;
}

DWORD GetBotMeso()
{
	//DebugMessageA(100, "GetBotMeso enter");
	DWORD dwMemAddr = MEMADDR_BOTINFO;
	if (IsBadReadPtr((LPVOID)dwMemAddr, 4) == TRUE)
	{
		return 0;
	}
	dwMemAddr = *(LPDWORD)dwMemAddr;
	dwMemAddr += MEMADDR_BOTMESO_OFFSET;

	if (IsBadReadPtr((LPVOID)dwMemAddr, 12) == TRUE)
	{
		return 0;
	}

	DWORD dwMeso = RolXor(*(LPDWORD)(dwMemAddr), *(LPDWORD)(dwMemAddr + 4), *(LPDWORD)(dwMemAddr + 8));
	//DebugMessageA(100, "GetBotMeso: MESO = %d", dwMeso);
	return dwMeso;
}


DWORD GetBotExp()
{
	//DebugMessageA(100, "GEtBotExp enter");
	DWORD dwReturn = 0;
	DWORD dwMemAddr;

	dwMemAddr = MEMADDR_BOTINFO;
	if (IsBadReadPtr((LPVOID)dwMemAddr, 4) == FALSE)
	{
		dwMemAddr = *(LPDWORD)dwMemAddr;
		//sdwReturn = *(PDWORD)(dwMemAddr+0x91);
		DWORD sub_432A00 = MEMADDR_EXPFUNC;
		__asm
		{
			pushad;
			mov		esi, dwMemAddr;
			mov     ecx, [esi+MEMADDR_BOTEXP_OFFSET2];
			push    ecx;
			lea     edx, [esi+MEMADDR_BOTEXP_OFFSET];
			push    edx;
			call    sub_432A00;
			add		esp, 8;
			mov		dwReturn, eax;
			popad;
		}
	}	
	return dwReturn;
}	

DWORD GetExpByLevel(BYTE bLevel)
{
	//DebugMessageA(100, "GetExpByLevel enter");
	if (IsBadReadPtr((LPVOID)MEMADDR_EXPBYLEVEL, 8)) 
	{			
		return 0;
	}

	if (bLevel < 1) 
	{

		return 0;
	}

	if (IsBadReadPtr((LPVOID)(MEMADDR_EXPBYLEVEL + 8 * bLevel), 4))
	{

		return 0;
	}

	DWORD dwRet = *(LPDWORD)(MEMADDR_EXPBYLEVEL + 8 * bLevel);
	DWORD dwRet1 = *(LPDWORD)(MEMADDR_EXPBYLEVEL + 8 * bLevel+4);

	PrintLogA("GetExpByLevel: EXP = %d, %d", dwRet, dwRet1);
	return dwRet;
}

WORD g_wKeyMouseState[0x30]={0};
void LeftKey_Down()
{
	if(g_wKeyMouseState[VK_LEFT])return;
	g_wKeyMouseState[VK_LEFT] = 0x8000;
	//PostMessage(g_hMainWnd, WM_KEYDOWN, VK_LEFT, (MapVirtualKeyA(VK_LEFT, 0) << 16) + 0x1000001);
}

void LeftKey_Up()
{
	if(g_wKeyMouseState[VK_LEFT]==0)return;
	g_wKeyMouseState[VK_LEFT] = 0;
	//PostMessage(g_hMainWnd, WM_KEYUP, VK_LEFT, (MapVirtualKeyA(VK_LEFT, 0) << 16) + 0xC1000001);
}
void RightKey_Down()
{
	if(g_wKeyMouseState[VK_RIGHT])return;
	g_wKeyMouseState[VK_RIGHT] = 0x8000;
	//PostMessage(g_hMainWnd, WM_KEYDOWN, VK_RIGHT, (MapVirtualKeyA(VK_RIGHT, 0) << 16) + 0x1000001);
}

void RightKey_Up()
{
	if(g_wKeyMouseState[VK_RIGHT]==0)return;
	g_wKeyMouseState[VK_RIGHT] = 0;
	//PostMessage(g_hMainWnd, WM_KEYUP, VK_RIGHT, (MapVirtualKeyA(VK_RIGHT, 0) << 16) + 0xC1000001);
}
void UpKey_Down()
{
	if(g_wKeyMouseState[VK_UP])return;
	g_wKeyMouseState[VK_UP] = 0x8000;
	//PostMessage(g_hMainWnd, WM_KEYDOWN, VK_UP, (MapVirtualKeyA(VK_UP, 0) << 16) + 0x1000001);
}
void UpKey_Up()
{
	if(g_wKeyMouseState[VK_UP]==0) return;
	g_wKeyMouseState[VK_UP] = 0;
	//PostMessage(g_hMainWnd, WM_KEYUP, VK_UP, (MapVirtualKeyA(VK_UP, 0) << 16) + 0xC1000001);
}
void DownKey_Down()
{
	if(g_wKeyMouseState[VK_DOWN])return;
	g_wKeyMouseState[VK_DOWN] = 0x8000;
	//PostMessage(g_hMainWnd, WM_KEYDOWN, VK_DOWN, (MapVirtualKeyA(VK_DOWN, 0) << 16) + 0x1000001);
}

void DownKey_Up()
{
	if(g_wKeyMouseState[VK_DOWN]==0)return;
	g_wKeyMouseState[VK_DOWN] = 0;
	//PostMessage(g_hMainWnd, WM_KEYUP, VK_DOWN, (MapVirtualKeyA(VK_DOWN, 0) << 16) + 0xC1000001);
}



DWORD GetBotAP()
{
	//DebugMessageA(100, "GetBotAP enter");
	DWORD dwMemAddr;
	DWORD dwAbilityPoint = 0;
	DWORD var_4;
	LPBYTE lpByte = (LPBYTE)(&var_4);
	DWORD eax, edx;

	dwMemAddr = MEMADDR_BOTINFO;

	if (IsBadReadPtr((LPVOID)dwMemAddr, 4) == FALSE)
	{
		dwMemAddr = *(LPDWORD)dwMemAddr;
		dwMemAddr += MEMADDR_BOTAP_OFFSET;

		if (IsBadReadPtr((LPVOID)dwMemAddr,4) == FALSE)
		{
			var_4 = *(LPDWORD)dwMemAddr;

			eax = lpByte[0] ^ lpByte[2];
			edx = lpByte[1] ^ lpByte[3];
			dwAbilityPoint = (edx << 8) + eax;
		}
	}

	PrintLogA("GetBotAP: AP = %d", dwAbilityPoint);

	return dwAbilityPoint;
}


int WINAPI Fn_1()
{	
	// 상점열기
	//CString strShop = "엔젤리나08";
	//DWORD dwTmp;
	//DWORD dwShopID = GetOtherCharacterShopID(strShop, dwTmp);
	//PrintLogA("상점아이디 = %d, Flag = %d", dwShopID, dwTmp);
	//OpenPersernalShop(dwShopID);

	// 캐릭위치
 	POINT ptBot;
 	DWORD dwMapId;
 	GetBotPosition(ptBot);
 	dwMapId = GetMapIdForBot();
 	PrintLogA("맵ID = %d, 위치 = (%d, %d)", dwMapId, ptBot.x, ptBot.y);
 
// 	CString strJob = GetBotJob();
// 	PrintLogA("캐릭직업 = %s", strJob);

	// 인벤슬럿의 아이템아뒤얻기
 	for (int tabIdx = 1; tabIdx <= 3; tabIdx ++)
 	{
		for (int slotIdx = 1; slotIdx <= 24; slotIdx ++)
 		{
			DWORD itemid = GetItemIdOfItemInventory(tabIdx, slotIdx);
 			if (itemid == 0) continue;
 			PrintLogA("인벤의 템아뒤조사: 탭[%d], 슬롯[%d], 아이디 = %d", tabIdx, slotIdx, itemid);
 		}
 	}

//	GetItemPosition();
//	GetMobPosition();

	// 스킬인벤	
//  	DWORD skillIdArr[10] = {0}, skillLevArr[10] = {0};
//  	GetSkillInventoryInfo(skillIdArr, skillLevArr);
//  	for (int i = 0; i < 10; i ++)
//  	{
//  		if (skillIdArr[i] == 0) continue;
//  		PrintLogA("스킬인벤의 스킬정보: [%d] ID = %d, LEV = %d", i + 1, skillIdArr[i], skillLevArr[i]);
//  	}

	// 키셋팅
	//KeySetting(10001244, 'H', 1);

	// 직업
// 	CString strJob = GetBotJob();
// 	PrintLogA("캐릭직업 = %s", strJob);

	// 텔레포탈정보얻기
	//POINT pt1, pt2;
	//ResetTeleportInfo(pt1, pt2);

	//g_bPacketCapture = FALSE;
	//RunFlag		= 0;
	//Platform	= 0;
	//g_bDupe		= TRUE;

	// 몹위치	
	//GetMobPosition();

	// 캐릭경험치
	//DWORD dwCurrentExp;
	//DWORD dwCurrentExp	= GetExpByLevel(50);
	//dwCurrentExp = GetBotExp();

	// 텍스쳐캡쳐
	//g_bTextureCapture = 1 - g_bTextureCapture;
	//PrintLogA("%s", g_bTextureCapture?"텍스쳐캡쳐시작":"텍스쳐캡쳐중지");

	// 인벤의 아이템개수얻기
	//int tabIdx = 1, slotIdx = 1;	
	//DWORD dwCnt = GetInvenItemCount(tabIdx, slotIdx);

	// 스킬레벨
	//DWORD skillId = 1111;
	//DWORD dwLev = GetSkillLevel(skillId);
	//PrintLogA("스킬[%d] = %d Lev", skillId, dwLev);	

  	//DWORD dwPoint = GetBotSP(3);
  	//PrintLogA("스킬포인터 = %d", dwPoint);

	// 장착템목록얻자
	//GetWearingEquipIdArray();

	// 버프스킬정보얻기
	//GetBuffSkillInfo();

	GetBotAP();

	return 0;
}

void GetBotObj(DWORD &botObj)
{
	botObj = 0;
	DWORD dwAddr = MEMADDR_BOTPOSITION;
	if (IsBadReadPtr((LPVOID)dwAddr, 4)) return;

	DWORD dwEax;
	dwAddr = *(LPDWORD) dwAddr;
	if (dwAddr == 0) return;

	if (IsBadReadPtr((LPVOID)(dwAddr + 0x4238), 4)) return;
	dwEax = *(LPDWORD) (dwAddr + 0x4238);
	DWORD dwEcx = dwEax - 0xC;
	if (dwEcx == 0) return;

	if (IsBadReadPtr((LPVOID)(dwEcx + 0xB4), 4)) return;
	botObj = *(LPDWORD) (dwEcx + 0xB4);
}

struct POSTDATA
{
	DWORD idx;
	DWORD hwnd;
	DWORD pid;	
};

void MyKeyMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	g_bUserKeySend = TRUE;
	g_KeyHwnd = (DWORD)hWnd;
	g_KeyMsg = (DWORD)Msg;
	g_KeyParam1 = (DWORD)wParam;
	g_KeyParam2 = (DWORD)lParam;
}

typedef struct _DRVBUF{
	DWORD	hThreadID;
	DWORD	Msg;
	DWORD	wParam;
	DWORD	lParam;
}DRVBUF, *PDRVBUF;

#define IOCTL_TEST1 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
void MyPostMessage(HWND hWnd, WORD Msg, WPARAM wParam, LPARAM lParam)
{
	if (g_hkbfDevice == NULL || g_GameThreadId == NULL) return;

	DRVBUF drvBuf;
	DWORD nb;

	drvBuf.hThreadID	= g_GameThreadId;
	drvBuf.Msg			= (DWORD)Msg;
	drvBuf.wParam		= (DWORD)wParam;
	drvBuf.lParam		= (DWORD)lParam;

	DeviceIoControl(g_hkbfDevice, IOCTL_TEST1, &drvBuf, sizeof(DRVBUF), NULL, 0, &nb, NULL);
}

void CommonKey_Press(UINT nkey)
{
	UINT uCode = nkey;
	if ( nkey >= 0x61 && nkey - 0x61 <= 25 ) uCode = nkey - 0x20;	

	DWORD dwBytesReturned;

// 	POSTMSG psgInfo;
// 	psgInfo.hwnd = (DWORD)g_GameHwnd;
// 	psgInfo.msg	= WM_KEYDOWN;
// 	psgInfo.wParam = nkey;	
// 	psgInfo.lParam = (MapVirtualKeyA(uCode, 0) << 16) + 1;	

	
	//DeviceIoControl(g_hMsDevice, IOCTL_POSTMSG_DATA, (LPVOID)&psgInfo, sizeof(POSTMSG), NULL, 0, &dwBytesReturned, NULL);
	MyPostMessage(g_GameHwnd, WM_KEYDOWN, nkey, (MapVirtualKeyA(uCode, 0) << 16) + 1); 
	//MyKeyMessage(g_GameHwnd, WM_KEYDOWN, nkey, (MapVirtualKeyA(uCode, 0) << 16) + 1); 
	//BOOL bRet = PostThreadMessage(g_GameThreadId, WM_KEYDOWN, nkey, (MapVirtualKeyA(uCode, 0) << 16) + 1);
	//PrintLogA("PostThreadMesasge = %d", bRet);
	Sleep(50);

//	psgInfo.lParam = (MapVirtualKeyA(uCode, 0) << 16) + 0xC0000001;
	//DeviceIoControl(g_hMsDevice, IOCTL_POSTMSG_DATA, (LPVOID)&psgInfo, sizeof(POSTMSG), NULL, 0, &dwBytesReturned, NULL);
	MyPostMessage(g_GameHwnd, WM_KEYUP, nkey, (MapVirtualKeyA(uCode, 0) << 16) + 0xC0000001);
	//MyKeyMessage(g_GameHwnd, WM_KEYUP, nkey, (MapVirtualKeyA(uCode, 0) << 16) + 0xC0000001);
	//PostThreadMessage(g_GameThreadId, WM_KEYUP, nkey, (MapVirtualKeyA(uCode, 0) << 16) + 0xC0000001);
}

int WINAPI Fn_2()
{
	PrintLogA("Fn_2 start");

// 	DWORD dwBotNameGenObj = 0x15D47DC;
// 	DWORD _ecx = *(DWORD*)(dwBotNameGenObj+0xA4);
// 	DWORD _esi = *(DWORD*)(_ecx+0x20);
// 	_ecx = _esi + 4;
// 	_ecx = *(DWORD*)(_ecx+0x98);
// 
// 	DWORD _edi = *(DWORD*)(_ecx+0x34);
// 	DWORD szlen = *(DWORD*)(_edi-0x4);
// 
// 	BYTE pBotName[13] = {0};
// 	memcpy(pBotName, (BYTE*)(_edi), szlen);
// 	PrintLogA("name = %s, %d", (char*)pBotName,szlen);

	

	DWORD dwBotNameObjAddr = 0x15D47DC;
	DWORD dwBotNameGenObj = *(DWORD*)dwBotNameObjAddr;
	DWORD dwEcx, dwEsi, dwEdi;
	BYTE pBotName[13] = {0};
	DWORD len = 7;

	strcpy((char*)pBotName, "저승똥a");


	//PrintLogA("dword_15D47DC = %x", *(DWORD*)dwBotNameGenObj);
	if (!IsBadReadPtr((LPVOID)(dwBotNameGenObj+0xA4), 4)) {
		dwEcx = *(DWORD*)(dwBotNameGenObj+0xA4);
		PrintLogA("ecx=%x", dwEcx);

		if (!IsBadReadPtr((LPVOID)(dwEcx+0x20), 4)) {
			dwEsi = *(DWORD*)(dwEcx+0x20);
			dwEcx = dwEsi + 4;

			if (!IsBadReadPtr((LPVOID)(dwEcx+0x98), 4)) {
				dwEcx = *(DWORD*)(dwEcx+0x98);
				if (!IsBadReadPtr((LPVOID)(dwEcx+0x34), 4)) {
					dwEdi = *(DWORD*)(dwEcx+0x34);
					if (!IsBadWritePtr((LPVOID)(dwEdi-0x4), 4)) {
						*(DWORD*)(dwEdi-0x4) = len;
							if (!IsBadWritePtr((LPVOID)dwEdi, 12)) {
								memset((BYTE*)dwEdi, 0, 12);
								memcpy((BYTE*)(dwEdi), pBotName, len);
								PrintLogA("캐릭이름쓰기성공");
							}

						//DWORD szlen = *(DWORD*)(dwEdi-0x4);
						//if (szlen>0 && szlen<=12) {
						//	PrintLogA("len = %d", szlen);
						//	if (!IsBadReadPtr((LPVOID)dwEdi, szlen)) {
						//		memcpy(pBotName, (BYTE*)(dwEdi), szlen);
						//		PrintLogA("캐릭이름 = %s, %d", (char*)pBotName, szlen);
						//	}
						//}

					}
				}
			}
		}
	}


	PrintLogA("Fn_2 end");
	return 0;
}

int WINAPI Fn_3()
{
	PrintLogA("Fn_3 Start");

	DWORD OldProtect;
	HMODULE hMod = GetModuleHandle("user32.dll");

	if (!hMod) hMod = LoadLibrary("user32.dll");
	if (hMod)
	{
		haiPostMessage.OrgProc = (FARPROC)GetProcedureAddress(hMod, "PostMessageA");
		DWORD dwPostMsg = (DWORD)haiPostMessage.OrgProc;
		if (haiPostMessage.OrgProc)
		{
			PrintLogA("PostMessageA 함수후킹 = %x", dwPostMsg);
			//ChangeAPICodes(&haiPostMessage, (FARPROC)MyPostMessageA);
			VirtualProtect((LPVOID)dwPostMsg, 0x5D, 0x40, &OldProtect);
			PrintBufToFile((PBYTE)dwPostMsg, 0x5D, "c:\\postmessage.raw");
			VirtualProtect((LPVOID)dwPostMsg, 0x5D, OldProtect, &OldProtect);
		}
	}

	PrintLogA("Fn_3 End");
	return 0;
}


int WINAPI Fn_4()
{
	PrintLogA("Fn_4 Start");
	DWORD dd = 0x12f5e4;
	DWORD ddd;
	for (int i = 0; i < 0x20;i += 4){
		ddd = dd+i;
		PrintBufToFile((BYTE*)ddd, 4, "c:\\MPSSend.txt");
	}
	PrintLogA("Fn_4 End");
	return 0;
}

void checkIPAddress()
{  
	WORD wVersionRequested;
	WSADATA wsaData;
	char  szName[255] = {0};
	CString strIP = ""; // 여기에 local ip가 저장.

	PHOSTENT hostinfo;
	wVersionRequested = MAKEWORD(2, 0);

	if (WSAStartup(wVersionRequested, &wsaData) == 0)
	{
		if (gethostname(szName, sizeof(szName)) == 0)
		{
			PrintLogA("host name = %s", szName);
			if ((hostinfo = gethostbyname(szName)) != NULL)
			{
				strIP = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
			}
		}     
		WSACleanup( );
	}
	PrintLogA("local ip = %s", strIP);
	return;
}

int WINAPI Fn_5()
{
	PrintLogA("Fn_5 start");
	//checkIPAddress();
	
	SendMessage(g_GameHwnd, WM_USER+0x101, 0, 0);
	//char szTitle[MAX_PATH] = "";
	//GetWindowTextA(g_GameHwnd, szTitle, MAX_PATH);
	//PrintLogA("창문이름 = %s",szTitle);

	PrintLogA("Fn_5 end");
	return 0;
}
int WINAPI Fn_6()
{
	PrintLogA("Fn_6 Start");

	DWORD dwProcessID = GetCurrentProcessId();
	DisplayModuleInProcess(dwProcessID);
	PrintLogA("Fn_6 End");
	return 0;
}

int WINAPI Fn_7()
{
	PrintLogA("Fn_7 Start");	
	PrintLogA("Fn_7 End");
	return 0;
}

int WINAPI Fn_8()
{
	PrintLogA("Fn_8 Start");
	PrintLogA("Fn_8 End");
	return 0;
}

int WINAPI Fn_9()
{
	PrintLogA("Fn_9 Start");
	PrintLogA("Fn_9 End");
	return 0;
}

void InstallHSPatch()
{
	PrintLogA("InstallHSPatch start");
	DWORD OldProtect, NewProtect;

	if (g_bMujok)
	{
		if (VirtualProtect((LPVOID)MEMADDR_HACKTIMENONENEMMY, 6, 0x40, &OldProtect))
		{
			*(BYTE*)(MEMADDR_HACKTIMENONENEMMY) = 0x90;
			*(BYTE*)(MEMADDR_HACKTIMENONENEMMY+1) = 0xE9;	
			dw_ORGJMP		= *(DWORD*)(MEMADDR_HACKTIMENONENEMMY+2);
			dw_NONENEMMYJMP = *(DWORD*)(MEMADDR_HACKTIMENONENEMMY+2);
			dw_NONENEMMYJMP += MEMADDR_HACKTIMENONENEMMY + 6;
			dw_NONENEMMYNEXT = MEMADDR_HACKTIMENONENEMMY + 6;
			*(DWORD*)(MEMADDR_HACKTIMENONENEMMY+2) = (DWORD)NONENEMMYPATCHFNC - MEMADDR_HACKTIMENONENEMMY - 5;
		}
	}
	
	InitForHackPatch1();
	InitForHackPatch2();
	InitForHackPatch3();
	InitForHackPatch4();
	InitForHackPatch5();

	PrintLogA("InstallHSPatch end");
}

void UnInstallHSPatch()
{
	PrintLogA("UnInstallHSPatch start");

	DWORD OldProtect, NewProtect;
	DWORD lpHackFunc2, lpHackFunc3, lpHackFunc4, lpHackFunc5;

	UnHookHackAddr();

	if (VirtualProtect((LPVOID)MEMADDR_HACKPATCHFUNC1, 6, 0x40, &OldProtect))
	{
		memcpy((LPBYTE)MEMADDR_HACKPATCHFUNC1, haiHackPatchFunc1.SaveBytes, 6);	
		VirtualProtect((LPVOID)MEMADDR_HACKPATCHFUNC1, 6, OldProtect, &NewProtect);
	}
	
	lpHackFunc2 = hfiHackPatchFunc2.OrgProc;
	if (VirtualProtect((LPVOID)lpHackFunc2, 8, 0x40, &OldProtect))
	{
		memcpy((LPBYTE)lpHackFunc2, hfiHackPatchFunc2.SaveBytes, 8);
		VirtualProtect((LPVOID)lpHackFunc2, 8, OldProtect, &NewProtect);
	}

	lpHackFunc3 = hfiHackPatchFunc3.OrgProc;
	if (VirtualProtect((LPVOID)lpHackFunc3, 8, 0x40, &OldProtect))
	{
		memcpy((LPBYTE)lpHackFunc3, hfiHackPatchFunc3.SaveBytes, 8);
		VirtualProtect((LPVOID)lpHackFunc3, 8, OldProtect, &NewProtect);
	}	

	lpHackFunc4 = hfiHackPatchFunc4.OrgProc;
	if (VirtualProtect((LPVOID)lpHackFunc4, 8, 0x40, &OldProtect))
	{
		memcpy((LPBYTE)lpHackFunc4, hfiHackPatchFunc4.SaveBytes, 8);
		VirtualProtect((LPVOID)lpHackFunc4, 8, OldProtect, &NewProtect);
	}
	lpHackFunc5 = hfiHackPatchFunc5.OrgProc;
	if (VirtualProtect((LPVOID)lpHackFunc5, 8, 0x40, &OldProtect))
	{
		memcpy((LPBYTE)lpHackFunc5, hfiHackPatchFunc5.SaveBytes, 8);
		VirtualProtect((LPVOID)lpHackFunc5, 8, OldProtect, &NewProtect);
	}

	PrintLogA("UnInstallHSPatch end");
}

int  index;
void WINAPI MyRaiseException( __in  DWORD dwExceptionCode,
							 __in  DWORD dwExceptionFlags,
							 __in  DWORD nNumberOfArguments,
							 __in  const ULONG_PTR *lpArguments)							 
{
	__asm
	{
		mov g_dwEsp, esp;
	}
	PrintLogA("MyRaiseException Code %x Flag %x NumOfArg %x lpArgu", dwExceptionCode, dwExceptionFlags, nNumberOfArguments);
	for (index = 0; index < 50; index++)
	{
		PrintLogA("index -%d Stack %x", index, *(PDWORD)(g_dwEsp+index*4));
	}
	RaiseException(dwExceptionCode, dwExceptionFlags, nNumberOfArguments, lpArguments);
}

//<////////////////////////////////////////////////////////////////////////

char* Myinet_ntoa(struct   in_addr in)
{
	PrintLogA("Myinet_ntoa Hooked!!");
	DWORD OldProtect;
	char *pszRtn = NULL;
	if (VirtualProtect(haiinet_ntoa.OrgProc, 6, PAGE_EXECUTE_READWRITE, &OldProtect) == 0) return 0;
	memcpy((LPBYTE)haiinet_ntoa.OrgProc, haiinet_ntoa.SaveBytes, 6);
	_inet_ntoa pfninet_ntoa = (_inet_ntoa)haiinet_ntoa.OrgProc;
	pszRtn = pfninet_ntoa(in);
	memcpy((LPBYTE)haiinet_ntoa.OrgProc, (LPBYTE)(&haiinet_ntoa.hookBytes), 6);
	VirtualProtect(haiinet_ntoa.OrgProc, 6, OldProtect, &OldProtect);
	PrintLogA("MyInet_ntoa = %s", pszRtn);
	return pszRtn;
};



int MySetSockOpt(SOCKET s, int level, int optname, char *optval, int optlen)
{
	DWORD	OldProtect;
	int		nRet;

	struct timeval *pTimeVal = (struct timeval*)optval;;	
	PrintLogA("setsockopt: socket = %x, level = %x, optname = %x, time = %x, optlen = %x", (DWORD)s, level, optname, pTimeVal->tv_sec, optlen);

	if (VirtualProtect(haisetsockopt.OrgProc, 6, PAGE_EXECUTE_READWRITE, &OldProtect) == 0) return 0;
	memcpy((LPBYTE)haisetsockopt.OrgProc, haisetsockopt.SaveBytes, 6);
	_setsockopt pfnsetsockopt = (_setsockopt)haisetsockopt.OrgProc;
	nRet = pfnsetsockopt(s, level, optname, optval, optlen);
	memcpy((LPBYTE)haisetsockopt.OrgProc, (LPBYTE)(&haisetsockopt.hookBytes), 6);
	VirtualProtect(haisetsockopt.OrgProc, 6, OldProtect, &OldProtect);

	return nRet;
}

BOOL g_bFF = TRUE;
DWORD g_dwForeEsp;

HWND MyGetForegroundWindow()
{
	HWND ret;
	DWORD OldProtect;	
	PrintLogA("MyGetforegroundWindow");

	//for (int i = 0; i < 100; i ++) PrintLogA("GetFore: [- %d], %x", i, *(DWORD*)(g_dwForeEsp+4*i));

	if (VirtualProtect(haiGetForegroundWindow.OrgProc, 6, 0x40, &OldProtect) == 0) return 0;
	memcpy((LPBYTE)haiGetForegroundWindow.OrgProc, haiGetForegroundWindow.SaveBytes, 6);
	_GetForegroundWindow pfnGetForegroundWindow = (_GetForegroundWindow)haiGetForegroundWindow.OrgProc;
	ret = pfnGetForegroundWindow();
	memcpy((LPBYTE)haiGetForegroundWindow.OrgProc, (LPBYTE)(&haiGetForegroundWindow.hookBytes), 6);
	VirtualProtect(haiGetForegroundWindow.OrgProc, 6, OldProtect, &OldProtect);	
	return ret;
}


int MyGetWindowTextA(HWND hWnd,LPCSTR lpString,int nMaxCount)
{
	int		ret = 0;
	DWORD	OldProtect;

	PrintLogA("MyGetWindowTextA hwnd = %x, maxcnt = %d", hWnd, nMaxCount);

	if (VirtualProtect(haiGetWindowTextA.OrgProc, 6, 0x40, &OldProtect) == 0) return 0;
	memcpy((LPBYTE)haiGetWindowTextA.OrgProc, haiGetWindowTextA.SaveBytes, 6);
	_GetWindowTextA pfnGetWindowTextA = (_GetWindowTextA)haiGetWindowTextA.OrgProc;
	ret = pfnGetWindowTextA(hWnd, lpString, nMaxCount);
	memcpy((LPBYTE)haiGetWindowTextA.OrgProc, (LPBYTE)(&haiGetWindowTextA.hookBytes), 6);
	VirtualProtect(haiGetWindowTextA.OrgProc, 6, OldProtect, &OldProtect);	

	PrintLogA("MyGetWindowTextA end: txt=%s, ret= %x", lpString,ret);	
	return ret;
}

BOOL MyGetMessage(LPMSG lpMsg,HWND hWnd,UINT wMsgFilterMin,UINT wMsgFilterMax)
{
	DWORD ret, OldProtect;
	PrintLogA("GetMessage");
	if (VirtualProtect(haiGetMessage.OrgProc, 6, 0x40, &OldProtect) == 0) 
	{
		PrintLogA("-----------------");
		return 0;
	}
	memcpy((LPBYTE)haiGetMessage.OrgProc, haiGetMessage.SaveBytes, 6);
	_GetMessage pfnGetMessage = (_GetMessage)haiGetMessage.OrgProc;
	ret = pfnGetMessage(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax);
	memcpy((LPBYTE)haiGetMessage.OrgProc, (LPBYTE)(&haiGetMessage.hookBytes), 6);
	VirtualProtect(haiGetMessage.OrgProc, 6, OldProtect, &OldProtect);

	PrintLogA("GetMessage: hwnd=%x, msg=%x, wParam=%x, ret = %x", lpMsg->hwnd, lpMsg->message, lpMsg->wParam, ret);

	return ret;
}
BOOL MyGetInputState(void)
{
	BOOL ret;
	DWORD OldProtect;	
	if (VirtualProtect(haiGetInputState.OrgProc, 6, 0x40, &OldProtect) == 0) return 0;
	memcpy((LPBYTE)haiGetInputState.OrgProc, haiGetInputState.SaveBytes, 6);
	_GetInputState pfnGetInputState = (_GetInputState)haiGetInputState.OrgProc;
	ret = pfnGetInputState();
	memcpy((LPBYTE)haiGetInputState.OrgProc, (LPBYTE)(&haiGetInputState.hookBytes), 6);
	VirtualProtect(haiGetInputState.OrgProc, 6, OldProtect, &OldProtect);
	return ret;
}

DWORD MyGetQueueStatus(UINT flags)
{
	DWORD ret, OldProtect;
	if (VirtualProtect(haiGetQueueStatus.OrgProc, 6, 0x40, &OldProtect) == 0) return 0;
	memcpy((LPBYTE)haiGetQueueStatus.OrgProc, haiGetQueueStatus.SaveBytes, 6);
	_GetQueueStatus pfnGetQueueStatus = (_GetQueueStatus)haiGetQueueStatus.OrgProc;
	ret = pfnGetQueueStatus(flags);
	memcpy((LPBYTE)haiGetQueueStatus.OrgProc, (LPBYTE)(&haiGetQueueStatus.hookBytes), 6);
	VirtualProtect(haiGetQueueStatus.OrgProc, 6, OldProtect, &OldProtect);

	PrintLogA("GetQueueStatus: flags = %x, ret = %x", flags, ret);

	return ret;
}


BOOL MyPostMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	BOOL	fRet;
	DWORD	dwOldProtect;
	DWORD	dwEsp;

	PrintLogA("MyPostMessageA: hwnd = %x, msg = %x, wParam = %x, lParam =%x", hWnd, Msg, wParam, lParam);

	__asm
	{
		mov dwEsp, esp;
	}

	//if (g_bFF)
	//if (Msg == 0x401 && g_bFF)
	//{
	//	g_bFF = FALSE;
	//	for (int i = 0; i < 200; i ++) PrintLogA("Post: [- %d], %x", i, *(DWORD*)(dwEsp+4*i));
	//}
		

	if (VirtualProtect(haiPostMessage.OrgProc, 6, PAGE_EXECUTE_READWRITE, &dwOldProtect) == 0) return FALSE;
	memcpy((LPBYTE)haiPostMessage.OrgProc, haiPostMessage.SaveBytes, 6);
	_PostMessageA pfnPostMessage = (_PostMessageA)haiPostMessage.OrgProc;
	fRet = pfnPostMessage(hWnd, Msg, wParam, lParam);
	memcpy((LPBYTE)haiPostMessage.OrgProc, (LPBYTE)(&haiPostMessage.hookBytes), 6);
	VirtualProtect(haiPostMessage.OrgProc, 6, dwOldProtect, &dwOldProtect);

	return fRet;
}

int MyCloseSocket(SOCKET s)
{
	BOOL	ret;
	DWORD	OldProtect;
	DWORD	dwEsp;

	__asm
	{
		mov dwEsp, esp;
	}
	for (int i = 0; i < 20; i ++)
	{
		PrintLogA("closesocket: [- %d], %x", i, *(DWORD*)(dwEsp+4*i));
	}

	PrintLogA("MyCloseSocket: 소켓 = %x", s);

	if (VirtualProtect(haiclosesocket.OrgProc, 6, PAGE_EXECUTE_READWRITE, &OldProtect) == 0) return SOCKET_ERROR;
	memcpy((LPBYTE)haiclosesocket.OrgProc, haiclosesocket.SaveBytes, 6);
	_closesocket pfnclosesocket = (_closesocket) haiclosesocket.OrgProc;
	ret = pfnclosesocket(s);
	memcpy((LPBYTE)haiclosesocket.OrgProc, (LPBYTE)&haiclosesocket.hookBytes, 6);
	VirtualProtect(haiclosesocket.OrgProc, 6, OldProtect, &OldProtect);

	return ret;
}

typedef HRESULT (*_DirectInput8Create)(HINSTANCE hinst, DWORD dwVersion,REFIID riidltf,LPVOID * ppvOut,LPUNKNOWN punkOuter);
HRESULT MyDirectInput8Create(HINSTANCE hinst, DWORD dwVersion,REFIID riidltf,LPVOID * ppvOut,LPUNKNOWN punkOuter)
{
	__asm
	{
		mov g_dwEsp, esp;
		mov g_dwEcx, ecx;
	}

	HRESULT ret;
	DWORD OldProtect;	
	if (VirtualProtect((LPVOID)haiDirectInput8Create.OrgProc, 6, 0x40, &OldProtect) == 0)
	{
		PrintLogA( "MyDirectInput8Create rtn(1)");
		return 0;
	}

	PrintLogA("MyDirectInput8Create: HINST=%x,VER=%x, ecx=%x", hinst, dwVersion, g_dwEcx);
	for (int i = 0; i < 10; i ++) PrintLogA( "esp+%d = %x", i, *(DWORD*)(g_dwEsp+i*4));

	memcpy((LPBYTE)haiDirectInput8Create.OrgProc, haiDirectInput8Create.SaveBytes, 6);
	_DirectInput8Create pfnDirectInput8Create = (_DirectInput8Create)haiDirectInput8Create.OrgProc;
	ret = pfnDirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);
	memcpy((LPBYTE)haiDirectInput8Create.OrgProc, (LPBYTE)(&haiDirectInput8Create.hookBytes), 6);

	VirtualProtect(haiDirectInput8Create.OrgProc, 6, OldProtect, &OldProtect);

	g_dwDIObj = (DWORD)ppvOut;
	AJLog(TRUE, "DirectInput 객체 = %x", g_dwDIObj);
// 	RaiseException(0, 0,0,0);

	return ret;
}



DWORD GetProcedureAddress(HMODULE hModule, LPSTR lpFuncName)
{
	if (hModule == NULL) return NULL;
	DWORD file_base = (DWORD)hModule;

	PIMAGE_DOS_HEADER pidh = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_NT_HEADERS pinh = (PIMAGE_NT_HEADERS)(file_base + pidh->e_lfanew);

	if (pidh->e_magic != IMAGE_DOS_SIGNATURE) return NULL;
	if (pinh->Signature != IMAGE_NT_SIGNATURE) return NULL;

	DWORD idei,idei_size;
	idei = pinh->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
	idei_size = pinh->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;

	PIMAGE_EXPORT_DIRECTORY pImageExportDirectory;
	pImageExportDirectory = (PIMAGE_EXPORT_DIRECTORY)(file_base + idei);

	DWORD AddressTable, NameTable, OrdinalTable, FuncNum;
	AddressTable = file_base + pImageExportDirectory->AddressOfFunctions;
	NameTable = file_base + pImageExportDirectory->AddressOfNames;
	OrdinalTable = file_base + pImageExportDirectory->AddressOfNameOrdinals;
	FuncNum = pImageExportDirectory->NumberOfFunctions;

	int i = 0;
	DWORD Offset, Number = 0xffffffff, Ordinal = 0xffffffff;
	for (i=0; i<FuncNum; i++)
	{
		Offset = *((LPDWORD)(NameTable + 4 * i));
		if (stricmp((PCHAR)(file_base + Offset), lpFuncName) == 0)
		{
			Number = i;
			break;
		}
	}
	if (Number == 0xffffffff) return NULL;

	Ordinal = (DWORD)(*((LPWORD)(OrdinalTable + 2 * Number)));

	Offset = *((LPDWORD)(AddressTable + 4 * Ordinal));

	return file_base + Offset;
}

//////////////////////////////////////////////////////////////////////////
LPDWORD	g_pIDirect3DSurface9;

int GetHashValue(LPBYTE lpBuffer, int nInterval, int nLength)
{
	int result; // eax@1
	LPBYTE lpEndPointer; // edi@1
	int Index; // esi@4
	BOOL v6; // zf@5
	int v7; // eax@5
	int nData; // ebx@7
	int v9; // ebx@8

	result = 0;
	lpEndPointer = lpBuffer + nLength - 4;
	if ( nInterval )
	{
		if ( lpBuffer < lpEndPointer )
		{
			for (Index = 0; lpBuffer < lpEndPointer; lpBuffer += nInterval, Index += nInterval)
			{
				v7 = (int)(0x26027A69 * result);
				v6 = (Index & 0x80000001) == 0;
				if (Index & 0x80000001 < 0)
					v6 = (((Index & 0x80000001) - 1) | 0xFFFFFFFE) == -1;

				nData = (int)(*lpBuffer);
				if ( v6 ) v9 = nData & 0xFFF0FFF;
				else v9 = nData & 0xFF0FFF0F;
				result = v9 ^ v7;
			}
		}
	}
	return result;
}

DWORD g_dwHashArr[1000];
DWORD g_dwHashImgCnt = 0;

LPBYTE m_ProcessBuffer42F58 = new BYTE[0x80BBC];
BOOL IdentifyTextureInfo(LPDWORD pVertexData, LPDWORD pDirect3DBaseTexture9)
{
	if (!pVertexData || !pDirect3DBaseTexture9) return FALSE;	

	BOOL			bRet = FALSE;
	DWORD			nWidth, nHeight;
	DWORD			nBufferSize;
	DWORD			nHashSize;
	int				nHashValue;

	LPDWORD			pSurfaceLevel = NULL;
	LPBYTE			pRectBuffer; 
	D3DSURFACE_DESC D3d9SurfaceDesc;
	D3DLOCKED_RECT	D3dLockedRect; 
	D3DMATRIX		D3dMatrix;
	int				nCount;
	

	//PrintLogA("IdnetifyTextureInfo start");

	_D3d9_Release_8 pfnRelease;
	_D3D9_GetLevelDesc pfnGetLevelDesc = (_D3D9_GetLevelDesc)(*(LPDWORD)(*pDirect3DBaseTexture9 + 0x44));	
	pfnGetLevelDesc(pDirect3DBaseTexture9, 0, &D3d9SurfaceDesc);

	nWidth		= D3d9SurfaceDesc.Width;
	nHeight		= D3d9SurfaceDesc.Height;
	nBufferSize = 2 * nWidth * nHeight;	

	_D3D9_GetSurfaceLevel pfnGetSurfaceLevel = (_D3D9_GetSurfaceLevel)(*(LPDWORD)(*pDirect3DBaseTexture9 + 0x48));	
	if (pfnGetSurfaceLevel(pDirect3DBaseTexture9, 0, &pSurfaceLevel) != 0 || pSurfaceLevel == NULL)
	{
		return 0;
	}

	_D3D9_LockRect pfnLockRect = (_D3D9_LockRect)(*(LPDWORD)(*pSurfaceLevel + 0x34));
	if (pfnLockRect(pSurfaceLevel, &D3dLockedRect, 0, D3DLOCK_NO_DIRTY_UPDATE | D3DLOCK_READONLY) != 0)
	{
		pfnRelease = (_D3d9_Release_8)(*(LPDWORD)(*pSurfaceLevel + 0x8));
		pfnRelease(pSurfaceLevel);
		return 0;
	}

	pRectBuffer = (LPBYTE)D3dLockedRect.pBits;

	int i;
	if (g_bTextureCapture/* && nWidth == 0x200 && nHeight == 0x80*/)
	{
		//LPBYTE pHashBuffer = m_ProcessBuffer42F58 + 0x80004;
		//LPBYTE pRowBuffer, pTemp;

		//pRowBuffer = pRectBuffer + 2*(nWidth * 22 + 80);
		//nCount = 0;
		//for (i = 6; i > 0; i --)
		//{
		//	pTemp = pRowBuffer;
		//	for (int j=90; j > 0; j --, nCount += 2, pTemp += 2)
		//	{
		//		pHashBuffer[nCount] = *pTemp;
		//		pHashBuffer[nCount + 1] = *(pTemp + 1);
		//	}
		//	pRowBuffer += (2 * nWidth);
		//}
		//nHashValue = GetHashValue(pHashBuffer, 1, 90 * 6 * 2);

		nHashSize = nBufferSize;
		nHashValue = GetHashValue(pRectBuffer, nHashSize/47, nHashSize);

		for (i = 0; i < g_dwHashImgCnt; i ++)
		{
			if (nHashValue == g_dwHashArr[i]) break;
		}
		if (i == g_dwHashImgCnt)
		{
			g_dwHashArr[g_dwHashImgCnt] = nHashValue;
			g_dwHashImgCnt ++;
			if (g_dwHashImgCnt == 1000) g_dwHashImgCnt = 999;

			SYSTEMTIME Time;
			GetLocalTime(&Time);
			char fileName[200]= {0};
			sprintf(fileName, "c:\\texture\\%d, %08x_%d%d%d%d%d_%d_%d_%d.raw", g_dwHashImgCnt, nHashValue, Time.wMonth, Time.wDay, Time.wHour, Time.wMinute, Time.wSecond, nWidth, nHeight, GetTickCount());
			FILE* fp;
			fp = fopen(fileName, "wb");
			if (fp)
			{
				fwrite(pRectBuffer, 1, 2*nWidth*nHeight, fp);
				fclose(fp);
			}
		}
	}

	_D3D9_UnlockRect pfnUnlockRect = (_D3D9_UnlockRect)(*(LPDWORD)(*pSurfaceLevel + 0x38));
	pfnUnlockRect(pSurfaceLevel);

	pfnRelease = (_D3d9_Release_8)(*(LPDWORD)(*pSurfaceLevel + 0x8));
	pfnRelease(pSurfaceLevel);

	return TRUE;
}


HRESULT MyDrawPrimitiveUP(LPDWORD arg_0, DWORD PrimitiveType, DWORD PrimitiveCount, LPDWORD pVertexStreamZeroData, DWORD VertexStreamZeroStride)
{
	LPDWORD				pTexture = NULL;
	D3DSURFACE_DESC		D3d9SurfaceDesc;
	DWORD				dwOldProtect;
	HRESULT				dwRet;

	{
		_D3d9_GetTexture pfnGetTexture = (_D3d9_GetTexture)(*(LPDWORD)(*arg_0 + 0x100));
		pfnGetTexture(arg_0, 0, &pTexture);

		if (pTexture)
		{
			if (pVertexStreamZeroData == NULL)
			{
				_D3d9_Release_8 pfnRelease = (_D3d9_Release_8)(*(LPDWORD)(*pTexture + 8));
				pfnRelease(pTexture);
				return 0;
			}
			else
			{
				_D3D9_GetLevelDesc pfnParam3Func = (_D3D9_GetLevelDesc)(*(LPDWORD)(*pTexture + 0x44));
				pfnParam3Func(pTexture, 0, &D3d9SurfaceDesc);

				if ( D3d9SurfaceDesc.Width == 0x400
					|| D3d9SurfaceDesc.Height == 0x400 || 
					D3d9SurfaceDesc.Width <= 0x20 || 
					D3d9SurfaceDesc.Height <= 0x20)
				{
					_D3d9_Release_8 pfnRelease = (_D3d9_Release_8)(*(LPDWORD)(*pTexture + 8));
					pfnRelease(pTexture);
				}
				else
				{
					// ---
					IdentifyTextureInfo(pVertexStreamZeroData, pTexture);
					// -- >


					_D3d9_Release_8 pfnRelease = (_D3d9_Release_8)(*(LPDWORD)(*pTexture + 8));
					pfnRelease(pTexture);
				}
			}
		}
	}	

	//if (!VirtualProtect(haiD3d9DrawPrimitiveUP.OrgProc, 6, PAGE_EXECUTE_READWRITE, &dwOldProtect))	return 0;
	memcpy((LPBYTE)haiD3d9DrawPrimitiveUP.OrgProc, haiD3d9DrawPrimitiveUP.SaveBytes, 6);
	_D3d9_DrawPrimitiveUP pfnDrawPrimitiveUP = (_D3d9_DrawPrimitiveUP)haiD3d9DrawPrimitiveUP.OrgProc;
	dwRet = pfnDrawPrimitiveUP(arg_0, PrimitiveType, PrimitiveCount, (DWORD)pVertexStreamZeroData, VertexStreamZeroStride);
	memcpy((LPBYTE)haiD3d9DrawPrimitiveUP.OrgProc, (LPBYTE)(&haiD3d9DrawPrimitiveUP.hookBytes), 6);
	//VirtualProtect(haiD3d9DrawPrimitiveUP.OrgProc, 6, dwOldProtect, &dwOldProtect);

	return dwRet;
}


DWORD MyCreateTexture(LPDWORD arg_0, DWORD arg_4, DWORD arg_8, DWORD arg_C, DWORD arg_10, DWORD arg_14, DWORD arg_18, DWORD arg_1C, DWORD arg_20)
{
	DWORD dwRet;
	DWORD dwOldProtect;

	if (g_pIDirect3DSurface9 == NULL)
	{
		g_pIDirect3DSurface9 = arg_0; 
		haiD3d9DrawPrimitiveUP.OrgProc =  (FARPROC)(*(LPDWORD)(*arg_0 + 0x14C));
		if (haiD3d9DrawPrimitiveUP.OrgProc)
		{
			ChangeAPICodes(&haiD3d9DrawPrimitiveUP, (FARPROC)MyDrawPrimitiveUP);
			VirtualProtect(haiD3d9DrawPrimitiveUP.OrgProc, 6, PAGE_EXECUTE_READWRITE, &dwOldProtect); // 121231
		}
	}

	memcpy((LPBYTE)haiD3d9CreateTexture.OrgProc, haiD3d9CreateTexture.SaveBytes, 6);
	_D3d9_CreateTexture pfnD3d9Func = (_D3d9_CreateTexture)haiD3d9CreateTexture.OrgProc;
	dwRet = pfnD3d9Func(arg_0, arg_4, arg_8, arg_C, arg_10, arg_14, arg_18, arg_1C, arg_20);
	memcpy((LPBYTE)haiD3d9CreateTexture.OrgProc, (LPBYTE)(&haiD3d9CreateTexture.hookBytes), 6);
	//VirtualProtect(haiD3d9CreateTexture.OrgProc, 6, dwOldProtect, &dwOldProtect);

	return dwRet;
}


//////////////////////////////////////////////////////////////////////////>


INLINE_HOOK_INFO g_HookInfo1;
DWORD	g_NakeKey_Call;
int __declspec(naked) WSAAPI MyNakeGetDeviceState()
{
	__asm
	{
		mov g_dwEbp, ebp;
		pushfd;
		pushad;
	}
	PrintLogA(" >> MyNakedGetDeiceState >> %x", g_dwEbp);
	__asm
	{
		popad;
		popfd;
		jmp [g_NakeKey_Call];
	}
}


void HookGetDeviceStateFunc()
{
	//DWORD pAddr = (DWORD)GetModuleHandle("dinput8.dll");
	//if ( pAddr == NULL )
	//{
	//	PrintLogA("dinput8.dll not found.");
	//	return;
	//}


	//BYTE * pTemp = memstr((PBYTE)pAddr, (PBYTE)"\xFF\x8B\xC7\x5F\x5E\x5B\x5D\xC2\x0C\x00", 0x10000, 10);
	//if ( pTemp == NULL )
	//{
	//	OutDbgStrA("HookGetDeviceStateFunc : pTemp is NULL.");
	//	return;
	//}

	//pTemp += 1;
	//OutDbgStrA("HookGetDeviceStateFunc : pTemp = %X", pTemp);

	DWORD pTemp;
	//pTemp = 0x6D2B815F; //GetDeviceState
	//pTemp = 0x77D0A817; // GetAsynKeyState
	//pTemp = 0x77D0AB3D; // GetKeyState;
	pTemp = 0x77D09823;	//NtUserGetForegroundWindow
	PrintLogA("HookGetDeviceStateFunc : pTemp = %X", pTemp);


	Hook_Api(pTemp, (DWORD)MyNakeGetDeviceState ,&g_HookInfo1, &g_NakeKey_Call); // GetDeviceState 후킹하기(0x6D2B815F)
	PrintLogA("HookGetDeviceStateFunc");
}


BOOL DisplayModuleInProcess(DWORD dwPID)
{
	HANDLE        hModuleSnap = NULL;
	MODULEENTRY32 me32        = {0};
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);

	if ( hModuleSnap == (HANDLE)-1 ) return NULL;

	me32.dwSize = sizeof(MODULEENTRY32);

	//해당 프로세스의 모듈리스트를 루프로 돌려서 프로세스이름과 동일하면
	//true를 리턴한다.
	if ( Module32First(hModuleSnap, &me32) )
	{
		do
		{
			PrintLogA("Module Name %s path = %s %x, size = %x", me32.szModule, me32.szExePath, me32.modBaseAddr, me32.modBaseSize);
		}
		while ( Module32Next(hModuleSnap, &me32) );
	}

	CloseHandle(hModuleSnap);
	return FALSE;
}


WNDPROC g_OldMsgProc;
long    g_PreWindowProc;


BOOL g_bRaiseException = FALSE;
LRESULT CALLBACK MsgProc(HWND hWnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
	if(!g_bRaiseException) 
	{
		RaiseException(0,0,0,0);
		g_bRaiseException = TRUE;
		PrintLogA("--RaiseException--");
	}

	if (msg == WM_USER+0x101)
	{
		PrintLogA("창문이름얻자");
		char szTitle[MAX_PATH] = "";
		GetWindowTextA(g_GameHwnd, szTitle, MAX_PATH);
		PrintLogA("창문이름 = %s",szTitle);
	}

	return CallWindowProc(g_OldMsgProc,hWnd,msg,wparam,lparam);
}


//////////////////////////////////////////////////////////////////////////

int WINAPI Fn_10(LPVOID pBRBuf)
{
	AJInitialize(0, 0);

	wcscpy(g_szLogFile, L"E:\\enginelog.txt");
	AJLog(TRUE, L"Fn_10 Start");

  	InstallVEH(); 	

	AJLog(TRUE, L"Fn_10 End");
	return 0;
}

int WINAPI Fn_11()
{
	AJLog(TRUE, L"Fn_11 Start");


	UnInstallHSPatch();

	
	AJLog(TRUE, L"Fn_11 End");
	return 0;
}

int WINAPI Fn_12()
{
	PrintLogA("Fn_12 start");

	PrintLogA("Fn_12 end");
	return 0;
}

int WINAPI Fn_13()
{	
	return 0;
}

BOOL GetMobPosition()
{
	PrintLogA("################## GetMobPosition ######################");
	DWORD dwMemAddr, ebp, eax, edi;
	DWORD dwBaseAddr;
	DWORD dwMobEdi, dwMobPropObj;

	
	dwMemAddr = MEMADDR_MOBPOSITION;
	if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) 
	{			
		return FALSE;
	}

	dwMemAddr = *(LPDWORD)dwMemAddr;
	if (dwMemAddr == 0) return FALSE;; // 120711 팅김

	dwMemAddr += MEMADDR_MOBPOSITION_OFFSET3;

	if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) 
	{			
		return FALSE;
	}
	ebp = *(LPDWORD)dwMemAddr;

	if (ebp == 0 || ebp == 0x10)
	{
		return FALSE;
	}

	dwMemAddr += 0x10;
	dwBaseAddr = dwMemAddr;

	tagPOINT point;
	do 
	{
		eax = dwBaseAddr;
		if (IsBadReadPtr((LPVOID)eax, 4))
		{				
			return FALSE;
		}
		dwMemAddr = ebp + 4;
		ebp -= 12;

		if (IsBadReadPtr((LPVOID)ebp, 4))
		{
			return FALSE;
		}

		ebp = *(LPDWORD)ebp;
		ebp += 0x10;

		if (IsBadReadPtr((LPVOID)dwMemAddr, 4))
		{
			return FALSE;
		}

		edi = *(LPDWORD)dwMemAddr;

		dwMobEdi = edi;

		edi += MEMADDR_MOBPOSITION_OFFSET1;

		if (IsBadReadPtr((LPVOID)(edi + 8), 4))	point.y = 0;
		else point.y = Descramble((LPDWORD)edi);

		edi += 12;
		if (IsBadReadPtr((LPVOID)(edi + 8), 4)) point.x = 0;
		else point.x  = Descramble((LPDWORD)edi);			


		eax = *(LPDWORD)dwMemAddr;
		eax += MEMADDR_MOBPOSITION_OFFSET2;

		if (IsBadReadPtr((LPVOID)eax, 4))
		{				
			return FALSE;
		}

		if (IsRectEmpty((LPRECT)eax)) continue;

		//pMobPosition->Add(point);

		// 0320 몹오브젝트주소 추가
		//pMobObjArr->Add(dwMobEdi); 
		
		DWORD dwMobPropObj = *(DWORD*)(dwMobEdi+HACK_MOB_STATUS_OFFSET1)-HACK_MOB_STATUS_OFFSET2;

		PrintLogA("몹위치: (%d, %d)",  point.x, point.y);

// 		if (dwMobPropObj 
// 			&& point.x >= 800 && point.x <= 1110 && point.y >= 1000 && point.y <= 1200)
// 		{			
// 			if (IsBadWritePtr((LPVOID)(dwMobPropObj+0x2F8), 4) == FALSE)
// 			{
// 				if (*(DWORD*)(dwMobPropObj+0x2F8) != 1)
// 				{
// 					PrintLogA("몹속성변경: 위치(%d, %d), 2F8 = %x", point.x, point.y, *(DWORD*)(dwMobPropObj+0x2F8));
// 					*(DWORD*)(dwMobPropObj+0x2F8) = 1;
// 					//*(DWORD*)(dwMobPropObj+0xF4) = 0;
// 				}				
// 			}
// 		}
		

	} while (ebp != 0x10);
	return FALSE;
}


int GetMobCountInArea(RECT rt)
{
	DWORD dwMemAddr, ebp, eax, edi;
	DWORD dwBaseAddr;
	DWORD dwMobEdi, dwMobPropObj;
	INT		nRet = 0;
	POINT point;
	dwMemAddr = MEMADDR_MOBPOSITION;
	if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) 
	{			
		return 0;
	}

	dwMemAddr = *(LPDWORD)dwMemAddr;
	if (dwMemAddr == 0) return FALSE;; // 120711 팅김

	dwMemAddr += MEMADDR_MOBPOSITION_OFFSET3;

	if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) 
	{			
		return 0;
	}
	ebp = *(LPDWORD)dwMemAddr;

	if (ebp == 0 || ebp == 0x10)
	{
		return 0;
	}

	dwMemAddr += 0x10;
	dwBaseAddr = dwMemAddr;

	do 
	{
		eax = dwBaseAddr;
		if (IsBadReadPtr((LPVOID)eax, 4))
		{				
			return 0;
		}
		dwMemAddr = ebp + 4;
		ebp -= 12;

		if (IsBadReadPtr((LPVOID)ebp, 4))
		{
			return 0;
		}

		ebp = *(LPDWORD)ebp;
		ebp += 0x10;

		if (IsBadReadPtr((LPVOID)dwMemAddr, 4))
		{
			return 0;
		}

		edi = *(LPDWORD)dwMemAddr;

		dwMobEdi = edi;

		edi += MEMADDR_MOBPOSITION_OFFSET1;

		if (IsBadReadPtr((LPVOID)(edi + 8), 4))	point.y = 0;
		else point.y = Descramble((LPDWORD)edi);

		edi += 12;
		if (IsBadReadPtr((LPVOID)(edi + 8), 4)) point.x = 0;
		else point.x  = Descramble((LPDWORD)edi);	
		if (PtInRect(&rt, point))
		{
			nRet++;
		}	
	} while (ebp != 0x10);
	return nRet;
}
// void SeBotPosition(DWORD X, DWORD Y)
// {
// 	DWORD dwAddr = MEMADDR_BOTPOSITION;
// 	DWORD ebx;
// 
// 	if (IsBadReadPtr((LPVOID)dwAddr, 4)) 
// 	{
// 		return;
// 	}
// 
// 	dwAddr = *(LPDWORD)dwAddr;
// 	if (dwAddr == 0) return; // 120711 팅김 
// 
// 	dwAddr = dwAddr + MEMADDR_BOTPOSITION_OFFSET;
// 	if ( IsBadReadPtr((LPVOID)(dwAddr + 8), 4))
// 	{
// 		ebx = 0;
// 	}
// 	else Encramle((LPDWORD)dwAddr, Y);
// 
// 
// 	dwAddr += 12;
// 	if ( IsBadReadPtr((LPVOID)(dwAddr + 8), 4) )
// 	{
// 
// 	}
// 	else
// 	{
// 		Encramle((LPDWORD)dwAddr, X);
// 	}
// }
DWORD GetBotObj()
{
	DWORD botObj = 0;
	DWORD dwAddr = MEMADDR_BOTPOSITION;
	__asm
	{
		push eax;
		push ecx;
		mov eax,dwAddr;   //캐배 
		mov eax, [eax];
		test eax,eax
		je NullPlat
		mov eax,[eax+4238h];
		lea ecx,[eax-0ch];
		test ecx,ecx
		je NullPlat;
		mov eax,[ecx+0b4h];
		test eax,eax
		je NullPlat;
		mov botObj, eax;
NullPlat:
		pop ecx;
		pop eax;
	}
	return botObj;
}

void GetMobPosFromMobProp(DWORD dwProp, POINT &pt)
{
	DWORD dwAddr = MEMADDR_BOTPOSITION;
	DWORD ebx;
	DWORD dwMemAddr, ebp, eax, edi;
	DWORD dwBaseAddr;
	DWORD dwMobEdi, dwMobPropObj;

	//POINT errpt = {-1000, -1000};
	pt.x = -1000;
	pt.y = -1000;

	dwMemAddr = MEMADDR_MOBPOSITION;
	if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) 
	{
		return;
	}

	dwMemAddr = *(LPDWORD)dwMemAddr;

	dwMemAddr += MEMADDR_MOBPOSITION_OFFSET3;

	if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) 
	{
		return ;
	}
	ebp = *(LPDWORD)dwMemAddr;

	if (ebp == 0 || ebp == 0x10)
	{
		return ;
	}

	dwMemAddr += 0x10;
	dwBaseAddr = dwMemAddr;

	tagPOINT point;
	do
	{
		eax = dwBaseAddr;
		if (IsBadReadPtr((LPVOID)eax, 4))
		{
			return ;
		}
		dwMemAddr = ebp + 4;
		ebp -= 12;

		if (IsBadReadPtr((LPVOID)ebp, 4))
		{
			return ;
		}

		ebp = *(LPDWORD)ebp;
		ebp += 0x10;

		if (IsBadReadPtr((LPVOID)dwMemAddr, 4))
		{				
			return ;
		}

		edi = *(LPDWORD)dwMemAddr;

		dwMobEdi = edi;

		DWORD dwMobPropObj = *(DWORD*)(dwMobEdi+HACK_MOB_STATUS_OFFSET1)-HACK_MOB_STATUS_OFFSET2;		
		if (dwMobPropObj == dwProp)
		{
			edi += MEMADDR_MOBPOSITION_OFFSET1;

			if (IsBadReadPtr((LPVOID)(edi + 8), 4))	point.y = 0;
			else point.y = Descramble((LPDWORD)edi);

			edi += 12;
			if (IsBadReadPtr((LPVOID)(edi + 8), 4)) point.x = 0;
			else point.x  = Descramble((LPDWORD)edi);	

			pt.x = point.x;
			pt.y = point.y;
			return;
		}

		// 0320 몹오브젝트주소 추가
	} while (ebp != 0x10);

	return ;
}

void ChangeMobStatus()
{
	PrintLogA("ChangeMobStatus start");
	DWORD dwAddr = MEMADDR_BOTPOSITION;
	DWORD ebx;

	DWORD dwMemAddr, ebp, eax, edi;
	DWORD dwBaseAddr;
	DWORD dwMobEdi, dwMobPropObj;

	dwMemAddr = MEMADDR_MOBPOSITION;
	if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) 
	{			
		return;
	}

	dwMemAddr = *(LPDWORD)dwMemAddr;

	dwMemAddr += MEMADDR_MOBPOSITION_OFFSET3;

	if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) 
	{			
		return;
	}
	ebp = *(LPDWORD)dwMemAddr;

	if (ebp == 0 || ebp == 0x10)
	{
		return;
	}

	dwMemAddr += 0x10;
	dwBaseAddr = dwMemAddr;

	tagPOINT point;
	do 
	{
		eax = dwBaseAddr;
		if (IsBadReadPtr((LPVOID)eax, 4))
		{				
			return;
		}
		dwMemAddr = ebp + 4;
		ebp -= 12;

		if (IsBadReadPtr((LPVOID)ebp, 4))
		{
			return;
		}

		ebp = *(LPDWORD)ebp;
		ebp += 0x10;

		if (IsBadReadPtr((LPVOID)dwMemAddr, 4))
		{				
			return;
		}

		edi = *(LPDWORD)dwMemAddr;

		dwMobEdi = edi;
		
		
		DWORD dwMobPropObj = *(DWORD*)(dwMobEdi+HACK_MOB_STATUS_OFFSET1)-HACK_MOB_STATUS_OFFSET2;		
		//DWORD dwChange = GetBotObj();
		PrintLogA("MobProp = %x, %x", dwMobEdi, dwMobPropObj);
		//if (dwChange)
		//{
		//	//*(PDWORD)(dwMobPropObj+0x2a8) = 6;
		//	*(PDWORD)(dwMobPropObj+0xb4) = dwChange;
		//}
	
		// 0320 몹오브젝트주소 추가
	} while (ebp != 0x10);

}



DWORD GetCurrentHP()
{
	DWORD dwMemAddr;
	DWORD dwReturn = 0;
	DWORD edx, eax, ecx;

	dwMemAddr = MEMADDR_BOTINFO;
	if (IsBadReadPtr((LPVOID)dwMemAddr, 4) == FALSE)
	{
		dwMemAddr = *(LPDWORD)dwMemAddr;
		dwMemAddr += MEMADDR_BOTCURRENTHP_OFFSET;
		if (IsBadReadPtr((LPVOID)dwMemAddr, 12)) eax = 0;
		else
		{
			edx = *(LPDWORD)(dwMemAddr + 8);
			eax = *(LPDWORD)(dwMemAddr + 4);
			ecx = *(LPDWORD)dwMemAddr;
			dwReturn = RolXor(ecx, eax, edx);
		}
	}

	return dwReturn;
}

DWORD GetEmployeeShopID(CString strShopName)
{
	//DebugMessageA(17, "GetEmployeeInfo start");
	DWORD dwAddr = MEMADDR_EMPLOYEEINFO;
	DWORD var_18, edi, ebx, ecx = 0;
	POINT ptPosition;

	if (IsBadReadPtr((void*)dwAddr, 4) == TRUE) 
	{
		//DebugMessageA(17, "GetEmployeeInfo end(1)");
		return ecx;
	}

	dwAddr = *(LPDWORD) dwAddr;

	dwAddr += MEMADDR_EMPLOYEEINFO_OFFSET2;
	if (IsBadReadPtr((void*)dwAddr, 4) == TRUE) 
	{
		//DebugMessageA(17, "GetEmployeeInfo end(2)");
		return ecx;
	}
	dwAddr = *(LPDWORD)dwAddr;
	if (dwAddr == 0) 
	{
		//DebugMessageA(17, "GetEmployeeInfo end(3)");
		return ecx;
	}

	var_18 = dwAddr;
	do
	{
		dwAddr += 0x10;
		if (IsBadReadPtr((void*)dwAddr, 4) == TRUE) 
		{
			//DebugMessageA(17, "GetEmployeeInfo break(1)");
			break;
		}
		edi = *(LPDWORD)dwAddr;
		ebx = edi + MEMADDR_EMPLOYEEINFO_OFFSET3;
		dwAddr = edi + MEMADDR_EMPLOYEEINFO_OFFSET4;
		if (IsBadReadPtr((void*)dwAddr, 4) == TRUE) 
		{
			//DebugMessageA(17, "GetEmployeeInfo break(2)");
			break;
		}
		if (IsBadReadPtr((void*)ebx, 4) == TRUE) 
		{
			//DebugMessageA(17, "GetEmployeeInfo break(3)");
			break;
		}
		ecx = *(LPDWORD)ebx;
		dwAddr = *(LPDWORD)dwAddr;
		if (stricmp((char*)dwAddr, strShopName) == 0)
		{
			return ecx;
		}

		edi = var_18;
		edi += 4;
		if (IsBadReadPtr((void*)edi, 4) == TRUE) 
		{
			//DebugMessageA(17, "GetEmployeeInfo break(6)");
			break;
		}
		edi = *(LPDWORD)edi;
		var_18 = edi;
		dwAddr = edi;
	} while (edi != 0);		

	return 0;
	//DebugMessageA(17, "GetEmployeeInfo end");
}

DWORD GetOtherCharacterExChangeID(char* strShopName)
{
	DWORD dwMemAddr, dwTemp, ecx = 0;
	DWORD dwPtrAddr;
	POINT pt;


	dwMemAddr = MEMADDR_OTHERCHARICINFO;
	if (IsBadReadPtr((LPVOID)dwMemAddr, 4) == TRUE)
	{

		return 0;
	}
	dwMemAddr = *(LPDWORD)dwMemAddr;
	dwMemAddr = dwMemAddr + MEMADDR_OTHERCHARICINFO_OFFSET1;
	if (IsBadReadPtr((LPVOID)dwMemAddr, 4) ==  FALSE)
	{
		dwMemAddr = *(LPDWORD)dwMemAddr;
		dwPtrAddr = dwMemAddr;

		while (dwPtrAddr)
		{
			dwMemAddr = dwPtrAddr;

			dwMemAddr = dwMemAddr + 4;
			if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) { break; }
			dwMemAddr = *(LPDWORD)dwMemAddr;

			dwMemAddr = dwMemAddr + 16;
			if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) { break; }
			dwTemp = *(LPDWORD)dwMemAddr;

			dwMemAddr = dwTemp + MEMADDR_OTHERCHARICINFO_OFFSET2 - 4;
			if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) { break; }

			ecx = *(PDWORD)dwMemAddr;

			dwMemAddr = dwTemp + MEMADDR_OTHERCHARICINFO_OFFSET2;
			if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) { break; }
			dwMemAddr = *(LPDWORD)(dwMemAddr);

			if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) { break; }

// 			dwMemAddr = dwTemp + 0x41f4;
// 			if (!IsBadReadPtr((LPVOID)dwMemAddr, 4))
// 			{
// 				PrintLogA("Find -- %x", *(PDWORD)dwMemAddr);
// 				ecx = *(PDWORD)dwMemAddr;
// 			}

			if (stricmp((char*)dwMemAddr, strShopName) == 0)
			{
				dwMemAddr = dwTemp + MEMADDR_EXCHANGECHARINFO;
				if (!IsBadReadPtr((LPVOID)dwMemAddr, 4))
				{				 
					ecx = *(PDWORD)dwMemAddr;
					return ecx;
				}
			}

			
			
			if (IsBadReadPtr((LPVOID)dwPtrAddr, 4)) { break; }

			if (IsBadReadPtr((LPVOID)(dwPtrAddr - 12), 4)) { break; }
			dwMemAddr = *(LPDWORD)(dwPtrAddr - 12);
			if (dwMemAddr == 0) break;

			dwPtrAddr = dwMemAddr + 0x10;
		}
	}		
	return 0;
 }
// void SendExChangeRequestPacket()
// {
// 	PrintLogA("SendExChangeRequestPacket");
// 	char szCharName[MAX_PATH];
// 	strcpy(szCharName, "장삼42");
// 	DWORD dwEdi = GetOtherCharacterExChangeID(szCharName);
// // 	
// 	BYTE var_2C[0x2C];
// 	DWORD var_4;
// 	DWORD sub_71E650 = ADDR_SEND_OPCODE, sub_415860 = ADDR_SEND_BYTE, sub_4158B0 = ADDR_SEND_DWORD, sub_4D0C00 = ADDR_SEND_FUNC,
// 		sub_71E5B0 = ADDR_SEND_EXCHANGE;
// 
// 	PrintLogA("ExChange Character %x", dwEdi);
// //	return;
// 	if (dwEdi)
// 	{
// 		PrintLogA("Find Character");
// 		__asm
// 		{
// 			pushad;
// 			push    0BDh;
// 			lea     ecx, var_2C;
// 			call    sub_71E650;
// 			push    6;
// 			lea     ecx, var_2C;
// 			mov     var_4, 4;
// 			call    sub_415860;				
// 			push    3;
// 			lea     ecx, var_2C;
// 			call    sub_415860;
// 			push    0;
// 			lea     ecx, var_2C;
// 			call    sub_415860;
// 			lea     ecx, var_2C;
// 			push    ecx;
// 			mov     ecx, ADDR_SOCKET_OBJ;
// 			mov		ecx, [ecx];
// 			call    sub_4D0C00;
// 
// 			push    0BDh
// 			lea     ecx, var_2C;
// 			call    sub_71E5B0
// 			push    0Bh
// 			lea     ecx, var_2C;
// 			call    sub_415860
// 			mov     esi, dwEdi;
// 			push    esi
// 			lea     ecx, var_2C;
// 			call    sub_4158B0
// 			lea     ecx, var_2C;
// 			push    ecx;
// 			mov     ecx, ADDR_SOCKET_OBJ;
// 			mov		ecx, [ecx];
// 			call    sub_4D0C00;
// 			popad;
// 		}
// 	}
// }
// 
// 
// 
// void SendExChangeForgivePacket(DWORD dwID)
// {
// 	BYTE var_2C[0x2C];
// 	DWORD var_4;
// 	DWORD sub_71E650 = ADDR_SEND_OPCODE, sub_415860 = ADDR_SEND_BYTE, sub_4158B0 = ADDR_SEND_DWORD, sub_4D0C00 = ADDR_SEND_FUNC;
// 	//PrintLogA("SendExChangeMesoPacket");
// 	__asm
// 	{
// 		pushad;
// 		push    PACKET_OPCODE;		
// 		lea     ecx, var_2C;
// 		call    sub_71E650;
// 		push    0Ch;
// 		lea     ecx, var_2C;
// 		call    sub_415860;
// 		mov     eax, dwID;
// 		push    eax;
// 		lea     ecx, var_2C;
// 		call    sub_4158B0;
// 		push    3;
// 		lea     ecx, var_2C;
// 		call    sub_415860;		
// 		lea     ecx, var_2C;
// 		push    ecx;
// 		mov     ecx, ADDR_SOCKET_OBJ;
// 		mov		ecx, [ecx];
// 		call    sub_4D0C00;
// 		popad;
// 	}
// }


DWORD GetItemIdOfItemInventory(DWORD nItemTabIndex, DWORD nSlotIndex)
{
	DWORD dwMemAddr = MEMADDR_BOTINFO;
	DWORD dwItemID = 0;

	if (nItemTabIndex == 3) nItemTabIndex = 4;
	else if (nItemTabIndex == 4) nItemTabIndex = 3;

	if (IsBadReadPtr((LPVOID)dwMemAddr, 4) == TRUE) 
	{			
		return 0;
	}
	dwMemAddr = *(LPDWORD)dwMemAddr;
	dwMemAddr = dwMemAddr + 4 * nItemTabIndex + MEMADDR_BOTITEMINVENTORY_OFFSET;

	if ( IsBadReadPtr((LPVOID)dwMemAddr, 4) == FALSE)
	{
		dwMemAddr = *(LPDWORD)dwMemAddr;
		dwMemAddr = dwMemAddr + 8 * nSlotIndex;
		if ( dwMemAddr )
		{
			dwMemAddr = dwMemAddr + 4;
			if (IsBadReadPtr((LPVOID)dwMemAddr, 4) == FALSE)
			{
				dwMemAddr = *(LPDWORD)dwMemAddr;
				DWORD dwTemp = dwMemAddr;
				DWORD dwCount;
				if ( dwMemAddr )
				{
					dwMemAddr += 12;
					if (IsBadReadPtr((LPVOID)(dwMemAddr + 8), 4) == FALSE)
					{
						dwItemID = Descramble((LPDWORD)dwMemAddr);
					}
					//PrintLogA("아이템아이디 %d", dwItemID);
					//for(int i = 0; i < 0x30; i++)
					//{
					//	PrintLogA("아이템개수- %d %d", i, *(PBYTE)(dwTemp+i));					
					//}					
				}
			}
		}
		
	}		
	return dwItemID;
}
DWORD GetOtherCharacterShopID(CString strShopName, DWORD &dwTmp)
{
	DWORD dwMemAddr, dwTemp, ecx = 0;
	DWORD dwPtrAddr;
	POINT pt;

	dwMemAddr = MEMADDR_OTHERCHARICINFO;
	if (IsBadReadPtr((LPVOID)dwMemAddr, 4) == TRUE)
	{
		return 0;
	}
	dwMemAddr = *(LPDWORD)dwMemAddr;
	dwMemAddr = dwMemAddr + MEMADDR_OTHERCHARICINFO_OFFSET1;
	if (IsBadReadPtr((LPVOID)dwMemAddr, 4) ==  FALSE)
	{
		dwMemAddr = *(LPDWORD)dwMemAddr;
		dwPtrAddr = dwMemAddr;

		while (dwPtrAddr)
		{
			dwMemAddr = dwPtrAddr;

			dwMemAddr = dwMemAddr + 4;
			if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) { break; }
			dwMemAddr = *(LPDWORD)dwMemAddr;

			dwMemAddr = dwMemAddr + 16;
			if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) { break; }
			dwTemp = *(LPDWORD)dwMemAddr;

			dwMemAddr = dwTemp + MEMADDR_OTHERCHARICINFO_OFFSET2 - 4;
			if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) { break; }

			ecx = *(PDWORD)dwMemAddr;

			dwMemAddr = dwTemp + MEMADDR_OTHERCHARICINFO_OFFSET2;
			if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) { break; }
			dwMemAddr = *(LPDWORD)(dwMemAddr);

			if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) { break; }

			//DebugMessageA(1, "GetOtherCharacterShopID = %s", (char*)dwMemAddr);
			if (stricmp((char*)dwMemAddr, strShopName.GetBuffer(0)) == 0)
			{
				dwMemAddr = dwTemp + MEMADDR_SHOPCHARINFO;

				if (*(PDWORD)(dwTemp+MEMADDR_SHOPCHARINFO2) == 0)
				{
					dwTmp = 0;
				}
				else dwTmp = 1;

				if (!IsBadReadPtr((LPVOID)dwMemAddr, 4))
				{				 
					ecx = *(PDWORD)dwMemAddr;
					return ecx;
				}
				return 0;
			}

			if (IsBadReadPtr((LPVOID)dwPtrAddr, 4)) { break; }

			if (IsBadReadPtr((LPVOID)(dwPtrAddr - 12), 4)) { break; }
			dwMemAddr = *(LPDWORD)(dwPtrAddr - 12);
			if (dwMemAddr == 0) break;

			dwPtrAddr = dwMemAddr + 0x10;
		}
	}
	return 0;
}



BOOL  IsExchangableItem(DWORD nItemTabIndex, DWORD nSlotIndex)
{
	DWORD dwMemAddr = MEMADDR_BOTINFO;
	BOOL bItemNoExchProperty = FALSE;


	if (nItemTabIndex == 3) nItemTabIndex = 4;
	else if (nItemTabIndex == 4) nItemTabIndex = 3;

	if (IsBadReadPtr((LPVOID)dwMemAddr, 4) == TRUE) 
	{			
		return FALSE;
	}
	dwMemAddr = *(LPDWORD)dwMemAddr;
	dwMemAddr = dwMemAddr + 4 * nItemTabIndex + MEMADDR_BOTITEMINVENTORY_OFFSET;

	if ( IsBadReadPtr((LPVOID)dwMemAddr, 4) == FALSE)
	{
		dwMemAddr = *(LPDWORD)dwMemAddr;
		dwMemAddr = dwMemAddr + 8 * nSlotIndex;
		if ( dwMemAddr )
		{
			dwMemAddr = dwMemAddr + 4;
			if (IsBadReadPtr((LPVOID)dwMemAddr, 4) == FALSE)
			{
				dwMemAddr = *(LPDWORD)dwMemAddr;
				if ( dwMemAddr )
				{
					DWORD sub_5FAB20 = ADDR_EXCHANGE_ITEMPROPERTY;
					__asm
					{
						pushad;
						mov		ecx, ADDR_EXCHANGE_ITEMOBJ;
						mov		ecx, [ecx];
						mov		esi, dwMemAddr;
						push	esi;
						call	sub_5FAB20;
						mov		bItemNoExchProperty, eax;							
						popad;
					}

					bItemNoExchProperty = !bItemNoExchProperty;
				}
			}
		}
	}		
	return bItemNoExchProperty;
}


DWORD GetDecryptValue(DWORD arg_0, DWORD arg_4, DWORD arg_8)
{
	DWORD dwVal = 0;
	__asm
	{
		pushad;
		mov		edx, arg_4;
		mov		eax, arg_0;
		mov		esi, arg_8;
		mov     ecx, edx
		rol     ecx, 5
		xor     ecx, eax
		xor     eax, 0BAADF00Dh
		ror     eax, 5
		add     eax, edx
		mov     dwVal, eax;
		popad;
	}
	if (dwVal == arg_8) return dwVal;
	else return 0;
}

// 
// void OpenPersernalShop(DWORD dwEdi)
// {
// 	BYTE  var_40[100];
// 	DWORD var_4;
// 
// 	DWORD sub_71E650 = ADDR_SEND_OPCODE, sub_415860 = ADDR_SEND_BYTE, sub_4158B0 = ADDR_SEND_DWORD, sub_4D0C00 = ADDR_SEND_FUNC;
// 	if (dwEdi)
// 	{
// 		__asm
// 		{
// 			pushad;
// 			push	PACKET_OPCODE;
// 			lea     ecx, var_40;
// 			call    sub_71E650;
// 			push    9;
// 			lea     ecx, var_40;
// 			mov     var_4, 6;
// 			call    sub_415860;
// 			mov     edi, dwEdi;
// 			push    edi;
// 			lea     ecx, var_40;
// 			call    sub_4158B0;
// 			push    0;
// 			lea     ecx, var_40;
// 			call    sub_415860;
// 			push    0;
// 			lea     ecx, var_40;
// 			call    sub_415860;
// 			lea     ecx, var_40;
// 			push    ecx;
// 			mov     ecx, ADDR_SOCKET_OBJ;
// 			mov		ecx, [ecx];
// 			call    sub_4D0C00;
// 			popad;
// 		}
// 	}
// }
// void Encramle(PDWORD pAddr, DWORD val)
// {
// 	DWORD sub_433500 = 0x433500;
// 	__asm
// 	{
// 		pushad;
// 		mov eax, val;
// 		push    eax;
// 		mov     ecx, pAddr;
// 		call    sub_433500;
// 		popad;
// 	}
// }

int GetItemPosition()
{

	PrintLogA("======== 아이템목록얻자 ===========");

	DWORD dwMemAddr, dwTemp, edi, eax;
	int nNum = 0;
	dwMemAddr = MEMADDR_ITEMPOSITION;		
	DWORD dwItemID, dwItemBotId;

	BOOL bNoPickItem = FALSE;

	if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) 
	{
		return 0;
	}

	dwMemAddr = *(LPDWORD)dwMemAddr;
	if (dwMemAddr == 0) return 0; // 120711 팅김

	dwMemAddr += MEMADDR_ITEMPOSITION_OFFSET1;

	if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) 
	{
		return 0;
	}

	dwTemp = *(LPDWORD)dwMemAddr;
	PrintLogA("dwTemp = %x", dwTemp);
	if (dwTemp == 0 || dwTemp == 0x10) 
	{
		return 0;
	}

	tagPOINT point;
	do 
	{
		dwMemAddr = dwTemp + 4;
		if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) 
		{
			return 0;
		}

		dwTemp -= 8;
		if (IsBadReadPtr((LPVOID)dwTemp, 4)) 
		{
			return 0;
		}

		edi = *(LPDWORD)dwMemAddr;
		edi += MEMADDR_ITEMPOSITION_POS_OFFSET;
		dwTemp = *(LPDWORD)dwTemp;
		dwTemp += 0x10;

		if (IsBadReadPtr((LPVOID)(edi + 8), 4) == TRUE) point.y = 0;
		else point.y = Descramble((LPDWORD)edi);

		edi += 12;
		if (IsBadReadPtr((LPVOID)(edi + 8), 4)) point.x = 0;
		else point.x = Descramble((LPDWORD)edi);

		eax = *(LPDWORD)dwMemAddr;

		if (IsBadReadPtr((LPVOID)(eax + MEMADDR_ITEMPOSITION_ID_OFFSET), 4)) // steels 111220
		{
			return 0;
		}

		dwItemID = *(LPDWORD)(eax + MEMADDR_ITEMPOSITION_ID_OFFSET);
		if (dwItemID == 0) continue;

		//DebugMessageA(1, "ITEMID: %d, POS: (%d, %d)", dwItemID, point.x, point.y);

		// 120814 타캐릭이 떨군 아이템은 포함하지 않기
		dwItemBotId = *(LPDWORD)(eax+MEMADDR_ITEMPOSITION_BOTID_OFFSET);
		//DebugMessageA(1, "아이템떨군 캐릭ID = %x", dwItemBotId);

		PrintLogA("템아이디 = %d, 템위치 = %d, %d, dwTmp = %x", dwItemID, point.x, point.y, dwTemp);
		nNum ++;	

	} while (dwTemp != 0x10);

	PrintLogA("아이템개수 = %d", nNum);
	return nNum;
}


DWORD GetSpeedItemObj(DWORD dwItemObj)
{
	DWORD dwMemAddr, dwTemp, edi, eax;
	dwMemAddr = MEMADDR_ITEMPOSITION;
	DWORD dwItem;
	DWORD dwPickItem = 0;
	

	if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) 
	{
		return dwPickItem;
	}

	dwMemAddr = *(LPDWORD)dwMemAddr;
	if (dwMemAddr == 0) return 0; // 120711 팅김

	dwMemAddr += MEMADDR_ITEMPOSITION_OFFSET1;

	if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) 
	{
		return dwPickItem;
	}

	dwTemp = *(LPDWORD)dwMemAddr;
	if (dwTemp == 0 || dwTemp == 0x10) 
	{
		return 0;
	}

	tagPOINT point;
	do 
	{
		dwMemAddr = dwTemp + 4;
		if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) 
		{
			return dwPickItem;
		}

		dwTemp -= 12;
		if (IsBadReadPtr((LPVOID)dwTemp, 4)) 
		{
			return dwPickItem;
		}

		edi = *(LPDWORD)dwMemAddr;
		dwItem = edi;
		PrintLogA("Item %x", edi);
		edi += MEMADDR_ITEMPOSITION_POS_OFFSET;
		dwTemp = *(LPDWORD)dwTemp;
		dwTemp += 0x10;

		if (IsBadReadPtr((LPVOID)(edi + 8), 4) == TRUE) point.y = 0;
		else point.y = Descramble((LPDWORD)edi);
		//Encramle((PDWORD)edi, botpt.y);
		edi += 12;
		if (IsBadReadPtr((LPVOID)(edi + 8), 4)) point.x = 0;
		else point.x = Descramble((LPDWORD)edi);
		//Encramle((PDWORD)edi, botpt.x);
		eax = *(LPDWORD)dwMemAddr;

		if (dwItem != dwItemObj)
		{
			dwPickItem = dwItem;
			break;
		}

		if (IsBadReadPtr((LPVOID)(eax + MEMADDR_ITEMPOSITION_ID_OFFSET), 4)) // steels 111220
		{
			return dwPickItem;
		}		
	} while (dwTemp != 0x10);
	return dwPickItem;
}

BOOL ProcessRecvPacket( PBYTE pBuf, int nlen )
{
	if ( nlen == 0 ) return FALSE;
	if (!pBuf) return FALSE;

	//PrintBufToFile(pBuf, nlen, "c:\\Log\\MPSRecv.txt");

	WORD opcode1 = *((PWORD)pBuf);
	if ( opcode1 != 0x1F7 ) return FALSE;

	BYTE opcode = *((PBYTE)(pBuf+2));

//	if (g_dwAutoManualModeFlag == MANUAL_MODE) return FALSE;

	switch ( opcode )
	{
	case 0xb: // 교환신청패킷
		{
			//pepsi_120623
			// 			if (g_dwAutoManualModeFlag == MANUAL_MODE)
			// 				g_dwAutoManualModeFlag = AUTO_MODE ; 
			//////////////////////////////////////////////////////////////////////////
			
			//////////////////////////////////////////////////////////////////////////
			char szName[MAX_PATH] = {0, };
			WORD slen = *((PWORD) (pBuf+4));
			memcpy(szName, (PBYTE)(pBuf+6), slen);
			g_dwBotID = *((PDWORD)(pBuf+6+slen));
			PrintLogA("Bot ID %x", g_dwBotID);
			//SendExChangeAcceptPacket(g_dwBotID);
			//SendExChangeForgivePacket(g_dwBotID);
		}
		break;
	}

	return 0;
 }
// 
// void SendExChangeAcceptPacket(DWORD dwID)
// {
// 	BYTE var_2C[0x2C];
// 	DWORD var_4;
// 	DWORD sub_71E650 = ADDR_SEND_OPCODE, sub_415860 = ADDR_SEND_BYTE, sub_4158B0 = ADDR_SEND_DWORD, sub_4D0C00 = ADDR_SEND_FUNC;
// 	//PrintLogA("SendExChangeMesoPacket");
// 	__asm
// 	{
// 		pushad;
// 		push    PACKET_OPCODE;		
// 		lea     ecx, var_2C;
// 		call    sub_71E650;
// 		push    9;
// 		lea     ecx, var_2C;
// 		call    sub_415860;
// 		mov     eax, dwID;
// 		push    eax;
// 		lea     ecx, var_2C;
// 		call    sub_4158B0;
// 		push    0;
// 		lea     ecx, var_2C;
// 		call    sub_415860;
// 		push    0;
// 		lea     ecx, var_2C;
// 		call    sub_415860;
// 		lea     ecx, var_2C;
// 		push    ecx;
// 		mov     ecx, ADDR_SOCKET_OBJ;
// 		mov		ecx, [ecx];
// 		call    sub_4D0C00;
// 		popad;
// 	}
// }

// void GetTeleportInfo()
// {
// 	DWORD dwTeleport = *(PDWORD)ADDR_TELEPORT_INFO;
// 	//PTELEPORT_NODE_INFO pTeleport = new PTELEPORT_NODE_INFO;
// 	//memset((void*)pTeleport, 0, sizeof(TELEPORT_NODE_INFO));
// 	PrintLogA("dwTeleport = %x", dwTeleport);
// 	if (dwTeleport == 0) return;
// 	if (!IsBadReadPtr((PVOID)(dwTeleport+4), 4))
// 	{
// 		DWORD  dwTelBase = *(PDWORD)(dwTeleport+0x4);
// 		
// 		PrintLogA("BaseAddress =  %x", dwTelBase);
// 		if (dwTelBase)
// 		{
// 			int nTeleport = (int)*(PDWORD)(dwTelBase-4) - 1;
// 			
// 			PrintLogA("텔레폿 개수 %d", nTeleport);
// 			for (int i = nTeleport; i >= 0; i--)
// 			{
// 				DWORD dwTelEntry = *(PDWORD)(dwTelBase + i*8 +4);
// 				
// 				if (dwTelEntry == 0) continue;
// 				
// 				if (*(PDWORD)(dwTelEntry + 8) == 0) continue;
// // 
// 				POINT pt;
// 				pt.x = (int)*(PDWORD)(dwTelEntry+0x0C);
// 				pt.y = (int)*(PDWORD)(dwTelEntry+0x10);
// 				PrintLogA("인덱스 %d 텔레폿 위치 %x  %x 속성 %x", i, pt.x, pt.y, *(PDWORD)(dwTelEntry + 0x08));
// 			}
// 			//////////////////////////////////////////////////////////////////////////
// 			
// // 			DWORD dwTelEntry = *(PDWORD)(dwTelBase + 5*8 +4);
// // 			//*(PDWORD)(dwTelEntry+0x0C) = 96;
// // 			*(PDWORD)(dwTelEntry+0x10) = 419;
// // 			*(PDWORD)(dwTelEntry + 8) = 0x1;
// 			
// 			//dwTelEntry = *(PDWORD)(dwTelBase + 8*8 +4);
// 			//*(PDWORD)(dwTelEntry+0x0C) = -1567;
// 			//*(PDWORD)(dwTelEntry+0x10) = -198;
// 			//*(PDWORD)(dwTelEntry + 8) = 0x2;
// 			
// 		}
// 		else PrintLogA("dwTelBase is null");
// 	}
// 	else PrintLogA("Read Bad Base");
// 	
// }

void InitMobPos(DWORD dwMobPosObj)
{
	DWORD sub_525BA0 = 0x525BA0;
	__asm
	{
		pushad;
		mov		esi, dwMobPosObj;
		lea     edx, [esi+16Ch];
		xor     ecx, ecx;
		call    sub_525BA0;
		popad;
	}
}

void MoveMob(DWORD mobObj, DWORD X,DWORD Y)
{
	
}
int GetCharacterCount()
{
	DWORD dwMemAddr, dwTemp;
	DWORD dwPtrAddr;
	POINT pt;
	DWORD dwMemAddr1, ecx = 0; // 0331
	DWORD dwLen; // 120708 캐릭네임길이
	char  szName[MAX_PATH];
	int nCount = 0;


	dwMemAddr = MEMADDR_OTHERCHARICINFO;
	if (IsBadReadPtr((LPVOID)dwMemAddr, 4) == TRUE)
	{			
		return 0;
	}
	dwMemAddr = *(LPDWORD)dwMemAddr;
	dwMemAddr = dwMemAddr + MEMADDR_OTHERCHARICINFO_OFFSET1;
	if (IsBadReadPtr((LPVOID)dwMemAddr, 4) ==  FALSE)
	{
		dwMemAddr = *(LPDWORD)dwMemAddr;
		dwPtrAddr = dwMemAddr;

		while (dwPtrAddr)
		{
			dwMemAddr = dwPtrAddr;

			dwMemAddr = dwMemAddr + 4;
			if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) { break; }
			dwMemAddr = *(LPDWORD)dwMemAddr;

			dwMemAddr = dwMemAddr + 16;
			if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) { break; }
			dwTemp = *(LPDWORD)dwMemAddr;

			dwMemAddr = dwTemp + MEMADDR_OTHERCHARICINFO_OFFSET2 - 4;
			if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) { break; }				

			dwMemAddr = dwTemp + MEMADDR_OTHERCHARICINFO_OFFSET2;
			if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) { break; }
			dwMemAddr = *(LPDWORD)(dwMemAddr);

			// 120708 캐릭네임길이
			if (IsBadReadPtr((LPVOID)(dwMemAddr-4), 4)) {break;}
			dwLen = *(LPDWORD)(dwMemAddr-4);
			if (dwLen == 0 || dwLen >= 256) {break;}
			//DebugMessageA(1, "타케릭네임길이 = %d", dwLen);

			if (IsBadReadPtr((LPVOID)dwMemAddr, dwLen)) { break; }

			memset(szName, 0, MAX_PATH);
			memcpy(szName, (char*)dwMemAddr, dwLen);

			CString strName;
			strName.Format("%s", szName);
			//DebugMessageA(1, "타캐릭네임 = %s", szName);

			//MEMADDR_OTHERCHARICINFO_OFFSET3 =  MEMADDR_BOTPOSITION_OFFSET
			dwMemAddr = dwTemp + MEMADDR_OTHERCHARICINFO_OFFSET3 + 8;
			if (IsBadReadPtr((LPVOID)dwMemAddr, 4) )
				pt.y = 0;
			else
				pt.y = Descramble((LPDWORD)(dwMemAddr-8));

			dwMemAddr = dwTemp + MEMADDR_OTHERCHARICINFO_OFFSET3 + 8 + 12;
			if (IsBadReadPtr((LPVOID)dwMemAddr, 4))
				pt.x = 0;
			else
				pt.x = Descramble((LPDWORD)(dwMemAddr-8));


			if (IsBadReadPtr((LPVOID)dwPtrAddr, 4)) { break; }

			if (IsBadReadPtr((LPVOID)(dwPtrAddr - 12), 4)) { break; }
			dwMemAddr = *(LPDWORD)(dwPtrAddr - 12);
			if (dwMemAddr == 0) break;
			nCount++;
			dwPtrAddr = dwMemAddr + 0x10;
		}
	}	
	return nCount;
}

// 120419 해당 슬롯의 아이템개수얻기
DWORD GetInvenItemCount(DWORD nItemTabIndex, DWORD nSlotIndex)
{
	DWORD dwMemAddr = MEMADDR_BOTINFO;
	DWORD dwItemProperty = 0;

	if (nItemTabIndex == 3) nItemTabIndex = 4;
	else if (nItemTabIndex == 4) nItemTabIndex = 3;

	if (IsBadReadPtr((LPVOID)dwMemAddr, 4) == TRUE) 
	{			
		return 0;
	}
	dwMemAddr = *(LPDWORD)dwMemAddr;

	if (dwMemAddr == 0) return 0; // 120711 팅김

	dwMemAddr = dwMemAddr + 4 * nItemTabIndex + MEMADDR_BOTITEMINVENTORY_OFFSET;

	if ( IsBadReadPtr((LPVOID)dwMemAddr, 4) == FALSE)
	{
		dwMemAddr = *(LPDWORD)dwMemAddr;
		dwMemAddr = dwMemAddr + 8 * nSlotIndex;
		if ( dwMemAddr )
		{
			dwMemAddr = dwMemAddr + 4;
			if (IsBadReadPtr((LPVOID)dwMemAddr, 4) == FALSE)
			{
				dwMemAddr = *(LPDWORD)dwMemAddr;
				if ( dwMemAddr )
				{
					DWORD dwCount;
					__asm
					{
						pushad;
						mov		ecx, dwMemAddr;
						mov     edx, [ecx];
						mov     eax, [edx+44h];
						call    eax;
						mov		dwItemProperty, eax;
						popad;
					}
				}
			}
		}
	}		
	return dwItemProperty;
}

// BOOL ResetTeleportInfo(POINT srcPt, POINT dstPt)
// {
// 	DWORD oldProtect1, oldProtect2;
// 	DWORD dwTeleport = *(PDWORD)ADDR_TELEPORT_INFO;
// 	PrintLogA("Reset: %x", dwTeleport);
// 	if (!IsBadReadPtr((PVOID)(dwTeleport+4), 4))
// 	{
// 		DWORD  dwTelBase = *(PDWORD)(dwTeleport+0x4);
// 		if (dwTelBase)
// 		{
// 			int nTeleport = (int)*(PDWORD)(dwTelBase-4) - 1;
// 			PrintLogA("텔레폿 개수 %d", nTeleport);
// 			for (int i = nTeleport; i >= 0; i--)
// 			{
// 				DWORD dwTelEntry = *(PDWORD)(dwTelBase + i*8 +4);
// 				if (*(PDWORD)(dwTelEntry + 8) == 0) continue;
// 				POINT pt;
// 				pt.x = (int)*(PDWORD)(dwTelEntry+0x0C);
// 				pt.y = (int)*(PDWORD)(dwTelEntry+0x10);
// 				PrintLogA("인덱스 %d 텔레폿 위치 %d--%d 속성 %x", i, pt.x, pt.y, *(PDWORD)(dwTelEntry + 8));
// 			}
// 			//////////////////////////////////////////////////////////////////////////
// 			/*{
// 
// 				DWORD dwTelEntry = *(PDWORD)(dwTelBase + 2*8 +4);
// 				if (!IsBadWritePtr((PVOID)(dwTelEntry+0x0C), 4)
// 					&& !IsBadWritePtr((PVOID)(dwTelEntry+0x10), 4))
// 				{
// 					*(PDWORD)(dwTelEntry+0x0C) = (DWORD)srcPt.x;
// 					*(PDWORD)(dwTelEntry+0x10) = (DWORD)srcPt.y;
// 				}
// 				else 
// 				{
// 					PrintLogA("ResetteleportInfo rtn(1): %d", GetLastError());
// 					return FALSE;
// 				}
// 				*(PDWORD)(dwTelEntry + 8) = 0xA;
// 
// 				dwTelEntry = *(PDWORD)(dwTelBase + 3*8 +4);
// 				if (!IsBadWritePtr((PVOID)(dwTelEntry+0x0C), 4) && !IsBadWritePtr((PVOID)(dwTelEntry+0x10), 4))
// 				{
// 					*(PDWORD)(dwTelEntry+0x0C) = (DWORD)dstPt.x;
// 					*(PDWORD)(dwTelEntry+0x10) = (DWORD)dstPt.y;
// 				}
// 				else 
// 				{
// 					PrintLogA("ResetteleportInfo rtn(2)");
// 					return FALSE;
// 				}
// 				*(PDWORD)(dwTelEntry + 8) = 0xA;
// 
// 				PrintLogA("ResetteleportInfo end");
// 				return TRUE;
// 			}*/
// 		}
// 	}
// 	PrintLogA("ResetteleportInfo rtn(3)");
// 	return FALSE;
// }




DWORD GetSkillLevel(DWORD dwSkillId)
{
	//DebugMessageA(100, "GetSKillLevel enter = %d", dwSkillId);
	LPDWORD var_14 = NULL;
	DWORD var_10 = MEMADDR_BOTINFO;
	DWORD pfnFunc = MEMADDR_GETSKILLLEVELFUNC;
	DWORD lpClass = MEMADDR_GETSKILLLEVELCLASS;

	if (IsBadReadPtr((LPVOID)var_10, 4) == TRUE) 
	{
		//DebugMessageA(1, "GetSkillLevel rtn(1)");
		return 0;
	}
	var_10 = *(LPDWORD)var_10;

	if (IsBadReadPtr((LPVOID)lpClass, 4) == TRUE) 
	{
		//DebugMessageA(1, "GetSkillLevel rtn(2)");
		return 0;
	}
	lpClass = *(LPDWORD)lpClass;

	DWORD dwSkillLevel = 0;

	//var_14 = new DWORD[1];
	//*var_14 = 0;

	__asm{
		push 0;
		push 0;
		push 0;
		push 0;
		push dwSkillId;
		push var_10;
		mov ecx, lpClass;
		call pfnFunc;
		mov dwSkillLevel, eax;
	}

	//delete[] var_14;

	//DebugMessageA(100, "GetSkillLevel: LEV= %d", dwSkillLevel);
	return dwSkillLevel; 
}


void GetSkillInventoryInfo(DWORD* pSkillIdArray, DWORD* pLevelArray)
{
	//DebugMessageA(100, "GetSkillInventoryInfo enter");
	DWORD dwMemAddr, edi, nFirst;
	int nNumber;
	DWORD dwSkillLevel;

	dwMemAddr = MEMADDR_SKILLINVENTORY;
	if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) 
	{
		//DebugMessageA(100, "GetSkillInventoryInfo rtn(1)");
		return;
	}
	dwMemAddr = *(LPDWORD)dwMemAddr;
	if (dwMemAddr == 0) 
	{
		//DebugMessageA(100, "GetSkillInventoryInfo rtn(2)");
		return;
	}

	edi = dwMemAddr + MEMADDR_SKILLINVENTORY_OFFSET1;
	if (IsBadReadPtr((LPVOID)edi,4)) 
	{
		//DebugMessageA(100, "GetSkillInventoryInfo rtn(3)");
		return;
	}
	edi = *(LPDWORD)edi;
	if (edi == 0) 
	{
		//DebugMessageA(100, "GetSkillInventoryInfo rtn(4)");
		return;
	}

	edi += MEMADDR_SKILLINVENTORY_OFFSET2;
	if (IsBadReadPtr((LPVOID)edi, 4)) 
	{
		//DebugMessageA(100, "GetSkillInventoryInfo rtn(5)");
		return;
	}

	nFirst = *(LPDWORD)edi;

	dwMemAddr += MEMADDR_SKILLINVENTORY_OFFSET3;

	if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) 
	{
		//DebugMessageA(100, "GetSkillInventoryInfo rtn(6)");
		return;
	}
	dwMemAddr += 8;
	if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) 
	{
		//DebugMessageA(100, "GetSkillInventoryInfo rtn(7)");
		return;
	}

	dwMemAddr = *(LPDWORD)dwMemAddr;
	if (IsBadReadPtr((LPVOID)(dwMemAddr - 4), 4)) 
	{
		//DebugMessageA(100, "GetSkillInventoryInfo rtn(8)");
		return;
	}

	nNumber = *(LPDWORD)(dwMemAddr - 4);
	if (nNumber > 3) nNumber = 4;

	if (nNumber <= 0) 
	{
		//DebugMessageA(100, "GetSkillInventoryInfo rtn(9)");
		return;
	}

	edi = dwMemAddr + 8*nFirst + 4;

	int idx = 0;

	for (int Index = 0; Index < nNumber; Index ++, edi += 8)
	{
		if (IsBadReadPtr((LPVOID)edi, 4)) break;
		dwMemAddr = *(LPDWORD)edi;
		if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) break;
		dwMemAddr = *(LPDWORD)dwMemAddr;

		//DebugMessageA(1, "GetSkillId: %d", dwMemAddr);

		dwSkillLevel = GetSkillLevel(dwMemAddr);
		//pLevelArray->SetAtGrow(pLevelArray->GetSize(), dwSkillLevel);
		//pSkillIdArray->SetAtGrow(pSkillIdArray->GetSize(), dwMemAddr);
		
		pLevelArray[idx] = dwSkillLevel;
		pSkillIdArray[idx] = dwMemAddr;
		idx ++;

		//DebugMessageA(100, "GetSKillInventoryInfo: ID = %d, LEV = %d", dwMemAddr, dwSkillLevel);
	}
}

DWORD g_dw2121312 = 0;
DWORD g_dwEesp;
BOOL MyDeviceIoControl(HANDLE hDevice,DWORD dwIoControlCode,LPVOID lpInBuffer,DWORD nInBufferSize,LPVOID lpOutBuffer,DWORD nOutBufferSize,LPDWORD lpBytesReturned,LPOVERLAPPED lpOverlapped)
{	
	__asm
	{
		mov g_dwEesp, esp;
	}
	BOOL ret;
	DWORD OldProtect;
	
	if (VirtualProtect((LPVOID)haiDeviceIoControl.OrgProc, 6, PAGE_EXECUTE_READWRITE, &OldProtect) == 0) return 0;

	if (dwIoControlCode != 0x2f0003)
	{
		for (int i = 0; i < 20; i ++) if(i==5)PrintLogA("Device[%d] %x", i, *(DWORD*)(g_dwEesp+i*4));
		//PrintLogA("Device = %x", *(DWORD*)(g_dwEesp+20));
		//PrintLogA("MyDeviceIoControl(prev): 핸들=%x, 코드=%x, In-Buf-Size=%d, Out-Buf-Size=%d", 
		//	hDevice, dwIoControlCode, nInBufferSize, nOutBufferSize);
		//if (nInBufferSize > 0)
		//	PrintBufToFile((BYTE*)lpInBuffer, nInBufferSize, "c:\\in.log");
	}

	memcpy((LPBYTE)haiDeviceIoControl.OrgProc, haiDeviceIoControl.SaveBytes, 6);
	_DeviceIoControl pfnDeviceIoControl = (_DeviceIoControl)haiDeviceIoControl.OrgProc;
	ret = pfnDeviceIoControl(hDevice, dwIoControlCode, lpInBuffer, nInBufferSize, lpOutBuffer, nOutBufferSize, lpBytesReturned, lpOverlapped);
	memcpy((LPBYTE)haiDeviceIoControl.OrgProc, (LPBYTE)(&haiDeviceIoControl.hookBytes), 6);
	VirtualProtect(haiDeviceIoControl.OrgProc, 6, OldProtect, &OldProtect);
	
	if (dwIoControlCode != 0x2f0003)
	{
		//PrintLogA("MyDeviceIoControl(aft): 결과=%d, 값=%x", ret, lpOutBuffer==NULL?0:*(DWORD*)lpOutBuffer);
		//if (nOutBufferSize > 0)
		//	PrintBufToFile((BYTE*)lpOutBuffer, nOutBufferSize, "c:\\out.log");
	}

	ret  = 0;
	

	return ret;
}

BOOL MyGetKeyboardState(PBYTE lpKeyState)
{
	BOOL ret;
	DWORD OldProtect;
	if (VirtualProtect((LPVOID)haiGetKeyboardState.OrgProc, 6, PAGE_EXECUTE_READWRITE, &OldProtect) == 0) return 0;
		
	PrintLogA("MyGetKeyboardState");

	memcpy((LPBYTE)haiGetKeyboardState.OrgProc, haiGetKeyboardState.SaveBytes, 6);
	_GetKeyboardState pfnGetKeyboardState = (_GetKeyboardState)haiGetKeyboardState.OrgProc;
	ret = pfnGetKeyboardState(lpKeyState);
	memcpy((LPBYTE)haiGetKeyboardState.OrgProc, (LPBYTE)(&haiGetKeyboardState.hookBytes), 6);
	VirtualProtect(haiGetKeyboardState.OrgProc, 6, OldProtect, &OldProtect);
}
DWORD g_dwAsynEsp;
SHORT MyGetAsyncKeyState(int vKey)
{
	// 귀환값 
	// 0: 키가 눌리우지 않은 상태
	// 0x8001 : 키가 눌렸으며 중간에 끊긴 상태
	// 0x8000 : 키가 눌렸으며 이전 GetAsyncKeyState호출때부터 끊기지 않고 계속 눌려 있는 상태

	__asm
	{
		mov g_dwAsynEsp, esp;
	}

	SHORT nRet;
	DWORD dwOldProtect;
	if (VirtualProtect((LPVOID)haiGetAsyncKeyState.OrgProc, 6, PAGE_EXECUTE_READWRITE, &dwOldProtect) == 0)
		return 0;

	memcpy((LPBYTE)haiGetAsyncKeyState.OrgProc, haiGetAsyncKeyState.SaveBytes, 6);
	_GetAsyncKeyState pfnGetAsyncKeyState = (_GetAsyncKeyState)haiGetAsyncKeyState.OrgProc;
	nRet = pfnGetAsyncKeyState(vKey);
	memcpy((LPBYTE)haiGetAsyncKeyState.OrgProc, (LPBYTE)(&haiGetAsyncKeyState.hookBytes), 6);
	VirtualProtect(haiGetAsyncKeyState.OrgProc, 6, dwOldProtect, &dwOldProtect);


	if (vKey - VK_LEFT <= 3 && vKey - VK_LEFT >= 0)
	{
		//PrintLogA("MyGetAsynKeyState: vKey = %d, state = %x", vKey, nRet);
		if(nRet == 0)
		{
			nRet = g_wKeyMouseState[vKey];
			return nRet;
		}		
	}
	else
	{
		//PrintLogA("MyGetAsynKeyState: vKey = %x, state = %x", vKey, nRet);	
		//for (int i = 0; i < 10; i ++)
		//{
		//	PrintLogA("MyGetAsyncKeyState [%d] %x", i, *(DWORD*)(g_dwAsynEsp+4*i));
		//}
	}

	

	//PrintLogA("MyGetAsynKeyState: vKey = %x, state = %x", vKey, nRet);

	return nRet;
}
SHORT MyGetKeyState(int vKey)
{
	SHORT nRet;
	DWORD dwOldProtect;
	if (VirtualProtect((LPVOID)haiGetKeyState.OrgProc, 6, PAGE_EXECUTE_READWRITE, &dwOldProtect) == 0)
		return 0;

	PrintLogA("MyGetKeyState: %x", vKey);

	memcpy((LPBYTE)haiGetKeyState.OrgProc, haiGetKeyState.SaveBytes, 6);
	_GetAsyncKeyState pfnGetKeyState = (_GetAsyncKeyState)haiGetKeyState.OrgProc;
	nRet = pfnGetKeyState(vKey);
	memcpy((LPBYTE)haiGetKeyState.OrgProc, (LPBYTE)(&haiGetKeyState.hookBytes), 6);
	VirtualProtect(haiGetKeyState.OrgProc, 6, dwOldProtect, &dwOldProtect);

	return nRet;
}

BOOL MyGetWindowRect(HWND hWnd, LPRECT lpRect)
{
	PrintLogA("MyGetWIndowRect enter: HWND = %x", hWnd);

	DWORD OldProtect;
	BOOL bRet = FALSE;

	if (VirtualProtect((LPVOID)haiGetWindowRect.OrgProc, 6, 0x40, &OldProtect) == 0) return 0;

	memcpy((LPBYTE)haiGetWindowRect.OrgProc, haiGetWindowRect.SaveBytes, 6);
	_GetWindowRect pfnGetWindowRect = (_GetWindowRect)haiGetWindowRect.OrgProc;
	bRet = pfnGetWindowRect(hWnd, lpRect);
	memcpy((LPBYTE)haiGetWindowRect.OrgProc, (LPBYTE)(&haiGetWindowRect.hookBytes), 6);
	VirtualProtect(haiGetWindowRect.OrgProc, 6, OldProtect, &OldProtect);

	//PrintLogA("MyGetWindowRect (%d, %d, %d, %d)", lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);

	return bRet;
}


HWND MyGetFocus()
{
	HWND nRetWnd;
	DWORD dwOldProtect;

	if (g_GameHwnd == NULL)
	{
		if (VirtualProtect(haiGetFocus.OrgProc, 6, PAGE_EXECUTE_READWRITE, &dwOldProtect) == 0)
			return 0;

		memcpy((LPBYTE)haiGetFocus.OrgProc, haiGetFocus.SaveBytes, 6);
		_GetFocus pfnGetFocus = (_GetFocus)haiGetFocus.OrgProc;
		nRetWnd = pfnGetFocus();
		memcpy((LPBYTE)haiGetFocus.OrgProc, (LPBYTE)(&haiGetFocus.hookBytes), 6);

		VirtualProtect(haiGetFocus.OrgProc, 6, dwOldProtect, &dwOldProtect);
	}
	else
	{
		nRetWnd = g_GameHwnd;
	}	
	return nRetWnd;
}


DWORD GetProcessID(LPCTSTR pszProcessName)
{

	HANDLE         hProcessSnap = NULL;
	BOOL           bRet      = FALSE;
	PROCESSENTRY32 pe32      = {0};

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == (HANDLE)-1)
		return false;

	pe32.dwSize = sizeof(PROCESSENTRY32);
	DWORD dwProcessID = 0;

	//프로세스가 메모리상에 있으면 첫번째 프로세스를 얻는다
	if (Process32First(hProcessSnap, &pe32))
	{
		BOOL bCurrent = FALSE;
		MODULEENTRY32 me32       = {0};
		do
		{
			bCurrent = GetProcessModule(pe32.th32ProcessID, pszProcessName);
			if(bCurrent)
			{
				dwProcessID = pe32.th32ProcessID;
				break;
			}

		} while (Process32Next(hProcessSnap, &pe32)); //다음 프로세스의 정보를 구하여 있으면 루프
	}

	CloseHandle (hProcessSnap);
	return dwProcessID;
}


BOOL GetProcessModule(DWORD dwPID, LPCTSTR pszProcessName)
{
	HANDLE        hModuleSnap = NULL;
	MODULEENTRY32 me32        = {0};

	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);

	if (hModuleSnap == (HANDLE)-1)
		return FALSE;

	me32.dwSize = sizeof(MODULEENTRY32);
	//해당 프로세스의 모듈리스트를 루프로 돌려서 프로세스이름과 동일하면
	//true를 리턴한다.
	if(Module32First(hModuleSnap, &me32))
	{
		do
		{
			if(_tcscmp(me32.szModule, pszProcessName) == 0)
			{				
				CloseHandle (hModuleSnap);
				return TRUE;
			}
		}while(Module32Next(hModuleSnap, &me32));
	}

	CloseHandle (hModuleSnap);

	return FALSE;
}

void GetModuleList(DWORD dwPID)
{
	HANDLE        hModuleSnap = NULL;
	MODULEENTRY32 me32        = {0};

	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);

	if (hModuleSnap == (HANDLE)-1)
		return ;

	me32.dwSize = sizeof(MODULEENTRY32);
	if(Module32First(hModuleSnap, &me32))
	{
		do
		{
			PrintLogA("MOD NAME=%s, ,PATH=%s, BA=%x, SIZE=%x", me32.szModule, me32.szExePath,me32.modBaseAddr, me32.modBaseSize);
		}while(Module32Next(hModuleSnap, &me32));
	}

	CloseHandle (hModuleSnap);
}

