#pragma once

class CSkillOperateCondition
{
public:
	virtual bool CanUse(CCreature *pCreature, CObject *pTarget) = 0;
};