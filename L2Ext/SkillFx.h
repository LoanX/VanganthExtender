#pragma once
#include "MagicalShieldDefense.h"

extern "C"
{
	void p_reflect_skill();
	void ChargeFix();
	void i_heal_hook();
	double GetHealEffect(CCreature *pCaster, CCreature *pTarget);
	void LookNeighborByNPCFix();
	void LookNeighborFix1();
}

struct EffectStruct
{
	LPVOID lpVTable;
	LPVOID lpUnkn;
	double fValue;
	DWORD nChangeType;	//PER/DIFF
	DWORD nSkillType;	//0 - skill 1- magic 2- other 3- dance/song
};

class CSkillEffect;

class CSkillFx
{
public:
	static void Initialize();
	static void UpdateAttackRange(CCreature *pCreature);
	static CSummon *CheckRangeInTransferPain(User *pUser);
	static double GetAttributeBonus(CCreature *pCaster, CCreature *pTarget);
	static void PerformCounterAttack(User *pUser, CCreature *pAttacker, CSkillInfo *pSI);
	static bool PerformAvoidAttack(User *pUser, CCreature *pAttacker);
	static bool DirectToHpCheck(CCreature *pCreature);
	static void BroadcastAutoCastSkill(int nObjectID, int nTargetID, int nSkillID, int nLevel, FVector *pPos);
	static void CSkillEffect_p_magic_mp_cost_Pump(EffectStruct* pEffect, CCreature *pCreature);
	static void ResurrectionAskFix(CUserSocket *pSocket, const char* format, BYTE opCode, int msgId, int paramAmount, int paramTypeString, const WCHAR* wName, int paramTypeNumber, int resurrectedExp);
	static void CSkillEffect_i_resurrection_Instant(CSkillEffect *pEffect, CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	static bool AvoidAgroSummonPetFix(CObject *pObject);
	static void CSkillEffect_i_hp_per_max_Instant(PVOID pEffect, CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	static void CSkillEffect_i_heal_Instant(PVOID pEffect, CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance);
	static bool OnCreatureValidateCriticalProb(CCreature *pCreature);
	static bool OnCreatureValidateAttackSpeed(CCreature *pCreature);
	static bool OnCreatureValidateMUseSpeed(CCreature *pCreature);

};