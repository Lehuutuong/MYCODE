#if !defined(AFX_ADVANCEDOPTIONPAGE_H__7A39C7D2_8A42_43CC_9C10_DE91F4858719__INCLUDED_)
#define AFX_ADVANCEDOPTIONPAGE_H__7A39C7D2_8A42_43CC_9C10_DE91F4858719__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdvancedOptionPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdvancedOptionPage dialog

class CAdvancedOptionPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CAdvancedOptionPage)

// Construction
public:
	CAdvancedOptionPage();
	~CAdvancedOptionPage();

// Dialog Data
	//{{AFX_DATA(CAdvancedOptionPage)
	enum { IDD = IDD_OPTION_ADVANCED };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAdvancedOptionPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAdvancedOptionPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADVANCEDOPTIONPAGE_H__7A39C7D2_8A42_43CC_9C10_DE91F4858719__INCLUDED_)
