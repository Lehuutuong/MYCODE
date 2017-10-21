// TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Test.h"
#include "TestDlg.h"
#include "MIPInstall.h"
#include "CmdDef.h"
#include <WinSvc.h>
#include <conio.h>

#include <winsvc.h>
#include <winioctl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define			SHARE_MEM_SIZE		0x10000

// CAboutDlg dialog used for App About
WCHAR g_szHSDVDriverName[MAX_PATH] = L"";
void ExtractService_HSDV();
int InstallService_HSDV();
void UninstallService_HSDV();
void RemoveService_HSDV();




// 키보드드라이버
WCHAR g_szkbfDriverName[MAX_PATH] = L"";
void ExtractService_kbf();
int  InstallService_kbf();
void UninstallService_kbf();
void RemoveService_kbf();


// threads
DWORD			RecvThread(LPVOID pParam); 
SHARE_INFO		g_ShareInfo;

char g_szModDir[MAX_PATH];

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTestDlg dialog


CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
	, m_strBR1(_T(""))
	, m_strBR2(_T(""))
{
	m_bInjected = FALSE;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BREAK, m_ctlBreakList);
	DDX_Text(pDX, IDC_EDIT_BR1, m_strBR1);
	DDX_Text(pDX, IDC_EDIT_BR2, m_strBR2);
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_INJECT, &CTestDlg::OnBnClickedBtnInject)
	ON_BN_CLICKED(IDC_BTN_DUMP, &CTestDlg::OnBnClickedBtnDump)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_WATCHMEM, &CTestDlg::OnBnClickedBtnWatchmem)
	ON_BN_CLICKED(IDC_BTN_HOOKVT, &CTestDlg::OnBnClickedBtnHookvt)	
	ON_BN_CLICKED(IDC_BUTTON7, &CTestDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BTN_BR_ADD, &CTestDlg::OnBnClickedBtnBrAdd)
	ON_BN_CLICKED(IDC_BTN_BR_DEL, &CTestDlg::OnBnClickedBtnBrDel)
	ON_BN_CLICKED(IDC_BTN_BR_CHA, &CTestDlg::OnBnClickedBtnBrCha)
	ON_NOTIFY(NM_CLICK, IDC_LIST_BREAK, &CTestDlg::OnNMClickListBreak)
	ON_BN_CLICKED(IDC_BTN_BR_INIT, &CTestDlg::OnBnClickedBtnBrInit)
	ON_BN_CLICKED(IDC_BUTTON8, &CTestDlg::OnBnClickedButton8)
END_MESSAGE_MAP()


// CTestDlg message handlers

