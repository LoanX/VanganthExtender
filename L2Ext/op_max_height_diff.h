#pragma once
#include "CSkillOperateCondition.h"

class CSkillOperateCondition_op_max_height_diff: public CSkillOperateCondition
{
	int nDiff;
public:
	virtual bool CanUse(CCreature *pCreature, CObject *pTarget);
};