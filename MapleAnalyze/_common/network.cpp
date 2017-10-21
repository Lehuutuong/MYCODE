/////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2009 PSJDC.
//
//						network.cpp	
//
//	This file is created [3/31/2009 by LMC]
//	comment is written in Korean.
//
//	Main purpose
//
//	��Ʈ��ũ ������ ��ɵ��� �����ϴ� ����Դϴ�.
//
//	Modified History:
//
//		
//
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "network.h"

#include <iphlpapi.h>
#include <ipifcons.h>
#include <afxinet.h>

#include "strmgr.h"
#include "AppDebug.h"

#include<lmshare.h>
#include<lmaccess.h>

#define MyOutputDebugStringEx

#define NET_API_STATUS          DWORD
#define API_RET_TYPE            NET_API_STATUS      // Old value: do not use
#if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
#define NET_API_FUNCTION    __stdcall
#else
#define NET_API_FUNCTION
#endif

typedef NET_API_STATUS (NET_API_FUNCTION *LMS_NETSHAREENUM)(IN LPWSTR servername, IN DWORD level, OUT LPBYTE *bufptr, IN DWORD prefmaxlen, OUT LPDWORD entriesread, OUT LPDWORD totalentries, IN OUT LPDWORD resume_handle);
typedef NET_API_STATUS (NET_API_FUNCTION *LMS_NETSHAREDEL)(IN LPWSTR servername, IN LPWSTR netname, IN DWORD reserved);

/* =============================================================================================== 

�Լ�����:	-----------------	DelShareFolder

	 ��ǻ���� ������ �������� ã�� �ʿ��� ������ �� ���� �������� �������¸� �����Ѵ�.

=============================================================================================== */
BOOL DelShareFolder(const WCHAR * foldername)
{
	HINSTANCE ghDll_NetApi32;

	LMS_NETSHAREENUM pLMSNetShareEnum;
	LMS_NETSHAREDEL pLMSNetShareDel;

	ghDll_NetApi32 = LoadLibrary(L"NETAPI32.DLL");
	if(ghDll_NetApi32==NULL) return FALSE;

	pLMSNetShareEnum = (LMS_NETSHAREENUM)GetProcAddress(ghDll_NetApi32, "NetShareEnum");
	pLMSNetShareDel = (LMS_NETSHAREDEL)GetProcAddress(ghDll_NetApi32, "NetShareDel");

	SHARE_INFO_502 *pshinfo;
	DWORD dwCount,dwTotal;

	pLMSNetShareEnum(NULL,502,(LPBYTE *)&pshinfo,-1,&dwCount,&dwTotal,NULL);
	for(DWORD i = 0; i < dwCount; i++) 
	{

		if(pshinfo->shi502_type ==STYPE_DISKTREE)
		{
			if(wcsncmp(foldername,pshinfo->shi502_path,wcslen(foldername)) == 0) 
			{
				pLMSNetShareDel(NULL,pshinfo->shi502_netname,0);
			}

		}

		pshinfo++;	
	}

	return TRUE;
}

