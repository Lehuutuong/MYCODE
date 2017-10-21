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
//  네트워크 관련의 기능들을 구현하는 모듈입니다.  
//	
//
//  Modified History:  
//
//	
//
/////////////////////////////////////////////////////////////////////////////  

#pragma once

//#include <winsock2.h>

// 컴퓨터의 공유된 폴더들을 찾고 필요한 폴더와 그 하위 폴더들의 공유상태를 해제한다.
BOOL DelShareFolder(const WCHAR* foldername);

// 입력한 문자열이 정확한 MAC주소 문자열인가를 검사 합니다.
BOOL CheckValidMacString(LPCTSTR p_strMacAddr);

// 바이너리 형식의 MAC주소가 유효한가를 검사합니다.
BOOL CheckValidMacBinary( const PBYTE p_MacData );

// IP 문자열 표현을 얻습니다.
CString GetIpString( DWORD dwIpAddr );

// 문자열 표현으로부터 IP값을 얻습니다.
DWORD GetIpValue( LPCTSTR p_strIpAddr );

// PC에 탑재된 네트워크 기판들의 MAC주소를 얻습니다.
int GetMacAddresses( OUT PBYTE p_AddrBuf, IN OUT int& nAddrNum);

// PC에 설정된 IP주소들을 얻습니다.
int GetIpAddresses( OUT CStringArray& p_IpArray, BOOL p_IsGateway = FALSE );

// Gateway IP를 얻습니다.
CString GetGatewayIP( );

// 타당한 Adapter의 정보(Mac,IP, Gateway IP)들을 얻는 함수.
int GetMacIPAddresses( OUT PBYTE p_MacAddr, OUT CStringArray& p_IPArray, OUT CStringArray& p_GatewayIPArray, int p_idxAdapter, OUT WCHAR* name = NULL);

// Url 로부터 호스트정보(이름, IP , 포트 등)를 얻는다
CString GetIpFromUrl(IN CString& strUrl);

// Mac주소를 바이트렬로 부터 문자열로 바꾸어주는 함수
BOOL GetMacAddressChar(OUT BYTE* MacChar, const BYTE* MacByte);

// 깨끗한 도메인 이름만을 얻어냅니다.
BOOL GetCleanDomainName(const char* p_strDomain, OUT char* p_strCleanDomain, int p_OutBufLen);

// domain->ip
BOOL GetIPAddr(const char* name, OUT IN_ADDR* addr);

// ip->domain
BOOL GetDomainName(IN_ADDR addr, OUT char* name, int bufLen);

// 도메인 이름으로 부터 IP 주소를 얻는다. 실패하면 0을 되돌린다.
DWORD GetIpAddressEx(LPCTSTR URL);



