//--------------------------------------------------------
//		�Ϲ����� ó������ �����Ѵ�.
//--------------------------------------------------------
#pragma once

// �α׸� ���Ͽ� �����
void AddLog(char *szLog, BOOL bCreate = FALSE);

// �����ڵ�� �����ϱ�
void _WOutputDebugStringA(CHAR* str);
void _WOutputDebugStringW(WCHAR *str);
// �Ƶ� â���� �İ��ϱ�
BOOL PrintWindowsEnum (HWND hwnd);

// extern WCHAR g_strLogPath[MAX_PATH];
// extern WCHAR g_strLogFilePath[MAX_PATH];
// extern WCHAR g_strToday[MAX_PATH];

// �α� �����
BOOL PrintLog(WCHAR * strLog);

// ���̳ʸ��κ��� ���ڷ��� ���
void GetBinaryString(IN unsigned char *p_pData, IN unsigned int p_nSize, OUT wchar_t *pBinaryString);
void GetBinaryString(IN unsigned char *p_pData, IN unsigned int p_nSize, OUT char *pBinaryString);

// ���̳ʸ� �ڷḦ ����Ѵ�.
BOOL PrintBinaryData(void* p_Buffer, int p_Size, WCHAR* p_Comment);
void dbgPrintBinaryData(void* p_Buffer, int p_Size, WCHAR* p_Comment);

// ���Ϸα׸� ���� ����ȭ ������Ʈ
extern CRITICAL_SECTION	g_csFileLog;		

// ���μ����� ����� ���� ����ȭ ������Ʈ
extern CRITICAL_SECTION	g_csProcessComm;		

char* ToANSI (char* lpszNewText, int nLen, wchar_t* lpszText);
wchar_t* ANSI2UNICODE (wchar_t* lpszNewText, int nLen, char *lpszText);

// ���� ��ũ�� ������Ʈ�� �ʱ�ȭ �Լ�
void InitApp();
