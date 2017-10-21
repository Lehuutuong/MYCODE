// _new.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include <shellapi.h>
#include <direct.h>
#include <stdio.h>
#include <psapi.h>
#include <malloc.h>

#include "_new.h"

//
// ������Ʈ�� �̸�.
//
char _szProjectName[MAX_PATH] = {0};
WCHAR _wzProjectName[MAX_PATH] = {0};

/* =============================================================================================== 
																			2009.10.21
�Լ�����:	-----------------	SetProjectName

	������Ʈ�� �̸��� �����Ѵ�.

=============================================================================================== */  
void SetProjectName(IN char* p_cName, IN WCHAR* p_wName)
{
	strcpy(_szProjectName, p_cName);
	wcscpy(_wzProjectName, p_wName);
}

#ifndef MyOutputDebugStringA
void
MyOutputDebugStringA(
					 IN	const CHAR*	p_lpFormat, ...
					 )
{
	va_list w_pVars;

	va_start(w_pVars, p_lpFormat);

	CHAR	Buffer[10000];
	CHAR	w_szBuffer[10000];
	CHAR	w_szLrCf[3];

	w_szLrCf[0] = 0xD; w_szLrCf[1] = 0xA;w_szLrCf[2] = 0;
	ZeroMemory(w_szBuffer, 10000 * sizeof(CHAR));

	vsprintf(w_szBuffer, p_lpFormat, w_pVars);
	strcat(w_szBuffer, w_szLrCf);


	sprintf(Buffer,  "%s:[%d] %s", _szProjectName, GetCurrentThreadId(), w_szBuffer);

	OutputDebugStringA(Buffer/*w_szBuffer*/);

	va_end(w_pVars);

}

void
MyOutputDebugStringW(
					 IN	const WCHAR*	p_lpFormat, ...
					 )
{
	va_list w_pVars;

	va_start(w_pVars, p_lpFormat);

	WCHAR	Buffer[10000];
	WCHAR	w_szBuffer[10000];
	WCHAR	w_szLrCf[3];

	w_szLrCf[0] = 0xD; w_szLrCf[1] = 0xA;w_szLrCf[2] = 0;
	ZeroMemory(w_szBuffer, 10000 * sizeof(WCHAR));

	vswprintf(w_szBuffer, p_lpFormat, w_pVars);
	wcscat(w_szBuffer, w_szLrCf);


	wsprintf(Buffer, L"%s:[%d] %s", _wzProjectName, GetCurrentThreadId(), w_szBuffer);

	OutputDebugStringW(Buffer);

	va_end(w_pVars);

}


//------------------------------------------------------------------------------
//		������ ���� �������� �Բ� ����Ѵ�.
//------------------------------------------------------------------------------
void
MyOutputDebugStringA_Err(
						 IN	const CHAR*	p_lpFormat, ...
						 )
{
	va_list w_pVars;

	va_start(w_pVars, p_lpFormat);

	CHAR	Buffer[10000];
	CHAR	w_szBuffer[10000];
	CHAR	w_szLrCf[3];

	w_szLrCf[0] = 0xD; w_szLrCf[1] = 0xA;w_szLrCf[2] = 0;
	ZeroMemory(w_szBuffer, 10000 * sizeof(CHAR));

	vsprintf(w_szBuffer, p_lpFormat, w_pVars);
	strcat(w_szBuffer, w_szLrCf);


	sprintf(Buffer,  "%s:[%d][err=%d] %s", _szProjectName, GetCurrentThreadId(), GetLastError(), w_szBuffer);

	OutputDebugStringA(Buffer/*w_szBuffer*/);

	va_end(w_pVars);

}

void
MyOutputDebugStringW_Err(
						 IN	const WCHAR*	p_lpFormat, ...
						 )
{
	va_list w_pVars;

	va_start(w_pVars, p_lpFormat);

	WCHAR	Buffer[10000];
	WCHAR	w_szBuffer[10000];
	WCHAR	w_szLrCf[3];

	w_szLrCf[0] = 0xD; w_szLrCf[1] = 0xA;w_szLrCf[2] = 0;
	ZeroMemory(w_szBuffer, 10000 * sizeof(WCHAR));

	vswprintf(w_szBuffer, p_lpFormat, w_pVars);
	wcscat(w_szBuffer, w_szLrCf);


	wsprintf(Buffer, L"%s:[%d][err=%d] %s", _wzProjectName, GetCurrentThreadId(), GetLastError(), w_szBuffer);

	OutputDebugStringW(Buffer);

	va_end(w_pVars);

}

