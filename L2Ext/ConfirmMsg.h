#pragma once
#include "ServerPacket.h"

class CConfirmMsg : public CServerPacket
{
	enum Type
	{
		ZONE_NAME = 7,
		SKILL_NAME = 4,
		ITEM_NAME = 3,
		NPC_NAME = 2,
		NUMBER = 1,
		TEXT = 0
	};
public:
	CConfirmMsg(int nMsgID)
	{
		Write((BYTE)0xED);
		Write((DWORD)nMsgID);		
	};
	void AddText(PWCHAR wsText)
	{
		Write((DWORD)TEXT);
		WriteS(wsText);
	};
	void AddNumber(int Value)
	{
		Write((DWORD)NUMBER);
		Write((int)Value);
	}
	void AddNpc(int NpcClassID)
	{
		Write((DWORD)NPC_NAME);
		Write((int)NpcClassID);
	}
	void AddItem(int ItemID)
	{
		Write((DWORD)ITEM_NAME);
		Write((int)ItemID);
	}
	void AddSkill(int nSkillID, int nLevel)
	{
		Write((DWORD)SKILL_NAME);
		Write((int)nSkillID);
		Write((int)nLevel);
	}
	void AddZone(int x, int y, int z)
	{
		Write((DWORD)ZONE_NAME);
		Write((int)x);
		Write((int)y);
		Write((int)z);
	}
	void SetTime(int miliseconds)
	{
		Write((int)miliseconds);
		Write(0);
	}

};