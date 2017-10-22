#ifndef _PASSWORD_H_
#define _PASSWORD_H_

DWORD sub_401040(PWSTR str, DWORD* dwData, HWND hWnd);
HWND sub_4010D0(HWND hWndParent, PPARAM param);
LRESULT CALLBACK sub_401210(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void sub_4015C0(HWND hWnd);
DWORD sub_401660(PWSTR str);

#endif