#include "StdAfx.h"
#include "PumpEffects.h"
#include "Random.h"

extern UINT g_BaseBuffLimit;

bool CSkillEffect_p_semi_invisible::CheckPumpCondition(CCreature *pCreature)
{
	if(User *pUser = pCreature->GetUser())
	{
		return true;
	}
	return false;
}

void CSkillEffect_p_semi_invisible::Pump(CCreature *pCreature, CSkillInfo *pSI)
{
	if(pCreature->ValidUser())
	{
		pCreature->pSD->pExData->SkillMod.semiInvisible = TRUE;
	}
}

bool CSkillEffect_p_summon_betray::CalcSuccess(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSI, CSkillAction* pAction)
{
	if(pCreature->ValidCreature() && pTarget->IsSummon())
	{
		double dLandRate = (double)pSI->nActivateRate;
		dLandRate *= pSI->nMagicLevel;
		dLandRate /= pTarget->pSD->nLevel;
		double dRnd = g_MTRandom.RandDouble(100);
		if(dRnd < dLandRate)
			return true;
	}
	return false;
}

bool CSkillEffect_p_summon_betray::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature->IsSummon())
	{
		if(pCreature->CastSummon()->GetMaster())
			return true;
	}
	return false;
};

void CSkillEffect_p_summon_betray::Pump(CCreature *pCreature, CSkillInfo *pSI)
{
	if(pCreature->ValidCreature())
	{
		int nTick = (int)dTick;
		pCreature->pSD->pExData->SummonBetrayData.Betraying = true;
		if(!pCreature->pSD->pExData->SummonBetrayData.ActiveTimer)
		{
			pCreature->pCreatureController->AsyncDoNothing();
			pCreature->pSD->pExData->TimerHandler.AddTimer(CCreatureTimerHandler::SUMMON_BETRAY, 0);
			pCreature->pSD->pExData->SummonBetrayData.ActiveTimer = true;
		}
	}
}

bool CSkillEffect_p_resist_stun::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature->ValidCreature())
	{
		return true;
	}
	return false;
}

void CSkillEffect_p_resist_stun::Pump(CCreature *pCreature, CSkillInfo *pSI)
{
	if(pCreature->ValidCreature())
	{
		double res = 100.0 - value;
		res /= 100;
		pCreature->pSD->pExData->SkillMod.resistStun *= res;
	}
}

bool CSkillEffect_p_strife_cp::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature->ValidUser())
	{
		if(pCreature->pSD->nSSQPart > 0)
		{
			return true;
		}
	}
	return false;
};

void CSkillEffect_p_strife_cp::Pump(CCreature *pCreature, CSkillInfo *pSI)
{
	if(pCreature->ValidUser())
	{
		int Part = SSQSystem::GetInstance()->GetSealOwner(SSQSeal::seal3);
		if(Part!= SSQPart::none)
		{
			double CP = Bonus;
			CP /= 100;
			if(Part == pCreature->pSD->nSSQPart)
			{
				//winner
				pCreature->maxCpPER += CP;
			}else
			{
				//loser
				pCreature->maxCpPER -= CP;
			}
		}
	}
}

bool CSkillEffect_p_self_resurrection::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature && pCreature->pSD->pExData)
	{
		if(pCreature->pSD)
		{
			int nWeaponType = pCreature->pSD->nWeaponType;
			if(nWeaponType < 11)
				return baWeaponType[nWeaponType];
			else 
				return false;
		}else
			return false;
	}else
		return false;
};

void CSkillEffect_p_self_resurrection::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(pCreature)
	{
		switch(ChangeType)
		{
		case DIFF:
			{
				int nSelfRes = pCreature->pSD->pExData->SkillMod.nSelfResurrection;
				nSelfRes += (int)RecoveryExp;
				pCreature->pSD->pExData->SkillMod.nSelfResurrection = nSelfRes;
				break;
			}
		case PER:
			{
				int nSelfRes = pCreature->pSD->pExData->SkillMod.nSelfResurrection;
				nSelfRes += (int)RecoveryExp;
				pCreature->pSD->pExData->SkillMod.nSelfResurrection = nSelfRes;
				break;
			}
		default:
			{
				g_Log.Add(CLog::Error, "[%s] invalid Change Stat Type[%d]", __FUNCTION__, this->ChangeType);
				break;
			}
		}
	}
}


bool CSkillEffect_p_reflect_melee_skill_dmg::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature->ValidCreature())
	{
		if(pCreature->pSD && pCreature->pSD->pExData)
		{
			int nWeaponType = pCreature->pSD->nWeaponType;
			if(nWeaponType < 11)
				return this->baWeaponType[nWeaponType];
			else 
				return false;
		}else
			return false;
	}else
		return false;
};

void CSkillEffect_p_reflect_melee_skill_dmg::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(pCreature->ValidCreature() && pCreature->pSD->pExData)
	{
		switch(this->ChangeType)
		{
		case DIFF:
			{
				int nReflect = pCreature->pSD->pExData->SkillMod.nReflectMeleeSkillDmg;
				nReflect += (int)this->Chance;
				pCreature->pSD->pExData->SkillMod.nReflectMeleeSkillDmg = nReflect;
				break;
			}
		case PER:
			{
				int nReflect = pCreature->pSD->pExData->SkillMod.nReflectMeleeSkillDmg;
				nReflect += (int)this->Chance;
				pCreature->pSD->pExData->SkillMod.nReflectMeleeSkillDmg = nReflect;
				break;
			}
		default:
			{
				g_Log.Add(CLog::Error, "[%s] invalid Change Stat Type[%d]", __FUNCTION__, this->ChangeType);
				break;
			}
		}
	}
}