#endif


/* =============================================================================================== 

�Լ�����:	-----------------	ExecuteProgram

	���μ����� �⵿��Ű�� ������ �ð����� ���μ����� �����⸦ ��ٸ���.

	�����ð��� 0�� �ƴϸ� �����ð� ���� ���μ����� ������ ��� �������̴�.

�Ķ����:

	 wait_ms : �̸��� ������ ��ٸ� �ð��� �ش�.

=============================================================================================== */
BOOL ExecuteProgramW( LPCWSTR FileName, LPCWSTR Params, OUT HANDLE* p_hProcess, DWORD wait_ms, BOOL p_bHide)
{
	SHELLEXECUTEINFOW execinfo;

	// ������ ���� ����ü ��Ʈ
	ZeroMemory( &execinfo, sizeof(execinfo) );
	execinfo.cbSize = sizeof(execinfo);
	execinfo.lpVerb = L"open";
	execinfo.lpFile = FileName;
	execinfo.lpParameters = Params;
	// ���� �Ⱥ��̰� ��������
	execinfo.fMask = SEE_MASK_NOCLOSEPROCESS	// hProcess �� �ڵ�ޱ�.
 		| SEE_MASK_FLAG_NO_UI					// �����뺸 �Ⱥ����ֱ�.
 		| SEE_MASK_NO_CONSOLE					// ���� â �Ⱥ����ֱ�.
		;

	if(p_bHide)
		execinfo.nShow = SW_HIDE;	// â�� ���߸� �����ϱ� 
	else
		execinfo.nShow = SW_SHOWDEFAULT;	// â�� �����ָ� �����ϱ�;

	MyOutputDebugStringW(L"ExecuteProgramW >>> execinfo.cbSize = %X lpFile = %s Params = %s", execinfo.cbSize, execinfo.lpFile, execinfo.lpParameters);
	// ���α׷��� �����Ѵ�.
	int r = (int)ShellExecuteExW( &execinfo );
	if( r == 0 ) return( FALSE );

	if(p_hProcess) *p_hProcess = execinfo.hProcess;

	// ���� �ð���ŭ
	// ������ ����� ������ ��ٸ���.
	if( wait_ms != 0 )
	{
		if( WAIT_TIMEOUT == WaitForSingleObject(execinfo.hProcess, wait_ms) )
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL ExecuteProgramA( LPCSTR FileName, LPCSTR Params, OUT HANDLE* p_hProcess, DWORD wait_ms, BOOL p_bHide)
{
	SHELLEXECUTEINFOA execinfo;

	// ������ ���� ����ü ��Ʈ
	ZeroMemory( &execinfo, sizeof(execinfo) );
	execinfo.cbSize = sizeof(execinfo);
	execinfo.lpVerb = "open";
	execinfo.lpFile = FileName;
	execinfo.lpParameters = Params;
	// ���� �Ⱥ��̰� ��������
	execinfo.fMask = SEE_MASK_NOCLOSEPROCESS	// hProcess �� �ڵ�ޱ�.
		| SEE_MASK_FLAG_NO_UI					// �����뺸 �Ⱥ����ֱ�.
		| SEE_MASK_NO_CONSOLE					// ���� â �Ⱥ����ֱ�.
		;

	if(p_bHide)
		execinfo.nShow = SW_HIDE;	// â�� ���߸� �����ϱ� 
	else
		execinfo.nShow = SW_SHOWDEFAULT;	// â�� �����ָ� �����ϱ�;

	// ���α׷��� �����Ѵ�.
	int r = (int)ShellExecuteExA( &execinfo );
	if( r == 0 ) return( FALSE );

	if(p_hProcess) *p_hProcess = execinfo.hProcess;

	// ���� �ð���ŭ
	// ������ ����� ������ ��ٸ���.
	if( wait_ms != 0 )
	{
		if( WAIT_TIMEOUT == WaitForSingleObject(execinfo.hProcess, wait_ms) )
		{
			return FALSE;
		}
	}
	return TRUE;
}

/* =============================================================================================== 

�Լ�����:	-----------------	GetExePath

	���α׷��� ���̳ʸ� ������ ��´�.

=============================================================================================== */
BOOL GetExePathW(OUT WCHAR *p_strExePath)
{
	wchar_t strCurPath[MAX_PATH];
	GetModuleFileNameW(NULL, strCurPath, MAX_PATH);
	wchar_t *pC = strCurPath + wcslen(strCurPath);
	while(*pC != '\\' && pC > strCurPath)
	{
		pC--;
	}
	pC[1] = 0;
	
	wcscpy(p_strExePath, strCurPath);
	return TRUE;

}

BOOL GetExePathA(OUT CHAR *p_strExePath)
{
	char strCurPath[MAX_PATH];
	GetModuleFileNameA(NULL, strCurPath, MAX_PATH);
	char *pC = strCurPath + strlen(strCurPath);
	while(*pC != '\\' && pC > strCurPath)
	{
		pC--;
	}
	pC[1] = 0;

	strcpy(p_strExePath, strCurPath);
	return TRUE;

}

/* =============================================================================================== 
																			2009.10.13
�Լ�����:	-----------------	DumpFileW

	�޸��ڷḦ ���Ͽ� �����ϱ�.

=============================================================================================== */  
BOOL DumpFileW(IN WCHAR* path_body, void* data, int len, OUT WCHAR* p_full_path, int p_max_len)
{
	if(p_full_path)
		p_full_path[0] = 0;

	WCHAR path[300];
	_mkdir("d:\\dump\\");

	SYSTEMTIME st;
	GetLocalTime(&st);

	swprintf(path, L"d:\\dump\\%s_%04d%02d%02d_%02d%02d%02d_%X_%X.bin"
		, path_body, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, (DWORD_PTR)data, len);
	FILE* pf = _wfopen(path, L"wb");
	if(pf)
	{
		fwrite(data, 1, len,  pf);

		fclose(pf);		pf = NULL;

		if(p_full_path)
			wcscpy_s(p_full_path, p_max_len, path);
		return TRUE;
	}
	
	return FALSE;

}

/* =============================================================================================== 
																			2009.10.18
�Լ�����:	-----------------	DumpFileWithPath

	���� ��ο� �޸� �ڷḦ �����ϱ�.

=============================================================================================== */  
BOOL DumpFileWithPath(IN WCHAR* p_full_path, IN void* data, int len)
{
	OutputDebugStringW(L">>> DumpFileWithPath ");
	if(p_full_path == NULL || p_full_path[0] == 0)
		return FALSE;

	FILE* pf = _wfopen(p_full_path, L"wb");

	if(pf)
	{
		fwrite(data, 1, len,  pf);

		fclose(pf);		pf = NULL;

		OutputDebugStringW(L"DumpFileWithPath >> OK");
		return TRUE;
	}
	OutputDebugStringW(L"DumpFileWithPath >> Fail");

	return FALSE;

}

/* =============================================================================================== 
																			2009.10.4
�Լ�����:	-----------------	DumpFileByProcessID

	���μ��� ���̵� �˰� �� ���μ��� �⺻����� �����ϱ�

=============================================================================================== */  
BOOL DumpFileByProcessID(DWORD ProcessID, IN WCHAR* p_DumpPath)
{
	BOOL r;
	HANDLE hProcess;
	DWORD DesiredAccess = PROCESS_VM_READ | /*PROCESS_VM_OPERATION | */PROCESS_QUERY_INFORMATION;

	hProcess = OpenProcess(DesiredAccess, FALSE, ProcessID);

	if(hProcess == NULL)
	{
		MyOutputDebugStringA("OpenProcess ���� ID = %d, DesiredAccess = %X, ErrCode = %d", ProcessID, DesiredAccess, GetLastError());
		return FALSE;
	}

	//------------------------------------------------------------------------------
	//		���μ����� ������ ��´�. 
	//------------------------------------------------------------------------------
	HMODULE hModList[100];
	DWORD nMod = 0;

	// ���μ����� �⺻ ��� ���� ���.
	EnumProcessModules(hProcess, hModList, 400, &nMod);

	MODULEINFO mi;
	GetModuleInformation(hProcess, hModList[0], &mi, sizeof(mi));

	void * BaseAddr = mi.lpBaseOfDll;
	int nLen = mi.SizeOfImage;
	DWORD dwReadLen = 0;
	BYTE* buf = new BYTE[nLen];

	// ���μ����� �޸� �б�
	r = ReadProcessMemory(hProcess, BaseAddr, buf, nLen, &dwReadLen);

	CloseHandle(hProcess);

	if(r == FALSE && dwReadLen == 0)
	{
		delete buf;
		MyOutputDebugStringA("ReadProcessMemory ���� ID = %d ErrCode = %d", ProcessID, GetLastError());
		return FALSE;
	}

	// ���μ��� �̸����� �����ϱ�.
	WCHAR *ProcessName = L"GameMon.des";//GetProcessNameById(ProcessID);

	if(p_DumpPath == NULL || p_DumpPath[0] == 0)
	{
		DumpFileW(ProcessName, buf, dwReadLen, NULL, 0);
		MyOutputDebugStringW(L"\n\n10.18 DumpFileW ȣ�� ErrCode= %d ID=%d �̸� = %s \n\n", GetLastError(), ProcessID, ProcessName);
	}
	else
	{
		DumpFileWithPath(p_DumpPath, buf, dwReadLen);
		MyOutputDebugStringW(L"\n\n10.18 DumpFileWithPath ȣ�� ErrCode= %d ID=%d dwReadLen = %X �̸� = %s \n\n", GetLastError(), ProcessID, dwReadLen, p_DumpPath);
	}

	delete buf;

	return TRUE;
}



//
// 16 ����Ʈ�� ����Ѵ�.
//
void DbgPrintGuard (void* p_data, char* comment)
{
	if(comment == NULL)
		comment = "";
	DWORD *p_outGuardData = (DWORD*)p_data;
	MyOutputDebugStringA("=[G]= %s:\t0x%X 0x%X 0x%X 0x%X"
		, comment, p_outGuardData[0], p_outGuardData[1], p_outGuardData[2], p_outGuardData[3]);

// 	if(comment == NULL)
// 		comment = "";
// 	BYTE* pb = (BYTE*)p_data;
// 	CHAR stBuf[MAX_PATH];
// 	sprintf(stBuf, "=[G]= %s:\t0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X"
// 		, comment, pb[0], pb[1], pb[2], pb[3], pb[4], pb[5], pb[6], pb[7], pb[8], pb[9], pb[10], pb[11], pb[12], pb[13], pb[14], pb[15]);
// 	MyOutputDebugStringA(stBuf);

}



/* =============================================================================================== 
																			2009.8.14
�Լ�����:	-----------------	ReadFileToBuffer

	������ ��ü �ڷḦ ����⿡ ���� �о���̱�.

=============================================================================================== */  
BOOL ReadWholeFileToBuffer(LPCTSTR strFilePath, OUT BYTE** p_ppBuffer, OUT int* p_nSize)
{
	HANDLE hFile = CreateFileW(strFilePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		// ���� ���⿡ ����.
		return FALSE;
	}

	LARGE_INTEGER lg;
	DWORD nSize = 0, nRead = 0;
	if(GetFileSizeEx(hFile, &lg) == TRUE)
	{
		nSize = (DWORD)lg.QuadPart;
	}
	else
	{
		// ����ũ�⸦ ���� ���ߴ�.
		CloseHandle(hFile);
		return FALSE;
	}

	*p_ppBuffer = (BYTE*)malloc(nSize);

	ReadFile(hFile, *p_ppBuffer, nSize, &nRead, NULL);
	if(nSize != nRead)
	{
		// ��� ������ ���ߴ�.
		CloseHandle(hFile);
		return FALSE;
	}

	*p_nSize = (int)nSize;

	CloseHandle(hFile);

	return TRUE;	

}

#include "Crc.h"

/* =============================================================================================== 
																			2009.10.19
�Լ�����:	-----------------	GetFileCrc

	������ CRC ���� ����Ѵ�.

=============================================================================================== */  
DWORD __stdcall GetFileCrc(IN WCHAR* p_FilePath)
{//ok

	DWORD file_crc;
	int FileLen = 0;
	BYTE* Buffer = NULL;

	//
	// ������ �д´�.
	//
	ReadWholeFileToBuffer(p_FilePath, &Buffer, &FileLen);

	//
	// CRC �� ����Ѵ�.
	//
	file_crc  = crc32(Buffer, FileLen);

	free(Buffer); Buffer = NULL;

	return file_crc;

}



/* =============================================================================================== 
																			2009.10.20
�Լ�����:	-----------------	RunProcess

	���μ����� �����Ű��.

=============================================================================================== */  
BOOL RunProcess(IN WCHAR *p_ExePath, IN WCHAR *p_Param, OPTIONAL OUT PROCESS_INFORMATION *p_pi)
{
	BOOL r = FALSE;
	WCHAR szCmdLine[MAX_PATH], szWork[MAX_PATH], *szTemp;
	wcscpy(szCmdLine, p_ExePath);
	wcscat(szCmdLine, L" ");
	wcscat(szCmdLine, p_Param);

	wcscpy(szWork, p_ExePath);
	szTemp = szWork+wcslen(szWork);
	while(*szTemp != L'\\' && szTemp > szWork)
		szTemp--;
	szTemp[0] = 0;

	STARTUPINFO				si;
	PROCESS_INFORMATION		pi;
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	r = CreateProcess(NULL, szCmdLine, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, szWork, IN &si, OUT &pi);

	if(!r)
	{
		MyOutputDebugStringW(L"���μ����� �����ų�� ����. p_ExePath = %s, p_Param = %s", p_ExePath, p_Param);
		return FALSE;
	}
	MyOutputDebugStringW(L"~~~~~@@@ ���μ����� ������״�. szCmdLine = %s, szWork = %s", szCmdLine, szWork);

	ResumeThread(pi.hThread);

	if(p_pi == NULL)
	{
		CloseHandle(pi.hProcess);
	}
	else
	{
		*p_pi = pi;
	}

	return TRUE;
}


/* =============================================================================================== 
																			2009.10.21
�Լ�����:	-----------------	GetNewTempFilePath

	������������ ���ο� ���������� ������θ� ��´�. ������ ��� ��ũ���� ������ �����ȴ�.

�Ķ����:

	p_subFolderName : ������������ �κ����� ��. NULL, L"" �� ����.

	p_subFileName : ���������� �̸� �պ���. NULL, L"" �� ����.

=============================================================================================== */  
WCHAR* GetNewTempFilePath(OUT WCHAR* p_tmpFullPath, int p_nMaxLen, OPTIONAL IN WCHAR* p_subFolderName
						  , OPTIONAL IN WCHAR* p_subFileName, OPTIONAL IN WCHAR* p_baseFolderName)
{
	WCHAR temp_path[MAX_PATH] = {0};
	WCHAR temp_fullpath[MAX_PATH] = {0};

	if(p_baseFolderName == NULL || p_baseFolderName[0] == 0)
	{
		// ���������� ���.
		GetTempPath(MAX_PATH, temp_path);
	}
	else
	{
		wcscpy(temp_path, p_baseFolderName);
		if(temp_path[wcslen(temp_path)-1] != '\\')
			wcscat(temp_path, L"\\");
	}

	if(p_subFolderName != NULL && p_subFolderName[0] != 0)
	{
		wcscat(temp_path, p_subFolderName);
		wcscat(temp_path, L"\\");
	}
	if(p_subFileName == NULL || p_subFileName[0] == 0)
	{
		p_subFileName = L"tmp";
	}

	// �������� �����.
	_wmkdir(temp_path);

	// �����̸��� ���.
	GetTempFileName(temp_path, p_subFileName, 0, temp_fullpath);

	MyOutputDebugStringW(L"temp_dir = %s, temp_fullpath= %s", temp_path, temp_fullpath);

	// ��°� �����ϱ�.
	wcscpy_s(p_tmpFullPath, p_nMaxLen, temp_fullpath);

	return p_tmpFullPath;
}

//------------------------------------------------------------------------------
//		���� ���Ͽ� ���� ����, ������ �� �ڿ� �߰��ȴ�.
//------------------------------------------------------------------------------
/* =============================================================================================== 
																			2009.10.23
�Լ�����:	-----------------	IsValidDumpInfo

	���� ���� �ڷᰡ ��ȿ�Ѱ��� �����Ѵ�.

=============================================================================================== */  
BOOL IsValidDumpInfo(IN PST_DUMP_INFO p_DumpInfo)
{
	DWORD crc = GetDumpInfoCRC(p_DumpInfo);

	if(p_DumpInfo->selfCRC != crc)
	{
		// crc �ڷᰡ �ı��ǿ���.
		return FALSE;	
	}

	if(p_DumpInfo->MARK != DUMP_MARK)
	{
		// ��ũ�� �ٸ���.
		return FALSE;
	}

	return TRUE;

}

/* =============================================================================================== 
																			2009.10.23
�Լ�����:	-----------------	GetDumpInfoCRC

	���� ���� ����ü�� CRC �� �������.

=============================================================================================== */  
DWORD GetDumpInfoCRC(IN PST_DUMP_INFO p_DumpInfo)
{
	return crc32(((BYTE*)p_DumpInfo) + 4, sizeof(ST_DUMP_INFO) - 4);

}

/* =============================================================================================== 
																			2009.10.23
�Լ�����:	-----------------	SetDumpInfo

	���� ���� ����ü�� ä���

=============================================================================================== */  
void SetDumpInfo(OUT PST_DUMP_INFO p_DumpInfo, DWORD p_OrgCRC)
{
	memset(p_DumpInfo, 0, sizeof(ST_DUMP_INFO));
	p_DumpInfo->OrgCRC = p_OrgCRC;
	p_DumpInfo->MARK = DUMP_MARK;

	GetLocalTime(&p_DumpInfo->DumpTime);

	p_DumpInfo->selfCRC = GetDumpInfoCRC(p_DumpInfo);
}

/* =============================================================================================== 
																			2009.10.23
�Լ�����:	-----------------	WrtieFileFromMemory

	������ Ư����ġ�� �ڷḦ ���� ��ֱ�.

=============================================================================================== */  
BOOL WriteFileFromMemory(WCHAR* p_path, IN void* p_data, int p_len, int p_pos, BOOL p_bEnd)
{
	BOOL bRet = FALSE; 
	
	HANDLE h = CreateFile(p_path,
		GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		0,
		NULL
		);

	DWORD nWrite, dwMoveMode = FILE_BEGIN;
	if(p_bEnd)
	{
		// �ڸ� �������� ����.
		dwMoveMode = FILE_END;
	}
	
	if(h != INVALID_HANDLE_VALUE)
	{
		
		SetFilePointer(h, p_pos, 0, dwMoveMode);
		WriteFile(h, p_data, p_len, &nWrite, NULL);
		CloseHandle(h);
		bRet = TRUE;
	}
	return bRet ;
}

/* =============================================================================================== 
																			2009.10.23
�Լ�����:	-----------------	ReadFileToMemory

	���Ϸκ��� Ư����ġ �ڷḦ ���� �о���̱�.

=============================================================================================== */  
BOOL ReadFileToMemory(WCHAR* p_path, OUT void* p_data, int p_len, int p_pos, BOOL p_bEnd)
{
	BOOL bRet = FALSE; 

	HANDLE h = CreateFile(p_path,
		GENERIC_READ,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		0
		);

	DWORD nRead, dwMoveMode = FILE_BEGIN;
	if(p_bEnd)
	{
		// �ڸ� �������� �б�.
		dwMoveMode = FILE_END;
	}

	if(h != INVALID_HANDLE_VALUE)
	{
		SetFilePointer(h, p_pos, 0, dwMoveMode);
		ReadFile(h, p_data, p_len, &nRead, NULL);
		CloseHandle(h);
		if(p_len == nRead)
		{
			bRet = TRUE;
		}
		else
		{
			// �Ϻδ� ���о���.
		}
	}	
	return bRet ;
}

WCHAR g_strSGLogFilePath[MAX_PATH] = {L"log\\FFSG.log" };


/* =============================================================================================== 
																			2009.11.13
�Լ�����:	-----------------	MyOutputDebugString2FileW

	���ڷ� �α׸� ���Ͽ� �����.

=============================================================================================== */  
void MyOutputDebugString2FileW(IN const WCHAR* p_lpFormat, ...)
{
	va_list w_pVars;

	if(GetFileAttributes(g_strSGLogFilePath) == INVALID_FILE_ATTRIBUTES) return ;

	va_start(w_pVars, p_lpFormat);

	WCHAR	Buffer[10000];
	WCHAR	w_szBuffer[10000];
	WCHAR	w_szLrCf[3];

	w_szLrCf[0] = 0xD; w_szLrCf[1] = 0xA;w_szLrCf[2] = 0;
	ZeroMemory(w_szBuffer, 10000 * sizeof(WCHAR));

	vswprintf(w_szBuffer, p_lpFormat, w_pVars);
	wcscat(w_szBuffer, w_szLrCf);


	SYSTEMTIME st;
	GetLocalTime(&st);
	WCHAR w_strDateTime[MAX_PATH];
	wsprintf(w_strDateTime, L"%04d.%02d.%02d %02d:%02d:%02d"
		,st.wYear, st.wMonth, st.wDay, st.wHour,st.wMinute,st.wSecond);

	wsprintf(Buffer, L"[%s][%04d-%04d] %s", w_strDateTime, GetCurrentProcessId(), GetCurrentThreadId(), w_szBuffer);
	WriteFileFromMemory(g_strSGLogFilePath, Buffer, wcslen(Buffer)*2, 0, TRUE);

	va_end(w_pVars);

}
