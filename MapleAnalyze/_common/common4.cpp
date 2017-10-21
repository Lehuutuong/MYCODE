/////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2009 PSJDC.
//
//						common4.cpp	
//
//	This file is created [2/4/2009 by LMC]
//	comment is written in Korean.
//
//	Main purpose
//
//	Implementation of general purpose functions.
//	
//	
//
//	Modified History:
//
//		
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "stdlib.h"
#include "stdio.h"
#include "shlobj.h"
#include "io.h"
#include "define.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 
// 파일의 완전경로에서 이름을 얻습니다.
// 
CString GetFileName(IN	LPCTSTR p_strFilePath )
{
	int		w_nPos;
	CString	w_strRet, strFile = p_strFilePath;

	w_nPos = strFile.ReverseFind(_T('\\'));
	if (w_nPos < 0)
	{
		w_nPos = strFile.ReverseFind(_T('/'));
		if (w_nPos < 0)
			return _T("");
	}

	w_strRet = strFile.Right(strFile.GetLength() - w_nPos - 1);

	return w_strRet;
}

//------------------------------------------------------------------------
//		파일 경로 관련 함수들
//------------------------------------------------------------------------
// 완전경로에서 파일이름 얻기
void GetFileNameFromFullName(const WCHAR *szFullName, WCHAR *szFileName)
{
	WCHAR	szFileExt[MAX_PATH];

	_wsplitpath(szFullName, NULL, NULL, szFileName, szFileExt);
	wcscat(szFileName, szFileExt);
}
// 완전경로에서 폴더경로 얻기
void GetPathNameFromFullName(const WCHAR *szFullName, WCHAR *szPathName)
{
	WCHAR	szDir[MAX_PATH];
	_wsplitpath(szFullName, szPathName, szDir, NULL, NULL);
	wcscat(szPathName, szDir);
}
// 모듈 폴더경로 얻기
void GetModulePathName(WCHAR *szPathName)
{
	WCHAR szPath[1024];
	GetModuleFileNameW(GetModuleHandle(NULL),szPath,sizeof(szPath));

	GetPathNameFromFullName(szPath, szPathName);
}



