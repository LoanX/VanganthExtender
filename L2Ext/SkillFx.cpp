#include "StdAfx.h"
#include "Geo.h"
#include "SkillFx.h"

extern vector<INT32> g_HealResistNpc;
INT32 g_TransferPainRange = 600;

double g_MaxCriticalRate = 50.0;
double g_MaxCastingSpeed = 0.0;
double g_MaxAttackSpeed = 0.0;

vector<INT32> g_DispelDebuffException;

void CSkillFx::Initialize()
{
	//disable old p_block_buff/debuff check
	g_HookManager.WriteJump(0x5053DF, reinterpret_cast<PVOID>(0x505444), 2);
	//Fix for p_reflect_skill (to not reflect buffs)
	g_HookManager.WriteJump( 0x00799208, p_reflect_skill, 1);
	//Fix Charge System
	g_HookManager.WriteJump( 0x6B8BB6, ChargeFix, 3);
	//Improve Heal effect
	g_HookManager.WriteJump( 0x7A820B, i_heal_hook, 3);
	//Attack Range - Snipe/RapidFire 0x4FFE73
	g_HookManager.WriteCall( 0x4FFE73, UpdateAttackRange, 0);
	//TransferPain - 600 Range Fix
	g_HookManager.WriteCall( 0x514757, CheckRangeInTransferPain, 0);
	//Fix User::Ride / User::GetOff - removing scroll/potions when riding/unriding a strider
	g_HookManager.NopMemory(0x835187, 5);
	g_HookManager.NopMemory(0x835B07, 5);
	//Fix for Holy Weapon etc
	g_HookManager.WriteCall( 0x7A87C5, GetAttributeBonus, 0);
	g_HookManager.WriteCall( 0x7A8F31, GetAttributeBonus, 0);
	g_HookManager.WriteCall( 0x7A979A, GetAttributeBonus, 0);
	g_HookManager.WriteCall( 0x7AA008, GetAttributeBonus, 0);
	g_HookManager.WriteCall( 0x7B261A, GetAttributeBonus, 0);
	//Sub Systems
	CMagicalShieldDefense::Initialize();
	//Direct To Hp fix
	g_HookManager.WriteCall(0x514EE3, DirectToHpCheck, 1);
	g_HookManager.WriteCall(0x514DDF, DirectToHpCheck, 1);
	//Resurrection fix
	g_HookManager.WriteCall(0x7AB866, CSkillFx::ResurrectionAskFix);
	g_HookManager.WriteMemoryDWORD(0xAD7FC0, (DWORD)CSkillFx::CSkillEffect_i_resurrection_Instant);

	//fixed p_magic_mp_cost effect
	g_HookManager.WriteMemoryDWORD(0xADB528, (DWORD)CSkillEffect_p_magic_mp_cost_Pump);
	g_HookManager.NopMemory(0x7AE6E2, 5);	//i_run_away msg nop

	g_HookManager.WriteJump(0x4F8012, LookNeighborByNPCFix, 29);
	g_HookManager.WriteJump(0x4F728A, LookNeighborFix1, 20);

	//p_avoid_agro fix to affect pet/summon
	g_HookManager.WriteCall(0x7BE944, CSkillFx::AvoidAgroSummonPetFix, 4);
	g_HookManager.WriteMemoryDWORD(0xAD80E0, (DWORD)CSkillFx::CSkillEffect_i_hp_per_max_Instant);
	g_HookManager.WriteMemoryDWORD(0xAD7C00, (DWORD)CSkillFx::CSkillEffect_i_heal_Instant);

	g_TransferPainRange = GetPrivateProfileInt(_T("SkillFactory"), _T("TransferPainRange"), 600, g_ConfigFile);
	TCHAR temp[0x4000] = { 0 };
	if(GetPrivateProfileString(_T("SkillFactory"), _T("DispelDebuffExceptionList"), 0, temp, sizeof(temp), g_ConfigFile))
	{
		wstringstream skillStream(temp);
		int skillId = 0;
		while(skillStream >> skillId)
		{
			g_DispelDebuffException.push_back(skillId);
		}
	}
	g_MaxCriticalRate = GetPrivateProfileDouble(_T("StatLimiter"), _T("MaxCriticalRate"), 500.0, g_ConfigFile);
	g_MaxCriticalRate /= 10.0;
	g_MaxAttackSpeed = GetPrivateProfileDouble(_T("StatLimiter"), _T("MaxAttackSpeed"), 0.0, g_ConfigFile);
	g_MaxCastingSpeed = GetPrivateProfileDouble(_T("StatLimiter"), _T("MaxCastingSpeed"), 0.0, g_ConfigFile);
	if(g_MaxCastingSpeed > 0.0)
	{
		g_MaxCastingSpeed /= 333.0;
	}
	
	
//	g_HookManager.WriteMemoryDWORD(0x989378, (UINT)CSkillFx::OnCreatureValidateCriticalProb);
//	g_HookManager.WriteMemoryDWORD(0x9B0CB8, (UINT)CSkillFx::OnCreatureValidateCriticalProb);
//	g_HookManager.WriteMemoryDWORD(0xA1AFB8, (UINT)CSkillFx::OnCreatureValidateCriticalProb);
//	g_HookManager.WriteMemoryDWORD(0xA28DD8, (UINT)CSkillFx::OnCreatureValidateCriticalProb);
//	g_HookManager.WriteMemoryDWORD(0xA4AB08, (UINT)CSkillFx::OnCreatureValidateCriticalProb);
//	g_HookManager.WriteMemoryDWORD(0xA4B478, (UINT)CSkillFx::OnCreatureValidateCriticalProb);
	//affect user only
	g_HookManager.WriteMemoryDWORD(0xA875B8, (UINT)CSkillFx::OnCreatureValidateCriticalProb);
//	g_HookManager.WriteMemoryDWORD(0xAA0DC8, (UINT)CSkillFx::OnCreatureValidateCriticalProb);

//	g_HookManager.WriteCall(0x6BE15B, CSkillFx::OnCreatureValidateAttackSpeed);
//	g_HookManager.WriteMemoryDWORD(0x9B0D38, (UINT)CSkillFx::OnCreatureValidateAttackSpeed);
	g_HookManager.WriteMemoryDWORD(0xA87638, (UINT)CSkillFx::OnCreatureValidateAttackSpeed);

//	g_HookManager.WriteMemoryDWORD(0x989360, (UINT)CSkillFx::OnCreatureValidateMUseSpeed);
//	g_HookManager.WriteMemoryDWORD(0x9B0CA0, (UINT)CSkillFx::OnCreatureValidateMUseSpeed);
//	g_HookManager.WriteMemoryDWORD(0xA1AFA0, (UINT)CSkillFx::OnCreatureValidateMUseSpeed);
//	g_HookManager.WriteMemoryDWORD(0xA28DC0, (UINT)CSkillFx::OnCreatureValidateMUseSpeed);
//	g_HookManager.WriteMemoryDWORD(0xA4AAF0, (UINT)CSkillFx::OnCreatureValidateMUseSpeed);
//	g_HookManager.WriteMemoryDWORD(0xA4B460, (UINT)CSkillFx::OnCreatureValidateMUseSpeed);
	g_HookManager.WriteMemoryDWORD(0xA875A0, (UINT)CSkillFx::OnCreatureValidateMUseSpeed);
//	g_HookManager.WriteMemoryDWORD(0xAA0DB0, (UINT)CSkillFx::OnCreatureValidateMUseSpeed);
	
}

