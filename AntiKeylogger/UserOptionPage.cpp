// UserOptionPage.cpp : implementation file
//

#include "stdafx.h"
#include "AntiKeylogger.h"
#include "UserOptionPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserOptionPage property page

IMPLEMENT_DYNCREATE(CUserOptionPage, CPropertyPage)

CUserOptionPage::CUserOptionPage() : CPropertyPage(CUserOptionPage::IDD)
{
	//{{AFX_DATA_INIT(CUserOptionPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CUserOptionPage::~CUserOptionPage()
{
}

void CUserOptionPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserOptionPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserOptionPage, CPropertyPage)
	//{{AFX_MSG_MAP(CUserOptionPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserOptionPage message handlers
