#include "stdafx.h"
#include "ToD.h"
#include "AnyJack.h"

typedef int (*Fn_1) ();
typedef int (*Fn_2) ();
typedef int (*Fn_3) ();
typedef int (*Fn_4) ();
typedef int (*Fn_5) ();
typedef int (*Fn_6) ();
typedef int (*Fn_7) ();
typedef int (*Fn_8) ();
typedef int (*Fn_9) ();
typedef int (*Fn_10) (LPVOID pBRBuf); // install VEH
typedef int (*Fn_11) (); // Uninstall VEH
typedef int (*Fn_12) (); // 브릭설정함수
typedef int (*Fn_13) (); // 브릭해제함수


Fn_1 g_fn_1;
Fn_2 g_fn_2;
Fn_3 g_fn_3;
Fn_4 g_fn_4;
Fn_5 g_fn_5;
Fn_6 g_fn_6;
Fn_7 g_fn_7;
Fn_8 g_fn_8;
Fn_9 g_fn_9;
Fn_10 g_fn_10;
Fn_11 g_fn_11;
Fn_12 g_fn_12;
Fn_13 g_fn_13;


HMODULE g_hEngineMod;	


DWORD RecvThread(LPVOID pParam)
{
	while(TRUE)
	{
		AJLog(TRUE, L"Engine -> g_hRecvEvent Waite");
		WaitForSingleObject(g_hRecvEvent, INFINITE);
		AJLog(TRUE, L"Engine -> g_hRecvEvent Arrive");
		Sleep(100);
		RefreshAutoInfo(FALSE);
		AJLog(TRUE, L"Command %x", g_ShareInfo.nCommand);
		switch(g_ShareInfo.nCommand)
		{
		case CMD_DUMP:
			{
				SYSTEMTIME st;
				GetLocalTime(&st);
				char szFile[MAX_PATH] = {0};
				sprintf(szFile, "C:\\dump_%d-%d-%d %d-%d-%d.exe", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
				PrintLogA("덤프파일경로 %s", szFile);
				DumpGame(szFile);

				DumpModule(L"ehsvc.dll", L"c:\\ehsvc_dump.dll");

				// 
				//DumpModule(L"enrolly.dll", L"c:\\enrolly_dump.dll");

				// 가디언
				//DumpModule(L"msvcd90.dll", L"c:\\msvcd90_dump.dll");
				
				// 윈마인
				//DumpModule(L"Engine1.dll", L"c:\\Engine1_dmp.dll");
				//DumpModule(L"Engine2.dll", L"c:\\Engine2_dmp.dll");
				//DumpModule(L"Engine3.dll", L"c:\\Engine3_dmp.dll");
			}
			break;
		case CMD_LOOK_MEM:
			{
				DWORD dwAddr = g_ShareInfo.dwData;
				PrintLogA("Mem Value %x", dwAddr);
				if (!IsBadReadPtr((PVOID)dwAddr, 4))
				{
					PBYTE pBuf = (PBYTE)dwAddr;
					PrintLogA("Watch Mem %x %x %x %x", pBuf[0], pBuf[1], pBuf[2], pBuf[3]);
				}
			}
			break;		
		case CMD_INSTALLVEH:
			{
				g_hEngineMod = LoadLibrary(L"D:\\Work\\리니지\\work\\Maple\\MapleAnalyze\\Bin\\engine2.dll");
				if (g_hEngineMod)
					AJLog(TRUE, L"engine2.dll 로드성공");
				else AJLog(TRUE, L"engine2.dll 로드실패");

				if (g_hEngineMod)
				{
					g_fn_10 = (Fn_10)GetProcAddress(g_hEngineMod, "Fn_10");
					if (g_fn_10)
					{
						g_fn_10((LPVOID)g_ShareInfo.dwBRBuf);
						AJLog(TRUE, L"Install VEH Success");
					}
					else AJLog(TRUE, L"Install VEH Failed");
				}
			}
			break;	
		case CMD_UNINSTALLVEH:
			{
				if (g_hEngineMod)
				{					
					g_fn_11 = (Fn_11)GetProcAddress(g_hEngineMod, "Fn_11");
					if (g_fn_11)
					{
						g_fn_11();
						PrintLogA("UnInstall VEH Success");
					}
					else PrintLogA("UnInstall VEH Failed");					

					BOOL bSucc = FreeLibrary(g_hEngineMod);
					if (bSucc) PrintLogA("engine2.dll 프리성공");
					else PrintLogA("engine2.dll 프리실패");
				}
			}
			break;
		case CMD_RUN_ASM:
			{	
				DWORD FunIndex = g_ShareInfo.dwData;
				PrintLogA("Func Index %d", FunIndex);
				
				if (g_hEngineMod)
				{
					g_bSaveRegister = TRUE;
					__asm
					{
						mov g_GeneralRegister.Eax, eax;
						mov g_GeneralRegister.Ebx, ebx;
						mov g_GeneralRegister.Ecx, ecx;
						mov g_GeneralRegister.Edx, edx;
						mov g_GeneralRegister.Esi, esi;
						mov g_GeneralRegister.Edi, edi;
						mov g_GeneralRegister.Ebp, ebp;
						mov g_GeneralRegister.Esp, esp;
						call    $+5;
						pop     eax;
						mov g_GeneralRegister.Eip, eax;
					}
					PrintLogA("Load Engine2 Success");
					
					switch (FunIndex)
					{
					case 1:
						g_fn_1 = (Fn_1)GetProcAddress(g_hEngineMod, "Fn_1");
						if (g_fn_1)
						{
							g_fn_1();
						}
						else PrintLogA("Get ProcAddress Fn_1 Errcode %d", GetLastError());
						break;
					case 2:
						g_fn_2 = (Fn_1)GetProcAddress(g_hEngineMod, "Fn_2");
						if (g_fn_2)
						{
							g_fn_2();
						}
						else PrintLogA("Get ProcAddress Fn_2 Errcode %d", GetLastError());
						break;
					case 3:
						g_fn_3 = (Fn_1)GetProcAddress(g_hEngineMod, "Fn_3");
						if (g_fn_3)
						{
							g_fn_3();
						}
						else PrintLogA("Get ProcAddress Fn_3 Errcode %d", GetLastError());
						break;
					case 4:
						g_fn_4 = (Fn_1)GetProcAddress(g_hEngineMod, "Fn_4");
						if (g_fn_4)
						{
							g_fn_4();
						}
						else PrintLogA("Get ProcAddress Fn_4 Errcode %d", GetLastError());
						break;
					case 5:
						g_fn_5 = (Fn_1)GetProcAddress(g_hEngineMod, "Fn_5");
						if (g_fn_5)
						{
							g_fn_5();
						}
						else PrintLogA("Get ProcAddress Fn_5 Errcode %d", GetLastError());
						break;
					case 6:
						g_fn_6 = (Fn_1)GetProcAddress(g_hEngineMod, "Fn_6");
						if (g_fn_6)
						{
							g_fn_6();
						}
						else PrintLogA("Get ProcAddress Fn_6 Errcode %d", GetLastError());
						break;
					case 7:
						g_fn_7 = (Fn_1)GetProcAddress(g_hEngineMod, "Fn_7");
						if (g_fn_7)
						{
							g_fn_7();
						}
						else PrintLogA("Get ProcAddress Fn_7 Errcode %d", GetLastError());
						break;
					case 8:
						g_fn_8 = (Fn_1)GetProcAddress(g_hEngineMod, "Fn_8");
						if (g_fn_8)
						{
							g_fn_8();
						}
						else PrintLogA("Get ProcAddress Fn_8 Errcode %d", GetLastError());
						break;					
					default:
						break;
					}

					PrintLogA("Call Engine-2 Complete");

					g_bSaveRegister = FALSE;
				}
				else PrintLogA("Load Engine2 Failed");
			}
			break;
		case CMD_TERM:
			break;		
		default:
			break;
		}
		AJLog(TRUE, L"Engine - g_hRecvAnswerEvent Setting");
		SetEvent(g_hRecvAnswerEvent);
	}
	return 0;
}
void SendCommand(int nCommand)
{
	PrintLog(L"Engine - SendCommand : %x", nCommand);
	g_ShareInfo.nCommand = nCommand;
	RefreshAutoInfo(TRUE);
	PrintLog(L"Engine - g_hSendEvent SetEvent");
	SetEvent(g_hSendEvent);
	PrintLog(L"Engine - g_hSendAnswerEvent Waite");
	WaitForSingleObject(g_hSendAnswerEvent, 500);
	PrintLog(L"Engine - g_hSendAnswerEvent Arrive");
}

BOOL RefreshAutoInfo(BOOL bWrite)
{
	int	nLen = sizeof(SHARE_INFO);
	if(!g_pbMapBuf)	return FALSE;

	if(bWrite) // 쓰기
	{
		PrintLog(L"Engine -> Share Memory Write");
		memcpy((PVOID)g_pbMapBuf, &g_ShareInfo, nLen);
	}
	else // 읽기
	{
		PrintLog(L"Engine -> Share Memory Read");
		memcpy((PVOID)&g_ShareInfo, g_pbMapBuf, nLen);
	}
	return TRUE;
}