/* ===============================================================================================

�Լ�����:	-----------------	CheckValidMacString

    �Է��� ���ڿ��� ��Ȯ�� MAC�ּ� ���ڿ��ΰ��� �˻� �մϴ�.

�Ķ����:

	p_strMacAddr	: MAC�ּ� ���ڿ�

���� ��:

    TRUE	: ��Ȯ��
	FALSE	: ��Ȯ���� ����

=============================================================================================== */
BOOL CheckValidMacString(LPCTSTR p_strMacAddr)
{
	if( wcslen(p_strMacAddr) < 17) return FALSE;

	for (int i = 0; i < 17; i++)
	{
		wchar_t c = p_strMacAddr[i];
		if ((i % 3) != 2)
		{
			if(!(c >= '0' && c <= '9' || c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F'))
			{
				return FALSE;
			}
		}
		else
		{
			if (c != ' ')
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

/* ===============================================================================================

�Լ�����:	-----------------	CheckValidMacBinary

    ���̳ʸ� ������ MAC�ּҰ� ��ȿ�Ѱ��� �˻��մϴ�.

�Ķ����:

	p_strMacAddr	: MAC�ּ� 

���� ��:

    TRUE/FALSE

=============================================================================================== */
BOOL CheckValidMacBinary( const PBYTE p_MacData )
{
	UINT sum;
	sum = (UINT)p_MacData[0] + p_MacData[1] + p_MacData[2] + p_MacData[3] + p_MacData[4] + p_MacData[5];

	return (BOOL)sum;
}

/* ===============================================================================================

�Լ�����:	-----------------	GetIpString

    IP ���ڿ� ǥ���� ����ϴ�.

�Ķ����:

	dwIpAddr : IP �ּ�

���� ��:

    IP ���ڿ� ǥ��

=============================================================================================== */
CString GetIpString( DWORD dwIpAddr )
{
	CString strRet;
	BYTE field0, field1, field2, field3;
	field0 = (BYTE)(dwIpAddr  & 0xFF);
	field1 = (BYTE)((dwIpAddr & 0xFF00) >> 8);
	field2 = (BYTE)((dwIpAddr & 0xFF0000) >> 16);
	field3 = (BYTE)((dwIpAddr & 0xFF000000) >> 24);

	//strRet.Format(L"%03d.%03d.%03d.%03d", field3, field2, field1, field0);
	strRet.Format(L"%d.%d.%d.%d", field3, field2, field1, field0);

	return strRet;
}

/* ===============================================================================================

�Լ�����:	-----------------	GetIpValue

    ���ڿ� ǥ�����κ��� IP���� ����ϴ�.

�Ķ����:

	strIpAddr : IP �ּ��� ���ڿ� ǥ��

���� ��:

    IP ��

=============================================================================================== */
DWORD GetIpValue( LPCTSTR p_strIpAddr )
{
	CStringArray parts;

	CString strIpAddr = p_strIpAddr;
	SplitString(strIpAddr, L".", parts);

	if (parts.GetCount() != 4)
	{
		return 0;
	}

	UINT field0, field1, field2, field3;
	WCHAR *ptr;
	field0 = (UINT)_wcstoi64( parts.GetAt(0), &ptr, 10);
	field1 = (UINT)_wcstoi64( parts.GetAt(1), &ptr, 10);
	field2 = (UINT)_wcstoi64( parts.GetAt(2), &ptr, 10);
	field3 = (UINT)_wcstoi64( parts.GetAt(3), &ptr, 10);

	
	DWORD dwAddr;
	dwAddr = 
		(field0 << 24) | 
		(field1 << 16) | 
		(field2 << 8) | 
		(field3 << 0);

	return dwAddr;
}

/* ===============================================================================================

�Լ�����:	-----------------	GetMacAddresses

    PC�� ž��� ��Ʈ��ũ ���ǵ��� MAC�ּҸ� ����ϴ�.
	������ ������ŭ ��⸦ �õ� �մϴ�.

�Ķ����:

	nAddrNum	: �ִ� MAC�ּ� ������ �����մϴ�. ���� MAC�ּҰ����� �����ϴϴ�.

���� ��:

    ���� MAC�ּҰ���

=============================================================================================== */
int GetMacAddresses( OUT PBYTE p_AddrBuf, IN OUT int& nAddrNum)
{
	PIP_ADAPTER_INFO pAdapterInfo = NULL;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;
	int count = 0;

	pAdapterInfo = (IP_ADAPTER_INFO *) malloc( sizeof(IP_ADAPTER_INFO) );
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);

	if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen); 
	}

	if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) == NO_ERROR) 
	{
		// 
		// ��ġ���� ��⿡ ����
		// 

		// 
		// ù ��ġ�� �ݴϴ�.
		// 
		pAdapter = pAdapterInfo;

		// 
		// ��� ��ġ�� ���Ͽ� ��ȯ�մϴ�.
		// 
		while (pAdapter) 
		{
			memcpy(p_AddrBuf + count * 6, pAdapter->Address, 6);

			// 
			// ���� ����
			// 
			count++;
			if (count >= nAddrNum)
			{
				break;
			}
			// 
			// ���� ��ġ��
			// 
			pAdapter = pAdapter->Next;
			
		}
	}

	nAddrNum = count;

	if(pAdapterInfo)
	{
		free(pAdapterInfo);
	}

	return count;
}