BOOL CTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	// 
	GetSystemDirectory(g_szHSDVDriverName, MAX_PATH);
	wcscat(g_szHSDVDriverName, L"\\drivers\\HSDV.sys");
	ExtractService_HSDV();
	if (InstallService_HSDV() == 0) // 0: ERR, 1: SUCC, 2:ALREADY_RUNNING
	{
		UninstallService_HSDV();
		InstallService_HSDV();
	}
	RemoveService_HSDV();

	DeleteFile(L"e:\\enginelog.txt");
	//<------------- 키보드드라이버
	//GetSystemDirectory(g_szkbfDriverName, MAX_PATH);
	//wcscat(g_szkbfDriverName, L"\\drivers\\HelloDDK.sys");
	//ExtractService_kbf();
	//if (InstallService_kbf() == 0) // 0: ERR, 1: SUCC, 2:ALREADY_RUNNING
	//{
	//	UninstallService_kbf();
	//	InstallService_kbf();
	//}
	//RemoveService_kbf();//>


	// engine.dll 과의 통신위해
	m_hRecvEvent		= CreateEventW(NULL, FALSE, FALSE, L"Global\\MyRecvEvent"); 
	m_hSendEvent		= CreateEventW(NULL, FALSE, FALSE, L"Global\\MySendEvent"); 
	m_hSendAnswerEvent	= CreateEventW(NULL, FALSE, FALSE, L"Global\\MySendAnswerEvent");
	m_hRecvAnswerEvent	= CreateEventW(NULL, FALSE, FALSE, L"Global\\MyRecvAnserEvent");
	
	int a = GetLastError();
	if (m_hRecvEvent == NULL || m_hSendEvent == NULL || m_hSendAnswerEvent == NULL || m_hRecvAnswerEvent == NULL)
	{
		AfxMessageBox(L"Create Global Event Failed");
		return FALSE;
	}

	m_ctlBreakList.SetExtendedStyle(m_ctlBreakList.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_ctlBreakList.InsertColumn(0, L"NO", 0, 30);
	m_ctlBreakList.InsertColumn(1, L"BR1", 0, 70);
	m_ctlBreakList.InsertColumn(2, L"BR2", 0, 70);
	m_nSelBRList = -1;

	GetModuleFileNameA(NULL, g_szModDir, MAX_PATH);
	char *ch = strrchr(g_szModDir, '\\');
	*ch = 0;



	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void ExtractService_HSDV()
{
	HRSRC hResource = FindResource(NULL, MAKEINTRESOURCE(IDR_DRIVER_HSDV), L"BINARY");
	HGLOBAL hGlobal = LoadResource(NULL, hResource);
	LPVOID lpBuffer = LockResource(hGlobal);

	HANDLE hFile = NULL;
	DWORD dwSize = SizeofResource(NULL, hResource);
	DWORD dwBytesWritten = 0;

	hFile = CreateFile(g_szHSDVDriverName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, 0, NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		WriteFile(hFile, lpBuffer, dwSize, &dwBytesWritten, NULL);
		CloseHandle(hFile);
		//OutputDebugStringA("hsdv service extract Succuss");
	}
	//else OutputDebugStringA("hsdv service extract Fail");
}


// 0 실패 1: 성공 2: 이미 시작
int InstallService_HSDV()
{
	SC_HANDLE	hSCManager;
	SC_HANDLE	hService;
	int			iRet = 0;	
	char		szErr[MAX_PATH];

	hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(hSCManager)
	{
		hService = CreateService(hSCManager, L"HSSD", L"HSSD", SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_AUTO_START, SERVICE_ERROR_NORMAL, 
			g_szHSDVDriverName, NULL, NULL, NULL, NULL, NULL);

		if(!hService) hService = OpenService(hSCManager, L"HSSD", SERVICE_ALL_ACCESS);

		if(hService)
		{
			if(StartService(hService, 0, NULL))
			{
				OutputDebugStringA("Install--hsdv service was successfully installed.\n");
				iRet = 1;
			}
			else if (GetLastError() == ERROR_SERVICE_ALREADY_RUNNING)
			{
				OutputDebugStringA("Install--hsdv service already running.\n");
				iRet = 2;
			}
			else 
			{
				sprintf(szErr, "Install-- hsdv service failed1: err = %d", GetLastError());
				OutputDebugStringA(szErr);			
			}
			CloseServiceHandle(hService);
		}
		else 
		{
			sprintf(szErr, "Install--hsdv OpenService service failed: err = %d", GetLastError());
			OutputDebugStringA(szErr);
		}
		CloseServiceHandle(hSCManager);
	}
	else
	{
		//OutputDebugStringA("Install--hsdv Create Service Manager Failed.\n");
	}
	return iRet;
}

void UninstallService_HSDV()
{
	SC_HANDLE hSCManager;
	SC_HANDLE hService; 
	SERVICE_STATUS serviceStatus;

	hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS); 
	if(hSCManager)
	{
		hService = OpenService(hSCManager, L"HSSD", SERVICE_ALL_ACCESS); 
		if(hService)
		{
			ControlService(hService, SERVICE_CONTROL_STOP , &serviceStatus);
			if(DeleteService(hService))
			{
				//OutputDebugStringA("UnInstall--service was successfully uninstalled.\n");
			}
			CloseServiceHandle(hService);
		}
		//else OutputDebugStringA("UnInstall--OpenService service failed.\n");

		CloseServiceHandle(hSCManager);
	}
	//else OutputDebugStringA("UnInstall--Create Service Manager Failed.\n");
}

