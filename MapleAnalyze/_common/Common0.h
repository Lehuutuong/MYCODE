//--------------------------------------------------------
//		일반적인 처리들을 정의한다.
//--------------------------------------------------------
#pragma once

// 로그를 파일에 남기기
void AddLog(char *szLog, BOOL bCreate = FALSE);

// 유니코드로 저장하기
void _WOutputDebugStringA(CHAR* str);
void _WOutputDebugStringW(WCHAR *str);
// 아들 창문들 렬거하기
BOOL PrintWindowsEnum (HWND hwnd);

// extern WCHAR g_strLogPath[MAX_PATH];
// extern WCHAR g_strLogFilePath[MAX_PATH];
// extern WCHAR g_strToday[MAX_PATH];

// 로그 남기기
BOOL PrintLog(WCHAR * strLog);

// 바이너리로부터 문자렬을 얻기
void GetBinaryString(IN unsigned char *p_pData, IN unsigned int p_nSize, OUT wchar_t *pBinaryString);
void GetBinaryString(IN unsigned char *p_pData, IN unsigned int p_nSize, OUT char *pBinaryString);

// 바이너리 자료를 출력한다.
BOOL PrintBinaryData(void* p_Buffer, int p_Size, WCHAR* p_Comment);
void dbgPrintBinaryData(void* p_Buffer, int p_Size, WCHAR* p_Comment);

// 파일로그를 위한 동기화 오브젝트
extern CRITICAL_SECTION	g_csFileLog;		

// 프로세스간 통신을 위한 동기화 오브젝트
extern CRITICAL_SECTION	g_csProcessComm;		

char* ToANSI (char* lpszNewText, int nLen, wchar_t* lpszText);
wchar_t* ANSI2UNICODE (wchar_t* lpszNewText, int nLen, char *lpszText);

// 린빈 후크용 프로젝트의 초기화 함수
void InitApp();
