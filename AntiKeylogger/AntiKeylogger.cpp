// AntiKeylogger.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AntiKeylogger.h"
#include "AntiKeyloggerDlg.h"
#include "Global.h"
#include "Init.h"
#include "ntdll.h"
#include "logo.h"
#include "DetectPage.h"
#include "WhitePage.h"
#include "UserOptionPage.h"
#include "AdvancedOptionPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL bool_445D20 = FALSE;
HANDLE hEvent_Exit = NULL;
/////////////////////////////////////////////////////////////////////////////
// CAntiKeyloggerApp

BEGIN_MESSAGE_MAP(CAntiKeyloggerApp, CWinApp)
	//{{AFX_MSG_MAP(CAntiKeyloggerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAntiKeyloggerApp construction

CAntiKeyloggerApp::CAntiKeyloggerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAntiKeyloggerApp object

CAntiKeyloggerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAntiKeyloggerApp initialization

BOOL CAntiKeyloggerApp::InitInstance()
{
	AfxEnableControlContainer();

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	if(GetOSVersion() == NOT_SUPPORTED)
	{
		sub_40AAF0(L"Sorry, this program supports only Win2K/XP.", MB_OK | MB_ICONEXCLAMATION | MB_SYSTEMMODAL, NULL);
		return FALSE;
	}

	if(!Start()) {
		End();
		return FALSE;
	}

	OleInitialize(NULL);

	RegisterMessages();
	CAntiKeyloggerDlg* pDlg = new CAntiKeyloggerDlg();
	SetAutoRun();
	if(TEST(pDlg->m_dw0D0[4], 1) == 0 && bool_445D20 == FALSE) {
		HANDLE hThread = ::CreateThread(NULL, 0, sub_4389E0, NULL, 0, NULL);
		CloseHandle(hThread);
	}
	bool_445D20 = TRUE;
	pDlg->AddPage(NULL, _T("Protection policy"), 0);
	pDlg->AddPage(RUNTIME_CLASS(CDetectPage), _T("Black List"), IDD_DETECT, _T("Protection policy"));
	pDlg->AddPage(RUNTIME_CLASS(CWhitePage), _T("White List"), IDD_WHITE, _T("Protection policy"));
//	pDlg->AddPage(NULL, _T("Options"), IDD_OPTION_USER);
//	pDlg->AddPage(RUNTIME_CLASS(CUserOptionPage), _T("User"), IDD_OPTION_USER, _T("Options"));
//	pDlg->AddPage(RUNTIME_CLASS(CAdvancedOptionPage), _T("Advanced"), IDD_OPTION_ADVANCED, _T("Options"));
	
	pDlg->SetTitle("AntiKeylogger");
	pDlg->SetLogoText("AntiKeylogger");	
	pDlg->Create(IDD_ANTIKEYLOGGER_DIALOG);
	m_pMainWnd = pDlg;
	hWndMain = pDlg->m_hWnd;
		
	MSG msg;
	while(GetMessageW(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
		if(hEvent_Exit && WaitForSingleObject(hEvent_Exit, 0) == ERROR_SUCCESS) break;
	}

	OleUninitialize();
	sub_410B70();

/*	PPEB peb = GetPEB();
	STARTUPINFOW StartupInfo;
	PROCESS_INFORMATION ProcInfo;
	RtlZeroMemory(&StartupInfo, sizeof(STARTUPINFOW));
	StartupInfo.cb = sizeof(STARTUPINFOW);
	CreateProcessW(peb->ProcessParameters->ImagePathName.Buffer, NULL, NULL, NULL, FALSE, 0, NULL, 
		peb->ProcessParameters->CurrentDirectoryPath.Buffer, &StartupInfo, &ProcInfo);
*/	SetEvent(hEvent_445E28);
	
	End();
	
	return FALSE;
}
