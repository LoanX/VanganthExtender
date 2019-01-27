#pragma once
#include "TerritoryEx.h"

class CUserCache
{
	struct UserData
	{
		UINT databaseId;
		WCHAR nickname[25];
		FVector lastPosition;
		BOOL isOnline;
	};
	CSLock m_lock;
	map<UINT, UserData> m_users;
public:
	CUserCache();
	~CUserCache();
	void OnEnterWorld(User *pUser);
	void OnLeaveWorld(User *pUser);
	void InstantTeleportInMyTerritory(int destX, int destY, int destZ, int range, vector<CTerritoryEx>& territories, vector<CTerritoryEx>& bannedTerritories, bool online = false);
};

extern CUserCache g_UserCache;