// MIPSPI.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <Tlhelp32.h>
#include "MIPSPI.H"
#include "ThemidaSDK.h"
#pragma comment(lib, "ws2_32.lib")

#define MUSSPI				L"engine"
#define INJECTEXE			"Bentley.exe"

CRITICAL_SECTION	SPI_CriticalSection;
DWORD	SPI_ReferenceCount = 0;
HMODULE	SPI_Module = NULL;
WSPPROC_TABLE		SPI_ProcTable;


typedef int (*ST) ();


VOID PrintLogA(
					 PCHAR FormatString,
					 ...
					 )
{
	char Buffer[MAX_PATH];
	va_list ap;

	va_start(ap, FormatString);
	vsprintf(Buffer, FormatString, ap);
	va_end(ap);

	strcat(Buffer, "\r\n");
	PrintLog(Buffer);
}

// MIPSPI.cpp : Defines the entry point for the DLL application.
//

// engine.dll 로드위한 처리 
int WINAPI WSPStartup(
		WORD wVersionRequested,
		LPWSPDATA lpWSPData,
		LPWSAPROTOCOL_INFOW lpProtocolInfo,
		WSPUPCALLTABLE UpcallTable,
		LPWSPPROC_TABLE lpProcTable)
{
	LPFNWSPSTARTUP lpfnWSPStartup;
	WSPPROC_TABLE ProcTable;
	WSPUPCALLTABLE TempUpcallTable;
	int nRet = ERROR_SUCCESS;
	
	////PrintLog("WSPStartup: enter(ReferenceCount=%d)", SPI_ReferenceCount);
	EnterCriticalSection(&SPI_CriticalSection);
	VM_START
	if(SPI_ReferenceCount == 0)
	{
		SPI_Module = LoadLibraryW(L"mswsock.dll");
		if(SPI_Module == NULL)
		{
			return WSAEPROVIDERFAILEDINIT;
		}
		lpfnWSPStartup = (LPFNWSPSTARTUP)GetProcAddress(SPI_Module, "WSPStartup");
		if(lpfnWSPStartup == NULL)
		{
			return WSAEPROVIDERFAILEDINIT;
		}
		RtlZeroMemory(&ProcTable, sizeof(ProcTable));
		TempUpcallTable = UpcallTable;
		nRet = lpfnWSPStartup(wVersionRequested, lpWSPData, lpProtocolInfo, TempUpcallTable, &ProcTable);
		SPI_ProcTable = ProcTable;
		DWORD PID = GetCurrentProcessId();
		char szExeName[MAX_PATH];
		GetProcessNameFromPID(szExeName, PID);
	//	if (!stricmp(szExeName, INJECTEXE))
		WriteFileLog("MIPSPI - exe name %s ", szExeName);
		if (strstr(szExeName, ".bin") > 0)
		{
			ST pStart;
			//PrintLogA("Load mscf.dll Success");
			pStart = (ST)GetProcAddress(LoadLibrary("E:\\first-task\\MapleAnalyze\\Bin\\engine.dll"), "ST");

			if (pStart)
			{
				WriteFileLog("Load engine.dll Success");
				pStart();
			}
			//else //PrintLogA("Load mscf.dll Failed"); 
		}
	}
	else
	{
		if(wVersionRequested > WSPVERSION)
			return WSAVERNOTSUPPORTED;
		lpWSPData->wVersion = WSPVERSION;
		lpWSPData->wHighVersion = WSPVERSION;
		wsprintfW(lpWSPData->szDescription, L"%ws Service Provider", MUSSPI);
	}
	RtlZeroMemory(lpProcTable, sizeof(WSPPROC_TABLE));
	lpProcTable->lpWSPAccept = WSPAccept;
	lpProcTable->lpWSPAddressToString = WSPAddressToString;
	lpProcTable->lpWSPAsyncSelect = WSPAsyncSelect;
	lpProcTable->lpWSPBind = WSPBind;
	lpProcTable->lpWSPCancelBlockingCall = WSPCancelBlockingCall;
	lpProcTable->lpWSPCleanup = WSPCleanup;
	lpProcTable->lpWSPCloseSocket = WSPCloseSocket;
	lpProcTable->lpWSPConnect = WSPConnect;
	lpProcTable->lpWSPDuplicateSocket = WSPDuplicateSocket;
	lpProcTable->lpWSPEnumNetworkEvents = WSPEnumNetworkEvents;
	lpProcTable->lpWSPEventSelect = WSPEventSelect;
	lpProcTable->lpWSPGetOverlappedResult = WSPGetOverlappedResult;
	lpProcTable->lpWSPGetPeerName = WSPGetPeerName;
	lpProcTable->lpWSPGetSockName = WSPGetSockName;
	lpProcTable->lpWSPGetSockOpt = WSPGetSockOpt;
	lpProcTable->lpWSPGetQOSByName = WSPGetQOSByName;
	lpProcTable->lpWSPIoctl = WSPIoctl;
	lpProcTable->lpWSPJoinLeaf = WSPJoinLeaf;
	lpProcTable->lpWSPListen = WSPListen;
	lpProcTable->lpWSPRecv = WSPRecv;
	lpProcTable->lpWSPRecvDisconnect = WSPRecvDisconnect;
	lpProcTable->lpWSPRecvFrom = WSPRecvFrom;
	lpProcTable->lpWSPSelect = WSPSelect;
	lpProcTable->lpWSPSend = WSPSend;
	lpProcTable->lpWSPSendDisconnect = WSPSendDisconnect;
	lpProcTable->lpWSPSendTo = WSPSendTo;
	lpProcTable->lpWSPSetSockOpt = WSPSetSockOpt;
	lpProcTable->lpWSPShutdown = WSPShutdown;
	lpProcTable->lpWSPSocket = WSPSocket;
	lpProcTable->lpWSPStringToAddress = WSPStringToAddress;

	SPI_ReferenceCount ++;
	VM_END
	LeaveCriticalSection(&SPI_CriticalSection);
	////PrintLog("WSPStartup: exit(Return=%d)", nRet);
	return nRet;
}

