#include "stdafx.h"
#include "ToD.h"
#include <time.h>
//pepsi_marius. 
#include "SkillInfo.h"

COLORREF pixel[800*600];

void WriteBMPFile(HBITMAP bitmap, LPTSTR filename, HDC hDC)
{
	BITMAPINFO dib_define;
	dib_define.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	dib_define.bmiHeader.biWidth = 220;
	dib_define.bmiHeader.biHeight = 60;
	dib_define.bmiHeader.biPlanes = 1;
	dib_define.bmiHeader.biBitCount = 24;
	dib_define.bmiHeader.biCompression = BI_RGB;
	dib_define.bmiHeader.biSizeImage = (((220 * 24 + 31) & ~31) >> 3) * 60;
	dib_define.bmiHeader.biXPelsPerMeter = 0;
	dib_define.bmiHeader.biYPelsPerMeter = 0;
	dib_define.bmiHeader.biClrImportant = 0;
	dib_define.bmiHeader.biClrUsed = 0;

	BYTE *p_image_data = NULL;
	HBITMAP h_bitmap = CreateDIBSection(hDC, &dib_define, DIB_RGB_COLORS, (void**) &p_image_data, 0, 0);
	HDC h_memory_dc = CreateCompatibleDC(hDC);
	HBITMAP h_old_bitmap = (HBITMAP) SelectObject(h_memory_dc, h_bitmap);
	BitBlt(h_memory_dc, 0, 0, 220, 60, hDC, 0, 0, SRCCOPY);
	SelectObject(h_memory_dc, h_old_bitmap);
	DeleteDC(h_memory_dc);
	BITMAPFILEHEADER dib_format_layout;
	ZeroMemory(&dib_format_layout, sizeof(BITMAPFILEHEADER));
	dib_format_layout.bfType = *(WORD*)"BM";
	dib_format_layout.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dib_define.bmiHeader.biSizeImage;
	dib_format_layout.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	int i = 0;	
	WCHAR szName[MAX_PATH];	
	HANDLE hf;

	while(TRUE)
	{		
		swprintf(szName, L"C:\\cleanpad%.5d.bmp", i);
		hf = CreateFile(szName, GENERIC_READ | GENERIC_WRITE, (DWORD) 0,
				NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL,
				(HANDLE) NULL);
		if (hf != INVALID_HANDLE_VALUE)
			break;
		i ++;
	}
	DWORD dwBytesWritten;
	WriteFile(hf, &dib_format_layout, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
	WriteFile(hf, &dib_define, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
	WriteFile(hf, p_image_data, dib_define.bmiHeader.biSizeImage, &dwBytesWritten, NULL);
/*	FILE* p_file = fopen(filename, "wb");
	if (p_file != NULL)
	{
		fwrite(&dib_format_layout, 1, sizeof(BITMAPFILEHEADER), p_file);
		fwrite(&dib_define, 1, sizeof(BITMAPINFOHEADER), p_file);
		fwrite(p_image_data, 1, dib_define.bmiHeader.biSizeImage, p_file);
		fclose(p_file);
	}*/
	FlushFileBuffers(hf);
	CloseHandle(hf);
	if (h_bitmap)
	{
		SelectObject(h_memory_dc, h_old_bitmap);
		DeleteObject(h_bitmap);
	}
	if (h_memory_dc)
		DeleteDC(h_memory_dc);
}

//pepsi_marius. 
void WriteBMPPatternFile(HBITMAP bitmap, LPTSTR filename, HDC hDC)
{
	BITMAP bmpTmp ; 
	int res = ::GetObject(bitmap, sizeof(BITMAP), (LPVOID)&bmpTmp);
	int nWIdth = (int)bmpTmp.bmWidth ;
	int nHeight = (int)bmpTmp.bmHeight ;

	BITMAPINFO dib_define;
	dib_define.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	dib_define.bmiHeader.biWidth = nWIdth;
	dib_define.bmiHeader.biHeight = nHeight;
	dib_define.bmiHeader.biPlanes = 1;
	dib_define.bmiHeader.biBitCount = 24;
	dib_define.bmiHeader.biCompression = BI_RGB;
	dib_define.bmiHeader.biSizeImage = (((nWIdth * 24 + 31) & ~31) >> 3) * nHeight;
	dib_define.bmiHeader.biXPelsPerMeter = 0;
	dib_define.bmiHeader.biYPelsPerMeter = 0;
	dib_define.bmiHeader.biClrImportant = 0;
	dib_define.bmiHeader.biClrUsed = 0;
	
	BYTE *p_image_data = NULL;
	HBITMAP h_bitmap = CreateDIBSection(hDC, &dib_define, DIB_RGB_COLORS, (void**) &p_image_data, 0, 0);
	HDC h_memory_dc = CreateCompatibleDC(hDC);
	HBITMAP h_old_bitmap = (HBITMAP) SelectObject(h_memory_dc, h_bitmap);
	BitBlt(h_memory_dc, 0, 0, nWIdth, nHeight, hDC, 0, 0, SRCCOPY);
	SelectObject(h_memory_dc, h_old_bitmap);
	DeleteDC(h_memory_dc);
	BITMAPFILEHEADER dib_format_layout;
	ZeroMemory(&dib_format_layout, sizeof(BITMAPFILEHEADER));
	dib_format_layout.bfType = *(WORD*)"BM";
	dib_format_layout.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dib_define.bmiHeader.biSizeImage;
	dib_format_layout.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	int i = 0;
	WCHAR szName[MAX_PATH];
	HANDLE hf;
	while(TRUE)
	{
		swprintf(szName, L"C:\\minate%.5d.bmp", i);
		hf = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, (DWORD) 0,
				NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL,
				(HANDLE) NULL);
		if (hf != INVALID_HANDLE_VALUE)
			break;
		i ++;
	}
	DWORD dwBytesWritten;
	WriteFile(hf, &dib_format_layout, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
	WriteFile(hf, &dib_define, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
	WriteFile(hf, p_image_data, dib_define.bmiHeader.biSizeImage, &dwBytesWritten, NULL);
/*	FILE* p_file = fopen(filename, "wb");
	if (p_file != NULL)
	{
		fwrite(&dib_format_layout, 1, sizeof(BITMAPFILEHEADER), p_file);
		fwrite(&dib_define, 1, sizeof(BITMAPINFOHEADER), p_file);
		fwrite(p_image_data, 1, dib_define.bmiHeader.biSizeImage, p_file);
		fclose(p_file);
	}*/

	//{ pepsi_marius. 
	// 이미지의 자료값을 헥스로 넘기기 
	//	wcscpy(szName ,filename) ;
	//	wcscat(szName, L".bin"	);
	//	char szBuf[MAX_PATH];
	//	wcstombs(szBuf, szName, MAX_PATH);
	//	
	//#ifdef _DEBUGPRINT
	//	PrintLogA(szBuf);
	//#endif
	//
	//	PrintHexToFile(p_image_data, dib_define.bmiHeader.biSizeImage, szBuf);
	//}

	FlushFileBuffers(hf);
	CloseHandle(hf);
	if (h_bitmap)
	{
		SelectObject(h_memory_dc, h_old_bitmap);
		DeleteObject(h_bitmap);
	}
	if (h_memory_dc)
		DeleteDC(h_memory_dc);
}
HDC GetPatternDC(LPTSTR filename)
{
	HDC dc; 
	char szAppPath[MAX_PATH];
	GetModuleFileNameA(NULL, szAppPath, MAX_PATH);
	
	char *p = strrchr(szAppPath, '\\');
	if(p)
	{
		*p = '\0';
	}
#ifdef _DEBUGPRINT
	PrintLogA(szAppPath);
#endif
	return dc ;
}

#define TDT_LEFT 1
#define TDT_TOP	2

BOOL IsTarget(HDC hDC, int i, int j, BYTE byDirection)
{
	int k1;
	if (IsSimillarColor(GetPixel(hDC, i, j), RGB(50, 50, 50)))
	{
		if (byDirection & TDT_LEFT)
		{
			for(k1 = 1; k1 < 10; k1 ++)
			{
				if (!IsSimillarColor(GetPixel(hDC, i - k1, j), RGB(0, 0, 0)))
					return FALSE;
				if (!IsSimillarColor(GetPixel(hDC, i + k1, j), RGB(50, 50, 50)))
					return FALSE;
			}
		}
		if (byDirection & TDT_TOP)
		{
			for(k1 = 1; k1 < 20; k1 ++)
			{
				if (!IsSimillarColor(GetPixel(hDC, i, j - k1), RGB(0, 0, 0)))
					return FALSE;
				if (!IsSimillarColor(GetPixel(hDC, i, j + k1), RGB(50, 50, 50)))
					return FALSE;
			}
		}
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL IsSimillarColor(COLORREF crPixel, COLORREF crSample)
{
	BYTE r = GetRValue(crPixel), g = GetGValue(crPixel), b = GetBValue(crPixel);
	BYTE rSample = GetRValue(crSample), gSample = GetGValue(crSample), bSample = GetBValue(crSample);
	if ((r - rSample) > 25 || (r < rSample))
		return FALSE;
	if ((b - bSample) > 25 || (g < gSample))
		return FALSE;
	if ((g - gSample) > 25 || (b < bSample))
		return FALSE;
	if (abs(r - g) > 23)
		return FALSE;
	if (abs(r - b) > 23)
		return FALSE;
	if (abs(g - b) > 23)
		return FALSE;
	return TRUE;
}

// kan 0818
BOOL CalculateCleanPadArea_3(HDC hDC)
{
	int i, j;
	int u, v;
	BOOL bFind = FALSE;

	RECT certiPt;
	certiPt.left = certiPt.right = certiPt.top = certiPt.bottom = 2000;
	BOOL bExistPrev = 0;
	BOOL bFont = 0;

	int top_x = 0, top_y = 0;
	PBYTE pGrayImgBuf = NULL, pEdgeImgBuf = NULL;
	int nW = 800, nH = 600; 
	pGrayImgBuf = (PBYTE) malloc(nW*nH); ZeroMemory(pGrayImgBuf, nW*nH);
	pEdgeImgBuf = (PBYTE) malloc(nW*nH); ZeroMemory(pEdgeImgBuf, nW*nH);

	for(i = 0; i < nH; i ++) for(j = 0; j < nW; j ++)
	{
		COLORREF pix = GetPixel(hDC, j, i);
		*(pGrayImgBuf+i*nW+j) = (GetRValue(pix) + GetGValue(pix) + GetBValue(pix))/3;
	}

	CannyEdge(pGrayImgBuf, nW, nH, 255, 255, pEdgeImgBuf);
	GetOutlineCoor(pEdgeImgBuf, nW, nH, top_x, top_y);

	if(pGrayImgBuf != NULL) free(pGrayImgBuf);
	if(pEdgeImgBuf != NULL) free(pEdgeImgBuf);

	if (top_x != 0 && top_y != 0)
	{		
		HDC hDCCleanPad = CreateCompatibleDC(hDC);
		HBITMAP hBmpCleanPad = CreateCompatibleBitmap(hDC, 220, 60);
		SelectObject(hDCCleanPad, hBmpCleanPad);
		BitBlt(hDCCleanPad, 0, 0, 220, 60, hDC, top_x + 46, top_y + 177, SRCCOPY);
		WriteBMPFile(hBmpCleanPad, L"C:\\cleanpad.bmp", hDCCleanPad);
		DeleteObject(hBmpCleanPad);
		DeleteDC(hDCCleanPad);
		g_ConfirmPt.x = top_x + 97;
		g_ConfirmPt.y = top_y + 290;
		g_CancelPt.x = g_ConfirmPt.x + 120;
		g_CancelPt.y = g_ConfirmPt.y;
		POINT pt;
		pt.x = top_x + 90;
		pt.y = top_y + 250;
		g_InputPt.x = pt.x;
		g_InputPt.y = pt.y;
#ifdef _DEBUGPRINT
		PrintLogA("CalculateCleanPadArea_3 이미지탐색 성공");
		PrintLogA("패드 이미지 위치 x: %x, y: %x", top_x + 68, top_y + 182);
#endif
		g_infoChar.nState = AS_CLICK_CLEANPAD;
		g_nRestTime = 0;		
	}
	else
	{
		PrintLogA("CalculateCleanPadArea_3 이미지 탐색 실패");
		return FALSE;
	}
	return TRUE;
}
// kus 0620
BOOL CalculateCleanPadArea_2(HDC hDC)
{
	int i, j;
	int u, v;
	BOOL bFind = FALSE;

	RECT certiPt;
	certiPt.left = certiPt.right = certiPt.top = certiPt.bottom = 2000;
	BOOL bExistPrev = 0;
	BOOL bFont = 0;

	RECT rect;
	::GetWindowRect(g_hGameWnd, &rect);

	int nW = rect.right - rect.left, nH = rect.bottom - rect.top; 

	for(u = nH-10; u >= 10 ; u --)
	{
		bFont = 0;
		for(v = 10; v < nW ; v ++)
		{
			COLORREF pix = GetPixel(hDC, v, u);
			if(GetRValue(pix) >= 253 && GetGValue(pix) >= 253 && GetBValue(pix) >= 253 )
			{
				if(certiPt.left == 2000)
					certiPt.left = v;
				else 
					certiPt.right = v;

				bFont = 1;				
			}			
		}
		if(bFont == 1)
		{
			if(bExistPrev == 0)
				certiPt.bottom = u; 
			else
				certiPt.top = u;
			bExistPrev = 1;
		}
		else
		{
			if(certiPt.left != 2000 && certiPt.right != 2000 && certiPt.top != 2000 && certiPt.bottom != 2000 )
				if(certiPt.bottom - certiPt.top < 14 && certiPt.right - certiPt.left > 110 && certiPt.right - certiPt.left < 130)
				{
					PrintLog(L"focus: %d, %d, %d, %d", certiPt.left, certiPt.right, certiPt.top, certiPt.bottom);
					break;
				}
				bExistPrev = 0;
				certiPt.left = certiPt.right = certiPt.top = certiPt.bottom = 2000;

		}
	}

	if (u > 220)
	{
		i = certiPt.left - 61; j = certiPt.top - 73;
		HDC hDCCleanPad = CreateCompatibleDC(hDC);
		HBITMAP hBmpCleanPad = CreateCompatibleBitmap(hDC, 220, 60);
		SelectObject(hDCCleanPad, hBmpCleanPad);
		BitBlt(hDCCleanPad, 0, 0, 220, 60, hDC, i + 33, j + 7, SRCCOPY);
		WriteBMPFile(hBmpCleanPad, L"C:\\cleanpad.bmp", hDCCleanPad);
		DeleteObject(hBmpCleanPad);
		DeleteDC(hDCCleanPad);
		g_ConfirmPt.x = i + 120;
		g_ConfirmPt.y = j + 120;
		g_CancelPt.x = g_ConfirmPt.x + 80;
		g_CancelPt.y = g_ConfirmPt.y;
		POINT pt;
		pt.x = i + 63;
		pt.y = j + 80;
		g_InputPt.x = pt.x;
		g_InputPt.y = pt.y;
#ifdef _DEBUGPRINT
		PrintLogA("패드 이미지 위치 x: %x, y: %x", i + 53, j + 11);
#endif		
		g_infoChar.nState = AS_CLICK_CLEANPAD;
		g_nRestTime = 0;		
	}
	else
	{

		return FALSE;
	}
	return TRUE;
}
BOOL CalculateCleanPadArea(HDC hDC)
{
	int i, j;
	BOOL bFind = FALSE;
	for(j = 150; j < 425; j ++)
	{
		for(i = 10; i < 618; i ++)
		{
			bFind = IsTarget(hDC, i, j, TDT_LEFT | TDT_TOP);
			if (bFind)
			{
				bFind &= IsTarget(hDC, i - 1, j + 5, TDT_LEFT);
				if (bFind)
					bFind &= IsTarget(hDC, i + 5, j, TDT_TOP);
				if (bFind)
					break;
			}
		}
		if (bFind)
			break;
	}
	if (bFind)
	{
		HDC hDCCleanPad = CreateCompatibleDC(hDC);
		HBITMAP hBmpCleanPad = CreateCompatibleBitmap(hDC, 220, 60);
		SelectObject(hDCCleanPad, hBmpCleanPad);
		BitBlt(hDCCleanPad, 0, 0, 220, 60, hDC, i + 33, j + 7, SRCCOPY);
		WriteBMPFile(hBmpCleanPad, L"C:\\cleanpad.bmp", hDCCleanPad);
		DeleteObject(hBmpCleanPad);
		DeleteDC(hDCCleanPad);
		g_ConfirmPt.x = i + 120;
		g_ConfirmPt.y = j + 120;
		g_CancelPt.x = g_ConfirmPt.x + 80;
		g_CancelPt.y = g_ConfirmPt.y;
		POINT pt;
		pt.x = i + 63;
		pt.y = j + 80;
		g_InputPt.x = pt.x;
		g_InputPt.y = pt.y;
#ifdef _DEBUGPRINT
		PrintLogA("패드 이미지 위치 x: %d, y: %d", i + 53, j + 11);
#endif
		g_infoChar.nState = AS_CLICK_CLEANPAD;
		g_nRestTime = 0;	
	}
	else
	{
		PrintLogA("이미지 탐색 실패 - 2");
		return FALSE;
	}
	return TRUE;
}
void Focus()
{
	// 마우스초점주기
	INPUT	input[3];
	RECT	rect;

	GetWindowRect(g_hGameWnd, &rect);
	ZeroMemory(input, sizeof(INPUT)*3);
	rect.left = (rect.left+10)*65536/GetSystemMetrics(SM_CXSCREEN);
	rect.top = (rect.top+10)*65536/GetSystemMetrics(SM_CYSCREEN);
	input[0].type = INPUT_MOUSE;
	input[0].mi.dwFlags = MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE;
	input[0].mi.dx = rect.left;
	input[0].mi.dy = rect.top;
	input[1].type = INPUT_MOUSE;
	input[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_ABSOLUTE;
	input[1].mi.dx = rect.left;
	input[1].mi.dy = rect.top;
	input[2].type = INPUT_MOUSE;
	input[2].mi.dwFlags = MOUSEEVENTF_LEFTUP|MOUSEEVENTF_ABSOLUTE;
	input[2].mi.dx = rect.left;
	input[2].mi.dy = rect.top;
	SendInput(3, input, sizeof(INPUT));
}

DWORD	InputCleanPadFunc(LPVOID lpParam)
{
	DWORD KeyNum = *(PDWORD)lpParam;
	SendKeyInput(g_pAuthKey, KeyNum);
	Sleep(200);
	memcpy(&g_ShareInfo.ptMousePos, &g_ConfirmPt, sizeof(POINT));
	SendCommand(CMD_SET_FOCUS);	
	return 0;
}
DWORD	InputKeyArray(LPVOID lpParam)
{
	DWORD KeyNum = *(PDWORD)lpParam;
	SendKeyInput(g_pAuthKey, KeyNum);
	Sleep(200);	
	return 0;
}
// kus 0604
void RButtonClick(int X, int Y)
{
#ifdef _DEBUGPRINT
	PrintLogA("오른 버튼 포커스 설정 X : %d, Y : %d", X, Y);
#endif
	SetForegroundWindow(g_hGameWnd);
	Sleep(100);
	FocusXY(X, Y);
	Sleep(100);
	::SendNotifyMessage(g_hGameWnd, WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(X, Y));
	Sleep(200);
	::SendNotifyMessage(g_hGameWnd, WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(X, Y));
	Sleep(100);
	FocusXY(X, Y);
}
void  SetFocus(int X, int Y)
{
/*
#ifdef _DEBUGPRINT
	PrintLogA("포커스 설정 X : %d, Y : %d", X, Y);
#endif
	SetForegroundWindow(g_hGameWnd);
	Sleep(100);
	FocusXY(X, Y);
	Sleep(100);
	::SendNotifyMessage(g_hGameWnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(X, Y));
	Sleep(200);
	::SendNotifyMessage(g_hGameWnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(X, Y));
	Sleep(100);
	FocusXY(X, Y);
*/
	RECT	rect;
	SetForegroundWindow(g_hGameWnd);
	GetWindowRect(g_hGameWnd, &rect);
	X+=rect.left;
	Y+=rect.top;
	SetCursorPos(X, Y);
	Sleep(10);	
	
	INPUT an_input = {};
	an_input.type = INPUT_MOUSE;

	an_input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &an_input, sizeof an_input);
	Sleep(500);

	an_input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &an_input, sizeof an_input);
	Sleep(10);
}

void	FocusXY(int X,int Y)
{
	// 마우스초점주기
	INPUT	input[3];
	RECT	rect;
	GetWindowRect(g_hGameWnd, &rect);
	ZeroMemory(input, sizeof(INPUT)*3);
	rect.left = (rect.left+X)*65536/GetSystemMetrics(SM_CXSCREEN);
	rect.top = (rect.top+Y)*65536/GetSystemMetrics(SM_CYSCREEN);
	input[0].type = INPUT_MOUSE;
	input[0].mi.dwFlags = MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE;
	input[0].mi.dx = rect.left;
	input[0].mi.dy = rect.top;
	input[1].type = INPUT_MOUSE;
	input[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_ABSOLUTE;
	input[1].mi.dx = rect.left;
	input[1].mi.dy = rect.top;
	input[2].type = INPUT_MOUSE;
	input[2].mi.dwFlags = MOUSEEVENTF_LEFTUP|MOUSEEVENTF_ABSOLUTE;
	input[2].mi.dx = rect.left;
	input[2].mi.dy = rect.top;
	SendInput(3, input, sizeof(INPUT));
#ifdef _DEBUGPRINT
	PrintLogA("포커스주기 완성");
#endif
}

void SendKeyInput(BYTE* pVirtKey, int nLen)
{
	int i;
#ifdef _DEBUGPRINT
	PrintLogA("키 입력 개수 %d", nLen);
#endif
	for (i = 0; i < nLen; i ++)
	{
		Sleep(200);
#ifdef _DEBUGPRINT
		PrintLogA("키입력 : %c", pVirtKey[i]);
#endif
		SendMessage(g_hGameWnd, WM_CHAR, pVirtKey[i], 0);
	}
#ifdef _DEBUGPRINT
	PrintLogA("키보드입력 완료");
#endif
}

void SendMouseDown(int x, int y)
{
//	INPUT	input[3];
	INPUT	input;
	ZeroMemory(&input, sizeof(input));

	RECT	rect;
	GetWindowRect(g_hGameWnd, &rect);
	rect.left += x; rect.top += y;
	SetCursorPos(rect.left, rect.top);
	Sleep(10);

	rect.left = (rect.left)*65536/GetSystemMetrics(SM_CXSCREEN);
	rect.top = (rect.top)*65536/GetSystemMetrics(SM_CYSCREEN);

	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE;
	input.mi.dx = rect.left;
	input.mi.dy = rect.top;
	SendInput(1, &input, sizeof(INPUT));
	Sleep(10);

	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_ABSOLUTE;
	SendInput(1, &input, sizeof(INPUT));
	Sleep(10);

	input.mi.dwFlags = MOUSEEVENTF_LEFTUP|MOUSEEVENTF_ABSOLUTE;
	SendInput(1, &input, sizeof(INPUT));
	Sleep(10);
}

void SendKeyPress(int nVirtKey)
{
	INPUT input;
	ZeroMemory(&input, sizeof(input));
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = nVirtKey;
	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));
}
// kus 0817 ESC건누름
BOOL	ISClickESC()
{
	POINT pt;
	HDC hDC = GetDC(g_hGameWnd);

	pt.x = 344;
	pt.y = 555;
	int nCount = 0;
#ifdef _DEBUGPRINT
	PrintLogA("HDC %d 게임종료 버튼클릭판정", hDC);
#endif

	for (int i = 0; i < 111; i++)
	{
		COLORREF pixel = GetPixel(hDC, pt.x+i, pt.y);
		int r = GetRValue(pixel), g = GetGValue(pixel), b = GetBValue(pixel);			
		//			PrintLogA("Pixel R-%d G-%d B-%d", r, g, b);
		if (abs(r-125) > 20) continue;
		if (abs(g-125) > 20) continue;
		if (abs(b-125) > 20) continue;
		nCount++;		
	}
#ifdef _DEBUGPRINT
	PrintLogA("일치하는 개수 %d", nCount);
#endif

	if (nCount > 80) return TRUE;
	else return FALSE;
}
// kus 스킬슬롯 활성화 체크
// kus 포화채널
BOOL	ISPoHuaChannel(int index)
{
	POINT pt;
	HDC hDC = GetDC(g_hGameWnd);
	
	pt.x = 274 + (index+2)%3*190;
	pt.y = 210 + (index-1)/3*44;
	int nCount = 0;
#ifdef _DEBUGPRINT
	PrintLogA("HDC %d 포화채널 판정 %d %d", hDC, pt.x, pt.y);
#endif
	
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			COLORREF pixel = GetPixel(hDC, pt.x+i, pt.y+j);
			int r = GetRValue(pixel), g = GetGValue(pixel), b = GetBValue(pixel);			
//			PrintLogA("Pixel R-%d G-%d B-%d", r, g, b);
			if (abs(r-255) > 40) continue;
			if (abs(g-0) > 40) continue;
			if (abs(b-240) > 40) continue;
			nCount++;
		}
	}
#ifdef _DEBUGPRINT
	PrintLogA("일치하는 개수 %d", nCount);
#endif
	
	if (nCount > 20) return TRUE;
	else return FALSE;
}
/*
// kus 0727 던전경매
BOOL	CheckDungonAuction()
{
	POINT pt;
	HDC hDC = GetDC(g_hGameWnd);

	pt.x = 674;
	pt.y = 398;
	int nCount = 0;
#ifdef _DEBUGPRINT
	PrintLogA("HDC %d 던전경매 판정 %d %d", hDC, pt.x, pt.y);
#endif

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			COLORREF pixel = GetPixel(hDC, pt.x+i, pt.y+j);
			int r = GetRValue(pixel), g = GetGValue(pixel), b = GetBValue(pixel);			
			//			PrintLogA("Pixel R-%d G-%d B-%d", r, g, b);
			if (abs(r-255) > 40) continue;
			if (abs(g-255) > 40) continue;
			if (abs(b-255) > 40) continue;
			nCount++;
		}
	}
#ifdef _DEBUGPRINT
	PrintLogA("일치하는 개수 %d", nCount);
#endif

	if (nCount > 40) return TRUE;
	else return FALSE;
}
*/
// kus 0904 라이너스
BOOL	CheckRainesPresent()
{
	POINT pt;
	HDC hDC = GetDC(g_hGameWnd);

	pt.x = 353;
	pt.y = 256;
	int nCount = 0;
#ifdef _DEBUGPRINT
	PrintLogA("HDC %d 던전경매 판정 %d %d", hDC, pt.x, pt.y);
#endif

	for (int i = 0; i < 110; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			COLORREF pixel = GetPixel(hDC, pt.x+i, pt.y+j);
			int r = GetRValue(pixel), g = GetGValue(pixel), b = GetBValue(pixel);			
			//			PrintLogA("Pixel R-%d G-%d B-%d", r, g, b);
			if (abs(r-255) > 40) continue;
			if (abs(g-0) > 40) continue;
			if (abs(b-240) > 40) continue;
			nCount++;
		}
	}
#ifdef _DEBUGPRINT
	PrintLogA("일치하는 개수 %d", nCount);
#endif

	if (nCount > 50) return TRUE;
	else return FALSE;
}