bool CSkillFx::OnCreatureValidateMUseSpeed(CCreature *pCreature)
{
	CTL;
	if(pCreature)
	{
		typedef bool (*f)(CCreature*);
		f(0x4E41D0L)(pCreature);	//CCreature::ValidateMUseSpeed

		if(g_MaxCastingSpeed > 0)
		{
			if(pCreature->pSD->castingSpeed > g_MaxCastingSpeed)
			{
				pCreature->pSD->castingSpeed = g_MaxCastingSpeed;
			}
		}
	}

	return true;
}

bool CSkillFx::OnCreatureValidateAttackSpeed(CCreature *pCreature)
{
	CTL;
	if(pCreature)
	{
		typedef bool (*f)(CCreature*);
		f(0x4DD0A0L)(pCreature);	//CCreature::ValidateAttackSpeed

		if(g_MaxAttackSpeed > 0)
		{
			if(pCreature->pSD->attackSpeed > g_MaxAttackSpeed)
			{
				pCreature->pSD->attackSpeed = g_MaxAttackSpeed;

				UINT64 funcAddr = (*(PUINT64)pCreature);
				funcAddr = (*(PUINT64)(funcAddr+0x3D8));
				return f(funcAddr)(pCreature);
			}
		}
	}

	return true;
}

bool CSkillFx::OnCreatureValidateCriticalProb(CCreature *pCreature)
{
	CTL;
	if(pCreature)
	{
		typedef double (*f)(CCreature*);

		UINT64 funcAddr = (*(PUINT64)pCreature);
		funcAddr = (*(PUINT64)(funcAddr+0x650));	//get dex bonus

		UINT64 funcAddrEx = (*(PUINT64)pCreature);
		funcAddrEx = (*(PUINT64)(funcAddrEx+0x318));	//equipped weapon crit rate

		double critRate = f(funcAddr)(pCreature);
		critRate *= f(funcAddrEx)(pCreature);

		critRate *= pCreature->criticalRatePER;
		critRate += pCreature->criticalRateDIFF;

		if(critRate > g_MaxCriticalRate)
		{
			pCreature->pSD->criticalRate = g_MaxCriticalRate;
		}else
		{
			pCreature->pSD->criticalRate = critRate;
		}
	}
	return true;
}

