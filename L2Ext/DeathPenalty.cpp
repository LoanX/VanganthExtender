#include "StdAfx.h"
#include "DeathPenalty.h"

CDeathPenalty g_DeathPenalty;

CDeathPenalty::CDeathPenalty() : m_Enabled(FALSE)
{

}

CDeathPenalty::~CDeathPenalty()
{

}
/*
[DeathPenalty]
Enabled=1
PenaltySkillId=5076
MaxPenaltyLevel=15
;Players that has less or equal level wont be affected by death penalty
MaxNewbieLevel=9
;Penalty chance in percents
PenaltyChance=33
;Penalty chance for pk players in percents
PenaltyChancePK=66
*/

void CDeathPenalty::LoadINI()
{
	const TCHAR* section = _T("DeathPenalty");
	m_Enabled = GetPrivateProfileInt(section, _T("Enabled"), FALSE, g_ConfigFile);
	m_PenaltySkillId = GetPrivateProfileInt(section, _T("PenaltySkillId"), 0, g_ConfigFile);
	m_MaxPenaltyLevel = GetPrivateProfileInt(section, _T("MaxPenaltyLevel"), 0, g_ConfigFile);
	m_MaxNewbieLevel = GetPrivateProfileInt(section, _T("MaxNewbieLevel"), 0, g_ConfigFile);
	m_PenaltyChance = GetPrivateProfileInt(section, _T("PenaltyChance"), 0, g_ConfigFile);
	m_PenaltyChancePK = GetPrivateProfileInt(section, _T("PenaltyChancePK"), 0, g_ConfigFile);
}

void CDeathPenalty::Init()
{
	LoadINI();
	if(m_Enabled)
	{
		g_Log.Add(CLog::Blue, "[%s] Feature enabled.", __FUNCTION__);
	}
}

bool CDeathPenalty::OnDieByMob(User *pUser)
{
	guard;

	if(m_Enabled)
	{
		if(pUser->pSD->nLevel > m_MaxNewbieLevel)
		{
			UINT dice = g_Random.RandInt(100);
			UINT chance = m_PenaltyChance;
			if(pUser->pSD->nKarma > 0)
			{
				chance = m_PenaltyChancePK;
			}

			if(dice < chance)
			{
				UINT currentLevel = pUser->pSD->nDeathPenalty;
				if(currentLevel < m_MaxPenaltyLevel)
				{
					currentLevel++;
					pUser->pSD->nDeathPenalty = currentLevel;
					pUser->AcquireSkill(m_PenaltySkillId, currentLevel);
					pUser->SendAcquireSkillToDB(m_PenaltySkillId, currentLevel, false);

					CSystemMessage msg(1916);
					msg.AddNumber(currentLevel);
					pUser->SendSystemMessage(&msg);
					pUser->SendETCBuffStatus();
					unguard;
					return true;
				}
			}
		}
	}

	unguard;
	return false;
}

bool CDeathPenalty::LiftPenalty(User *pUser, double chance)
{
	guard;
	if(m_Enabled)
	{
		if(pUser->ValidUser())
		{
			if(pUser->pSD->nDeathPenalty > 0)
			{
				UINT nPenaltyLevel = pUser->pSD->nDeathPenalty;
				if(nPenaltyLevel <= m_MaxPenaltyLevel)
				{
					nPenaltyLevel--;
					pUser->pSD->nDeathPenalty = nPenaltyLevel;
					if(nPenaltyLevel == 0)
					{
						pUser->DeleteSkill(m_PenaltySkillId);
						pUser->SendDeleteSkillToDB(m_PenaltySkillId);
						CSystemMessage msg(1917);
						pUser->SendSystemMessage(&msg);
					}else
					{				
						pUser->AcquireSkill(m_PenaltySkillId, nPenaltyLevel, TRUE);
						pUser->SendAcquireSkillToDB(m_PenaltySkillId, nPenaltyLevel, true);
						CSystemMessage msg(1916);
						msg.AddNumber(nPenaltyLevel);
						pUser->SendSystemMessage(&msg);
					}
					pUser->ValidateSkillList();
					pUser->ValidateParameters();
					pUser->SendETCBuffStatus();
					unguard;
					return true;
				}else
				{
					//Level overflow
					g_Log.Add(CLog::Error, "[%s] Invalid Penalty Level[%d][%S]!", __FUNCTION__, nPenaltyLevel, pUser->pSD->wszName);
					pUser->pSD->nDeathPenalty = 0;
					pUser->DeleteSkill(m_PenaltySkillId);
					pUser->SendDeleteSkillToDB(m_PenaltySkillId);
					CSystemMessage msg(1917);
					pUser->SendSystemMessage(&msg);
					pUser->ValidateSkillList();
					pUser->ValidateParameters();
					pUser->SendETCBuffStatus();
					unguard;
					return true;
				}
			}
		}
	}
	unguard;
	return false;
}

void CDeathPenalty::ValidatePenalty(User *pUser)
{
	if(m_Enabled)
	{
		if(pUser->ValidUser())
		{
			int nDeathPenalty = pUser->GetAcquiredSkillLevel(m_PenaltySkillId);
			if(nDeathPenalty > 0)
			{
				pUser->pSD->nDeathPenalty = nDeathPenalty;
			}
			else
			{
				pUser->pSD->nDeathPenalty = 0;
			}
		}
	}
}