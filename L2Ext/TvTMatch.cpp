#include "stdafx.h"
#include "TvTMatch.h"
#include "NpcServer.h"
#include "Door.h"
#include "PlayerAction.h"

using namespace TvT;

CMatch::CMatch(TvT::TvTInfo *pInfo) : m_state(TvT::StateNone), m_startTime(0), m_stateTime(0), m_winnerTeam(TvT::TeamNone), m_managerServerId(0), m_killCountBlue(0), m_killCountRed(0)
{
	InitializeCriticalSection(&m_lock);
	m_lpInfo = pInfo;
}

CMatch::~CMatch()
{
	DeleteCriticalSection(&m_lock);
}

void CMatch::Init()
{
	m_startTime = time(NULL);
	m_stateTime = time(NULL);
	m_state = TvT::StateRegistration;
	if(m_lpInfo && m_lpInfo->enabled)
	{
		if(m_lpInfo->registerNpcClassId)
		{
			//spawn npc
			map<wstring, wstring> aiParams;
			aiParams.insert(pair<wstring, wstring>(L"fnHi", L"tvt_manager_001.htm"));
			m_managerServerId = NpcServer::SpawnNpcEx(m_lpInfo->registerNpcClassId, m_lpInfo->registerNpcPos.x, m_lpInfo->registerNpcPos.y, m_lpInfo->registerNpcPos.z, aiParams);
		}
		if(m_lpInfo->registrationStartMsg1.size() > 0)
		{
			Utils::BroadcastToAllUser_Announce(m_lpInfo->registrationStartMsg1.c_str());
		}
		if(m_lpInfo->registrationStartMsg2.size() > 0)
		{
			Utils::BroadcastToAllUser_Announce(m_lpInfo->registrationStartMsg2.c_str());
		}
	}else
	{
		g_Log.Add(CLog::Error, "[%s] Invalid TvT info!", __FUNCTION__);
		m_state = TvT::StateDelete;
	}
}

void CMatch::OnDelete()
{
	if(m_lpInfo->finishTime > 0)
	{
		for(map<UINT, User*>::iterator Iter = m_users.begin();Iter!=m_users.end();Iter++)
		{
			User *pUser = Iter->second;
			if(pUser->ValidUser())
			{
				if(pUser->pED->tvtUser.orgPos.x != 0 || pUser->pED->tvtUser.orgPos.y != 0)
				{
					pUser->StopMove();
					PlayerAction::OnTeleportToLocation(pUser, pUser->pED->tvtUser.orgPos.x, pUser->pED->tvtUser.orgPos.y, pUser->pED->tvtUser.orgPos.z, false);
				}
			}
		}
	}
	if(m_lpInfo->doorList.size() > 0)
	{
		//open doors
		for(list<wstring>::iterator Iter = m_lpInfo->doorList.begin(); Iter!= m_lpInfo->doorList.end();Iter++)
		{
			CDoor *pDoor = g_DoorDB.GetDoor(Iter->c_str());
			if(pDoor)
			{
				pDoor->Open();
			}
		}
	}

	if(m_lpInfo->endEventMsg.size() > 0)
	{
		Utils::BroadcastToAllUser_Announce(m_lpInfo->endEventMsg.c_str());
	}

	for(map<UINT, User*>::iterator Iter = m_users.begin();Iter!=m_users.end();Iter++)
	{
		User *pUser = Iter->second;
		if(pUser->ValidUser())
		{
			pUser->pED->tvtUser.Clear();
			pUser->pSD->teamType = 0;
			pUser->SetUserInfoChanged();
			pUser->SetCharInfoChanged();
		}
	}
	m_users.clear();
	//despawn npc
	if(m_managerServerId)
	{
		CNPC *pNpc = CNPC::GetNPCByServerId(&m_managerServerId);
		if(pNpc)
		{
			if(pNpc->pSD->nAlive)
			{
				pNpc->KillNPC();
			}
		}else
			g_Log.Add(CLog::Error, "[%s] cannot find manager npc", __FUNCTION__);

		m_managerServerId = 0;
	}
}

