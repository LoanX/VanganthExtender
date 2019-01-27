#pragma once

#include "DuelUser.h"

class CDuel : private CThreadSafe
{
public:
	enum MODE
	{
		SOLO = 0,
		PARTY = 1
	};
	enum WINNER
	{
		NONE = 0,
		TEAM_1 = 1,
		TEAM_2 = 2,
		DRAW = 3
	};
	enum STATE
	{
		COUNTDOWN = 0,
		FIGHT = 1,
		END = 2
	};
	MODE Mode;
	WINNER Winner;
	time_t StartTime;
	STATE State;
	list<CDuelUser> lTeam1;
	list<CDuelUser> lTeam2;
	wstring wsLeader1;
	wstring wsLeader2;
	int m_FightTime;
	int m_CountdownTime;
	CDuel(int fightTime, int countdownTime ): Mode(SOLO), Winner(NONE), StartTime(0), State(COUNTDOWN), m_FightTime(fightTime), m_CountdownTime(countdownTime) {  }
	~CDuel() { }
	void BroadcastToAll(PCHAR buff, int Size);
	void BroadcastToTeam(int Team, PCHAR buff, int Size);
	void Start(User *pUser1, User *pUser2, bool Party, int nDuelID);
	void StartFight();
	void End();
	void BroadcastUpdateStatus();
	void TimerExpired();
	bool Die(User *pUser);
	void Interrupt();
	void BlockUser(User *pUser, int nTeam);
	bool IsAnyUnblocked(int nTeam);
	void SetWinner(WINNER win);
	void Withdraw(User *pUser);
	void GetTeamMembers(vector<User*>& vMembers, int team);
};