SOCKET WSPAPI WSPAccept(
    SOCKET s,
    struct sockaddr FAR * addr,
    LPINT addrlen,
    LPCONDITIONPROC lpfnCondition,
    DWORD_PTR dwCallbackData,
    LPINT lpErrno
    )
{
	PrintLog("WSPAccept");
	return SPI_ProcTable.lpWSPAccept(s, addr, addrlen, lpfnCondition, dwCallbackData, lpErrno);
}

INT WSPAPI WSPAddressToString(
    LPSOCKADDR lpsaAddress,
    DWORD dwAddressLength,
    LPWSAPROTOCOL_INFOW lpProtocolInfo,
    LPWSTR lpszAddressString,
    LPDWORD lpdwAddressStringLength,
    LPINT lpErrno
    )
{
	PrintLog("WSPAddressToString");
	return SPI_ProcTable.lpWSPAddressToString(lpsaAddress, dwAddressLength, lpProtocolInfo, lpszAddressString, lpdwAddressStringLength, lpErrno);
}

int WSPAPI WSPAsyncSelect(
    SOCKET s,
    HWND hWnd,
    unsigned int wMsg,
    long lEvent,
    LPINT lpErrno
    )
{
	PrintLog("WSPAsyncSelect");
	return SPI_ProcTable.lpWSPAsyncSelect(s, hWnd, wMsg, lEvent, lpErrno);
}

#define NEW_IP_ADDRESS	0xC0A8000A // 192.168.1.10

int WSPAPI WSPBind(
    SOCKET s,
    const struct sockaddr FAR * name,
    int namelen,
    LPINT lpErrno
    )
{
	struct sockaddr_in *pname = (struct sockaddr_in*)name, *pTempname, Tempsockaddr;
	int nTempnamelen;
	
	PrintLog("WSPBind");
/*	pTempname = pname;
	RtlZeroMemory(&Tempsockaddr, sizeof(Tempsockaddr));
	if(pname == NULL 
		|| pname->sin_family != AF_INET
		|| (pname->sin_addr.s_addr == htonl(INADDR_ANY))
		|| (pname->sin_addr.s_addr == htonl(INADDR_LOOPBACK)))
	{
		nTempnamelen = namelen;
	}
	else
	{
		Tempsockaddr.sin_family = pname->sin_family;
		Tempsockaddr.sin_port = pname->sin_port;
		Tempsockaddr.sin_addr.s_addr = htonl(NEW_IP_ADDRESS); // 여기서 아이피를 변경함
		pTempname = &Tempsockaddr;
		nTempnamelen = sizeof(Tempsockaddr);
		//PrintLog("Bind(New) %s:%d", inet_ntoa(pTempname->sin_addr), pTempname->sin_port);
	}
	return SPI_ProcTable.lpWSPBind(s, (struct sockaddr*)pTempname, nTempnamelen, lpErrno);
	*/
	return SPI_ProcTable.lpWSPBind(s, name, namelen, lpErrno);
}