void CMatch::OnFinish()
{
	m_stateTime = time(NULL);
	if(m_lpInfo->finishTime > 0)
	{
		m_state = TvT::StateFinish;
	}else
	{
		//just end
		m_state = TvT::StateDelete;
	}

	for(map<UINT, User*>::iterator Iter = m_users.begin(); Iter!=m_users.end();Iter++)
	{
		User *pUser = Iter->second;
		if(pUser->ValidUser())
		{
			if(m_lpInfo->rewardId > 0 && m_lpInfo->rewardCount > 0)
			{
				if(m_winnerTeam != TvT::TeamNone && m_winnerTeam == pUser->pED->tvtUser.team)
				{
					if(pUser->IsNowTrade())
						pUser->TradeCancel();

					double count = static_cast<double>(m_lpInfo->rewardCount);
					count *= pUser->pED->itemBonus.Get(m_lpInfo->rewardId);
					pUser->AddItemToInventory(m_lpInfo->rewardId, static_cast<INT32>(count));
					g_Logger.Add(L"TvT Reward[%d][%d] User[%s]", m_lpInfo->rewardId, static_cast<INT32>(count), pUser->pSD->wszName);
				}
			}
			pUser->pED->tvtUser.status = TvT::UserFinishing;
		}
	}
	//broadcast who the winner is
	if(m_winnerTeam == TvT::TeamNone)
	{
		if(m_lpInfo->endMsg[0].size() > 0)
		{
			Utils::BroadcastToAllUser_Announce(m_lpInfo->endMsg[0].c_str());
		}
	}else if(m_winnerTeam == TvT::TeamBlue)
	{
		if(m_lpInfo->endMsg[1].size() > 0)
		{
			Utils::BroadcastToAllUser_Announce(m_lpInfo->endMsg[1].c_str());
		}
	}if(m_winnerTeam == TvT::TeamRed)
	{
		if(m_lpInfo->endMsg[2].size() > 0)
		{
			Utils::BroadcastToAllUser_Announce(m_lpInfo->endMsg[2].c_str());
		}
	}
}

void CMatch::Broadcast(const char *format, ...)
{
	va_list va;
	va_start(va, format);

	char buff[8190];
	int len = Utils::VAssemble(buff, 8190, format, va);
	va_end(va);

	for(map<UINT, User*>::iterator Iter = m_users.begin();Iter!=m_users.end();Iter++)
	{
		User *pUser = Iter->second;
		if(pUser->ValidUser())
		{
			pUser->pSocket->Send("b", len, buff);
		}
	}
}

bool CMatch::ValidateWinner( bool timeout )
{
	if(m_state == TvT::StateFight)
	{
		UINT blueAlive = 0;
		UINT redAlive = 0;
		for(map<UINT, User*>::iterator Iter = m_users.begin();Iter!=m_users.end();Iter++)
		{
			User *pUser = Iter->second;
			if(pUser->ValidUser())
			{
				if(pUser->pED->tvtUser.status == TvT::UserFighting)
				{
					if(pUser->pED->tvtUser.team == TvT::TeamBlue)
						blueAlive++;
					else if(pUser->pED->tvtUser.team == TvT::TeamRed)
						redAlive++;
				}
			}
		}
		
		if(blueAlive == 0 || redAlive == 0)
		{
			if(blueAlive == 0)
				m_winnerTeam = TvT::TeamRed;
			if(redAlive == 0)
				m_winnerTeam = TvT::TeamBlue;
			if(blueAlive == redAlive && blueAlive == 0)
				m_winnerTeam = TvT::TeamNone;

			return true;
		}
		if(timeout)
		{
			if(m_lpInfo->type == TvT::TvTLastTeamStanding)
			{
				if(blueAlive > redAlive)
				{
					m_winnerTeam = TvT::TeamBlue;
				}else if(redAlive > blueAlive)
				{
					m_winnerTeam = TvT::TeamRed;
				}
			}else if(m_lpInfo->type == TvT::TvTDeathMatch)
			{
				if(m_killCountBlue > m_killCountRed)
				{
					m_winnerTeam = TvT::TeamBlue;
				}else if(m_killCountRed > m_killCountBlue)
				{
					m_winnerTeam = TvT::TeamRed;
				}else
				{
					m_winnerTeam = TvT::TeamNone;
				}
			}
		}

	}
	return false;
}

void CMatch::RandomizeTeams()
{
	guard;

	size_t total = m_users.size();
	UINT red = 0;
	UINT blue = 0;

	for(map<UINT, User*>::iterator it = m_users.begin();it!=m_users.end();it++)
	{
		if(User *pUser = it->second->SafeCastUser())
		{
			TeamType team = TeamBlue;
			if(red == blue)
			{
				if(g_Random.RandInt(2) == 1)
				{
					team = TeamRed;
				}
			}else if( blue > red)
			{
				team = TeamRed;
			}
			if(team == TeamRed)
				red++;
			else
				blue++;

			pUser->pED->tvtUser.team = team;
		}
	}

	unguard;
}