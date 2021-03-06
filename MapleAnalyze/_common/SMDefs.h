#pragma once


// ȸ�� ��Ī
#define MANUFACTURER			L"Brainzsquare"
// ��ǰ ��Ī
#define PRODUCT_NAMEA					"SecuDrive"
#define PRODUCT_NAMEW					L"SecuDrive"

#define SERVICE_NAME					"SecuDrive"
#define SERVICE_NAMEW					L"SecuDrive"
// ���� ����
#define VERSION_STRING                  "1.01"

// Version number to compare against driver
#define VERSION_NUM						0x0101

// Version number written to volume header during format,
// specifies the minimum program version required to mount the volume
#define VOL_REQ_PROG_VERSION			0x0100

// Volume header version
#define VOLUME_HEADER_VERSION			0x0002 

// Sector size of encrypted filesystem, which may differ from sector size of host filesystem/device (this 
// is fully supported since v4.3). To retain maximum portability and compatibility, it should always be 512.
#ifndef  SECTOR_SIZE
#define SECTOR_SIZE                     512
#endif
#define BYTES_PER_KB                    1024LL
#define BYTES_PER_MB                    1048576LL
#define BYTES_PER_GB                    1073741824LL
#define BYTES_PER_TB                    1099511627776LL
#define BYTES_PER_PB                    1125899906842624LL

/* GUI/driver errors */

#define ERR_SUCCESS                     0
#define ERR_OS_ERROR                    1
#define ERR_OUTOFMEMORY                 2
#define ERR_PASSWORD_WRONG              3
#define ERR_VOL_FORMAT_BAD              4
#define ERR_DRIVE_NOT_FOUND             6
#define ERR_FILES_OPEN                  7
#define ERR_VOL_SIZE_WRONG              8
#define ERR_COMPRESSION_NOT_SUPPORTED   9
#define ERR_PASSWORD_CHANGE_VOL_TYPE    10
#define ERR_PASSWORD_CHANGE_VOL_VERSION 11
#define ERR_VOL_SEEKING                 12
#define ERR_VOL_WRITING                 13
#define ERR_FILES_OPEN_LOCK             14
#define ERR_VOL_READING                 15
#define ERR_DRIVER_VERSION				16
#define ERR_NEW_VERSION_REQUIRED		17
#define ERR_CIPHER_INIT_FAILURE			18
#define ERR_CIPHER_INIT_WEAK_KEY		19
#define ERR_SELF_TESTS_FAILED			20
#define ERR_SECTOR_SIZE_INCOMPATIBLE	21

#define ERR_VOL_ALREADY_MOUNTED         32
#define ERR_NO_FREE_DRIVES              34
#define ERR_FILE_OPEN_FAILED            35
#define ERR_VOL_MOUNT_FAILED            36
#define ERR_INVALID_DEVICE              37
#define ERR_ACCESS_DENIED               38
#define ERR_MODE_INIT_FAILED            39

#define ERR_DONT_REPORT                 100

#define MIN_VOLUME_SIZE                 19456
#define MAX_VOLUME_SIZE                 0x7fffFFFFffffFFFFLL
#define MAX_FAT_VOLUME_SIZE				0x20000000000LL

#define WIDE(x) (LPWSTR)L##x

#ifdef MIRA_DRIVER

#pragma warning( disable : 4201 )
#pragma warning( disable : 4214 )
#pragma warning( disable : 4115 )
#pragma warning( disable : 4100 )
#pragma warning( disable : 4101 )
#pragma warning( disable : 4057 )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4514 )
#pragma warning( disable : 4127 )


#include <ntddk.h>		/* Standard header file for nt drivers */
#undef _WIN32_WINNT
#define	_WIN32_WINNT 0x0501
#include <ntdddisk.h>		/* Standard I/O control codes  */
#include <ntiologc.h>

#pragma warning( default : 4201 )
#pragma warning( default : 4214 )
#pragma warning( default : 4115 )
#pragma warning( default : 4100 )
#pragma warning( default : 4101 )
#pragma warning( default : 4057 )
#pragma warning( default : 4244 )
#pragma warning( default : 4127 )

/* #pragma warning( default : 4514 ) this warning remains disabled */

#define SMAlloc(size) ((void *) ExAllocatePoolWithTag( NonPagedPool, size, 'MMCT' ))
#define SMFree(memblock) ExFreePoolWithTag( memblock, 'MMCT' )

#define _DRIVER_PRJ_

#ifndef BOOL
typedef int BOOL;
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE !TRUE
#endif

/* Define dummies for the drivers */
typedef int HFILE;
typedef unsigned int WPARAM;
typedef unsigned __int32 LPARAM;
#define CALLBACK

#ifndef UINT
typedef unsigned int UINT;
#endif

#ifndef LRESULT
typedef unsigned __int32 LRESULT;
#endif
/* MIRA_DRIVER */

#elif defined(LINUX_DRIVER)	

#define SMAlloc(size) (kmalloc( size, GFP_KERNEL ))
#define SMFree(memblock) kfree( memblock )

#else

#ifdef _DRIVER_PRJ_
	#define SMAlloc(size) ((void *) ExAllocatePoolWithTag( NonPagedPool, size, 'MMCT' ))
	#define SMFree(memblock) ExFreePoolWithTag( memblock, 'MMCT' )
#else
	#define SMAlloc malloc
	#define SMFree free
#endif


#ifndef _DRIVER_PRJ_

#pragma warning( disable : 4201 )
#pragma warning( disable : 4214 )
#pragma warning( disable : 4115 )
#pragma warning( disable : 4514 )

#undef _WIN32_WINNT
#define	_WIN32_WINNT 0x0501
#include <windows.h>		/* Windows header */
#include <commctrl.h>		/* The common controls */
#include <process.h>		/* Process control */
#include <winioctl.h>
#include <stdio.h>		/* For sprintf */



#pragma warning( default : 4201 )
#pragma warning( default : 4214 )
#pragma warning( default : 4115 )

/* #pragma warning( default : 4514 ) this warning remains disabled */

/* This is needed to fix a bug with VC 5, the TCHAR macro _ttoi64 maps
   incorrectly to atoLL when it should be _atoi64 */
#define atoi64 _atoi64

#endif				/* _DRIVER_PRJ_ */

#endif				/* MIRA_DRIVER */

#define burn(mem,size) do { volatile char *burnm = (volatile char *)(mem); int burnc = size; RtlSecureZeroMemory (mem, size); while (burnc--) *burnm++ = 0; } while (0)

#ifndef MAX_PATH
#define MAX_PATH		260	/* Includes the null terminator */
#endif

#define MAX_URL_LENGTH	2084 /* Internet Explorer limit. Includes the terminating null character. */

#ifndef BYTE
	typedef unsigned long       DWORD;
	typedef int                 BOOL;
	typedef unsigned char       BYTE;
	typedef unsigned short      WORD;
#endif


#ifndef _FILETIME_
	typedef struct _FILETIME {
		DWORD dwLowDateTime;
		DWORD dwHighDateTime;
	} FILETIME, *PFILETIME, *LPFILETIME;
	#define  _FILETIME_
#endif
