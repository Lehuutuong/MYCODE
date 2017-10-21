//--------------------------------------------------------
//		�Ϲ����� ó������ �����Ѵ�.
//--------------------------------------------------------
// 2009.1.9
// 
//
#include "stdafx.h"

#include "Common0.h"

// #define ISFILELOG

WCHAR g_strLogPath[MAX_PATH] = {0};
WCHAR g_strLogFilePath[MAX_PATH] = {0};
WCHAR g_strToday[MAX_PATH] = {0};


//----------------------------------------------------
//		â�� �İ��ϱ�
//----------------------------------------------------

static BOOL CALLBACK PrintWindowsInfo (HWND hwnd, LPARAM driveNo)
{
	WCHAR s[MAX_PATH], s2[MAX_PATH];
	WCHAR buf[MAX_PATH*5];
	RECT rt;

	if(hwnd==NULL) return TRUE;
	GetWindowRect(hwnd, &rt);
	GetClassNameW (hwnd, s, sizeof(s));
	GetWindowText (hwnd, s2, sizeof(s2));
	wsprintf(buf, L"�Ƶ�â�� ���� : hwnd=%x classname=%s text=%s lest=%d top=%d width=%d height=%d"
		, s, s2, rt.left, rt.top, rt.right-rt.left, rt.bottom-rt.top);
	OutputDebugStringW(buf);

	//	PostMessage (hwnd, WM_CLOSE, 0, 0);

	return TRUE;
}

BOOL PrintWindowsEnum (HWND hwnd)
{

	OutputDebugStringW(L">>> PrintWindowsEnum ");
	PrintWindowsInfo(hwnd, 0);
	PrintWindowsInfo(GetActiveWindow(), 0);
	return EnumChildWindows  (hwnd, PrintWindowsInfo, (LPARAM)NULL);
}


void GetBinaryString(IN unsigned char *p_pData, IN unsigned int p_nSize, OUT wchar_t *pBinaryString)
{
	wchar_t sample[16] = {
		L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', 
		L'8', L'9', L'A', L'B', L'C', L'D', L'E', L'F'
	};

	for(unsigned int i = 0; i < p_nSize; i++)
	{
		pBinaryString[i * 3 + 0] = sample[(p_pData[i] & 0xF0) >> 4];
		pBinaryString[i * 3 + 1] = sample[p_pData[i] & 0x0F];
		pBinaryString[i * 3 + 2] = ' ';
	}
	pBinaryString[p_nSize * 3] = 0;

}

//  [1/13/2009 Administrator]
int AddAddressInfo(int addr, OUT char* pBinaryString, int idx)
{
	char sample[16] = {
		L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', 
		L'8', L'9', L'A', L'B', L'C', L'D', L'E', L'F'
	};

	pBinaryString[idx + 0] = sample[(addr & 0xF0000000) >> 28];
	pBinaryString[idx + 1] = sample[(addr & 0x0F000000) >> 24];
	pBinaryString[idx + 2] = sample[(addr & 0x00F00000) >> 20];
	pBinaryString[idx + 3] = sample[(addr & 0x000F0000) >> 16];
	pBinaryString[idx + 4] = sample[(addr & 0xF000) >> 12];
	pBinaryString[idx + 5] = sample[(addr & 0x0F00) >> 8];
	pBinaryString[idx + 6] = sample[(addr & 0xF0) >> 4];
	pBinaryString[idx + 7] = sample[(addr & 0x0F)];
	pBinaryString[idx + 8] = ' ';
	pBinaryString[idx + 9] = ' ';
	idx+=10;

	return idx;

}