/* ===============================================================================================

�Լ�����:	-----------------	GetIpAddresses

    PC�� ������ IP�ּҵ��� ����ϴ�.

�Ķ����:

	p_IpArray	: IP�ּ� ���ڿ����� �޾ư� �����

	p_IsGateway : Gateway IP�ּҸ� ��ڴ°� �ϴ� ���.

���� ��:

	���� IP����

=============================================================================================== */
int GetIpAddresses( OUT CStringArray& p_IpArray, BOOL p_IsGateway )
{
	PIP_ADAPTER_INFO pAdapterInfo = NULL;
	PIP_ADAPTER_INFO pAdapter = NULL;
	PIP_ADDR_STRING  pIpString = NULL;
	DWORD dwRetVal = 0;
	CString strIp;

	pAdapterInfo = (IP_ADAPTER_INFO *) malloc( sizeof(IP_ADAPTER_INFO) );
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);

	if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen); 
	}

	p_IpArray.RemoveAll();
	if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) == NO_ERROR) 
	{
		// 
		// ��ġ���� ��⿡ ����
		// 

		// 
		// ù ��ġ�� �ݴϴ�.
		// 
		pAdapter = pAdapterInfo;

		// 
		// ��� ��ġ�� ���Ͽ� ��ȯ�մϴ�.
		// 
		while (pAdapter) 
		{
			if (p_IsGateway == FALSE)
			{
				// 
				// IP �ּ� ���
				// 
				pIpString = &pAdapter->IpAddressList;
			}
			else
			{
				// 
				// Gateway IP �ּ� ���
				// 
				pIpString = &pAdapter->GatewayList;
			}
			while(pIpString)
			{
				strIp = GetStringFromAnsiChars( pIpString->IpAddress.String );
				p_IpArray.Add(strIp);
				pIpString = pIpString->Next;
			}

			// 
			// ���� ��ġ��
			// 
			pAdapter = pAdapter->Next;
		}
	}

	if(pAdapterInfo)
	{
		free(pAdapterInfo);
	}

	return (int)p_IpArray.GetCount();
}

/* ===============================================================================================

�Լ�����:	-----------------	GetGatewayIP

	Gateway IP�� ����ϴ�.

=============================================================================================== */
CString GetGatewayIP( )
{
	PIP_ADAPTER_INFO pAdapterInfo = NULL;
	PIP_ADAPTER_INFO pAdapter = NULL;
	PIP_ADDR_STRING  pIpString = NULL;
	DWORD dwRetVal = 0;
	CString strIp = L"";

	pAdapterInfo = (IP_ADAPTER_INFO *) malloc( sizeof(IP_ADAPTER_INFO) );
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);

	if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen); 
	}

	if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) == NO_ERROR) 
	{
		// 
		// ��ġ���� ��⿡ ����
		// 

		// 
		// ù ��ġ�� �ݴϴ�.
		// 
		pAdapter = pAdapterInfo;

		// 
		// ��� ��ġ�� ���Ͽ� ��ȯ�մϴ�.
		// 
		while (pAdapter) 
		{
			pIpString = &pAdapter->GatewayList;
			while(pIpString)
			{
				strIp = GetStringFromAnsiChars( pIpString->IpAddress.String );
				goto EXIT_HANDLE;
			}

			// 
			// ���� ��ġ��
			// 
			pAdapter = pAdapter->Next;
		}
	}

EXIT_HANDLE:

	if(pAdapterInfo)
	{
		free(pAdapterInfo);
	}

	return strIp;
}

