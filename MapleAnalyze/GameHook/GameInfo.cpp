// GameInfo.cpp
#include "stdafx.h"
#include "GameInfo.h"



CString GetBotJob()
{
	DWORD dwMemAddr = MEMADDR_BOTINFO;
	DWORD var_4, var_C, var_8;
	LPBYTE pByte;
	CString strJob = "";

	if (IsBadReadPtr((LPVOID)dwMemAddr, 4) == TRUE) 
	{			
		return strJob;
	}

	dwMemAddr = *(LPDWORD)dwMemAddr;
	dwMemAddr += MEMADDR_BOTJOB_OFFSET;
	if (IsBadReadPtr((LPVOID)dwMemAddr, 4) == TRUE) 
	{
		return strJob;
	}

	pByte = (LPBYTE)dwMemAddr;

	var_C = (pByte[1] ^ pByte[3]) * 256 + (pByte[0] ^ pByte[2]);

	var_4 = MEMADDR_BOTJOBFUNC;

	__asm{
		pushad
			push var_C
			call var_4
			add esp, 4
			mov var_8, eax
			popad
	}

	strJob.Format("%s-%d", var_8, var_C);
	return strJob;
}


void GetBotPosition(POINT &pBotPoint)
{
	DWORD dwAddr = MEMADDR_BOTPOSITION;
	DWORD ebx;

	pBotPoint.x = -1000;
	pBotPoint.y = -1000;

	if (IsBadReadPtr((LPVOID)dwAddr, 4)) 
	{
		return;
	}

	dwAddr = *(LPDWORD)dwAddr;
	if (dwAddr == 0) return; // 120711 팅김 

	dwAddr = dwAddr + MEMADDR_BOTPOSITION_OFFSET;
	if ( IsBadReadPtr((LPVOID)(dwAddr + 8), 4))
	{
		ebx = 0;
	}
	else ebx = Descramble((LPDWORD)dwAddr);


	dwAddr += 12;
	if ( IsBadReadPtr((LPVOID)(dwAddr + 8), 4) )
	{
		pBotPoint.y= ebx;
		pBotPoint.x = 0;
	}
	else
	{
		pBotPoint.x = Descramble((LPDWORD)dwAddr);
		pBotPoint.y = ebx;
	}
	//PrintLogA("Bot Pos %d--%d", pBotPoint->x, pBotPoint->y);
}

DWORD Descramble(LPDWORD pEax)
{
	//DebugMessage(1, "Descramble start pEax = 0x%0x", (DWORD)pEax);
	LPBYTE pEsi = NULL;
	BYTE dl, cl, al;
	DWORD var_4;
	LPBYTE lpRet = (LPBYTE)&var_4;

	pEsi = (LPBYTE)pEax[2];
	//DebugMessage(1, "Descramble start pEsi = 0x%0x", (DWORD)pEsi);

	dl = pEsi[4];
	if ( !dl ) dl = 42;

	al = *pEsi;
	cl = al + dl + 42;
	al = al ^ dl;
	*lpRet = al;
	if ( !cl ) cl = 42;

	dl = pEsi[1];
	al = dl + cl + 42;
	dl = dl ^ cl;
	*(lpRet + 1) = dl;
	if ( !al ) al = 42;

	dl = pEsi[2];
	cl = dl + al + 42;
	dl = dl ^ al;
	*(lpRet + 2) = dl;
	if ( !cl ) cl = 42;

	//  loc_1001E18E
	dl = pEsi[3];
	dl = dl ^ cl;
	*(lpRet + 3) = dl;

	//DebugMessage(1, "Descramble end return = 0x%0x", (DWORD)var_4);
	return var_4;
}


BYTE g_KeyConvertTable[0x4A] = {
	0,		1,		0x19,	0x19,//0
	0x19,	0x19,	0x19,	0x19,//4
	0x19,	0x19,	0x19,	0x19,//8
	0x19,	0x19,	0x19,	0x19,//c
	0x19,	2,		3,		4,	//10
	5,		0x19,	0x19,	0x19,//14
	0x19,	0x19,	0x19,	0x19,//18
	0x19,	6,		7,		0x19,//1c
	0x19,	0x19,	0x19,	0x19,//20
	0x19,	0x19,	0x19,	0x19,//24
	0x19,	0x19,	0x19,	0x19,//28
	0x19,	0x19,	0x19,	0x19,//2c
	0x19,	8,		9,		0x0A,//30
	0x0B,	0x19,	0x0C,	0x0D,//34
	0x0E,	0x19,	0x0F,	0x19,//38
	0x10,	0x19,	0x11,	0x12,//3c
	0x19,	0x19,	0x13,	0x19,//40
	0x14,	0x15,	0x16,	0x19,//44
	0x17,	0x18	};//48

