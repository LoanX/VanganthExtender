#include "StdAfx.h"
#include "SpawnProtection.h"

CSpawnProtection g_SpawnProtection;

void CSpawnProtection::Initialize()
{
	LoadINI();
	if(m_Enabled)
	{
		g_Log.Add(CLog::Blue, "[%s] Feature enabled, timeout[%d].", __FUNCTION__, m_TimeoutTime);
	}
}

void CSpawnProtection::LoadINI()
{
	const TCHAR* section = _T("SpawnProtection");
	m_Enabled = GetPrivateProfileInt(section, _T("Enabled"), 0, g_ConfigFile) ? true : false;
	m_DeactivateOnMoving = GetPrivateProfileInt(section, _T("DeactivateOnMoving"), 1, g_ConfigFile) ? true : false;
	m_SkillId = GetPrivateProfileInt(section, _T("SkillId"), 10011, g_ConfigFile);
	m_TimeoutTime = GetPrivateProfileInt(section, _T("TimeoutTime"), 0, g_ConfigFile);
	//2514	1	a,The effect of Spawn Protection will expire in $s1 second(s).\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0

}

void CSpawnProtection::ActivateProtection(User *pUser)
{
	guard;
	if(pUser->ValidCreature() && !pUser->pSD->nInPeaceZone && !pUser->pSD->nInBattleField && pUser->olympiadUser.GetState() == 0 && !pUser->pSD->nIsObserver)
	{
		if(m_TimeoutTime)
		{
			pUser->pED->spawnProtectionTimeout = static_cast<UINT>(time(0) + m_TimeoutTime);
		}
		pUser->AcquireSkill(m_SkillId, 1);
		pUser->pED->spawnProtection = 2;
		pUser->ValidateParameters();
		//2512	1	a,The effects of Spawn Protection flow through you.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
		pUser->pSocket->SendSystemMessage(2512);
		if(CSummon *pSummon = pUser->GetSummonOrPet())
		{
			pSummon->AcquireSkill(m_SkillId, 1);
			pSummon->ValidateParameters();
		}
	}
	unguard;
}

void CSpawnProtection::DeactivateProtection(User *pUser)
{
	guard;
	if(pUser->ValidUser())
	{
		if(pUser->pED->spawnProtection == 2)
		{
			pUser->DeleteSkill(m_SkillId);
			pUser->pED->spawnProtection = 0;
			pUser->pED->spawnProtectionTimeout = 0;
			pUser->ValidateParameters();
			//2513	1	a,The effect of Spawn Protection has been removed.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
			pUser->pSocket->SendSystemMessage(2513);
			if(CSummon *pSummon = pUser->GetSummonOrPet())
			{
				pSummon->DeleteSkill(m_SkillId);
				pSummon->ValidateParameters();
			}
		}else if(pUser->pED->spawnProtection == 1)
		{
			pUser->pED->spawnProtection = 0;
			pUser->pED->spawnProtectionTimeout = 0;
		}
	}
	unguard;
}