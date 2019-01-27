#pragma once

class AtomicSetHero : public MemoryObject
{
public:
/* 18 */	UINT unkn18;
/* 1C */	UINT playerId;	//sid
/* 20 */	UINT heroType;
};

class CHeroSystem
{
	BOOL m_enabled;
	vector<CSkillKey> m_skills;
	INT32 m_reputationReward;
	INT32 m_rewardId;
	INT32 m_rewardCount;
	void LoadINI();
public:
	CHeroSystem();
	void Init();
	static bool AtomicSetHero_Do(AtomicSetHero* pAtomic);
	void AcquireHeroSkills(User *pUser);
	void TimerExpired(User *pUser);
	~CHeroSystem();
};

extern CHeroSystem g_HeroSystem;