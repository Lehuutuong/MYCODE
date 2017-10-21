#include "stdafx.h"
#include "InjectGuard.h"


typedef LONG NTSTATUS;
#define FASTCALL __fastcall
#define NT_SUCCESS(Status) (((NTSTATUS)Status) >= 0)
#ifndef NTAPI
#define NTAPI WINAPI
#endif
#ifndef UNICODE_STRING
typedef struct _LSA_UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR Buffer;
} LSA_UNICODE_STRING, *PLSA_UNICODE_STRING, UNICODE_STRING, *PUNICODE_STRING;
#endif

typedef HMODULE (WINAPI *LOADLIBRARYA)(
									   LPCSTR lpFileName
									   );
typedef HMODULE (WINAPI *LOADLIBRARYW)(
									   LPCWSTR lpFileName
									   );
typedef HMODULE (WINAPI *LOADLIBRARYEXA)(
	LPCSTR lpFileName,
	HANDLE hFile,
	DWORD dwFlags
	);
typedef HMODULE (WINAPI *LOADLIBRARYEXW)(
	LPCWSTR lpFileName,
	HANDLE hFile,
	DWORD dwFlags
	);
typedef NTSTATUS (NTAPI *LDRLOADDLL)(PWCHAR, ULONG, PUNICODE_STRING, PHANDLE);
#define CALCULATE_JUMP_OFFSET(_BaseOffset, _RelativeOffset) ( \
	(LONG)(_BaseOffset) + (LONG)(_RelativeOffset) + 5 \
	)
#define MAKE_JUMP_OFFSET(_BaseOffset, _TargetOffset) ( \
	(LONG)(_TargetOffset) - (LONG)(_BaseOffset) - 5 \
	)

LOADLIBRARYA _LoadLibraryA, _tLoadLibraryA;
LOADLIBRARYW _LoadLibraryW, _tLoadLibraryW;
LOADLIBRARYEXA _LoadLibraryExA, _tLoadLibraryExA;
LOADLIBRARYEXW _LoadLibraryExW, _tLoadLibraryExW;
LDRLOADDLL _LdrLoadDll, _tLdrLoadDll;
DWORD dwUser32Start, dwUser32End;


BOOL WINAPI CheckCallerAddress(IN PVOID Caller)
{
	// �� �����忡�� ȣ��Ǿ����� Ȯ��
	if( Caller == NULL )
	{
		OutputDebugString(L"New Thread DLL �������� �����Ǿ����ϴ�.");
		return FALSE;
	}
	// Ŀ�� ��� �޸�?
	else if( (DWORD)Caller >= (DWORD)0x80000000L )
	{
		return FALSE;
	}
	// �������� �޸�?
	else if(IsBadCodePtr( (FARPROC)Caller ))
	{
		return FALSE;
	}
	else
	{
		MEMORY_BASIC_INFORMATION mbi;
		VirtualQuery(Caller, &mbi, sizeof(MEMORY_BASIC_INFORMATION));

		// Private Memory���� ȣ��Ǿ���
		if(mbi.Type == MEM_PRIVATE)
		{
			OutputDebugString(L"Private Memory DLL �������� �����Ǿ����ϴ�.");
			return FALSE;
		}
		else
		{
			// Ȥ�� Caller�� BaseThreadStartThunk ���� �����߳�?
			LPBYTE pbCaller = (LPBYTE) Caller;
			if(*pbCaller == (BYTE)0x50 && 
				/*pbCaller[1] == (BYTE)0xE8 && */
				CALCULATE_JUMP_OFFSET(pbCaller + 1, *(PLONG)(pbCaller + 2)) == (LONG)ExitThread)
			{
				OutputDebugString(L"CreateRemoteThread DLL �������� �����Ǿ����ϴ�.");
				return FALSE;
			}

			// Ȥ�� Caller�� ClientLoadLibrary ���� �����߳�?
			if(((DWORD)pbCaller >= dwUser32Start && (DWORD)pbCaller < dwUser32End) ||
				*(pbCaller-7) == (BYTE)0x1C)
			{
				// MessageBox( NULL, "SetWindowsHookEx DLL �������� �����Ǿ����ϴ�.", "Error", MB_OK | MB_ICONEXCLAMATION );
				return FALSE;
			}
		}
	}

	return TRUE;
}

