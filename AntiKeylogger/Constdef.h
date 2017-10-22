#ifndef _CONSTDEF_H_
#define _CONSTDEF_H_

#include "ntdll.h"

#define TEST(a, b) (a & b)

typedef struct _PARAM
{
	PWSTR	str;
	HWND	hWnd;
	LONG	para1;
	LONG	para2;
} PARAM, *PPARAM;

typedef struct _STRUCTA
{
	LIST_ENTRY	ListA;
	FARPROC		ProcOld;
	FARPROC		ProcNew;
	char		szFuncName[260];
	char		szDllName[260];
} STRUCTA, *PSTRUCTA;

typedef struct _STRUCTB
{
	HANDLE		FileHandle;
	PWSTR		FileName;
	PWSTR		FullFileName;
	BOOL		IsFileProtected;
	DWORD		Unknown4;
	DWORD		Unknown5;
	LIST_ENTRY	ListB;
} STRUCTB, *PSTRUCTB;

typedef struct _STRUCTC
{
	LIST_ENTRY	ListC;
	HANDLE		PortHandle;
	HANDLE		ThreadHandle;
	PVOID		Buffer;
	FARPROC		Process;
	DWORD		UniqueProcess;
	DWORD		UniqueThread;	
} STRUCTC, *PSTRUCTC;

typedef struct _STRUCTD
{
	DWORD	Process;
	DWORD	Thread;
	ULONG	Status;
	PVOID	BufferBase;
	ULONG	BufferLength;
	ULONG	Code;
} STRUCTD, *PSTRUCTD;

typedef struct _STRUCTE
{
	OSVERSIONINFOW			OSVersionInfo;
	WCHAR					lpData[0x20];
	BOOL					bQueryCpuInformation;
	SYSTEM_CPU_INFORMATION	CpuInfo;
} STRUCTE, *PSTRUCTE;

typedef struct _STRUCTH
{
	DWORD	para1;
	DWORD	MessageId;
	DWORD	para3;
	DWORD	para4;
	DWORD	strMsgLen;
	STRUCTE structe;
	PWSTR	strMsg;
} STRUCTH, *PSTRUCTH;

typedef struct _STRUCTG
{
	DWORD		para1;
	DWORD		para2;
	PSTRUCTH	buffer;
} STRUCTG, *PSTRUCTG;

typedef struct _STRUCTF
{
	BOOL		Extendable;
	PSTRUCTG	Data;
	DWORD		Mask;
} STRUCTF, *PSTRUCTF;

typedef struct _STRUCTJ
{
	char*	szData1;
	char*	szData2;
	DWORD	dwData;
	BYTE	pbData[0x10];
} STRUCTJ, *PSTRUCTJ;

typedef struct _STRUCTK
{
	RTL_CRITICAL_SECTION	rcs00;
	DWORD					dw18;
	BYTE*					pbBuffer1C;
	DWORD					dwLength20;
	DWORD*					pdwNewBuffer24;
	DWORD					dwCount28;
	DWORD					dwDriveCount2C;
	DWORD*					pdwDriveName30;
} STRUCTK, *PSTRUCTK;

#endif // _CONSTDEF_H_