/* =============================================================================================== 

�Լ�����:	-----------------	GetMacIPAddresses

	 Ÿ���� Adapter�� ����(Mac,IP, Gateway IP)���� ��� �Լ�.

	 ���� ���� �ش� ���� Adapter�� ÷���̴�.

=============================================================================================== */
int GetMacIPAddresses( OUT PBYTE p_MacAddr, OUT CStringArray& p_IPArray, OUT CStringArray& p_GatewayIPArray, int p_idxAdapter, OUT WCHAR* name)
{
	PIP_ADAPTER_INFO pAdapterInfo = NULL;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;
	int idx = -1;
	
	PIP_ADDR_STRING  pIpString = NULL;
	CString strIp = L"";
	int ip;

	pAdapterInfo = (IP_ADAPTER_INFO *) malloc( sizeof(IP_ADAPTER_INFO) );
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);

	if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen); 
	}

	if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) == NO_ERROR) 
	{
		// 
		// ��ġ���� ��⿡ ����
		// 

		// 
		// ù ��ġ�� �ݴϴ�.
		// 
		pAdapter = pAdapterInfo;

		// 
		// ��� ��ġ�� ���Ͽ� ��ȯ�մϴ�.
		// 
		while (pAdapter) 
		{
			idx++;
			// MAC �ּ�
			if (p_MacAddr)
			{
				memcpy(p_MacAddr, pAdapter->Address, 6);
			}

			if (name)
			{
				ANSI2UNICODE(name, MAX_PATH, pAdapter->AdapterName);
			}

			// IP �ּ�

			p_IPArray.RemoveAll();
			pIpString = &pAdapter->IpAddressList;

			while(pIpString)
			{
				strIp = GetStringFromAnsiChars( pIpString->IpAddress.String );
				if(ip = GetIpValue(strIp) != 0)
				{
					p_IPArray.Add(strIp);
				}
				pIpString = pIpString->Next;
			}

			// Gateway IP �ּ�
			p_GatewayIPArray.RemoveAll();
			pIpString = &pAdapter->GatewayList;

			while(pIpString)
			{
				strIp = GetStringFromAnsiChars( pIpString->IpAddress.String );
				if(ip = GetIpValue(strIp) != 0)
				{
					p_GatewayIPArray.Add(strIp);
				}
				pIpString = pIpString->Next;
			}

			// 
			// ���� ����
			// 
			if (idx >= p_idxAdapter)
			{
				if(p_idxAdapter >= 0 || (p_IPArray.GetCount() > 0 && p_GatewayIPArray.GetCount() > 0))
					break;
			}
			// 
			// ���� ��ġ��
			// 
			pAdapter = pAdapter->Next;

		}
	}


	if(pAdapterInfo)
	{
		free(pAdapterInfo);
	}

	return idx;
}

