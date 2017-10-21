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
//	���ڿ�&�������� ó�� �Լ����� �����մϴ�.
//
//	Modified History:
//
//		
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

// ���ڿ��� �־��� �и� ���ڿ��� �и��Ͽ� ���ڿ� �迭�� �����մϴ�.
void SplitString(IN	CString p_strText, IN CString p_strSplit, OUT CStringArray& p_strArray);

// ��ȭ��ȣ�� ������ ��Ȯ�Ѱ��� �˻��Ѵ�.
BOOL IsInvalidPhoneNumber(LPCTSTR p_PhoneNumber);

// ���ڿ��� ������ ��Ȯ�Ѱ��� �˻��Ѵ�.
BOOL IsInvalidEMail(LPCTSTR p_Email);

// ���ڿ��� Int ������ ��ȯ�ϱ�.
int ConvertStringToInt(LPCTSTR p_Str);

// Int�� ���ڿ��� ��ȯ�ϱ�.
CString ConvertIntToCstring(int  p_Number);

// char* �κ��� CString�� ����ϴ�.
CString GetStringFromAnsiChars( const char* p_Ansi );


// 16���� ǥ�� ������ �Է¹��ڿ��� ���̳ʸ� �ķ� ��ȯ�Ѵ�.
void ConvertStringToBynary(LPCTSTR p_srcData, UINT p_nCount, OUT BYTE* p_dstBynary);

// ���̳ʸ� �ڷ��� ���ڿ�ǥ���� ����ϴ�.
void GetBinaryString(const unsigned char* p_pData, unsigned int p_nSize, OUT wchar_t* pBinaryString);

// ������ ���� ���̳ʸ� �ڷ��� ���ڿ�ǥ���� ����ϴ�.
void GetBinaryStringNoSpace(const unsigned char* p_pData, unsigned int p_nSize, OUT wchar_t* pBinaryString);

// ���������� �����ڵ� �������� �б�/���� �մϴ�.
CString ReadTextByUnicode(LPCTSTR p_strFilepath);
BOOL SaveTextByUnicode(LPCTSTR p_strFilepath, LPCTSTR p_strText);

// �����ڵ� ���ڿ��� ANSI ���ڿ��� ��ȯ.
char* ToANSI (OUT char* lpszNewText, int nLen, LPCTSTR lpszText);

// ANSI���ڿ��� �����ڵ� ���ڿ��� ��ȯ.
wchar_t* ANSI2UNICODE (OUT wchar_t* lpszNewText, int nLen, const char* lpszText);

// ���ڿ� ������ �����ڵ�.
int strcpy_X(OUT char* dst, int dstLen, const char* src);

// ���ڿ� ������ �����ڵ�.
int wcscpy_X(OUT wchar_t* dst, int dstLen, LPCTSTR src);

// �ð� ���ڿ��� ���.
void GetSystemTimeString(OUT wchar_t*  strTime, int bufChLen, IN SYSTEMTIME* st);

// �־��� ��Ʈ���� �����ڷķ� �ٲپ��ִ� �Լ�.
int GetAlphaNumericString(OUT wchar_t* dst, LPCTSTR src, int len, wchar_t wcDefault);

// ����Ʈ ��� �ڷḦ ��ȿ�� �ƽ�Ű ���ڿ��� ��ȯ�Ѵ�. �񰡿�.
int GetAlphaNumericString( OUT char* p_dst, int p_DstMaxLen, const BYTE* p_src, int p_SrcLen, char cDefault = 0);

// CString �� ������ �ִ� ���ڿ��� ������ ����� �Լ�.    
void ClearCString(OUT CString &p_strText);

// ����ȭ�� ����ǥ���� ��´�. ��) 10000 => 10,000
// nFracDigits = -1 : use no. of frac. digits from string
CString XFormatNumber(LPCTSTR lpszNumber, int nFracDigits);

// Int64 �� ����ȭ�� ���ڿ��� ����ϱ�
CString XFormatNumber(__int64 number);

// ����Ʈ������ ũ��� ���� KB������ ũ�� ���ڿ��� ��´�.
CString GetSizeString(__int64 nSize);

// ����Ʈ������ ũ��� ���� ����Ʈ ������ ũ�� ���ڿ��� ��´�.
CString GetSizeByteString(__int64 nSize);

// ����Ʈ������ ũ��� ���� MByte ������ ũ�� ���ڿ��� ��´�.
CString GetMByteString(__int64 nSize);

// InfMByte ���� ũ�� MB ������ SupByte ���� ������ ����Ʈ ������ ũ�⸦ ���ڿ��� �ǵ�����.
CString MyGetSizeString(INT64 nSize, int InfMByte = 20*1024*1024, int SupByte = 9*1024*1024);

// Ư�� ���ڵ��� �����ϸ鼭 ���ڿ��κ��� int �� ������ ��� �Լ�
int GetValue(LPCTSTR p_str, LPCTSTR p_ExceptChars);

// ���ҽ� �����ϱ�
CString  GetString(UINT nID);

// �������ڿ��κ��� ���� ���� ��´�. 99.99.99.99
BOOL GetVerValue(LPCTSTR p_strVer, OUT BYTE* ver);

// �������ڿ��κ��� DWORD ���� ���� ��´�.
DWORD GetVerValue(LPCTSTR p_strVer);

// ���� �����κ��� ���� ���ڿ��� ��´�.
CString GetVerString(DWORD p_Ver);

// ���� ���� �ڷḦ �÷������� 2008.11.18
CString GetWebPath(LPCTSTR path, LPCTSTR splitChar = NULL, BOOL withBSlash = FALSE);

// �ð��� ���� ���ڿ��� ����ϴ�.
CString GetDateString(UINT64 p_Time);

//  ü������� �뺸���� ���.
BOOL GetSystemErrorMessage(int nErr, OUT wchar_t *errMsg, int nMaxLen);