void RemoveService_HSDV()
{
	DeleteFile(g_szHSDVDriverName);
}

void CTestDlg::OnBnClickedBtnInject()
{	
	// 1. engine.dll 인젝션위한 msvc.dll을 SPI후킹을 통해 등록
	WSADATA	WSAData;
	if(WSAStartup(MAKEWORD(2, 2), &WSAData) == ERROR_SUCCESS)
	{
		PrintLog(L"InstallSPI");
		if (InstallSPI())
		{
			PrintLog(L"SPI Install Success");
		}
		else PrintLog(L"SPI Install Failed");
		WSACleanup();
	}

	/*OutputDebugString(g_szHSDVDriverName);
	GetSystemDirectory(g_szHSDVDriverName, MAX_PATH);
	wcscat(g_szHSDVDriverName, L"\\drivers\\HSDV.sys");	
	ExtractService_HSDV();
	if (InstallService_HSDV() == 0) // 0: ERR, 1: SUCC, 2:ALREADY_RUNNING
	{
		UninstallService_HSDV();
		InstallService_HSDV();
	}
	RemoveService_HSDV();*/


	// 2. engine.dll 과의 데이터통신위한 공유메모리생성
	m_hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,    
		SHARE_MEM_SIZE,    
		L"MAPFILE");   

	if(m_hMapFile==NULL)
	{
		OutputDebugStringA("CreateFileMapping() fail");
		return ;
	}

	m_pbMapBuf = (BYTE*) MapViewOfFile(m_hMapFile, // handle to map object
		FILE_MAP_ALL_ACCESS,  // read/write permission
		0, 
		0, 
		SHARE_MEM_SIZE);

	// engine.dll 로부터 데이터 받기위한 스레드
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)RecvThread, this, NULL, NULL);	
}
void CTestDlg::SendCommand(int nCommand)
{
	if (!m_bInjected) return;
	PrintLog(L"UI -> Send sendCommand : %x", nCommand);
	g_ShareInfo.nCommand = nCommand;
	CString strInput;
	char recog[10] = {0};
	DWORD dwVal;
	CString strBR1, strBR2;
	int cnt = 0;

	switch (nCommand)
	{
	case CMD_LOOK_MEM:
		{
			this->GetDlgItemText(IDC_EDIT_MEM, strInput);
			WideCharToMultiByte(CP_ACP, 0, strInput, -1, recog, 10, NULL, NULL);		
			sscanf(recog, "%08X", &dwVal);
			g_ShareInfo.dwData = dwVal;			
		}
		break;		
	case CMD_RUN_ASM:
		{
			CString strInput;
			char recog[10] = {0};
			this->GetDlgItemText(IDC_EDIT_FNNUM, strInput);
			DWORD dwVal = _wtoi(strInput.GetBuffer(0));
			g_ShareInfo.dwData = dwVal;
			PrintLog(L"Caller Index %d", dwVal);
		}
		break;	
	case CMD_INSTALLVEH:
		{			
			g_ShareInfo.dwData = 0;
			memset( g_ShareInfo.dwBRBuf, 0, sizeof(DWORD)*80 );
			cnt = 0;
			for (int i = 0; i < m_ctlBreakList.GetItemCount(); i ++)
			{
				strBR1 = m_ctlBreakList.GetItemText(i, 1);
				strBR2 = m_ctlBreakList.GetItemText(i, 2);
				if (strBR1 == "" || strBR2 == "") continue;
				WideCharToMultiByte(CP_ACP, 0, strBR1, -1, recog, 10, NULL, NULL);		
				sscanf(recog, "%08X", &dwVal);
				g_ShareInfo.dwBRBuf[cnt++] = dwVal;

				WideCharToMultiByte(CP_ACP, 0, strBR2, -1, recog, 10, NULL, NULL);		
				sscanf(recog, "%08X", &dwVal);
				g_ShareInfo.dwBRBuf[cnt++] = dwVal;
			}
		}
		break;
	default:
		break;
	}
	RefreshAutoInfo(TRUE);
	PrintLog(L"UI -> m_hSendEvent Set");
	SetEvent(m_hSendEvent);
	PrintLog(L"UI ->  m_hSendAnswerEvent Waite");
	WaitForSingleObject(m_hSendAnswerEvent, 1000);
	PrintLog(L"UI ->  m_hSendAnswerEvent Arrive");
}
BOOL CTestDlg::RefreshAutoInfo(BOOL bWrite)
{
	if(bWrite)				// 쓰기
	{
		PrintLog(L"UI-ShareMemory Write");
		memcpy((PVOID)m_pbMapBuf, &g_ShareInfo, sizeof(SHARE_INFO));
	}
	else					// 읽기
	{
		PrintLog(L"UI-ShareMemory Read");
		memcpy((PVOID)&g_ShareInfo, m_pbMapBuf, sizeof(SHARE_INFO));
	}
	return TRUE;
}
DWORD RecvThread(LPVOID pParam)
{
	WriteUIFileLog(L"Start Recv Thread");
	CTestDlg *pTestDlg = (CTestDlg*) pParam;
	while(TRUE)
	{
		WaitForSingleObject(pTestDlg->m_hRecvEvent, INFINITE);

		pTestDlg->RefreshAutoInfo(FALSE); 

		switch(g_ShareInfo.nCommand)
		{
		case CMD_READY:
			{
				pTestDlg->m_bInjected = TRUE;
				pTestDlg->SendCommand(CMD_INIT);
			}
			break;		
		default:
			break;
		}

		
		WriteUIFileLog(L"UI - Send m_hRecvAnswerEvent");
		SetEvent(pTestDlg->m_hRecvAnswerEvent);
	}
	return 0;
}

