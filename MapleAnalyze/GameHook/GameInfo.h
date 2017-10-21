// GameInfo.h
#pragma once

#include "../_common/patch.h"


DWORD Descramble(LPDWORD pEax);
CString GetBotJob();
void GetBotPosition(POINT &pBotPoint);



BOOL KeySetting(DWORD dwID /*arg_0*/, BYTE key /*al*/, BYTE type /*bl*/);
DWORD GetBotSP(int nChangeJobNumber);

void GetWearingEquipIdArray();

void GetBuffSkillInfo();