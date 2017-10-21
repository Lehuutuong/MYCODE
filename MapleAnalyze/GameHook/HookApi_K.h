#ifndef _HOOKAPI_H
#define _HOOKAPI_H

#pragma pack(push,1)
typedef struct _INLINE_HOOK_INFO
{
	CHAR SaveBuffer[0x10];
	WORD wJmpFlag;
	DWORD Hook_End_Addr;
	DWORD dwSaveNum;
	DWORD dw_Hook_Start;
	DWORD dw_Hook_End;
	CHAR IsHook;
}INLINE_HOOK_INFO,*PINLINE_HOOK_INFO;
#pragma pack(pop)

extern "C" BOOL __cdecl Hook_Api(DWORD dwApiAddr,DWORD dwMyFunction,PINLINE_HOOK_INFO MyStruct,DWORD *dwSaveCall);
extern "C" BOOL __cdecl Restore_Hook_Api(PINLINE_HOOK_INFO MyStruct);

#endif