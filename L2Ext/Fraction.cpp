#include "stdafx.h"
#include "Fraction.h"
#include "DB.h"

CFraction::CFraction() : m_enabled(FALSE), m_loadedFromDb(FALSE)
{
}

CFraction::~CFraction()
{
}

void CFraction::Init(UINT id)
{
	guard;

	m_lock.Enter();
	m_fractionId = id;
	TCHAR section[255];
	swprintf(section, 255, L"Fraction_%d", id);
	TCHAR file[260];
	GetCurrentDirectory(MAX_PATH, file);
	lstrcat(file, TEXT("\\Fraction.ini"));

	m_enabled = GetPrivateProfileInt(section, _T("Enabled"), 0, file);
	GetPrivateProfileString(section, _T("FractionName"), 0, m_fractionName, 256, file);
	m_fractionCoinId = GetPrivateProfileInt(section, _T("FractionCoinId"), 0, file);
	m_maxMembers = GetPrivateProfileInt(section, _T("MaxMembers"), 1000, file);
	m_minRequiredLevel = GetPrivateProfileInt(section, _T("MinRequiredLevel"), 0, file);
	m_requiredItemId = GetPrivateProfileInt(section, _T("RequiredItemId"), 0, file);
	m_requiredItemCount = GetPrivateProfileInt(section, _T("RequiredItemCount"), 0, file);
	m_leavePenaltyTime = GetPrivateProfileInt(section, _T("LeavePenaltyTime"), 0, file);
	GetPrivateProfileString(section, _T("TitleAddon"), 0, m_titleAddon, 25, file);
	m_titleColor = GetPrivateProfileInt(section, _T("TitleColor"), 0, file);
	GetPrivateProfileString(section, _T("NameAddon"), 0, m_nameAddon, 25, file);
	m_nameColor = GetPrivateProfileInt(section, _T("NameColor"), 0, file);

	m_epicBosses.clear();
	TCHAR temp[8190];
	if(GetPrivateProfileString(section, _T("EpicBosses"), 0, temp, 8190, file))
	{
		tstringstream dataStream(temp);
		INT32 bossId = 0;
		while(dataStream >> bossId)
		{
			m_epicBosses.push_back(bossId);
		}
	}

	m_epicBossPoints = GetPrivateProfileInt(section, _T("EpicBossPoints"), 0, file);
	m_raidBossPoints = GetPrivateProfileInt(section, _T("RaidBossPoints"), 0, file);
	m_normalMobPoints = GetPrivateProfileInt(section, _T("NormalMobPoints"), 0, file);
	m_normalMobChance = GetPrivateProfileDouble(section, _T("NormalMobChance"), 0, file);
	m_normalMobCoinLimitPerHour = GetPrivateProfileInt(section, _T("NormalMobCoinLimitPerHour"), 0, file);
	m_heroPoints = GetPrivateProfileDouble(section, _T("HeroPoints"), 0, file);
	
	m_winSiegePoints = GetPrivateProfileDouble(section, _T("WinSiegePoints"), 0, file);
	m_rewardOtherFractionKill = GetPrivateProfileDouble(section, _T("RewardOtherFractionKill"), 0, file);
	m_normalMobCoinDropTimestmap = time(0);
	m_lock.Leave();
	unguard;
}

void CFraction::OnKillNpc(User *pUser, CNPC *pNpc)
{
	guard;
	if(m_enabled)
	{
		if(pNpc->pSD->nLevel + 9 > pUser->pSD->nLevel)
		{
			INT32 itemId = 0;
			INT32 itemCount = 0;
			m_lock.Enter();
			if(pNpc->IsBoss())
			{
				bool epic = false;
				for(INT32 n=0;n<m_epicBosses.size();n++)
				{
					if(m_epicBosses[n] == pNpc->pSD->nNpcClassID)
					{
						epic = true;
						break;
					}
				}
				if(epic)
				{
					if(m_epicBossPoints > 0)
					{
						m_totalPoints += m_epicBossPoints;
						//broadcast message
					}
					if(m_fractionCoinId > 0 && m_epicBossPoints > 0)
					{
						itemId = m_fractionCoinId;
						itemCount = m_epicBossPoints;
					}
				}else
				{
					if(m_raidBossPoints > 0)
					{
						m_totalPoints += m_raidBossPoints;
						//broadcast message
					}
					if(m_fractionCoinId > 0 && m_raidBossPoints > 0)
					{
						itemId = m_fractionCoinId;
						itemCount = m_raidBossPoints;
					}
				}
			}else if(pNpc->IsSummon() == false && pNpc->IsPet() == false)
			{
				if(m_normalMobCoinDropCount < m_normalMobCoinLimitPerHour)
				{
					m_normalMobCoinDropCount++;
					double dice = g_Random.RandDouble(100.0);
					if(dice < m_normalMobChance)
					{
						if(m_normalMobPoints > 0)
						{
							m_totalPoints += m_normalMobPoints;
						}
						if(m_fractionCoinId > 0 && m_normalMobPoints > 0)
						{
							itemId = m_fractionCoinId;
							itemCount = m_normalMobPoints;
						}
					}
				}
			}
			m_lock.Leave();
			if(itemId > 0 && itemCount > 0)
			{
				pNpc->AddItemToInventory(itemId, itemCount);
			}
		}
	}
	unguard;
}

void CFraction::DBLoad(INT32 totalPoints)
{
	guard;
	if(m_enabled)
	{
		m_lock.Enter();
		m_loadedFromDb = TRUE;
		m_totalPoints = totalPoints;
		m_lock.Leave();
	}
	unguard;
}

