#pragma once
#include "PartyRoom.h"

class CSharedPartyData;

enum ItemRoutingType
{
	IR_FINDERS_KEEPERS = 0,
	IR_RANDOM = 1,
	IR_RANDOM_INCLUDING_SPOIL = 2,
	IR_BY_TURN = 3,
	IR_BY_TURN_INCLUDING_SPOIL = 4
};

class CParty : public CObject
{
public:
	/* 30 */ LPCIOCriticalSection lpLock;
	/* 38 */ CSharedPartyData *pSD;
	/* 40 */ LPVOID _unkn40;
	/* 48 */ UINT _align48;
	/* 4C */ UINT memberId[9];
	/* 70 */ UINT indexByOrder;
	/* 74 */ UINT memberCount;
	/* 78 */ ItemRoutingType itemRoutingType;

	User *GetMasterCandidate();
	int GetMembersAmount();
	void HandOverMaster(User *pNewMaster);
	void Withdraw(User *pUser);
	void Dismiss();
	User *GetMaster();
	void GetAllMembersEx(UserVector& vUsers, bool lock = true);
	void GetAllMembersInRange(UserVector& vUsers, FVector& pos, double range);
	void LeaveParty(User *pUser);
	bool ValidateMembersForDuel();
	void GetMinMaxLevel(INT32& minLevel, INT32& maxLevel);
	void Broadcast(int len, PCHAR buff);
	
};