int WSPAPI WSPCancelBlockingCall(
    LPINT lpErrno
    )
{
	PrintLog("WSPCancelBlockingCall");
	return SPI_ProcTable.lpWSPCancelBlockingCall(lpErrno);
}

int WSPAPI WSPCleanup(
    LPINT lpErrno
    )
{
	int nRet;
	PrintLog("WSPCleanup");
	if(SPI_ReferenceCount == 0)
	{
		*lpErrno = WSANOTINITIALISED;
		return SOCKET_ERROR;
	}
	nRet = SPI_ProcTable.lpWSPCleanup(lpErrno);
	EnterCriticalSection(&SPI_CriticalSection);
	SPI_ReferenceCount --;
	if(SPI_ReferenceCount == 0)
	{
		FreeLibrary(SPI_Module);
		RtlZeroMemory(&SPI_ProcTable, sizeof(SPI_ProcTable));
		SPI_Module = NULL;
	}
	LeaveCriticalSection(&SPI_CriticalSection);
	return nRet;
}
DWORD g_dwEsp;
int i;
int WSPAPI WSPCloseSocket(
    SOCKET s,
    LPINT lpErrno
    )
{
	__asm
	{
		mov g_dwEsp, esp;
	}
	int nRet;
	PrintLog("WSPCloseSocket %d", (int)s);
	if ((int)s == 1008)
	{
		for (i = 0; i < 50; i++)
		{
			PrintLog("sp %x", *(PDWORD)(g_dwEsp+i*4));
		}		
	}
	
	nRet = SPI_ProcTable.lpWSPCloseSocket(s, lpErrno);
	if(nRet != SOCKET_ERROR) nRet = ERROR_SUCCESS;
	return nRet;
}

int WSPAPI WSPConnect(
    SOCKET s,
    const struct sockaddr FAR * name,
    int namelen,
    LPWSABUF lpCallerData,
    LPWSABUF lpCalleeData,
    LPQOS lpSQOS,
    LPQOS lpGQOS,
    LPINT lpErrno
    )
{
	struct sockaddr_in *pname = (struct sockaddr_in*)name, *pTempname, Tempsockaddr;
	int nTempnamelen, nErrno;
	int Port;
	char ip[50];
	
	PrintLog("WSPConnect %s %s %s", (char*)name, (char*)lpCallerData, (char*)lpCalleeData);
	return SPI_ProcTable.lpWSPConnect(s, name, namelen, lpCallerData, lpCalleeData, lpSQOS, lpGQOS, lpErrno);
}

int WSPAPI WSPDuplicateSocket(
    SOCKET s,
    DWORD dwProcessId,
    LPWSAPROTOCOL_INFOW lpProtocolInfo,
    LPINT lpErrno
    )
{
	PrintLog("WSPDuplicateSocket");
	return SPI_ProcTable.lpWSPDuplicateSocket(s, dwProcessId, lpProtocolInfo, lpErrno);
}

int WSPAPI WSPEnumNetworkEvents(
    SOCKET s,
    WSAEVENT hEventObject,
    LPWSANETWORKEVENTS lpNetworkEvents,
    LPINT lpErrno
    )
{
	PrintLog("WSPEnumNetworkEvents");
	return SPI_ProcTable.lpWSPEnumNetworkEvents(s, hEventObject, lpNetworkEvents, lpErrno);
}

int WSPAPI WSPEventSelect(
    SOCKET s,
    WSAEVENT hEventObject,
    long lNetworkEvents,
    LPINT lpErrno
    )
{
	PrintLog("WSPEventSelect");
	return SPI_ProcTable.lpWSPEventSelect(s, hEventObject, lNetworkEvents, lpErrno);
}