bool CSkillEffect_p_reduce_exp_penalty::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature)
	{
		CreatureSharedData *pSD = pCreature->pSD;
		if(pSD && pCreature->pSD->pExData)
		{
			return true;
		}else
			return false;
	}else
		return false;
};
void CSkillEffect_p_reduce_exp_penalty::Pump(CCreature *pCreature, CSkillInfo *pSI)
{
	if(pCreature->ValidCreature() && pCreature->pSD->pExData)
	{
		int nPercent = (int)this->dPercent;
		nPercent += pCreature->pSD->pExData->SkillMod.nDecreaseLostExp;
		if( nPercent > 100 )
		{
			g_Log.Add(CLog::Error,"[%s] Too high reduce exp penalty amount[%d] Creature[%S]", __FUNCTION__, nPercent, pCreature->pSD->wszName);
			nPercent = 100;
		}
		pCreature->pSD->pExData->SkillMod.nDecreaseLostExp = nPercent;
		//g_Log.Add(CLog::Error,"[%s] reduce exp penalty amount[%d] Creature[%S]", __FUNCTION__, nPercent, pCreature->pSD->wszName);
	}
}

bool CSkillEffect_p_physical_attack_by_hp2_weapon::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature)
	{
		CreatureSharedData *pSD = pCreature->pSD;
		if(pSD)
		{
			int nWeaponType = pSD->nWeaponType;
			if(nWeaponType < 11)
				return this->baWeaponType[nWeaponType];
			else 
				return false;
		}else
			return false;
	}else
		return false;
};

void CSkillEffect_p_physical_attack_by_hp2_weapon::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(pCreature)
	{
		double MaxHp = pCreature->pSD->MaxHP;
		MaxHp *= 0.60; //60% hp
		double CurrentHp = pCreature->pSD->fHP;
		pCreature->_bUnkn78D = true;
		if(CurrentHp < MaxHp)
		{
			switch(this->ChangeType)
			{
			case DIFF:
				{
					double dPatk = pCreature->physicalAttackDIFF;
					dPatk += this->PAtkAmount;
					pCreature->physicalAttackDIFF = dPatk;
					break;
				}
			case PER:
				{
					double dPatk = this->PAtkAmount;
					dPatk /= 100;
					dPatk += pCreature->physicalAttackPER;
					pCreature->physicalAttackPER = dPatk;
					break;
				}
			default:
				{
					g_Log.Add(CLog::Error, "[%s] invalid Change Stat Type[%d]", __FUNCTION__, this->ChangeType);
					break;
				}
			};
			if(pCreature->ValidUser() && pCreature->informAboutEffectByHp2)
				pCreature->GetUser()->GetSocket()->Send("cddddd", 0x64, 1133, 1, 4, pSkillInfo->nSkillID, pSkillInfo->nLevel);
		}else
		{
			if(pCreature->ValidUser() && pCreature->informAboutEffectByHp2)
				pCreature->GetUser()->GetSocket()->Send("cddddd", 0x64, 1134, 1, 4, pSkillInfo->nSkillID, pSkillInfo->nLevel);
		}
	}
}


bool CSkillEffect_p_mul_skill_dmg::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature->ValidUser())
	{
		return true;
	}
	return false;
};

void CSkillEffect_p_mul_skill_dmg::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(User *pUser = pCreature->SafeCastUser())
	{
		DWORD nSkillID = 0;
		DWORD nLevel = 0;
		
		nSkillID = nClassID >> 8;
		nLevel = nClassID - (nSkillID << 8);
		double multipler = (double)(nValue + 100);
		multipler /= 100;

		SkillDmgMultipler sdm;
		sdm.skillId = nSkillID;
		sdm.level = nLevel;
		sdm.multipler = multipler;
		pUser->pED->lock.Enter();
		pUser->pED->skillDmgMultiplers.push_back(sdm);
		pUser->pED->lock.Leave();
	}
}


bool CSkillEffect_p_magic_speed_by_weapon::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature->ValidCreature())
	{
		CreatureSharedData *pSD = pCreature->pSD;
		if(pSD)
		{
			int nWeaponType = pSD->nWeaponType;
			if(nWeaponType < 11)
				return this->baWeaponType[nWeaponType];
			else 
				return false;
		}else
			return false;
	}else
		return false;
};

void CSkillEffect_p_magic_speed_by_weapon::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(pCreature)
	{
		switch(this->ChangeType)
		{
		case DIFF:
			{
				double dValue = pCreature->magicSpeedDIFF;
				dValue += Speed;
				pCreature->magicSpeedDIFF = dValue;
				break;
			}
		case PER:
			{
				double dValue = Speed;
				dValue += 100;
				dValue /= 100;
				dValue *= pCreature->magicSpeedPER;
				pCreature->magicSpeedPER = dValue;
				break;
			}
		default:
			{
				g_Log.Add(CLog::Error, "[%s] invalid Change Stat Type[%d]", __FUNCTION__, this->ChangeType);
				break;
			}
		}
	}
}

bool CSkillEffect_p_improve_heal::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature)
	{
		CreatureSharedData *pSD = pCreature->pSD;
		if(pSD && pCreature->pSD->pExData)
		{
			return true;
		}else
			return false;
	}else
		return false;
};

void CSkillEffect_p_improve_heal::Pump(CCreature *pCreature, CSkillInfo *pSI)
{
	if(pCreature->ValidCreature())
	{
		int nImprove = (int)this->dStat;
		if(pCreature->pSD->pExData)
		{
			int nStat = pCreature->pSD->pExData->SkillMod.nImproveHeal;
			nStat += nImprove;
			pCreature->pSD->pExData->SkillMod.nImproveHeal = nStat;
		}
	}
}

