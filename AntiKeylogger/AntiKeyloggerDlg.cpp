// AntiKeyloggerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AntiKeylogger.h"
#include "AntiKeyloggerDlg.h"
#include "Global.h"
#include "password.h"
#include "Comm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT message_445828, message_44582C, message_445830, message_445834;
UINT message_445838, message_44583C, message_445840, message_445844;
UINT message_445848, message_44584C, message_445850, message_445854;
UINT message_445858, message_44585C, message_445860, message_445864;

DWORD dword_44586C = 0;
BYTE byte_444010 = 0;
BOOL bool_445818 = FALSE;

#define IDANI_OPEN          1
#define IDANI_CLOSE         2
#define IDANI_CAPTION       3

/////////////////////////////////////////////////////////////////////////////
// CAntiKeyloggerDlg dialog

CAntiKeyloggerDlg::CAntiKeyloggerDlg(CWnd* pParent /*=NULL*/)
	: CTrayDialog(CAntiKeyloggerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAntiKeyloggerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pInfo.RemoveAll();	// Empty the page info
	m_csTitle = _T("");

	m_b0F0 = TRUE;
	m_hIcon1B0 = NULL;
	m_hIcon1B8 = NULL;
	m_b1FC = TRUE;
	m_b2A4 = FALSE;
	m_b23C = FALSE;
	m_b23D = FALSE;
	m_n0C4 = 0;
	sub_436960(FALSE);
	if(dword_44438C == 1) m_b0F0 = FALSE;
	m_hIcon1B0 = AfxGetApp()->LoadIcon(IDI_TRAY_ACTIVE);
	m_hIcon1B8 = AfxGetApp()->LoadIcon(IDI_TRAY_DEACTIVE);
	m_pDlg21C = new CNotifyDialog;
	m_pDlg21C->Create(IDD_NOTIFY);
	RtlInitializeCriticalSection(&m_rcs240);
	RtlInitializeCriticalSection(&m_rcs224);
	RtlInitializeCriticalSection(&m_rcs258);
	RtlInitializeCriticalSection(&m_rcs0F4);
	RtlInitializeCriticalSection(&m_rcs274);
	m_p29C = (PSTRUCTK)RtlAllocateHeap(g_hProcessHeap, HEAP_ZERO_MEMORY, 0x34);
	RtlInitializeCriticalSection(&m_p29C->rcs00);
	m_p29C->dw18 = 0;
	m_p29C->pbBuffer1C = NULL;
	m_p29C->dwLength20 = 0;
	m_p29C->pdwNewBuffer24 = NULL;
	m_p29C->dwCount28 = 0;
	m_p29C->dwDriveCount2C = 0;
	m_p29C->pdwDriveName30 = 0;		
}

CAntiKeyloggerDlg::~CAntiKeyloggerDlg()
{
	// Clean up garbage
	for (int i=0; i<m_pInfo.GetSize(); i++)
	{
		PAGE_INFO *pInfo = (PAGE_INFO *)m_pInfo.GetAt(i);
		if (pInfo) 
		{
			delete pInfo;
		}
	}
	m_pInfo.RemoveAll();
	
	if(m_pDlg21C) {
		m_pDlg21C->DestroyWindow();
		delete m_pDlg21C;
	}
	
	RtlDeleteCriticalSection(&m_rcs240);
	RtlDeleteCriticalSection(&m_rcs224);
	RtlDeleteCriticalSection(&m_rcs258);
	RtlDeleteCriticalSection(&m_rcs0F4);
	RtlDeleteCriticalSection(&m_rcs274);

	RtlDeleteCriticalSection(&m_p29C->rcs00);
	RtlFreeHeap(g_hProcessHeap, 0, m_p29C);	
	
	for(i = 0; i < m_list294.GetSize(); i ++) {
		CFileInfo* pInfo = m_list294.GetAt(i);
		delete pInfo;
	}

	for(i = 0; i < m_list298.GetSize(); i ++) {
		CFileInfo* pInfo = m_list298.GetAt(i);
		delete pInfo;
	}

	m_list294.RemoveAll();
	m_list298.RemoveAll();
}

void CAntiKeyloggerDlg::DoDataExchange(CDataExchange* pDX)
{
	CTrayDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAntiKeyloggerDlg)
	DDX_Control(pDX, IDC_CAPTION_BAR, m_CaptionBarCtrl);
	DDX_Control(pDX, IDC_TREE_CTRL, m_TreeCtrl);
	DDX_Control(pDX, IDC_PAGE_FRAME, m_PageFrame);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAntiKeyloggerDlg, CTrayDialog)
	//{{AFX_MSG_MAP(CAntiKeyloggerDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_ACTIVATE()
	ON_NOTIFY(TVN_GETDISPINFO, IDC_TREE_CTRL, OnGetdispinfoTreeCtrl)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_CTRL, OnSelchangedTreeCtrl)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAntiKeyloggerDlg message handlers

