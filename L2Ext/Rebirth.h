#pragma once

class CRebirthSystem
{
	bool m_Enabled;
	ItemReq m_Price;
	ItemReq m_Reward;
	INT32 m_MinLevel;
	INT32 m_RebirthLevel;
	INT32 m_StatPointBonus;
	INT32 m_RebirthLimit;
	INT32 m_MaxBonusPerStat;
	bool m_EnableCommand;
	void LoadINI();
public:
	void Init();
	inline bool Enabled() { return m_Enabled; }
	inline bool EnableCommand() { return m_EnableCommand; };
	void ShowMenu(User *pUser);
	void OnRebirth(User *pUser);
	void OnDistributeStats(User *pUser);
	void OnIncreaseStats(User *pUser, UINT statType);
};

extern CRebirthSystem g_RebirthSystem;