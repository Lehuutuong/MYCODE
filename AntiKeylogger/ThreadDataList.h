// ThreadDataList.h: interface for the CThreadDataList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREADDATALIST_H__B48D5DA3_A01F_449F_AB5D_12D8987F6A75__INCLUDED_)
#define AFX_THREADDATALIST_H__B48D5DA3_A01F_449F_AB5D_12D8987F6A75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

typedef struct _THREAD_DATA
{
	int		nIndex;
	HANDLE	hThread; 
} THREAD_DATA, *PTHREAD_DATA;

class CThreadDataList  
{
public:
	HANDLE RemoveThread(int nIndex);
	HANDLE GetThread(int nIndex);
	void SetThread(int nIndex, HANDLE hThread);
	int AddThread();
	CThreadDataList();
	virtual ~CThreadDataList();

	int m_nMax;         // size of slot table below (in bits)
	CTypedPtrArray <CPtrArray, THREAD_DATA*> m_list;  // list of CThreadData structures
	CRITICAL_SECTION m_sect;
};

#endif // !defined(AFX_THREADDATALIST_H__B48D5DA3_A01F_449F_AB5D_12D8987F6A75__INCLUDED_)
