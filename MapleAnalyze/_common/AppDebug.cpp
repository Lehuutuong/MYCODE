/////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2007 PSJDC.
//
//						AppDebug.cpp	
//
//	This file is created [8/23/2007 by LMC]
//	comment is written in Korean.
//
//	Main purpose
//
//	�� 3�� �������α׶� debug�� ���� ����Դϴ�.
//
//	Modified History:
//
//		
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "windows.h"
//#include "strsafe.h"
#include <stdio.h>
//#include <stdarg.h>

#include "AppDebug.h"

int g_nDbgLevel = 4;

#ifndef REMOVE_DBG_STR
/* ===============================================================================================

â���ð� :	2008�� 12�� 10��

�Լ�����:	

    ���ٱ� ������ ����ϴ� �Լ�.
	��������� ������ �ټ� �ִ�.

�Ķ����:

	p_lpFormat 	: ����� ���ٱ� ���� (...)

���� ��:

    ���� ����.

=============================================================================================== */
#define MAX_DBG_STR_LEN	1024
void DebugPrint(const wchar_t*	p_lpFormat, ...)
{
	if (g_nDbgLevel == 0)
	{
		return;
	}

	va_list w_pVars;
	CString strMsg;

	va_start(w_pVars, p_lpFormat);

	wchar_t	w_szBuffer[MAX_DBG_STR_LEN + 1];
	wchar_t	w_szLrCf[3];

	w_szLrCf[0] = 0xD; w_szLrCf[1] = 0xA;w_szLrCf[2] = 0;
	ZeroMemory(w_szBuffer, MAX_DBG_STR_LEN * sizeof(wchar_t));

	vswprintf_s(w_szBuffer, MAX_DBG_STR_LEN + 1, p_lpFormat, w_pVars);
	wcscat_s(w_szBuffer, MAX_DBG_STR_LEN + 1, w_szLrCf);

  	strMsg.Format(L"Fifa : %s", w_szBuffer);
  	OutputDebugStringW(strMsg);

	va_end(w_pVars);
}

void DebugPrint(const char*	p_lpFormat, ...)
{
	if (g_nDbgLevel == 0)
	{
		return;
	}

	va_list w_pVars;
	
	va_start(w_pVars, p_lpFormat);
	
	char	w_szBuffer[MAX_DBG_STR_LEN + 1];
	char	w_szLrCf[3];
	
	w_szLrCf[0] = 0xD; w_szLrCf[1] = 0xA;w_szLrCf[2] = 0;
	ZeroMemory(w_szBuffer, MAX_DBG_STR_LEN);
	
	vsprintf_s(w_szBuffer, MAX_DBG_STR_LEN + 1, p_lpFormat, w_pVars);
	strcat_s(w_szBuffer, MAX_DBG_STR_LEN + 1, w_szLrCf);
	
	OutputDebugStringA((LPCSTR)w_szBuffer);
	
	va_end(w_pVars);
}
#endif

CString ReadTextByUnicode(LPCTSTR p_strFilepath);
void SplitString(CString &strText, LPCTSTR strSplit, CStringArray &arraySplited);
void LoadDebugSetting()
{
	WCHAR bufPath[MAX_PATH];
	BOOL bRet = FALSE;
	CString strText;
	CStringArray lines;

	// ���μ����̹��� ������ ������ ��´�.
	GetModuleFileName(NULL, bufPath, MAX_PATH);
	WCHAR *p = wcsrchr(bufPath, '\\');
	if (p == NULL)
	{
		return;
	}

	*p = 0;
	wcscat_s(bufPath, MAX_PATH, L"\\dbg.ini");

	if (GetFileAttributes(bufPath) == INVALID_FILE_ATTRIBUTES)
	{
		return;
	}

	strText = ReadTextByUnicode(bufPath);
	SplitString(strText, L"\r\n", lines);

	for (int i = 0;i < lines.GetCount();i++)
	{
		if (lines[i] == L"DBG_ERR")
		{
			g_nDbgLevel = 1;
		}
		else if (lines[i] == L"DBG_WARN")
		{
			g_nDbgLevel = 2;
		}
		else if (lines[i] == L"DBG_TRACE")
		{
			g_nDbgLevel = 3;
		}
		else if (lines[i] == L"DBG_INFO")
		{
			g_nDbgLevel = 4;
		}
		else if (lines[i] == L"DBG_VERB")
		{
			g_nDbgLevel = 5;
		}
	}

}