void GetBinaryString(IN unsigned char *p_pData, IN unsigned int p_nSize, OUT char *pBinaryString)
{
	char sample[16] = {
		L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', 
		L'8', L'9', L'A', L'B', L'C', L'D', L'E', L'F'
	};

	int i = 0;
	int idx = 0;
	int nFore = (int)((DWORD_PTR) p_pData % 16);

	DWORD addr = (DWORD)((DWORD_PTR) p_pData - nFore);

	// 	idx = AddAddressInfo(addr, OUT pBinaryString, idx);
	// 
	// 	for(i = 0; i < nFore; i++)
	// 	{
	// 		pBinaryString[idx + 0] = '*';
	// 		pBinaryString[idx + 1] = '*';
	// 		pBinaryString[idx + 2] = ' ';
	// 		idx += 3;
	// 		if(i % 16 == 7)
	// 		{
	// 			//�����ϳ� �� ���̱�
	// 			pBinaryString[idx] = ' '; idx++;
	// 		}
	// 	}

	for(i = -nFore; i < (int)p_nSize; i++)
	{
		if((i + nFore) % 16 == 0)
		{
			idx = AddAddressInfo((DWORD)((DWORD_PTR)p_pData + i), OUT pBinaryString, idx);
		}

		if(i < 0)
		{
			pBinaryString[idx + 0] = '*';
			pBinaryString[idx + 1] = '*';
			pBinaryString[idx + 2] = ' ';
			idx += 3;
		}
		else
		{
			pBinaryString[idx + 0] = sample[(p_pData[i] & 0xF0) >> 4];
			pBinaryString[idx + 1] = sample[p_pData[i] & 0x0F];
			pBinaryString[idx + 2] = ' ';
			idx += 3;

		}

		if((i + nFore) % 16 == 7)
		{
			//�����ϳ� �� ���̱�
			pBinaryString[idx] = ' '; idx++;
		}


		if((i + nFore) % 16 == 15)
		{
			pBinaryString[idx] = ' '; idx++;
			pBinaryString[idx] = ' '; idx++;
			pBinaryString[idx] = ' '; idx++;
			//�ƽ�Ű ���
			for(int j = 0; j < 16; j++)
			{
				int ind = i - 15 + j;
				if(ind < 0)
				{
					pBinaryString[idx] = '*';
				}
				else if(p_pData[ind] < 0x20 && (j == 0 || p_pData[ind-1] < 0x80)) //�ι���Ʈ ���ڶ�����..
				{
					pBinaryString[idx] = '.';
				}
				else
				{
					pBinaryString[idx] = p_pData[ind];
					if(pBinaryString[idx] <= 0x20) pBinaryString[idx] = '.';
				}
				idx++;
			}
			if(i == (int)p_nSize - 1) break;
			// ��ٲٱ�
			pBinaryString[idx] = '\r'; idx++;
			pBinaryString[idx] = '\n'; idx++;
#ifdef OUTPRINT_MODE
			pBinaryString[idx] = '\t'; idx++;
			pBinaryString[idx] = '\t'; idx++;
#else
			if((i + nFore) % 512 == 0)
			{
				// ��ٲٱ�
				pBinaryString[idx] = '\r'; idx++;
				pBinaryString[idx] = '\n'; idx++;
			}		
#endif
		}
	}
	pBinaryString[idx] = 0;

}

void DbgPrintBinary(void* dwBaseAddr, DWORD dwLen)
{
	char *buf;

	buf = new char[dwLen*5+100];

	if(IsBadReadPtr(dwBaseAddr, dwLen))
	{
		OutputDebugStringA(">>> ������ ���� �ּ��Դϴ�.");
	}
	else
	{
		GetBinaryString((BYTE*)dwBaseAddr, dwLen, buf);
		char ss[200];
		sprintf(ss, ">>> ���� �մϴ�. �ּ� = %x ���� = %x ===================================", dwBaseAddr, dwLen);

		OutputDebugStringA(ss);
		OutputDebugStringA(buf);
	}

	delete buf;

}


char* ToANSI (char* lpszNewText, int nLen, wchar_t* lpszText)
{
	// 2����Ʈ���ڴ� 1����Ʈ���ڿ��� ���̸� 2��� ���� ��Ų��.
	int j = (int)wcslen (lpszText)*2;
	if (lpszNewText == 0)
	{
		lpszNewText = (char*) malloc ((j + 1) * 2);
		nLen = (j + 1) * 2;
	}
	if (j == 0)
	{
		lpszNewText[0] = 0;
		return lpszNewText;
	}
	else
	{
		if (nLen > 0 && j > nLen - 1)
		{
			j = nLen - 1;
		}
		WideCharToMultiByte (CP_ACP, 0L, lpszText, -1, lpszNewText, j + 1, NULL, NULL);
		lpszNewText[j] = 0;

	}
	return lpszNewText;
}
wchar_t* ANSI2UNICODE (wchar_t* lpszNewText, int nLen, char *lpszText)
{
	int j = (int)strlen (lpszText);
	if (lpszNewText == 0)
	{
		lpszNewText = (wchar_t*) malloc ((j + 1) * 2);
		nLen = (j + 1) * 2;// 2009.2.6 23 zc(test). 
	}
	if (j == 0)
	{
		lpszNewText[0] = 0;
		return lpszNewText;
	}
	else
	{
		if (nLen > 0 && j > nLen - 1)
		{
			j = nLen - 1;
		}
		j = MultiByteToWideChar (CP_ACP, 0L, lpszText, -1, lpszNewText, j + 1);
	}
	return lpszNewText;	
}


