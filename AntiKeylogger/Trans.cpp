#include "StdAfx.h"
#include "hash&crypt.h"
#include "Trans.h"

DWORD dword_444020[0x41] = {0x00040000, 0xE0FCA6D0, 0x05E431F5, 0x9596FF37, 0x29A52AAB, 0xEF1A5B02, 0x72D17AF1, 0xBEC7BB8E, 0x6242814A, \
							0x3C9182FA, 0xAD7E9984, 0xDA7F1753, 0x9CB28293, 0x4C32157B, 0x30613297, 0x9F86B6B9, 0x5CF601B0, \
							0xEB7B1FDE, 0xA04B7DD9, 0xA3BD3DBB, 0xDF1505F5, 0x0BB38D53, 0xA6112DB3, 0x1AF8CAAC, 0xFE094B79, \
							0x14A5066A, 0x7E0CFE0B, 0xC6A208D5, 0xD3208A62, 0x57C3C234, 0x4469D52F, 0x4359B693, 0xDF43B72C, \
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
							0x01000100};

DWORD sub_40E0E0(BYTE* buf, DWORD len, PSTRUCTJ pData)
{
	DWORD dwRet = 2;
	DWORD var_584 = 0;
	DWORD var_50C[0x41];
	RC4_CTX var_408;
	HASH_CTX var_56C;
	BYTE var_57C[0x10];

	if(buf == NULL || len <= 0 || pData == NULL)
		return dwRet;

	if(sub_420430(dword_444020, var_50C) != 0)
		return dwRet;

	sub_41F130(buf, &var_584, buf, 0x80, var_50C);
	sub_41DF60(&var_408, buf, 0x75);
	sub_41E170(&var_408, buf + 0x80, buf + 0x80, len - 80);
	sub_41CE70(&var_56C);
	sub_41CED0(&var_56C, buf + 0x90, len - 0x90);
	sub_41CFD0(&var_56C);
	sub_41D090(&var_56C, var_57C);
	if(memcmp(var_57C, buf + 0x80, 0x10) != 0)
		return dwRet;

	BYTE *esi = buf + 0x90;
	DWORD eax = 0;
	dwRet = !((esi[0] >> 2) & 1);
	pData->dwData = esi[4];// dword_444264
	memcpy(pData->pbData, esi + 0xC, 0x10);
	esi += 0x1C;
	eax = *((DWORD*)esi);
	esi += (eax + 4);
	eax = *((DWORD*)esi);
	esi += (eax + 4);
	eax = *((DWORD*)esi);
	esi += (eax + 4);
	pData->szData1 = (char*)esi + 4;// sz_44425C
    eax = *((DWORD*)esi);
	pData->szData2 = (char*)esi + eax + 8;// sz_444260

	// ".prtext", ".pdtext", ".pctext" 섹션내용을 해문한다.
	return dwRet;
}

DWORD sub_420430(DWORD* source, DWORD* target)
{
	if(source == NULL || target == NULL)
		return 3;

	DWORD var_4 = sub_420790(source[0]);
	if(var_4 < 0x1FC || var_4 > 0x400)
		return 0x10;

	target[0] = var_4;
	memcpy(&target[1], &source[1], 0x100);
	return 0;
}

DWORD sub_420790(DWORD arg_0)
{
	return ((arg_0 & 0xff) << 0x18) + (((arg_0 >> 8) & 0xff) << 0x10) + (((arg_0 >> 0x10) & 0xff) << 0x8) + (((arg_0 >> 0x18) & 0xff));
}

