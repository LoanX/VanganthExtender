#include "StdAfx.h"
#include "OperateConditions.h"
#include "TerritoryData.h"
#include "MiningSystem.h"
#include "SpiritSystem.h"
#include "ClanSys.h"

bool CSkillOperateCondition_op_can_release_spirit::CanUse(CCreature *pCreature, CObject *pTarget)
{
	if(User *pUser = pCreature->GetUser())
	{
		if(pUser->olympiadUser.IsInside() == FALSE && pUser->pSD->nSpiritCounter >= count)
		{
			return true;
		}
	}
	return false;
}

bool CSkillOperateCondition_op_can_dig_at::CanUse(CCreature *pCreature, CObject *pTarget)
{
	if(User *pUser = pCreature->GetUser())
	{
		if(type == 0)
		{
			return g_MiningSystem.CanDig(pUser);
		}else if(type == 1)
		{
			if(g_L2Time.IsNight() == false)
			{
				return g_MiningSystem.CanDig(pUser);
			}
		}else if(type == 2) //night
		{
			if(g_L2Time.IsNight() == true)
			{
				return g_MiningSystem.CanDig(pUser);
			}
		}
	}
	return false;
}

bool CSkillOperateCondition_op_strife_not_owned_by::CanUse(CCreature *pCreature, CObject *pTarget)
{
	if(SSQSystem::GetInstance()->GetSealOwner(SSQSeal::seal3) != nOwner)
		return true;

	return false;
}

bool CSkillOperateCondition_op_spell_force_count::CanUse(CCreature *pCreature, CObject *pTarget)
{
	if(pCreature->ValidCreature() && pCreature->pSD->pExData)
	{
		if(pCreature->pSD->nBuilder == 1)
			return true;

		if(pCreature->HasAbnormal(173) >= this->nCount)
			return true;
	}
	return false;
}

bool CSkillOperateCondition_op_remain_mp_per::CanUse(CCreature *pCreature, CObject *pTarget)
{
	if(pCreature->ValidCreature())
	{
		double RemainMpPer = pCreature->pSD->fMP / pCreature->pSD->MaxMP * 100;
		if(RemainMpPer <= this->nPer)
			return true;
		else
			return false;
	}
	return false;
}

bool CSkillOperateCondition_op_not_in_territory::CanUse(CCreature *pCreature, CObject *pTarget)
{
	if(pCreature->ValidCreature())
	{
		TerritoryType type = CTerritoryEx::CastType(nTerritoryType);	
		if(g_TerritoryChecker.IsInTerritory(pCreature, type))
		{
			return false;
		}else
			return true;
	}
	return false;
}

bool CSkillOperateCondition_op_not_class::CanUse(CCreature *pCreature, CObject *pTarget)
{
	CCreature *pTg = pTarget->CastCreature();
	if(pTg->ValidCreature())
	{
		if(pTg->pSD->nClass != nClassID)
			return true;
	}
	return false;
}

bool CSkillOperateCondition_op_max_height_diff::CanUse(CCreature *pCreature, CObject *pTarget)
{
	CCreature *pT = pTarget->CastCreature();
	if(pCreature->ValidCreature() && pT->ValidCreature())
	{
		//Check Z
		double dCZ = pCreature->pSD->Pos.z;
		double dTZ = pT->pSD->Pos.z;
		if(dCZ<0)
			dCZ = dCZ*(-1);
		if(dTZ<0)
			dTZ = dTZ*(-1);
		dCZ -= dTZ;
		double dDiff = (double)this->nDiff;
		if(dCZ < dDiff && dCZ > -dDiff)
			return true;
		else
			return false;
	}
	return false;
}

bool CSkillOperateCondition_op_is_type::CanUse(CCreature *pCreature, CObject *pTarget)
{
	//1- PC 2 - NPC 3-SUMMON 4 - PC & SUMMON 5- BOSS and ZZoldagu 6 - NOT Boss And NOT Zzoldagu
	CCreature *pTg = pTarget->CastCreature();
	if(pTg->ValidCreature())
	{
		switch(nType)
		{
		case 1:
			{
				if(pTarget->CastCreature()->ValidUser())
					return true;
				break;
			}
		case 2:
			{
				if(pTarget->CastCreature()->IsNPC())
					return true;
				break;
			}
		case 3:
			{
				if(pTarget->CastCreature()->IsSummon())
					return true;
				break;
			}
		case 4:
			{
				if(pTarget->CastCreature()->ValidUser() || pTarget->CastCreature()->IsSummon() || pTarget->CastCreature()->IsPet())
					return true;
				break;
			}
		case 5:
			{
				if(pTarget->CastCreature()->IsBoss() || pTarget->CastCreature()->IsZZoldagu())
					return true;
			}
		case 6:
			{
				if(!pTarget->CastCreature()->IsBoss() && !pTarget->CastCreature()->IsZZoldagu())
					return true;
			}
		}
	}
	return false;
}