/* =============================================================================================== 

�Լ�����:	-----------------	GetIpFromUrl
	
	Url �κ��� ȣ��Ʈ����(�̸�, IP , ��Ʈ ��)�� ��´�

�Ķ����: 
	
	strUrl : ������ url ��Ʈ��

���ϰ�:  
	
	IP �ּ� ��Ʈ��

	IP �ּҰ� �������� �� �� IP �ּҸ� @�� �����Ѵ�.(��:192.168.0.111@192.168.0.200)
	IP �ּҸ� ���� ���ϸ� �� ���ڿ� �����Ѵ�.
	�� �Լ��� ����ð��� �������� �ɸ�.

=============================================================================================== */
CString GetIpFromUrl(IN CString& strUrl)
{
	DWORD dwServiceType;
	CString strServer,strObject,strIPAddress=L"";
	WORD/*INTERNET_PORT*/ nPort;
	WSADATA wsaData;
	int iResult;
	DWORD dwError;
	struct hostent *remoteHost;
	struct in_addr addr;
	CString strMsg=L"",strMsg1;
	int len,i;

	char address[4][256];
	char* ptr = NULL, *temp;
	int result;
	BOOL isUrl = FALSE;

	if(strUrl==L"")  goto L_EXIT;

	//url�� �Ľ��Ѵ�.
	if ( ! ::AfxParseURL( strUrl, dwServiceType, strServer, strObject, nPort ) )
	{
		//AfxMessageBox(L"URL ������ ��Ȯġ �ʽ��ϴ�.");
		goto L_EXIT;
	}

	char host_name[200];

	// ������ �ּҸ� �����ڵ�κ��� ANSI�ڵ�� ��ȯ�Ѵ�.
	WideCharToMultiByte(CP_ACP, 0, strServer.GetBuffer(), -1, 
		host_name, strServer.GetLength() + 1, NULL, NULL);

	//Winsock �ʱ�ȭ
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		strMsg1.Format(L"WSAStartup ����: %d\n", iResult);
		//AfxMessageBox(strMsg1);
		goto L_EXIT ;
	}

	temp = host_name;
	for( i = 0; i < 4; i++ ){
		ptr = strchr( temp, '.' );
		if( ptr ){
			len = (int)(ptr - temp);
			strncpy_s( address[i], 256, temp, len );
			address[i][len] = 0;
			ptr++;
			temp = ptr;
		}
		else{
			if( i == 3 ){
				strcpy_s( address[i], 256, temp );
				break;
			}
			else
				break;
		}
	}
	if( i == 3 ){
		for( i = 0 ; i < 4; i++ ){
			result = atoi( address[i] );
			if( result == 0 ){
				if( ( strlen( address[i] ) > 1 ) || ( address[i][0] != 0x30 ) ){
					isUrl = TRUE;
					break;
				}
			}
			if( ( result >= 0 ) && (  result <= 255  ) )
				continue;
			else{
				isUrl = TRUE;
				break;
			}
		}
	}
	else
		isUrl = TRUE;

	// ȣ��Ʈ�̸��� �����̸�  gethostbyname() ����
	if (isUrl) {        

		//ȣ��Ʈ������ ��´�.
		remoteHost = gethostbyname(host_name);//������ �ð��� �ɸ�
	}
	//���ڰ� �ƴϸ�(�� IPv4 �ּ��̸�) 
	else {

		addr.s_addr = inet_addr(host_name);
		if (addr.s_addr == INADDR_NONE) {
			//AfxMessageBox(L"IP�ּҰ� �ǹٸ��� �ʴ�.");
			goto L_EXIT;
		} else
			//IP�ּҷκ��� ȣ��Ʈ������ ��´�. ������ �ð��� �ɸ�
			remoteHost = gethostbyaddr((char *) &addr, 4, AF_INET);
	}

	//��������
	if (remoteHost == NULL) {
		dwError = WSAGetLastError();
		if (dwError != 0) {
			if (dwError == WSAHOST_NOT_FOUND) {
				//AfxMessageBox(L"Host not found");
				goto L_EXIT ;
			} else if (dwError == WSANO_DATA) {
				//AfxMessageBox(L"No data record found");
				goto L_EXIT;
			} else {
				strMsg1.Format(L"Function failed with error: %ld\n", dwError);
				//AfxMessageBox(strMsg1);
				goto L_EXIT ;
			}
		}
	}
	else
	{
		strMsg += L"Function returned:\n\n";
		//ȣ��Ʈ�̸��� ��´�. hostent.h_name
		len = (int)strlen(remoteHost->h_name);
		WCHAR* lpWstr;
		lpWstr = new WCHAR[len + 1];
		MultiByteToWideChar(CP_ACP, 0, remoteHost->h_name, -1, lpWstr, len + 1);
		strMsg1.Format(L"\tOfficial name: %s\n", lpWstr);
		strMsg += strMsg1;
		delete [] lpWstr;

		//ȣ��Ʈ�� �ٸ� �̸����� ��´� hostent.h_aliases
		for (i=0;remoteHost->h_aliases[i]!=NULL;i++)
		{
			len = (int)strlen(remoteHost->h_aliases[i]);
			lpWstr=new WCHAR[len+1];
			MultiByteToWideChar(CP_ACP,0,remoteHost->h_aliases[i],-1,lpWstr,len+1);
			strMsg1.Format(L"\tAlternate names %d: %s\n", i+1, lpWstr);
			strMsg += strMsg1;
			delete [] lpWstr;

		}

		//ip�ּ������� ��´�
		strMsg1=L"\tAddress type: ";
		switch (remoteHost->h_addrtype) {
	case AF_INET:
		strMsg1+=L"AF_INET\n";
		break;
	case AF_INET6:
		strMsg1+=L"AF_INET6\n";
		break;
	case AF_NETBIOS:
		strMsg1+=L"AF_NETBIOS\n";
		break;
	default:
		CString strMsg2;
		strMsg2.Format(L"%d\n", remoteHost->h_addrtype);
		strMsg1+=strMsg2;
		break;
		}

		//IP�ּұ��� ��´�
		strMsg+=strMsg1;
		strMsg1.Format(L"\tAddress length: %d\n", remoteHost->h_length);
		strMsg+=strMsg1;

		//IP �ּ� ��´�
		for (i=0;remoteHost->h_addr_list[i]!=NULL;i++)
		{
			addr.s_addr = *(u_long *) remoteHost->h_addr_list[i];
			len = (int)strlen(inet_ntoa(addr));
			lpWstr=new WCHAR[len+1];
			MultiByteToWideChar(CP_ACP,0,inet_ntoa(addr),-1,lpWstr,len+1);
			strMsg1.Format(L"\tIP Address %d: %s\n", i+1, lpWstr);

			//IP�ּ� ��Ʈ��
			//if (i!=0)
			//{
			// strIPAddress+=L"@";//ip�ּ� ���б�ȣ
			//}

			strIPAddress+=lpWstr;

			strMsg+=strMsg1;
			delete [] lpWstr;

			// ó������ ��� IP�ּҸ� ��ȿ�Ѱ����� �Ѵ�.
			if( i == 0 ) 
				break;
		}

		//��Ʈ
		strMsg1.Format(L"\tPort : %d",nPort);
		strMsg+=strMsg1;

		//ȣ��Ʈ�� ���� �ⱸ
		//AfxMessageBox(strMsg);
	} 