bool CSkillEffect_p_enlarge_buff_slots::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature)
	{
		CreatureSharedData *pSD = pCreature->pSD;
		if(pSD && pCreature->pSD->pExData)
		{
			return true;
		}
	}

	return false;
};

void CSkillEffect_p_enlarge_buff_slots::Pump(CCreature *pCreature, CSkillInfo *pSI)
{
	if(pCreature->ValidCreature())
	{
		int nSlots = static_cast<int>(dSlots);
		nSlots += pCreature->pSD->pExData->SkillMod.nEnlargeBuffSlots;
		if((nSlots+g_BaseBuffLimit) > MAX_BUFF_SLOT)
		{
			g_Log.Add(CLog::Error,"[%s] Buff Slot overflow max[%d] current[%d] Creature[%S]", __FUNCTION__, MAX_BUFF_SLOT, (nSlots+20), pCreature->pSD->wszName);
			nSlots = MAX_BUFF_SLOT - g_BaseBuffLimit;
		}
		pCreature->pSD->pExData->SkillMod.nEnlargeBuffSlots = nSlots;
	}
}

bool CSkillEffect_p_digger_chance::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature->ValidUser())
	{
		CreatureSharedData *pSD = pCreature->pSD;
		if(pSD && pCreature->pSD->pExData)
		{
			return true;
		}else
			return false;
	}else
		return false;
};

void CSkillEffect_p_digger_chance::Pump(CCreature *pCreature, CSkillInfo *pSI)
{
	if(pCreature->ValidUser())
	{
		double chance = baseChance;
		chance /= 100;
		pCreature->pSD->pExData->SkillMod.fDiggerMultipler *= chance;
	}
}

bool CSkillEffect_p_critical_rate_by_weapon::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature)
	{
		CreatureSharedData *pSD = pCreature->pSD;
		if(pSD)
		{
			int nWeaponType = pSD->nWeaponType;
			if(nWeaponType < 11)
				return this->baWeaponType[nWeaponType];
			else 
				return false;
		}else
			return false;
	}else
		return false;
};

void CSkillEffect_p_critical_rate_by_weapon::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(pCreature)
	{
		switch(this->ChangeType)
		{
		case DIFF:
			{
				double dCriticalRate = pCreature->criticalRateDIFF;
				dCriticalRate+=this->CriticalAmount;
				pCreature->criticalRateDIFF = dCriticalRate;
				break;
			}
		case PER:
			{
				double dCriticalRate = this->CriticalAmount;
				dCriticalRate /= 100;
				dCriticalRate += pCreature->criticalRatePER;
				pCreature->criticalRatePER = dCriticalRate;
				break;
			}
		default:
			{
				g_Log.Add(CLog::Error, "[%s] invalid Change Stat Type[%d]", __FUNCTION__, this->ChangeType);
				break;
			}
		}
	}
}

bool CSkillEffect_p_critical_rate_by_hp2_weapon::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature)
	{
		CreatureSharedData *pSD = pCreature->pSD;
		if(pSD)
		{
			int nWeaponType = pSD->nWeaponType;
			if(nWeaponType < 11)
				return this->baWeaponType[nWeaponType];
			else 
				return false;
		}else
			return false;
	}else
		return false;
};

void CSkillEffect_p_critical_rate_by_hp2_weapon::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(pCreature)
	{
		double MaxHp = pCreature->pSD->MaxHP;
		MaxHp *= 0.60; //60% hp
		double CurrentHp = pCreature->pSD->fHP;
		pCreature->_bUnkn78D = true;
		if(CurrentHp < MaxHp)
		{
			switch(this->ChangeType)
			{
			case DIFF:
				{
					double dCritical = pCreature->criticalRateDIFF;
					dCritical+=this->CriticalAmount;
					pCreature->criticalRateDIFF = dCritical;
					break;
				}
			case PER:
				{
					double dCritical = this->CriticalAmount;
					dCritical /= 100;
					dCritical += pCreature->criticalRatePER;
					pCreature->criticalRatePER = dCritical;
					break;
				}
			default:
				{
					g_Log.Add(CLog::Error, "[%s] invalid Change Stat Type[%d]", __FUNCTION__, this->ChangeType);
					break;
				}
			}
			if(pCreature->ValidUser() && pCreature->informAboutEffectByHp2)
				pCreature->GetUser()->GetSocket()->Send("cddddd", 0x64, 1133, 1, 4, pSkillInfo->nSkillID, pSkillInfo->nLevel);
		}else
		{
			if(pCreature->ValidUser() && pCreature->informAboutEffectByHp2)
				pCreature->GetUser()->GetSocket()->Send("cddddd", 0x64, 1134, 1, 4, pSkillInfo->nSkillID, pSkillInfo->nLevel);
		}
	}
}

bool CSkillEffect_p_critical_rate_by_hp1_weapon::CheckPumpCondition(CCreature *pCreature)
{
	
	
	if(pCreature)
	{
		CreatureSharedData *pSD = pCreature->pSD;
		if(pSD)
		{
			int nWeaponType = pSD->nWeaponType;
			if(nWeaponType < 11)
				return this->baWeaponType[nWeaponType];
			else 
				return false;
		}else
			return false;
	}else
		return false;
};