BOOL WSPAPI WSPGetOverlappedResult(
    SOCKET s,
    LPWSAOVERLAPPED lpOverlapped,
    LPDWORD lpcbTransfer,
    BOOL fWait,
    LPDWORD lpdwFlags,
    LPINT lpErrno
    )
{
	PrintLog("WSPGetOverlappedResult");
	if(lpOverlapped->Internal != ERROR_NO_MORE_ITEMS)
	{
		*lpcbTransfer = lpOverlapped->InternalHigh;
		*lpdwFlags = lpOverlapped->Offset;
		*lpErrno = lpOverlapped->OffsetHigh;
		return (lpOverlapped->OffsetHigh == ERROR_SUCCESS);
	}
	if(fWait == FALSE)
	{
		*lpErrno = WSA_IO_INCOMPLETE;
		return FALSE;
	}
	if(WaitForSingleObject(lpOverlapped->hEvent, INFINITE) != ERROR_SUCCESS
		|| lpOverlapped->Internal == ERROR_NO_MORE_ITEMS)
	{
		*lpErrno = WSASYSCALLFAILURE;
		return FALSE;
	}
	*lpcbTransfer = lpOverlapped->InternalHigh;
	*lpdwFlags = lpOverlapped->Offset;
	*lpErrno = lpOverlapped->OffsetHigh;
	return (lpOverlapped->OffsetHigh == ERROR_SUCCESS);
}

int WSPAPI WSPGetPeerName(
    SOCKET s,
    struct sockaddr FAR * name,
    LPINT namelen,
    LPINT lpErrno
    )
{
	PrintLog("WSPGetPeerName");
	return SPI_ProcTable.lpWSPGetPeerName(s, name, namelen, lpErrno);
}

int WSPAPI WSPGetSockName(
    SOCKET s,
    struct sockaddr FAR * name,
    LPINT namelen,
    LPINT lpErrno
    )
{
	PrintLog("WSPGetSockName");
	return SPI_ProcTable.lpWSPGetSockName(s, name, namelen, lpErrno);
}

int WSPAPI WSPGetSockOpt(
    SOCKET s,
    int level,
    int optname,
    char FAR * optval,
    LPINT optlen,
    LPINT lpErrno
    )
{
	PrintLog("WSPGetSockOpt");
	return SPI_ProcTable.lpWSPGetSockOpt(s, level, optname, optval, optlen, lpErrno);
}

BOOL WSPAPI WSPGetQOSByName(
    SOCKET s,
    LPWSABUF lpQOSName,
    LPQOS lpQOS,
    LPINT lpErrno
    )
{
	PrintLog("WSPGetQOSByName");
	return SPI_ProcTable.lpWSPGetQOSByName(s, lpQOSName, lpQOS, lpErrno);
}

int WSPAPI WSPIoctl(
    SOCKET s,
    DWORD dwIoControlCode,
    LPVOID lpvInBuffer,
    DWORD cbInBuffer,
    LPVOID lpvOutBuffer,
    DWORD cbOutBuffer,
    LPDWORD lpcbBytesReturned,
    LPWSAOVERLAPPED lpOverlapped,
    LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
    LPWSATHREADID lpThreadId,
    LPINT lpErrno
    )
{
	PrintLog("WSPIoctl");
	return SPI_ProcTable.lpWSPIoctl(s, dwIoControlCode, lpvInBuffer, cbInBuffer,
		lpvOutBuffer, cbOutBuffer, lpcbBytesReturned, lpOverlapped,
		lpCompletionRoutine, lpThreadId, 
		lpErrno);
}

SOCKET WSPAPI WSPJoinLeaf(
    SOCKET s,
    const struct sockaddr FAR * name,
    int namelen,
    LPWSABUF lpCallerData,
    LPWSABUF lpCalleeData,
    LPQOS lpSQOS,
    LPQOS lpGQOS,
    DWORD dwFlags,
    LPINT lpErrno
    )
{
	PrintLog("WSPJoinLeaf");
	return SPI_ProcTable.lpWSPJoinLeaf(s, name, namelen, lpCallerData, lpCalleeData, lpSQOS, lpGQOS, dwFlags, lpErrno);
}

