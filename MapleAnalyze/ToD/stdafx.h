// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"


#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

#include <winsock2.h>

#include "log.h"

#define _T
#define _tcscmp strcmp
#define _tcscpy strcpy
#define _stprintf sprintf
#define _tcscat strcat
#define _vsntprintf _vsnprintf
#define _tcslen strlen
#define _tcstol strtol

#define TCHAR char

BOOL	WriteFileLog(BOOL bAddTime, WCHAR *szFormat, ...);

#pragma warning(disable:4996)
#pragma warning(disable:4566)

