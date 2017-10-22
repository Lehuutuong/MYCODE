#ifndef _WINTRUST_H_
#define _WINTRUST_H_

#pragma comment (linker, "/defaultlib:wintrust.lib")

#define _WIN32_WINNT 0x600
#include <wincrypt.h>

typedef HANDLE HCATADMIN;
typedef HANDLE HCATINFO;
typedef struct CATALOG_INFO_ {
	DWORD cbStruct;  
	WCHAR wszCatalogFile[MAX_PATH];
} CATALOG_INFO;


/* dwUnionChoice */
#define WTD_CHOICE_FILE           1
#define WTD_CHOICE_CATALOG        2
#define WTD_CHOICE_BLOB           3
#define WTD_CHOICE_SIGNER         4
#define WTD_CHOICE_CERT           5

#define TRUST_E_CERT_SIGNATURE _HRESULT_TYPEDEF_(0x80096004L)
#define TRUST_E_PROVIDER_UNKNOWN _HRESULT_TYPEDEF_(0x800B0001L)
#define TRUST_E_ACTION_UNKNOWN _HRESULT_TYPEDEF_(0x800B0002L)
#define TRUST_E_SUBJECT_FORM_UNKNOWN _HRESULT_TYPEDEF_(0x800B0003L)
#define TRUST_E_SUBJECT_NOT_TRUSTED _HRESULT_TYPEDEF_(0x800B0004L)

#ifdef __cplusplus
extern "C" {
#endif
	
BOOL      
WINAPI 
CryptCATAdminAcquireContext(
							HCATADMIN* phCatAdmin,
							const GUID* pgSubsystem,
							DWORD dwFlags
							);

BOOL      
WINAPI 
CryptCATAdminCalcHashFromFileHandle(
                                    HANDLE hFile,
                                    DWORD* pcbHash,
                                    BYTE* pbHash,
                                    DWORD dwFlags
                                    );

HCATINFO  
WINAPI 
CryptCATAdminEnumCatalogFromHash(
								 HCATADMIN hCatAdmin,
								 BYTE* pbHash,
								 DWORD cbHash,
								 DWORD dwFlags,
								 HCATINFO* phPrevCatInfo
								 );

BOOL
WINAPI 
CryptCATAdminReleaseContext(
							HCATADMIN hCatAdmin,
							DWORD dwFlags
							);

BOOL 
WINAPI
CryptCATCatalogInfoFromContext(
                               HCATINFO hCatInfo,
                               CATALOG_INFO* psCatInfo,
                               DWORD dwFlags
                               );

BOOL 
WINAPI
CryptCATAdminReleaseCatalogContext(
                                   HCATADMIN hCatAdmin,
                                   HCATINFO hCatInfo,
                                   DWORD dwFlags
                                   );

	
#ifdef __cplusplus
}
#endif

#endif