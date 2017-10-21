// MIPInstall.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MIPInstall.h"
#include <WS2spi.h>
#include <WinSock2.h>
#include <initguid.h>
#include <SpOrder.h>
#include <Shlwapi.h>

#pragma comment(lib, "ws2_32.lib")

#define MUSSPI				L"msvc"
#define MUSSPIDLL			L"msvc.dll"
#define	MUS_SPIDLL_PATH		L"%SystemRoot%\\System32\\" MUSSPIDLL

int		g_nNumberOfProtocols = 0;
DWORD	g_dwBufferLength = 0;
LPWSAPROTOCOL_INFOW	g_lpProtocolsInfo = NULL;

DEFINE_GUID( GUID_TRANSPORT_PROVIDER, 0xE70F1AA0, 0xAB8B, 0x11CF, \
			0x8C, 0xA3, 0x00, 0x80, 0x5F, 0x48, 0xA1, 0x92);
// {8942CF6A-323C-4ac5-AB95-1FD19227C805}
DEFINE_GUID(GUID_MUS_IP_PROVIDER,  0x8942cf6a, 0x323c, 0x4ac5, \
			0xab, 0x95, 0x1f, 0xd1, 0x92, 0x27, 0xc8, 0x5);
// {AC16AE1E-36AC-4a14-9007-316F2C1B0698}
DEFINE_GUID(GUID_MUS_TRANSPORT_PROVIDER, 
			0xac16ae1e, 0x36ac, 0x4a14, 0x90, 0x7, 0x31, 0x6f, 0x2c, 0x1b, 0x6, 0x98);

BOOL SPIEnumProtocols()
{
	int nErrno;

	g_lpProtocolsInfo = NULL;
	g_dwBufferLength = 0;
	g_nNumberOfProtocols = 0;

	if(WSCEnumProtocols(NULL, g_lpProtocolsInfo, &g_dwBufferLength, &nErrno) == SOCKET_ERROR
		&& nErrno != WSAENOBUFS)
	{
		return FALSE;
	}

	g_lpProtocolsInfo = (LPWSAPROTOCOL_INFOW)GlobalAlloc(GMEM_ZEROINIT, g_dwBufferLength);
	if(g_lpProtocolsInfo == NULL)
	{
		return FALSE;
	}

	g_nNumberOfProtocols = WSCEnumProtocols(NULL, g_lpProtocolsInfo, &g_dwBufferLength, &nErrno);
	if(g_nNumberOfProtocols == SOCKET_ERROR)
	{
		g_nNumberOfProtocols = 0;
		return FALSE;
	}
	return TRUE;
}

