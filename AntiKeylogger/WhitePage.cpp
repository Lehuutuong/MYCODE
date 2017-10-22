// WhitePage.cpp : implementation file
//

#include "stdafx.h"
#include "AntiKeylogger.h"
#include "WhitePage.h"
#include "AntiKeyloggerDlg.h"
#include "FileInfo.h"
#include "Comm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWhitePage property page

IMPLEMENT_DYNCREATE(CWhitePage, CPropertyPage)

CWhitePage::CWhitePage() : CPropertyPage(CWhitePage::IDD)
{
	//{{AFX_DATA_INIT(CWhitePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CWhitePage::~CWhitePage()
{
}

void CWhitePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWhitePage)
	DDX_Control(pDX, IDC_WHITE_LIST, m_listWhite);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWhitePage, CPropertyPage)
	//{{AFX_MSG_MAP(CWhitePage)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWhitePage message handlers

void CWhitePage::OnAdd() 
{
	CAntiKeyloggerDlg* pDlg = (CAntiKeyloggerDlg*)GetParent();
	OPENFILENAMEW ofn;
	WCHAR wsPath[MAX_PATH];
	CFileInfo* pInfo;
	PWSTR str;
	LVITEMW lvi;
	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetParent()->m_hWnd;
	ofn.lpstrFile = wsPath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = L"실행가능한 파일(*.exe)\0*.exe\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrTitle = L"파일을 선택하시오";
	ofn.lpstrDefExt = L"exe";
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if(!GetOpenFileNameW(&ofn)) return;
	if(wcsicmp(wsPath + wcslen(wsPath) - 4, L".exe") != 0) return;
	for(int i = 0; i < m_listWhite.GetItemCount(); i ++) {
		pInfo = (CFileInfo*)m_listWhite.GetItemData(i);
		if(wcsicmp(wsPath, pInfo->m_strPathName) == 0) return;
	}

	DWORD dwBuffer[4];
	BYTE* tmp = (BYTE*)dwBuffer;
	str = (PWSTR)RtlAllocateHeap(g_hProcessHeap, HEAP_ZERO_MEMORY, wcslen(wsPath) * 2 + 2);
	wcscpy(str, wsPath);
	tmp[0] = 0; tmp[1] = 1; tmp[2] = 1;
	dwBuffer[1] = (DWORD)str;
	pInfo = new CFileInfo(dwBuffer);
	pDlg->m_list298.Add(pInfo);
	ShowList();

	PSTRUCTD structd = sub_4107C0(str, wcslen(str) * 2 + 2);
	structd->Code = 0x16;
	sub_410530(pStruct_445EE0, NULL, NULL, structd);
}

void CWhitePage::OnRemove() 
{
	if(m_listWhite.GetItemCount() == 0) return;
	POSITION pos = m_listWhite.GetFirstSelectedItemPosition();
	if(pos == NULL) return;
	int nIndex = m_listWhite.GetNextSelectedItem(pos);
	CFileInfo* pInfo = (CFileInfo*)m_listWhite.GetItemData(nIndex);
	CAntiKeyloggerDlg* pDlg = (CAntiKeyloggerDlg*)GetParent();

	pDlg->m_list298.RemoveAt(nIndex);
	ShowList();

	PSTRUCTD structd = sub_4107C0(pInfo->m_strPathName, wcslen(pInfo->m_strPathName) * 2 + 2);
	structd->Code = 0x17;
	sub_410530(pStruct_445EE0, NULL, NULL, structd);
	delete pInfo;
}

void CWhitePage::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CPropertyPage::OnShowWindow(bShow, nStatus);
	
	if(bShow) ShowList();	
}

void CWhitePage::ShowList()
{
	CAntiKeyloggerDlg* pDlg = (CAntiKeyloggerDlg*)GetParent();
	CFileInfo* pInfo;
	
	ClearList();

	LVITEMW lvi;
	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	
	if(pDlg->m_list298.GetSize() == 0) return;
	for(int i = 0; i < pDlg->m_list298.GetSize(); i ++) {
		pInfo = pDlg->m_list298.GetAt(i);
		m_pImageList->Add(pInfo->m_hIcon);
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.iImage = i;
		lvi.lParam = (LPARAM)pInfo;
		lvi.pszText = pInfo->m_strFileName;
		m_listWhite.SendMessage(LVM_INSERTITEMW, 0, (LPARAM)&lvi);
		lvi.iSubItem = 1;
		lvi.pszText = pInfo->m_strInfo;
		m_listWhite.SendMessage(LVM_SETITEMTEXTW, i, (LPARAM)&lvi);				
	}
}

void CWhitePage::ClearList()
{
	CFileInfo* pInfo;

	for(int i = 0; i < m_pImageList->GetImageCount(); i ++) {
		m_pImageList->Remove(i);
	}
	m_listWhite.DeleteAllItems();
}

BOOL CWhitePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	LV_COLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM;

	lvc.iSubItem = 0;
	lvc.cx  = 150;
	lvc.fmt = LVCFMT_LEFT;
	m_listWhite.InsertColumn(0, &lvc);
	lvc.iSubItem = 1;
	lvc.cx  = 480;
	m_listWhite.InsertColumn(1, &lvc);

	m_pImageList = new CImageList();
	
	m_pImageList->Create(32, 32, ILC_COLOR32 | ILC_MASK, 0, 2);	
	m_listWhite.SetImageList(m_pImageList, LVSIL_SMALL);

	DWORD dwStyle = m_listWhite.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_FLATSB | LVS_EX_INFOTIP;
	m_listWhite.SetExtendedStyle(dwStyle);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWhitePage::OnDestroy() 
{
	CPropertyPage::OnDestroy();
	
	ClearList();
	delete m_pImageList;
	m_pImageList = NULL;	
}
