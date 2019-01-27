#pragma once

/*
	Spirit System
	You can collect spirits by killing players in PvP (PK wont count), for each frag you'll get +1 spirit
	(there's level range limit ex. 10). When releasing spirits, each spirit will heal your hp/cp/mp by 10% or 200.
*/

class CSpiritSystem
{
	enum BONUS_TYPE
	{
		HP,
		MP,
		CP
	};
	
	int MinReleaseSpiritLimit;
	int MaxReleaseSpiritLimit;
	int LevelRangeLimit;
	int MinRequiredLevel;
	int StaticBonus;
	int PercentBonus;
	int SkillID;
public:
	CSpiritSystem();
	~CSpiritSystem();
	inline int GetMinReleaseSpiritLimit() { return MinReleaseSpiritLimit; };
	double GetBonus(User *pHunter, BONUS_TYPE Type);
	void ValidateSkill(User *pUser);
	bool CanRelease(User *pHunter);
	bool CanHunt(User *pHunter, User *pSpirit);
	void UpdateClientStatus(User *pUser);
	void OnHunt(CCreature *pHunter, User *pSpirit);
	void OnRelease(User *pHunter);
	void Initialize();
};
extern CSpiritSystem g_SpiritSystem;