BOOL KeySetting(DWORD dwID /*arg_0*/, BYTE key /*al*/, BYTE type /*bl*/)
{		
	key -= 0x10;
	int esi = -1;

	DWORD dwMemAddr;

	if (key < 0x4A)
	{
		switch (g_KeyConvertTable[key])
		{
		case 0:esi = 0xD2;break;
		case 1:esi = 0x91;break;
		case 2:esi = 0x16D;break;
		case 3:esi = 0x195;break;
		case 4:esi = 0x18B;break;
		case 5:esi = 0x163;	break;
		case 6:esi = 0x19A;break;
		case 7:esi = 0x19F;break;
		case 8:esi = 0x96;break;
		case 9:esi = 0xF0;break;
		case 10:esi = 0xE6;break;
		case 11:esi = 0xA0;break;
		case 12:esi = 0xA5;break;
		case 13:esi = 0xAA;break;
		case 14:esi = 0xAF;break;
		case 15:esi = 0xB4;break;
		case 16:esi = 0xBE;break;
		case 17:esi = 0xF5;break;
		case 18:esi = 0x78;break;
		case 19:esi = 0x5F;break;
		case 20:esi = 0x64;break;
		case 21:esi = 0x6E;break;
		case 22:esi = 0xEB;break;
		case 23:esi = 0xE1;break;
		case 24:esi = 0x69;break;
		case 25:break;
		default:;
		}
	}

	if (type == 0) 
	{
		return FALSE;
	}
	dwMemAddr = MEMADDR_KEYSETTING;
	if (IsBadReadPtr((LPVOID)dwMemAddr, 4) == TRUE) 
	{
		goto KEYSETTING_RETRY;
	}

	dwMemAddr = *(LPDWORD)dwMemAddr;

	dwMemAddr += 4;

	if (IsBadWritePtr((LPVOID)dwMemAddr, 0x200) == TRUE) 
	{
		goto KEYSETTING_RETRY;
	}

	*((LPBYTE)(dwMemAddr + esi)) = type;
	*((LPDWORD)(dwMemAddr + esi + 1)) = dwID;

	return TRUE;

KEYSETTING_RETRY:
	Sleep(1000);

	dwMemAddr = MEMADDR_KEYSETTING;
	if (IsBadReadPtr((LPVOID)dwMemAddr, 4) == TRUE) 
	{
		return FALSE;
	}
	dwMemAddr = *(LPDWORD)dwMemAddr;
	dwMemAddr += 4;

	if (IsBadWritePtr((LPVOID)dwMemAddr, 0x200) == TRUE) 
	{
		return FALSE;
	}

	*((LPBYTE)(dwMemAddr + esi)) = type;
	*((LPDWORD)(dwMemAddr + esi + 1)) = dwID;

	return TRUE;
}



DWORD GetBotSP(int nChangeJobNumber)
{
	DWORD dwMemAddr, dwBotMemAddr;
	DWORD dwReturn = 0;
	DWORD var_4;
	LPBYTE lpByte;
	DWORD ecx;


	dwBotMemAddr = MEMADDR_BOTINFO;
	if (IsBadReadPtr((LPVOID)dwBotMemAddr, 4) ==  FALSE)
	{
		dwBotMemAddr = *(LPDWORD)dwBotMemAddr;

		dwMemAddr = dwBotMemAddr + MEMADDR_BOTJOB_OFFSET;
		if (IsBadReadPtr((LPVOID)dwMemAddr, 4) == FALSE)
		{
			var_4 = *(LPDWORD)dwMemAddr;
			lpByte = (LPBYTE)(&var_4);
			ecx = (lpByte[1] ^ lpByte[3]) * 256 + (lpByte[0] ^ lpByte[2]);

			PrintLogA("GetBotSP: ecx = %d", ecx);

			if (ecx / 1000 == 3 || ecx / 100 == 22 || ecx == 2001 
				|| ecx / 100 == 27 // 루미너스
				|| ecx  / 100 == 13// 시그너스
				) 
			{
				dwMemAddr = dwBotMemAddr + MEMADDR_BOTSP_OFFSET2;
				DWORD pfnFunc = MEMADDR_BOTSP_FUNC1;

				__asm{
					pushad
						push	nChangeJobNumber
						mov		ecx, dwMemAddr
						call	pfnFunc
						movzx	ecx, al
						mov		dwReturn, ecx
						popad
				}
			}
			else
			{
				dwMemAddr = dwBotMemAddr + MEMADDR_BOTSP_OFFSET1;
				if (IsBadReadPtr((LPVOID)dwMemAddr, 8) == FALSE)
				{
					var_4 = *(LPDWORD)dwMemAddr;
					lpByte = (LPBYTE)(&var_4);
					dwReturn = (lpByte[1] ^ lpByte[3]) * 256 + (lpByte[0] ^ lpByte[2]);
				}
			}
		}
	}

	PrintLogA("GetBotSP: SP = %d", dwReturn);


	return dwReturn;
}



