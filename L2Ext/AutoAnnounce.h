#pragma once


struct AutoAnnounceData
{
	wstring announce;
	BOOL onEnterWorld;
	time_t interval;	//in seconds
	time_t nextAnnounceTime;
};

class CAutoAnnounce
{
	CSLock m_lock;
	BOOL m_enabled;
	BOOL m_reload;
	vector<AutoAnnounceData> m_data;
	wstring m_BossAnnounce;
	vector<INT32> m_BossList;
	void LoadINI();
public:
	CAutoAnnounce();
	~CAutoAnnounce();
	inline void SetReload() { m_reload = TRUE; };
	void Init();
	void TimerExpired();
	void OnEnterWorld(User *pUser);
	void OnBossEnterWorld(UINT npcClassId);
};

extern CAutoAnnounce g_AutoAnnounce;