// 
// 본문파일을 유니코드 형식으로 읽어 들입니다.
//
/* 
CString ReadTextByUnicode(LPCTSTR p_strFilepath)
{
	BYTE	w_header[2];
	FILE*	w_pFile		= 0;
	WCHAR*	w_lpwszBuf	= 0;
	int		w_nCchSize;
	CString strText;

	//
	//. 파일을 연댜.
	//
	if (_wfopen_s(&w_pFile, p_strFilepath, _T("rb")))
	{
		goto L_EXIT;
	}

	// 
	// 파일의 유니코드 문자로서의 크기를 얻는다.
	// 
	fseek(w_pFile, 0, SEEK_END);
	w_nCchSize = ftell(w_pFile) / sizeof(WCHAR);
	fseek(w_pFile, 0, SEEK_SET);

	//
	// 첫 두바이트를 읽어서 유니코드 머리부가 있는가를 검사한다.
	// 만일 있으면 SKIP한다. 없으면 처음부터 유니코드 문자렬자료라고 생각한다.
	//
	fread(w_header, 1, 2, w_pFile);
	if (w_header[0] != 0xFF || w_header[1] != 0xFE)
	{
		//
		// 유니코드 머리부는 없다. 파일지적자를 다시 처음으로 가져간다.
		//
		fseek(w_pFile, 0, SEEK_SET);
	}
	else
	{
		w_nCchSize--;
	}

	// 
	// 완충기 할당
	// 
	w_lpwszBuf = new WCHAR[w_nCchSize + 1];
	if( !w_lpwszBuf )
	{
		goto L_EXIT;
	}

	// 
	// 파일의 전체 자료 읽기
	// 
	fread(w_lpwszBuf, sizeof(WCHAR), w_nCchSize, w_pFile);
	w_lpwszBuf[w_nCchSize] = 0;

	// 
	// 본문자료를 CString형식으로 변환한다.
	// 
	strText = w_lpwszBuf;

L_EXIT:

	// 
	// 림시 완중기를 해방한다.
	// 
	if(w_lpwszBuf)
	{
		delete []w_lpwszBuf;
		w_lpwszBuf = 0;
	}

	// 
	// 파일을 닫는다.
	// 
	if(w_pFile)
	{
		fclose(w_pFile);
		w_pFile = 0;
	}

	return strText;
}

BOOL SaveTextByUnicode(LPCTSTR p_strFilepath, LPCTSTR p_strText)
{
	BYTE	w_header[2] = {0xFF, 0xFE};
	FILE*	w_pFile		= 0;
	int		w_nCchSize;
	BOOL	w_bRet = FALSE;

	//
	//. 파일을 연댜.
	//
	if (_wfopen_s(&w_pFile, p_strFilepath, _T("wb")))
	{
		goto L_EXIT;
	}

	//
	// 유니코드 머리부를 쓴다.
	//
	if( fwrite(w_header, 1, 2, w_pFile) != 2)
		goto L_EXIT;

	// 
	// 파일의 전체 자료 읽기
	// 
	w_nCchSize = (int)wcslen(p_strText);
	if( fwrite(p_strText, sizeof(WCHAR), w_nCchSize, w_pFile) != w_nCchSize)
		goto L_EXIT;

	w_bRet = TRUE;

L_EXIT:

	// 
	// 파일을 닫는다.
	// 
	if(w_pFile)
	{
		fclose(w_pFile);
		w_pFile = 0;
	}

	return w_bRet;
}
*/
void SplitString(CString &strText, LPCTSTR strSplit, CStringArray &arraySplited)
{
	int pos_e = 0, pos_s = 0;
	CString strPart;
	arraySplited.RemoveAll();

	while((pos_e = strText.Find(strSplit, pos_s)) >= 0)
	{
		strPart = strText.Mid(pos_s, pos_e - pos_s);
		arraySplited.Add(strPart);

		pos_s = pos_e + (int)wcslen(strSplit);
	}
	if (pos_s < strText.GetLength())
	{
		strPart = strText.Mid(pos_s, strText.GetLength() - pos_s);
		arraySplited.Add(strPart);
	}

}
/* ===============================================================================================

함수설명:

    지정된 프로세스를 실행하고 끝날 때 까기 기다립니다.

파라메터:

	p_CommandLine : 례 “c:\windows\system32\notepad.exe c:\boot.ini”  

귀환값
	성공하면 TRUE, 실패하면 FALSE
=============================================================================================== */
BOOL ExecuteFile( LPCTSTR p_CommandLine )   
{  
	PROCESS_INFORMATION proc_info;  
	STARTUPINFO s_info;  
	memset(&s_info, 0, sizeof(s_info));  
	s_info.cb = sizeof(s_info);  
	s_info.dwFlags |= STARTF_USESHOWWINDOW;
	s_info.wShowWindow = SW_SHOW;  

	BOOL r = CreateProcess(	NULL, 
							(LPTSTR)p_CommandLine, 
							NULL, 
							NULL, 
							FALSE, 
							0 , 
							NULL, 
							NULL, 
							&s_info, 
							&proc_info);  
	if(r == FALSE)  
	{  
		return FALSE;  
	}  
	DWORD dwRet = WaitForSingleObject(proc_info.hThread, INFINITE);
	switch(dwRet)  
	{  
	case WAIT_TIMEOUT:  
		TerminateProcess(proc_info.hProcess, -1);  
		//MessageBox("WAIT_TIMEOUT");  
		return FALSE;  
		break;  
	case WAIT_ABANDONED:  
		//MessageBox("WAIT_ABANDONED");  
		return FALSE;  
		break;  
	case WAIT_OBJECT_0:  
		//MessageBox("WAIT_OBJECT_0");  
		break;  
	}  
	return TRUE;  
}  

