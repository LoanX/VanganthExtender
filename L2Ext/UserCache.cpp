#include "stdafx.h"
#include "UserCache.h"
#include "DB.h"
#include "Geo.h"

CUserCache g_UserCache;

CUserCache::CUserCache()
{

}

CUserCache::~CUserCache()
{
}

void CUserCache::OnEnterWorld(User *pUser)
{
	guard;

	if(pUser->ValidUser())
	{
		m_lock.Enter();
		map<UINT, UserData>::iterator it = m_users.find(pUser->nDBID);
		if(it!=m_users.end())
		{
			UserData& ud = it->second;
			ud.isOnline = TRUE;
			ud.lastPosition = pUser->pSD->Pos;
			wcscpy_s(ud.nickname, 25, pUser->pSD->wszName);
		}else
		{
			UserData ud;
			ud.databaseId = pUser->nDBID;
			ud.isOnline = TRUE;
			ud.lastPosition = pUser->pSD->Pos;
			wcscpy_s(ud.nickname, 25, pUser->pSD->wszName);
			m_users.insert(pair<UINT, UserData>(pUser->nDBID, ud));
		}
		m_lock.Leave();
	//	g_Log.Add(CLog::Blue, "[%s] User[%S] Enter World", __FUNCTION__, pUser->pSD->wszName);
	}

	unguard;
}

void CUserCache::OnLeaveWorld(User *pUser)
{
	guard;

	if(pUser->ValidUser())
	{
		m_lock.Enter();
		map<UINT, UserData>::iterator it = m_users.find(pUser->nDBID);
		if(it!=m_users.end())
		{
			UserData& ud = it->second;
			ud.isOnline = FALSE;
			ud.lastPosition = pUser->pSD->Pos;
		}else
		{
			g_Log.Add(CLog::Error, "[%s] No User[%S] in Cache!", __FUNCTION__, pUser->pSD->wszName);
		}
		m_lock.Leave();
	//	g_Log.Add(CLog::Blue, "[%s] User[%S] Leave World", __FUNCTION__, pUser->pSD->wszName);
	}

	unguard;
}

void CUserCache::InstantTeleportInMyTerritory(int destX, int destY, int destZ, int range, std::vector<CTerritoryEx>& territories, vector<CTerritoryEx>& bannedTerritories, bool online)
{
	guard;

	destZ = CGeo::GetInstance()->GetZLoc(destX, destY, destZ);

	PUINT pWorldId = reinterpret_cast<PUINT>(0xBF0384L);
	UINT worldId = (*pWorldId);

	m_lock.Enter();

	for(map<UINT, UserData>::iterator iter = m_users.begin();iter!=m_users.end();iter++)
	{
		if(iter->second.isOnline == FALSE)
		{
			bool inTerritory = false;
			bool skip = false;
			for(UINT n=0;n<bannedTerritories.size();n++)
			{
				if(bannedTerritories[n].IsInside(iter->second.lastPosition))
				{
					skip = true;
					break;
				}
			}
			if(skip)
			{
				continue;
			}
			for(UINT n=0;n<territories.size();n++)
			{
				if(territories[n].IsInside(iter->second.lastPosition))
				{
					g_DB.RequestChangePos(iter->second.nickname, worldId, destX, destY, destZ);
					g_Logger.Add(L"InstantTeleportInMyTerritory for offline palyer[%s] to %d, %d, %d", iter->second.nickname, destX, destY, destZ);
					break;
				}
			}
		}
	}

	m_lock.Leave();



	unguard;
}