void CSkillEffect_p_critical_rate_by_hp1_weapon::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(pCreature)
	{
		double MaxHp = pCreature->pSD->MaxHP;
		MaxHp *= 0.30; //60% hp
		double CurrentHp = pCreature->pSD->fHP;
		pCreature->_bUnkn78D = true;
		if(CurrentHp < MaxHp)
		{
			switch(this->ChangeType)
			{
			case DIFF:
				{
					double dCritical = pCreature->criticalRateDIFF;
					dCritical+=this->CriticalAmount;
					pCreature->criticalRateDIFF = dCritical;
					break;
				}
			case PER:
				{
					double dCritical = this->CriticalAmount;
					dCritical /= 100;
					dCritical += pCreature->criticalRatePER;
					pCreature->criticalRatePER = dCritical;
					break;
				}
			default:
				{
					g_Log.Add(CLog::Error, "[%s] invalid Change Stat Type[%d]", __FUNCTION__, this->ChangeType);
					break;
				}
			}
			if(pCreature->ValidUser() && pCreature->informAboutEffectByHp1)
				pCreature->GetUser()->GetSocket()->Send("cddddd", 0x64, 1133, 1, 4, pSkillInfo->nSkillID, pSkillInfo->nLevel);
		}else
		{
			if(pCreature->ValidUser() && pCreature->informAboutEffectByHp1)
				pCreature->GetUser()->GetSocket()->Send("cddddd", 0x64, 1134, 1, 4, pSkillInfo->nSkillID, pSkillInfo->nLevel);
		}
	}
}

bool CSkillEffect_p_critical_damage_position_front_by_weapon::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature)
	{
		CreatureSharedData *pSD = pCreature->pSD;
		if(pSD)
		{
			int nWeaponType = pSD->nWeaponType;
			if(nWeaponType < 11)
				return this->baWeaponType[nWeaponType];
			else 
				return false;
		}else
			return false;
	}else
		return false;
};

bool CSkillEffect_p_critical_damage_position_side_by_weapon::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature)
	{
		CreatureSharedData *pSD = pCreature->pSD;
		if(pSD)
		{
			int nWeaponType = pSD->nWeaponType;
			if(nWeaponType < 11)
				return this->baWeaponType[nWeaponType];
			else 
				return false;
		}else
			return false;
	}else
		return false;
};

bool CSkillEffect_p_critical_damage_position_back_by_weapon::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature)
	{
		CreatureSharedData *pSD = pCreature->pSD;
		if(pSD)
		{
			int nWeaponType = pSD->nWeaponType;
			if(nWeaponType < 11)
				return this->baWeaponType[nWeaponType];
			else 
				return false;
		}else
			return false;
	}else
		return false;
};

void CSkillEffect_p_critical_damage_position_front_by_weapon::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(pCreature)
	{
		switch(this->ChangeType)
		{
		case DIFF:
			{
				double dCritical = pCreature->criticalDamagePositionFrontDIFF;
				dCritical+=this->CriticalAmount;
				pCreature->criticalDamagePositionFrontDIFF = dCritical;
				break;
			}
		case PER:
			{
				double dCritical = this->CriticalAmount;
				dCritical /= 100;
				dCritical += pCreature->criticalDamagePositionFrontPER;
				pCreature->criticalDamagePositionFrontPER = dCritical;
				break;
			}
		default:
			{
				g_Log.Add(CLog::Error, "[%s] invalid Change Stat Type[%d]", __FUNCTION__, this->ChangeType);
				break;
			}
		}
	}
}

void CSkillEffect_p_critical_damage_position_side_by_weapon::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(pCreature)
	{
		switch(this->ChangeType)
		{
		case DIFF:
			{
				double dCritical = pCreature->criticalDamagePositionSideDIFF;
				dCritical+=this->CriticalAmount;
				pCreature->criticalDamagePositionSideDIFF = dCritical;
				break;
			}
		case PER:
			{
				double dCritical = this->CriticalAmount;
				dCritical /= 100;
				dCritical += pCreature->criticalDamagePositionSidePER;
				pCreature->criticalDamagePositionSidePER = dCritical;
				break;
			}
		default:
			{
				g_Log.Add(CLog::Error, "[%s] invalid Change Stat Type[%d]", __FUNCTION__, this->ChangeType);
				break;
			}
		}
	}
}

void CSkillEffect_p_critical_damage_position_back_by_weapon::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(pCreature)
	{
		switch(this->ChangeType)
		{
		case DIFF:
			{
				double dCritical = pCreature->criticalDamagePositionBackDIFF;
				dCritical+=this->CriticalAmount;
				pCreature->criticalDamagePositionBackDIFF = dCritical;
				break;
			}
		case PER:
			{
				double dCritical = this->CriticalAmount;
				dCritical /= 100;
				dCritical += pCreature->criticalDamagePositionBackPER;
				pCreature->criticalDamagePositionBackPER = dCritical;
				break;
			}
		default:
			{
				g_Log.Add(CLog::Error, "[%s] invalid Change Stat Type[%d]", __FUNCTION__, this->ChangeType);
				break;
			}
		}
	}
}

bool CSkillEffect_p_critical_damage_by_weapon::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature->ValidCreature())
	{
		CreatureSharedData *pSD = pCreature->pSD;
		if(pSD)
		{
			int nWeaponType = pSD->nWeaponType;
			if(nWeaponType < 11)
				return this->baWeaponType[nWeaponType];
			else 
				return false;
		}else
			return false;
	}else
		return false;
};

void CSkillEffect_p_critical_damage_by_weapon::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(pCreature)
	{
		switch(this->ChangeType)
		{
		case DIFF:
			{
				double dCriticalDamage = pCreature->criticalDamageDIFF;
				dCriticalDamage+=this->DamageAmount;
				pCreature->criticalDamageDIFF = dCriticalDamage;
				break;
			}
		case PER:
			{
				double dCriticalDamage = this->DamageAmount;
				dCriticalDamage += 100;
				dCriticalDamage /= 100;
				dCriticalDamage *= pCreature->criticalDamagePER;
				pCreature->criticalDamagePER = dCriticalDamage;
				break;
			}
		default:
			{
				g_Log.Add(CLog::Error, "[%s] invalid Change Stat Type[%d]", __FUNCTION__, this->ChangeType);
				break;
			}
		}
	}
}