void CSkillFx::CSkillEffect_i_heal_Instant(PVOID pEffect, CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	guard;

	if(pTarget->ValidCreature())
	{
		if(User *pUser = pCreature->SafeCastUser())
		{
			if(pTarget->pSD->nIsPC == 0)
			{
				for(UINT n=0;n<g_HealResistNpc.size();n++)
				{
					if(pTarget->pSD->nNpcClassID == g_HealResistNpc[n])
					{
						unguard;
						return;
					}
				}
			}
		}
	}

	typedef void(*f)(PVOID, CCreature*, CCreature*, CSkillInfo*, CSkillAction*, double);
	f(0x7A7F70L)(pEffect, pCreature, pTarget, pSkillInfo, pAction, distance);

	unguard;
}

void CSkillFx::CSkillEffect_i_hp_per_max_Instant(PVOID pEffect, CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	guard;
	//check for celestial shield (block_hp)
	if(pTarget->ValidCreature())
	{
		if(pTarget->canDamageHP == false)
		{
			unguard;
			return;
		}
		if(User *pUser = pCreature->SafeCastUser())
		{
			if(pTarget->pSD->nIsPC == 0)
			{
				for(UINT n=0;n<g_HealResistNpc.size();n++)
				{
					if(pTarget->pSD->nNpcClassID == g_HealResistNpc[n])
					{
						unguard;
						return;
					}
				}
			}
		}
	}

	typedef void(*f)(PVOID, CCreature*, CCreature*, CSkillInfo*, CSkillAction*, double);
	f(0x7AB970L)(pEffect, pCreature, pTarget, pSkillInfo, pAction, distance);

	unguard;
}

void CSkillFx::ResurrectionAskFix(CUserSocket *pSocket, const char* format, BYTE opCode, int msgId, int paramAmount, int paramTypeString, const WCHAR* wName, int paramTypeNumber, int resurrectedExp)
{
	pSocket->Send("cdddSdd", opCode, msgId, paramAmount, paramTypeString, wName, paramTypeNumber, resurrectedExp);
	//pSocket->Send("cdddSdddd", opCode, msgId, paramAmount, paramTypeString, wName, paramTypeNumber, resurrectedExp, 30000, 0);
}

void CSkillFx::CSkillEffect_i_resurrection_Instant(CSkillEffect *pEffect, CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	guard;

	if(User *pUser = pCreature->SafeCastUser())
	{
		if(pTarget->ValidUser() || pTarget->IsPet())
		{
			if(pTarget->pSD->nAlive == 0)
			{
				if(pTarget->pSD->nDuringRessurection)
				{
					pUser->SendSystemMessage(L"You cannot resurrect your target until he accepts or rejects previous resurrection!"); 
					unguard;
					return;
				}
				if(pTarget->pSD->bGuilty || pTarget->pSD->nKarma)
				{
					if(pUser->pSD->bGuilty == 0)
					{
						pUser->SetPvPFlag();
						pUser->SetRelationChangeRequest(1);
						pUser->SetCharInfoChanged();
						pUser->SetUserInfoChanged();
					}
				}

				typedef void(*f)(CSkillEffect*, CCreature*, CCreature*, CSkillInfo*, CSkillAction*, double);
				f(0x7AB250L)(pEffect, pCreature, pTarget, pSkillInfo, pAction, distance);
			}

		}
	}

	unguard;
}

