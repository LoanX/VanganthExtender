#pragma once
#include "SkillMod.h"
#include "HerbAbnormalMgr.h"
#include "CreatureTimerHandler.h"

class CTeleportBypassData
{
	wstring wName;
	time_t tRequestTime;
public:
	CTeleportBypassData() : tRequestTime(0) {};
	__forceinline void Clear() { wName.clear(); tRequestTime = 0; };
	__forceinline void Set(wstring Name)
	{
		wName = Name;
		tRequestTime = time(NULL);
	}
	__forceinline time_t GetRequestTime() { return tRequestTime; };
	__forceinline wstring GetName() { return wName; };
};

class CRequestJoinPartyRoom
{
	UINT requestorId;
	time_t requestTime;
	int roomId;
public:
	CRequestJoinPartyRoom();
	UINT GetRequestor();
	int GetRoomId();
	void Clear();
	void Expired();
	void Set(UINT RequestorId, int RoomId);
};

class CExData
{
	CRITICAL_SECTION lock;
public:
	__forceinline void Lock() { EnterCriticalSection(&lock); };
	__forceinline void Unlock() { LeaveCriticalSection(&lock); };
	CExData();
	~CExData();
	CTargetGroundLoc TargetGroundLoc;
	CSkillMod SkillMod;
	CSkillKey CastedSkill;
	CSummonFriend SummonFriend;
	CSummonBetrayData SummonBetrayData;
	CHerbAbnormalMgr HerbAbnormalMgr;
	bool SoulshotActivated;
	bool AttackDirectToHp;
	bool ArmorPenalty;
	DWORD bufferBypassTimestamp;
	CCreatureTimerHandler TimerHandler;
	CTeleportBypassData TeleportBypassData;
	CRequestJoinPartyRoom requestJoinParyRoom;
	INT32 kickTick;
	DWORD teleportTick;
	BOOL stopIncExp;
	TerritoryData territoryData;
	BOOL dispelAllZoneDone;
};