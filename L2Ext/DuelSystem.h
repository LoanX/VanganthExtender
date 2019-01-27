#pragma once
#include "DuelData.h"

typedef map<int, CDuel*> DuelMap;

class CDuelSystem
{
	BOOL m_Enabled;
	BOOL m_DispelAbnormals;
	INT32 m_CountdownTime;
	INT32 m_FightTime;
	CSLock m_Lock;
	DuelMap m_Duels;
	void LoadINI();
public:
	CDuelSystem();
	~CDuelSystem();
	void Initialize();
	bool CanDuel(CUserSocket *pSocket, User *pTarget);
	void StartDuel(User *pUser1, User *pUser2, bool Party);
	bool RequestAskDuel(CUserSocket *pSocket, const unsigned char* packet);
	bool RequestAnswerDuel(CUserSocket *pSocket, const unsigned char* packet);
	void UpdateUserStatus(User *pUser);
	void TimerExpired();
	int GenerateDuelID();
	bool Die(User *pUser);
	void Damaged(CCreature *pCreature, CCreature *pEnemy);
	void UpdateDuelUserInfo(User *pUser);
	void Interrupt(int nDuelID);
	void LeaveWorld(User *pUser);
	bool Withdraw(User *pUser);
	inline BOOL IsEnabled() { return m_Enabled; };
	inline BOOL IsDispelAbnormals() { return m_DispelAbnormals; };
};

extern CDuelSystem g_DuelSystem;