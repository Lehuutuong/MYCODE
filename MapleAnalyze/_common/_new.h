// _new.h 프로세스 후크 기술입니다.
#pragma once

void SetProjectName(IN char* p_cName, IN WCHAR* p_wName);

#ifndef MyOutputDebugStringA
//
// 디버그 정보 출력 함수.
//
void MyOutputDebugStringW(
						  IN	const WCHAR*	p_lpFormat, ...
						  );

void MyOutputDebugStringA(
						  IN	const CHAR*	p_lpFormat, ...
						  );
//
// 디버그 정보 출력 함수. 오유에 대한 정보를 추가로 출력한다.
//

void MyOutputDebugStringW_Err(
							  IN	const WCHAR*	p_lpFormat, ...
							  );

void MyOutputDebugStringA_Err(
							  IN	const CHAR*	p_lpFormat, ...
							  );

#endif

// 문자렬 로그를 파일에 남기기.
void MyOutputDebugString2FileW(IN const WCHAR* p_lpFormat, ...);

//------------------------------------------------------------------------------
//		덤프 파일에 대한 정보, 파일의 맨 뒤에 추가된다.
//------------------------------------------------------------------------------
typedef struct _ST_DUMP_INFO	// 64 바이트
{
	DWORD		selfCRC;		// 자체 자료의 crc
	SYSTEMTIME	DumpTime;		// 덤프한 시간
	DWORD		OrgCRC;			// 원본파일의 CRC 값.
	DWORD		MARK;			// 유효성 식별정보.
	BYTE		Reserved[0x24];	// 예약공간
}ST_DUMP_INFO, *PST_DUMP_INFO;

typedef struct _GUARD_DATA_SET	// 36 바이트
{
	BYTE		Reserved[0x1C];	// 예약공간
	DWORD		MonCRC;			// 몬파일의 CRC 값.
	BYTE		Req[0x10];		// 가드 요청 자료.
	BYTE		Resp[0x10];		// 가드 응답 자료.
}GUARD_DATA_SET, *PGUARD_DATA_SET;

#define DUMP_MARK		(0x706D7544) //Dump
//------------------------------------------------------------------------------
//		검토중의 함수들.
//------------------------------------------------------------------------------

// 프로세스를 기동시키며 지정한 시간동안 프로세스가 끝나기를 기다린다.
BOOL ExecuteProgramW( LPCWSTR FileName, LPCWSTR Params, OUT HANDLE* p_hProcess = NULL, DWORD wait_ms = 0, BOOL p_bHide = FALSE);
BOOL ExecuteProgramA( LPCSTR FileName, LPCSTR Params, OUT HANDLE* p_hProcess = NULL, DWORD wait_ms = 0, BOOL p_bHide = FALSE);

// 프로그램의 바이너리 폴더를 얻는다.
BOOL GetExePathW(OUT WCHAR *p_strExePath);
BOOL GetExePathA(OUT CHAR *p_strExePath);

// 지정 경로에 메모리 자료를 덤프하기.
BOOL DumpFileWithPath(IN WCHAR* p_full_path, IN void* data, int len);

// 프로세스 아이디를 알고 그 프로세스 기본모듈을 덤프하기
BOOL DumpFileByProcessID(DWORD ProcessID, IN WCHAR* p_DumpPath);

// 16바이트 자료 출력하기.
void DbgPrintGuard (void* p_data, char* comment);


// 프로세스를 실행시키기.
BOOL RunProcess(IN WCHAR *p_ExePath, IN WCHAR *p_Param, OPTIONAL OUT PROCESS_INFORMATION *p_pi);

// 파일의 CRC 얻기
DWORD __stdcall GetFileCrc(IN WCHAR* p_FilePath);

// 림시폴더에서 새로운 림시파일의 완전경로를 얻는다.
WCHAR* GetNewTempFilePath(OUT WCHAR* p_tmpFullPath, int p_nMaxLen, OPTIONAL IN WCHAR* p_subFolderName = NULL
						  , OPTIONAL IN WCHAR* p_subFileName = NULL, OPTIONAL IN WCHAR* p_baseFolderName = NULL);

// 파일로부터 특정위치 자료를 직접 읽어들이기.
BOOL ReadFileToMemory(WCHAR* p_path, OUT void* p_data, int p_len, int p_pos, BOOL p_bEnd);
// 파일의 특정위치에 자료를 직접 써넣기.
BOOL WriteFileFromMemory(WCHAR* p_path, IN void* p_data, int p_len, int p_pos, BOOL p_bEnd);

// 덤프 정보 구조체를 채우기
void SetDumpInfo(OUT PST_DUMP_INFO p_DumpInfo, DWORD p_OrgCRC);

// 덤프 정보 구조체의 CRC 를 설정얻기.
DWORD GetDumpInfoCRC(IN PST_DUMP_INFO p_DumpInfo);

// 덤프 정보 자료가 유효한가를 판정한다.
BOOL IsValidDumpInfo(IN PST_DUMP_INFO p_DumpInfo);

