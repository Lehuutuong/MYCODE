// AntiKeyloggerDlg.h : header file
//

#if !defined(AFX_ANTIKEYLOGGERDLG_H__4586BCF6_D7F3_4174_BBFC_166D2FED7E6D__INCLUDED_)
#define AFX_ANTIKEYLOGGERDLG_H__4586BCF6_D7F3_4174_BBFC_166D2FED7E6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "PrefsStatic.h"
#include "TrayDialog.h"
#include "NotifyDialog.h"
#include "FileInfo.h"
#include "ThreadDataList.h"

typedef class _PageInfo		PAGE_INFO;

class _PageInfo
{
public:
	BOOL		bViewClass;			// View flag for runtime checking
	UINT		nID;				// Resource ID for the Page
	UINT		nOrder;				// Page order number
	CWnd		*pWnd;				// Pointer to the page
	CWnd		*pWndParent;		// Pointer to the parent page if has
	CString		csCaption;			// Caption on the tree
	CString		csParentCaption;	// Caption of the parent on the tree
};

typedef CTypedPtrArray <CPtrArray, PAGE_INFO*>		PAGE_LIST;
typedef CMap<CWnd *, CWnd *, DWORD, DWORD&>			WNDTREE_MAP;
typedef CTypedPtrArray <CPtrArray, CFileInfo*>		FILE_LIST;

/////////////////////////////////////////////////////////////////////////////
// CAntiKeyloggerDlg dialog

class CAntiKeyloggerDlg : public CTrayDialog
{
// Construction
public:
	int m_n0C4;
	CNotifyDialog* m_pDlg21C;
	BOOL m_b1FC;
	BOOL m_b2A4;
	HICON m_hIcon1B0, m_hIcon1B8;
	BOOL m_b0F0;
	DWORD m_dw0D0[8];
	BOOLEAN m_b23C, m_b23D, m_b23E;
	HANDLE m_h270, m_h2AC;
	RTL_CRITICAL_SECTION m_rcs240;
	RTL_CRITICAL_SECTION m_rcs224;
	RTL_CRITICAL_SECTION m_rcs258;
	RTL_CRITICAL_SECTION m_rcs0F4;
	RTL_CRITICAL_SECTION m_rcs274;
	PSTRUCTK m_p29C;
	CThreadDataList	m_ThreadData28C;
	FILE_LIST m_list294, m_list298;

	CAntiKeyloggerDlg(CWnd* pParent = NULL);	// standard constructor
	~CAntiKeyloggerDlg();

public:
	void sub_436F60();
	void sub_4367D0();
	DWORD sub_436890(BOOL bFlag);
	DWORD sub_436960(BOOL bFlag);
	void sub_436200();
	void sub_435D90();
	void sub_436500();
	void sub_436260();
	void sub_435CB0();
	void ExpandTree();
	BOOL DestroyPages();
	BOOL CreatePage( const PAGE_INFO *pInfo);
	void ShowPage(const PAGE_INFO *pInfo, UINT nShow = SW_SHOW);
	void SetLogoText(CString sText);
	void SetTitle(CString sTitle);
	HTREEITEM GetNextItemCOrS(HTREEITEM hItem);
	HTREEITEM FindItem(const CString &csCaption);
	HTREEITEM FindItem(CWnd *pWnd);
	CWnd* AddPage(CRuntimeClass *pWndClass, const char *pCaption, UINT nID,const char *pParentCaption);
	CWnd* AddPage(CRuntimeClass *pWndClass, const char *pCaption, UINT nID = 0,CWnd *pDlgParent = NULL);

// Dialog Data
	PAGE_LIST		m_pInfo;		// Containing page info
	CRect			m_FrameRect;	// Rectangle size of a setting page
	CString			m_csTitle;		// Title of selected page
	CString			m_csLogoText;	// Logo text
	WNDTREE_MAP		m_wndMap;		// MFC CMap class for internal page management

	virtual BOOL CreateWnd(CWnd *pWnd, CCreateContext *pContext = NULL);
// Dialog Data
	//{{AFX_DATA(CAntiKeyloggerDlg)
	enum { IDD = IDD_ANTIKEYLOGGER_DIALOG };
	CPrefsStatic	m_CaptionBarCtrl;
	CTreeCtrl	m_TreeCtrl;
	CStatic	m_PageFrame;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAntiKeyloggerDlg)
	protected:
	virtual void OnTrayLButtonDblClk(CPoint pt);
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	void ExpandBranch(HTREEITEM hti);
	void InitTreeCtrl();
	BOOL RefreshData();

	// Generated message map functions
	//{{AFX_MSG(CAntiKeyloggerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnGetdispinfoTreeCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedTreeCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStop();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

DWORD WINAPI sub_403080( LPVOID lpParam );
void sub_434FF0(CAntiKeyloggerDlg* pDlg, BOOL bMinimize);
DWORD WINAPI sub_432F40(LPVOID lpParameter);
DWORD WINAPI sub_4315E0(LPVOID lpParameter);
DWORD WINAPI sub_435F60(LPVOID lpParameter);
DWORD WINAPI sub_435EC0(LPVOID lpParameter);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANTIKEYLOGGERDLG_H__4586BCF6_D7F3_4174_BBFC_166D2FED7E6D__INCLUDED_)
