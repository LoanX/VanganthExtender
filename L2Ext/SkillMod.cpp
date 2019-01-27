#include "StdAfx.h"

CSkillMod::CSkillMod()
{ 
	Clear(); 
};

CSkillMod::~CSkillMod()
{
}

void CSkillMod::Clear()
{
	try
	{
		nAvoidMeleeSkill = 0;
		nCancelAttackerTarget = 0;
		nReflectMeleeSkillDmg = 0;
		nSelfResurrection = 0;
		nEnlargeBuffSlots = 0;
		nDecreaseLostExp = 0;
		nBlockIconAction = 0;
		nImproveHeal = 0;
		nAntiBuff = 0;
		fDiggerMultipler = 1;
		fSongDanceMpMultipler = 1;
		fSongDanceReuseMultipler = 1;
		semiInvisible = FALSE;
		bonusExp = 1.0;
		bonusSp = 1.0;
		bonusItemDrop = 1.0;
		bonusItemDropOnlyChance = 1.0;
		bonusAdena = 1.0;
		bonusSpoil = 1.0;
		bonusHarvesting = 1.0;
		bonusRb = 1.0;
		bonusRbOnlyChance = 1.0;
		bonusEpicRb = 1.0;
		resistStun = 1.0;
		regenCpMul = 1.0;
		regenCpAdd = 0;
	}catch(...)
	{
		EXCEPTION_LOG;
	}
}