// kus 0727 던전경매
//int	GetActiveSlotIndex()
DWORD GetActiveSlotIndex(LPVOID pParam)
{
	while (TRUE)
	{
		if(g_bGetActiveSlotThreadRunning == FALSE )
		{
			return 0;
		}
		POINT pt;
		HDC hDC = GetDC(g_hGameWnd);

		pt.x = 483;
		pt.y = 556;

		int nCount;
		g_nActiveSlot = 0;
		BYTE Slot = 0;

		for (int i = 0; i < 6; i++)
		{
			nCount = 0;
			for (int j = 0; j < 30; j++)
			{
				for (int k = 0; k < 8; k++)
				{
					COLORREF pixel = GetPixel(hDC, pt.x+i*30+j, pt.y+k);
					int r = GetRValue(pixel), g = GetGValue(pixel), b = GetBValue(pixel);			
					//PrintLogA("Pixel R-%d G-%d B-%d", r, g, b);
					if (abs(r-230) > 40) continue;
					if (abs(g-204) > 100) continue;
					if (abs(b-57) > 60) continue;
					nCount++;
				}			
			}
			PrintLogA("인덱스 %d 갯 %d", i, nCount);
			if (nCount > 10)
			{			
				Slot |= 1 << i;
				g_bActiveSkill[g_nActiveSlot] = i;
				g_nActiveSlot++;
			}
		}
		srand((unsigned int)time(NULL));	


		if (g_nActiveSlot > 0)
		{
			PrintLogA("스킬활성화 슬롯 %x Active Slot Count %d", Slot, g_nActiveSlot);
			//return g_bActiveSkill[rand()%g_nActiveSlot];
		}
		else
		{
			PrintLogA("스킬활성화 없음");
			//return -1;
		}
		Sleep(500) ; 
	}

}
// kus 0727 던전경매
BOOL	CheckDungonAuction()
{
	POINT pt;
	HDC hDC = GetDC(g_hGameWnd);

	pt.x = 622;
	pt.y = 430;
	int nCount = 0;
#ifdef _DEBUGPRINT
	PrintLogA("HDC %d 던전경매 판정 %d %d", hDC, pt.x, pt.y);
#endif

	for (int i = 0; i < 137; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			COLORREF pixel = GetPixel(hDC, pt.x+i, pt.y+j);
			int r = GetRValue(pixel), g = GetGValue(pixel), b = GetBValue(pixel);			
			//			PrintLogA("Pixel R-%d G-%d B-%d", r, g, b);
			if (abs(r-255) > 40) continue;
			if (abs(g-0) > 40) continue;
			if (abs(b-240) > 40) continue;
			nCount++;
		}
	}
