// ThreadDataList.cpp: implementation of the CThreadDataList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AntiKeylogger.h"
#include "ThreadDataList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CThreadDataList::CThreadDataList()
{
	m_nMax = 0;
	InitializeCriticalSection(&m_sect);
}

CThreadDataList::~CThreadDataList()
{
	THREAD_DATA* pData;
	for (int i = 0; i < m_list.GetSize(); i++)
	{
		pData = (THREAD_DATA*)m_list.GetAt(i);
		if (pData) 
			delete pData;
	}
	m_list.RemoveAll();

	DeleteCriticalSection(&m_sect);
}

int CThreadDataList::AddThread()
{
	EnterCriticalSection(&m_sect);

	THREAD_DATA* pData = new THREAD_DATA;
	pData->nIndex = m_nMax;
	m_nMax ++;
	m_list.Add(pData);

	LeaveCriticalSection(&m_sect);
	return pData->nIndex;
}

void CThreadDataList::SetThread(int nIndex, HANDLE hThread)
{
	EnterCriticalSection(&m_sect);

	THREAD_DATA* pData;
	for (int i = 0; i < m_list.GetSize(); i++)
	{
		pData = (THREAD_DATA*)m_list.GetAt(i);
		if (pData && pData->nIndex == nIndex) {
			pData->hThread = hThread;
			break;
		}
	}

	LeaveCriticalSection(&m_sect);
}

HANDLE CThreadDataList::GetThread(int nIndex)
{
	EnterCriticalSection(&m_sect);

	THREAD_DATA* pData;
	HANDLE hThread = NULL;
	for (int i = 0; i < m_list.GetSize(); i++)
	{
		pData = (THREAD_DATA*)m_list.GetAt(i);
		if (pData && pData->nIndex == nIndex) {
			hThread = pData->hThread;
			break;
		}
	}

	LeaveCriticalSection(&m_sect);
	return hThread;
}

HANDLE CThreadDataList::RemoveThread(int nIndex)
{
	EnterCriticalSection(&m_sect);

	THREAD_DATA* pData;
	HANDLE hThread = NULL;
	for (int i = 0; i < m_list.GetSize(); i++)
	{
		pData = (THREAD_DATA*)m_list.GetAt(i);
		if (pData && pData->nIndex == nIndex) {
			m_list.RemoveAt(i);
			hThread = pData->hThread;
			delete pData;
			break;
		}
	}

	LeaveCriticalSection(&m_sect);
	return hThread;
}