DWORD sub_41F130(BYTE* buf, DWORD* arg_4, BYTE* buf1, DWORD len, DWORD* arg_10)
{
	BYTE var_90[0x80];
	DWORD var_10, var_C, var_8, var_4;
	
	if(buf == NULL || arg_4 == NULL || buf1 == NULL || arg_10 == NULL)
		return 3;

	if(arg_10[0] < 0x1FC || arg_10[0] > 0x400)
		return 6;

	var_C = (arg_10[0] + 7) / 8;
	if(len > var_C)
		return 6;

	var_10 = sub_41EFE0(var_90, &var_8, buf1, len, arg_10);
	if(var_10 != 0)
		return var_10;

	if(var_90[0] != 0 || var_90[1] != 1)
		return 0x10;

	for(var_4 = 2; var_4 < var_C - 1 ; var_4 ++)
	{
		if(var_90[var_4] != 0xFF)
			break;
	}

	if(var_90[var_4] != 0)
		return 0x10;

	var_4 ++;
	*arg_4 = var_C - var_4;

	if(*arg_4 + 0x0B > var_C)
		return 6;

	memcpy(buf, var_90 + var_4, *arg_4);
	memset(var_90, 0, 0x80);

	return 0;
}

DWORD sub_41EFE0(BYTE* arg_0, DWORD* arg_4, BYTE* arg_8, DWORD arg_C, DWORD* arg_10)
{
	DWORD var_230[0x21], var_90[0x21], var_1A8[0x21], var_120[0x21], var_94, var_4;

	sub_420950(var_230, 0x21, arg_8, arg_C);
	sub_420950(var_90, 0x21, (BYTE*)arg_10 + 4, 0x80);
	sub_420950(var_1A8, 0x21, (BYTE*)arg_10 + 0x84, 0x80);
	var_94 = sub_4218C0(var_90, 0x21);
	var_4 = sub_4218C0(var_1A8, 0x21);

	if(sub_4217D0(var_230, var_90, var_94) >= 0)
		return 0x10;

	sub_421210(var_120, var_230, var_1A8, var_4, var_90, var_94);
	*arg_4 = (arg_10[0] + 7) / 8;
	sub_420A00(arg_0, *arg_4, var_120, var_94);
	memset(var_120, 0, 0x84);
	memset(var_230, 0, 0x84);

	return 0;
}

void sub_420950(DWORD* arg_0, DWORD arg_4, BYTE* arg_8, DWORD arg_C)
{
    DWORD i, var_8, var_C, temp;
	LONG var_10;

	var_10 = arg_C - 1;
	for(i = 0; i < arg_4 ; i ++) {
		if(var_10 < 0) break;
		for(var_8 = 0, var_C = 0; var_10 >= 0 && var_8 < 0x20; var_10 --, var_8 += 8) {
			temp = arg_8[var_10];
			temp = temp << var_8;
			temp |= var_C;
			var_C = temp;
		}
		arg_0[i] = var_C;
	}
	for(; i < arg_4 ; i ++) {
		arg_0[i] = 0;
	}

	return;
}

DWORD sub_4218C0(DWORD* arg_0, DWORD arg_4)
{
	LONG i ;
	for(i = arg_4 - 1; i >= 0 ; i --) {
		if(arg_0[i] != 0) break;
	}
	return (DWORD)i + 1;
}

LONG sub_4217D0(DWORD* arg_0, DWORD* arg_4, DWORD arg_8)
{
    for(LONG i = arg_8 - 1; i >= 0 ; i --) {
		if(arg_0[i] > arg_4[i]) return 1;
		if(arg_0[i] < arg_4[i]) return -1;
	}
	return 0;
}

