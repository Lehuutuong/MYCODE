/////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2007 PSJDC.
//
//						AppDebug.h	
//
//	This file is created [8/23/2007 by LMC]
//	comment is written in Korean.
//
//	Main purpose
//
//	�������α׶� debug�� ���� ����Դϴ�.
//
//	Modified History:
//
//		
//
/////////////////////////////////////////////////////////////////////////////

//#define REMOVE_DBG_STR

#ifndef REMOVE_DBG_STR
void DebugPrint(const wchar_t*	p_lpFormat, ...);
void DebugPrint(const char*	p_lpFormat, ...);
#else
#define DebugPrint
#endif

void LoadDebugSetting();
extern BOOL g_bMsgDbgOut;
extern BOOL g_bMsgDbgAdd;
