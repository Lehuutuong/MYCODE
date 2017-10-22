// NotifyDialog.cpp : implementation file
//

#include "stdafx.h"
#include "AntiKeylogger.h"
#include "NotifyDialog.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNotifyDialog dialog


CNotifyDialog::CNotifyDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CNotifyDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNotifyDialog)
	//}}AFX_DATA_INIT
	m_strNotify = _T("");
	m_pParent0A0 = pParent;
	m_hEvent088 = CreateEventW(NULL, TRUE, TRUE, NULL);
}


void CNotifyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNotifyDialog)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNotifyDialog, CDialog)
	//{{AFX_MSG_MAP(CNotifyDialog)
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	ON_WM_SYSCOMMAND()
	ON_WM_TIMER()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNotifyDialog message handlers

void CNotifyDialog::OnClose() 
{
	return;
}

void CNotifyDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	HWND hTrayWnd;
	APPBARDATA Data;
	RECT rect;
	int cxScreen, cyScreen;

	m_n0A4 = 0;
	if(bShow == TRUE) {
		cxScreen = GetSystemMetrics(SM_CXSCREEN);
		cyScreen = GetSystemMetrics(SM_CYSCREEN);
		hTrayWnd = FindWindowW(L"Shell_TrayWnd", NULL);
		if(hTrayWnd == NULL) {
			m_n094 = cyScreen;
			m_n098 = cxScreen;
			m_n09C = cyScreen + m_n0AC;
			m_n090 = cxScreen - m_n0A8;
			m_n0A4 = cyScreen - m_n0AC;
		}
		else {
			Data.cbSize = 0x24;
			Data.hWnd = hTrayWnd;
			SHAppBarMessage(ABM_GETTASKBARPOS, &Data);
			::GetWindowRect(hTrayWnd, &rect);
			m_n0B8 = Data.uEdge;
			switch(Data.uEdge)
			{
			case ABE_LEFT:
				m_n098 = 1;
				m_n090 = 1 - m_n0A8;
				m_n09C = cyScreen;
				m_n094 = cyScreen - m_n0AC;
				m_n0A4 = m_n0A8 + rect.right;
				break;
			case ABE_RIGHT:
				m_n09C = cyScreen;
				m_n098 = m_n0A8 + cxScreen - 1;
				m_n090 = cxScreen - 1;
				m_n094 = cyScreen - m_n0AC;
				m_n0A4 = rect.left - m_n0A8;
				break;
			case ABE_TOP:
				m_n098 = cxScreen;
				m_n090 = cyScreen - m_n0A8;
				m_n09C = 1;
				m_n094 = 1 - m_n0AC;
				m_n0A4 = m_n0AC + rect.bottom;
				break;
			case ABE_BOTTOM:
				m_n098 = cxScreen;
				m_n090 = cxScreen - m_n0A8;
				m_n09C = cyScreen + m_n0AC - 1;
				m_n094 = cyScreen - 1;
				m_n0A4 = rect.top - m_n0AC;
				break;
			}
		}
		SetWindowPos(&wndTopMost, m_n090, m_n094, m_n098 - m_n090, m_n09C - m_n094, 0);
		SetTimer(1, 25, NULL);		
	}

	CDialog::OnShowWindow(bShow, nStatus);
}

void CNotifyDialog::OnSysCommand(UINT nID, LPARAM lParam) 
{
	return;
}