#ifdef _DEBUGPRINT
	PrintLogA("일치하는 개수 %d", nCount);
#endif

	if (nCount > 40) return TRUE;
	else return FALSE;
}
// kan 0818 
void CannyEdge(BYTE *pImage, int width, int height, int th_high, int th_low, BYTE *pEdge) 
{ 
	int i, j; 

	int dx, dy, mag, slope, direction; 
	int index, index2; 

	const int fbit = 10; 
	const int tan225 =   424;       // tan25.5 << fbit, 0.4142 
	const int tan675 =   2472;      // tan67.5 << fbit, 2.4142 

	const int CERTAIN_EDGE = 255; 
	const int PROBABLE_EDGE = 100; 

	bool bMaxima; 

	int *mag_tbl = new int[width*height]; 
	int *dx_tbl = new int[width*height]; 
	int *dy_tbl = new int[width*height]; 

	BYTE **stack_top, **stack_bottom; 
	stack_top = new BYTE*[width*height]; 
	stack_bottom = stack_top; 
#define CANNY_PUSH(p)    *(p) = CERTAIN_EDGE, *(stack_top++) = (p) 
#define CANNY_POP()      *(--stack_top) 

	for(i=0; i<width*height; i++) { 
		mag_tbl[i] = 0; 
		pEdge[i] = 0; 
	} 

	// Sobel Edge Detection 
	for(i=1; i<height-1; i++) { 
		index = i*width; 
		for(j=1; j<width-1; j++) { 
			index2 = index+j; 
			// -1 0 1 
			// -2 0 2 
			// -1 0 1 
			dx = pImage[index2-width+1] + (pImage[index2+1]<<1) + pImage[index2+width+1] 
			-pImage[index2-width-1] - (pImage[index2-1]<<1) - pImage[index2+width-1]; 

			// -1 -2 -1 
			//  0  0  0 
			//  1  2  1 
			dy = -pImage[index2-width-1] - (pImage[index2-width]<<1) - pImage[index2-width+1] 
			+pImage[index2+width-1] + (pImage[index2+width]<<1) + pImage[index2+width+1]; 

			mag = abs(dx)+abs(dy);     // magnitude 
			//mag = sqrtf(dx*dx + dy*dy); 

			dx_tbl[index2] = dx; 
			dy_tbl[index2] = dy; 
			mag_tbl[index2] = mag;              // store the magnitude in the table 
		}   // for(j) 
	}   // for(i) 

	for(i=1; i<height-1; i++) { 
		index = i*width; 
		for(j=1; j<width-1; j++) { 
			index2 = index+j; 

			mag = mag_tbl[index2];              // retrieve the magnitude from the table 

			// if the magnitude is greater than the lower threshold 
			if(mag > th_low) { 

				// determine the orientation of the edge 
				dx = dx_tbl[index2]; 
				dy = dy_tbl[index2]; 

				if(dx != 0) { 
					slope = (dy<<fbit)/dx; 

					if(slope > 0) { 
						if(slope < tan225) 
							direction = 0; 
						else if(slope < tan675) 
							direction = 1; 
						else
							direction = 2; 
					} 
					else { 
						if(-slope > tan675) 
							direction = 2; 
						else if(-slope > tan225) 
							direction = 3; 
						else 
							direction = 0; 
					} 
				} 
				else
					direction = 2; 

				bMaxima = true; 
				// perform non-maxima suppression 
				if(direction == 0) { 
					if(mag < mag_tbl[index2-1] || mag < mag_tbl[index2+1]) 
						bMaxima = false; 
				} 
				else if(direction == 1) { 
					if(mag < mag_tbl[index2+width+1] || mag < mag_tbl[index2-width-1]) 
						bMaxima = false; 
				} 
				else if(direction == 2){ 
					if(mag < mag_tbl[index2+width] || mag < mag_tbl[index2-width]) 
						bMaxima = false; 
				} 
				else { // if(direction == 3) 
					if(mag < mag_tbl[index2+width-1] || mag < mag_tbl[index2-width+1]) 
						bMaxima = false; 
				} 

				if(bMaxima) { 
					if(mag > th_high) { 
						pEdge[index2] = CERTAIN_EDGE;           // the pixel does belong to an edge 
						*(stack_top++) = (BYTE*)(pEdge+index2); 
					} 
					else
						pEdge[index2] = PROBABLE_EDGE;          // the pixel might belong to an edge 
				} 
			} 

		}   // for(j) 
	}   // for(i) 

	while(stack_top != stack_bottom) { 
		BYTE* p = CANNY_POP(); 

		if(*(p+1) == PROBABLE_EDGE) 
			CANNY_PUSH(p+1); 
		if(*(p-1) == PROBABLE_EDGE) 
			CANNY_PUSH(p-1); 
		if(*(p+width) == PROBABLE_EDGE) 
			CANNY_PUSH(p+width); 
		if(*(p-width) == PROBABLE_EDGE) 
			CANNY_PUSH(p-width); 
		if(*(p-width-1) == PROBABLE_EDGE) 
			CANNY_PUSH(p-width-1); 
		if(*(p-width+1) == PROBABLE_EDGE) 
			CANNY_PUSH(p-width+1); 
		if(*(p+width-1) == PROBABLE_EDGE) 
			CANNY_PUSH(p+width-1); 
		if(*(p+width+1) == PROBABLE_EDGE) 
			CANNY_PUSH(p+width+1); 
	} 

	for(i=0; i<width*height; i++) 
		if(pEdge[i]!=CERTAIN_EDGE) 
			pEdge[i] = 0; 

	delete mag_tbl; 
	delete dx_tbl; 
	delete dy_tbl; 
	delete [] stack_bottom; 
} 
// kan 0818
void GetOutlineCoor(PBYTE pImg, int imgW, int imgH, int &topx, int &topy)
{
	int i, j;
	topx = 0; topy = 0;
	// topx 좌표 구하기
	for(j = 1; j < 500; j ++) // x
	{
		for(i = 1; i < 300; i ++) // y
		{
			if(*(pImg+i*imgW+j) == 0) continue;
			topy = i;
			int ncnt = 0;
			int ncol = 0;
			do 
			{ 
				i ++;
				ncnt ++;
				ncol = *(pImg+i*imgW+j);
			} while(ncol == 255 && i < imgH);

			if(ncnt == 321) // 클린패드창 높이
			{
				topx = j;
				return;
			}
		}
	}
}

