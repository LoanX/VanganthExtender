#pragma once

class CUserDB : public CThreadSafe
{
	map<UINT, User*> m_byServerId;
	map<UINT, User*> m_byDatabaseId;
	map<wstring, User*> m_byName;
	CSLock m_InstanceLock;
	map<UINT, map<HWID, UINT>> m_InstanceByHWID;

	INT32 nExpTable[82];
	map<wstring, int> mClass;
public:
	void Initialize();
	map<wstring, int>& GetClassMap()
	{
		return mClass;
	}
	
	int GetClassID(wstring wName);
	map<UINT, User*>& GetUserMap() { return m_byServerId; };
	void AddUser(User *pUser);
	void DelUser(User *pUser);
	void DelUser(UINT serverId);
	User* GetUserByName(wstring wName);
	User* GetUserByDBID(UINT databaseId);
	User* GetUserByServerId(UINT serverId);
	INT32 GetExp(int nLevel);
	void DeleteItemInInventory(IntList& lItems);
	void KickOutFromColiseum(IVector pos);
	void KickOutFromRange(IVector pos, int minX, int maxX, int minY, int maxY);
	bool AnyPrivateStoreInRange(FVector pos, double range);
	void GiveItemToAllUser(INT32 itemId, INT32 count, BOOL checkIp=FALSE);
	void GetUsersByIp(UINT ip, vector<User*>& vUsers);
	void GetUsersByHwid(LPBYTE lpHwid, vector<User*>& vUsers);
	void GetUsersByIpHwid(UINT ip, LPBYTE lpHwid, vector<User*>& vUsers);
	INT32 GetOlympiadUserAmountByIPnHWID(UINT ip, LPBYTE lpHwid);
	void SetInstanceTimeout(HWID hwid, UINT instanceId, UINT timeout);
	UINT GetInstanceTimeout(HWID hwid, UINT instanceId);
};

extern CUserDB g_UserDB;

#define FOR_ALL_USERS_IN_MAP(function)	\
	g_UserDB.Lock();	\
	map<UINT, User*>& mData = g_UserDB.GetUserMap();	\
	for(map<UINT, User*>::iterator Iter = mData.begin(); Iter!=mData.end(); Iter++)	\
	{	\
		function(Iter->second->GetUser());	\
	}	\
	g_UserDB.Unlock();