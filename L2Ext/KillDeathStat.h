#pragma once


class CKillDeathStat
{
public:
	enum PVP_STATE
	{
		PK = 0,
		PVP = 1,
		CLAN_WAR = 2,
		KARMA = 3,
		BOTH_PK = 4
	};
	void Initialize()
	{
		g_Log.Add(CLog::Blue, "[%s] Done.", __FUNCTION__);
	}
	void OnUserDie(User *pUser, CCreature *pEnemy);
};

extern CKillDeathStat g_KillDeathStat;