#if !defined(AFX_USEROPTIONPAGE_H__6B514C72_4F3C_4385_91BD_9BFD4AF0F149__INCLUDED_)
#define AFX_USEROPTIONPAGE_H__6B514C72_4F3C_4385_91BD_9BFD4AF0F149__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserOptionPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserOptionPage dialog

class CUserOptionPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CUserOptionPage)

// Construction
public:
	CUserOptionPage();
	~CUserOptionPage();

// Dialog Data
	//{{AFX_DATA(CUserOptionPage)
	enum { IDD = IDD_OPTION_USER };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CUserOptionPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CUserOptionPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USEROPTIONPAGE_H__6B514C72_4F3C_4385_91BD_9BFD4AF0F149__INCLUDED_)