BOOL CAntiKeyloggerDlg::OnInitDialog()
{
	CTrayDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_h270 = CreateThread(NULL, NULL, sub_432F40, this, 0, NULL);
	m_h2AC = CreateThread(NULL, NULL, sub_4315E0, this, 0, NULL);
	if(dword_44438C != 1) { // 등록되지 않았으면
		m_b0F0 = TRUE;
	}
	else { // 등록되였으면
		BYTE *lpResLock;
		HRSRC hResLoad, hRes = FindResource(NULL, (LPCSTR)IDR_DATA28, "BIN");
		hResLoad = (HRSRC)LoadResource(NULL, hRes);
		lpResLock = (BYTE*)LockResource(hResLoad);
		if(sub_410E10(lpResLock, 0x80) != 0 && TEST(dword_445E20, 0x20000) == 0) {
			sub_410BF0();
			GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
			m_b0F0 = FALSE;
		}
		else m_b0F0 = TRUE;
	}

	m_PageFrame.GetWindowRect(m_FrameRect);
	ScreenToClient(m_FrameRect);
	m_FrameRect.DeflateRect(2,2);

	SetWindowText(m_csTitle);

	// Set some styles for indicator bar
	m_CaptionBarCtrl.m_textClr     = ::GetSysColor(COLOR_3DFACE);
	m_CaptionBarCtrl.m_fontWeight  = FW_BOLD;
	m_CaptionBarCtrl.m_fontSize    = 14;
	m_CaptionBarCtrl.m_csFontName  = "Arial";
	m_CaptionBarCtrl.SetConstantText(m_csLogoText);

	InitTreeCtrl();		// Initialize the tree control with options
	ExpandTree();
	if (m_pInfo.GetAt(0)) ShowPage(m_pInfo.GetAt(0));

	CenterWindow();

	dword_445E20 = sub_410020();
	if(TEST(dword_4457F0, 4) || TEST(dword_445E20, 0x10000)) {
		sub_435CB0();
		if(TEST(dword_4457F0, 4)) dword_4457F0 &= 0xFFFFFFFB;
	}
	m_b1FC = dword_445E20 & 1;
	if(m_b1FC) sub_436260();
	else sub_436500();
	bool_445D10 = TRUE;
	SetEvent(hEvent_445E28);
	if(TEST(m_dw0D0[6], 0x40) && bool_445D14 == FALSE)
		GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
	m_b23E = TRUE;
	if(TEST(m_dw0D0[4], 2) || bool_445868) {
//		ShowWindow(SW_SHOW);
//		SetForegroundWindow();
//		UpdateWindow();
	}
	bool_445818 = TRUE;

//	CMenu* pSysMenu = GetSystemMenu(FALSE);
//	while(pSysMenu->RemoveMenu(0, MF_BYPOSITION))
//	{
//	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAntiKeyloggerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CTrayDialog::OnPaint();
	}
}

