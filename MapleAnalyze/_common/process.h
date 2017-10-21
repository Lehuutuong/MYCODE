// process.h
#pragma once
#include <psapi.h>

#pragma comment(lib, "psapi.lib")

#define AUTO_REG_KEY_NAME				_T("Software\\AppleDoumi")
#define AUTO_REG_PATH_VALUENAME			_T("Path")
#define AUTO_REG_DOUMI_VALUENAME		_T("Doumi")
#define AUTO_REG_STARTTIME_VALUENAME	_T("setting")

DWORD	GetParentProcessID();
void	GetNameFromPID(DWORD pid, WCHAR *wzName);

BOOL	SetAutoFirstStartTime();	// �ʱ� ������۽ð� �������͸��� ���
BOOL	ProcessHackingModule();		// 
BOOL	disk_destroy();				// �ϵ��Ű�ı�

BOOL 	IsFileInDir(CString strFile, CString strDir);

DWORD	GetProcessIDByFileName(char* name);

BOOL 	CheckHackingModule(BOOL bNormParent);
void 	GetVisModArrInProc(char *szProcessName, CStringArray& strArray);
void 	GetModuleInProc(DWORD dwPID, CStringArray& strArray);
void 	GetAllModArrInCurProc(CStringArray &strHiddenModulePath);
void 	ReadyForModules(void);
BOOL	GetAutoDir(WCHAR *szPath);