bool CSkillFx::AvoidAgroSummonPetFix(CObject *pObject)
{
	bool ret = false;
	if(pObject->IsValid(VT_CSUMMON) || pObject->IsValid(VT_CPET))
	{
		ret = true;
	}else
	{
		UINT64 addr = (*(PUINT64)pObject);
		addr += 0xD0;
		addr = (*(PUINT64)addr);
		typedef bool (*f)(CObject*);
		ret = f(addr)(pObject);
	}
	return ret;
}

bool CSkillFx::DirectToHpCheck(CCreature *pCreature)
{
	typedef bool (*_func)(CCreature*);
	INT64 nAddr = *(INT64*)(*(INT64*)pCreature + 0x28);
	_func isUser = (_func) nAddr;
	if(pCreature->ValidCreature())
	{
		if(pCreature->pSD->pExData->AttackDirectToHp)
		{
			pCreature->pSD->pExData->AttackDirectToHp = false;
			return false;
		}
	}
	return isUser(pCreature);
}

void CSkillFx::UpdateAttackRange(CCreature *pCreature)
{	
	if(pCreature->ValidCreature())
	{
		pCreature->ValidateBaseCP();
		pCreature->ValidateAllOnWeapon();
	}
}
CSummon* CSkillFx::CheckRangeInTransferPain(User *pUser)
{
	if(pUser->ValidUser())
	{
		CSummon *pSummon = pUser->GetSummonOrPet();
		if(pSummon->IsSummon())
		{
			if(g_TransferPainRange > 0)
			{
				if(pUser->CheckDistance(pSummon, g_TransferPainRange))
				{
					return pSummon;
				}
			}else
			{
				return pSummon;
			}
		}else
			return 0;
	}
	return NULL;
}
double CSkillFx::GetAttributeBonus(CCreature *pCaster, CCreature *pTarget)
{
	return 1;
}

void CSkillFx::PerformCounterAttack(User *pUser, CCreature *pAttacker, CSkillInfo *pSI)
{
	if(pUser->ValidUser() && pAttacker->ValidCreature())
	{
		try
		{
			if(pUser->pSD->nAlive && pUser->pSD->pExData)
			{
				int nRnd = rand() % 100;
				if(nRnd < pUser->pSD->pExData->SkillMod.nReflectMeleeSkillDmg)
				{				
					//Send sys messages
					CSystemMessage msg(SystemMessage::YOU_COUNTERED_$C1_S_ATTACK_);
					if(pAttacker->pSD->nIsPC)
						msg.AddText(pAttacker->pSD->wszName);
					else
						msg.AddNpc(pAttacker->pSD->nNpcClassID);
					pUser->SendSystemMessage(&msg);
					if(pAttacker->ValidUser())
					{
						CSystemMessage msg2(SystemMessage::$C1_IS_PERFORMING_A_COUNTERATTACK_);
						msg2.AddText(pUser->pSD->wszName);
						pAttacker->GetUser()->SendSystemMessage(&msg2);
					}
					pAttacker->pSD->bGuilty = true;
					//Counterattack
					//((PATK+5898 )*70)/PDEF
					double pAtk = pUser->pSD->fPAtk + 5898;
					if(pUser->pSD->dSoulshotPower > 0)
						pAtk *= 2;
					double pDef = pAttacker->pSD->fPDef;
					double Dmg = pAtk * 70 / pDef;
					
					typedef double (*gdb)(CCreature*, CCreature*, double, int, bool);

					gdb(0x513970L)(pAttacker, pUser, Dmg, 1, true);
					if(pAttacker->pSD->nAlive)
					{
						switch(pSI->nSkillID)
						{
						case 16:	//Mortal Blow
						case 30:	//Backstab
						case 263:	//Deadly Blow
						case 344:	//Lethal Blow
							{
								gdb(0x513970L)(pAttacker, pUser, Dmg, 1, true);
								break;
							}
						case 321:	//Blinding Blow
							{
								gdb(0x513970L)(pAttacker, pUser, Dmg, 1, true);
								if(pAttacker->pSD->nAlive)
									gdb(0x513970L)(pAttacker, pUser, Dmg, 1, true);
								break;
							}
						default:
							{
								break;
							}
						}
					}
				}
			}
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception Detected.", __FUNCTION__);
		}
	}
}
bool CSkillFx::PerformAvoidAttack(User *pUser, CCreature *pAttacker)
{
	if(pUser->ValidUser() && pAttacker->ValidCreature())
	{
		int nRnd = rand() % 100;
		try
		{
			if(nRnd < pUser->pSD->pExData->SkillMod.nAvoidMeleeSkill)
			{
				if(pAttacker->ValidUser())
				{
					CSystemMessage msg(SystemMessage::$C1_DODGES_THE_ATTACK_);
					msg.AddText(pUser->pSD->wszName);
					pAttacker->GetUser()->SendSystemMessage(&msg);
				}
				
				CSystemMessage msg2(SystemMessage::YOU_HAVE_AVOIDED_$C1_S_ATTACK_);
				if(pAttacker->pSD->nIsPC)
					msg2.AddText(pAttacker->pSD->wszName);
				else
					msg2.AddNpc(pAttacker->pSD->nNpcClassID);
				pUser->SendSystemMessage(&msg2);
				return true;
			}
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception Detected.", __FUNCTION__);
		}
	}
	return false;
}

