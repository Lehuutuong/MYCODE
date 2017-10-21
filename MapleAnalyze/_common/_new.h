// _new.h ���μ��� ��ũ ����Դϴ�.
#pragma once

void SetProjectName(IN char* p_cName, IN WCHAR* p_wName);

#ifndef MyOutputDebugStringA
//
// ����� ���� ��� �Լ�.
//
void MyOutputDebugStringW(
						  IN	const WCHAR*	p_lpFormat, ...
						  );

void MyOutputDebugStringA(
						  IN	const CHAR*	p_lpFormat, ...
						  );
//
// ����� ���� ��� �Լ�. ������ ���� ������ �߰��� ����Ѵ�.
//

void MyOutputDebugStringW_Err(
							  IN	const WCHAR*	p_lpFormat, ...
							  );

void MyOutputDebugStringA_Err(
							  IN	const CHAR*	p_lpFormat, ...
							  );

#endif

// ���ڷ� �α׸� ���Ͽ� �����.
void MyOutputDebugString2FileW(IN const WCHAR* p_lpFormat, ...);

//------------------------------------------------------------------------------
//		���� ���Ͽ� ���� ����, ������ �� �ڿ� �߰��ȴ�.
//------------------------------------------------------------------------------
typedef struct _ST_DUMP_INFO	// 64 ����Ʈ
{
	DWORD		selfCRC;		// ��ü �ڷ��� crc
	SYSTEMTIME	DumpTime;		// ������ �ð�
	DWORD		OrgCRC;			// ���������� CRC ��.
	DWORD		MARK;			// ��ȿ�� �ĺ�����.
	BYTE		Reserved[0x24];	// �������
}ST_DUMP_INFO, *PST_DUMP_INFO;

typedef struct _GUARD_DATA_SET	// 36 ����Ʈ
{
	BYTE		Reserved[0x1C];	// �������
	DWORD		MonCRC;			// �������� CRC ��.
	BYTE		Req[0x10];		// ���� ��û �ڷ�.
	BYTE		Resp[0x10];		// ���� ���� �ڷ�.
}GUARD_DATA_SET, *PGUARD_DATA_SET;

#define DUMP_MARK		(0x706D7544) //Dump
//------------------------------------------------------------------------------
//		�������� �Լ���.
//------------------------------------------------------------------------------

// ���μ����� �⵿��Ű�� ������ �ð����� ���μ����� �����⸦ ��ٸ���.
BOOL ExecuteProgramW( LPCWSTR FileName, LPCWSTR Params, OUT HANDLE* p_hProcess = NULL, DWORD wait_ms = 0, BOOL p_bHide = FALSE);
BOOL ExecuteProgramA( LPCSTR FileName, LPCSTR Params, OUT HANDLE* p_hProcess = NULL, DWORD wait_ms = 0, BOOL p_bHide = FALSE);

// ���α׷��� ���̳ʸ� ������ ��´�.
BOOL GetExePathW(OUT WCHAR *p_strExePath);
BOOL GetExePathA(OUT CHAR *p_strExePath);

// ���� ��ο� �޸� �ڷḦ �����ϱ�.
BOOL DumpFileWithPath(IN WCHAR* p_full_path, IN void* data, int len);

// ���μ��� ���̵� �˰� �� ���μ��� �⺻����� �����ϱ�
BOOL DumpFileByProcessID(DWORD ProcessID, IN WCHAR* p_DumpPath);

// 16����Ʈ �ڷ� ����ϱ�.
void DbgPrintGuard (void* p_data, char* comment);


// ���μ����� �����Ű��.
BOOL RunProcess(IN WCHAR *p_ExePath, IN WCHAR *p_Param, OPTIONAL OUT PROCESS_INFORMATION *p_pi);

// ������ CRC ���
DWORD __stdcall GetFileCrc(IN WCHAR* p_FilePath);

// ������������ ���ο� ���������� ������θ� ��´�.
WCHAR* GetNewTempFilePath(OUT WCHAR* p_tmpFullPath, int p_nMaxLen, OPTIONAL IN WCHAR* p_subFolderName = NULL
						  , OPTIONAL IN WCHAR* p_subFileName = NULL, OPTIONAL IN WCHAR* p_baseFolderName = NULL);

// ���Ϸκ��� Ư����ġ �ڷḦ ���� �о���̱�.
BOOL ReadFileToMemory(WCHAR* p_path, OUT void* p_data, int p_len, int p_pos, BOOL p_bEnd);
// ������ Ư����ġ�� �ڷḦ ���� ��ֱ�.
BOOL WriteFileFromMemory(WCHAR* p_path, IN void* p_data, int p_len, int p_pos, BOOL p_bEnd);

// ���� ���� ����ü�� ä���
void SetDumpInfo(OUT PST_DUMP_INFO p_DumpInfo, DWORD p_OrgCRC);

// ���� ���� ����ü�� CRC �� �������.
DWORD GetDumpInfoCRC(IN PST_DUMP_INFO p_DumpInfo);

// ���� ���� �ڷᰡ ��ȿ�Ѱ��� �����Ѵ�.
BOOL IsValidDumpInfo(IN PST_DUMP_INFO p_DumpInfo);

