#pragma once

struct DailyPvPInfo
{
	UINT charId;
	time_t timestmap;
	HWID hardwareId;
};

class CDailyPvP
{
	BOOL m_Enabled;
	INT32 m_RequiredPvP;
	INT32 m_MinLevel;
	time_t m_CleanUpTimestamp;
	CSLock m_Lock;
	vector<DailyPvPInfo> m_DB;
	void LoadINI();
public:
	void Init();
	inline BOOL IsEnabled() { return m_Enabled; };
	void OnDBLoad(UINT charId, LPBYTE lpHWID, UINT timestamp);
	bool GetPoint(User *pKiller, User *pEnemy);
	void TimerExpired(User *pUser);
	void TimerExpired();
	void ShowPoints(User *pUser);
};

extern CDailyPvP g_DailyPvP;