// stdafx.cpp : source file that includes just the standard includes
//	MIPSPI.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

void PrintLog(const char *format,...)
{
	//return;
	//return;
	char logData1[1024];
	va_list arglist;
	va_start(arglist, format);
	_vsnprintf(logData1, 1024, format, arglist);
	OutputDebugString( logData1 );
	va_end(arglist);
}


BOOL	WriteFileLog(char *szFormat, ...)
{
	FILE	*fp;
	char	*aszLog;
	char	szLog[1024];
	va_list	arg;

	va_start(arg, szFormat);
	vsprintf(szLog, szFormat, arg);
	va_end(arg);
	aszLog = new char[strlen(szLog)*2+10];
	fp = fopen("e:\\enginelog.txt", "a+");
	if(!fp) return FALSE;
	//sprintf(aszLog, "%8dms - ", GetTickCount()-g_dwTick);
	//WideCharToMultiByte(CP_ACP, 0, szLog, wcslen(szLog)+1, aszLog, wcslen(szLog)*2+6, 0, 0);
	strcpy(aszLog, szLog);
	strcat(aszLog, "\n");
	fprintf(fp, aszLog);
	fclose(fp);
	delete [] aszLog;

	return TRUE;
}