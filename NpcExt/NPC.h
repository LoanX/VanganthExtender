#pragma once
#include "Creature.h"

//vt 6AEDD8h
class CNPC : public CCreature
{
public:
	static void ExtInit();
	static int IsNewbie(CNPC *pNpc, CSharedCreatureData *pTargetData);
	static void AddUseSkillDesire(CNPC* pNPC, CSharedCreatureData* pTargetData, int SkillNameID, int SkillType, int ActionMoveType, float desireValue);
};