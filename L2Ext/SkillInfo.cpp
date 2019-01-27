#include "StdAfx.h"
#include "SkillInfo.h"

void CSkillInfo::ActivateSkill(CObject *pCreature, CObject *pTarget, double dUnkn, INT64 nUnkn)
{
	typedef void (*f)(CSkillInfo*, CObject*, CObject*, double, INT64);
	f(0x00799000L)(this, pCreature, pTarget, dUnkn, nUnkn);
}

bool CSkillInfo::IsMeleeAtk()
{
	if(this && this->nIsMagic == 0 && this->nCastRange < 100 && this->nEffectPoint < 0)
		return true;
	else
		return false;
}

bool CSkillInfo::IsSelfBuff()
{
	if(this)
	{
		if(targetType == CSkillInfo::TargetSelf && AffectScope == CSkillInfo::AS_SINGLE)
		{
			return true;
		}
	}
	return false;
}

bool CSkillInfo::IsSongOrDance()
{
	if(this)
	{
		if(nAbnormalType > 63 &&  nAbnormalType < 78)
		{
			//song
			return true;

		}else if(nAbnormalType > 77 && nAbnormalType < 91)
		{
			//dance
			return true;
		}
	}
	return false;
}

bool CSkillInfo::CanUsedBy(CCreature *pCreature)
{
	typedef bool(*f)(CSkillInfo*, CCreature*);
	return f(0x798960L)(this, pCreature);
}

//CSkillDB
CSkillDB::CSkillDB()
{
	pSkillDB = (CSkillDB*)0x0E4172C8;
}

CSkillInfo *CSkillDB::GetSkillInfo(int nSkillID, int nLevel)
{
	typedef CSkillInfo* (*f)(CSkillDB*, CSkillKey*);
	CSkillKey c(nSkillID, nLevel);
	return f(0x79A580L)(pSkillDB, &c);
}

void CSkillDB::GetSkillIdLevelByName(const WCHAR* wName, LPINT Id, LPINT Level)
{
	typedef void (*f)(CSkillDB*, const WCHAR*, LPINT, LPINT);
	f(0x79AAD0)(pSkillDB, wName, Id, Level);
}