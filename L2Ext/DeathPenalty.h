#pragma once


class CDeathPenalty
{
	BOOL m_Enabled;
	INT32 m_PenaltySkillId;
	UINT m_MaxPenaltyLevel;
	INT32 m_MaxNewbieLevel;
	UINT m_PenaltyChance;
	UINT m_PenaltyChancePK;
	void LoadINI();
public:
	CDeathPenalty();
	~CDeathPenalty();
	void Init();
	bool OnDieByMob(User *pUser);
	bool LiftPenalty(User *pUser, double chance);
	void ValidatePenalty(User *pUser);
	inline BOOL IsEnabled() { return m_Enabled; };
};

extern CDeathPenalty g_DeathPenalty;