// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__522DC4A4_8336_4EB7_B434_5E8D6BA987D9__INCLUDED_)
#define AFX_STDAFX_H__522DC4A4_8336_4EB7_B434_5E8D6BA987D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <atlstr.h>
//#include "Common.h"

#include "../_common/log.h"


#pragma pack(1)
// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

LONG WINAPI VectoredHandler_Engine(struct _EXCEPTION_POINTERS *ExceptionInfo);

#endif // !defined(AFX_STDAFX_H__522DC4A4_8336_4EB7_B434_5E8D6BA987D9__INCLUDED_)