BOOL InstallSPI()
{
	int i, nErrno;
	BOOL bRawProviderFound = FALSE, bTcpProviderFound = FALSE, bUdpProviderFound = FALSE;
	WSAPROTOCOL_INFOW RawProtocolInfo1, RawProtocolInfo2, TcpProtocolInfo, UdpProtocolInfo;
	WSAPROTOCOL_INFOW ProtocolInfo[3];
	DWORD RawEntryId, TcpEntryId, UdpEntryId, dwEntryId = 0, dwNumOfEntries = 0;
	WCHAR szProtocol[MAX_PATH];
	LPDWORD lpdwCatalogEntryId;
	WCHAR szSPIPath[MAX_PATH];
	
	GetSystemDirectory(szSPIPath, MAX_PATH);
	wcscat(szSPIPath, L"\\");
	wcscat(szSPIPath, MUSSPIDLL);
	if(!PathFileExists(szSPIPath))
	{
		OutputDebugString(L"SPI 엔진없음");
		return FALSE;
	}
	
	SPIEnumProtocols();
	for(i = 0; i < g_nNumberOfProtocols; i ++)
	{
		if(bRawProviderFound == FALSE
			&& g_lpProtocolsInfo[i].iAddressFamily == AF_INET
			&& g_lpProtocolsInfo[i].iProtocol == IPPROTO_IP)
		{
			bRawProviderFound = TRUE;
			RawEntryId = g_lpProtocolsInfo[i].dwCatalogEntryId;
			RawProtocolInfo1 = g_lpProtocolsInfo[i];
			RawProtocolInfo2 = g_lpProtocolsInfo[i];
		}
		if(bTcpProviderFound == FALSE
			&& g_lpProtocolsInfo[i].iAddressFamily == AF_INET
			&& g_lpProtocolsInfo[i].iProtocol == IPPROTO_TCP)
		{
			bTcpProviderFound = TRUE;
			TcpEntryId = g_lpProtocolsInfo[i].dwCatalogEntryId;
			TcpProtocolInfo = g_lpProtocolsInfo[i];
		}
		if(bUdpProviderFound == FALSE
			&& g_lpProtocolsInfo[i].iAddressFamily == AF_INET
			&& g_lpProtocolsInfo[i].iProtocol == IPPROTO_UDP)
		{
			bUdpProviderFound = TRUE;
			UdpEntryId = g_lpProtocolsInfo[i].dwCatalogEntryId;
			UdpProtocolInfo = g_lpProtocolsInfo[i];
		}
	}
	wsprintfW(RawProtocolInfo2.szProtocol, L"%ws IP", MUSSPI);
	RawProtocolInfo2.ProtocolChain.ChainLen = 0;
	if(WSCInstallProvider((LPGUID)&GUID_MUS_IP_PROVIDER, MUS_SPIDLL_PATH, &RawProtocolInfo2, 1, &nErrno) == SOCKET_ERROR)
	{	
		OutputDebugString(L"WSCInstallProvider Failed");
		return FALSE;
	}
	GlobalFree((HGLOBAL)g_lpProtocolsInfo);

	SPIEnumProtocols();
	for(i = 0; i < g_nNumberOfProtocols; i ++)
	{
		if(IsEqualGUID(g_lpProtocolsInfo[i].ProviderId, GUID_MUS_IP_PROVIDER))
		{
			dwEntryId = g_lpProtocolsInfo[i].dwCatalogEntryId;
			break;
		}
	}
	if(bTcpProviderFound)
	{
		wsprintfW(szProtocol, L"%ws TCP/IP [TCP/IP]", MUSSPI);
		wcscpy(TcpProtocolInfo.szProtocol, szProtocol);
		if(TcpProtocolInfo.ProtocolChain.ChainLen == 1)
		{
			TcpProtocolInfo.ProtocolChain.ChainEntries[1] = TcpEntryId;
		}
		else
		{
			if(TcpProtocolInfo.ProtocolChain.ChainLen > 0)
			{
				for(i = TcpProtocolInfo.ProtocolChain.ChainLen; i > 0; i ++)
				{
					TcpProtocolInfo.ProtocolChain.ChainEntries[i + 1] = TcpProtocolInfo.ProtocolChain.ChainEntries[i];
				}
			}
		}
		TcpProtocolInfo.ProtocolChain.ChainLen ++;
		TcpProtocolInfo.ProtocolChain.ChainEntries[0] = dwEntryId;
		ProtocolInfo[dwNumOfEntries] = TcpProtocolInfo;
		dwNumOfEntries ++;
	}
	if(bUdpProviderFound)
	{
		wsprintfW(szProtocol, L"%ws UDP/IP [UDP/IP]", MUSSPI);
		wcscpy(UdpProtocolInfo.szProtocol, szProtocol);
		if(UdpProtocolInfo.ProtocolChain.ChainLen == 1)
		{
			UdpProtocolInfo.ProtocolChain.ChainEntries[1] = UdpEntryId;
		}
		else
		{
			if(UdpProtocolInfo.ProtocolChain.ChainLen > 0)
			{
				for(i = UdpProtocolInfo.ProtocolChain.ChainLen; i > 0; i ++)
				{
					UdpProtocolInfo.ProtocolChain.ChainEntries[i + 1] = UdpProtocolInfo.ProtocolChain.ChainEntries[i];
				}
			}
		}
		UdpProtocolInfo.ProtocolChain.ChainLen ++;
		UdpProtocolInfo.ProtocolChain.ChainEntries[0] = dwEntryId;
		ProtocolInfo[dwNumOfEntries] = UdpProtocolInfo;
		dwNumOfEntries ++;
	}
	if(bRawProviderFound)
	{
		wsprintfW(szProtocol, L"%ws RAW/IP [RAW/IP]", MUSSPI);
		wcscpy(RawProtocolInfo1.szProtocol, szProtocol);
		if(RawProtocolInfo1.ProtocolChain.ChainLen == 1)
		{
			RawProtocolInfo1.ProtocolChain.ChainEntries[1] = RawEntryId;
		}
		else
		{
			if(RawProtocolInfo1.ProtocolChain.ChainLen > 0)
			{
				for(i = RawProtocolInfo1.ProtocolChain.ChainLen; i > 0; i ++)
				{
					RawProtocolInfo1.ProtocolChain.ChainEntries[i + 1] = RawProtocolInfo1.ProtocolChain.ChainEntries[i];
				}
			}
		}
		RawProtocolInfo1.ProtocolChain.ChainLen ++;
		RawProtocolInfo1.ProtocolChain.ChainEntries[0] = dwEntryId;
		ProtocolInfo[dwNumOfEntries] = RawProtocolInfo1;
		dwNumOfEntries ++;
	}
	if(WSCInstallProvider((LPGUID)&GUID_MUS_TRANSPORT_PROVIDER, MUS_SPIDLL_PATH, &ProtocolInfo[0], dwNumOfEntries, &nErrno) == SOCKET_ERROR)
	{
		return FALSE;
	}
	GlobalFree((HGLOBAL)g_lpProtocolsInfo);

	SPIEnumProtocols();
	lpdwCatalogEntryId = (LPDWORD)GlobalAlloc(GMEM_ZEROINIT, g_nNumberOfProtocols * sizeof(DWORD));
	if(lpdwCatalogEntryId == NULL)
	{
		GlobalFree((HGLOBAL)g_lpProtocolsInfo);
		return FALSE;
	}
	dwNumOfEntries = 0;
	for(i = 0; i < g_nNumberOfProtocols; i ++)
	{
		if(IsEqualGUID(g_lpProtocolsInfo[i].ProviderId, GUID_MUS_IP_PROVIDER)
			|| IsEqualGUID(g_lpProtocolsInfo[i].ProviderId, GUID_MUS_TRANSPORT_PROVIDER))
		{
			lpdwCatalogEntryId[dwNumOfEntries] = g_lpProtocolsInfo[i].dwCatalogEntryId;
			dwNumOfEntries ++;
		}
	}
	for(i = 0; i < g_nNumberOfProtocols; i ++)
	{
		if(!IsEqualGUID(g_lpProtocolsInfo[i].ProviderId, GUID_MUS_IP_PROVIDER)
			&& !IsEqualGUID(g_lpProtocolsInfo[i].ProviderId, GUID_MUS_TRANSPORT_PROVIDER))
		{
			lpdwCatalogEntryId[dwNumOfEntries] = g_lpProtocolsInfo[i].dwCatalogEntryId;
			dwNumOfEntries ++;
		}
	}
	if(WSCWriteProviderOrder(lpdwCatalogEntryId, g_nNumberOfProtocols) == ERROR_SUCCESS)
	{
		GlobalFree((HGLOBAL)lpdwCatalogEntryId);
		GlobalFree((HGLOBAL)g_lpProtocolsInfo);
		return TRUE;
	}
	GlobalFree((HGLOBAL)lpdwCatalogEntryId);
	GlobalFree((HGLOBAL)g_lpProtocolsInfo);
	return FALSE;
}

BOOL UninstallSPI()
{
	int nErrno;
	if(WSCDeinstallProvider((LPGUID)&GUID_MUS_IP_PROVIDER, &nErrno) == SOCKET_ERROR
		|| WSCDeinstallProvider((LPGUID)&GUID_MUS_TRANSPORT_PROVIDER, &nErrno) == SOCKET_ERROR)
	{
	}
/*	WCHAR szFileName[MAX_PATH], szTempName[MAX_PATH];
	ExpandEnvironmentStringsW(MUS_SPIDLL_PATH, szFileName, MAX_PATH);
	wcscpy(szTempName, szFileName);
	szTempName[wcslen(szTempName) - 1] = L'_';
	MoveFileExW(szFileName, szTempName, MOVEFILE_COPY_ALLOWED);
	if (!DeleteFileW(szTempName) && GetLastError() != ERROR_FILE_NOT_FOUND)
	{
		MoveFileExW(szTempName, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
	}
*/
	return TRUE;
}

