#pragma once


struct PKPvPUserInfo
{
	wstring name;
	UINT classId;
	INT32 level;
	INT32 pvpAmount;
	INT32 pkAmount;
};

class CPKPvPStatus
{
	BOOL m_Enabled;
	CSLock m_Lock;
	UINT m_RefreshTimeout;
	time_t m_RefreshTime;
	vector<PKPvPUserInfo> m_DBPK;
	vector<PKPvPUserInfo> m_DBPvP;
	vector<wstring> m_PagesPvP;
	vector<wstring> m_PagesPK;
	void LoadINI();
public:
	void Init();
	void TimerExpired();
	void OnDBLoadPK(UINT charId, wstring name, UINT classId, INT32 level, INT32 pvp, INT32 pk);
	void OnDBLoadPvP(UINT charId, wstring name, UINT classId, INT32 level, INT32 pvp, INT32 pk);
	void RequestPKStatus(User *pUser, UINT page = 0);
	void RequestPvPStatus(User *pUser, UINT page = 0);
	void GeneratePagesPK();
	void GeneratePagesPvP();
};

extern CPKPvPStatus g_PKPvPStatus;