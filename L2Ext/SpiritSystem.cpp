#include "StdAfx.h"

CSpiritSystem::CSpiritSystem()
{ 
	MinReleaseSpiritLimit = 0; 
	MaxReleaseSpiritLimit = 0; 
	LevelRangeLimit = 0; 
	MinRequiredLevel = 0; 
	StaticBonus=0; 
	SkillID = 0;
	PercentBonus=0; 

}
CSpiritSystem::~CSpiritSystem(){}

void CSpiritSystem::ValidateSkill(User *pUser)
{
	if(pUser->ValidUser())
	{
		if(SkillID > 0 && pUser->pSD->nLevel >= MinRequiredLevel )
		{
			if(!pUser->HaveSkill(SkillID, 1))
			{
				pUser->AcquireSkill(SkillID, 1);
				pUser->SendAcquireSkillToDB(SkillID, 1, false);
				pUser->ValidateParameters();
			}
		}
	}
}

double CSpiritSystem::GetBonus(User *pHunter, BONUS_TYPE Type)
{
	double dBonus = 0;
	switch(Type)
	{
	case HP:
		 dBonus = (pHunter->pSD->MaxHP / 100 * PercentBonus);
		break;
	case MP:
		dBonus = (pHunter->pSD->MaxMP / 100 * PercentBonus);
		break;
	case CP:
		dBonus = (pHunter->pSD->MaxCP / 100 * PercentBonus);
		break;
	}
	if(dBonus < StaticBonus)
		dBonus = StaticBonus;
	return dBonus;
}
bool CSpiritSystem::CanRelease(User *pHunter)
{
	if(pHunter->ValidUser() && pHunter->olympiadUser.IsInside() == FALSE)
	{
		if(pHunter->pSD->nSpiritCounter >= MinReleaseSpiritLimit)
		{
			return true;
		}else
		{
			pHunter->SendSystemMessage(L"You've got not enough collected spirits.");
		}
	}
	return false;
}
bool CSpiritSystem::CanHunt(User *pHunter, User *pSpirit)
{
	if(pHunter->ValidUser() && pSpirit->ValidUser())
	{
		if(pSpirit->pSD->bGuilty)
		{
			if(pHunter->pSD->nLevel >= MinRequiredLevel && pSpirit->pSD->nLevel >= MinRequiredLevel)
			{
				if( ValidRange(pHunter->pSD->nLevel, pSpirit->pSD->nLevel, LevelRangeLimit) )
				{
					return true;
				}
			}
		}
	}
	return false;
}
void CSpiritSystem::UpdateClientStatus(User *pUser)
{
	CSystemMessage msg(2154);
	msg.AddNumber(pUser->pSD->nSpiritCounter);
	pUser->SendSystemMessage(&msg);
}
void CSpiritSystem::OnHunt(CCreature *pHunter, User *pSpirit)
{
	User *pHunterU = pHunter->GetUser();
	if(pHunter->IsSummon())
		pHunterU = pHunter->CastSummon()->GetMaster();
	if(CanHunt(pHunterU, pSpirit))
	{
		if(pSpirit->pSD->nSpiritCounter > 0)
		{
			pSpirit->pSD->nSpiritCounter--;
			UpdateClientStatus(pSpirit);
		}
		pSpirit->SendSystemMessage(L"You've just lost part of your spirit.");
		pHunterU->pSD->nSpiritCounter++;
		pHunterU->SendSystemMessage(L"You've just hunted new spirit.");
		UpdateClientStatus(pHunterU);
	}
}
void CSpiritSystem::OnRelease(User *pHunter)
{
	if( pHunter->ValidUser() )
	{
		if( CanRelease(pHunter) )
		{
			int nSouls = pHunter->pSD->nSpiritCounter;
			if(MaxReleaseSpiritLimit > 0 && nSouls > MaxReleaseSpiritLimit)
				nSouls = MaxReleaseSpiritLimit;
			double dHPBoost = GetBonus(pHunter, HP) * nSouls;
			double dMPBoost = GetBonus(pHunter, MP) * nSouls;
			double dCPBoost = GetBonus(pHunter, CP) * nSouls;
			double dStat = 0;
			
			dStat = pHunter->pSD->fHP; dStat += dHPBoost;
			if(dStat > pHunter->pSD->MaxHP)
				dStat = pHunter->pSD->MaxHP;
			pHunter->pSD->fHP = dStat;
			dStat = pHunter->pSD->fMP; dStat += dMPBoost;
			if(dStat > pHunter->pSD->MaxMP)
				dStat = pHunter->pSD->MaxMP;
			pHunter->pSD->fMP = dStat;
			dStat = pHunter->pSD->fCP; dStat += dCPBoost;
			if(dStat > pHunter->pSD->MaxCP)
				dStat = pHunter->pSD->MaxCP;
			pHunter->pSD->fCP = dStat;
			pHunter->pSD->nSpiritCounter -= nSouls;
			pHunter->UpdateStatus(CCreature::UPDATE_HP);
			pHunter->UpdateStatus(CCreature::UPDATE_MP);
			pHunter->UpdateStatus(CCreature::UPDATE_CP);
			WCHAR wDest[8];
			wstring wMsg = L"You've just released ";
			wMsg += _itow(nSouls, wDest, 10);
			wMsg += L" spirits.";
			pHunter->SendSystemMessage((PWCHAR)wMsg.c_str());
			UpdateClientStatus(pHunter);
		}
	}
}
void CSpiritSystem::Initialize()
{
	MinReleaseSpiritLimit = g_Config.SpiritSystemInfo.GetMinReleaseSpiritLimit();
	MaxReleaseSpiritLimit = g_Config.SpiritSystemInfo.GetMaxReleaseSpiritLimit();
	LevelRangeLimit = g_Config.SpiritSystemInfo.GetLevelRangeLimit();
	MinRequiredLevel = g_Config.SpiritSystemInfo.GetMinRequiredLevel();
	StaticBonus = g_Config.SpiritSystemInfo.GetStaticBonus();
	PercentBonus = g_Config.SpiritSystemInfo.GetPercentBonus();
	SkillID = g_Config.SpiritSystemInfo.GetSkillID();
	g_Log.Add(CLog::Blue, "[%s] ReleaseSpiritLimit - Max[%d] Min[%d]", __FUNCTION__, MaxReleaseSpiritLimit, MinReleaseSpiritLimit);
	g_Log.Add(CLog::Blue, "[%s] LevelRange[%d] RequiredLevel[%d]", __FUNCTION__, LevelRangeLimit, MinRequiredLevel);
	g_Log.Add(CLog::Blue, "[%s] StaticBonus[%d] PercentBonus[%d]", __FUNCTION__, StaticBonus, PercentBonus);
}