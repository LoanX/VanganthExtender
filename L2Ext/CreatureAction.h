#pragma once
#include "MoveAction.h"

class CreatureAction
{
public:
	static void Initialize();
	static void OnInitializeSkillMod(CCreature *pCreature);
	static bool OnInfectedSkillBy(CCreature *pCreature, CCreature *pSource, CSkillInfo *pInfo);
	static double OnGotDamageBy(CCreature *pCreature, CCreature *pAttacker, double Damage, int nDmgType, bool DmgFlag);
	static void OnActivateSkill(CSkillInfo *pSI, CCreature *pCreature, CCreature *pTarget, double dUnkn, INT64 nUnkn);
	static bool OnValidateSetItem(CCreature *pCreature, int nSlotType);
	static void OnValidateAttachedSkills(CCreature *pCreature);
	static CCreature *OnDoNothing(LPUINT pSID);
	static void OnTimerExpired(CCreature *pCreature, int nTimerID);
	static void OnBroadcastMagicSkillLanuched(LPVOID pVoid, UINT nObjectID, int Dist, FVector *Pos, int Dist2, const char *format, ...);
	static void OnAttackCheckItemSkill(CAttackAction *pAction, AttackTargetDataType *pTargetData);
	static CCreature *OnCreatureConstruction(CCreature *pCreature);
	static CCreature *OnCreatureConstructionOther(CCreature *pCreature, CCreature &pOther);
	static void OnCreatureDestruction(CCreature *pCreature);
	static double OnAttackedBy(CCreature *pCreature, CCreature *pAttacker, double Damage, int nDmgType, int nUnkn);
	static bool OnValidateBaseData(CCreature *pCreature);
	static bool OnIsShieldDefend(CCreature *pCreature, CCreature *pAttacker, double CriticalBonus);
	static DWORD* OnGetSkillUsedTime(CCreature *pCreature, int SkillID, int Level);
	static void OnSetSkillUsableTime(CCreature *pCreature, int nSkillID, int nLevel, DWORD *stamp, DWORD Reuse);
	static void OnSetSkillUsableTime2(CCreature *pCreature, int nSkillID, int nLevel, DWORD *stamp, DWORD Reuse);
	static void OnSetItemUsableTime(CCreature *pCreature, int ItemClassID, DWORD* stamp, DWORD Reuse);
	static DWORD *OnGetItemUsedTime(CCreature *pCreature, int ItemClassID);
	static bool IsValidTarget(CSkillInfo *pSI, CCreature *pSkillUser, CObject *pTarget, bool bForce);
	static void OnDeleteAbnormalStatus(CreatureSharedData *pSD, int nAbnormalType);
	static int OnUpdateSP(CCreature* pCreature, int SP);
	static int ExpInc(CCreature *pCreature, int exp, int affectKarma);
	static bool UseItem(CCreature *pCreature, CItem *pItem, BOOL force);
	static bool OnDeleteItemInInventory(CCreature *pCreature, INT32 itemId, INT32 count);
	static void OnIHarvestingEffect(LPVOID lpEffect, CCreature *pCaster, CCreature *pTarget, CSkillInfo *pSI, CSkillAction *pAction, double distance);
	static void ActionBroadcastSkillCancelled(CCreatureController *pCC, CSkillAction *pAction);
	static void ActionBroadcastSkillLanuched(CCreatureController *pCC, int skillId, int level, LPVOID pContainer);
	static void ActionBroadcastSkillLanuchedEx(CCreatureController *pCC, int skillId, int level, UINT targetId);
	static void TurnOffSoulshot(CCreature *pCreature);
	static void TurnOffSpiritshot(CCreature *pCreature);
	static void SkillAction(CCreature *pCreature, int skillId, bool force, bool inPlace, CItem *pItem);
	static bool CheckAttackValidity(CAttackAction *pAction, CCreature *pActor, CObject *pTarget);
	
	static bool AttackActionOnStart(CAttackAction *pAction);
	static bool SkillActionOnStart(CSkillAction *pAction);
	static bool SkillActionOnTick(CSkillAction *pAction);
	static bool GatherIsNotActive(CCreature *pCreature);
	static bool GatherIsActive(CCreature *pCreature);
	static bool MoveActionOnStart(CMoveAction *pAction);
	static bool MoveActionExOnStart(CMoveActionEx *pAction);
};