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

// �ؽ����۸� �ʱ�ȭ�Ѵ�.
void sub_41CE70(HASH_CTX* pctx);
// �Ա����۸� �ؽ��Ѵ�.
void sub_41CED0(HASH_CTX* pctx, PBYTE buf, DWORD len);
// �ؽ��� ����ó���� �Ѵ�.
void sub_41CFD0(HASH_CTX* pctx);
// ������ ������ 0x10����Ʈ��ŭ �����Ѵ�.
void sub_41D090(HASH_CTX* pctx, PBYTE pbHash);
// �ؽ��Լ��̴�.
void sub_41D0B0(PBYTE source, HASH_CTX* pctx);
// buf�� ��ȯ�� �����Ͽ� prc(��ȣ����)�� �ʱ�ȭ�Ѵ�.
void sub_41DF60(RC4_CTX* prc, BYTE* buf, DWORD len);
// prc(����)�� �����Ͽ� ��õ���۸� ��ȣȭ�Ͽ� �������۸� ��´�.
void sub_41E170(RC4_CTX* prc, PBYTE source, PBYTE target, ULONG len);

#endif // _HASH_CRYPT_H_