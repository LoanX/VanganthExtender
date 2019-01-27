#include "stdafx.h"
#include "NPC.h"
#include "SharedCreatureData.h"


bool GetObject_IsNpcFix(CNPC *pNpc)
{
	try
	{
		UINT64 addr = *(PUINT64)pNpc;	//vt
		addr += 0xE0;
		addr = *(PUINT64)addr;	//IsNpc
		typedef bool (*f)(CNPC*);
		return f(addr)(pNpc);
	}catch(...)
	{
	}
	return false;
}

void CNPC::ExtInit()
{
	WriteCall(0x4CC863L, GetObject_IsNpcFix, 4);
	WriteJump(0x4C4A43, (UINT64)CNPC::IsNewbie);	//Fix IsNewbie func
	WriteCall(0x4C2921, CNPC::AddUseSkillDesire, 0);	//Added log for invalid skill pch id
}

int CNPC::IsNewbie(CNPC *pNpc, CSharedCreatureData *pTargetData)
{
	if(pTargetData)
	{
		if(pTargetData->nLevel < 25)
		{
			return 1;
		}
	}else
	{
		g_Log.Add(LogError, "[%s] Null target data - NPC[%S][%d]!", __FUNCTION__, pNpc->pSD->wszName, pNpc->pSD->nNpcClassID);
	}
	return 0;
}

void CNPC::AddUseSkillDesire(CNPC* pNpc, CSharedCreatureData* pTargetData, int SkillNameID, int SkillType, int ActionMoveType, float desireValue)
{
	if(pTargetData)
	{
		int nSkillID = SkillNameID >> 8;
		int nLevel = SkillNameID - (nSkillID << 8);
		if(SkillNameID < 1 || nSkillID > 65535 || nSkillID < 1)
		{
			g_Log.Add(LogError, "[%s] Npc[%S] Trying to use invalid skill_name_id[%d] - check AI or NpcData", __FUNCTION__, pNpc->pSD->wszName, SkillNameID);
		}
		typedef void (*f)(CNPC*, CSharedCreatureData*, int, int, int, float);
		f(0x4E1030L)(pNpc, pTargetData, SkillNameID, SkillType, ActionMoveType, desireValue);
	}else
	{
		g_Log.Add(LogError, "[%s] Null target data - NPC[%S][%d]!", __FUNCTION__, pNpc->pSD->wszName, pNpc->pSD->nNpcClassID);
	}
}