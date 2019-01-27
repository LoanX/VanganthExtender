#pragma once
#include "ThreadSafe.h"

class CTimerData
{
	int ID;
	time_t StartTime;
	time_t ExpireTime;
public:
	CTimerData() : ID(0), StartTime(0), ExpireTime(0) {};
	CTimerData(int nID, int nTime) { Set(nID, nTime); };
	void Set(int nID, int TimeInSec)
	{
		ID = nID;
		StartTime = time(NULL);
		ExpireTime = (StartTime + TimeInSec);
	}
	int GetID() { return ID; };
	bool IsExpired()
	{
		if( ExpireTime <= time(NULL))
			return true;
		else
			return false;
	}
};

class CCreatureTimerHandler : private CThreadSafe
{
	list<CTimerData> lData;
public:
	enum TIMER_ID
	{
		USER_SPAWN_PROTECTION = 1,
		SUMMON_BETRAY,
		USER_SEND_ITEM_LIST,
		USER_DISMOUNT_WYVERN,
		CRASH,
		USER_SEND_SPIRIT_STATUS,
		USER_SEND_ERA_MOON,
		DISPEL_AUGMENTATION,
		USER_VALIDATE_ACQUIRED_SKILLS,
		VALIDATE_CLIENT_COUNT,
		VALIDATE_ITEM_REUSE,
		VALIDATE_STATS,
		USER_RESET_PVP_FLAG,
		USER_SHADOW_ITEM_START = -1000000
	};
	void AddTimer(int ID, int Time);
	void DelTimer(int ID);
	void TimerExpired(CCreature *pCreature);
	void Handle(int ID, CCreature *pCreature);
	void HandleCrash(CCreature *pCreature);
	void HandleUserShadowItem(CCreature *pCreature, int ID);
	void HandleUserDispelAugmentation(CCreature *pCreature);
	void HandleUserSendEraMoon(CCreature *pCreature);
	void HandleUserSendSpiritStatus(CCreature *pCreature);
	void HandleUserDismountWyvern(CCreature *pCreature);
	void HandleUserSendItemList(CCreature *pCreature);
	void HandleUserSpawnProtection(CCreature *pCreature);
	void HandleValidateStat(CCreature *pCreature);
	void HandleSummonBetray(CCreature *pCreature);
	void HandleUserValidateAcquiredSkills(CCreature *pCreature);
	void HandleValidateItemReuse(CCreature *pCreature);
	void HandleUserResetPvPFlag(CCreature *pCreature);
};