void CTestDlg::OnBnClickedBtnDump()
{
	// TODO: Add your control notification handler code here
	SendCommand(CMD_DUMP);
}

void CTestDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	WSADATA	WSAData;
	if(WSAStartup(MAKEWORD(2, 2), &WSAData) == ERROR_SUCCESS)
	{
		UninstallSPI();
		PrintLog(L"UninstallSPI");
		WSACleanup();
	}
	
	UninstallService_HSDV();	
	UninstallService_kbf();
	
	CDialog::OnClose();
}

void CTestDlg::OnBnClickedBtnWatchmem()
{
	SendCommand(CMD_LOOK_MEM);
}

void CTestDlg::OnBnClickedBtnHookvt()
{
	SendCommand(CMD_RUN_ASM);
}

void CTestDlg::OnBnClickedButton7()
{
	CString strCaption;
	UpdateData(TRUE);
	this->GetDlgItemText(IDC_BUTTON7, strCaption);
	if (strCaption == L"Install VEH")
	{
		SendCommand(CMD_INSTALLVEH);
		this->SetDlgItemText(IDC_BUTTON7, L"UnInstall VEH");
	}
	else
	{
		SendCommand(CMD_UNINSTALLVEH);
		this->SetDlgItemText(IDC_BUTTON7, L"Install VEH");
	}
}


void CTestDlg::OnBnClickedBtnBrAdd()
{
	UpdateData(TRUE);
	if (m_strBR1 == "" || m_strBR2 == "") return;
	int n = m_ctlBreakList.GetItemCount();
	CString str1;
	str1.Format(L"%d", n+1);
	m_ctlBreakList.InsertItem(n, str1);
	m_ctlBreakList.SetItemText(n, 1, m_strBR1);
	m_ctlBreakList.SetItemText(n, 2, m_strBR2);
	m_strBR1 = m_strBR2 = "";
	UpdateData(FALSE);
}

void CTestDlg::OnBnClickedBtnBrDel()
{
	if (m_nSelBRList < 0) {	AfxMessageBox(L"삭제할 항목을 선택하세요");	return;	}	
	m_ctlBreakList.DeleteItem(m_nSelBRList);
	m_nSelBRList = -1;
	m_strBR1 = m_strBR2 = "";
	UpdateData(FALSE);
}