void sub_421210(DWORD* arg_0, DWORD* arg_4, DWORD* arg_8, DWORD arg_C, DWORD* arg_10, DWORD arg_14)
{
	DWORD var_228[0x21], var_190[0x63];
	DWORD j, var_1A0, var_19C, var_194;
	sub_420AA0(var_190, arg_4, arg_14);
	sub_4211B0(var_190 + 0x21, var_190, arg_4, arg_10, arg_14);
	sub_4211B0(var_190 + 0x42, var_190 + 0x21, arg_4, arg_10, arg_14);
	sub_420AE0(var_228, arg_14);
	var_228[0] = 1;
	arg_C = sub_4218C0(arg_8, arg_C);
	for(LONG i = arg_C - 1; i >= 0; i --) {
		var_1A0 = arg_8[i];
		var_194 = 0x20;
		if(i == (LONG)arg_C - 1) {
			while(((var_1A0 >> 0x1E) & 3) == 0) {
				var_1A0 = var_1A0 << 2;
				var_194 -= 2;
			}
		}
		for(j = 0; j < var_194; j += 2, var_1A0 = var_1A0 << 2) {
			sub_4211B0(var_228, var_228, var_228, arg_10, arg_14);
			sub_4211B0(var_228, var_228, var_228, arg_10, arg_14);
			var_19C = ((var_1A0 >> 0x1E) & 3);
			if(var_19C != 0) {
				sub_4211B0(var_228, var_228, var_190 + 0x21 * (var_19C - 1), arg_10, arg_14);
			}
		}
	}
	sub_420AA0(arg_0, var_228, arg_14);
	memset(var_190, 0, 0x18C);
	memset(var_228, 0, 0x84);
}

void sub_420AA0(DWORD* arg_0, DWORD* arg_4, DWORD arg_8)
{
	for(DWORD i = 0; i < arg_8; i ++) arg_0[i] = arg_4[i];
}

void sub_4211B0(DWORD* arg_0, DWORD* arg_4, DWORD* arg_8, DWORD* arg_C, DWORD arg_10)
{
	DWORD var_108[0x42];
	sub_420CA0(var_108, arg_4, arg_8, arg_10);
	sub_421160(arg_0, var_108, arg_10 * 2, arg_C, arg_10);
	memset(var_108, 0, 0x108);
}

void sub_420CA0(DWORD* arg_0, DWORD* arg_4, DWORD* arg_8, DWORD arg_C)
{
	DWORD var_114, var_110[0x42], var_8, i;
	sub_420AE0(var_110, arg_C * 2);
	var_114 = sub_4218C0(arg_4, arg_C);
	var_8 = sub_4218C0(arg_8, arg_C);
	for(i = 0; i < var_114 ; i ++) {
		var_110[i + var_8] += sub_421900(var_110 + i, var_110 + i, arg_4[i], arg_8, var_8);
	}
	sub_420AA0(arg_0, var_110, arg_C * 2);
	memset(var_110, 0, 0x108);
}

void sub_420AE0(DWORD* arg_0, DWORD arg_4)
{
	for(DWORD i = 0 ; i < arg_4 ; i ++) arg_0[i] = 0;
}

DWORD sub_421900(DWORD* arg_0, DWORD* arg_4, DWORD arg_8, DWORD* arg_C, DWORD arg_10)
{
	DWORD i, var_8, var_10[2];
	if(arg_8 == 0) return 0;

	for(i = 0, var_8 = 0 ; i < arg_10 ; i ++) {
		sub_421AE0(var_10, arg_8, arg_C[i]);
		arg_0[i] = arg_4[i] + var_8;
		if(arg_0[i] < var_8) var_8 = 1;
		else var_8 = 0;
		arg_0[i] += var_10[0];
		if(arg_0[i] < var_10[0]) var_8 ++;
		var_8 += var_10[1];
	}
	return var_8;
}

void sub_421AE0(DWORD* arg_0, DWORD arg_4, DWORD arg_8)
{
	DWORD var_4, var_8, var_C, var_10, var_14, var_18;
	var_8 = (arg_4 >> 0x10) & 0xFFFF;
	var_10 = arg_4 & 0xFFFF;
	var_4 = (arg_8 >> 0x10) & 0xFFFF;
	var_14 = arg_8 & 0xFFFF;
	arg_0[0] = var_10 * var_14;
	var_18 = var_10 * var_4;
	var_C = var_8 * var_14;
	arg_0[1] = var_8 * var_4;
	var_18 += var_C;
	if(var_18 < var_C) arg_0[1] += 0x10000;
	var_C = var_18 << 0x10;
	arg_0[0] += var_C;
	if(arg_0[0] < var_C) arg_0[1] ++;
	arg_0[1] += (var_18 >> 0x10) & 0xFFFF;
}