L_EXIT:
	WSACleanup();

	WCHAR sPort[10] = { 0x00, };
	_itow_s( nPort, sPort, 10, 10 );
	strIPAddress.AppendFormat( L":%s", sPort );
	return strIPAddress;
}

/* =============================================================================================== 

�Լ�����:	-----------------	GetMacAddressChar

	 Mac�ּҸ� ����Ʈ�ķ� ���� ���ڿ��� �ٲپ��ִ� �Լ�

=============================================================================================== */
BOOL GetMacAddressChar(OUT BYTE* MacChar, const BYTE* MacByte)
{
	char buf[13];
	sprintf_s(buf,13, "%2X%2X%2X%2X%2X%2X", MacByte[0], MacByte[1], MacByte[2], MacByte[3], MacByte[4], MacByte[5]);
	memcpy(MacChar, buf, 12);
	return TRUE;
}

/* =============================================================================================== 

�Լ�����:	-----------------	GetCleanDomainName

	 ������ ������ �̸����� �����ϴ�.

=============================================================================================== */
BOOL GetCleanDomainName(const char * p_strDomain, OUT char* p_strCleanDomain, int p_OutBufLen)
{

	if(p_strDomain == NULL) return FALSE;
	if(p_strCleanDomain == NULL) return FALSE;

	const char * st;
	if(_strnicmp(p_strDomain, "https://", strlen("https://")) == 0)
	{
		st = p_strDomain + strlen("https://");
	}
	else
	{
		st = p_strDomain;
	}

	const char * en_1, *en;
	en = st + strlen(st); 

	en_1 = strchr(st, ':');
	if(en_1 > 0 && en_1 < en) en = en_1;

	en_1 = strchr(st, '/');
	if(en_1 > 0 && en_1 < en) en = en_1;

	en_1 = strchr(st, '\\');
	if(en_1 > 0 && en_1 < en) en = en_1;

	if(en == 0 || en < st) return FALSE;

	strncpy_s(p_strCleanDomain, p_OutBufLen, st, en - st);
	p_strCleanDomain[en - st] = 0;
	return TRUE;

}

/* =============================================================================================== 

�Լ�����:	-----------------	GetIPAddr

	 domain ���κ��� ip ���

=============================================================================================== */
BOOL GetIPAddr(const char *name, OUT IN_ADDR *addr)
{
	HOSTENT *ptr = gethostbyname(name);
	if(ptr == NULL) return FALSE;
	memcpy(addr, ptr->h_addr, ptr->h_length);
	return TRUE; 
}

/* =============================================================================================== 

�Լ�����:	-----------------	GetDomainName

	 ip �κ��� domain �̸� ���

=============================================================================================== */
BOOL GetDomainName(IN_ADDR addr, OUT char *name, int bufLen)
{
	HOSTENT *ptr = gethostbyaddr((char *)&addr, sizeof(addr), AF_INET);
	if(ptr == NULL) return FALSE;
	strcpy_s(name, bufLen, ptr->h_name);
	return TRUE;
}

/* =============================================================================================== 

�Լ�����:	-----------------	GetIpAddressEx

	 ������ �̸����� ���� IP �ּҸ� ��´�. �����ϸ� 0�� �ǵ�����.

=============================================================================================== */
DWORD GetIpAddressEx(LPCTSTR URL)
{
	int ret = 0;

	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2),&wsa)!= 0) return ret;

	char url[MAX_PATH], domain[MAX_PATH];
	if(wcslen(URL) > MAX_PATH) return ret;
	ToANSI(url,MAX_PATH, (wchar_t*)URL);

	GetCleanDomainName(url, domain, MAX_PATH);

	IN_ADDR addr;
	if(GetIPAddr(domain, &addr) == TRUE)
	{
		ret = addr.S_un.S_addr;
	}

// 	MyOutputDebugStringEx(DBG_SECUDRIVE, DBG_TRACE, L"GetIpAddressEx IP %X ", ret);

	ret = htonl(ret);

//	MyOutputDebugStringEx(DBG_SECUDRIVE, DBG_TRACE, L"GetIpAddressEx IP htonl %X ", ret);

	WSACleanup();
	return ret;

}

