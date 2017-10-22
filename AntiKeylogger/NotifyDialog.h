#if !defined(AFX_NOTIFYDIALOG_H__A8DC770E_A93C_4F19_9EA4_52DED9BE3821__INCLUDED_)
#define AFX_NOTIFYDIALOG_H__A8DC770E_A93C_4F19_9EA4_52DED9BE3821__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NotifyDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNotifyDialog dialog

class CNotifyDialog : public CDialog
{
// Construction
public:
	void sub_423EB0(PWSTR strFile);
	void sub_423FB0();
	CBitmap m_Bmp;
	CWnd* m_pParent0A0;
	int m_n090, m_n094, m_n098, m_n09C, m_n0A4, m_n0A8, m_n0AC, m_n0B8;
	HANDLE m_hEvent088;
	CNotifyDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNotifyDialog)
	enum { IDD = IDD_NOTIFY };
	CString	m_strNotify;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNotifyDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNotifyDialog)
	afx_msg void OnClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOTIFYDIALOG_H__A8DC770E_A93C_4F19_9EA4_52DED9BE3821__INCLUDED_)