/************************************************************************/
/*							pepsi_marius;  이미지처리관련               */
/************************************************************************/

BOOL GetMinateArea(HDC hDC)
{
	int i, j;
	int u, v;
	BOOL bFind = FALSE;

	RECT certiPt;
	certiPt.left = certiPt.right = certiPt.top = certiPt.bottom = 2000;
	BOOL bExistPrev = 0;
	BOOL bFont = 0;

	int top_x = 0, top_y = 0;
	PBYTE pGrayImgBuf = NULL, pEdgeImgBuf = NULL;
	int nW = 800, nH = 600; 
	pGrayImgBuf = (PBYTE) malloc(nW*nH); ZeroMemory(pGrayImgBuf, nW*nH);
	pEdgeImgBuf = (PBYTE) malloc(nW*nH); ZeroMemory(pEdgeImgBuf, nW*nH);

	for(i = 0; i < nH; i ++) for(j = 0; j < nW; j ++)
	{
		COLORREF pix = GetPixel(hDC, j, i);
		*(pGrayImgBuf+i*nW+j) = (GetRValue(pix) + GetGValue(pix) + GetBValue(pix))/3;
	}

		HDC hDCCleanPadDC = CreateCompatibleDC(hDC);
		HBITMAP hBmpCleanPad = CreateCompatibleBitmap(hDC, 93, 223);
		SelectObject(hDCCleanPadDC, hBmpCleanPad);
		BitBlt(hDCCleanPadDC, 0, 0, 93, 223, hDC, top_x + 295, top_y + 202, SRCCOPY);
		WriteBMPPatternFile(hBmpCleanPad, L"C:\\minate.bmp", hDCCleanPadDC);
		DeleteObject(hBmpCleanPad);
		DeleteDC(hDCCleanPadDC);

		g_ConfirmPt.x = top_x + 97;
		g_ConfirmPt.y = top_y + 290;
		g_CancelPt.x = g_ConfirmPt.x + 120;
		g_CancelPt.y = g_ConfirmPt.y;
		POINT pt;
		pt.x = top_x + 90;
		pt.y = top_y + 250;
		g_InputPt.x = pt.x;
		g_InputPt.y = pt.y;
#ifdef _DEBUGPRINT
		PrintLogA("미네트 이미지탐색 성공");
		PrintLogA("미네트 이미지 위치 x: %x, y: %x", top_x + 68, top_y + 182);
#endif
		//g_infoChar.nState = AS_CLICK_CLEANPAD;
		g_infoChar.nState = 0;
		g_nRestTime = 0;	
	
		
		//memcpy(&g_ShareInfo.ptMousePos, &pt, sizeof(POINT));
		//SendCommand(CMD_SET_FOCUS);					


	return TRUE;
}
BOOL GetSkillImage(HDC hDC)
{
	int u, v;
	BOOL bFind = FALSE;

	RECT certiPt;
	certiPt.left = certiPt.right = certiPt.top = certiPt.bottom = 2000;
	BOOL bExistPrev = 0;
	BOOL bFont = 0;

	int top_x = 0, top_y = 0;
	PBYTE pGrayImgBuf = NULL, pEdgeImgBuf = NULL;
	int nW = 800, nH = 600; 
	pGrayImgBuf = (PBYTE) malloc(nW*nH); ZeroMemory(pGrayImgBuf, nW*nH);
	pEdgeImgBuf = (PBYTE) malloc(nW*nH); ZeroMemory(pEdgeImgBuf, nW*nH);

	/*for(i = 0; i < nH; i ++) for(j = 0; j < nW; j ++)
	{
		COLORREF pix = GetPixel(hDC, j, i);
		*(pGrayImgBuf+i*nW+j) = (GetRValue(pix) + GetGValue(pix) + GetBValue(pix))/3;
	}*/

	/*
	POINT			g_ptSkillMinate = {328, 223}; //{318, 308};
	int				g_nMinateStep = 0;
	POINT			g_ptSkillList = {68, 150};
	int				g_nSkillListGap = 42;
	SIZE			g_sizeSkillList = {80, 12};
	*/
	HDC hDCCleanPadDC = CreateCompatibleDC(hDC);
	HBITMAP hBmpCleanPad = CreateCompatibleBitmap(hDC, g_sizeSkillList.cx, g_sizeSkillList.cy);

	SelectObject(hDCCleanPadDC, hBmpCleanPad);
	
	top_x = g_ptSkillList.x ;
	WCHAR szFileName[MAX_PATH];
	char szLogFileName[MAX_PATH];
	for(int k = 0; k < 8; k ++)
	{
		top_y = g_ptSkillList.y + g_nSkillListGap * k ;

		BitBlt(hDCCleanPadDC, 0, 0, g_sizeSkillList.cx, g_sizeSkillList.cy, hDC, top_x, top_y, SRCCOPY);
		
		swprintf(szFileName, L"c:\\SkillImage-%d.bmp", k);
		sprintf(szLogFileName, "c:\\SkillImage-%d.bmp", k);
#ifdef _DEBUGPRINT
		PrintLog(szFileName) ;
#endif
		WriteLog("\nSkillImage - %s", szLogFileName);
		//{ pepsi_marius. 111102
		for(int i = 0; i < g_sizeSkillList.cx; i ++) 
		{
			WriteLogSkillPattern("{");
			for(int j = 0; j < g_sizeSkillList.cy; j ++)
			{
				COLORREF pix = GetPixel(hDCCleanPadDC, i, j);
				BYTE bR = GetRValue(pix);
				BYTE bG = GetGValue(pix);
				BYTE bB = GetBValue(pix);
				if(j == g_sizeSkillList.cy - 1)
				{
					WriteLogSkillPattern("RGB(%d, %d, %d)",bR, bG, bB);
				}
				else
				{
					WriteLogSkillPattern("RGB(%d, %d, %d),",bR, bG, bB);
				}
				
			}
			WriteLogSkillPattern("},\n");
		}	
		//}

		//WriteBMPPatternFile(hBmpCleanPad, L"C:\\minate.bmp", hDCCleanPadDC);
		WriteBMPPatternFile(hBmpCleanPad, szFileName, hDCCleanPadDC);
	}

	DeleteObject(hBmpCleanPad);
	DeleteDC(hDCCleanPadDC);

	g_ConfirmPt.x = top_x + 97;
	g_ConfirmPt.y = top_y + 290;
	g_CancelPt.x = g_ConfirmPt.x + 120;
	g_CancelPt.y = g_ConfirmPt.y;
	POINT pt;
	pt.x = top_x + 90;
	pt.y = top_y + 250;
	g_InputPt.x = pt.x;
	g_InputPt.y = pt.y;
#ifdef _DEBUGPRINT
	PrintLogA("미네트 이미지탐색 성공");
	PrintLogA("미네트 이미지 위치 x: %x, y: %x", top_x + 68, top_y + 182);
#endif
	//g_infoChar.nState = AS_CLICK_CLEANPAD;
	//g_infoChar.nState = 0;
	g_nRestTime = 0;	

	//memcpy(&g_ShareInfo.ptMousePos, &pt, sizeof(POINT));
	//SendCommand(CMD_SET_FOCUS);					
	return TRUE;
}

