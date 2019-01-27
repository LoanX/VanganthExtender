#pragma once
#include "Fraction.h"

class CFractionSystem
{
	BOOL m_enabled;
	CSLock m_lock;
	map<UINT, CFraction*> m_fractions;
	void LoadINI();
public:
	CFractionSystem();
	~CFractionSystem();
	void Init();
	inline BOOL IsEnabled() { return m_enabled; };
	void OnDie(User *pUser, CCreature *pKiller);
	void OnDie(CNPC *pNpc, CCreature *pKiller);
	void OnBecomeHero(User *pUser);
	void OnWinCastleSiege(User *pUser);
	void Register(User *pUser, UINT id);
	void Unregister(User *pUser);
	void TimerExpired();
	void DBLoad(UINT fractionId, INT32 points);
};

extern CFractionSystem g_FractionSystem;