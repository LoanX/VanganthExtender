#pragma once

extern "C"
{
	void CheckBuffAmount();
	int GetMaxAbnormals(CCreature *pCreature, CSkillInfo *pSI);
}

class BuffSlots
{
public:
	static void Initialize();
	static int GetMax(CCreature *pCreature, CSkillInfo *pSI);
};