void CSkillFx::BroadcastAutoCastSkill(int nObjectID, int nTargetID, int nSkillID, int nLevel, FVector *pPos)
{
	CHAR pck[8190];
	int nSize = Assemble(pck, 512, "cdddddddddhhhh", 0x48, nObjectID, nTargetID, nSkillID, nLevel, 0, 0, static_cast<int>(pPos->x), static_cast<int>(pPos->y), static_cast<int>(pPos->z), 0, 0, 0, 0);
	L2Server::BroadcastToNeighborDist(nObjectID, pPos, 1500, nSize, pck);
}

int AssembleTarget(CCreature *pCreature, PCHAR packet, int nSizeLeft, const char *format, UINT targetId, int nSkillID)
{
	CSkillInfoEx *pData = g_SkillDBEx.GetSkillInfoEx(nSkillID, 0);
	if(pData)
	{
		if(pData->geoDataTest)
		{
			if(pCreature->ValidCreature())
			{
				CCreature *pTarget = CCreature::GetCreatureBySID(&targetId);
				if(pTarget->ValidCreature())
				{
					if(CGeo::GetInstance()->CanSee(&pCreature->pSD->Pos, &pTarget->pSD->Pos))
					{
						return Assemble(packet, nSizeLeft, format, targetId);
					}
				}
			}else
				g_Log.Add(CLog::Error, "[%s] Invalid Creature", __FUNCTION__);
		}else
		{
			return Assemble(packet, nSizeLeft, format, targetId);
		}
	}else
	{
		return Assemble(packet, nSizeLeft, format, targetId);
	}
	return 0;
}

double GetHealEffect(CCreature *pCaster, CCreature *pTarget)
{
	double dReturn = 1;
	if(pCaster->ValidCreature() && pTarget->ValidCreature())
	{
		dReturn = pTarget->healEffect;
		if(pCaster->pSD->pExData)
		{
			if(pCaster->pSD->pExData->SkillMod.nImproveHeal)
			{
				double dBonus = (double) pCaster->pSD->pExData->SkillMod.nImproveHeal;
				dBonus /= 100;
				dReturn += dBonus;
			}
		}
	}
	return dReturn;
}

void CSkillFx::CSkillEffect_p_magic_mp_cost_Pump(EffectStruct *pEffect, CCreature *pCreature)
{
	if(pEffect)
	{
		if(pEffect->nSkillType == 3)
		{
		//	g_Log.Add(CLog::Blue, "[%s] SkillType 3 value[%f] changeType[%d]", __FUNCTION__, pEffect->fValue, pEffect->nChangeType);
			if(pCreature->ValidCreature())
			{
				double fMultipler = pEffect->fValue;
				fMultipler += 100;
				fMultipler /= 100;
				pCreature->pSD->pExData->SkillMod.fSongDanceMpMultipler *= fMultipler;
		//		g_Log.Add(CLog::Blue, "[%s] resoult value[%f]", __FUNCTION__, pCreature->pSD->pExData->SkillMod.fSongDanceMpMultipler);
			}
		}else
		{
			typedef void (*f)(LPVOID, CCreature*);
			f(0x7C55B0L)(pEffect, pCreature);
		}
	}
}