#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "log.h"

void PrintError(DWORD dwErrorNo)
{
	LPVOID lpMsgBuf;
	if (!FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwErrorNo,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),// Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL ))
	{
		// Handle the error.
		return;
	}

	OutputDebugString((LPCTSTR)lpMsgBuf);
	// Free the buffer.
	LocalFree( lpMsgBuf );
}

void PrintLog(const WCHAR *format,...)
{
	WCHAR logData1[1024];
	va_list arglist;
	va_start(arglist, format);
	_vsnwprintf(logData1, 1024, format, arglist);
	OutputDebugStringW(logData1);
	va_end(arglist);
}

void PrintLogA(const char *format,...)
{
	char logData1[1024];
	va_list arglist;
	va_start(arglist, format);
	_vsnprintf(logData1, 1024, format, arglist);
	OutputDebugStringA( logData1 );
	va_end(arglist);
}

void PrintBufToFile(BYTE * pBuf, int nSize, char * pszFileName)
{
	if ( pBuf == NULL ) return;
	if ( nSize == 0 ) return;

	int i;
	char * pszMsg = new char[nSize * 4];
	ZeroMemory(pszMsg, nSize * 4);

	char szTemp[5];
	for ( i = 0 ; i < nSize ; i ++ )
	{
		sprintf(szTemp, "%02x ", pBuf[i]);
		strcat(pszMsg, szTemp);
	}

	FILE * fp;
	if ( pszFileName == NULL )
	{
		fp = fopen("C:\\MPSRecv.log", "ab");		
	}
	else fp = fopen(pszFileName, "ab");

	fprintf(fp, "%s\x0d\x0a", pszMsg);
	fclose(fp);
	delete[] pszMsg;
}

void PrintHexToFile(BYTE * pBuf, int nSize, char * pszFileName)
{
	if ( pBuf == NULL ) return;
	if ( nSize == 0 ) return;

	FILE * fp = fopen(pszFileName, "ab");
	fwrite(pBuf, 1, nSize, fp);
	fclose(fp);
}

void PrintBuf(BYTE * pBuf, int nSize)
{
	if ( pBuf == NULL ) return;
	if ( nSize == 0 ) return;

	int i;
	WCHAR * pszMsg = new WCHAR[nSize * 4];
	ZeroMemory(pszMsg, nSize * 4);

	WCHAR szTemp[5];
	for ( i = 0 ; i < nSize ; i ++ )
	{
		swprintf(szTemp, L"%02x ", pBuf[i]);
		wcscat(pszMsg, szTemp);
	}

	OutputDebugStringW(pszMsg);
	delete[] pszMsg;
}

void WCHARToFile(WCHAR * pszStr, char * pszFileName)
{
	if ( pszStr == NULL || pszFileName == NULL ) return;

	char * pF = NULL;
	int nLen = WideCharToMultiByte(CP_ACP, 0, pszStr, -1, pF, 0, NULL, NULL);
	if ( nLen == 0 ) return;
	pF = (char *)malloc(nLen + 1);
	if ( pF == NULL ) return;
	pF[nLen] = 0;
	WideCharToMultiByte(CP_ACP, 0, pszStr, -1, pF, nLen, NULL, NULL);

	FILE * fp = fopen(pszFileName, "wb"); if ( fp == NULL ) return;
	fprintf(fp, "%s", pF);
	fclose(fp);
	free(pF);
}

char* WCharToChar(const wchar_t* pwstrSrc)
{
#if !defined _DEBUG
	int len = 0;
	len = (wcslen(pwstrSrc) + 1)*2;
	char* pstr      = (char*) malloc ( sizeof( char) * len);

	WideCharToMultiByte( 949, 0, pwstrSrc, -1, pstr, len, NULL, NULL);
#else

	int nLen = wcslen(pwstrSrc);

	char* pstr      = (char*) malloc ( sizeof( char) * nLen + 1);
	wcstombs(pstr, pwstrSrc, nLen+1);
#endif

	return pstr;
}