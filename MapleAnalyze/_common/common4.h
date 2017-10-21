/////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2009 PSJDC.
//
//						common4.h
//
//	This file is created [2/4/2009 by LMC]
//	comment is written in Korean.
//
//	Main purpose
//
//	Declear of functions in common4.h
//	
//	
//
//	Modified History:
//
//		
//
/////////////////////////////////////////////////////////////////////////////
#pragma once
//------------------------------------------------------------------------
//		���� ��� ���� �Լ���
//------------------------------------------------------------------------
// ������ο��� �����̸� ���
void GetFileNameFromFullName(const WCHAR *szFullName, WCHAR *szFileName);
// ������ο��� ������� ���
void GetPathNameFromFullName(const WCHAR *szFullName, WCHAR *szPathName);
// ��� ������� ���
void GetModulePathName(WCHAR *szPathName);

CString GetFileName(IN	LPCTSTR p_strFilePath );
//CString ReadTextByUnicode(LPCTSTR p_strFilepath);
//BOOL SaveTextByUnicode(LPCTSTR p_strFilepath, LPCTSTR p_strText);
void SplitString(CString &strText, LPCTSTR strSplit, CStringArray &arraySplited);

BOOL ExecuteFile( LPCTSTR p_CommandLine ) ;

void SortUintArray ( IN OUT CUIntArray &p_Array) ;
CString GetCurrentProcessDir();


//void FindFilesInFolder( IN CString p_TopFolder, OUT CStringArray &p_FileList, OUT UINT64 &p_FileSize );

CString ConvertWideChar(char * src);


BOOL IsValidIdPwd(LPCTSTR id, LPCTSTR pwd, int level, BOOL user = FALSE);
