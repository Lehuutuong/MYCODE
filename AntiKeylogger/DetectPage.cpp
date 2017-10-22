// DetectPage.cpp : implementation file
//

#include "stdafx.h"
#include "AntiKeylogger.h"
#include "DetectPage.h"
#include "AntiKeyloggerDlg.h"
#include "Constdef.h"
#include "FileInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDetectPage property page

IMPLEMENT_DYNCREATE(CDetectPage, CPropertyPage)

CDetectPage::CDetectPage() : CPropertyPage(CDetectPage::IDD)
{
	//{{AFX_DATA_INIT(CDetectPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pImageList = NULL;
}

CDetectPage::~CDetectPage()
{
}

void CDetectPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetectPage)
	DDX_Control(pDX, IDC_DETECT_LIST, m_listDetect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDetectPage, CPropertyPage)
	//{{AFX_MSG_MAP(CDetectPage)
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_DETECT_LIST, OnDblclkDetectList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetectPage message handlers

void CDetectPage::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CPropertyPage::OnShowWindow(bShow, nStatus);
	
	if(bShow) ShowList();	
}

void CDetectPage::ShowList()
{
	CAntiKeyloggerDlg* pDlg = (CAntiKeyloggerDlg*)GetParent();
	CFileInfo* pInfo;
	
	ClearList();

	LVITEMW lvi;
	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	
	if(pDlg->m_list294.GetSize() == 0) return;
	for(int i = 0; i < pDlg->m_list294.GetSize(); i ++) {
		pInfo = pDlg->m_list294.GetAt(i);
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.iImage = pInfo->m_pbState[1] ? 0 : 1;
		lvi.lParam = (LPARAM)pInfo;
		lvi.pszText = pInfo->m_strFileName;
		m_listDetect.SendMessage(LVM_INSERTITEMW, 0, (LPARAM)&lvi);
		lvi.iSubItem = 1;
		lvi.pszText = pInfo->m_strInfo;
		m_listDetect.SendMessage(LVM_SETITEMTEXTW, i, (LPARAM)&lvi);		
	}
}

void CDetectPage::ClearList()
{
	m_listDetect.DeleteAllItems();
}

BOOL CDetectPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	LV_COLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM;

	lvc.iSubItem = 0;
	lvc.cx  = 150;
	lvc.fmt = LVCFMT_LEFT;
	m_listDetect.InsertColumn(0, &lvc);
	lvc.iSubItem = 1;
	lvc.cx  = 480;
	m_listDetect.InsertColumn(1, &lvc);

	m_pImageList = new CImageList();
	
	m_pImageList->Create(32, 32, ILC_COLOR32 | ILC_MASK, 0, 2);	
	m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_TRAY_ACTIVE));
	m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_TRAY_DEACTIVE));
	m_listDetect.SetImageList(m_pImageList, LVSIL_SMALL);

	DWORD dwStyle = m_listDetect.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_FLATSB | LVS_EX_INFOTIP;
	m_listDetect.SetExtendedStyle(dwStyle);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDetectPage::OnDestroy() 
{
	CPropertyPage::OnDestroy();
	
	ClearList();
	delete m_pImageList;
	m_pImageList = NULL;
}

void CDetectPage::OnDblclkDetectList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_listDetect.GetItemCount() == 0/* || ((CAntiKeyloggerDlg*)GetParent())->m_b1FC == FALSE*/) return;
	POSITION pos = m_listDetect.GetFirstSelectedItemPosition();
	if(pos == NULL) return;
	int nIndex = m_listDetect.GetNextSelectedItem(pos);
	CFileInfo* pInfo = (CFileInfo*)m_listDetect.GetItemData(nIndex);
	if(pInfo) {
		pInfo->m_pbState[0] = pInfo->m_pbState[1] ? 0 : 1;
		pInfo->m_pbState[1] = pInfo->m_pbState[0];
		sub_43F270(((CAntiKeyloggerDlg*)GetParent())->m_p29C);
	}
	ShowList();
	*pResult = 0;
}