int WSPAPI WSPListen(
    SOCKET s,
    int backlog,
    LPINT lpErrno
    )
{
	PrintLog("WSPListen");
	return SPI_ProcTable.lpWSPListen(s, backlog, lpErrno);
}

int WSPAPI WSPRecv(
    SOCKET s,
    LPWSABUF lpBuffers,
    DWORD dwBufferCount,
    LPDWORD lpNumberOfBytesRecvd,
    LPDWORD lpFlags,
    LPWSAOVERLAPPED lpOverlapped,
    LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
    LPWSATHREADID lpThreadId,
    LPINT lpErrno
    )
{
// 	PrintLog("WSPRecv");
	return SPI_ProcTable.lpWSPRecv(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, lpCompletionRoutine, lpThreadId, lpErrno);
}

int WSPAPI WSPRecvDisconnect(
    SOCKET s,
    LPWSABUF lpInboundDisconnectData,
    LPINT lpErrno
    )
{
	PrintLog("WSPRecvDisconnect");
	return SPI_ProcTable.lpWSPRecvDisconnect(s, lpInboundDisconnectData, lpErrno);
}

int WSPAPI WSPRecvFrom(
    SOCKET s,
    LPWSABUF lpBuffers,
    DWORD dwBufferCount,
    LPDWORD lpNumberOfBytesRecvd,
    LPDWORD lpFlags,
    struct sockaddr FAR * lpFrom,
    LPINT lpFromlen,
    LPWSAOVERLAPPED lpOverlapped,
    LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
    LPWSATHREADID lpThreadId,
    LPINT lpErrno
    )
{
	PrintLog("WSPRecvFrom");
	return SPI_ProcTable.lpWSPRecvFrom(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpFrom, lpFromlen, lpOverlapped, lpCompletionRoutine, lpThreadId, lpErrno);
}

int WSPAPI WSPSelect(
    int nfds,
    fd_set FAR * readfds,
    fd_set FAR * writefds,
    fd_set FAR * exceptfds,
    const struct timeval FAR * timeout,
    LPINT lpErrno
    )
{
	PrintLog("WSPSelect");
	return SPI_ProcTable.lpWSPSelect(nfds, readfds, writefds, exceptfds, timeout, lpErrno);
}

int WSPAPI WSPSend(
    SOCKET s,
    LPWSABUF lpBuffers,
    DWORD dwBufferCount,
    LPDWORD lpNumberOfBytesSent,
    DWORD dwFlags,
    LPWSAOVERLAPPED lpOverlapped,
    LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
    LPWSATHREADID lpThreadId,
    LPINT lpErrno
    )
{
// 	PrintLog("WSPSend");
	return SPI_ProcTable.lpWSPSend(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpOverlapped, lpCompletionRoutine, lpThreadId, lpErrno);
}

int WSPAPI WSPSendDisconnect(
    SOCKET s,
    LPWSABUF lpOutboundDisconnectData,
    LPINT lpErrno
    )
{
	PrintLog("WSPSendDisconnect");
	return SPI_ProcTable.lpWSPSendDisconnect(s, lpOutboundDisconnectData, lpErrno);
}