void CTestDlg::OnBnClickedBtnBrCha()
{
	UpdateData(TRUE);
	if (m_strBR1 == "" || m_strBR2 == "") return;
	if (m_nSelBRList < 0) { AfxMessageBox(L"변경할 항목을 선택하세요"); return; }
	m_ctlBreakList.SetItemText(m_nSelBRList, 1, m_strBR1);
	m_ctlBreakList.SetItemText(m_nSelBRList, 2, m_strBR2);	
	m_strBR1 = m_strBR2 = "";
	UpdateData(FALSE);
}

void CTestDlg::OnNMClickListBreak(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNmListView = (LPNMLISTVIEW)pNMHDR;
	int sel = pNmListView->iItem;
	if (sel < 0) return;
	m_nSelBRList = sel;
	m_strBR1 = m_ctlBreakList.GetItemText(m_nSelBRList, 1);
	m_strBR2 = m_ctlBreakList.GetItemText(m_nSelBRList, 2);
	UpdateData(FALSE);
}

#include <imm.h>
void CTestDlg::OnBnClickedBtnBrInit()
{
	m_ctlBreakList.DeleteAllItems();
	m_nSelBRList = -1;
	m_strBR1 = m_strBR2 = "";
	UpdateData(FALSE);


	Sleep(2000);
	HWND hwnd = ::FindWindow(NULL, _T("제목 없음 - EmEditor"));
	if(hwnd != NULL)
	{
		PrintLog(L"메시지날리자");
		Sleep(1000);

		// 영문 -> 한글
// 		HIMC hIMC;
// 		if(!(hIMC=ImmGetContext(hwnd1))) {
// 			PrintLog(L"err = %d, %d", GetLastError(), hIMC);
// 			return;
// 		}
// 		ImmSetConversionStatus( hIMC, IME_CMODE_HANGEUL, IME_SMODE_NONE );
// 		ImmReleaseContext( hwnd1, hIMC );
		PrintLog(L"#######");

		WCHAR szString[] = L"조선";
		char szppp[MAX_PATH];


// 		wchar_t strUnicode[256] = {0,};
// 		char    strMultibyte[256] = {0,};
// 		wcscpy_s(strUnicode,256,L"유니코드");
// 		int len = WideCharToMultiByte( CP_ACP, 0, strUnicode, -1, NULL, 0, NULL, NULL );   
// 		WideCharToMultiByte( CP_ACP, 0, strUnicode, -1, strMultibyte, len, NULL, NULL );


		//strcpy(szppp, "akdfke");

	
		if(hwnd != NULL)
		{
			int nFirstChar    = 0xc7d0; // '학' 자임
			HWND hwnd1 = ::GetWindow(hwnd, GW_CHILD);
			//::SendMessage(hwnd1, WM_IME_CHAR, (WPARAM)nVal, 0L);

			char	cc1 = 0xc7;
			char	cc2 = 0xd0;

			::PostMessage(hwnd1, WM_IME_ENDCOMPOSITION, cc2, 0);
			::PostMessage(hwnd1, WM_IME_STARTCOMPOSITION, cc2, 0);
			::PostMessage(hwnd1, WM_IME_COMPOSITION, cc2, 0);
			::PostMessage(hwnd1, WM_IME_CHAR, cc2, 0);
			::PostMessage(hwnd1, WM_IME_COMPOSITION, cc1, 0);
			::PostMessage(hwnd1, WM_IME_CHAR, cc1, 0);
			::PostMessage(hwnd1, WM_IME_ENDCOMPOSITION, nFirstChar, 0);
		}
	}
}


BOOL CTestDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)) return FALSE;
	return CDialog::PreTranslateMessage(pMsg);
}



//////////////////////////////////////////////////////////////////////////


