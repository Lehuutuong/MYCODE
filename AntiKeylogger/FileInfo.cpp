// FileInfo.cpp: implementation of the CFileInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AntiKeylogger.h"
#include "Global.h"
#include "FileInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileInfo::CFileInfo(DWORD* pdwData)
{
	wcscpy(m_strInfo, L"Unknown module");
	m_pbState = (BYTE*)pdwData;
	char szPathName[MAX_PATH];
	
	wcscpy(m_strPathName, (LPWSTR)pdwData[1]);
	wcscpy(m_strFileName, wcsrchr(m_strPathName, '\\') + 1);
	wcstombs(szPathName, m_strPathName, MAX_PATH);
	m_hIcon = ExtractIcon(AfxGetInstanceHandle(), szPathName, 0);
	GetFileInfo();
}

CFileInfo::~CFileInfo()
{
	DestroyIcon(m_hIcon);
}

void CFileInfo::GetFileInfo()
{
	UINT dwLen = 0;
	PWORD pwBuffer;
	WCHAR wsInfo[200];
	
	dwLen = GetFileVersionInfoSizeW(m_strPathName, (DWORD*)&dwLen);
	BYTE* pbInfo = new BYTE[dwLen];
	if(GetFileVersionInfoW(m_strPathName, 0, dwLen, pbInfo)) {
		VerQueryValueW(pbInfo, L"\\VarFileInfo\\Translation", (LPVOID*)&pwBuffer, &dwLen);
		wsprintfW(wsInfo, L"\\StringFileInfo\\%04x%04x\\FileDescription", pwBuffer[0], pwBuffer[1]);
		VerQueryValueW(pbInfo, wsInfo, (LPVOID*)&pwBuffer, &dwLen);
		wcscpy(m_strInfo, pwBuffer);
		VerQueryValueW(pbInfo, L"\\VarFileInfo\\Translation", (LPVOID*)&pwBuffer, &dwLen);
		wsprintfW(wsInfo, L"\\StringFileInfo\\%04x%04x\\LegalCopyright", pwBuffer[0], pwBuffer[1]);
		VerQueryValueW(pbInfo, wsInfo, (LPVOID*)&pwBuffer, &dwLen);
		wcscat(m_strInfo, pwBuffer);
	}
	delete pbInfo;
}
