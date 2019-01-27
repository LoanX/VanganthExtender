#pragma once

#include "ServerPacket.h"

class CMagicSkillUse : public CServerPacket
{
public:
	CMagicSkillUse(int nCasterSID, int targetId, int nSkillID, int nLevel, int nHitTime, int nReuseDelay, int x, int y, int z)
	{
		Write((BYTE)0x48);
		Write((DWORD) nCasterSID);
		Write((DWORD) targetId);
		Write((DWORD) nSkillID);
		Write((DWORD) nLevel);
		Write((DWORD) nHitTime);
		Write((DWORD) nReuseDelay);
		Write((DWORD) x);
		Write((DWORD) y);
		Write((DWORD) z);
		Write((WORD) 0);
		Write((WORD) 0);
		Write((WORD) 0);
		Write((WORD) 0);
	};
};