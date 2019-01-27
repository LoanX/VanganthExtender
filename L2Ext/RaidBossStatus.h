#pragma once


struct RaidBossInfo
{
	wstring name;
	wstring dbName;
	bool alive;
	IVector pos;
	UINT deathTime;
	UINT staticRespawnTime;
	RaidBossInfo() : alive(false), deathTime(0), staticRespawnTime(0) {}
};

struct RaidBossRespawnAnnounce
{
	wstring npcName;
	UINT announceTime;
	wstring message;
};

class CRaidBossStatus
{
	BOOL m_Enabled;
	CSLock m_Lock;
	UINT m_RefreshTimeout;
	time_t m_RefreshTime;
	vector<wstring> m_DBNames;
	map<wstring, RaidBossRespawnAnnounce> m_Announce;
	map<wstring, RaidBossInfo> m_DB;
	map<wstring, UINT> m_StaticDB;
	vector<wstring> m_Pages;
	wstring m_AliveColor;
	wstring m_DeadColor;
	void LoadINI();
	UINT GetStaticRespawn(wstring name);
	UINT GetDeathTime(wstring name);
public:
	void Init();
	void TimerExpired();
	void OnDBLoad(wstring name, int x, int y, int z, bool alive, UINT deathTime);
	void RequestRaidBossStatus(User *pUser, UINT page = 0);
	void GeneratePages();
};

extern CRaidBossStatus g_RaidBossStatus;