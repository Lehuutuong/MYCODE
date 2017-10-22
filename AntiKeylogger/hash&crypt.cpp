#include "StdAfx.h"
#include "hash&crypt.h"

BYTE byte_443130[64] = {0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
						0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

DWORD dword_44302C[64] = {	0xD76AA478, 0xE8C7B756, 0x242070DB, 0xC1BDCEEE, 0xF57C0FAF, 0x4787C62A, 0xA8304613, 0xFD469501,
							0x698098D8, 0x8B44F7AF, 0xFFFF5BB1, 0x895CD7BE, 0x6B901122, 0xFD987193, 0xA679438E, 0x49B40821,
							0xF61E2562, 0xC040B340, 0x265E5A51, 0xE9B6C7AA, 0xD62F105D, 0x02441453, 0xD8A1E681, 0xE7D3FBC8,
							0x21E1CDE6, 0xC33707D6, 0xF4D50D87, 0x455A14ED, 0xA9E3E905, 0xFCEFA3F8, 0x676F02D9, 0x8D2A4C8A,
							0xFFFA3942, 0x8771F681, 0x6D9D6122, 0xFDE5380C, 0xA4BEEA44, 0x4BDECFA9, 0xF6BB4B60, 0xBEBFBC70,
							0x289B7EC6, 0xEAA127FA, 0xD4EF3085, 0x04881D05, 0xD9D4D039, 0xE6DB99E5, 0x1FA27CF8, 0xC4AC5665,
							0xF4292244, 0x432AFF97, 0xAB9423A7, 0xFC93A039, 0x655B59C3, 0x8F0CCC92, 0xFFEFF47D, 0x85845DD1,
							0x6FA87E4F, 0xFE2CE6E0, 0xA3014314, 0x4E0811A1, 0xF7537E82, 0xBD3AF235, 0x2AD7D2BB, 0xEB86D391};


void sub_41CE70(HASH_CTX* pctx)
{
	pctx->count = 0;
	pctx->length = 0;
	pctx->state[0] = 0x67452301;
	pctx->state[1] = 0xEFCDAB89;
	pctx->state[2] = 0x98BADCFE;
	pctx->state[3] = 0x10325476;
	memset(pctx->buffer, 0, 0x40);
}

void sub_41CED0(HASH_CTX* pctx, PBYTE buf, DWORD len)
{
	DWORD var_4, var_8 = 0x40 - pctx->length;

	pctx->count += len << 3;

	if(len < var_8)
	{
		memcpy(pctx->buffer + pctx->length, buf, len);
		pctx->length += len;
	}
	else
	{
		memcpy(pctx->buffer + pctx->length, buf, var_8);
		sub_41D0B0(pctx->buffer, pctx);
		var_4 = var_8;

		while(var_4 + 0x40 <= len)
		{
			sub_41D0B0(buf + var_4, pctx);
			var_4 += 0x40;
		}
		pctx->length = len - var_4;
		memcpy(pctx->buffer, buf + var_4, pctx->length);
	}
}

void sub_41CFD0(HASH_CTX* pctx)
{
	DWORD var_4 = 0x40 - pctx->length;
	
	if(var_4 > 8)
		memcpy(pctx->buffer + pctx->length, byte_443130, var_4 - 8);
	else
	{
		memcpy(pctx->buffer + pctx->length, byte_443130, var_4);
		sub_41D0B0(pctx->buffer, pctx);
		memcpy(pctx->buffer, byte_443130 + var_4, 0x38);
	}
	memcpy(pctx->buffer + 0x38, &pctx->count, 8);
	sub_41D0B0(pctx->buffer, pctx);
}

void sub_41D090(HASH_CTX* pctx, PBYTE pbHash)
{
	memcpy(pbHash, pctx->state, 0x10);
}

void sub_41D0B0(PBYTE source, HASH_CTX* pctx)
{
	DWORD var_4, var_8, var_C, var_10, var_14;

	__asm {
		mov     eax, pctx
		mov     ecx, [eax]
		mov     var_C, ecx
		mov     edx, pctx
		mov     eax, [edx+4]
		mov     var_4, eax
		mov     ecx, pctx
		mov     edx, [ecx+8]
		mov     var_8, edx
		mov     eax, pctx
		mov     ecx, [eax+0Ch]
		mov     var_14, ecx
		mov     edx, source
		mov     var_10, edx
		mov     eax, var_4
		and     eax, var_8
		mov     ecx, var_4
		not     ecx
		and     ecx, var_14
		or      eax, ecx
		add     eax, var_C
		mov     edx, var_10
		add     eax, [edx]
		add     eax, dword_44302C[0]
		mov     var_C, eax
		mov     eax, var_C
		shl     eax, 7
		mov     ecx, var_C
		shr     ecx, 19h
		or      eax, ecx
		mov     var_C, eax
		mov     edx, var_4
		add     edx, var_C
		mov     var_C, edx
		mov     eax, var_C
		and     eax, var_4
		mov     ecx, var_C
		not     ecx
		and     ecx, var_8
		or      eax, ecx
		add     eax, var_14
		mov     edx, var_10
		add     eax, [edx+4]
		add     eax, dword_44302C[0x4]
		mov     var_14, eax
		mov     eax, var_14
		shl     eax, 0Ch
		mov     ecx, var_14
		shr     ecx, 14h
		or      eax, ecx
		mov     var_14, eax
		mov     edx, var_C
		add     edx, var_14
		mov     var_14, edx
		mov     eax, var_14
		and     eax, var_C
		mov     ecx, var_14
		not     ecx
		and     ecx, var_4
		or      eax, ecx
		add     eax, var_8
		mov     edx, var_10
		add     eax, [edx+8]
		add     eax, dword_44302C[0x8]
		mov     var_8, eax
		mov     eax, var_8
		shl     eax, 11h
		mov     ecx, var_8
		shr     ecx, 0Fh
		or      eax, ecx
		mov     var_8, eax
		mov     edx, var_14
		add     edx, var_8
		mov     var_8, edx
		mov     eax, var_8
		and     eax, var_14
		mov     ecx, var_8
		not     ecx
		and     ecx, var_C
		or      eax, ecx
		add     eax, var_4
		mov     edx, var_10
		add     eax, [edx+0Ch]
		add     eax, dword_44302C[0x0C]
		mov     var_4, eax
		mov     eax, var_4
		shl     eax, 16h
		mov     ecx, var_4
		shr     ecx, 0Ah
		or      eax, ecx
		mov     var_4, eax
		mov     edx, var_8
		add     edx, var_4
		mov     var_4, edx
		mov     eax, var_4
		and     eax, var_8
		mov     ecx, var_4
		not     ecx
		and     ecx, var_14
		or      eax, ecx
		add     eax, var_C
		mov     edx, var_10
		add     eax, [edx+10h]
		add     eax, dword_44302C[0x10]
		mov     var_C, eax
		mov     eax, var_C
		shl     eax, 7
		mov     ecx, var_C
		shr     ecx, 19h
		or      eax, ecx
		mov     var_C, eax
		mov     edx, var_4
		add     edx, var_C
		mov     var_C, edx
		mov     eax, var_C
		and     eax, var_4
		mov     ecx, var_C
		not     ecx
		and     ecx, var_8
		or      eax, ecx
		add     eax, var_14
		mov     edx, var_10
		add     eax, [edx+14h]
		add     eax, dword_44302C[0x14]
		mov     var_14, eax
		mov     eax, var_14
		shl     eax, 0Ch
		mov     ecx, var_14
		shr     ecx, 14h
		or      eax, ecx
		mov     var_14, eax
		mov     edx, var_C
		add     edx, var_14
		mov     var_14, edx
		mov     eax, var_14
		and     eax, var_C
		mov     ecx, var_14
		not     ecx
		and     ecx, var_4
		or      eax, ecx
		add     eax, var_8
		mov     edx, var_10
		add     eax, [edx+18h]
		add     eax, dword_44302C[0x18]
		mov     var_8, eax
		mov     eax, var_8
		shl     eax, 11h
		mov     ecx, var_8
		shr     ecx, 0Fh
		or      eax, ecx
		mov     var_8, eax
		mov     edx, var_14
		add     edx, var_8
		mov     var_8, edx
		mov     eax, var_8
		and     eax, var_14
		mov     ecx, var_8
		not     ecx
		and     ecx, var_C
		or      eax, ecx
		add     eax, var_4
		mov     edx, var_10
		add     eax, [edx+1Ch]
		add     eax, dword_44302C[0x1C]
		mov     var_4, eax
		mov     eax, var_4
		shl     eax, 16h
		mov     ecx, var_4
		shr     ecx, 0Ah
		or      eax, ecx
		mov     var_4, eax
		mov     edx, var_8
		add     edx, var_4
		mov     var_4, edx
		mov     eax, var_4
		and     eax, var_8
		mov     ecx, var_4
		not     ecx
		and     ecx, var_14
		or      eax, ecx
		add     eax, var_C
		mov     edx, var_10
		add     eax, [edx+20h]
		add     eax, dword_44302C[0x20]
		mov     var_C, eax
		mov     eax, var_C
		shl     eax, 7
		mov     ecx, var_C
		shr     ecx, 19h
		or      eax, ecx
		mov     var_C, eax
		mov     edx, var_4
		add     edx, var_C
		mov     var_C, edx
		mov     eax, var_C
		and     eax, var_4
		mov     ecx, var_C
		not     ecx
		and     ecx, var_8
		or      eax, ecx
		add     eax, var_14
		mov     edx, var_10
		add     eax, [edx+24h]
		add     eax, dword_44302C[0x24]
		mov     var_14, eax
		mov     eax, var_14
		shl     eax, 0Ch
		mov     ecx, var_14
		shr     ecx, 14h
		or      eax, ecx
		mov     var_14, eax
		mov     edx, var_C
		add     edx, var_14
		mov     var_14, edx
		mov     eax, var_14
		and     eax, var_C
		mov     ecx, var_14
		not     ecx
		and     ecx, var_4
		or      eax, ecx
		add     eax, var_8
		mov     edx, var_10
		add     eax, [edx+28h]
		add     eax, dword_44302C[0x28]
		mov     var_8, eax
		mov     eax, var_8
		shl     eax, 11h
		mov     ecx, var_8
		shr     ecx, 0Fh
		or      eax, ecx
		mov     var_8, eax
		mov     edx, var_14
		add     edx, var_8
		mov     var_8, edx
		mov     eax, var_8
		and     eax, var_14
		mov     ecx, var_8
		not     ecx
		and     ecx, var_C
		or      eax, ecx
		add     eax, var_4
		mov     edx, var_10
		add     eax, [edx+2Ch]
		add     eax, dword_44302C[0x2C]
		mov     var_4, eax
		mov     eax, var_4
		shl     eax, 16h
		mov     ecx, var_4
		shr     ecx, 0Ah
		or      eax, ecx
		mov     var_4, eax
		mov     edx, var_8
		add     edx, var_4
		mov     var_4, edx
		mov     eax, var_4
		and     eax, var_8
		mov     ecx, var_4
		not     ecx
		and     ecx, var_14
		or      eax, ecx
		add     eax, var_C
		mov     edx, var_10
		add     eax, [edx+30h]
		add     eax, dword_44302C[0x30]
		mov     var_C, eax
		mov     eax, var_C
		shl     eax, 7
		mov     ecx, var_C
		shr     ecx, 19h
		or      eax, ecx
		mov     var_C, eax
		mov     edx, var_4
		add     edx, var_C
		mov     var_C, edx
		mov     eax, var_C
		and     eax, var_4
		mov     ecx, var_C
		not     ecx
		and     ecx, var_8
		or      eax, ecx
		add     eax, var_14
		mov     edx, var_10
		add     eax, [edx+34h]
		add     eax, dword_44302C[0x34]
		mov     var_14, eax
		mov     eax, var_14
		shl     eax, 0Ch
		mov     ecx, var_14
		shr     ecx, 14h
		or      eax, ecx
		mov     var_14, eax
		mov     edx, var_C
		add     edx, var_14
		mov     var_14, edx
		mov     eax, var_14
		and     eax, var_C
		mov     ecx, var_14
		not     ecx
		and     ecx, var_4
		or      eax, ecx
		add     eax, var_8
		mov     edx, var_10
		add     eax, [edx+38h]
		add     eax, dword_44302C[0x38]
		mov     var_8, eax
		mov     eax, var_8
		shl     eax, 11h
		mov     ecx, var_8
		shr     ecx, 0Fh
		or      eax, ecx
		mov     var_8, eax
		mov     edx, var_14
		add     edx, var_8
		mov     var_8, edx
		mov     eax, var_8
		and     eax, var_14
		mov     ecx, var_8
		not     ecx
		and     ecx, var_C
		or      eax, ecx
		add     eax, var_4
		mov     edx, var_10
		add     eax, [edx+3Ch]
		add     eax, dword_44302C[0x3C]
		mov     var_4, eax
		mov     eax, var_4
		shl     eax, 16h
		mov     ecx, var_4
		shr     ecx, 0Ah
		or      eax, ecx
		mov     var_4, eax
		mov     edx, var_8
		add     edx, var_4
		mov     var_4, edx
		mov     eax, var_4
		and     eax, var_14
		mov     ecx, var_14
		not     ecx
		and     ecx, var_8
		or      eax, ecx
		add     eax, var_C
		mov     edx, var_10
		add     eax, [edx+4]
		add     eax, dword_44302C[0x40]
		mov     var_C, eax
		mov     eax, var_C
		shl     eax, 5
		mov     ecx, var_C
		shr     ecx, 1Bh
		or      eax, ecx
		mov     var_C, eax
		mov     edx, var_4
		add     edx, var_C
		mov     var_C, edx
		mov     eax, var_C
		and     eax, var_8
		mov     ecx, var_8
		not     ecx
		and     ecx, var_4
		or      eax, ecx
		add     eax, var_14
		mov     edx, var_10
		add     eax, [edx+18h]
		add     eax, dword_44302C[0x44]
		mov     var_14, eax
		mov     eax, var_14
		shl     eax, 9
		mov     ecx, var_14
		shr     ecx, 17h
		or      eax, ecx
		mov     var_14, eax
		mov     edx, var_C
		add     edx, var_14
		mov     var_14, edx
		mov     eax, var_14
		and     eax, var_4
		mov     ecx, var_4
		not     ecx
		and     ecx, var_C
		or      eax, ecx
		add     eax, var_8
		mov     edx, var_10
		add     eax, [edx+2Ch]
		add     eax, dword_44302C[0x48]
		mov     var_8, eax
		mov     eax, var_8
		shl     eax, 0Eh
		mov     ecx, var_8
		shr     ecx, 12h
		or      eax, ecx
		mov     var_8, eax
		mov     edx, var_14
		add     edx, var_8
		mov     var_8, edx
		mov     eax, var_8
		and     eax, var_C
		mov     ecx, var_C
		not     ecx
		and     ecx, var_14
		or      eax, ecx
		add     eax, var_4
		mov     edx, var_10
		add     eax, [edx]
		add     eax, dword_44302C[0x4C]
		mov     var_4, eax
		mov     eax, var_4
		shl     eax, 14h
		mov     ecx, var_4
		shr     ecx, 0Ch
		or      eax, ecx
		mov     var_4, eax
		mov     edx, var_8
		add     edx, var_4
		mov     var_4, edx
		mov     eax, var_4
		and     eax, var_14
		mov     ecx, var_14
		not     ecx
		and     ecx, var_8
		or      eax, ecx
		add     eax, var_C
		mov     edx, var_10
		add     eax, [edx+14h]
		add     eax, dword_44302C[0x50]
		mov     var_C, eax
		mov     eax, var_C
		shl     eax, 5
		mov     ecx, var_C
		shr     ecx, 1Bh
		or      eax, ecx
		mov     var_C, eax
		mov     edx, var_4
		add     edx, var_C
		mov     var_C, edx
		mov     eax, var_C
		and     eax, var_8
		mov     ecx, var_8
		not     ecx
		and     ecx, var_4
		or      eax, ecx
		add     eax, var_14
		mov     edx, var_10
		add     eax, [edx+28h]
		add     eax, dword_44302C[0x54]
		mov     var_14, eax
		mov     eax, var_14
		shl     eax, 9
		mov     ecx, var_14
		shr     ecx, 17h
		or      eax, ecx
		mov     var_14, eax
		mov     edx, var_C
		add     edx, var_14
		mov     var_14, edx
		mov     eax, var_14
		and     eax, var_4
		mov     ecx, var_4
		not     ecx
		and     ecx, var_C
		or      eax, ecx
		add     eax, var_8
		mov     edx, var_10
		add     eax, [edx+3Ch]
		add     eax, dword_44302C[0x58]
		mov     var_8, eax
		mov     eax, var_8
		shl     eax, 0Eh
		mov     ecx, var_8
		shr     ecx, 12h
		or      eax, ecx
		mov     var_8, eax
		mov     edx, var_14
		add     edx, var_8
		mov     var_8, edx
		mov     eax, var_8
		and     eax, var_C
		mov     ecx, var_C
		not     ecx
		and     ecx, var_14
		or      eax, ecx
		add     eax, var_4
		mov     edx, var_10
		add     eax, [edx+10h]
		add     eax, dword_44302C[0x5C]
		mov     var_4, eax
		mov     eax, var_4
		shl     eax, 14h
		mov     ecx, var_4
		shr     ecx, 0Ch
		or      eax, ecx
		mov     var_4, eax
		mov     edx, var_8
		add     edx, var_4
		mov     var_4, edx
		mov     eax, var_4
		and     eax, var_14
		mov     ecx, var_14
		not     ecx
		and     ecx, var_8
		or      eax, ecx
		add     eax, var_C
		mov     edx, var_10
		add     eax, [edx+24h]
		add     eax, dword_44302C[0x60]
		mov     var_C, eax
		mov     eax, var_C
		shl     eax, 5
		mov     ecx, var_C
		shr     ecx, 1Bh
		or      eax, ecx
		mov     var_C, eax
		mov     edx, var_4
		add     edx, var_C
		mov     var_C, edx
		mov     eax, var_C
		and     eax, var_8
		mov     ecx, var_8
		not     ecx
		and     ecx, var_4
		or      eax, ecx
		add     eax, var_14
		mov     edx, var_10
		add     eax, [edx+38h]
		add     eax, dword_44302C[0x64]
		mov     var_14, eax
		mov     eax, var_14
		shl     eax, 9
		mov     ecx, var_14
		shr     ecx, 17h
		or      eax, ecx
		mov     var_14, eax
		mov     edx, var_C
		add     edx, var_14
		mov     var_14, edx
		mov     eax, var_14
		and     eax, var_4
		mov     ecx, var_4
		not     ecx
		and     ecx, var_C
		or      eax, ecx
		add     eax, var_8
		mov     edx, var_10
		add     eax, [edx+0Ch]
		add     eax, dword_44302C[0x68]
		mov     var_8, eax
		mov     eax, var_8
		shl     eax, 0Eh
		mov     ecx, var_8
		shr     ecx, 12h
		or      eax, ecx
		mov     var_8, eax
		mov     edx, var_14
		add     edx, var_8
		mov     var_8, edx
		mov     eax, var_8
		and     eax, var_C
		mov     ecx, var_C
		not     ecx
		and     ecx, var_14
		or      eax, ecx
		add     eax, var_4
		mov     edx, var_10
		add     eax, [edx+20h]
		add     eax, dword_44302C[0x6C]
		mov     var_4, eax
		mov     eax, var_4
		shl     eax, 14h
		mov     ecx, var_4
		shr     ecx, 0Ch
		or      eax, ecx
		mov     var_4, eax
		mov     edx, var_8
		add     edx, var_4
		mov     var_4, edx
		mov     eax, var_4
		and     eax, var_14
		mov     ecx, var_14
		not     ecx
		and     ecx, var_8
		or      eax, ecx
		add     eax, var_C
		mov     edx, var_10
		add     eax, [edx+34h]
		add     eax, dword_44302C[0x70]
		mov     var_C, eax
		mov     eax, var_C
		shl     eax, 5
		mov     ecx, var_C
		shr     ecx, 1Bh
		or      eax, ecx
		mov     var_C, eax
		mov     edx, var_4
		add     edx, var_C
		mov     var_C, edx
		mov     eax, var_C
		and     eax, var_8
		mov     ecx, var_8
		not     ecx
		and     ecx, var_4
		or      eax, ecx
		add     eax, var_14
		mov     edx, var_10
		add     eax, [edx+8]
		add     eax, dword_44302C[0x74]
		mov     var_14, eax
		mov     eax, var_14
		shl     eax, 9
		mov     ecx, var_14
		shr     ecx, 17h
		or      eax, ecx
		mov     var_14, eax
		mov     edx, var_C
		add     edx, var_14
		mov     var_14, edx
		mov     eax, var_14
		and     eax, var_4
		mov     ecx, var_4
		not     ecx
		and     ecx, var_C
		or      eax, ecx
		add     eax, var_8
		mov     edx, var_10
		add     eax, [edx+1Ch]
		add     eax, dword_44302C[0x78]
		mov     var_8, eax
		mov     eax, var_8
		shl     eax, 0Eh
		mov     ecx, var_8
		shr     ecx, 12h
		or      eax, ecx
		mov     var_8, eax
		mov     edx, var_14
		add     edx, var_8
		mov     var_8, edx
		mov     eax, var_8
		and     eax, var_C
		mov     ecx, var_C
		not     ecx
		and     ecx, var_14
		or      eax, ecx
		add     eax, var_4
		mov     edx, var_10
		add     eax, [edx+30h]
		add     eax, dword_44302C[0x7C]
		mov     var_4, eax
		mov     eax, var_4
		shl     eax, 14h
		mov     ecx, var_4
		shr     ecx, 0Ch
		or      eax, ecx
		mov     var_4, eax
		mov     edx, var_8
		add     edx, var_4
		mov     var_4, edx
		mov     eax, var_4
		xor     eax, var_8
		xor     eax, var_14
		add     eax, var_C
		mov     ecx, var_10
		add     eax, [ecx+14h]
		add     eax, dword_44302C[0x80]
		mov     var_C, eax
		mov     edx, var_C
		shl     edx, 4
		mov     eax, var_C
		shr     eax, 1Ch
		or      edx, eax
		mov     var_C, edx
		mov     ecx, var_4
		add     ecx, var_C
		mov     var_C, ecx
		mov     edx, var_C
		xor     edx, var_4
		xor     edx, var_8
		add     edx, var_14
		mov     eax, var_10
		add     edx, [eax+20h]
		add     edx, dword_44302C[0x84]
		mov     var_14, edx
		mov     ecx, var_14
		shl     ecx, 0Bh
		mov     edx, var_14
		shr     edx, 15h
		or      ecx, edx
		mov     var_14, ecx
		mov     eax, var_C
		add     eax, var_14
		mov     var_14, eax
		mov     ecx, var_14
		xor     ecx, var_C
		xor     ecx, var_4
		add     ecx, var_8
		mov     edx, var_10
		add     ecx, [edx+2Ch]
		add     ecx, dword_44302C[0x88]
		mov     var_8, ecx
		mov     eax, var_8
		shl     eax, 10h
		mov     ecx, var_8
		shr     ecx, 10h
		or      eax, ecx
		mov     var_8, eax
		mov     edx, var_14
		add     edx, var_8
		mov     var_8, edx
		mov     eax, var_8
		xor     eax, var_14
		xor     eax, var_C
		add     eax, var_4
		mov     ecx, var_10
		add     eax, [ecx+38h]
		add     eax, dword_44302C[0x8C]
		mov     var_4, eax
		mov     edx, var_4
		shl     edx, 17h
		mov     eax, var_4
		shr     eax, 9
		or      edx, eax
		mov     var_4, edx
		mov     ecx, var_8
		add     ecx, var_4
		mov     var_4, ecx
		mov     edx, var_4
		xor     edx, var_8
		xor     edx, var_14
		add     edx, var_C
		mov     eax, var_10
		add     edx, [eax+4]
		add     edx, dword_44302C[0x90]
		mov     var_C, edx
		mov     ecx, var_C
		shl     ecx, 4
		mov     edx, var_C
		shr     edx, 1Ch
		or      ecx, edx
		mov     var_C, ecx
		mov     eax, var_4
		add     eax, var_C
		mov     var_C, eax
		mov     ecx, var_C
		xor     ecx, var_4
		xor     ecx, var_8
		add     ecx, var_14
		mov     edx, var_10
		add     ecx, [edx+10h]
		add     ecx, dword_44302C[0x94]
		mov     var_14, ecx
		mov     eax, var_14
		shl     eax, 0Bh
		mov     ecx, var_14
		shr     ecx, 15h
		or      eax, ecx
		mov     var_14, eax
		mov     edx, var_C
		add     edx, var_14
		mov     var_14, edx
		mov     eax, var_14
		xor     eax, var_C
		xor     eax, var_4
		add     eax, var_8
		mov     ecx, var_10
		add     eax, [ecx+1Ch]
		add     eax, dword_44302C[0x98]
		mov     var_8, eax
		mov     edx, var_8
		shl     edx, 10h
		mov     eax, var_8
		shr     eax, 10h
		or      edx, eax
		mov     var_8, edx
		mov     ecx, var_14
		add     ecx, var_8
		mov     var_8, ecx
		mov     edx, var_8
		xor     edx, var_14
		xor     edx, var_C
		add     edx, var_4
		mov     eax, var_10
		add     edx, [eax+28h]
		add     edx, dword_44302C[0x9C]
		mov     var_4, edx
		mov     ecx, var_4
		shl     ecx, 17h
		mov     edx, var_4
		shr     edx, 9
		or      ecx, edx
		mov     var_4, ecx
		mov     eax, var_8
		add     eax, var_4
		mov     var_4, eax
		mov     ecx, var_4
		xor     ecx, var_8
		xor     ecx, var_14
		add     ecx, var_C
		mov     edx, var_10
		add     ecx, [edx+34h]
		add     ecx, dword_44302C[0x0A0]
		mov     var_C, ecx
		mov     eax, var_C
		shl     eax, 4
		mov     ecx, var_C
		shr     ecx, 1Ch
		or      eax, ecx
		mov     var_C, eax
		mov     edx, var_4
		add     edx, var_C
		mov     var_C, edx
		mov     eax, var_C
		xor     eax, var_4
		xor     eax, var_8
		add     eax, var_14
		mov     ecx, var_10
		add     eax, [ecx]
		add     eax, dword_44302C[0x0A4]
		mov     var_14, eax
		mov     edx, var_14
		shl     edx, 0Bh
		mov     eax, var_14
		shr     eax, 15h
		or      edx, eax
		mov     var_14, edx
		mov     ecx, var_C
		add     ecx, var_14
		mov     var_14, ecx
		mov     edx, var_14
		xor     edx, var_C
		xor     edx, var_4
		add     edx, var_8
		mov     eax, var_10
		add     edx, [eax+0Ch]
		add     edx, dword_44302C[0x0A8]
		mov     var_8, edx
		mov     ecx, var_8
		shl     ecx, 10h
		mov     edx, var_8
		shr     edx, 10h
		or      ecx, edx
		mov     var_8, ecx
		mov     eax, var_14
		add     eax, var_8
		mov     var_8, eax
		mov     ecx, var_8
		xor     ecx, var_14
		xor     ecx, var_C
		add     ecx, var_4
		mov     edx, var_10
		add     ecx, [edx+18h]
		add     ecx, dword_44302C[0x0AC]
		mov     var_4, ecx
		mov     eax, var_4
		shl     eax, 17h
		mov     ecx, var_4
		shr     ecx, 9
		or      eax, ecx
		mov     var_4, eax
		mov     edx, var_8
		add     edx, var_4
		mov     var_4, edx
		mov     eax, var_4
		xor     eax, var_8
		xor     eax, var_14
		add     eax, var_C
		mov     ecx, var_10
		add     eax, [ecx+24h]
		add     eax, dword_44302C[0x0B0]
		mov     var_C, eax
		mov     edx, var_C
		shl     edx, 4
		mov     eax, var_C
		shr     eax, 1Ch
		or      edx, eax
		mov     var_C, edx
		mov     ecx, var_4
		add     ecx, var_C
		mov     var_C, ecx
		mov     edx, var_C
		xor     edx, var_4
		xor     edx, var_8
		add     edx, var_14
		mov     eax, var_10
		add     edx, [eax+30h]
		add     edx, dword_44302C[0x0B4]
		mov     var_14, edx
		mov     ecx, var_14
		shl     ecx, 0Bh
		mov     edx, var_14
		shr     edx, 15h
		or      ecx, edx
		mov     var_14, ecx
		mov     eax, var_C
		add     eax, var_14
		mov     var_14, eax
		mov     ecx, var_14
		xor     ecx, var_C
		xor     ecx, var_4
		add     ecx, var_8
		mov     edx, var_10
		add     ecx, [edx+3Ch]
		add     ecx, dword_44302C[0x0B8]
		mov     var_8, ecx
		mov     eax, var_8
		shl     eax, 10h
		mov     ecx, var_8
		shr     ecx, 10h
		or      eax, ecx
		mov     var_8, eax
		mov     edx, var_14
		add     edx, var_8
		mov     var_8, edx
		mov     eax, var_8
		xor     eax, var_14
		xor     eax, var_C
		add     eax, var_4
		mov     ecx, var_10
		add     eax, [ecx+8]
		add     eax, dword_44302C[0x0BC]
		mov     var_4, eax
		mov     edx, var_4
		shl     edx, 17h
		mov     eax, var_4
		shr     eax, 9
		or      edx, eax
		mov     var_4, edx
		mov     ecx, var_8
		add     ecx, var_4
		mov     var_4, ecx
		mov     edx, var_14
		not     edx
		or      edx, var_4
		xor     edx, var_8
		add     edx, var_C
		mov     eax, var_10
		add     edx, [eax]
		add     edx, dword_44302C[0x0C0]
		mov     var_C, edx
		mov     ecx, var_C
		shl     ecx, 6
		mov     edx, var_C
		shr     edx, 1Ah
		or      ecx, edx
		mov     var_C, ecx
		mov     eax, var_4
		add     eax, var_C
		mov     var_C, eax
		mov     ecx, var_8
		not     ecx
		or      ecx, var_C
		xor     ecx, var_4
		add     ecx, var_14
		mov     edx, var_10
		add     ecx, [edx+1Ch]
		add     ecx, dword_44302C[0x0C4]
		mov     var_14, ecx
		mov     eax, var_14
		shl     eax, 0Ah
		mov     ecx, var_14
		shr     ecx, 16h
		or      eax, ecx
		mov     var_14, eax
		mov     edx, var_C
		add     edx, var_14
		mov     var_14, edx
		mov     eax, var_4
		not     eax
		or      eax, var_14
		xor     eax, var_C
		add     eax, var_8
		mov     ecx, var_10
		add     eax, [ecx+38h]
		add     eax, dword_44302C[0x0C8]
		mov     var_8, eax
		mov     edx, var_8
		shl     edx, 0Fh
		mov     eax, var_8
		shr     eax, 11h
		or      edx, eax
		mov     var_8, edx
		mov     ecx, var_14
		add     ecx, var_8
		mov     var_8, ecx
		mov     edx, var_C
		not     edx
		or      edx, var_8
		xor     edx, var_14
		add     edx, var_4
		mov     eax, var_10
		add     edx, [eax+14h]
		add     edx, dword_44302C[0x0CC]
		mov     var_4, edx
		mov     ecx, var_4
		shl     ecx, 15h
		mov     edx, var_4
		shr     edx, 0Bh
		or      ecx, edx
		mov     var_4, ecx
		mov     eax, var_8
		add     eax, var_4
		mov     var_4, eax
		mov     ecx, var_14
		not     ecx
		or      ecx, var_4
		xor     ecx, var_8
		add     ecx, var_C
		mov     edx, var_10
		add     ecx, [edx+30h]
		add     ecx, dword_44302C[0x0D0]
		mov     var_C, ecx
		mov     eax, var_C
		shl     eax, 6
		mov     ecx, var_C
		shr     ecx, 1Ah
		or      eax, ecx
		mov     var_C, eax
		mov     edx, var_4
		add     edx, var_C
		mov     var_C, edx
		mov     eax, var_8
		not     eax
		or      eax, var_C
		xor     eax, var_4
		add     eax, var_14
		mov     ecx, var_10
		add     eax, [ecx+0Ch]
		add     eax, dword_44302C[0x0D4]
		mov     var_14, eax
		mov     edx, var_14
		shl     edx, 0Ah
		mov     eax, var_14
		shr     eax, 16h
		or      edx, eax
		mov     var_14, edx
		mov     ecx, var_C
		add     ecx, var_14
		mov     var_14, ecx
		mov     edx, var_4
		not     edx
		or      edx, var_14
		xor     edx, var_C
		add     edx, var_8
		mov     eax, var_10
		add     edx, [eax+28h]
		add     edx, dword_44302C[0x0D8]
		mov     var_8, edx
		mov     ecx, var_8
		shl     ecx, 0Fh
		mov     edx, var_8
		shr     edx, 11h
		or      ecx, edx
		mov     var_8, ecx
		mov     eax, var_14
		add     eax, var_8
		mov     var_8, eax
		mov     ecx, var_C
		not     ecx
		or      ecx, var_8
		xor     ecx, var_14
		add     ecx, var_4
		mov     edx, var_10
		add     ecx, [edx+4]
		add     ecx, dword_44302C[0x0DC]
		mov     var_4, ecx
		mov     eax, var_4
		shl     eax, 15h
		mov     ecx, var_4
		shr     ecx, 0Bh
		or      eax, ecx
		mov     var_4, eax
		mov     edx, var_8
		add     edx, var_4
		mov     var_4, edx
		mov     eax, var_14
		not     eax
		or      eax, var_4
		xor     eax, var_8
		add     eax, var_C
		mov     ecx, var_10
		add     eax, [ecx+20h]
		add     eax, dword_44302C[0x0E0]
		mov     var_C, eax
		mov     edx, var_C
		shl     edx, 6
		mov     eax, var_C
		shr     eax, 1Ah
		or      edx, eax
		mov     var_C, edx
		mov     ecx, var_4
		add     ecx, var_C
		mov     var_C, ecx
		mov     edx, var_8
		not     edx
		or      edx, var_C
		xor     edx, var_4
		add     edx, var_14
		mov     eax, var_10
		add     edx, [eax+3Ch]
		add     edx, dword_44302C[0x0E4]
		mov     var_14, edx
		mov     ecx, var_14
		shl     ecx, 0Ah
		mov     edx, var_14
		shr     edx, 16h
		or      ecx, edx
		mov     var_14, ecx
		mov     eax, var_C
		add     eax, var_14
		mov     var_14, eax
		mov     ecx, var_4
		not     ecx
		or      ecx, var_14
		xor     ecx, var_C
		add     ecx, var_8
		mov     edx, var_10
		add     ecx, [edx+18h]
		add     ecx, dword_44302C[0x0E8]
		mov     var_8, ecx
		mov     eax, var_8
		shl     eax, 0Fh
		mov     ecx, var_8
		shr     ecx, 11h
		or      eax, ecx
		mov     var_8, eax
		mov     edx, var_14
		add     edx, var_8
		mov     var_8, edx
		mov     eax, var_C
		not     eax
		or      eax, var_8
		xor     eax, var_14
		add     eax, var_4
		mov     ecx, var_10
		add     eax, [ecx+34h]
		add     eax, dword_44302C[0x0EC]
		mov     var_4, eax
		mov     edx, var_4
		shl     edx, 15h
		mov     eax, var_4
		shr     eax, 0Bh
		or      edx, eax
		mov     var_4, edx
		mov     ecx, var_8
		add     ecx, var_4
		mov     var_4, ecx
		mov     edx, var_14
		not     edx
		or      edx, var_4
		xor     edx, var_8
		add     edx, var_C
		mov     eax, var_10
		add     edx, [eax+10h]
		add     edx, dword_44302C[0x0F0]
		mov     var_C, edx
		mov     ecx, var_C
		shl     ecx, 6
		mov     edx, var_C
		shr     edx, 1Ah
		or      ecx, edx
		mov     var_C, ecx
		mov     eax, var_4
		add     eax, var_C
		mov     var_C, eax
		mov     ecx, var_8
		not     ecx
		or      ecx, var_C
		xor     ecx, var_4
		add     ecx, var_14
		mov     edx, var_10
		add     ecx, [edx+2Ch]
		add     ecx, dword_44302C[0x0F4]
		mov     var_14, ecx
		mov     eax, var_14
		shl     eax, 0Ah
		mov     ecx, var_14
		shr     ecx, 16h
		or      eax, ecx
		mov     var_14, eax
		mov     edx, var_C
		add     edx, var_14
		mov     var_14, edx
		mov     eax, var_4
		not     eax
		or      eax, var_14
		xor     eax, var_C
		add     eax, var_8
		mov     ecx, var_10
		add     eax, [ecx+8]
		add     eax, dword_44302C[0x0F8]
		mov     var_8, eax
		mov     edx, var_8
		shl     edx, 0Fh
		mov     eax, var_8
		shr     eax, 11h
		or      edx, eax
		mov     var_8, edx
		mov     ecx, var_14
		add     ecx, var_8
		mov     var_8, ecx
		mov     edx, var_C
		not     edx
		or      edx, var_8
		xor     edx, var_14
		add     edx, var_4
		mov     eax, var_10
		add     edx, [eax+24h]
		add     edx, dword_44302C[0x0FC]
		mov     var_4, edx
		mov     ecx, var_4
		shl     ecx, 15h
		mov     edx, var_4
		shr     edx, 0Bh
		or      ecx, edx
		mov     var_4, ecx
		mov     eax, var_8
		add     eax, var_4
		mov     var_4, eax
		mov     ecx, pctx
		mov     edx, [ecx]
		add     edx, var_C
		mov     eax, pctx
		mov     [eax], edx
		mov     ecx, pctx
		mov     edx, [ecx+4]
		add     edx, var_4
		mov     eax, pctx
		mov     [eax+4], edx
		mov     ecx, pctx
		mov     edx, [ecx+8]
		add     edx, var_8
		mov     eax, pctx
		mov     [eax+8], edx
		mov     ecx, pctx
		mov     edx, [ecx+0Ch]
		add     edx, var_14
		mov     eax, pctx
		mov     [eax+0Ch], edx
	}
}

void sub_41DF60(RC4_CTX* prc, BYTE* buf, DWORD len)
{
	for(DWORD i = 0; i < 0x100; i ++)
		prc->key[i] = i;
	prc->idx1 = 0;
	prc->idx2 = 0;
	DWORD var_C = 0;
	DWORD var_8, var_18 = var_C;
	DWORD idx;
	for(i = 0; i < 0x100; i += 4)
	{
		var_8 = prc->key[i];
		idx = buf[var_18];
		idx += (var_8 + var_C);
		idx &= 0xFF;
		var_C = idx;
		var_18 ++;
		if(var_18 == len)
			var_18 = 0;
		prc->key[i] = prc->key[var_C];
		prc->key[var_C] = var_8;

		var_8 = prc->key[i + 1];
		idx = buf[var_18];
		idx += (var_8 + var_C);
		idx &= 0xFF;
		var_C = idx;
		var_18 ++;
		if(var_18 == len)
			var_18 = 0;
		prc->key[i + 1] = prc->key[var_C];
		prc->key[var_C] = var_8;  

		var_8 = prc->key[i + 2];
		idx = buf[var_18];
		idx += (var_8 + var_C);
		idx &= 0xFF;
		var_C = idx;
		var_18 ++;
		if(var_18 == len)
			var_18 = 0;
		prc->key[i + 2] = prc->key[var_C];
		prc->key[var_C] = var_8;  

		var_8 = prc->key[i + 3];
		idx = buf[var_18];
		idx += (var_8 + var_C);
		idx &= 0xFF;
		var_C = idx;
		var_18 ++;
		if(var_18 == len)
			var_18 = 0;
		prc->key[i + 3] = prc->key[var_C];
		prc->key[var_C] = var_8;  
	}
}

void sub_41E170(RC4_CTX* prc, PBYTE source, PBYTE target, ULONG len)
{
	DWORD var_8 = prc->idx1;
	DWORD var_4 = prc->idx2;
	DWORD var_10, var_18;
	DWORD idx;

	for(ULONG i = 0 ; i < len ; i ++)
	{
		var_8 = (var_8 + 1) & 0xFF;
		var_18 = prc->key[var_8];
		var_4 = (var_18 + var_4) & 0xFF;
		var_10 = prc->key[var_4];
		prc->key[var_8] = var_10;
		prc->key[var_4] = var_18;
		idx = (var_18 + var_10) & 0xFF;
		target[i] = source[i] ^ (BYTE)prc->key[idx];
	}
	prc->idx1 = var_8;
	prc->idx2 = var_4;
}