bool CSkillEffect_p_cancel_attacker_target::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature && pCreature->pSD->pExData)
	{
		CreatureSharedData *pSD = pCreature->pSD;
		if(pSD)
		{
			int nWeaponType = pSD->nWeaponType;
			if(nWeaponType < 11)
				return this->baWeaponType[nWeaponType];
			else 
				return false;
		}else
			return false;
	}else
		return false;
};

void CSkillEffect_p_cancel_attacker_target::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(pCreature)
	{
		switch(this->ChangeType)
		{
		case DIFF:
			{
				int nAvoid = pCreature->pSD->pExData->SkillMod.nCancelAttackerTarget;
				nAvoid += (int)this->Chance;
				pCreature->pSD->pExData->SkillMod.nCancelAttackerTarget = nAvoid;
				break;
			}
		case PER:
			{
				int nAvoid = pCreature->pSD->pExData->SkillMod.nCancelAttackerTarget;
				nAvoid += (int)this->Chance;
				pCreature->pSD->pExData->SkillMod.nCancelAttackerTarget = nAvoid;
				break;
			}
		default:
			{
				g_Log.Add(CLog::Error, "[%s] invalid Change Stat Type[%d]", __FUNCTION__, this->ChangeType);
				break;
			}
		}
	}
}

bool CSkillEffect_p_block_icon_action::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature)
	{
		CreatureSharedData *pSD = pCreature->pSD;
		if(pSD && pCreature->pSD->pExData)
		{
			return true;
		}else
			return false;
	}else
		return false;
};

void CSkillEffect_p_block_icon_action::Pump(CCreature *pCreature, CSkillInfo *pSI)
{
	if(pCreature->ValidCreature())
	{
		int nAction = (int)this->dActionID;
		pCreature->pSD->pExData->SkillMod.nBlockIconAction = nAction;		
	}
}

bool CSkillEffect_p_avoid_melee_skill::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature)
	{
		CreatureSharedData *pSD = pCreature->pSD;
		if(pSD && pCreature->pSD->pExData)
		{
			int nWeaponType = pSD->nWeaponType;
			if(nWeaponType < 11)
				return this->baWeaponType[nWeaponType];
			else 
				return false;
		}else
			return false;
	}else
		return false;
};

void CSkillEffect_p_avoid_melee_skill::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(pCreature)
	{
		switch(this->ChangeType)
		{
		case DIFF:
			{
				int nAvoid = pCreature->pSD->pExData->SkillMod.nAvoidMeleeSkill;
				nAvoid += (int)this->Chance;
				pCreature->pSD->pExData->SkillMod.nAvoidMeleeSkill = nAvoid;
				break;
			}
		case PER:
			{
				int nAvoid = pCreature->pSD->pExData->SkillMod.nAvoidMeleeSkill;
				nAvoid += (int)this->Chance;
				pCreature->pSD->pExData->SkillMod.nAvoidMeleeSkill = nAvoid;
				break;
			}
		default:
			{
				g_Log.Add(CLog::Error, "[%s] invalid Change Stat Type[%d]", __FUNCTION__, this->ChangeType);
				break;
			}
		}
	}
}

bool CSkillEffect_p_autocast_when_attacked::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature)
	{
		if(pCreature->pSD->nIsPC)
		{
			return true;
		}
	}
	return false;
};

void CSkillEffect_p_autocast_when_attacked::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(User *pUser = pCreature->SafeCastUser())
	{
		DWORD nSkillID = nClassID >> 8;
		DWORD nLevel = nClassID - (nSkillID << 8);

		if(nLevel < 256)
		{
			AutoCastSkill acs;
			acs.skillId = nSkillID;
			acs.level = nLevel;
			acs.chance = (double)nChance;
			acs.type = AutoCastSkill::OnDamaged;
			pUser->pED->lock.Enter();
			pUser->pED->autoCastSkills.push_back(acs);
			pUser->pED->lock.Leave();
		}else
			g_Log.Add(CLog::Error, "[%s] Invalid skill level[%d] or chance[%d]", __FUNCTION__, nLevel, this->nChance);
	}
}


bool CSkillEffect_p_autocast_on_patk::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature)
	{
		if(pCreature->pSD->nIsPC)
		{
			return true;
		}
	}
	return false;
};

void CSkillEffect_p_autocast_on_patk::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(User *pUser = pCreature->SafeCastUser())
	{
		DWORD nSkillID = nClassID >> 8;
		DWORD nLevel = nClassID - (nSkillID << 8);

		if(nLevel < 256)
		{
			AutoCastSkill acs;
			acs.skillId = nSkillID;
			acs.level = nLevel;
			acs.chance = (double)nChance;
			acs.type = AutoCastSkill::OnPhysicalAttack;
			pUser->pED->lock.Enter();
			pUser->pED->autoCastSkills.push_back(acs);
			pUser->pED->lock.Leave();
		}else
			g_Log.Add(CLog::Error, "[%s] Invalid skill level[%d] or chance[%d]", __FUNCTION__, nLevel, this->nChance);
	}
}


bool CSkillEffect_p_autocast_on_matk::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature)
	{
		if(pCreature->pSD->nIsPC)
		{
			return true;
		}
	}
	return false;
};