void GetWearingEquipIdArray()
{
	PrintLogA("장착템목록얻자");

	// 착용한 장비 아이디 묶음을 얻는 함수
	DWORD dwMemAddr, dwEquipId, dwMemAddr1; // edi@1
	int Index;
	int num = 0;

	dwMemAddr = MEMADDR_WEAREDEQUIPMENT;
	if (IsBadReadPtr((LPVOID)dwMemAddr, 4) == TRUE)
	{			
		return;
	}
	dwMemAddr1 = *(LPDWORD)dwMemAddr;

		dwMemAddr = dwMemAddr1 + MEMADDR_WEAREDEQUIPMENT_OFFSET;

		num = 0;
		if ( IsBadReadPtr((LPVOID)dwMemAddr, 0x80) == FALSE )
		{
			for (Index = 0; Index < 30; Index ++)
			{
				dwEquipId = *(LPDWORD)(dwMemAddr + 4 * Index);
				if (dwEquipId) 
				{
					//pEquipIdArray->Add(dwEquipId);
					PrintLogA("[%d] 아뒤 = %d", num++, dwEquipId);
				}
			}
		}
		PrintLogA("GetWearingEquipIdArray 옵셋=%x, 장착템개수 %d", MEMADDR_WEAREDEQUIPMENT_OFFSET, num);
}

void GetBuffSkillInfo(/*CDWordArray* pBuffSkillIdArray, CDWordArray* pBuffSkillRemainTimeArray*/)
{
	PrintLogA("==> GetBuffSkillInfo call");

	//DebugMessageA(100, "GetBuffSkillInfo enter");
	DWORD dwMemAddr, ebp, ebx, dwSkillId, dwRemainTime;
	dwMemAddr = MEMADDR_BUFFSKILLINFO;
	int nCnt = 0;

	if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) 
	{
		return;
	}

	ebp = *(LPDWORD)dwMemAddr;
	if (IsBadReadPtr((LPVOID)ebp, 4)) 
	{
		return;
	}

	if (ebp == 0 || ebp == 0x10) 
	{
		return;
	}

	do
	{
		dwMemAddr = ebp + 4;
		if (IsBadReadPtr((LPVOID)dwMemAddr, 4) == TRUE) 
		{
			break;
		}

		dwMemAddr = *(LPDWORD)dwMemAddr;
		ebx = dwMemAddr + MEMADDR_BUFFSKILLINFO_ID_OFFSET;
		if (IsBadReadPtr((LPVOID)ebx, 4))
		{
			break;
		}

		dwSkillId = *(LPDWORD)ebx;
		//pBuffSkillIdArray->Add(dwSkillId);

		dwMemAddr += MEMADDR_BUFFSKILLINFO_TIME_OFFSET;
		if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) 
		{
			break;
		}

		dwRemainTime = *(LPDWORD)dwMemAddr;
		//pBuffSkillRemainTimeArray->Add(dwRemainTime);
		PrintLogA("GetBuffSkillInfo[%d]: ID = %d, TIME = %d", nCnt++, dwSkillId, dwRemainTime);

		dwMemAddr = ebp-12;
		if (IsBadReadPtr((LPVOID)dwMemAddr, 4) == TRUE) 
		{
			break;
		}
		dwMemAddr = *(LPDWORD)dwMemAddr;

		if (IsBadReadPtr((LPVOID)dwMemAddr, 4)) 
		{
			break;
		}

		ebp = dwMemAddr + 16;
	}
	while (ebp != 0x10);
}