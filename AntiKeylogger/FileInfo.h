// FileInfo.h: interface for the CFileInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEINFO_H__841F42B7_0AE2_44E6_8D1F_F1B1F9F22945__INCLUDED_)
#define AFX_FILEINFO_H__841F42B7_0AE2_44E6_8D1F_F1B1F9F22945__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFileInfo  
{
public:
	BYTE* m_pbState;
	HICON m_hIcon;
	WCHAR m_strInfo[MAX_PATH], m_strPathName[MAX_PATH], m_strFileName[_MAX_FNAME];
	CFileInfo(DWORD* pdwData);
	virtual ~CFileInfo();
protected:
	void GetFileInfo();
};

#endif // !defined(AFX_FILEINFO_H__841F42B7_0AE2_44E6_8D1F_F1B1F9F22945__INCLUDED_)
