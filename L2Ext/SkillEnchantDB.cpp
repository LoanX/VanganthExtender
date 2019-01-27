#include "StdAfx.h"

SkillEnchantInfo* CSkillEnchantDB::GetSkillEnchantInfo(int nSkillId, int nLevel)
{
	typedef SkillEnchantInfo* (*f)(CSkillEnchantDB*, int, int);
	return f(0x0079E7A0L)(this, nSkillId, nLevel);
}

int CSkillEnchantDB::GetProbIndex(int nUserLevel)
{
	typedef int (*f)(int);
	return f(0x0079DB10L)(nUserLevel);
}

bool CSkillEnchantDB::IsLearnableLevel(int nUserLevel)
{
	typedef bool (*f) (int);
	return f(0x0079DA30L)(nUserLevel);
}
