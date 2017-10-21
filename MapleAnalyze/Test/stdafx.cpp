// stdafx.cpp : source file that includes just the standard includes
// Test.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


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


BOOL	WriteUIFileLog(WCHAR *szFormat, ...)
{
	FILE	*fp;
	char	*aszLog;
	WCHAR	szLog[1024];
	va_list	arg;

	va_start(arg, szFormat);
	vswprintf(szLog, szFormat, arg);
	va_end(arg);
	aszLog = new char[wcslen(szLog)*2+20];
	fp = _wfopen(L"e:\\enginelog.txt", L"a+");
	if(!fp) return FALSE;

	WideCharToMultiByte(CP_ACP, 0, szLog, wcslen(szLog)+1, aszLog+13, wcslen(szLog)*2+6, 0, 0);
	strcat(aszLog, "\n");
	fprintf(fp, aszLog);
	fclose(fp);
	delete [] aszLog;

	return TRUE;
}