bool CSkillOperateCondition_op_is_in_siege::CanUse(CCreature *pCreature, CObject *pTarget)
{
	if(pCreature->ValidCreature())
	{
		if(g_ResidenceDB.GetResidence(nResidenceID)->IsSiegeTime())
			return true;
	}
	return false;
}

bool CSkillOperateCondition_op_hour_lower_than::CanUse(CCreature *pCreature, CObject *pTarget)
{
	if(pCreature->ValidCreature())
	{
		tm timeinfo;
		GetTimeInfo(timeinfo);
		if(hour > timeinfo.tm_hour)
			return true;
	}
	return false;
}

bool CSkillOperateCondition_op_hour_higher_than::CanUse(CCreature *pCreature, CObject *pTarget)
{
	if(pCreature->ValidCreature())
	{
		tm timeinfo;
		GetTimeInfo(timeinfo);
		if(hour < timeinfo.tm_hour)
			return true;
	}
	return false;
}

bool CSkillOperateCondition_op_collected_soul_count::CanUse(CCreature *pCreature, CObject *pTarget)
{
	if(pCreature->ValidUser() && pCreature->pSD->pExData)
	{
		if(pCreature->pSD->nSpiritCounter >= g_SpiritSystem.GetMinReleaseSpiritLimit())
			return true;
	}
	return false;
}

bool CSkillOperateCondition_op_battle_force_count::CanUse(CCreature *pCreature, CObject *pTarget)
{
	if(pCreature->ValidCreature() && pCreature->pSD->pExData)
	{
		if(pCreature->pSD->nBuilder == 1)
			return true;

		if(pCreature->HasAbnormal(174) >= this->nCount)
			return true;
	}
	return false;
}


bool CSkillOperateCondition_op_can_acquire_skill::CanUse(CCreature *pCreature, CObject *pTarget)
{
	if(User *pUser = pCreature->GetUser())
	{
		SkillPchId id(skillpch);
		if( pUser->GetAcquiredSkillLevel(id.skillId) >= id.level )
		{
			return false;
		}
		return true;
	}
	return false;
}

bool CSkillOperateCondition_op_can_get_item::CanUse(CCreature *pCreature, CObject *pTarget)
{
	if(User *pUser = pCreature->GetUser())
	{
		int available = pUser->pSD->nMaxSlot - pUser->pSD->nCurrSlot;
		if(available >= amount)
		{
			return true;
		}
	}
	return false;
}

bool CSkillOperateCondition_op_combat_mode::CanUse(CCreature *pCreature, CObject *pTarget)
{
	if(User *pUser = pCreature->SafeCastUser())
	{
		if(m_Type == 1)
		{
			if(pUser->IsInCombatMode())
			{
				return true;
			}
		}else
		{
			if(pUser->IsInCombatMode())
			{
				return false;
			}
			return true;
		}
	}
	return false;
}

bool CSkillOperateCondition_op_can_update_pledge_skill::CanUse(CCreature *pCreature, CObject *pTarget)
{
	if(User *pUser = pCreature->SafeCastUser())
	{	
		if(pUser->pSD->nIsPledgeLeader)
		{
			if(CPledge *pPledge = pUser->GetPledge())
			{
				int acquiredLevel = 0;
				CPledgeEx *pPledgeEx = pPledge->pSD->pPledgeEx;
				if(pPledgeEx)
				{
					pPledgeEx->m_Lock.Enter();
					PledgeSkillMap::iterator it = pPledgeEx->m_Skills.find(m_SkillId);
					if(it!=pPledgeEx->m_Skills.end())
					{
						acquiredLevel = it->second.skillLevel;
					}
					pPledgeEx->m_Lock.Leave();

					acquiredLevel++;
					const PledgeSkillInfo& psi = g_PledgeSkillDb.GetSkillInfo(m_SkillId, acquiredLevel, false);
					if(psi.skillId > 0)
					{
						if(psi.requiredPledgeLevel <= pPledge->pSD->nLevel)
						{
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

bool CSkillOperateCondition_op_peace_zone::CanUse(CCreature *pCreature, CObject *pTarget)
{
	if(User *pUser = pCreature->SafeCastUser())
	{	
		if(m_InPeaceZone)
		{
			if(pUser->pSD->nInPeaceZone)
			{
				return true;
			}
		}else
		{
			if(!pUser->pSD->nInPeaceZone)
			{
				return true;
			}
		}
	}
	return false;
}