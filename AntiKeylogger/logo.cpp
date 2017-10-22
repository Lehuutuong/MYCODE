#include "StdAfx.h"
#include "Global.h"
#include "ntdll.h"
#include "logo.h"
#include "Resource.h"

typedef BOOL (_stdcall *lpfn)(HWND, COLORREF, BYTE, DWORD);
lpfn SetLayeredWindowAttributes = NULL;
DWORD dword_4443B0;
BYTE byte_4443B4 = 5;
BOOL bool_4443B8 = FALSE;
BOOL bool_445D10 = FALSE;
HBRUSH hBrush_4443BC = NULL;
HWND prog_444390 = NULL;
HFONT hFont_444394 = NULL;
DWORD dword_445D1C = 0;

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED           0x00080000
#define LWA_COLORKEY            0x00000001
#define LWA_ALPHA               0x00000002
#endif

DWORD WINAPI sub_4389E0(LPVOID lpParam)
{
	UUID Uuid;
	PWSTR StringUuid;
	WNDCLASSW wc;
	PPEB peb = GetPEB();
	HWND hWnd;
	MSG msg;

	SetLayeredWindowAttributes = (lpfn)GetProcAddress(hLibUser32, "SetLayeredWindowAttributes");
	UuidCreate(&Uuid);
	UuidToStringW(&Uuid, &StringUuid);
	wc.cbWndExtra = 0;
	wc.cbClsExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.hCursor = LoadCursorW(NULL, (LPCWSTR)IDC_WAIT);
	wc.hIcon = NULL;
	wc.hInstance = (HINSTANCE)peb->ImageBaseAddress;
	wc.lpfnWndProc = (WNDPROC)sub_438640;
	wc.lpszClassName = StringUuid;
	wc.lpszMenuName = NULL;
	wc.style = 0;
	dword_4443B0 = 0x14;
	byte_4443B4 = 0;
	if(RegisterClassW(&wc) == 0)
	{
		RpcMgmtStatsVectorFree((RPC_STATS_VECTOR **)&StringUuid);
		DeleteObject(wc.hCursor);
		return 0;
	}

	hWnd = CreateWindowExW(0xA8, StringUuid, NULL, WS_BORDER | WS_POPUP, GetSystemMetrics(SM_CXSCREEN) / 2 - 0x7E, GetSystemMetrics(SM_CYSCREEN) / 2 - 0x60,
							0xFC, 0xC1, NULL, NULL, wc.hInstance, NULL);
	SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetTimer(hWnd, 1, 1000, NULL);
	SetTimer(hWnd, 2, 10, NULL);
	ShowWindow(hWnd, SW_SHOW);
	bool_4443B8 = TRUE;

	while(bool_4443B8) {
		if(GetMessageW(&msg, NULL, 0, 0))
		{
			if(bool_445D10)
			{
				KillTimer(hWnd, 2);
				SetTimer(hWnd, 3, 10, NULL);
			}
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}
	
	DestroyWindow(hWnd);
	UnregisterClassW(StringUuid, wc.hInstance);
	RpcMgmtStatsVectorFree((RPC_STATS_VECTOR **)&StringUuid);
	DeleteObject(wc.hCursor);

	return 0;
}

LRESULT CALLBACK sub_438640(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HBITMAP hBmp, hOldBmp;
	HDC hdc, hMemDC;
	HFONT hOldFont;
	HBRUSH hBrush;
	RECT rect;
	BITMAP bmp;
	PAINTSTRUCT ps;

	switch(message)
	{
	case WM_DESTROY:
		if(InSendMessage()) break;
		if(hBrush_4443BC)
			DeleteObject(hBrush_4443BC);
		if(prog_444390)
		{
			DestroyWindow(prog_444390);
			KillTimer(hWnd, 4);
		}
		DeleteObject(hFont_444394);
		break;

	case WM_CREATE:
		if(InSendMessage()) break;
		hFont_444394 = CreateFontW(0xC, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"Arial");
		hBrush_4443BC = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
		prog_444390 = CreateWindowExW(0, PROGRESS_CLASSW, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 0x0A, 0x9B, 0xE8, 0xE, hWnd, NULL, NULL, NULL);
		SendMessageW(prog_444390, PBM_SETRANGE, 0, MAKELPARAM(0, 50));
		SendMessageW(prog_444390, PBM_SETBARCOLOR, 0, RGB(0, 0, 0));
		SetClassLongW(prog_444390, GCL_HBRBACKGROUND, (LONG)hBrush_4443BC);
		SetTimer(hWnd, 4, 10, NULL);
		break;
	case WM_TIMER:
		{
			switch(wParam)
			{
			case 1:
				dword_4443B0 --;
				if(dword_4443B0 == 0)
				{
					KillTimer(hWnd, 1);
					SetTimer(hWnd, 3, 10, NULL);
				}
				return 0;

			case 2:
				byte_4443B4 += 5;
				SetLayeredWindowAttributes(hWnd, 0, byte_4443B4, LWA_ALPHA);
				if(byte_4443B4 >= 255)
					KillTimer(hWnd, 2);
				return 0;

			case 3:
				byte_4443B4 -= 5;
				SetLayeredWindowAttributes(hWnd, 0, byte_4443B4, LWA_ALPHA);
				if(byte_4443B4 <= 0)
				{
					KillTimer(hWnd, 3);
					bool_4443B8 = FALSE;
				}
				return 0;

			case 4:
				sub_4385F0();
				if(wsz_445E18 == NULL) return 0;
				hdc = GetDC(hWnd);
				hOldFont = (HFONT)SelectObject(hdc, hFont_444394);
				SetBkMode(hdc, TRANSPARENT);
				SetTextColor(hdc, 0);
				hBrush = (HBRUSH)GetClassLongW(hWnd, GCL_HBRBACKGROUND);
				rect.left = 0x11; rect.top = 0xAB; rect.right = 0xEC; rect.bottom = 0xBC;
				FillRect(hdc, &rect, hBrush);
				DrawTextW(hdc, wsz_445E18, wcslen(wsz_445E18), &rect, DT_END_ELLIPSIS | DT_NOCLIP | DT_SINGLELINE | DT_VCENTER);
				SelectObject(hdc, hOldFont);
				ReleaseDC(hWnd, hdc);
				return 0;
			}
		}

	case WM_PAINT:
		hBmp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_LOGO));
		if(hBmp == NULL) return 0;
		hdc = BeginPaint(hWnd, &ps);
		hMemDC = CreateCompatibleDC(hdc);
		hOldBmp = (HBITMAP)SelectObject(hMemDC, hBmp);
		GetObject(hBmp, sizeof(BITMAP), &bmp);
		BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, hMemDC, 0, 0, SRCCOPY);
		SelectObject(hMemDC, hOldBmp);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_COMMAND:
	case WM_SYSCOMMAND:
	case WM_CLOSE:
	case WM_QUIT:
		return 0;

	default:
		break;
	}

	return DefWindowProcW(hWnd, message, wParam, lParam);
}

BOOL sub_4385F0()
{
	if(prog_444390 == NULL)
		return FALSE;

	dword_445D1C ++;
	if(dword_445D1C <= 50)
	{
		SendMessageW(prog_444390, PBM_SETPOS, dword_445D1C, 0);
		return TRUE;
	}
	else
	{
		dword_445D1C = 0;
		SendMessageW(prog_444390, PBM_SETPOS, dword_445D1C, 0);
		return FALSE;
	}
}