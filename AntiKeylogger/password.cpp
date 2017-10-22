#include "StdAfx.h"
#include "Constdef.h"
#include "Global.h"
#include "password.h"
#include "Resource.h"

HWND hWnd_4457FC = NULL;
HWND btnOk, btnCancel, label_445808, edit_44580C;
PPARAM param_444144 = NULL;
BOOL bool_445868 = FALSE;

DWORD sub_401040(PWSTR str, DWORD* dwData, HWND hWnd)
{
	PARAM param;

	SendMessageW(hWnd_4457FC, WM_COMMAND, 0, (LPARAM)btnCancel);
	
	param.str = str;
	param.hWnd = hWnd;
	param.para1 = -1;
	param.para2 = 1;
	hWnd_4457FC = sub_4010D0(hWnd, &param);
	
	if(hWnd_4457FC == NULL)
		return 1;

	sub_4015C0(hWnd_4457FC);
    *dwData = param.para1;
	hWnd_4457FC = NULL;

	return param.para2;
}

HWND sub_4010D0(HWND hWndParent, PPARAM param)
{
    int cxScreen, cyScreen, cyCaption, nHeight;
	WNDCLASSW wc;
	PPEB peb;
	DWORD dwExStyle, dwStyle;

    RtlZeroMemory(&wc, sizeof(WNDCLASSW));

	cxScreen = GetSystemMetrics(SM_CXSCREEN);
	cyScreen = GetSystemMetrics(SM_CYSCREEN);
    cyCaption = GetSystemMetrics(SM_CYCAPTION);
	nHeight = cyCaption + 0x80;

	wc.hbrBackground = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
    wc.hCursor = LoadCursorW(NULL, (LPCWSTR)IDC_ARROW);
	
	peb = GetPEB();

	wc.hInstance = (HINSTANCE)peb->ImageBaseAddress;
	wc.lpfnWndProc = (WNDPROC)sub_401210;
	wc.lpszClassName = L"AntiKeylogger";
	wc.hIcon = LoadIcon(wc.hInstance, (LPCTSTR)IDR_MAINFRAME);
	RegisterClassW(&wc);

	dwExStyle = WS_EX_CONTROLPARENT | WS_EX_WINDOWEDGE | WS_EX_TOPMOST | WS_EX_DLGMODALFRAME;
	dwStyle = WS_POPUP | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CAPTION | WS_SYSMENU;
	
	return CreateWindowExW(dwExStyle, wc.lpszClassName, L"AntiKeylogger", dwStyle, (cxScreen - 320) / 2, (cyScreen - nHeight) / 2,
							320, nHeight, hWndParent, NULL, wc.hInstance, param);

}