void sub_421160(DWORD* arg_0, DWORD* arg_4, DWORD arg_8, DWORD* arg_C, DWORD arg_10)
{
	DWORD var_108[0x42];
	sub_420EB0(var_108, arg_0, arg_4, arg_8, arg_C, arg_10);
	memset(var_108, 0, 0x108);
}

void sub_420EB0(DWORD* arg_0, DWORD* arg_4, DWORD* arg_8, DWORD arg_C, DWORD* arg_10, DWORD arg_14)
{
	DWORD var_4, var_1B0, var_118[0x45], var_1A8[0x23], var_1AC, var_1B4;
	
	var_4 = sub_4218C0(arg_10, arg_14);
	if(var_4 == 0) return;
	var_1B0 = 0x20 - sub_421AA0(arg_10[var_4 - 1]);
	sub_420AE0(var_118, var_4);
	var_118[arg_C] = sub_420D90(var_118, arg_8, var_1B0, arg_C);
	sub_420D90(var_1A8, arg_10, var_1B0, var_4);
	var_1AC = var_1A8[var_4 - 1];
	sub_420AE0(arg_0, arg_C);
	for(LONG i = arg_C - var_4; i >= 0 ; i --) {
		if(var_1AC == 0xFFFFFFFF) {
			var_1B4 = var_118[i + var_4];
		}
		else {
			sub_421BD0(&var_1B4, &var_118[i + var_4 - 1], var_1AC + 1);
		}
		var_118[i + var_4] -= sub_4219D0(var_118 + i, var_118 + i, var_1B4, var_1A8, var_4);
		for(;;) {
			if(var_118[i + var_4] == 0) {
				if(sub_4217D0(var_118 + i, var_1A8, var_4) < 0) break;
			}
			var_1B4 ++;
			var_118[i + var_4] -= sub_420C00(var_118 + i, var_118 + i, var_1A8, var_4);
		}
		arg_0[i] = var_1B4;		
	}
	sub_420AE0(arg_4, arg_14);
	sub_420E20(arg_4, var_118, var_1B0, var_4);
	memset(var_118, 0, 0x10C);
	memset(var_1A8, 0, 0x84);
}

DWORD sub_421AA0(DWORD arg_0)
{
	DWORD i ;
	for(DWORD i = 0 ; i < 0x20 ; i ++, arg_0 = arg_0 >> 1)
		if(arg_0 == 0) break;
	return i;
}

DWORD sub_420D90(DWORD* arg_0, DWORD* arg_4, DWORD arg_8, DWORD arg_C)
{
	DWORD i, var_8, var_C, var_10, var_14;

	if(arg_8 >= 0x20) return 0;
	var_10 = 0x20 - arg_8;
	for(i = 0, var_8 = 0; i < arg_C; i ++) {
		var_C = arg_4[i];
		arg_0[i] = (var_C << (BYTE)arg_8) | var_8;
		if(arg_8 == 0) var_14 = 0;
		else var_14 = var_C >> (BYTE)var_10;
		var_8 = var_14;
	}
	return var_8;
}

