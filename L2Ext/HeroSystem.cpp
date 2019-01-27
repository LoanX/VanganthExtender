#include "stdafx.h"
#include "HeroSystem.h"
#include "HookManager.h"
#include "FractionSystem.h"

CHeroSystem g_HeroSystem;

CHeroSystem::CHeroSystem() : m_enabled(FALSE)
{
}

CHeroSystem::~CHeroSystem()
{
}

void CHeroSystem::Init()
{
	LoadINI();
	if(m_enabled)
	{
		g_Log.Add(CLog::Blue, "Initializing Hero System");
		g_HookManager.WriteMemoryDWORD(0x980A60, (UINT)CHeroSystem::AtomicSetHero_Do);
	}
}

/*
[HeroSystem]
Enabled=1
;Skill pch ids - heroic miracle, berserker, valor, granduer, dread
AcquireSkillList=101121 101377 351745 352001 352257
ReputationReward=1000
ItemRewardId=0
ItemRewardCount=0
*/

void CHeroSystem::LoadINI()
{
	const TCHAR* section = _T("HeroSystem");
	m_enabled = GetPrivateProfileInt(section, _T("Enabled"), FALSE, g_ConfigFile);
	m_reputationReward = GetPrivateProfileInt(section, _T("ReputationReward"), 1000, g_ConfigFile);
	m_rewardId = GetPrivateProfileInt(section, _T("ItemRewardId"), 0, g_ConfigFile);
	m_rewardCount = GetPrivateProfileInt(section, _T("ItemRewardCount"), 0, g_ConfigFile);

	TCHAR temp[8190];
	if(GetPrivateProfileString(section, _T("AcquireSkillList"), 0, temp, 8190, g_ConfigFile))
	{
		tstringstream sstr;
		sstr << temp;
		SkillPchId id;
		while( sstr >> id.pchId)
		{
			CSkillKey key(id.skillId, id.level);
			m_skills.push_back(key);
		}
	}
}

bool CHeroSystem::AtomicSetHero_Do(AtomicSetHero *pAtomic)
{
	guard;
	typedef bool (*f)(AtomicSetHero*);

	bool ret = f(0x4363D0L)(pAtomic);
//	g_Log.Add(CLog::Blue, "[%s] 18[%d] playerId[%d] heroType[%d] ret[%d]", __FUNCTION__, pAtomic->unkn18, pAtomic->playerId, pAtomic->heroType, ret);
	if(ret)
	{
		User *pUser = User::GetUserBySID(&pAtomic->playerId);
		if(pUser->ValidUser())
		{
			if(pAtomic->heroType == 2)
			{
				g_Log.Add(CLog::Blue, "Player [%S] received hero status.", pUser->pSD->wszName);
				if(pUser->pSD->nSubClassIndex == 0)
				{
					g_HeroSystem.AcquireHeroSkills(pUser);
				}

				if(pUser->IsNowTrade())
				{
					pUser->TradeCancel();
				}
				if(g_HeroSystem.m_rewardId > 0 && g_HeroSystem.m_rewardCount > 0)
				{
					pUser->AddItemToInventory(g_HeroSystem.m_rewardId, g_HeroSystem.m_rewardCount);
				}
				CPledge *pPledge = pUser->GetPledge();
				if(pPledge && g_HeroSystem.m_reputationReward > 0)
				{
					pPledge->NotifyMemberNamedHero(pUser->pSD->wszName);
					pPledge->UpdatePledgeNameValue( g_HeroSystem.m_reputationReward );
				}
				g_FractionSystem.OnBecomeHero(pUser);
			}
		}
	}

	unguard;

	return ret;
}

void CHeroSystem::TimerExpired(User *pUser)
{
	guard;
	if(m_enabled)
	{
		if(pUser->pSD->nSubClassIndex == 0 && pUser->pSD->nHero == 2)
		{
			//check if player has hero skills
			if(m_skills.size() > 0)
			{
				if(!pUser->HaveSkill(m_skills[0].skill_id, m_skills[0].level))
				{
					AcquireHeroSkills(pUser);
				}
			}
		}else if(pUser->pSD->nHero != 2 || pUser->pSD->nSubClassIndex != 0)
		{
			if(m_skills.size() > 0)
			{
				bool done = false;
				for(UINT n=0;n<m_skills.size();n++)
				{
					if(pUser->HaveSkill(m_skills[n].skill_id, m_skills[n].level))
					{
						pUser->DeleteSkill(m_skills[n].skill_id);
						pUser->SendDeleteSkillToDB(m_skills[n].skill_id);
						done = true;
					}
				}
				if(done)
				{
					pUser->ValidateParameters();
					g_Log.Add(CLog::Blue, "Deleting hero skills from user[%S] subjob index[%d]", pUser->pSD->wszName, pUser->pSD->nSubClassIndex);					
				}
			}
		}
	}
	unguard;
}

void CHeroSystem::AcquireHeroSkills(User *pUser)
{
	guard;
	if(pUser->ValidUser())
	{
		for(UINT n=0;n< m_skills.size();n++)
		{
			pUser->AcquireSkill(m_skills[n].skill_id, m_skills[n].level);
			pUser->SendAcquireSkillToDB(m_skills[n].skill_id, m_skills[n].level);
		}
		pUser->ValidateParameters();
	}
	unguard;
}