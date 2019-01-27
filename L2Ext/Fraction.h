#pragma once

class CFraction
{
	//Ini File
	BOOL m_enabled;
	UINT m_fractionId;
	WCHAR m_fractionName[256];
	INT32 m_fractionCoinId;
	UINT m_maxMembers;	//member limit
	INT32 m_minRequiredLevel;	//required level to join
	INT32 m_requiredItemId;
	INT32 m_requiredItemCount;
	INT32 m_leavePenaltyTime;	//penalty in seconds for leaving the fraction
	WCHAR m_titleAddon[25];
	INT32 m_titleColor;
	WCHAR m_nameAddon[25];
	INT32 m_nameColor;
	vector<INT32> m_epicBosses;
	INT32 m_epicBossPoints;
	INT32 m_raidBossPoints;
	INT32 m_normalMobPoints;
	INT32 m_normalMobCoinLimitPerHour;
	double m_normalMobChance;
	INT32 m_heroPoints;
	INT32 m_winSiegePoints;
	BOOL m_rewardOtherFractionKill;
public:
	inline BOOL IsEnabled() { return m_enabled; };
	inline UINT GetId() { return m_fractionId; };
	inline const WCHAR* GetName() { return m_fractionName; };
	inline const WCHAR* GetTitleAddon() { return m_titleAddon; };
	inline INT32 GetTitleColor() { return m_titleColor; };
	inline const WCHAR* GetNameAddon() { return m_nameAddon; };
	inline INT32 GetNameColor() { return m_nameColor; };
	inline BOOL IsRewardOtherFractionKill() { return m_rewardOtherFractionKill; };
	inline INT32 GetLeavePenaltyTime() { return m_leavePenaltyTime; };
private:

	/* Dynamic Data */
	CSLock m_lock;
	UINT m_totalPoints;
	map<UINT, User*> m_members;
	INT32 m_normalMobCoinDropCount;
	UINT m_normalMobCoinDropTimestmap;
	UINT m_dbSaveTimestamp;
	BOOL m_loadedFromDb;

	void LoadINI();
public:
	CFraction();
	~CFraction();
	void Init(UINT id);
	void OnKillNpc(User *pUser, CNPC *pNpc);
	void OnKillPC(User *pKiller, User *pVictim);
	void TimerExpired();
	void OnBecomeHero(User *pUser);
	void OnWinCastleSiege(User *pUser);
	bool Register(User *pUser);
	void Broadcast(UINT len, PCHAR buff);
	void Join(User *pUser);
	void DBLoad(INT32 totalPoints);
};