#define IOCTL_POSTMSG_DATA		CTL_CODE(FILE_DEVICE_UNKNOWN, 5, METHOD_BUFFERED, FILE_ANY_ACCESS)
HANDLE g_hMsDevice = NULL;
HWND g_GameHwnd = NULL;
struct POSTMSG
{
	DWORD hwnd;
	DWORD msg;
	DWORD wParam;
	DWORD lParam;
};
void CommonKey_Press(UINT nkey)
{
	UINT uCode = nkey;
	if ( nkey >= 0x61 && nkey - 0x61 <= 25 ) uCode = nkey - 0x20;	

	DWORD dwBytesReturned;

	POSTMSG psgInfo;
	psgInfo.hwnd = (DWORD)g_GameHwnd;
	psgInfo.msg	= WM_KEYDOWN;
	psgInfo.wParam = nkey;	

	psgInfo.lParam = (MapVirtualKeyA(uCode, 0) << 16) + 1;	
	DeviceIoControl(g_hMsDevice, IOCTL_POSTMSG_DATA, (LPVOID)&psgInfo, sizeof(POSTMSG), NULL, 0, &dwBytesReturned, NULL);
	//PostMessageA(g_GameHwnd, WM_KEYDOWN, nkey, (MapVirtualKeyA(uCode, 0) << 16) + 1); 
	Sleep(50);

	psgInfo.lParam = (MapVirtualKeyA(uCode, 0) << 16) + 0xC0000001;
	DeviceIoControl(g_hMsDevice, IOCTL_POSTMSG_DATA, (LPVOID)&psgInfo, sizeof(POSTMSG), NULL, 0, &dwBytesReturned, NULL);
	//PostMessageA(g_GameHwnd, WM_KEYUP, nkey, (MapVirtualKeyA(uCode, 0) << 16) + 0xC0000001);
}


//////////////////////////////////////////////////////////////////////////<<
CStringArray	g_saWordList;
void RandString(char * pszResult, int nLen)
{
	char szPat[37] = "0123456789abcdefghijklmnopqrstuvwxyz";
	//TCHAR szPat[37] = L"령일이삼사오육칠팔구에비시디예프쥐취아제케엘엠엔올피큐알엣티유브더엑와젯";
	//TCHAR szPat[37] = L"땡금아쉬프트공일포이같기투엔딱따구리탱크뚜껑웃대가노랑판다곰갈비뼉떠굴동";
	//TCHAR szPat[37] = L"땡금앗쉬프트굉잇포첫같긷투엔딱따굳릿탱크뚜껑웃대갇놋랑판담곰갈빈뼉떠굴동";
	for ( int i = 0 ; i < nLen ; i ++ ) pszResult[i] = szPat[rand() % 36];
}

void GetRandomActorName(char * pszName)
{
	srand(time(NULL));

	int nRan = rand();   
	PrintLogA("rand1 = %d", nRan);
	
	int nWordIdx = nRan % g_saWordList.GetCount();

	int n2 = g_saWordList[nWordIdx].GetLength();
	PrintLogA("word = %s, %d", g_saWordList[nWordIdx], n2);
	
	nRan = rand();		
	int n1 = nRan % 2;
	PrintLogA("rand2 = %d, n1 = %d", nRan, n1);
	
	nRan = rand();	
	int n3 = nRan % (12 - n2 - n1);
	PrintLogA("rand3 = %d, n3 = %d", nRan, n3);

	char sz1[12] = {0}, sz3[12] = {0};

	RandString(sz1, n1);
	PrintLogA("sz1 = %s", sz1);
	RandString(sz3, n3);	
	PrintLogA("sz3 = %s", sz3);
	
	sprintf(pszName, "%s%s%s", sz1, g_saWordList[nWordIdx].GetBuffer(), sz3);	

	PrintLogA("이름=%s, %d", pszName, strlen(pszName));
}

void ReadWordList()
{
// 	char szFilePath[MAX_PATH];
// 	sprintf(szFilePath, "%s\\www.dat", g_szModDir);
// 	PrintLogA("한글디비경로 = %s", szFilePath);
// 
// 	FILE * fp = NULL;
// 	fp = fopen(szFilePath, "rb");
// 	if ( fp == NULL ) return;
// 	char szLine[20];
// 	CString str;
// 	while ( fgets(szLine, 20, fp) )
// 	{
// 		szLine[strlen(szLine) - 2] = 0;	
// 
// 		str.Format("%s", szLine);
// 		g_saWordList.Add(str);
// 		PrintLogA("%s", str.GetBuffer());
// 	}
// 
// 	fclose(fp);
}
//////////////////////////////////////////////////////////////////////////>>