__declspec(naked) HMODULE WINAPI _nLoadLibraryA(
	LPCSTR lpFileName
	)
{
	__asm
	{
		XOR EAX, EAX
			JNZ $+30000

			PUSHAD					  // �������� �� ����
			PUSH DWORD PTR [ESP+0x20] // ȣ���� �ּ� push
		CALL CheckCallerAddress   // ȣ���� ����
			TEST EAX, EAX			  // ��ȯ���� FALSE �ΰ�?
			JZ _NotVerifiedCaller	  // �߸��� ȣ��

			// �ùٸ� ȣ��
			POPAD
			JMP _tLoadLibraryA

_NotVerifiedCaller:
		POPAD
			XOR EAX, EAX			  // EAX = 0
			RETN 0x4				  // ����
	}
}

__declspec(naked) HMODULE WINAPI _nLoadLibraryW(
	LPCWSTR lpFileName
	)
{
	__asm
	{
		XOR EAX, EAX
			JNZ $+30000

			PUSHAD					  // �������� �� ����
			PUSH DWORD PTR [ESP+0x20] // ȣ���� �ּ� push
		CALL CheckCallerAddress   // ȣ���� ����
			TEST EAX, EAX			  // ��ȯ���� FALSE �ΰ�?
			JZ _NotVerifiedCaller	  // �߸��� ȣ��

			// �ùٸ� ȣ��
			POPAD
			JMP _tLoadLibraryW

_NotVerifiedCaller:
		POPAD
			XOR EAX, EAX			  // EAX = 0
			RETN 0x4				  // ����
	}
}

__declspec(naked) HMODULE WINAPI _nLoadLibraryExA(
	LPCSTR lpFileName,
	HANDLE hFile,
	DWORD dwFlags
	)
{
	__asm
	{
		XOR EAX, EAX
			JNZ $+30000

			PUSHAD					  // �������� �� ����
			PUSH DWORD PTR [ESP+0x20] // ȣ���� �ּ� push
		CALL CheckCallerAddress   // ȣ���� ����
			TEST EAX, EAX			  // ��ȯ���� FALSE �ΰ�?
			JZ _NotVerifiedCaller	  // �߸��� ȣ��

			// �ùٸ� ȣ��
			POPAD
			JMP _tLoadLibraryExA

_NotVerifiedCaller:
		POPAD
			XOR EAX, EAX			  // EAX = 0
			RETN 0xC				  // ����
	}
}

__declspec(naked) HMODULE WINAPI _nLoadLibraryExW(
	LPCWSTR lpFileName,
	HANDLE hFile,
	DWORD dwFlags
	)
{
	__asm
	{
		XOR EAX, EAX
			JNZ $+30000

			PUSHAD					  // �������� �� ����
			PUSH DWORD PTR [ESP+0x20] // ȣ���� �ּ� push
		CALL CheckCallerAddress   // ȣ���� ����
			TEST EAX, EAX			  // ��ȯ���� FALSE �ΰ�?
			JZ _NotVerifiedCaller	  // �߸��� ȣ��

			// �ùٸ� ȣ��
			POPAD
			JMP _tLoadLibraryExW

_NotVerifiedCaller:
		POPAD
			XOR EAX, EAX			  // EAX = 0
			RETN 0xC				  // ����
	}
}

