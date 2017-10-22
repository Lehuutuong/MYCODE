// AntiKeylogger.h : main header file for the ANTIKEYLOGGER application
//

#if !defined(AFX_ANTIKEYLOGGER_H__8988949B_D044_480E_B2A6_452116905B39__INCLUDED_)
#define AFX_ANTIKEYLOGGER_H__8988949B_D044_480E_B2A6_452116905B39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAntiKeyloggerApp:
// See AntiKeylogger.cpp for the implementation of this class
//

class CAntiKeyloggerApp : public CWinApp
{
public:
	CAntiKeyloggerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAntiKeyloggerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAntiKeyloggerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANTIKEYLOGGER_H__8988949B_D044_480E_B2A6_452116905B39__INCLUDED_)