void checkIPAddress()
{  
	WORD wVersionRequested;
	WSADATA wsaData;
	char  szName[255] = {0};
	CString strIP; // 여기에 local ip가 저장.

	PHOSTENT hostinfo;
	wVersionRequested = MAKEWORD(2, 0);

	if (WSAStartup(wVersionRequested, &wsaData) == 0)
	{
		if (gethostname(szName, sizeof(szName)) == 0)
		{
			PrintLogA("host name = %s", szName);
			if ((hostinfo = gethostbyname(szName)) != NULL)
			{
				strIP = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
			}
		}     
		WSACleanup( );
	}
	PrintLogA("local ip = %s", strIP);
	return;
}

void CTestDlg::OnBnClickedButton8()
{
	//ReadWordList();
	//BYTE  szBotName[13] = {0};
	//DWORD len = 0;
	//GetRandomActorName((char*)szBotName);	

	checkIPAddress();

}


void ExtractService_kbf()
{
	RemoveService_kbf();

	HRSRC hResource = FindResource(NULL, MAKEINTRESOURCE(IDR_KEYDRIVER), L"BINARY");
	HGLOBAL hGlobal = LoadResource(NULL, hResource);
	LPVOID lpBuffer = LockResource(hGlobal);

	HANDLE hFile = NULL;
	DWORD dwSize = SizeofResource(NULL, hResource);
	DWORD dwBytesWritten = 0;

	hFile = CreateFile(g_szkbfDriverName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, 0, NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		WriteFile(hFile, lpBuffer, dwSize, &dwBytesWritten, NULL);
		CloseHandle(hFile);
	}
}
int InstallService_kbf()
{
	SC_HANDLE	hSCManager;
	SC_HANDLE	hService;
	int			iRet = 0;	
	char		szErr[MAX_PATH];

	hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(hSCManager)
	{
		hService = CreateService(hSCManager, L"HelloDDK", L"HelloDDK", SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_AUTO_START, SERVICE_ERROR_NORMAL, 
			g_szkbfDriverName, NULL, NULL, NULL, NULL, NULL);

		if(!hService) hService = OpenService(hSCManager, L"HelloDDK", SERVICE_ALL_ACCESS);

		if(hService)
		{
			if(StartService(hService, 0, NULL))
			{
				OutputDebugStringA("Install--TestDrv service was successfully installed.\n");
				iRet = 1;
			}
			else if (GetLastError() == ERROR_SERVICE_ALREADY_RUNNING)
			{
				OutputDebugStringA("Install--TestDrv service already running.\n");
				iRet = 2;
			}
			else 
			{
				sprintf(szErr, "Install-- TestDrv service failed1: err = %d", GetLastError());
				OutputDebugStringA(szErr);			
			}
			CloseServiceHandle(hService);
		}
		else 
		{
			sprintf(szErr, "Install--TestDrv OpenService service failed: err = %d", GetLastError());
			OutputDebugStringA(szErr);
		}
		CloseServiceHandle(hSCManager);
	}
	else
	{
		//OutputDebugStringA("Install--hsdv Create Service Manager Failed.\n");
	}
	return iRet;
}

void UninstallService_kbf()
{
	SC_HANDLE hSCManager;
	SC_HANDLE hService; 
	SERVICE_STATUS serviceStatus;

	hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS); 
	if(hSCManager)
	{
		hService = OpenService(hSCManager, L"HelloDDK", SERVICE_ALL_ACCESS); 
		if(hService)
		{
			ControlService(hService, SERVICE_CONTROL_STOP , &serviceStatus);
			if(DeleteService(hService))
			{
				//OutputDebugStringA("UnInstall--service was successfully uninstalled.\n");
			}
			CloseServiceHandle(hService);
		}
		//else OutputDebugStringA("UnInstall--OpenService service failed.\n");

		CloseServiceHandle(hSCManager);
	}
	//else OutputDebugStringA("UnInstall--Create Service Manager Failed.\n");
}

void RemoveService_kbf()
{
	DeleteFile(g_szkbfDriverName);
}
