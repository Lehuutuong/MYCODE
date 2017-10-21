/////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2009 PSJDC.
//
//						strmgr.h	
//
//	This file is created [3/31/2009 by LMC]
//	comment is written in Korean.
//
//	Main purpose
//
//	문자열&본문관련 처리 함수들을 선언합니다.
//
//	Modified History:
//
//		
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

// 문자열을 주어진 분리 문자열로 분리하여 문자열 배열을 리턴합니다.
void SplitString(IN	CString p_strText, IN CString p_strSplit, OUT CStringArray& p_strArray);

// 전화번호의 형식이 정확한가를 검사한다.
BOOL IsInvalidPhoneNumber(LPCTSTR p_PhoneNumber);

// 전자우편 형식이 정확한가를 검사한다.
BOOL IsInvalidEMail(LPCTSTR p_Email);

// 문자열을 Int 형으로 변환하기.
int ConvertStringToInt(LPCTSTR p_Str);

// Int를 문자열로 변환하기.
CString ConvertIntToCstring(int  p_Number);

// char* 로부터 CString을 얻습니다.
CString GetStringFromAnsiChars( const char* p_Ansi );


// 16진수 표현 형식의 입력문자열을 바이너리 렬로 변환한다.
void ConvertStringToBynary(LPCTSTR p_srcData, UINT p_nCount, OUT BYTE* p_dstBynary);

// 바이너리 자료의 문자열표현을 얻습니다.
void GetBinaryString(const unsigned char* p_pData, unsigned int p_nSize, OUT wchar_t* pBinaryString);

// 공백이 없는 바이너리 자료의 문자열표현을 얻습니다.
void GetBinaryStringNoSpace(const unsigned char* p_pData, unsigned int p_nSize, OUT wchar_t* pBinaryString);

// 본문파일을 유니코드 형식으로 읽기/쓰기 합니다.
CString ReadTextByUnicode(LPCTSTR p_strFilepath);
BOOL SaveTextByUnicode(LPCTSTR p_strFilepath, LPCTSTR p_strText);

// 유니코드 문자열을 ANSI 문자열로 변환.
char* ToANSI (OUT char* lpszNewText, int nLen, LPCTSTR lpszText);

// ANSI문자열을 유니코드 문자열로 변환.
wchar_t* ANSI2UNICODE (OUT wchar_t* lpszNewText, int nLen, const char* lpszText);

// 문자열 복사의 안전코드.
int strcpy_X(OUT char* dst, int dstLen, const char* src);

// 문자열 복사의 안전코드.
int wcscpy_X(OUT wchar_t* dst, int dstLen, LPCTSTR src);

// 시간 문자열을 얻기.
void GetSystemTimeString(OUT wchar_t*  strTime, int bufChLen, IN SYSTEMTIME* st);

// 주어진 스트림을 영숫자렬로 바꾸어주는 함수.
int GetAlphaNumericString(OUT wchar_t* dst, LPCTSTR src, int len, wchar_t wcDefault);

// 바이트 배렬 자료를 유효한 아스키 문자열로 변환한다. 비가역.
int GetAlphaNumericString( OUT char* p_dst, int p_DstMaxLen, const BYTE* p_src, int p_SrcLen, char cDefault = 0);

// CString 형 변수에 있는 문자열의 내용을 지우는 함수.    
void ClearCString(OUT CString &p_strText);

// 형식화된 수자표현을 얻는다. 례) 10000 => 10,000
// nFracDigits = -1 : use no. of frac. digits from string
CString XFormatNumber(LPCTSTR lpszNumber, int nFracDigits);

// Int64 를 형식화된 문자열로 출력하기
CString XFormatNumber(__int64 number);

// 바이트단위의 크기로 부터 KB단위의 크기 문자열을 얻는다.
CString GetSizeString(__int64 nSize);

// 바이트단위의 크기로 부터 바이트 단위의 크기 문자열을 얻는다.
CString GetSizeByteString(__int64 nSize);

// 바이트단위의 크기로 부터 MByte 단위의 크기 문자열을 얻는다.
CString GetMByteString(__int64 nSize);

// InfMByte 보다 크면 MB 단위로 SupByte 보다 작으면 바이트 단위로 크기를 문자열로 되돌린다.
CString MyGetSizeString(INT64 nSize, int InfMByte = 20*1024*1024, int SupByte = 9*1024*1024);

// 특정 문자들을 제외하면서 문자열로부터 int 형 수값을 얻는 함수
int GetValue(LPCTSTR p_str, LPCTSTR p_ExceptChars);

// 리소스 오픈하기
CString  GetString(UINT nID);

// 버전문자열로부터 버전 값을 얻는다. 99.99.99.99
BOOL GetVerValue(LPCTSTR p_strVer, OUT BYTE* ver);

// 버전문자열로부터 DWORD 버전 값을 얻는다.
DWORD GetVerValue(LPCTSTR p_strVer);

// 버전 값으로부터 버전 문자열을 얻는다.
CString GetVerString(DWORD p_Ver);

// 웹에 폴더 자료를 올려보낼때 2008.11.18
CString GetWebPath(LPCTSTR path, LPCTSTR splitChar = NULL, BOOL withBSlash = FALSE);

// 시간에 대한 문자열을 얻습니다.
CString GetDateString(UINT64 p_Time);

//  체계오류의 통보문을 얻기.
BOOL GetSystemErrorMessage(int nErr, OUT wchar_t *errMsg, int nMaxLen);