// �α׸� ����� ������ ����̴�. �ڿ� Ȯ���ڸ� ���̸� �ȴ�.
void SetLogFilePath()
{
	// ���� ��� + log ����
	// ��ξ��
	WCHAR	*szTemp, m_szPath[MAX_PATH] = {0};

	GetModuleFileName(0, m_szPath, MAX_PATH);
	szTemp = m_szPath+wcslen(m_szPath);
	while(*szTemp != L'\\')
		szTemp--;
	*szTemp = 0;
	
	wcscpy(g_strLogPath, m_szPath);
	wcscat(g_strLogPath, L"\\log\\");
	_wmkdir(g_strLogPath);

	// �α��̸��� �պ���.

	CTime cur = CTime::GetCurrentTime();
	wsprintf(g_strToday, L"%04d-%02d-%02d.log", cur.GetYear(), cur.GetMonth(), cur.GetDay());

	WCHAR buf[100];
	wsprintf(buf, L"lin.%s", g_strToday);
	wcscat(g_strLogPath, buf);
	wcscpy(g_strLogFilePath, g_strLogPath);
	wcscat(g_strLogFilePath, L".log");

	if(GetFileAttributes(g_strLogFilePath) == INVALID_FILE_ATTRIBUTES)
	{
		// �����ڵ� ���� �ھ� �ֱ�
		BYTE aa[4] = {0xFF,0xFE};
		FILE* fp = _wfopen(g_strLogFilePath, L"wb");
		fwrite(aa, 1, 2, fp);
		fclose(fp);
	}

}

// ���Ϸα׸� ���� ����ȭ ������Ʈ
CRITICAL_SECTION	g_csFileLog;		
// ���μ����� ����� ���� ����ȭ ������Ʈ
CRITICAL_SECTION	g_csProcessComm;		

/* =============================================================================================== 

�Լ�����: 2009�� 4�� 9�� 23��
 
	����� ������ ���� ���Ͽ� ����Ѵ�.     

=============================================================================================== */
void _WOutputDebugStringA(CHAR* str)
{
	WCHAR *sss = ANSI2UNICODE(NULL, 0, str);
	_WOutputDebugStringW(sss);
	free(sss);
}

void _WOutputDebugStringW(WCHAR *str)
{

	if(g_strLogFilePath[0] == 0) return;

	BYTE aa[4] = {0xD, 0,0xA, 0};
	WCHAR ss[100] = {0};
	EnterCriticalSection(&g_csFileLog);

 	FILE* fp = _wfopen(g_strLogFilePath, L"ab");
 
 	fwrite(str, 2, wcslen(str), fp);
 	fwrite(aa, 1, 4, fp);
 	fclose(fp);

	LeaveCriticalSection(&g_csFileLog);

}
/* =============================================================================================== 

�Լ�����: 2009�� 4�� 9�� 22��

	���Ͽ� �α׸� ����Ѵ�.

=============================================================================================== */

BOOL PrintLog(WCHAR * strLog)
{
	CTime cur = CTime::GetCurrentTime();
	WCHAR buf[MAX_PATH*2];

	wsprintf(buf, L"%02d:%02d:%02d - %s", cur.GetHour(), cur.GetMinute(), cur.GetSecond(), strLog);

	_WOutputDebugStringW(buf);

	return TRUE;	
}

/* =============================================================================================== 

�Լ�����: 2009�� 4�� 9�� 22��
 
	���̳ʸ� �ڷḦ ����Ѵ�.
 
=============================================================================================== */
BOOL PrintBinaryData(void* p_Buffer, int p_Size, WCHAR* p_Comment)
{
	WCHAR  * buf = new WCHAR[p_Size*5+100];
	wcscpy(buf, p_Comment);
	wcscat(buf, L" ");
	GetBinaryString((BYTE*)p_Buffer, p_Size, buf + wcslen(buf));
	BOOL r = TRUE;
	//r = PrintLog(buf);
	delete buf;

	return r;

}

void dbgPrintBinaryData(void* p_Buffer, int p_Size, WCHAR* p_Comment)
{
	WCHAR  * buf = new WCHAR[p_Size*5+100];
	wcscpy(buf, p_Comment);
	wcscat(buf, L" ");
	GetBinaryString((BYTE*)p_Buffer, p_Size, buf + wcslen(buf));
	BOOL r = TRUE;
	OutputDebugStringW(buf);
	delete buf;
	return ;
}

/* =============================================================================================== 

�Լ�����: 2009�� 4�� 9�� 22��

	Application �ʱ�ȭ ó��

=============================================================================================== */
void InitApp()
{
	// Initialize the critical section one time only.
	InitializeCriticalSection(&g_csFileLog);

	// Initialize the critical section one time only.
	InitializeCriticalSection(&g_csProcessComm);

// 	// �α� ��� ����
// 	SetLogFilePath();

}

/* =============================================================================================== 

�Լ�����: 2009�� 4�� 9�� 22��

	Application ������ ó��

=============================================================================================== */
void TerminateApp()
{
	// Release resources used by the critical section object.
	DeleteCriticalSection(&g_csFileLog);
}