__declspec(naked)
NTSTATUS
NTAPI
_nLdrLoadDll(
			 IN PWCHAR               PathToFile OPTIONAL,
			 IN ULONG                Flags OPTIONAL,
			 IN PUNICODE_STRING      ModuleFileName,
			 OUT PHANDLE             ModuleHandle )
{
	__asm
	{
		XOR EAX, EAX
			JNZ $+30000

			PUSHAD					  // �������� �� ����
			PUSH DWORD PTR [ESP+0x20] // ȣ���� �ּ� push
		CALL CheckCallerAddress   // ȣ���� ����
			TEST EAX, EAX			  // ��ȯ���� FALSE �ΰ�?
			JZ _NotVerifiedCaller	  // �߸��� ȣ��

			// �ùٸ� ȣ��
			POPAD
			JMP _tLdrLoadDll

_NotVerifiedCaller:
		POPAD
			XOR EAX, EAX			  // EAX = 0
			RETN 0x10				  // ����
	}
}

VOID FASTCALL HookApi( LPBYTE TargetFunction, LPBYTE Trampoline, LPVOID DetouredFunction, DWORD MinimumSize )
{
	//
	// Don't Use Disassembly Library ;-)
	// But, Only Supports 2 byte & 5 byte for XP =_=;
	//

	DWORD OldProtect;
	VirtualProtect( TargetFunction-5, 5+MinimumSize, PAGE_EXECUTE_READWRITE, &OldProtect );

	if(MinimumSize == 2)
	{
		LPBYTE JmpOffset = TargetFunction - 5;
		*JmpOffset = (BYTE)0xE9; // JMP LONG
		*(PLONG)(JmpOffset + 1) = MAKE_JUMP_OFFSET(JmpOffset, DetouredFunction);
		RtlMoveMemory(Trampoline, TargetFunction, MinimumSize);

		JmpOffset = Trampoline + MinimumSize;
		*JmpOffset = (BYTE)0xE9;
		*(PLONG)(JmpOffset + 1) = MAKE_JUMP_OFFSET(JmpOffset, (LPBYTE)TargetFunction + MinimumSize);

		*TargetFunction = (BYTE)0xEB; // JMP SHORT
		TargetFunction[1] = (BYTE)0xF9;
	}
	else if(MinimumSize == 5)
	{
		RtlMoveMemory(Trampoline, TargetFunction, MinimumSize);

		LPBYTE JmpOffset = Trampoline + MinimumSize;
		*JmpOffset = (BYTE)0xE9;
		*(PLONG)(JmpOffset + 1) = MAKE_JUMP_OFFSET(JmpOffset, (LPBYTE)TargetFunction + MinimumSize);

		JmpOffset = TargetFunction;
		*JmpOffset = (BYTE)0xE9; // JMP LONG
		*(PLONG)(JmpOffset + 1) = MAKE_JUMP_OFFSET(JmpOffset, DetouredFunction);
	}

	VirtualProtect( TargetFunction-5, 5+MinimumSize, OldProtect, &OldProtect );
}

VOID FASTCALL UnHookApi( LPBYTE TargetFunction, LPBYTE Trampoline, DWORD MinimumSize )
{
	DWORD OldProtect;
	VirtualProtect( TargetFunction, MinimumSize, PAGE_EXECUTE_READWRITE, &OldProtect );
	RtlMoveMemory( TargetFunction, Trampoline, MinimumSize );
	VirtualProtect( TargetFunction, MinimumSize, OldProtect, &OldProtect );
}