int WSPAPI WSPSendTo(
    SOCKET s,
    LPWSABUF lpBuffers,
    DWORD dwBufferCount,
    LPDWORD lpNumberOfBytesSent,
    DWORD dwFlags,
    const struct sockaddr FAR * lpTo,
    int iTolen,
    LPWSAOVERLAPPED lpOverlapped,
    LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine,
    LPWSATHREADID lpThreadId,
    LPINT lpErrno
    )
{
	struct sockaddr_in *pname = (struct sockaddr_in*)lpTo, *pTempname, Tempsockaddr;
	int nTempnamelen, nErrno;
	
	PrintLog("WSPSendTo");
	pTempname = pname;
	RtlZeroMemory(&Tempsockaddr, sizeof(Tempsockaddr));
/*	if(pname == NULL 
		|| pname->sin_family != AF_INET
		|| (pname->sin_addr.s_addr == htonl(INADDR_ANY))
		|| (pname->sin_addr.s_addr == htonl(INADDR_LOOPBACK)))
	{
		nTempnamelen = iTolen;
	}
	else
	{
		RtlZeroMemory(&Tempsockaddr, sizeof(Tempsockaddr));
		nTempnamelen = sizeof(Tempsockaddr);
		nErrno = ERROR_SUCCESS;
		SPI_ProcTable.lpWSPGetSockName(s, (struct sockaddr*)&Tempsockaddr, &nTempnamelen, &nErrno);
		if(nErrno != WSA_INVALID_PARAMETER)
		{
			Tempsockaddr.sin_family = pname->sin_family;
			Tempsockaddr.sin_port = pname->sin_port;
			Tempsockaddr.sin_addr.s_addr = htonl(NEW_IP_ADDRESS); // 여기서 아이피를 변경함
			pTempname = &Tempsockaddr;
			nTempnamelen = sizeof(Tempsockaddr);
			//PrintLog("WSPConnect(New) %s:%d", inet_ntoa(pTempname->sin_addr), pTempname->sin_port);
		}
	}
*/	return SPI_ProcTable.lpWSPSendTo(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpTo, iTolen, lpOverlapped, lpCompletionRoutine, lpThreadId, lpErrno);
}

int WSPAPI WSPSetSockOpt(
    SOCKET s,
    int level,
    int optname,
    const char FAR * optval,
    int optlen,
    LPINT lpErrno
    )
{
	PrintLog("WSPSetSockOpt");
	return SPI_ProcTable.lpWSPSetSockOpt(s, level, optname, optval, optlen, lpErrno);
}

int WSPAPI WSPShutdown(
    SOCKET s,
    int how,
    LPINT lpErrno
    )
{
	PrintLog("WSPShutdown");
	return SPI_ProcTable.lpWSPShutdown(s, how, lpErrno);
}

SOCKET WSPAPI WSPSocket(
    int af,
    int type,
    int protocol,
    LPWSAPROTOCOL_INFOW lpProtocolInfo,
    GROUP g,
    DWORD dwFlags,
    LPINT lpErrno
    )
{
	
	SOCKET rtSocket = SPI_ProcTable.lpWSPSocket(af, type, protocol, lpProtocolInfo, g, dwFlags, lpErrno);
	PrintLog("WSPSocket %d", (int)rtSocket);
	return rtSocket;
}

INT WSPAPI WSPStringToAddress(
    LPWSTR AddressString,
    INT AddressFamily,
    LPWSAPROTOCOL_INFOW lpProtocolInfo,
    LPSOCKADDR lpAddress,
    LPINT lpAddressLength,
    LPINT lpErrno
    )
{
	PrintLog("WSPStringToAddress");
	return SPI_ProcTable.lpWSPStringToAddress(AddressString, AddressFamily, lpProtocolInfo, lpAddress, lpAddressLength, lpErrno);
}

BOOL IsEqualProcess(LPWSTR lpProcessName)
{
	WCHAR szFileName[MAX_PATH], *p;
	GetModuleFileNameW(NULL, szFileName, sizeof(szFileName));
	p = wcsrchr(szFileName, L'\\');
	if(p)
	{
		p ++;
		if(wcsicmp(p, lpProcessName) == 0)
			return TRUE;
	}
	return FALSE;
}


BOOL GetProcessNameFromPID(LPSTR lpProcessName, DWORD PID)
{
	HANDLE         hProcessSnap = NULL;
	PROCESSENTRY32 pe32         = {0};
	BOOL		   bRet = FALSE;	

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return (DWORD)INVALID_HANDLE_VALUE;
	}
	pe32.dwSize = sizeof(PROCESSENTRY32);
	int nRunnigNum = 0;
	if (Process32First(hProcessSnap, &pe32))
	{
		DWORD	Code = 0;
		do
		{			
			if (pe32.th32ProcessID == PID)
			{				
				strcpy(lpProcessName, pe32.szExeFile);								
				bRet = TRUE;				
				//PrintLog("동일프로세스 찿음");
				break;
			}			
		} while (Process32Next(hProcessSnap, &pe32));
	}

	CloseHandle (hProcessSnap);


}


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		//AfxMessageBox("Inject");
		InitializeCriticalSection(&SPI_CriticalSection);
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		DeleteCriticalSection(&SPI_CriticalSection);
	}
    return TRUE;
}

