#include "StdAfx.h"
#include "CAttackAction.h"

CCreature* CAttackAction::GetAttacker()
{
	typedef CCreature*(*f)(PUINT);
	return f(0x07F07F0L)(&attackerId);
}

CObject* CAttackAction::GetTarget()
{
	if(targetId)
	{
		typedef CObject*(*f)(PUINT);
		return f(0x07F07F0L)(&targetId);
	}
	return 0;
}

void CAttackAction::CheckItemSkill(AttackTargetDataType *pData)
{
	typedef void (*_cis)(CAttackAction*, AttackTargetDataType *pData);
	_cis cis = (_cis) 0x00696A00;
	return cis(this, pData);
}