void CSkillEffect_p_autocast_on_matk::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(User *pUser = pCreature->SafeCastUser())
	{
		DWORD nSkillID = nClassID >> 8;
		DWORD nLevel = nClassID - (nSkillID << 8);

		if(nLevel < 256)
		{
			AutoCastSkill acs;
			acs.skillId = nSkillID;
			acs.level = nLevel;
			acs.chance = (double)nChance;
			acs.type = AutoCastSkill::OnMagicalAttack;
			pUser->pED->lock.Enter();
			pUser->pED->autoCastSkills.push_back(acs);
			pUser->pED->lock.Leave();
		}else
			g_Log.Add(CLog::Error, "[%s] Invalid skill level[%d] or chance[%d]", __FUNCTION__, nLevel, this->nChance);
	}
}


bool CSkillEffect_p_autocast_on_crit::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature)
	{
		if(pCreature->pSD->nIsPC)
		{
			return true;
		}
	}
	return false;
};

void CSkillEffect_p_autocast_on_crit::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(User *pUser = pCreature->SafeCastUser())
	{
		DWORD nSkillID = nClassID >> 8;
		DWORD nLevel = nClassID - (nSkillID << 8);

		if(nLevel < 256)
		{
			AutoCastSkill acs;
			acs.skillId = nSkillID;
			acs.level = nLevel;
			acs.chance = (double)nChance;
			acs.type = AutoCastSkill::OnCriticalAttack;
			pUser->pED->lock.Enter();
			pUser->pED->autoCastSkills.push_back(acs);
			pUser->pED->lock.Leave();
		}else
			g_Log.Add(CLog::Error, "[%s] Invalid skill level[%d] or chance[%d]", __FUNCTION__, nLevel, this->nChance);
	}
}

bool CSkillEffect_p_anti_buff::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature)
	{
		CreatureSharedData *pSD = pCreature->pSD;
		if(pSD && pCreature->pSD->pExData)
		{
			return true;
		}else
			return false;
	}else
		return false;
};

void CSkillEffect_p_anti_buff::Pump(CCreature *pCreature, CSkillInfo *pSI)
{
	if(pCreature->ValidCreature())
	{
		if(dSelf == 1)
		{
			pCreature->pSD->pExData->SkillMod.nAntiBuff = 1;		
		}else
		{
			pCreature->pSD->pExData->SkillMod.nAntiBuff = 2;		
		}
	}
}

bool CSkillEffect_p_defence_attribute_by_armor::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature)
	{
		if(pCreature->pSD->nArmorType < 4)
		{
			return armorType[pCreature->pSD->nArmorType];
		}
	}
	return false;
}

void CSkillEffect_p_defence_attribute_by_armor::Pump(CCreature* pCreature, CSkillInfo* pSI)
{
	if(pCreature->ValidCreature())
	{
		if(attribute >= 0 && attribute < 34)
		{
			double attr = value;
			attr += 100;
			attr /= 100;
			attr *= pCreature->defenceAttribute[attribute];
			pCreature->defenceAttribute[attribute] = attr;
		}
	}
}


bool CSkillEffect_p_speed_by_weapon::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature->ValidCreature())
	{
		CreatureSharedData *pSD = pCreature->pSD;
		if(pSD)
		{
			int nWeaponType = pSD->nWeaponType;
			if(nWeaponType < 11)
				return weaponType[nWeaponType];
		}
	}
	return false;
};

void CSkillEffect_p_speed_by_weapon::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(pCreature)
	{
		switch(ChangeType)
		{
		case DIFF:
			{
				double speed = pCreature->speedDIFF;
				speed += value;
				pCreature->speedDIFF = speed;
				break;
			}
		case PER:
			{
				double speed = value;
				speed += 100;
				speed /= 100;
				speed *= pCreature->speedPER;
				pCreature->speedPER = speed;
				break;
			}
		default:
			{
				g_Log.Add(CLog::Error, "[%s] invalid Change Stat Type[%d]", __FUNCTION__, this->ChangeType);
				break;
			}
		}
		pCreature->ValidateSpeed();
		pCreature->broadcastInfoPacket = true;
	}
}

bool CSkillEffect_p_attack_speed_by_weapon2::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature->ValidCreature())
	{
		CreatureSharedData *pSD = pCreature->pSD;
		if(pSD)
		{
			int nWeaponType = pSD->nWeaponType;
			if(nWeaponType < 11)
				return weaponType[nWeaponType];
		}
	}
	return false;
};

void CSkillEffect_p_attack_speed_by_weapon2::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(pCreature->ValidCreature())
	{
		switch(ChangeType)
		{
		case DIFF:
			{
				double speed = pCreature->attackSpeedDP;
				speed += value;
				pCreature->attackSpeedDP = speed;
				break;
			}
		case PER:
			{
				double speed = value + 100.0;
				speed /= 100;
				speed *= pCreature->attackSpeedDP;
				pCreature->attackSpeedDP = speed;
				break;
			}
		default:
			{
				g_Log.Add(CLog::Error, "[%s] invalid Change Stat Type[%d]", __FUNCTION__, this->ChangeType);
				break;
			}
		}
		pCreature->ValidateAttackSpeed();
		pCreature->broadcastInfoPacket = true;
	}
}

bool CSkillEffect_p_physical_defence_by_weapon::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature->ValidCreature())
	{
		CreatureSharedData *pSD = pCreature->pSD;
		if(pSD)
		{
			int nWeaponType = pSD->nWeaponType;
			if(nWeaponType < 11)
				return weaponType[nWeaponType];
		}
	}
	return false;
};

void CSkillEffect_p_physical_defence_by_weapon::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(pCreature)
	{
		switch(ChangeType)
		{
		case DIFF:
			{
				double tmp = pCreature->physicalDefenceDIFF;
				tmp += value;
				pCreature->physicalDefenceDIFF = tmp;
				break;
			}
		case PER:
			{
				double tmp = value;
				tmp += 100;
				tmp /= 100;
				tmp *= pCreature->physicalDefencePER;
				pCreature->physicalDefencePER = tmp;
				break;
			}
		default:
			{
				g_Log.Add(CLog::Error, "[%s] invalid Change Stat Type[%d]", __FUNCTION__, this->ChangeType);
				break;
			}
		}
	}
}

