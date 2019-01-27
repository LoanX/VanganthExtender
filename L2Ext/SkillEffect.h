#pragma once

class CCreature;
class CSkillInfo;
class CSkillAction;

class CSkillEffect
{
public:
	enum ChangeType
	{
		PER = 0,
		DIFF = 1
	};
protected:
	LPVOID pPointer;
public:
/* 00h */	virtual bool CalcSuccess(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction) = 0; // pure
/* 08h */	virtual void Instant(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance) = 0; // pure
/* 10h */	virtual void Unk() = 0; // pure
/* 18h */	virtual void Tick() = 0; // pure
/* 20h */	virtual double Unk2() = 0; // pure
/* 28h */	virtual bool CheckPumpCondition(CCreature* pTarget) = 0; // pure
/* 30h */	virtual void Pump(CCreature* pTarget, CSkillInfo* pSkillInfo) = 0; // pure
/* 38h */	virtual bool Consume(CCreature *pCreature, double param2) = 0; // pure
/* 40h */	virtual bool CheckResisted() = 0; // pure
};