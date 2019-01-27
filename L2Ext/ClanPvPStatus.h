#pragma once

class CClanPvPStatus
{
	typedef struct {
		UINT pledgeId;
		wstring pledgeName;
		UINT pvpCount;
	} PvPStatus;

	bool m_Enabled;
	CSLock m_Lock;
	time_t m_RefreshTimeout;
	time_t m_RefreshTime;
	UINT m_PledgeLevel;
	UINT m_TopLimit;
	typedef vector<PvPStatus> StatusVector;
	StatusVector m_Status;
	vector<wstring> m_Pages;
	void LoadINI();
public:
	void Init();
	void TimerExpired();
	void OnDBLoadPvP(UINT pledgeId, const WCHAR* wName, UINT pvpCount);
	void RequestPvPStatus(User *pUser, UINT page = 0);
	void GeneratePagesPvP();
	inline bool Enabled() const { return m_Enabled; }
};

extern CClanPvPStatus g_ClanPvPStatus;