BOOL FindSkillList(HDC hDC, int nListPattern, POINT &ptSkill)
{
	int u, v;
	BOOL bFind = TRUE ;

	/*for(i = 0; i < nH; i ++) for(j = 0; j < nW; j ++)
	{
		COLORREF pix = GetPixel(hDC, j, i);
		*(pGrayImgBuf+i*nW+j) = (GetRValue(pix) + GetGValue(pix) + GetBValue(pix))/3;
	}*/
	/*
	POINT			g_ptSkillMinate = {328, 223}; //{318, 308};
	int				g_nMinateStep = 0;
	POINT			g_ptSkillList = {68, 150};
	int				g_nSkillListGap = 42;
	SIZE			g_sizeSkillList = {80, 12};
	*/
	HDC hDCCleanPadDC = CreateCompatibleDC(hDC);
	HBITMAP hBmpCleanPad = CreateCompatibleBitmap(hDC, g_sizeSkillList.cx, g_sizeSkillList.cy);

	SelectObject(hDCCleanPadDC, hBmpCleanPad);
	
	int top_x = 0 ;
	int top_y = 0 ;
	top_x = g_ptSkillList.x ;
	
	WCHAR szFileName[MAX_PATH];
	char szLogFileName[MAX_PATH];
	SkillPattern *pSkillPat ;
	for(int k = 0; k < 8; k ++)
	{
		bFind = TRUE ;
		//int nAbsCount = 0 ; //pepsi_marius. 111104
		int n0xFFCount = 0 ; //pepsi_marius. 111104

		top_y = g_ptSkillList.y + g_nSkillListGap * k ;

		BitBlt(hDCCleanPadDC, 0, 0, g_sizeSkillList.cx, g_sizeSkillList.cy, hDC, top_x, top_y, SRCCOPY);
		
		switch (nListPattern)
		{
		case SKILL_YEOMHUA:
		{
			//{ pepsi_marius. 111102
			for(int i = 0; i < g_sizeSkillList.cx; i ++) 
			{
				for(int j = 0; j < g_sizeSkillList.cy; j ++)
				{
					COLORREF pix = GetPixel(hDCCleanPadDC, i, j);
					BYTE bR = GetRValue(pix);
					BYTE bG = GetGValue(pix);
					BYTE bB = GetBValue(pix);

#ifdef _DEBUGPRINT
					//PrintLogA("bR  = %d, bG  = %d, bB  = %d", bR, bG, bB);
#endif
					COLORREF pixPattern = g_skillYeomHuaPattern[i][j];
					BYTE bR1 = GetRValue(pixPattern);
					BYTE bG1 = GetGValue(pixPattern);
					BYTE bB1 = GetBValue(pixPattern);
					
					if(bR1 == 0xFF && bG1 == 0xFF && bB1 ==  0xFF)
					{
						//if(abs(bR - bR1) < 11 && abs(bG - bG1) < 11 && abs(bB - bB1) < 11)
						//{
						//	nAbsCount ++ ;
						//}
						if(bR1 == bR && bG1 == bG && bB1 == bB)
						{
							n0xFFCount ++ ;
						}
					}
					
#ifdef _DEBUGPRINT
					/*
					PrintLogA("bR1 = %d, bG1 = %d, bB1 = %d", bR1, bG1, bB1);
					PrintLogA(" R  = %d,  G  = %d,  B  = %d", abs(bR1 - bR), abs(bG1 - bG), abs(bB1 - bB));
					*/
#endif

					if(bR != bR1 || bG != bG1 || bB != bB1)
					{
						bFind = FALSE ;
					}
				}
			}	
			if(bFind == TRUE)
			{
#ifdef _DEBUGPRINT
				PrintLogA("***** Find Skill Area - %d", k);
#endif
				ptSkill.x = top_x ;
				ptSkill.y = top_y + g_sizeSkillList.cy ;
				return bFind ;
			}


			//}
			break ;
		}
		case SKILL_CANCEL_YEOMHUA:		//캔슬 염화참 
			{
			//{ pepsi_marius. 111102
			for(int i = 0; i < g_sizeSkillList.cx; i ++)   //80, 12
			{
				for(int j = 0; j < g_sizeSkillList.cy; j ++)
				{
					COLORREF pix = GetPixel(hDCCleanPadDC, i, j);
					BYTE bR = GetRValue(pix);
					BYTE bG = GetGValue(pix);
					BYTE bB = GetBValue(pix);

					COLORREF pixPattern = g_skillCancelYeomHuaPattern[i][j];
					BYTE bR1 = GetRValue(pixPattern);
					BYTE bG1 = GetGValue(pixPattern);
					BYTE bB1 = GetBValue(pixPattern);
					
					if(bR1 == 0xFF && bG1 == 0xFF && bB1 == 0xFF)
					{
				
						if(bR1 == bR && bG1 == bG && bB1 == bB)
						{
							n0xFFCount ++ ;
						}
					}
					
					if(bR != bR1 || bG != bG1 || bB != bB1)
					{
						bFind = FALSE ;
					}
				}
			}	
			if(bFind == TRUE)
			{
#ifdef _DEBUGPRINT
				PrintLogA("***** Find Skill Area - %d", k);
#endif
				ptSkill.x = top_x ;
				ptSkill.y = top_y + g_sizeSkillList.cy ;
				return bFind ;
			}


			//}
			break ;
			}
		case SKILL_SHININGCUT:			//샤이닝컷
			{
			//{ pepsi_marius. 111102
			for(int i = 0; i < g_sizeSkillList.cx; i ++) 
			{
				for(int j = 0; j < g_sizeSkillList.cy; j ++)
				{
					COLORREF pix = GetPixel(hDCCleanPadDC, i, j);
					BYTE bR = GetRValue(pix);
					BYTE bG = GetGValue(pix);
					BYTE bB = GetBValue(pix);

					COLORREF pixPattern = g_skillShiningCut[i][j];
					BYTE bR1 = GetRValue(pixPattern);
					BYTE bG1 = GetGValue(pixPattern);
					BYTE bB1 = GetBValue(pixPattern);
					
					if(bR1 == 0xFF && bG1 == 0xFF && bB1 ==  0xFF)
					{
						if(bR1 == bR && bG1 == bG && bB1 == bB)
						{
							n0xFFCount ++ ;
						}
					}
				}
			}	
			//}
			}
			break ;
		case SKILL_MULI_CRITICAL:		//물리크리티컬(패시브)  
			{
				//{ pepsi_marius. 111102
				for(int i = 0; i < g_sizeSkillList.cx; i ++) 
				{
					for(int j = 0; j < g_sizeSkillList.cy; j ++)
					{
						COLORREF pix = GetPixel(hDCCleanPadDC, i, j);
						BYTE bR = GetRValue(pix);
						BYTE bG = GetGValue(pix);
						BYTE bB = GetBValue(pix);

						COLORREF pixPattern = g_skillMulCritical[i][j];
						BYTE bR1 = GetRValue(pixPattern);
						BYTE bG1 = GetGValue(pixPattern);
						BYTE bB1 = GetBValue(pixPattern);

						if(bR1 == 0xFF && bG1 == 0xFF && bB1 ==  0xFF)
						{
							if(bR1 == bR && bG1 == bG && bB1 == bB)
							{
								n0xFFCount ++ ;
							}
						}
					}
				}	
				//}
			}
			break ;
		case SKILL_DANGEOM_MASTERY:		//단검 마스터리(패시브)
			{
				//{ pepsi_marius. 111102
				for(int i = 0; i < g_sizeSkillList.cx; i ++) 
				{
					for(int j = 0; j < g_sizeSkillList.cy; j ++)
					{
						COLORREF pix = GetPixel(hDCCleanPadDC, i, j);
						BYTE bR = GetRValue(pix);
						BYTE bG = GetGValue(pix);
						BYTE bB = GetBValue(pix);

						COLORREF pixPattern = g_skillDanGeomMastery[i][j];
						BYTE bR1 = GetRValue(pixPattern);
						BYTE bG1 = GetGValue(pixPattern);
						BYTE bB1 = GetBValue(pixPattern);

						if(bR1 == 0xFF && bG1 == 0xFF && bB1 ==  0xFF)
						{
							if(bR1 == bR && bG1 == bG && bB1 == bB)
							{
								n0xFFCount ++ ;
							}
						}
					}
				}	
				//}
			}
			break ;
		case SKILL_HIT_AND:				//히트 앤드
			{
				//{ pepsi_marius. 111102
				for(int i = 0; i < g_sizeSkillList.cx; i ++) 
				{
					for(int j = 0; j < g_sizeSkillList.cy; j ++)
					{
						COLORREF pix = GetPixel(hDCCleanPadDC, i, j);
						BYTE bR = GetRValue(pix);
						BYTE bG = GetGValue(pix);
						BYTE bB = GetBValue(pix);

						COLORREF pixPattern = g_skillHitAnd[i][j];
						BYTE bR1 = GetRValue(pixPattern);
						BYTE bG1 = GetGValue(pixPattern);
						BYTE bB1 = GetBValue(pixPattern);

						if(bR1 == 0xFF && bG1 == 0xFF && bB1 ==  0xFF)
						{
							if(bR1 == bR && bG1 == bG && bB1 == bB)
							{
								n0xFFCount ++ ;
							}
						}
					}
				}	
				//}
			}
			break ;
		case SKILL_BANDDI_CUT:			//벤디트컷
			{
				//{ pepsi_marius. 111102
				for(int i = 0; i < g_sizeSkillList.cx; i ++) 
				{
					for(int j = 0; j < g_sizeSkillList.cy; j ++)
					{
						COLORREF pix = GetPixel(hDCCleanPadDC, i, j);
						BYTE bR = GetRValue(pix);
						BYTE bG = GetGValue(pix);
						BYTE bB = GetBValue(pix);

						COLORREF pixPattern = g_skillBandiCut[i][j];
						BYTE bR1 = GetRValue(pixPattern);
						BYTE bG1 = GetGValue(pixPattern);
						BYTE bB1 = GetBValue(pixPattern);

						if(bR1 == 0xFF && bG1 == 0xFF && bB1 ==  0xFF)
						{
							if(bR1 == bR && bG1 == bG && bB1 == bB)
							{
								n0xFFCount ++ ;
							}
						}
					}
				}	
				//}
			}
			break ;
		case SKILL_CHAINSO:				//체인소		
			{
				//{ pepsi_marius. 111102
				for(int i = 0; i < g_sizeSkillList.cx; i ++) 
				{
					for(int j = 0; j < g_sizeSkillList.cy; j ++)
					{
						COLORREF pix = GetPixel(hDCCleanPadDC, i, j);
						BYTE bR = GetRValue(pix);
						BYTE bG = GetGValue(pix);
						BYTE bB = GetBValue(pix);

						COLORREF pixPattern = g_skillChainSo[i][j];
						BYTE bR1 = GetRValue(pixPattern);
						BYTE bG1 = GetGValue(pixPattern);
						BYTE bB1 = GetBValue(pixPattern);

						if(bR1 == 0xFF && bG1 == 0xFF && bB1 ==  0xFF)
						{
							if(bR1 == bR && bG1 == bG && bB1 == bB)
							{
								n0xFFCount ++ ;
							}
						}
					}
				}	
				//}
			}
			break ;
		case SKILL_CANCEL_CHAINSO:		//캔슬 체인소 
			{
				//{ pepsi_marius. 111102
				for(int i = 0; i < g_sizeSkillList.cx; i ++) 
				{
					for(int j = 0; j < g_sizeSkillList.cy; j ++)
					{
						COLORREF pix = GetPixel(hDCCleanPadDC, i, j);
						BYTE bR = GetRValue(pix);
						BYTE bG = GetGValue(pix);
						BYTE bB = GetBValue(pix);

						COLORREF pixPattern = g_skillCancelChainSo[i][j];
						BYTE bR1 = GetRValue(pixPattern);
						BYTE bG1 = GetGValue(pixPattern);
						BYTE bB1 = GetBValue(pixPattern);

						if(bR1 == 0xFF && bG1 == 0xFF && bB1 ==  0xFF)
						{
							if(bR1 == bR && bG1 == bG && bB1 == bB)
							{
								n0xFFCount ++ ;
							}
						}
					}
				}	
				//}
			}
			break ;
		case SKILL_HERIKAIN:				//허리케인
			{
				//{ pepsi_marius. 111102
				for(int i = 0; i < g_sizeSkillList.cx; i ++) 
				{
					for(int j = 0; j < g_sizeSkillList.cy; j ++)
					{
						COLORREF pix = GetPixel(hDCCleanPadDC, i, j);
						BYTE bR = GetRValue(pix);
						BYTE bG = GetGValue(pix);
						BYTE bB = GetBValue(pix);

						COLORREF pixPattern = g_skillHeriKain[i][j];
						BYTE bR1 = GetRValue(pixPattern);
						BYTE bG1 = GetGValue(pixPattern);
						BYTE bB1 = GetBValue(pixPattern);

						if(bR1 == 0xFF && bG1 == 0xFF && bB1 ==  0xFF)
						{
							if(bR1 == bR && bG1 == bG && bB1 == bB)
							{
								n0xFFCount ++ ;
							}
						}
					}
				}	
				//}
			}
			break ;
		case SKILL_CANCEL_HERIKAIN:		//캔슬 허리케인
			{
				//{ pepsi_marius. 111102
				for(int i = 0; i < g_sizeSkillList.cx; i ++) 
				{
					for(int j = 0; j < g_sizeSkillList.cy; j ++)
					{
						COLORREF pix = GetPixel(hDCCleanPadDC, i, j);
						BYTE bR = GetRValue(pix);
						BYTE bG = GetGValue(pix);
						BYTE bB = GetBValue(pix);

						COLORREF pixPattern = g_skillCancelHeriKain[i][j];
						BYTE bR1 = GetRValue(pixPattern);
						BYTE bG1 = GetGValue(pixPattern);
						BYTE bB1 = GetBValue(pixPattern);

						if(bR1 == 0xFF && bG1 == 0xFF && bB1 ==  0xFF)
						{
							if(bR1 == bR && bG1 == bG && bB1 == bB)
							{
								n0xFFCount ++ ;
							}
						}
					}
				}	
				//}
			}
			break ;
		case SKILL_DOUBLE_PIERCE:		//더블 피어스
			{
				//{ pepsi_marius. 111102
				for(int i = 0; i < g_sizeSkillList.cx; i ++) 
				{
					for(int j = 0; j < g_sizeSkillList.cy; j ++)
					{
						COLORREF pix = GetPixel(hDCCleanPadDC, i, j);
						BYTE bR = GetRValue(pix);
						BYTE bG = GetGValue(pix);
						BYTE bB = GetBValue(pix);

						COLORREF pixPattern = g_skillDoublePierce[i][j];
						BYTE bR1 = GetRValue(pixPattern);
						BYTE bG1 = GetGValue(pixPattern);
						BYTE bB1 = GetBValue(pixPattern);

						if(bR1 == 0xFF && bG1 == 0xFF && bB1 ==  0xFF)
						{
							if(bR1 == bR && bG1 == bG && bB1 == bB)
							{
								n0xFFCount ++ ;
							}
						}
					}
				}	
				//}
			}
			break ;
		case SKILL_CANCEL_DOUBLE_PIERCE:	//캔슬 더블 피어스
			{
				//{ pepsi_marius. 111102
				for(int i = 0; i < g_sizeSkillList.cx; i ++) 
				{
					for(int j = 0; j < g_sizeSkillList.cy; j ++)
					{
						COLORREF pix = GetPixel(hDCCleanPadDC, i, j);
						BYTE bR = GetRValue(pix);
						BYTE bG = GetGValue(pix);
						BYTE bB = GetBValue(pix);

						COLORREF pixPattern = g_skillCancelDoublePierce[i][j];
						BYTE bR1 = GetRValue(pixPattern);
						BYTE bG1 = GetGValue(pixPattern);
						BYTE bB1 = GetBValue(pixPattern);

						if(bR1 == 0xFF && bG1 == 0xFF && bB1 ==  0xFF)
						{
							if(bR1 == bR && bG1 == bG && bB1 == bB)
							{
								n0xFFCount ++ ;
							}
						}
					}
				}	
				//}
			}
			break ;
		}


#ifdef _DEBUGPRINT

		PrintLogA("***** %d 번째 패턴 ", k);
		//PrintLogA("***** 절대유사개수 - %d", nAbsCount );
		PrintLogA("***** 흰색유사개수 - %d", n0xFFCount );
#endif
		if(n0xFFCount  > g_nSkillThreshold[nListPattern])
		{
			ptSkill.x = top_x ;
			ptSkill.y = top_y + g_sizeSkillList.cy ;
			return TRUE ;
		}
		else
		{
			bFind = FALSE ;
		}


	}

	DeleteObject(hBmpCleanPad);
	DeleteDC(hDCCleanPadDC);

	//g_infoChar.nState = 0;
	g_nRestTime = 0;	

	return bFind;
}

