// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"


#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
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