BOOL InitForAntiInject()
{
	HMODULE hKernel32 = GetModuleHandle(L"kernel32");
	HMODULE hNtDLL = GetModuleHandle(L"ntdll");
	HMODULE hUser32 = GetModuleHandle(L"user32");
	if(!hKernel32 || !hNtDLL) return FALSE;
	if(!hUser32)
	{
		hUser32 = LoadLibrary( TEXT("user32") );
		if(!hUser32) return FALSE;
	}
	if(!(_LoadLibraryA = (LOADLIBRARYA) GetProcAddress(hKernel32, "LoadLibraryA"))) return FALSE;
	if(!(_LoadLibraryW = (LOADLIBRARYW) GetProcAddress(hKernel32, "LoadLibraryW"))) return FALSE;
	if(!(_LoadLibraryExA = (LOADLIBRARYEXA) GetProcAddress(hKernel32, "LoadLibraryExA"))) return FALSE;
	if(!(_LoadLibraryExW = (LOADLIBRARYEXW) GetProcAddress(hKernel32, "LoadLibraryExW"))) return FALSE;
	if(!(_LdrLoadDll = (LDRLOADDLL) GetProcAddress(hNtDLL, "LdrLoadDll"))) return FALSE;

	if(!(_tLoadLibraryA = (LOADLIBRARYA) VirtualAlloc(NULL, 10, MEM_COMMIT, PAGE_EXECUTE_READWRITE))) return FALSE;
	if(!(_tLoadLibraryW = (LOADLIBRARYW) VirtualAlloc(NULL, 10, MEM_COMMIT, PAGE_EXECUTE_READWRITE))) return FALSE;
	if(!(_tLoadLibraryExA = (LOADLIBRARYEXA) VirtualAlloc(NULL, 10, MEM_COMMIT, PAGE_EXECUTE_READWRITE))) return FALSE;
	if(!(_tLoadLibraryExW = (LOADLIBRARYEXW) VirtualAlloc(NULL, 10, MEM_COMMIT, PAGE_EXECUTE_READWRITE))) return FALSE;
	if(!(_tLdrLoadDll = (LDRLOADDLL) VirtualAlloc(NULL, 10, MEM_COMMIT, PAGE_EXECUTE_READWRITE))) return FALSE;

	// USER32.DLL ���� ����
	dwUser32Start = (DWORD)hUser32;

	PIMAGE_DOS_HEADER idh = (PIMAGE_DOS_HEADER)hUser32;
	if(idh->e_magic != IMAGE_DOS_SIGNATURE) return FALSE; // IMAGE_DOS_SIGNATURE üũ
	PIMAGE_NT_HEADERS inh = (PIMAGE_NT_HEADERS)(dwUser32Start +	idh->e_lfanew);
	if(inh->Signature != IMAGE_NT_SIGNATURE) return FALSE; // IMAGE_NT_SIGNATURE üũ
	dwUser32End ^= (dwUser32Start ^ dwUser32End);
	dwUser32End += inh->OptionalHeader.SizeOfImage;

	HookApi( (LPBYTE)_LoadLibraryA, (LPBYTE)_tLoadLibraryA, (LPVOID)_nLoadLibraryA, 5 );
	HookApi( (LPBYTE)_LoadLibraryW, (LPBYTE)_tLoadLibraryW, (LPVOID)_nLoadLibraryW, 5 );
	HookApi( (LPBYTE)_LoadLibraryExA, (LPBYTE)_tLoadLibraryExA, (LPVOID)_nLoadLibraryExA, 5 );
	HookApi( (LPBYTE)_LoadLibraryExW, (LPBYTE)_tLoadLibraryExW, (LPVOID)_nLoadLibraryExW, 2 );
	HookApi( (LPBYTE)_LdrLoadDll, (LPBYTE)_tLdrLoadDll, (LPVOID)_nLdrLoadDll, 5 );
}

void FreeHookAPI()
{
	UnHookApi( (LPBYTE)_LoadLibraryA, (LPBYTE)_tLoadLibraryA, 5 );
	VirtualFree( _tLoadLibraryA, 0, MEM_RELEASE );

	UnHookApi( (LPBYTE)_LoadLibraryW, (LPBYTE)_tLoadLibraryW, 5 );
	VirtualFree( _tLoadLibraryW, 0, MEM_RELEASE );

	UnHookApi( (LPBYTE)_LoadLibraryExA, (LPBYTE)_tLoadLibraryExA, 5 );
	VirtualFree( _tLoadLibraryExA, 0, MEM_RELEASE );

	UnHookApi( (LPBYTE)_LoadLibraryExW, (LPBYTE)_tLoadLibraryExW, 2 );
	VirtualFree( _tLoadLibraryExW, 0, MEM_RELEASE );

	UnHookApi( (LPBYTE)_LdrLoadDll, (LPBYTE)_tLdrLoadDll, 5 );
	VirtualFree( _tLdrLoadDll, 0, MEM_RELEASE );
}