LRESULT CALLBACK sub_401210(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    DWORD dwWaitResult;
	PWSTR str = NULL;
	
	while(dwWaitResult = WaitForSingleObject(hEvent_445810, -1))
	{
		if(dwWaitResult == WAIT_FAILED)
			break;
	}

	if(param_444144 != NULL && param_444144->hWnd != NULL) 
	{
		if(bool_445868 == FALSE && message != WM_DESTROY) {
			SetEvent(hEvent_445810);
			DestroyWindow(hWnd);
			EnableWindow(param_444144->hWnd, FALSE);
		}
		else {
			SetEvent(hEvent_445810);
			EnableWindow(param_444144->hWnd, FALSE);
		}
	}
	else
		SetEvent(hEvent_445810);

    switch(message)
	{
	case WM_CREATE:
		HMENU hMenu;
		CREATESTRUCT* cs;
		DWORD dwStyle;

		hMenu = GetSystemMenu(hWnd, FALSE);
		DeleteMenu(hMenu, SC_SIZE, MF_BYCOMMAND);
		DeleteMenu(hMenu, SC_MINIMIZE, MF_BYCOMMAND);
		DeleteMenu(hMenu, SC_MAXIMIZE, MF_BYCOMMAND);
		DeleteMenu(hMenu, SC_RESTORE, MF_BYCOMMAND);
		
		cs = (CREATESTRUCT*)lParam;
		param_444144 = (PPARAM)cs->lpCreateParams;
		
		while(dwWaitResult = WaitForSingleObject(hEvent_445810, -1))
		{
			if(dwWaitResult == WAIT_FAILED)
				break;
		}

		if(param_444144)
		{
			long len = wcslen(param_444144->str);
			str = (PWSTR)RtlAllocateHeap(g_hProcessHeap, 8, 2 * len + 2);
			if(str)
				wcscpy(str, param_444144->str);
		}
		
		SetEvent(hEvent_445810);
		if(str)
		{
			dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_TABSTOP;
			btnOk = CreateWindowExW(WS_OVERLAPPED, L"Button", L"Ok", dwStyle | BS_DEFPUSHBUTTON, 0x30, 0x4A, 0x40, 0x16, hWnd, NULL, NULL, NULL);
			btnCancel = CreateWindowExW(WS_OVERLAPPED, L"Button", L"Cancel", dwStyle, 0xD0, 0x4A, 0x40, 0x16, hWnd, NULL, NULL, NULL);
			label_445808 = CreateWindowExW(WS_OVERLAPPED, L"Static", NULL, WS_CHILD | WS_VISIBLE, 0x20, 0x08, 0x100, 0x16, hWnd, NULL, NULL, NULL);
            SetWindowTextW(label_445808, str);
			edit_44580C = CreateWindowExW(WS_EX_CLIENTEDGE, L"Edit", NULL, dwStyle | WS_EX_TRANSPARENT, 0x20, 0x20, 0x100, 0x16, hWnd, NULL, NULL, NULL);
			SetFocus(edit_44580C);
			RtlFreeHeap(g_hProcessHeap, 0, str);
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		param_444144 = NULL;
		break;

	case WM_CLOSE:
		while(dwWaitResult = WaitForSingleObject(hEvent_445810, -1))
		{
			if(dwWaitResult == WAIT_FAILED)
				break;
		}
		if(param_444144)
		{
			param_444144->para1 = -1;
			param_444144->para2 = 1;
		}
		SetEvent(hEvent_445810);
		DestroyWindow(hWnd);
		break;

	case WM_COMMAND:
		if(lParam == (LPARAM)btnOk)
		{
			while(dwWaitResult = WaitForSingleObject(hEvent_445810, -1))
			{
				if(dwWaitResult == WAIT_FAILED)
					break;
			}

			LONG len = GetWindowTextLengthW(edit_44580C);
			if(param_444144)
			{
				str = (PWSTR)RtlAllocateHeap(g_hProcessHeap, 8, 2 * len + 2);
				GetWindowTextW(edit_44580C, str, len + 1);
				if(wcslen(str) >= 5)
				{
					param_444144->para1 = sub_401660(str);
					param_444144->para2 = 0;
				}
				else
				{
					param_444144->para1 = -1;
					param_444144->para2 = 2;
				}
				RtlFreeHeap(g_hProcessHeap, 0, str);
			}

			SetEvent(hEvent_445810);
			DestroyWindow(hWnd);
		}
		else if(lParam == (LPARAM)btnCancel)
		{
			while(dwWaitResult = WaitForSingleObject(hEvent_445810, -1))
			{
				if(dwWaitResult == WAIT_FAILED)
					break;
			}

			if(param_444144)
			{
				param_444144->para1 = -1;
				param_444144->para2 = 1;
			}

			SetEvent(hEvent_445810);
			DestroyWindow(hWnd);
		}

		break;

	default:
		break;
	}

	return DefWindowProcW(hWnd, message, wParam, lParam);
}

void sub_4015C0(HWND hWnd)
{
    MSG msg;

	while(GetMessageW(&msg, NULL, 0, 0))
	{
		if(IsDialogMessageW(hWnd, &msg))
		{
			if(msg.message == WM_KEYDOWN)
			{
				if(msg.wParam == VK_RETURN)
					SendMessageW(hWnd, WM_COMMAND, 0, (LPARAM)btnOk);
				else if(msg.wParam == VK_ESCAPE) 
					SendMessageW(hWnd, WM_COMMAND, 0, (LPARAM)btnCancel);
			}			
		}
		else
		{
            TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	return;
}

DWORD sub_401660(PWSTR str)
{
	int index = 0;
	DWORD dwValue, dwShift, dwResult = 0;
    
	while(str[index + 1])
	{
        dwShift = (str[index] & 0x5F5F) % 5;
        dwValue = str[index + 1] & 0x5F5F;
		dwResult += dwValue << dwShift;
		index ++;
	}

    dwValue = dwResult % 0xDfD5;
	dwValue = dwValue << 0x0E;
	dwValue |= (dwResult % 0x358F);
	dwResult = (dwValue << 2) | (dwResult & 3);

	return dwResult;
}