bool CSkillEffect_p_magical_defence_by_weapon::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature->ValidCreature())
	{
		CreatureSharedData *pSD = pCreature->pSD;
		if(pSD)
		{
			int nWeaponType = pSD->nWeaponType;
			if(nWeaponType < 11)
				return weaponType[nWeaponType];
		}
	}
	return false;
};

void CSkillEffect_p_magical_defence_by_weapon::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(pCreature)
	{
		switch(ChangeType)
		{
		case DIFF:
			{
				double tmp = pCreature->magicalDefenceDIFF;
				tmp += value;
				pCreature->magicalDefenceDIFF = tmp;
				break;
			}
		case PER:
			{
				double tmp = value;
				tmp += 100;
				tmp /= 100;
				tmp *= pCreature->magicalDefencePER;
				pCreature->magicalDefencePER = tmp;
				break;
			}
		default:
			{
				g_Log.Add(CLog::Error, "[%s] invalid Change Stat Type[%d]", __FUNCTION__, this->ChangeType);
				break;
			}
		}
	}
}

bool CSkillEffect_p_avoid_by_weapon::CheckPumpCondition(CCreature *pCreature)
{
	if(pCreature->ValidCreature())
	{
		CreatureSharedData *pSD = pCreature->pSD;
		if(pSD)
		{
			int nWeaponType = pSD->nWeaponType;
			if(nWeaponType < 11)
				return weaponType[nWeaponType];
		}
	}
	return false;
};

void CSkillEffect_p_avoid_by_weapon::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(pCreature)
	{
		switch(ChangeType)
		{
		case DIFF:
			{
				double tmp = pCreature->evasionDIFF;
				tmp += value;
				pCreature->evasionDIFF = tmp;
				break;
			}
		case PER:
			{
				double tmp = value;
				tmp += 100;
				tmp /= 100;
				tmp *= pCreature->evasionDIFF;
				pCreature->evasionDIFF = tmp;
				break;
			}
		default:
			{
				g_Log.Add(CLog::Error, "[%s] invalid Change Stat Type[%d]", __FUNCTION__, this->ChangeType);
				break;
			}
		}
	}
}

bool CSkillEffect_p_bonus_exp::CheckPumpCondition(CCreature *pCreature)
{
	if(User *pUser = pCreature->GetUser())
	{
		if(pUser->pSD)
		{
			UINT nWeaponType = pUser->pSD->nWeaponType;
			if(nWeaponType < 11)
				return weaponType[nWeaponType];
		}
	}
	return false;
};

void CSkillEffect_p_bonus_exp::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(User *pUser = pCreature->GetUser())
	{
		double temp = value;	//xx ex 30
		temp /= 100;			//0.xx ex 0.30
		temp += pUser->pSD->pExData->SkillMod.bonusExp;
		pUser->pSD->pExData->SkillMod.bonusExp = temp;
	}
}

bool CSkillEffect_p_bonus_sp::CheckPumpCondition(CCreature *pCreature)
{
	if(User *pUser = pCreature->GetUser())
	{
		if(pUser->pSD)
		{
			UINT nWeaponType = pUser->pSD->nWeaponType;
			if(nWeaponType < 11)
				return weaponType[nWeaponType];
		}
	}
	return false;
};

void CSkillEffect_p_bonus_sp::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(User *pUser = pCreature->GetUser())
	{
		double temp = value;	//xx ex 30
		temp /= 100;			//0.xx ex 0.30
		temp += pUser->pSD->pExData->SkillMod.bonusSp;
		pUser->pSD->pExData->SkillMod.bonusSp = temp;
	}
}

bool CSkillEffect_p_bonus_item_drop::CheckPumpCondition(CCreature *pCreature)
{
	if(User *pUser = pCreature->GetUser())
	{
		if(pUser->pSD)
		{
			UINT nWeaponType = pUser->pSD->nWeaponType;
			if(nWeaponType < 11)
				return weaponType[nWeaponType];
		}
	}
	return false;
};

void CSkillEffect_p_bonus_item_drop::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(User *pUser = pCreature->GetUser())
	{
		if(ChangeType == DIFF)
		{
			double temp = value;	//xx ex 30
			temp /= 100;			//0.xx ex 0.30
			temp += pUser->pSD->pExData->SkillMod.bonusItemDropOnlyChance;
			pUser->pSD->pExData->SkillMod.bonusItemDropOnlyChance = temp;
		}else
		{
			double temp = value;	//xx ex 30
			temp /= 100;			//0.xx ex 0.30
			temp += pUser->pSD->pExData->SkillMod.bonusItemDrop;
			pUser->pSD->pExData->SkillMod.bonusItemDrop = temp;
		}
	}
}

bool CSkillEffect_p_bonus_adena::CheckPumpCondition(CCreature *pCreature)
{
	if(User *pUser = pCreature->GetUser())
	{
		if(pUser->pSD)
		{
			UINT nWeaponType = pUser->pSD->nWeaponType;
			if(nWeaponType < 11)
				return weaponType[nWeaponType];
		}
	}
	return false;
};

void CSkillEffect_p_bonus_adena::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(User *pUser = pCreature->GetUser())
	{
		double temp = value;	//xx ex 30
		temp /= 100;			//0.xx ex 0.30
		temp += pUser->pSD->pExData->SkillMod.bonusAdena;
		pUser->pSD->pExData->SkillMod.bonusAdena = temp;
	}
}