void CNotifyDialog::OnTimer(UINT nIDEvent) 
{
	int cxScreen, cyScreen;
	BOOL bShow, bStop;
	DWORD sign;

	cxScreen = GetSystemMetrics(SM_CXSCREEN);
	cyScreen = GetSystemMetrics(SM_CYSCREEN);
	
	switch(nIDEvent)
	{
	case 2:// 2초간 지연된다.
		KillTimer(2);
		SetTimer(3, 25, NULL);
		return;
	case 1:// 통보창이 Tray우에까지 올라간다.
	case 3:// 통보창이 Tray아래로 내려간다.
		{
			bShow = (nIDEvent == 1);
			switch(m_n0B8)
			{
			case ABE_LEFT:
				sign = (bShow == TRUE);
				sign = sign * 2 - 1;
				m_n090 += 5 * sign;
				m_n098 += 5 * sign;
				if(bShow == FALSE)
					bStop = (m_n098 <= 0);
				else
					bStop = (m_n098 >= m_n0A4);
				break;
			case ABE_TOP:
				sign = (bShow == TRUE);
				sign = sign * 2 - 1;
				m_n094 += 5 * sign;
				m_n09C += 5 * sign;
				if(bShow == FALSE)
					bStop = (m_n09C <= 0);
				else
					bStop = (m_n09C >= m_n0A4);
				break;
			case ABE_RIGHT:
				sign = (bShow == FALSE);
				sign = sign * 2 - 1;
				m_n090 += 5 * sign;
				m_n098 += 5 * sign;
				if(bShow == FALSE)
					bStop = (m_n090 >= cxScreen);
				else
					bStop = (m_n090 <= m_n0A4);
				break;
			case ABE_BOTTOM:
				sign = (bShow == FALSE);
				sign = sign * 2 - 1;
				m_n094 += 5 * sign;
				m_n09C += 5 * sign;
				if(bShow == FALSE)
					bStop = (m_n094 >= cyScreen);
				else
					bStop = (m_n094 <= m_n0A4);
				break;
			}
			SetWindowPos(&wndTopMost, m_n090, m_n094, m_n098 - m_n090, m_n09C - m_n094, 0);
			if(bStop == FALSE) return;
			KillTimer(nIDEvent);
			if(bShow == FALSE)
			{
				sub_423FB0();
				return;
			}
			else
			{
				SetTimer(2, 0x7D0, NULL);
				return;
			}
		}
	}
	
	CDialog::OnTimer(nIDEvent);
}

BOOL CNotifyDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect rect;
	BITMAP bmp;
	if(m_Bmp.LoadBitmap(IDB_NOTIFY))
	{
		m_Bmp.GetBitmap(&bmp);
		rect.SetRect(0, 0, bmp.bmWidth, bmp.bmHeight);
		MoveWindow(rect);
	}	
	GetWindowRect(&rect);
	m_n0A8 = rect.Width();		//	273
	m_n0AC = rect.Height();		//	116
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNotifyDialog::sub_423FB0()
{
	KillTimer(1);
	KillTimer(2);
	KillTimer(3);
	ShowWindow(SW_HIDE);
	m_n090 = 0;
	m_n094 = 0;
	m_n098 = 0;
	m_n09C = 0;
	SetEvent(m_hEvent088);
}

void CNotifyDialog::sub_423EB0(PWSTR strFile)
{
	char szPath[MAX_PATH];
	if(WaitForSingleObject(m_hEvent088, 0) == WAIT_TIMEOUT) return;
	ResetEvent(m_hEvent088);
	wcstombs(szPath, strFile, MAX_PATH);
	m_strNotify.Format("%s is detected.", szPath);
	GetDlgItem(IDC_NOTIFY)->SetWindowText(m_strNotify);
	KillTimer(1);
	KillTimer(2);
	KillTimer(3);
	ShowWindow(SW_SHOW);
}

void CNotifyDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;
	GetClientRect(rect);
	GetDlgItem(IDC_NOTIFY)->ShowWindow(SW_HIDE);
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap *pOldBmp = dcMem.SelectObject(&m_Bmp);
	BITMAP bmp;
	m_Bmp.GetBitmap(&bmp);
	dc.BitBlt(0, 0, bmp.bmWidth, bmp.bmHeight, &dcMem, 0, 0, SRCCOPY);
	rect.SetRect(rect.left + 10, rect.top + 10, rect.right - 10, rect.bottom - 10);
	int nOldMode = dc.SetBkMode(TRANSPARENT);
	COLORREF clrOld = dc.SetTextColor(RGB(255, 255, 255));
	dc.DrawText(m_strNotify, m_strNotify.GetLength(), rect, DT_WORDBREAK);
	dc.SetTextColor(clrOld);
	dc.SetBkMode(nOldMode);
	dcMem.SelectObject(pOldBmp);
}
