#if !defined(AFX_DETECTPAGE_H__CBA6489C_764D_4A8B_8C55_52E4CF2FDEAD__INCLUDED_)
#define AFX_DETECTPAGE_H__CBA6489C_764D_4A8B_8C55_52E4CF2FDEAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetectPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDetectPage dialog

class CDetectPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CDetectPage)

// Construction
public:
	CImageList* m_pImageList;
	void ClearList();
	void ShowList();
	CDetectPage();
	~CDetectPage();

// Dialog Data
	//{{AFX_DATA(CDetectPage)
	enum { IDD = IDD_DETECT };
	CListCtrl	m_listDetect;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDetectPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDetectPage)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnDblclkDetectList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETECTPAGE_H__CBA6489C_764D_4A8B_8C55_52E4CF2FDEAD__INCLUDED_)