void SortUintArray ( IN OUT CUIntArray &p_Array) 
{ 
	int indexOfMin; 
	int pass; 
	int j; 
	int size = (int)p_Array.GetSize(); 
	
	UINT temp; 

	for ( pass = 0; pass < size - 1; pass++ ) 
	{ 
		indexOfMin = pass; 

		for ( j = pass + 1; j < size; j++ ) 
			if ( p_Array[j] < p_Array[pass] ) 
				indexOfMin = j; 

		temp = p_Array[pass]; 
		p_Array[pass] = p_Array[indexOfMin]; 
		p_Array[indexOfMin] = temp; 
	} 
} 

/* ===============================================================================================

함수설명:

현재 프로세스이미지 파일이 있는 폴더의 경로를 반환합니다.

=============================================================================================== */
CString GetCurrentProcessDir()
{
	// 
	TCHAR path[MAX_PATH];
	TCHAR *pos;

	GetModuleFileName(NULL, path, MAX_PATH);
	pos = _tcsrchr(path, '\\');
	pos[0] = 0;

	return path;
}




/* ===============================================================================================

함수설명:	-----------------	FindFilesInFolder

폴더내 파일들을 찾습니다.
재귀적으로 호출됩니다.

=============================================================================================== */
/*
void FindFilesInFolder( IN CString p_TopFolder, OUT CStringArray &p_FileList, OUT UINT64 &p_FileSize )
{
	CFile fp;

	FILE *ffp;
	CString str;

	//일반 파일인가 확인..
	int ss  = _wfopen_s(&ffp, p_TopFolder, L"rb");
	if(ss)
	{
	}
	else 
	{
		UINT ffsize = 0;
		fseek(ffp,0,SEEK_END);ffsize = ftell(ffp);
		fclose(ffp);
		p_FileList.Add(p_TopFolder);
		p_FileSize+=ffsize;
		return;
	}


	//폴더라고 보고 파일목록 찾기.
	CFileFind finder;

	CString strWildcard(p_TopFolder);
	strWildcard += _T("\\*.*");

	BOOL bWorking = finder.FindFile(strWildcard);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		// 
		// ".", ".."이면 무시
		if (finder.IsDots())
			continue;

		CString strPath = finder.GetFilePath();
		if (finder.IsDirectory())
		{
			// 
			// 폴더이면 재귀적으로 함수를 호출
			FindFilesInFolder(strPath, p_FileList, p_FileSize);
		}
		else
		{
			p_FileList.Add(strPath);
			p_FileSize+=finder.GetLength();	
		}
	}
	finder.Close();
}
*/
CString ConvertWideChar(char * src)
{
	CString w_ret = L"" ;
	WCHAR * str = new WCHAR[(strlen(src)+1)*2];

	MultiByteToWideChar(CP_ACP, 0, src, strlen(src)+1,str,(strlen(src)+1)*2);
	w_ret = str;
	delete str;
	return w_ret;
}


///////////////////////
BOOL IsValidIdPwd(LPCTSTR id, LPCTSTR pwd, int level, BOOL user)
{
	if(wcslen(id) > 20 || wcslen(id) < 4)
	{
		AfxMessageBox(L"아이디는 4 ~ 20문자이여야 합니다.");
		return FALSE;
	}
	if(wcslen(pwd) > 20 || wcslen(pwd) < 4)
	{
		AfxMessageBox(L"패스워드는 4 ~ 20문자이여야 합니다.");
		return FALSE;
	}
	if(user)
	{
		if(level < 1 || level > 365)
		{
			AfxMessageBox(L"인증일자가 잘못되였습니다. 1~365일사이로 줄수 있습니다.");
			return FALSE;
		}

	}
	else
	{
		if(level < ADMIN_USER || level > ADMIN_MANAGEMANAGER)
		{
			AfxMessageBox(L"관리권한이 잘못되였습니다.");
			return FALSE;
		}

	}
	return TRUE;

}