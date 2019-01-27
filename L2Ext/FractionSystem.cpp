#include "stdafx.h"
#include "FractionSystem.h"
#include "DB.h"

CFractionSystem g_FractionSystem;

CFractionSystem::CFractionSystem() : m_enabled(FALSE)
{
}

CFractionSystem::~CFractionSystem()
{
}

void CFractionSystem::Init()
{
	guard;
	m_lock.Enter();
	m_enabled = FALSE;
	LoadINI();
	m_lock.Leave();

	if(m_enabled)
	{
		g_Log.Add(CLog::Blue, "Fraction system is enabled. Loaded [%d] fraction(s).", m_fractions.size());

		for(map<UINT, CFraction*>::iterator it = m_fractions.begin();it!=m_fractions.end();it++)
		{
			if(it->second->IsEnabled())
			{
				g_DB.RequestLoadFractionData(it->first);
			}
		}
	}

	unguard;
}


void CFractionSystem::LoadINI()
{
	TCHAR file[260];
	GetCurrentDirectory(MAX_PATH, file);
	lstrcat(file, TEXT("\\Fraction.ini"));
	const TCHAR* section = _T("Setting");
	m_enabled = GetPrivateProfileInt(section, _T("Enabled"), FALSE, file);
	if(m_enabled)
	{
		UINT count = GetPrivateProfileInt(section, _T("Count"), 0, file);

		for(UINT n=0;n<count;n++)
		{
			CFraction *pFraction = new CFraction();
			pFraction->Init(n+1);
			m_fractions.insert(pair<UINT, CFraction*>(pFraction->GetId(), pFraction));
		}
	}
}

void CFractionSystem::OnDie(User *pUser, CCreature *pKiller)
{
	guard;
	if(m_enabled)
	{
		User *pEnemy = pKiller->GetUser();
		if(pEnemy == 0 && (pKiller->IsSummon() || pKiller->IsPet()))
		{
			pEnemy = pKiller->CastSummon()->GetMaster();
		}

		if(pUser->pED->fractionUser.fractionId > 0 && pEnemy->ValidUser())
		{
			if(pEnemy->pED->fractionUser.fractionId > 0 && pEnemy->pED->fractionUser.fractionId != pUser->pED->fractionUser.fractionId)
			{
				if(!memcmp(pEnemy->pSocket->pED->longHWID.hash, pUser->pSocket->pED->longHWID.hash, 32) || pEnemy->pSocket->addr.S_un.S_addr != pUser->pSocket->addr.S_un.S_addr)
				{
					map<UINT, CFraction*>::iterator it = m_fractions.find(pEnemy->pED->fractionUser.fractionId);
					if(it!=m_fractions.end())
					{
						it->second->OnKillPC(pEnemy, pUser);
					}
				}
			}
		}
	}
	unguard;
}

void CFractionSystem::Register(User *pUser, UINT id)
{
	guard;
	if(m_enabled)
	{
		FractionUser& fu = pUser->pED->fractionUser;
		if(fu.fractionId == 0)
		{
			map<UINT, CFraction*>::iterator it = m_fractions.find(id);
			if(it!=m_fractions.end())
			{
				it->second->Register(pUser);
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Invalid fraction id[%d] user[%S]", __FUNCTION__, id, pUser->pSD->wszName);
			}
		}else
		{
			pUser->SendSystemMessage(L"You already signed up to a fraction!");
		}
	}else
	{
		pUser->SendSystemMessage(L"Fraction System is disabled.");
	}
	unguard;
}

void CFractionSystem::Unregister(User *pUser)
{
	guard;
	if(m_enabled)
	{
		FractionUser& fu = pUser->pED->fractionUser;
		if(fu.fractionId > 0)
		{
			INT32 penaltyTime = 0;
			map<UINT, CFraction*>::iterator it = m_fractions.find(fu.fractionId);
			if(it!=m_fractions.end())
			{
				penaltyTime = it->second->GetLeavePenaltyTime();
			}
			fu.Clear();
			fu.leaveTimestamp = time(0) + penaltyTime;
			pUser->SendSystemMessage(L"You have been removed from fraction.");
			pUser->SetUserInfoChanged();
			pUser->SetCharInfoChanged();
			g_DB.RequestSaveUserFraction(pUser);
		}else
		{
			pUser->SendSystemMessage(L"You're not in any fraction at the moment.");
		}
	}
	unguard;
}

void CFractionSystem::OnDie(CNPC *pNpc, CCreature *pKiller)
{
	guard;
	if(m_enabled)
	{
		User *pEnemy = pKiller->GetUser();
		if(pEnemy == 0 && (pKiller->IsSummon() || pKiller->IsPet()))
		{
			pEnemy = pKiller->CastSummon()->GetMaster();
		}

		if(pEnemy->ValidUser())
		{
			if(pEnemy->pED->fractionUser.fractionId > 0)
			{
				map<UINT, CFraction*>::iterator it = m_fractions.find(pEnemy->pED->fractionUser.fractionId);
				if(it!=m_fractions.end())
				{
					it->second->OnKillNpc(pEnemy, pNpc);
				}else
				{
					g_Log.Add(CLog::Error, "[%s] Invalid fraction id[%d] user[%S]!", __FUNCTION__, pEnemy->pED->fractionUser.fractionId, pEnemy->pSD->wszName);
				}
			}
		}
	}
	unguard;
}

void CFractionSystem::TimerExpired()
{
	guard;
	if(m_enabled)
	{
		for(map<UINT, CFraction*>::iterator it = m_fractions.begin();it!=m_fractions.end();it++)
		{
			it->second->TimerExpired();
		}
	}
	unguard;
}

void CFractionSystem::DBLoad(UINT fractionId, INT32 points)
{
	guard;
	if(m_enabled)
	{
		map<UINT, CFraction*>::iterator it = m_fractions.find(fractionId);
		if(it!=m_fractions.end())
		{
			it->second->DBLoad(points);
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Invalid fraction[%d]!", __FUNCTION__, fractionId);
		}
	}
	unguard;
}

void CFractionSystem::OnBecomeHero(User *pUser)
{
	guard;
	if(m_enabled)
	{
		if(pUser->pED->fractionUser.fractionId > 0)
		{
			map<UINT, CFraction*>::iterator it = m_fractions.find(pUser->pED->fractionUser.fractionId);
			if(it!=m_fractions.end())
			{
				it->second->OnBecomeHero(pUser);
			}
		}
	}
	unguard;
};