// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__09091C3D_C314_4F2B_AD62_302409636E7F__INCLUDED_)
#define AFX_STDAFX_H__09091C3D_C314_4F2B_AD62_302409636E7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define DWORD_PTR DWORD

// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <Ws2spi.h>
#include <WinSock2.h>
#include <stdio.h>

void PrintLog(const char *format,...);
// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

BOOL	WriteFileLog(char *szFormat, ...);

#endif // !defined(AFX_STDAFX_H__09091C3D_C314_4F2B_AD62_302409636E7F__INCLUDED_)
