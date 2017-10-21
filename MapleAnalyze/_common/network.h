/////////////////////////////////////////////////////////////////////////////  
//
// Copyright (c) 2009 PSJDC.  
//
//                      network.h    
//
//  This file is created [2009-6-10 by ZC]  
//  comment is written in Korean.  
//
//  Main purpose  
//
//  ��Ʈ��ũ ������ ��ɵ��� �����ϴ� ����Դϴ�.  
//	
//
//  Modified History:  
//
//	
//
/////////////////////////////////////////////////////////////////////////////  

#pragma once

//#include <winsock2.h>

// ��ǻ���� ������ �������� ã�� �ʿ��� ������ �� ���� �������� �������¸� �����Ѵ�.
BOOL DelShareFolder(const WCHAR* foldername);

// �Է��� ���ڿ��� ��Ȯ�� MAC�ּ� ���ڿ��ΰ��� �˻� �մϴ�.
BOOL CheckValidMacString(LPCTSTR p_strMacAddr);

// ���̳ʸ� ������ MAC�ּҰ� ��ȿ�Ѱ��� �˻��մϴ�.
BOOL CheckValidMacBinary( const PBYTE p_MacData );

// IP ���ڿ� ǥ���� ����ϴ�.
CString GetIpString( DWORD dwIpAddr );

// ���ڿ� ǥ�����κ��� IP���� ����ϴ�.
DWORD GetIpValue( LPCTSTR p_strIpAddr );

// PC�� ž��� ��Ʈ��ũ ���ǵ��� MAC�ּҸ� ����ϴ�.
int GetMacAddresses( OUT PBYTE p_AddrBuf, IN OUT int& nAddrNum);

// PC�� ������ IP�ּҵ��� ����ϴ�.
int GetIpAddresses( OUT CStringArray& p_IpArray, BOOL p_IsGateway = FALSE );

// Gateway IP�� ����ϴ�.
CString GetGatewayIP( );

// Ÿ���� Adapter�� ����(Mac,IP, Gateway IP)���� ��� �Լ�.
int GetMacIPAddresses( OUT PBYTE p_MacAddr, OUT CStringArray& p_IPArray, OUT CStringArray& p_GatewayIPArray, int p_idxAdapter, OUT WCHAR* name = NULL);

// Url �κ��� ȣ��Ʈ����(�̸�, IP , ��Ʈ ��)�� ��´�
CString GetIpFromUrl(IN CString& strUrl);

// Mac�ּҸ� ����Ʈ�ķ� ���� ���ڿ��� �ٲپ��ִ� �Լ�
BOOL GetMacAddressChar(OUT BYTE* MacChar, const BYTE* MacByte);

// ������ ������ �̸����� �����ϴ�.
BOOL GetCleanDomainName(const char* p_strDomain, OUT char* p_strCleanDomain, int p_OutBufLen);

// domain->ip
BOOL GetIPAddr(const char* name, OUT IN_ADDR* addr);

// ip->domain
BOOL GetDomainName(IN_ADDR addr, OUT char* name, int bufLen);

// ������ �̸����� ���� IP �ּҸ� ��´�. �����ϸ� 0�� �ǵ�����.
DWORD GetIpAddressEx(LPCTSTR URL);



