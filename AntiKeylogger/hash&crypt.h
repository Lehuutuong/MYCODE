#ifndef _HASH_CRYPT_H_
#define _HASH_CRYPT_H_

typedef struct _HASH_CTX
{
	ULONG state[4];		//	state (ABCD)
	ULONGLONG count;	//	number of bits, modulo 2^64 (lsb first)
	ULONG length;			//  buffer length
	BYTE buffer[64];	//	input buffer
} HASH_CTX, *PHASH_CTX;

typedef struct _RC4_CTX {
	ULONG idx1;
	ULONG idx2;
	ULONG key[0x100];
} RC4_CTX, *PRC4_CTX;

// 해쉬바퍼를 초기화한다.
void sub_41CE70(HASH_CTX* pctx);
// 입구바퍼를 해쉬한다.
void sub_41CED0(HASH_CTX* pctx, PBYTE buf, DWORD len);
// 해쉬의 마감처리를 한다.
void sub_41CFD0(HASH_CTX* pctx);
// 바퍼의 내용을 0x10바이트만큼 복사한다.
void sub_41D090(HASH_CTX* pctx, PBYTE pbHash);
// 해쉬함수이다.
void sub_41D0B0(PBYTE source, HASH_CTX* pctx);
// buf에 변환을 적용하여 prc(암호열쇠)를 초기화한다.
void sub_41DF60(RC4_CTX* prc, BYTE* buf, DWORD len);
// prc(열쇠)에 기초하여 원천바퍼를 암호화하여 목적바퍼를 얻는다.
void sub_41E170(RC4_CTX* prc, PBYTE source, PBYTE target, ULONG len);

#endif // _HASH_CRYPT_H_