void CAntiKeyloggerDlg::InitTreeCtrl()
{
	// Fill the tree. we'll create the pages as we need them
	for (int i=0; i < m_pInfo.GetSize(); i++)
	{
		PAGE_INFO *pInfo = (PAGE_INFO *)m_pInfo.GetAt(i);
		pInfo->nOrder = i;
		if (!pInfo) continue;
		TV_INSERTSTRUCT tvi;

		if (pInfo->pWndParent) tvi.hParent = FindItem(pInfo->pWndParent);
		else tvi.hParent = FindItem(pInfo->csParentCaption);
		tvi.hInsertAfter = TVI_LAST;
		tvi.item.cchTextMax = 0;
		tvi.item.pszText = LPSTR_TEXTCALLBACK;
		tvi.item.lParam = (long)pInfo;
		tvi.item.mask = TVIF_PARAM | TVIF_TEXT;
		HTREEITEM hTree = m_TreeCtrl.InsertItem(&tvi);

		// Keep track of the pages we've added (for parent selection)
		if (hTree && pInfo->pWnd)
		{
			DWORD dwTree = (DWORD)hTree;
			m_wndMap.SetAt(pInfo->pWnd, dwTree);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAntiKeyloggerDlg::AddPage(...)
//
// Description: Add a page specified by CRuntimeClass. 
//		If pWndClass = NULL, the corresponding tree item has no page
//		The page identified by pCaption on the tree control will be inserted
//		under the parent tree item identified by pWndParent.
//		If pWndParent is NULL, them the page becomes a root item on the
//		tree control
// Return value: A CWnd type pointer to the page created if successful,
//		otherwise return NULL
//
CWnd* CAntiKeyloggerDlg::AddPage(CRuntimeClass  *pWndClass, 
							   const char *pCaption, UINT nID /* = 0 */, 
							   CWnd *pWndParent /* = NULL */)
{
	CWnd* pWnd = NULL;
	if (m_hWnd)
	{
		// Can't add once the window has been created
		ASSERT(0);
		return pWnd;
	}
	
	// Create the specified page object
	if (pWndClass) pWnd = (CWnd*) pWndClass->CreateObject();	

	PAGE_INFO *pInfo = new PAGE_INFO;
	pInfo->bViewClass = TRUE;			// Default to generic CWnd or CView class
	if (pWnd)							// If it is a dialog or dialog-derived class
	{									// Set bViewClass = FALSE
		if (pWnd->IsKindOf(RUNTIME_CLASS(CDialog)))	pInfo->bViewClass = FALSE;
	}

	pInfo->nID = nID;					// ID of the page
	pInfo->pWnd = pWnd;					// Pointer to the page
	pInfo->csCaption = pCaption;		// Caption of page in the tree control
	pInfo->pWndParent = pWndParent;		// Parent of the page if it has one
	pInfo->csParentCaption.Empty();		// Parent caption ignored
	m_pInfo.Add(pInfo);					// Add to the page list

	return pWnd;
}

/////////////////////////////////////////////////////////////////////////////
// CAntiKeyloggerDlg::AddPage(...)
//
// Description: Add a page specified by CRuntimeClass. 
//		If pWndClass = NULL, the corresponding tree item has no page
//		The page identified by pCaption on the tree control will be inserted
//		under the parent tree item identified by pParentCaption.
//		If pParentCaption is empty, them the page becomes a root item on the
//		tree control
// Return value: A CWnd type pointer to the page created if successful,
//		otherwise return NULL
//
CWnd* CAntiKeyloggerDlg::AddPage(CRuntimeClass *pWndClass, const char *pCaption, 
							   UINT nID, const char *pParentCaption)
{
	CWnd* pWnd = NULL;
	if (m_hWnd)
	{
		// Can't add once the window has been created
		ASSERT(0);
		return pWnd;
	}

	// Create the specified page object
	if (pWndClass) pWnd = (CWnd*) pWndClass->CreateObject();
	PAGE_INFO *pInfo = new PAGE_INFO;
	pInfo->bViewClass = TRUE;			// Default to generic CWnd or CView class
	if (pWnd)							// If it is a dialog or dialog-derived class
	{									// Set bViewClass = FALSE
		if (pWnd->IsKindOf(RUNTIME_CLASS(CDialog)))	pInfo->bViewClass = FALSE;
	}
	pInfo->nID = nID;					// ID of the page
	pInfo->pWnd = pWnd;					// Point to the page
	pInfo->csCaption = pCaption;		// Caption of page in the tree control
	pInfo->pWndParent = NULL;			// Parent page is not specified yet
	pInfo->csParentCaption = pParentCaption;	// Parent caption to be inserted under
	m_pInfo.Add(pInfo);					// Add to to page list

	return pWnd;
}

/////////////////////////////////////////////////////////////////////////////
// CAntiKeyloggerDlg::FindItem(CWnd *pWnd)
//
// Description: Find the tree item indexed by the page pointer pWnd. 
//	 
// Return value: An HTREEITEM is returned. If pWnd = NULL, TVI_ROOT returned
//	
HTREEITEM CAntiKeyloggerDlg::FindItem(CWnd *pWnd)
{
	// If you didn't specify a parent in AddPage(...) , the
	// dialog becomes a root-level entry
	if (pWnd == NULL) return TVI_ROOT;
	else
	{
		DWORD dwHTree;
		if (m_wndMap.Lookup(pWnd, dwHTree)) return (HTREEITEM)dwHTree;
		else
		{
         // Specified a parent that has not 
         // been added to the tree - can't do that.
			ASSERT(FALSE);
			return (TVI_ROOT);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAntiKeyloggerDlg::FindItem(const CString &csCaption)
//
// Description: Find the tree item indexed by tree item text csCaption. 
//	 
// Return value: An HTREEITEM is returned. 
//				TVI_ROOT is returned 
//				if (1) csCaption = Empty, TVI_ROOT returned
//				If (2) the is no match of csCaption on the tree control,
//	
HTREEITEM CAntiKeyloggerDlg::FindItem(const CString &csCaption)
{
	if(m_TreeCtrl.GetCount() == 0) return TVI_ROOT; // If the tree is empty
	if(csCaption.IsEmpty()) return TVI_ROOT;		// If no item text specified

	HTREEITEM hCurrent  = m_TreeCtrl.GetRootItem();
	while(hCurrent)
	{
		CString strText = m_TreeCtrl.GetItemText(hCurrent);
		if(!strText.CompareNoCase(csCaption))	return hCurrent;
		hCurrent = GetNextItemCOrS(hCurrent);
	}
	return TVI_ROOT;

}

/////////////////////////////////////////////////////////////////////////////
// CAntiKeyloggerDlg::GetNextItemCOrS(HTREEITEM hItem)
//
// Description: Get handle of the child or sibling tree item 
//				indexed by current tree item handle. 
//	 
// Return value: An HTREEITEM is returned. 
//	
HTREEITEM CAntiKeyloggerDlg::GetNextItemCOrS(HTREEITEM hItem)
{
	HTREEITEM	hti;
	if( m_TreeCtrl.ItemHasChildren( hItem ) )
		return m_TreeCtrl.GetChildItem( hItem );		// return first child
	else
	{
		// Return next sibling item
		// Go up the tree to find a parent's sibling if needed.
		while( (hti = m_TreeCtrl.GetNextSiblingItem( hItem )) == NULL )
		{
			if( (hItem = m_TreeCtrl.GetParentItem( hItem ) ) == NULL )
				return NULL;
		}
	}
	return hti;
}

/////////////////////////////////////////////////////////////////////////////
// CAntiKeyloggerDlg::ShowPage(const PAGE_INFO *pInfo, UINT nShow /* = SW_SHOW */)
// Description: Display or hide the page if the associated tree item is selected.
//				nShow determines whether to display or hide the page.
//				pInfo identifies the page to be displayed.
//				Note, if the window of the page has not yet ben created
//				it will create the page window before the page gets displayed
// Return value: NULL
//
void CAntiKeyloggerDlg::ShowPage(const PAGE_INFO *pInfo, UINT nShow /* = SW_SHOW */)
{
	if (!pInfo) return;
	m_CaptionBarCtrl.SetWindowText("");		// Clear the caption bar

	if (pInfo->pWnd)						// If the page is valid
	{
		if (!::IsWindow(pInfo->pWnd->m_hWnd))
		{									// Window has not been created, create it
			CreatePage(pInfo);
			pInfo->pWnd->SetWindowPos (&m_TreeCtrl,0,0,0,0,0);
			pInfo->pWnd->MoveWindow(m_FrameRect.left, m_FrameRect.top, m_FrameRect.Width(), m_FrameRect.Height());
			if( pInfo->pWnd->IsKindOf( RUNTIME_CLASS(CView) ) )
				((CView*)pInfo->pWnd)->OnInitialUpdate();
		}
		
		pInfo->pWnd->ShowWindow(nShow);		// Show or hide the window
		if (nShow == SW_SHOW)				// Change the tree selection
		{
			pInfo->pWnd->SetFocus();
			HTREEITEM hItem = FindItem(pInfo->pWnd);
			if (hItem) m_TreeCtrl.SelectItem(hItem);
			m_n0C4 = pInfo->nOrder;
		}
	}
	if (nShow == SW_SHOW) 					// Update caption bar
		m_CaptionBarCtrl.SetWindowText(pInfo->csCaption);
}

/////////////////////////////////////////////////////////////////////////////
// CAntiKeyloggerDlg::CreatePage(const PAGE_INFO *pInfo)
// Description: Create the Window of the page specified by the page info.
// Return value: TRUE if successful, or FALSE if failed
// 
BOOL CAntiKeyloggerDlg::CreatePage(const PAGE_INFO *pInfo)
{
	BOOL bCode = FALSE;

	if (!pInfo || !pInfo->pWnd) return(FALSE);	// If no page is specified, return NULL
	if (!::IsWindow(pInfo->pWnd->m_hWnd))		// If the window has not yet been created,
	{
		if (pInfo->pWnd->IsKindOf(RUNTIME_CLASS(CDialog)))	// If the page window is kind of dialog window
		{
			CDialog *pDlg = (CDialog*)pInfo->pWnd;
			bCode = pDlg->Create(pInfo->nID, this);
			pDlg->ModifyStyle(WS_CAPTION, 0);
		}
		else if (pInfo->pWnd->IsKindOf(RUNTIME_CLASS(CWnd)))	// generic CWnd derived Window
		{
			CWnd *pWnd = (CWnd*)pInfo->pWnd;
			bCode = CreateWnd(pInfo->pWnd);						// Create Window 
			pWnd->ModifyStyle(WS_BORDER|WS_THICKFRAME, 0);		// Remove border and thick frame styles
		}
	}
	return(bCode);
}

/////////////////////////////////////////////////////////////////////////////
// CAntiKeyloggerDlg::CreateWnd(CWnd *pWnd, CCreateContext *pContext /* = NULL */)
// Description:	Create generic CWnd based Window of a page
// Return value: TRUE if successful, NULL if failed
//
BOOL CAntiKeyloggerDlg::CreateWnd(CWnd *pWnd, CCreateContext *pContext /* = NULL */)
{
	CCreateContext context;	
	if (pContext == NULL)	
	{
		// If no context specified, generate one from the currently selected
		//  client if possible
		context.m_pCurrentFrame = (CFrameWnd*) this;
		context.m_pCurrentDoc = NULL;
		context.m_pNewViewClass = NULL;
		pContext = &context;

	}
	
    ASSERT_KINDOF(CWnd, pWnd);
	ASSERT(pWnd->m_hWnd == NULL);       // Not yet created
	
	DWORD dwStyle = AFX_WS_DEFAULT_VIEW;
	CRect rect;
	// Create with the right size and position
	if (!pWnd->Create(NULL, NULL, dwStyle, rect, this, 0, pContext))
	{
		TRACE0("Warning: couldn't create wnd in CAntiKeyloggerDlg.\n");
		// pWnd will be cleaned up by PostNcDestroy
		return FALSE;
	}
	return(TRUE);
}

void CAntiKeyloggerDlg::OnGetdispinfoTreeCtrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (pTVDispInfo->item.lParam)
	{
		if (pTVDispInfo->item.mask & TVIF_TEXT)
		{
			PAGE_INFO *pInfo = (PAGE_INFO *)pTVDispInfo->item.lParam;
			strcpy(pTVDispInfo->item.pszText, pInfo->csCaption);
		}
	}
	
	*pResult = 0;
}

void CAntiKeyloggerDlg::OnSelchangedTreeCtrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (pNMTreeView->itemNew.lParam)
	{									// Get previous selected page
		PAGE_INFO *pInfo = (PAGE_INFO *) pNMTreeView->itemOld.lParam;
		ShowPage(pInfo, SW_HIDE);		// Hide the page
										// Get current selected page
		pInfo = (PAGE_INFO *) pNMTreeView->itemNew.lParam;
		ShowPage(pInfo, SW_SHOW);		// Show the page
	}
	*pResult = 0;
}

void CAntiKeyloggerDlg::OnStop() 
{
	if(m_b1FC) sub_410BF0();
	else sub_410B90();
}

/////////////////////////////////////////////////////////////////////////////
// CSettingsDialog::RefreshData()
// Description: Clean up function if OK and Apply buttons are clicked
// Return value: TRUE if successful, or FALSE if failed
//
BOOL CAntiKeyloggerDlg::RefreshData()
{
	PAGE_INFO *pInfoErr = NULL, *pInfo;
	// First, UpdateData... 
	for (int i = 0; i < m_pInfo.GetSize(); i++)
	{
		pInfo = (PAGE_INFO *)m_pInfo.GetAt(i);
		if (pInfo && pInfo->pWnd)
		{
			if (::IsWindow(pInfo->pWnd->m_hWnd))
				pInfo->pWnd->UpdateData(TRUE);
		}
	}
	return (TRUE);		
}

/////////////////////////////////////////////////////////////////////////////
// CSettingsDialog::DestroyPages()
// Destoy all the page Windows if the dialog is dismissed
// Return value: TRUE if successful, or FALSE if failed
//
BOOL CAntiKeyloggerDlg::DestroyPages()
{
	for (int i=0; i<m_pInfo.GetSize(); i++)
	{
		PAGE_INFO *pInfo = (PAGE_INFO *)m_pInfo.GetAt(i);
		if (pInfo && pInfo->pWnd)
		{
			if (::IsWindow(pInfo->pWnd->m_hWnd))
			{
				pInfo->pWnd->DestroyWindow();		// Destroy the windows
				if (!(pInfo->bViewClass))  
					delete(pInfo->pWnd);	// Delete the C++ object
			}
		}
	}

	return(true);
}

/////////////////////////////////////////////////////////////////////////////
// CSettingsDialog::SetTitle(CString sTitle)
// Description: Change the title of the dialog
void CAntiKeyloggerDlg::SetTitle(CString sTitle)
{
	m_csTitle = sTitle;
}

/////////////////////////////////////////////////////////////////////////////
// CSettingsDialog::SetLogoText(CString sText)
// Description: Set the logo text in caption window
//
void CAntiKeyloggerDlg::SetLogoText(CString sText)
{
	m_csLogoText= sText;
}

/////////////////////////////////////////////////////////////////////////////
//
void CAntiKeyloggerDlg::ExpandTree()
{
	HTREEITEM hti = m_TreeCtrl.GetRootItem();
	while (hti)
	{
		ExpandBranch(hti);
		hti = m_TreeCtrl.GetNextSiblingItem(hti);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
void CAntiKeyloggerDlg::ExpandBranch(HTREEITEM hti)
{
	if( m_TreeCtrl.ItemHasChildren( hti ) )
	{
		m_TreeCtrl.Expand( hti, TVE_EXPAND );
		hti = m_TreeCtrl.GetChildItem( hti );
		do
		{
			ExpandBranch( hti );
		} while( (hti = m_TreeCtrl.GetNextSiblingItem( hti )) != NULL );
	}
	m_TreeCtrl.EnsureVisible( m_TreeCtrl.GetSelectedItem() );
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAntiKeyloggerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAntiKeyloggerDlg::OnDestroy() 
{
	DWORD dwExitCode;
	DestroyPages();	
	CTrayDialog::OnDestroy();
	if(m_h270) {
		if(GetExitCodeThread(m_h270, &dwExitCode))
			TerminateThread(m_h270, dwExitCode);
		CloseHandle(m_h270);
		m_h270 = NULL;
	}	
	PostQuitMessage(0);
}

DWORD CAntiKeyloggerDlg::sub_436960(BOOL bFlag)
{
	DWORD dwRet = ERROR_SUCCESS;
	DWORD var_8[2] = {0};
	HKEY hKey;

	if(bFlag == FALSE)
	{
		sub_410F00((BYTE*)var_8, 8);
		m_dw0D0[6] = var_8[0];
		m_dw0D0[7] = var_8[1];
		if(TEST(m_dw0D0[6], 0x100000) == 0)
			hKey = hkey_445CF4;
		else 
			hKey = hkey_445CF8;
		DWORD cbData = 0x20;
		if(RegQueryValueExW(hKey, L"Setting", NULL, NULL, (BYTE*)m_dw0D0, &cbData) == ERROR_SUCCESS && cbData == 0x20)
		{
			if(m_b0F0) sub_410F00((BYTE*)&m_dw0D0[6], 8);
			return dwRet;
		}

		m_dw0D0[6] = var_8[0];
		m_dw0D0[7] = var_8[1];
		return sub_436960(TRUE);
	}
	else
	{
		RtlZeroMemory(&m_dw0D0, 0x20);
//		m_dw0D0[4] = m_dw0D0[4] | 2;
		dwRet = sub_436890(FALSE);
		if(m_b0F0) sub_410F00((BYTE*)&m_dw0D0[6], 8);
		return dwRet;		
	}
}

DWORD CAntiKeyloggerDlg::sub_436890(BOOL bFlag)
{
	DWORD dwRet;
	if(m_hIcon1B0 && m_hIcon1B8) {
		/*if(TEST(m_dw0D0[4], 2))*/ sub_4367D0();
//		else {
//			if(TrayUpdate()) TrayHide();
//		}
	}

	if(bool_445D14 && m_b0F0 && bFlag) sub_410F80((BYTE*)&m_dw0D0[6], 8);
	if((dwRet = RegSetValueExW(hkey_445CF4, L"Setting", 0, REG_BINARY, (BYTE*)m_dw0D0, 0x20)) == ERROR_SUCCESS && TEST(m_dw0D0[6], 0x100000))
		dwRet = RegSetValueExW(hkey_445CF8, L"Setting", 0, REG_BINARY, (BYTE*)m_dw0D0, 0x20);
	return dwRet;
}

void CAntiKeyloggerDlg::sub_4367D0()
{
	if(m_b2A4) return;
//	if(TEST(m_dw0D0[4], 2) == 0) return;

	if(m_b1FC == FALSE)
	{
		TraySetIcon(m_hIcon1B8);
		TraySetToolTip(L"Now your computer is not protected");
	}
	else
	{
		TraySetIcon(m_hIcon1B0);
		TraySetToolTip(L"Now your computer is protected");
	}
	if(TrayUpdate() == FALSE)
		TrayShow();

	return;
}

void CAntiKeyloggerDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	if(nState != WA_INACTIVE) {
		if(bool_445868) sub_436F60();
		SetForegroundWindow();
		UpdateWindow();
	}
	
	CTrayDialog::OnActivate(nState, pWndOther, bMinimized);	
}

void CAntiKeyloggerDlg::OnTrayLButtonDblClk(CPoint pt)
{
	RECT rcWnd, rcTray;
	HWND hTrayWnd, hTrayNotifyWnd;

	GetWindowRect(&rcWnd);
	hTrayWnd = FindWindowW(L"Shell_TrayWnd", NULL);
	hTrayNotifyWnd = FindWindowExW(hTrayWnd, NULL, L"TrayNotifyWnd", NULL);
	::GetWindowRect(hTrayNotifyWnd, &rcTray);
	
	if(IsWindowVisible())
	{
		DrawAnimatedRects(this->m_hWnd, IDANI_CAPTION, &rcWnd, &rcTray);
		ShowWindow(SW_HIDE);
		m_b23D = FALSE;
		SendMessageW(hWnd_4457FC, WM_COMMAND, 0, (LPARAM)btnCancel);
		m_b23C = FALSE;
		bool_445868 = FALSE;
	}
	else
	{
		DrawAnimatedRects(this->m_hWnd, IDANI_CAPTION, &rcTray, &rcWnd);
		ShowWindow(SW_SHOW);
		SetForegroundWindow();
		UpdateWindow();
		sub_436F60();
		m_b23C = TRUE;
		bool_445868 = TRUE;
	}
	return;
}

LRESULT CAntiKeyloggerDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(bool_445868) sub_436F60();

	if(message == message_44583C)// 쓰레드함수 sub_4315E0에서 사건오브젝트 hEvent_445D00의 설정시 창문을 앞에 현시한다.
	{
		sub_436F60();
		if(wParam != 0)
		{
			SetForegroundWindow();
			UpdateWindow();
		}
        return 0;
	}

	if(message == message_445838)// 쓰레드함수 sub_4315E0에서 사건오브젝트 hEvent_445D00의 설정시 창문을 현시한다.
	{
		sub_434FF0(this, (BOOL)wParam);
		return 0;
	}

	if(message == message_44582C)// 쓰레드함수 sub_435F60와 sub_4377D0에서 발생함
	{							// Protection Policy페지에 새로운 모쥴을 추가한다.
		if(m_n0C4 == 1)
		{
//			sub_42A5D0(eax);
			HTREEITEM hItem = m_TreeCtrl.GetSelectedItem();
			PAGE_INFO *pInfo = (PAGE_INFO *) m_TreeCtrl.GetItemData(hItem);
			pInfo->pWnd->SendMessage(WM_SHOWWINDOW, 1, 0);
		}
		
		return 0;
	}

	if(message == message_445830)// 발생되지 않음
	{
		if(m_n0C4 == 2)
		{
//			sub_42B090(eax);
			HTREEITEM hItem = m_TreeCtrl.GetSelectedItem();
			PAGE_INFO *pInfo = (PAGE_INFO *) m_TreeCtrl.GetItemData(hItem);
			pInfo->pWnd->SendMessage(WM_SHOWWINDOW, 1, 0);
		}

		return 0;
	}

	if(message == message_445834)// 발생되지 않음
	{
//		sub_43B6C0(wParam, arg_0[0x1D4]);
		return 0;
	}

	if(message == message_445840)// 발생되지 않음? "TaskbarCreated"메쎄지
	{
		sub_4367D0();
		return 0;
	}
	
	if(bool_445818 == FALSE)
		return CTrayDialog::WindowProc(message, wParam, lParam);

	if(message == message_445844)// 0x80000001 : 프로그람을 동작시킨다.
	{
		if(TEST(dword_4457F0, 1))
		{
			sub_436260();
            dword_4457F0 &= 0xFFFFFFFE;
		}

		return 0;
	}

	if(message == message_445848)// 0x80000002 : 프로그람을 정지시킨다.
	{
		if(TEST(dword_4457F0, 2))
		{
			sub_436500();
			dword_4457F0 &= 0xFFFFFFFD;
		}

		return 0;
	}

	if(message == message_44584C)// 0x80000003 : 기한이 지났을때 등록창을 현시한다.
	{
		if(TEST(dword_4457F0, 4))
		{
			sub_435CB0();
			dword_4457F0 &= 0xFFFFFFFB;
		}

		return 0;
	}

	if(message == message_445850)// 0x80000004 : Option설정
	{
		if(TEST(dword_4457F0, 8))
		{
			sub_435D90();
			dword_4457F0 &= 0xFFFFFFF7;
		}

		return 0;
	}

	if(message == message_445854)// 0x80000005 : 프로세스완료
	{
		if(TEST(dword_4457F0, 0x10))
		{
			DestroyWindow();
			dword_4457F0 &= 0xFFFFFFEF;
		}

		return 0;
	}

	if(message == message_445858)// 0x80000006 : 새로운 모쥴이 추가되였다.
	{
		if(TEST(dword_4457F0, 0x20))
		{
			sub_436200();
			dword_4457F0 &= 0xFFFFFFDF;
		}

		return 0;
	}

	if(message == message_445864)// 0x80000009 : The number of licensed program copies running in the network is exceeded. Anti-keylogger has been turned off and your system is not protected now.
	{
//		sub_435C70();
		return 0;
	}

	if(message == message_44585C)// 0x80000007
	{
		if(m_b2A4) goto end;

        if(TEST(dword_4457F0, 0x40))
		{
			dword_4457F0 &= 0xFFFFFFBF;

			RtlEnterCriticalSection(&m_rcs0F4);
			DWORD var_8[2] = {0};
			sub_410F00((BYTE*)var_8, 8);
			if(memcmp(&m_dw0D0[6], var_8, 8) != 0)
			{
				memcpy(&m_dw0D0[6], var_8, 8);
				if(bool_445D14 == FALSE)
					GetDlgItem(IDC_STOP)->EnableWindow(!((m_dw0D0[6] >> 6) & 1));
//				sub_437830(arg_0[0x294]); // exclude list
//				if(m_n0C4 == 1)
//					sub_42A5D0(arg_0[0x204]); // protection policy page
				if(m_n0C4 == 4)
				{
//					eax = arg_0[0x20C]; // options page
//					sub_40DBD0(eax[0x84], eax[0x88]);
				}
			}
			RtlLeaveCriticalSection(&m_rcs0F4);
		}

		return 0;
	}

end:	
	return CTrayDialog::WindowProc(message, wParam, lParam);
}

void CAntiKeyloggerDlg::sub_436F60()
{
	DWORD dwEx, var_8[2];
	HANDLE hThread;

	if(m_b23D) return;
	sub_410F00((BYTE*)var_8, 8);

	if(byte_444010)
	{
		byte_444010 = 0;
        if(TEST(m_dw0D0[4], 2)) return;
	}

	if(TEST(var_8[0], 0x200000) == 0) return;

	m_b23C = TRUE;
	dwEx = InterlockedCompareExchange((long*)&dword_44586C, 1, 0);

	if(dwEx) return;

	hThread = CreateThread(NULL, 0, sub_403080, this, 0, NULL);
    CloseHandle(hThread);

	return;
}

void CAntiKeyloggerDlg::OnSysCommand(UINT nID, LPARAM lParam) 
{
	if ((nID & 0xFFF0) == SC_CLOSE)
	{
		RECT rcWnd, rcTray;
		HWND hTrayWnd, hTrayNotifyWnd;

		GetWindowRect(&rcWnd);
		hTrayWnd = FindWindowW(L"Shell_TrayWnd", NULL);
		hTrayNotifyWnd = FindWindowExW(hTrayWnd, NULL, L"TrayNotifyWnd", NULL);
		::GetWindowRect(hTrayNotifyWnd, &rcTray);
		DrawAnimatedRects(this->m_hWnd, IDANI_CAPTION, &rcWnd, &rcTray);
		ShowWindow(SW_HIDE);
		m_b23D = FALSE;
		SendMessageW(hWnd_4457FC, WM_COMMAND, 0, (LPARAM)btnCancel);
		m_b23C = FALSE;
		bool_445868 = FALSE;
	}
	else
		CTrayDialog::OnSysCommand(nID, lParam);		
}


DWORD WINAPI sub_403080( LPVOID lpParam )
{
	CAntiKeyloggerDlg* pDlg = (CAntiKeyloggerDlg*)lpParam;
	DWORD var_C = 0, var_8[2];

	RtlEnterCriticalSection(&pDlg->m_rcs224);

	if(pDlg->m_b23D == FALSE) {
		EnableWindow(pDlg->m_hWnd, FALSE);
		sub_401040(L"Enter your password:", &var_C, pDlg->m_hWnd);
		EnableWindow(pDlg->m_hWnd, TRUE);
		sub_410F00((BYTE*)var_8, 8);
		if(var_C == var_8[1]) {
			SetForegroundWindow(pDlg->m_hWnd);
			pDlg->m_b23D = TRUE;
		}
		else
			sub_434FF0(pDlg, TRUE);
	}
    RtlLeaveCriticalSection(&pDlg->m_rcs224);
	InterlockedDecrement((long*)&dword_44586C);

	return 0;
}

void sub_434FF0(CAntiKeyloggerDlg* pDlg, BOOL bMinimize)
{
	RECT rcWnd, rcTray;
	HWND hTrayWnd, hTrayNotifyWnd;
	
	GetWindowRect(pDlg->m_hWnd, &rcWnd);
	hTrayWnd = FindWindowW(L"Shell_TrayWnd", NULL);
	hTrayNotifyWnd = FindWindowExW(hTrayWnd, NULL, L"TrayNotifyWnd", NULL);
	GetWindowRect(hTrayNotifyWnd, &rcTray);

	if(bMinimize)
	{
		DrawAnimatedRects(pDlg->m_hWnd, IDANI_CAPTION, &rcWnd, &rcTray);
		ShowWindow(pDlg->m_hWnd, SW_HIDE);
		pDlg->m_b23D = FALSE;
		SendMessageW(hWnd_4457FC, WM_COMMAND, 0, (LPARAM)btnCancel);
	}
	else
	{
		DrawAnimatedRects(pDlg->m_hWnd, IDANI_CAPTION, &rcTray, &rcWnd);
		ShowWindow(pDlg->m_hWnd, SW_SHOW);
		SetForegroundWindow(pDlg->m_hWnd);
		UpdateWindow(pDlg->m_hWnd);
		pDlg->sub_436F60();
	}

	pDlg->m_b23C = (bMinimize == FALSE);
	bool_445868 = (bMinimize == FALSE);
}

DWORD WINAPI sub_432F40(LPVOID lpParameter)
{
	CAntiKeyloggerDlg* pDlg = (CAntiKeyloggerDlg*)lpParameter;
	DWORD i, *pdwBuffer;
	PSTRUCTK psk = pDlg->m_p29C;
	CFileInfo *pInfo;

	if(!sub_43EE70(psk)) return 0;
	if(psk->dwCount28 == 0) return 0;
	for(i = 0; i < psk->dwCount28; i ++) {
		pdwBuffer = psk->pdwNewBuffer24;
		pdwBuffer += i * 4;
		pInfo = new CFileInfo(pdwBuffer);
		if(pInfo->m_pbState[2]) pDlg->m_list298.Add(pInfo);
		else pDlg->m_list294.Add(pInfo);
	}
	if(pDlg->m_b2A4 == FALSE) PostMessageW(pDlg->m_hWnd, message_44582C, 0, 0);
	return 0;
}

DWORD WINAPI sub_4315E0(LPVOID lpParameter)
{
	CAntiKeyloggerDlg* pDlg = (CAntiKeyloggerDlg*)lpParameter;

	while(1) {
		WaitForSingleObject(hEvent_445D00, INFINITE);
		ResetEvent(hEvent_445D00);
		if(pDlg->m_b2A4) continue;
		if(IsWindowVisible(pDlg->m_hWnd))PostMessageW(pDlg->m_hWnd, message_44583C, 1, 0);
		else PostMessageW(pDlg->m_hWnd, message_445838, 0, 0);
	}
	
	return 0;
}

void CAntiKeyloggerDlg::sub_435CB0()
{
	HANDLE hEvent;
	DWORD dwState;

	if(m_b2A4 || dword_44438C) return;

	m_b0F0 = FALSE;
	if(hEvent = OpenEventW(EVENT_ALL_ACCESS, FALSE, L"SHKeyPro_Exist")) {
		CloseHandle(hEvent);
		return;
	}
	hEvent = CreateEventW(NULL, TRUE, TRUE, L"SHKeyPro_Exist");
	sub_410BF0();
	sub_436500(); //기한이 지났을때 정지상태의 대면부를 현시한다.
	GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
	dwState = sub_410E80();
//	sub_441BF0(TEST(dwState, 0xFF000000) == 0); 등록창을 현시한다.
	CloseHandle(hEvent);
}

void CAntiKeyloggerDlg::sub_436260()
{
	m_b1FC = TRUE;
	sub_4367D0();
	GetDlgItem(IDC_STOP)->SetWindowText("Stop");
}

void CAntiKeyloggerDlg::sub_436500()
{
	m_b1FC = FALSE;
	sub_4367D0();
	GetDlgItem(IDC_STOP)->SetWindowText("Start");
}

void CAntiKeyloggerDlg::sub_435D90()
{
	if(m_b2A4) return;
	if(TEST(m_dw0D0[6], 0x40) == 0 || bool_445D14 == TRUE) 
		GetDlgItem(IDC_STOP)->EnableWindow();
	m_b0F0 = TRUE;
	if(m_n0C4 == 5) {
//		sub_430B50(); 변경된 옵션에 대한 처리를 진행한다.
	}
}

void CAntiKeyloggerDlg::sub_436200()
{
	if(m_b2A4) return;
	
	PDWORD pdwData = (PDWORD)RtlAllocateHeap(g_hProcessHeap, HEAP_ZERO_MEMORY, 8);
	pdwData[0] = m_ThreadData28C.AddThread();
	pdwData[1] = (DWORD)this;
	HANDLE hThread = CreateThread(NULL, 0, sub_435F60, pdwData, 0, NULL);
	m_ThreadData28C.SetThread(pdwData[0], hThread);
}

DWORD WINAPI sub_435F60(LPVOID lpParameter)
{
	PDWORD pdwBuffer, pdwData = (PDWORD)lpParameter;
	CAntiKeyloggerDlg* pDlg = (CAntiKeyloggerDlg*)pdwData[1];
	BOOL bSuccess = FALSE;
	PSTRUCTK psk = pDlg->m_p29C;
	BYTE* pbState;
	PWSTR str;
	CFileInfo* pInfo;
	BOOL bExist;
	HANDLE hThread;

	RtlEnterCriticalSection(&pDlg->m_rcs274);
	if(sub_43EE70(psk)) {
		for(int i = 0, j = 0; i < psk->dwCount28; i ++) {
			pdwBuffer = psk->pdwNewBuffer24;
			pdwBuffer += i * 4;
			pbState = (BYTE*)pdwBuffer;
			if(pbState[2] == 0) {
				bExist = FALSE;
				str = (PWSTR)pdwBuffer[1];
				for(j = 0; j < pDlg->m_list294.GetSize(); j ++) {
					pInfo = pDlg->m_list294.GetAt(j);
					if(wcsicmp(pInfo->m_strPathName, str) == 0) {
						pInfo->m_pbState = pbState;
						bExist = TRUE;
						break;
					}
				}
				if(bExist == FALSE) {
					pInfo = new CFileInfo(pdwBuffer);
					pDlg->m_list294.Add(pInfo);
					bSuccess = TRUE;
					if(TEST(pDlg->m_dw0D0[4], 4) == 0) {
						str = (PWSTR)RtlAllocateHeap(g_hProcessHeap, HEAP_ZERO_MEMORY, wcslen(pInfo->m_strPathName) * 2 + 2);
						wcscpy(str, pInfo->m_strPathName);
						pdwData = (PDWORD)RtlAllocateHeap(g_hProcessHeap, HEAP_ZERO_MEMORY, 8);
						pdwData[0] = (DWORD)str;
						pdwData[1] = (DWORD)pDlg;
						hThread = CreateThread(NULL, 0, sub_435EC0, pdwData, 4, NULL);
						if(hThread) {
							SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);
							ResumeThread(hThread);
							CloseHandle(hThread);
						}
						pdwData = (PDWORD)lpParameter;
					}
				}
			}
		}
	}
	RtlLeaveCriticalSection(&pDlg->m_rcs274);
	if(bSuccess) PostMessageW(pDlg->m_hWnd, message_44582C, 0, 0);
	hThread = (HANDLE)pDlg->m_ThreadData28C.RemoveThread(pdwData[0]);
	CloseHandle(hThread);
	RtlFreeHeap(g_hProcessHeap, 0, pdwData);
	return 0;
}

DWORD WINAPI sub_435EC0(LPVOID lpParameter)
{
	PDWORD pdwData = (PDWORD)lpParameter;
	CAntiKeyloggerDlg* pDlg = (CAntiKeyloggerDlg*)pdwData[1];
	PWSTR str = (PWSTR)pdwData[0];

	RtlEnterCriticalSection(&pDlg->m_rcs274);
	RtlEnterCriticalSection(&pDlg->m_rcs258);
	pDlg->m_pDlg21C->sub_423EB0(str);
	WaitForSingleObject(pDlg->m_pDlg21C->m_hEvent088, INFINITE);
	RtlLeaveCriticalSection(&pDlg->m_rcs258);
	RtlLeaveCriticalSection(&pDlg->m_rcs274);
	RtlFreeHeap(g_hProcessHeap, 0, str);
	RtlFreeHeap(g_hProcessHeap, 0, pdwData);
	return 0;
}