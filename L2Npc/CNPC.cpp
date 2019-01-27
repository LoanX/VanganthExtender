#include "L2NPC.h"

void CNPC::OverrideFunctions()
{
	WriteHook(0xE9, 0x4C4A43, (DWORD)CNPC::IsNewbie2, 0);
	WriteHook(0xE8, 0x4C2921, (DWORD)CNPC::AddUseSkillDesire, 0);
}

int CNPC::GetTimeHour()
{
	typedef int (*f)();
	return f(0x4E69C0L)();
}

CNPC *CNPC::GetObject(int nObjectID)
{
	typedef CNPC* (*f)(int);
	return f(0x4CC6F0L)(nObjectID);
}

bool CNPC::IsNewbie2(CNPC* pNPC, CreatureSharedData* pSD)
{
	if(pSD)
	{
		if(pSD->nLevel < 25)
			return true;
	}
	return false;
}

void CNPC::AddUseSkillDesire(CNPC* pNPC, CreatureSharedData* pSD, int SkillNameID, int SkillType, int ActionMoveType, float value)
{
	typedef void (*f)(CNPC*, CreatureSharedData*, int, int, int, float);
	try
	{
		int nSkillID = SkillNameID >> 8;
		int nLevel = SkillNameID - (nSkillID << 8);
		if(SkillNameID < 1 || nSkillID > 65535 || nSkillID < 1)
		{
			g_Log.Add(CLog::Error, "[%s] Npc[%S] Trying to use invalid skill_name_id[%d]", __FUNCTION__, pNPC->pSD->wszName, SkillNameID);
		}
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception!", __FUNCTION__);
	}
	f(0x4E1030L)(pNPC, pSD, SkillNameID, SkillType, ActionMoveType, value);
}