bool CSkillEffect_p_bonus_spoil::CheckPumpCondition(CCreature *pCreature)
{
	if(User *pUser = pCreature->GetUser())
	{
		if(pUser->pSD)
		{
			UINT nWeaponType = pUser->pSD->nWeaponType;
			if(nWeaponType < 11)
				return weaponType[nWeaponType];
		}
	}
	return false;
};

void CSkillEffect_p_bonus_spoil::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(User *pUser = pCreature->GetUser())
	{
		double temp = value;	//xx ex 30
		temp /= 100;			//0.xx ex 0.30
		temp += pUser->pSD->pExData->SkillMod.bonusSpoil;
		pUser->pSD->pExData->SkillMod.bonusSpoil = temp;
	}
}

bool CSkillEffect_p_bonus_harvesting::CheckPumpCondition(CCreature *pCreature)
{
	if(User *pUser = pCreature->GetUser())
	{
		if(pUser->pSD)
		{
			UINT nWeaponType = pUser->pSD->nWeaponType;
			if(nWeaponType < 11)
				return weaponType[nWeaponType];
		}
	}
	return false;
};

void CSkillEffect_p_bonus_harvesting::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(User *pUser = pCreature->GetUser())
	{
		double temp = value;	//xx ex 30
		temp /= 100;			//0.xx ex 0.30
		temp += pUser->pSD->pExData->SkillMod.bonusHarvesting;
		pUser->pSD->pExData->SkillMod.bonusHarvesting = temp;
	}
}

bool CSkillEffect_p_bonus_rb::CheckPumpCondition(CCreature *pCreature)
{
	if(User *pUser = pCreature->GetUser())
	{
		if(pUser->pSD)
		{
			UINT nWeaponType = pUser->pSD->nWeaponType;
			if(nWeaponType < 11)
				return weaponType[nWeaponType];
		}
	}
	return false;
};

void CSkillEffect_p_bonus_rb::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(User *pUser = pCreature->GetUser())
	{
		if(this->ChangeType == DIFF)
		{
			double temp = value;	//xx ex 30
			temp /= 100;			//0.xx ex 0.30
			temp += pUser->pSD->pExData->SkillMod.bonusRbOnlyChance;
			pUser->pSD->pExData->SkillMod.bonusRbOnlyChance = temp;
		}else
		{
			double temp = value;	//xx ex 30
			temp /= 100;			//0.xx ex 0.30
			temp += pUser->pSD->pExData->SkillMod.bonusRb;
			pUser->pSD->pExData->SkillMod.bonusRb = temp;
		}
	}
}

bool CSkillEffect_p_bonus_epic_rb::CheckPumpCondition(CCreature *pCreature)
{
	if(User *pUser = pCreature->GetUser())
	{
		if(pUser->pSD)
		{
			UINT nWeaponType = pUser->pSD->nWeaponType;
			if(nWeaponType < 11)
				return weaponType[nWeaponType];
		}
	}
	return false;
};

void CSkillEffect_p_bonus_epic_rb::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	if(User *pUser = pCreature->GetUser())
	{
		double temp = value;	//xx ex 30
		temp /= 100;			//0.xx ex 0.30
		temp += pUser->pSD->pExData->SkillMod.bonusEpicRb;
		pUser->pSD->pExData->SkillMod.bonusEpicRb = temp;
	}
}

bool CSkillEffect_p_bonus_item_id::CheckPumpCondition(CCreature *pCreature)
{
	if(User *pUser = pCreature->GetUser())
	{
		return true;
	}
	return false;
};

void CSkillEffect_p_bonus_item_id::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	guard;
	if(User *pUser = pCreature->GetUser())
	{
		double temp = static_cast<double>(nValue);	//xx ex 30
		temp /= 100;			//0.xx ex 0.30
		pUser->pED->itemBonus.Add(nClassID, temp);
	}
	unguard;
}

bool CSkillEffect_p_anti_pk::CheckPumpCondition(CCreature *pCreature)
{
	if(User *pUser = pCreature->GetUser())
	{
		if(pUser->pSD->nLevel <= static_cast<INT32>(nMaxPlayerLevel))
		{
			return true;
		}
	}
	return false;
}

void CSkillEffect_p_anti_pk::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	guard;
	if(User *pUser = pCreature->GetUser())
	{
		pUser->pED->antiPkLevelRange = static_cast<INT32>(nLevelRange);
	}
	unguard;
}

bool CSkillEffect_p_regen_cp::CheckPumpCondition(CCreature *pCreature)
{
	guard;
	bool ret = false;
	if(User *pUser = pCreature->SafeCastUser())
	{
	//	g_Log.Add(CLog::Blue, "[%s] weaponType[%d][%d]", __FUNCTION__, pUser->pSD->nWeaponType, m_WeaponType[pUser->pSD->nWeaponType]);
		if(pUser->pSD->nWeaponType < 11)
		{
			ret = m_WeaponType[pUser->pSD->nWeaponType];
		}
	}
	unguard;
	return ret;
};

void CSkillEffect_p_regen_cp::Pump(CCreature *pCreature, CSkillInfo *pSkillInfo)
{
	guard;
	if(User *pUser = pCreature->SafeCastUser())
	{
	//	g_Log.Add(CLog::Blue, "[%s] Type[%d] amount[%f] offset[%x]", __FUNCTION__, m_Type, m_Amount, offsetof(CSkillEffect_p_regen_cp,CSkillEffect_p_regen_cp::m_Amount));
		if(m_Type == DIFF)
		{
			pUser->pSD->pExData->SkillMod.regenCpAdd += m_Amount;
		}else if(m_Type == PER)
		{
			double regen = m_Amount;
			regen /= 100.0;
			pUser->pSD->pExData->SkillMod.regenCpMul += regen;
		}
	}
	unguard;
}