void CFraction::TimerExpired()
{
	guard;
	if(m_enabled)
	{
		UINT currentTime = time(0);
		m_lock.Enter();
		if(m_normalMobCoinDropTimestmap < time(0))
		{
			m_normalMobCoinDropTimestmap = time(0);
			m_normalMobCoinDropCount = 0;
		}
		if( m_loadedFromDb && m_dbSaveTimestamp < currentTime)
		{
			m_dbSaveTimestamp = currentTime + 30;
			g_DB.RequestSaveFractionData(m_fractionId, m_totalPoints, 0);
		}
		m_lock.Leave();
	}
	unguard;
}

void CFraction::OnBecomeHero(User *pUser)
{
	guard;
	if(m_enabled)
	{
		INT32 points = 0;
		INT32 itemId = 0;
		INT32 itemCount = 0;
		m_lock.Enter();
		if(m_heroPoints > 0)
		{
			m_totalPoints += m_heroPoints;
			points = m_heroPoints;
		}
		if(m_fractionCoinId > 0 && m_heroPoints > 0)
		{
			itemId = m_fractionCoinId;
			itemCount = m_heroPoints;
		}
		m_lock.Leave();
		if(points > 0)
		{
			CSystemMessage msg(MessageString);
			wstringstream msgStream;
			msgStream << pUser->pSD->wszName << L" acquired hero status and earned " << points << L" point(s) for fraction.";
			msg.AddText(msgStream.str().c_str());
			Broadcast(msg.GetSize(), msg.GetBuff());
		}
		if(itemId > 0 && itemCount > 0)
		{
			if(pUser->IsNowTrade())
			{
				pUser->TradeCancel();
			}
			pUser->AddItemToInventory(itemId, itemCount);
		}
	}
	unguard;
}

void CFraction::OnWinCastleSiege(User *pUser)
{
	guard;
	if(m_enabled)
	{
		INT32 points = 0;
		m_lock.Enter();
		if(m_winSiegePoints > 0)
		{
			m_totalPoints += m_winSiegePoints;
			points = m_winSiegePoints;
		}
		m_lock.Leave();
		if(points > 0)
		{
			if(CPledge *pPledge = pUser->GetPledge())
			{
				CSystemMessage msg(MessageString);
				wstringstream msgStream;
				msgStream << pPledge->pSD->wszClanName << L" won the castle siege and earned " << points << L" point(s) for fraction.";
				msg.AddText(msgStream.str().c_str());
				Broadcast(msg.GetSize(), msg.GetBuff());
			}
		}
	}
	unguard;
}

void CFraction::Broadcast(UINT len, PCHAR buff)
{
	guard;
	if(m_enabled)
	{
		m_lock.Enter();
		for(map<UINT, User*>::iterator it = m_members.begin(); it!=m_members.end();it++)
		{
			if(User *pUser = it->second->GetUser())
			{
				pUser->pSocket->Send("b", len, buff);
			}
		}
		m_lock.Leave();
	}
	unguard;
}

bool CFraction::Register(User *pUser)
{
	guard;
	bool ret = false;
	if(m_enabled)
	{
		if(pUser->pED->fractionUser.leaveTimestamp < time(0))
		{
			if(pUser->pED->fractionUser.fractionId == 0)
			{
				if(m_minRequiredLevel >= pUser->pSD->nLevel)
				{ 
					if(m_requiredItemId > 0 && m_requiredItemCount > 0)
					{
						if(pUser->inventory.HaveItemByClassID(m_requiredItemId, m_requiredItemCount))
						{
							pUser->DeleteItemInInventory(m_requiredItemId, m_requiredItemCount);
						}else
						{
							pUser->SendSystemMessage(L"You don't have enough required item(s) to join this fraction.");
							unguard;
							return false;
						}
					}
					Join(pUser);
					ret = true;
				}else
				{
					pUser->SendSystemMessage(L"Your level is too low to join this fraction.");
				}
			}else
			{
				pUser->SendSystemMessage(L"You already are in a fraction.");
			}
		}else
		{
			pUser->SendSystemMessage(L"You cannot join any fraction at the moment.");
		}
	}
	unguard;
	return ret;
}

void CFraction::Join(User *pUser)
{
	guard;
	pUser->pED->fractionUser.fractionId = m_fractionId;
	pUser->pED->fractionUser.leaveTimestamp = 0;
	pUser->pED->fractionUser.points = 0;
	pUser->SendSystemMessage(L"You have been registered in a fraction.");
	pUser->SetUserInfoChanged();
	pUser->SetCharInfoChanged();
	g_DB.RequestSaveUserFraction(pUser);
	unguard;
}

void CFraction::OnKillPC(User *pKiller, User *pVictim)
{
	guard;
	if(m_enabled && pKiller->ValidUser() && pVictim->ValidUser() && m_rewardOtherFractionKill)
	{
		//check for last timestamp
		UINT timestamp = 0;
		UINT currentTime = time(0);
		bool reward = false;
		FractionUser& killer = pKiller->pED->fractionUser;
		killer.lock.Enter();
		map<UINT, UINT>::iterator it = killer.killedPlayerTimestamps.find(pVictim->nDBID);
		if(it!=killer.killedPlayerTimestamps.end())
		{
			timestamp = it->second;
		}

		if((timestamp + 1800) < currentTime)
		{
			//update timestamp
			killer.killedPlayerTimestamps.insert(pair<UINT, UINT>(pVictim->nDBID, currentTime));
			reward = true;
		}
		killer.lock.Leave();

		if(reward)
		{
			m_lock.Enter();
			m_totalPoints += 1;
			m_lock.Leave();
			if(m_fractionCoinId > 0)
			{
				if(pKiller->IsNowTrade())
				{
					pKiller->TradeCancel();
				}
				pKiller->AddItemToInventory(m_fractionCoinId, 1);
			}
		}
	}
	unguard;
}