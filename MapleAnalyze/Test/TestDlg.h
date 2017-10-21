// TestDlg.h : header file
//

#pragma once
#include "afxcmn.h"


// CTestDlg dialog
class CTestDlg : public CDialog
{
// Construction
public:
	CTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnInject();
	BOOL RefreshAutoInfo(BOOL bWrite);
	void SendCommand(int nCommand);

public:
	HANDLE		m_hRecvEvent;
	HANDLE		m_hSendEvent;
	HANDLE		m_hSendAnswerEvent;
	HANDLE		m_hRecvAnswerEvent;
	HANDLE		m_hMapFile;
	BYTE*		m_pbMapBuf;
	BOOL		m_bInjected;


	afx_msg void OnBnClickedBtnDump();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnWatchmem();
	afx_msg void OnBnClickedBtnHookvt();
	afx_msg void OnBnClickedButton7();
	CListCtrl m_ctlBreakList;
	CString m_strBR1;
	CString m_strBR2;
	afx_msg void OnBnClickedBtnBrAdd();
	afx_msg void OnBnClickedBtnBrDel();
	afx_msg void OnBnClickedBtnBrCha();
	afx_msg void OnNMClickListBreak(NMHDR *pNMHDR, LRESULT *pResult);
	int m_nSelBRList;
	afx_msg void OnBnClickedBtnBrInit();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButton8();
};
