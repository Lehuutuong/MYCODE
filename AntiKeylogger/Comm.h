#ifndef _COMM_H_
#define _COMM_H_

#include "Constdef.h"

void sub_40FA20();
DWORD sub_410020();
BOOL sub_410100(DWORD* arg_0);
BOOL sub_410B10();
PSTRUCTC sub_4101E0(char* szPortName);
PSTRUCTD sub_4107C0(PVOID buffer, ULONG length);
NTSTATUS sub_4110E0(PSTRUCTD source, PSTRUCTD* target);
PSTRUCTC sub_40FEF0(PSTRUCTC structc);
HANDLE CreateThread_410850(FARPROC proc, DWORD dwSize, STRUCTC* structc);
void sub_410990(PSTRUCTC structc);
void sub_4111D0(PSTRUCTD arg_0, DWORD* arg_4);
NTSTATUS sub_410530(PSTRUCTC structc, PPORT_MESSAGE arg_4, PSTRUCTD* arg_8, PSTRUCTD structd);
void sub_40F860(DWORD arg_0, DWORD* arg_4, DWORD arg_8);
HANDLE LoadDriver();
BOOL SetupRegistry();

#endif // _COMM_H_