void sub_421BD0(DWORD* arg_0, DWORD* arg_4, DWORD arg_8)
{
	DWORD var_8, var_C, var_1C, var_20;
	WORD var_4, var_10, var_14, var_18;

	var_4 = HIWORD(arg_8);
	var_14 = LOWORD(arg_8);
	var_20 = arg_4[0];
	var_1C = arg_4[1];
	if(var_4 == 0xFFFF) var_18 = HIWORD(var_1C);
	else var_18 = (WORD)(var_1C / (var_4 + 1));
	var_C = (DWORD)var_18 * var_14;
	var_8 = (DWORD)var_18 * var_4;
	var_20 -= var_C << 0x10;
	if(var_20 > 0xFFFFFFFF - (var_C << 0x10)) var_1C --;
	var_1C -= HIWORD(var_C);
	var_1C -= var_8;
	while(1) {
		if(var_1C <= (DWORD)var_4) {
			if(var_1C != (DWORD)var_4) break;
			if(var_20 < ((DWORD)var_14 << 0x10)) break;
		}
		var_20 -= ((DWORD)var_14 << 0x10);
		if(var_20 > 0xFFFFFFFF - ((DWORD)var_14 << 0x10)) var_1C --;
		var_1C -= (DWORD)var_4;
		var_18 ++;
	}
	if(var_4 == 0xFFFF) var_10 = LOWORD(var_1C);
	else var_10 = (WORD)(((var_1C << 0x10) + (var_20 >> 0x10) & 0xFFFF) / ((DWORD)var_4 + 1));
	var_C = (DWORD)var_10 * var_14;
	var_8 = (DWORD)var_10 * var_4;
	var_20 -= var_C;
	if(var_20 > 0xFFFFFFFF - var_C) var_1C--;
	var_20 -= var_8 << 0x10;
	if(var_20 > 0xFFFFFFFF - (var_8 << 0x10)) var_1C --;
	var_1C -= (var_8 >> 0x10) & 0xFFFF;
	while(1) {
		if((LONG)var_1C <= 0) {
			if(var_1C != 0) break;
			if(var_20 < arg_8) break;
		}
		var_20 -= arg_8;
		if(var_20 > 0xFFFFFFFF - arg_8) var_1C--;
		var_10 ++;
	}
	arg_0[0] = MAKELONG(var_10, var_18);
}

DWORD sub_4219D0(DWORD* arg_0, DWORD* arg_4, DWORD arg_8, DWORD* arg_C, DWORD arg_10)
{
	DWORD i, var_8, var_10[2];

	if(arg_8 == 0) return 0;
	for(i = 0, var_8 = 0; i < arg_10; i ++){
		sub_421AE0(var_10, arg_8, arg_C[i]);
		arg_0[i] = arg_4[i] - var_8;
		if(arg_0[i] > 0xFFFFFFFF - var_8) var_8 = 1;
		else var_8 = 0;
		arg_0[i] -= var_10[0];
		if(arg_0[i] > 0xFFFFFFFF - var_10[0]) var_8 ++;
		var_8 += var_10[1];
	}
	return var_8;
}

DWORD sub_420C00(DWORD* arg_0, DWORD* arg_4, DWORD* arg_8, DWORD arg_C)
{
	DWORD i, var_8, var_C;

	for(i = 0, var_8 = 0; i < arg_C; i ++) {
		var_C = arg_4[i] - var_8;
		if(var_C > 0xFFFFFFFF - var_8) var_C = 0xFFFFFFFF - arg_8[i];
		else {
			var_C -= arg_8[i];
			if(var_C > 0xFFFFFFFF - arg_8[i]) var_8 = 1;
			else var_8 = 0;
		}
		arg_0[i] = var_C;
	}
	return var_8;
}

DWORD sub_420E20(DWORD* arg_0, DWORD* arg_4, DWORD arg_8, DWORD arg_C)
{
	DWORD var_8, var_C, var_10, var_14;
	LONG i;
	
	if(arg_8 >= 0x20) return 0;
	var_10 = 0x20 - arg_8;
	for(i = arg_C - 1, var_8 = 0; i >= 0; i --) {
		var_C = arg_4[i];
		arg_0[i] = (var_C >> arg_8) | var_8;
		if(arg_8 == 0) var_14 = 0;
		else var_14 = var_C << var_10;
		var_8 = var_14;
	}
	return var_8;
}

void sub_420A00(BYTE* arg_0, DWORD arg_4, DWORD* arg_8, DWORD arg_C)
{
	DWORD i, var_8, var_C;
	LONG var_10;

	for(i = 0, var_10 = arg_4 - 1; i < arg_C, var_10 >= 0; i ++) {
		for(var_C = arg_8[i], var_8 = 0; var_10 >= 0, var_8 < 0x20; var_10 --, var_8 += 8) {
			arg_0[var_10] = (BYTE)(var_C >> var_8);
		}
	}
	for(; var_10 >= 0; var_10 --) arg_0[var_10] = 0;
}