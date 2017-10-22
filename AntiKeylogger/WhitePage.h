#if !defined(AFX_WHITEPAGE_H__3406599D_5635_4982_AABE_A32F7331F3DC__INCLUDED_)
#define AFX_WHITEPAGE_H__3406599D_5635_4982_AABE_A32F7331F3DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WhitePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWhitePage dialog

class CWhitePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CWhitePage)

// Construction
public:
	CImageList* m_pImageList;
	void ClearList();
	void ShowList();
	CWhitePage();
	~CWhitePage();

// Dialog Data
	//{{AFX_DATA(CWhitePage)
	enum { IDD = IDD_WHITE };
	CListCtrl	m_listWhite;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWhitePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWhitePage)
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WHITEPAGE_H__3406599D_5635_4982_AABE_A32F7331F3DC__INCLUDED_)
