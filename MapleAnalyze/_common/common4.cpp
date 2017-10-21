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
// ������ ������ο��� �̸��� ����ϴ�.
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
//		���� ��� ���� �Լ���
//------------------------------------------------------------------------
// ������ο��� �����̸� ���
void GetFileNameFromFullName(const WCHAR *szFullName, WCHAR *szFileName)
{
	WCHAR	szFileExt[MAX_PATH];

	_wsplitpath(szFullName, NULL, NULL, szFileName, szFileExt);
	wcscat(szFileName, szFileExt);
}
// ������ο��� ������� ���
void GetPathNameFromFullName(const WCHAR *szFullName, WCHAR *szPathName)
{
	WCHAR	szDir[MAX_PATH];
	_wsplitpath(szFullName, szPathName, szDir, NULL, NULL);
	wcscat(szPathName, szDir);
}
// ��� ������� ���
void GetModulePathName(WCHAR *szPathName)
{
	WCHAR szPath[1024];
	GetModuleFileNameW(GetModuleHandle(NULL),szPath,sizeof(szPath));

	GetPathNameFromFullName(szPath, szPathName);
}



// 
// ���������� �����ڵ� �������� �о� ���Դϴ�.
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
	//. ������ ����.
	//
	if (_wfopen_s(&w_pFile, p_strFilepath, _T("rb")))
	{
		goto L_EXIT;
	}

	// 
	// ������ �����ڵ� ���ڷμ��� ũ�⸦ ��´�.
	// 
	fseek(w_pFile, 0, SEEK_END);
	w_nCchSize = ftell(w_pFile) / sizeof(WCHAR);
	fseek(w_pFile, 0, SEEK_SET);

	//
	// ù �ι���Ʈ�� �о �����ڵ� �Ӹ��ΰ� �ִ°��� �˻��Ѵ�.
	// ���� ������ SKIP�Ѵ�. ������ ó������ �����ڵ� ���ڷ��ڷ��� �����Ѵ�.
	//
	fread(w_header, 1, 2, w_pFile);
	if (w_header[0] != 0xFF || w_header[1] != 0xFE)
	{
		//
		// �����ڵ� �Ӹ��δ� ����. ���������ڸ� �ٽ� ó������ ��������.
		//
		fseek(w_pFile, 0, SEEK_SET);
	}
	else
	{
		w_nCchSize--;
	}

	// 
	// ����� �Ҵ�
	// 
	w_lpwszBuf = new WCHAR[w_nCchSize + 1];
	if( !w_lpwszBuf )
	{
		goto L_EXIT;
	}

	// 
	// ������ ��ü �ڷ� �б�
	// 
	fread(w_lpwszBuf, sizeof(WCHAR), w_nCchSize, w_pFile);
	w_lpwszBuf[w_nCchSize] = 0;

	// 
	// �����ڷḦ CString�������� ��ȯ�Ѵ�.
	// 
	strText = w_lpwszBuf;

L_EXIT:

	// 
	// ���� ���߱⸦ �ع��Ѵ�.
	// 
	if(w_lpwszBuf)
	{
		delete []w_lpwszBuf;
		w_lpwszBuf = 0;
	}

	// 
	// ������ �ݴ´�.
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
	//. ������ ����.
	//
	if (_wfopen_s(&w_pFile, p_strFilepath, _T("wb")))
	{
		goto L_EXIT;
	}

	//
	// �����ڵ� �Ӹ��θ� ����.
	//
	if( fwrite(w_header, 1, 2, w_pFile) != 2)
		goto L_EXIT;

	// 
	// ������ ��ü �ڷ� �б�
	// 
	w_nCchSize = (int)wcslen(p_strText);
	if( fwrite(p_strText, sizeof(WCHAR), w_nCchSize, w_pFile) != w_nCchSize)
		goto L_EXIT;

	w_bRet = TRUE;

L_EXIT:

	// 
	// ������ �ݴ´�.
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

�Լ�����:

    ������ ���μ����� �����ϰ� ���� �� ��� ��ٸ��ϴ�.

�Ķ����:

	p_CommandLine : �� ��c:\windows\system32\notepad.exe c:\boot.ini��  

��ȯ��
	�����ϸ� TRUE, �����ϸ� FALSE
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

�Լ�����:

���� ���μ����̹��� ������ �ִ� ������ ��θ� ��ȯ�մϴ�.

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

�Լ�����:	-----------------	FindFilesInFolder

������ ���ϵ��� ã���ϴ�.
��������� ȣ��˴ϴ�.

=============================================================================================== */
/*
void FindFilesInFolder( IN CString p_TopFolder, OUT CStringArray &p_FileList, OUT UINT64 &p_FileSize )
{
	CFile fp;

	FILE *ffp;
	CString str;

	//�Ϲ� �����ΰ� Ȯ��..
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


	//������� ���� ���ϸ�� ã��.
	CFileFind finder;

	CString strWildcard(p_TopFolder);
	strWildcard += _T("\\*.*");

	BOOL bWorking = finder.FindFile(strWildcard);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		// 
		// ".", ".."�̸� ����
		if (finder.IsDots())
			continue;

		CString strPath = finder.GetFilePath();
		if (finder.IsDirectory())
		{
			// 
			// �����̸� ��������� �Լ��� ȣ��
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
		AfxMessageBox(L"���̵�� 4 ~ 20�����̿��� �մϴ�.");
		return FALSE;
	}
	if(wcslen(pwd) > 20 || wcslen(pwd) < 4)
	{
		AfxMessageBox(L"�н������ 4 ~ 20�����̿��� �մϴ�.");
		return FALSE;
	}
	if(user)
	{
		if(level < 1 || level > 365)
		{
			AfxMessageBox(L"�������ڰ� �߸��ǿ����ϴ�. 1~365�ϻ��̷� �ټ� �ֽ��ϴ�.");
			return FALSE;
		}

	}
	else
	{
		if(level < ADMIN_USER || level > ADMIN_MANAGEMANAGER)
		{
			AfxMessageBox(L"���������� �߸��ǿ����ϴ�.");
			return FALSE;
		}

	}
	return TRUE;

}