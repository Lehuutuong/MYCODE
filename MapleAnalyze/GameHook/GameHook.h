#pragma once
#include "engine.h"


int WINAPI Fn_1();
int WINAPI Fn_2();
int WINAPI Fn_3();
int WINAPI Fn_4();
int WINAPI Fn_5();
int WINAPI Fn_6();
int WINAPI Fn_7();
int WINAPI Fn_8();
int WINAPI Fn_9();
int WINAPI Fn_10(LPVOID pBRBuf);
int WINAPI Fn_11();
int WINAPI Fn_12(); // 브릭설정
int WINAPI Fn_13(); // 브릭해제함수


void WINAPI MyRaiseException(
						   __in  DWORD dwExceptionCode,
						   __in  DWORD dwExceptionFlags,
						   __in  DWORD nNumberOfArguments,
						   __in  const ULONG_PTR *lpArguments
);


void SendExChangeRequestPacket();
//void SendExChangeAcceptPacket(DWORD dwID);
void SendExChangeForgivePacket(DWORD dwID);
void SendExChangeItemPacket();
void SendExChangeMesoPacket(DWORD dwGold);

void SendMoveItemPacket(int nTab, int nMySlot, int nCount, int nExChangeSlot);

DWORD GetItemIdOfItemInventory(DWORD nItemTabIndex, DWORD nSlotIndex);

BOOL  IsExchangableItem(DWORD nItemTabIndex, DWORD nSlotIndex);
DWORD GetInvenItemCount(DWORD nItemTabIndex, DWORD nSlotIndex);
DWORD GetDecryptValue(DWORD arg_0, DWORD arg_4, DWORD arg_8);

int  GetItemPosition();
void  PickItem(DWORD XPos, DWORD YPos, DWORD ItemObject);
void  ChangeObjectValue(DWORD dwObj,  DWORD Value);
void OpenPersernalShop(DWORD dwEdi);
void Encramle(PDWORD pAddr, DWORD val);
void SuddenMove();
BOOL ProcessRecvPacket( PBYTE pBuf, int nlen );
void InitMobPos(DWORD dwMobPosObj);
void MoveMob(DWORD X,DWORD Y);
int GetCharacterCount();
DWORD GetSpeedItemObj(DWORD dwItemObj);
DWORD	MyCreateTexture(LPDWORD arg_0, DWORD arg_4, DWORD arg_8, DWORD arg_C, DWORD arg_10, DWORD arg_14, DWORD arg_18, DWORD arg_1C, DWORD arg_20);
DWORD	My867130(DWORD arg_0, DWORD arg_4, DWORD arg_8, DWORD arg_C, DWORD arg_10, DWORD arg_14);

int GetMobCountInArea(RECT rt);
extern PVOID g_hVectoredHandler;


typedef struct _TELEPORT_NODE_INFO{
	DWORD dwtmp1;
	DWORD dwtmp2;
	DWORD dwType;
	int   X;
	int   Y;
}TELEPORT_NODE_INFO, *PTELEPORT_NODE_INFO; //6


void SetHardwareBreakOne(HANDLE hThread, BOOL bRemove, BOOL bData);
void SetHardwareBreakThread(DWORD dwThreadId, BOOL bRemove, BOOL bData);
void SetHardwareBreak(DWORD dwAddress1, DWORD dwAddress2/* = 0*/, DWORD dwAddress3/* = 0*/, DWORD dwAddress4/* = 0*/, BOOL bData);
void RemoveHardwareBreak();
