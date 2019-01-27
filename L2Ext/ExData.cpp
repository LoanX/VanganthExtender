#include "StdAfx.h"

CExData::CExData()  : SoulshotActivated(false), AttackDirectToHp(false),  ArmorPenalty(false), bufferBypassTimestamp(0)
{
	InitializeCriticalSection(&lock);
	kickTick = 0;
	teleportTick = 0;
	stopIncExp = FALSE;
	dispelAllZoneDone = FALSE;
};

CExData::~CExData()
{
	DeleteCriticalSection(&lock);
}

CRequestJoinPartyRoom::CRequestJoinPartyRoom() : requestorId(0), requestTime(0), roomId(0) {};
UINT CRequestJoinPartyRoom::GetRequestor()
{
	return requestorId;
}
int CRequestJoinPartyRoom::GetRoomId() { return roomId; };
void CRequestJoinPartyRoom::Clear()
{
	requestorId = 0;
	requestTime = 0;
	roomId = 0;
}
void CRequestJoinPartyRoom::Expired()
{
	if(requestorId)
	{
		if((requestTime+15) < time(NULL))
		{
			Clear();
		}
	}
}
void CRequestJoinPartyRoom::Set(UINT RequestorId, int RoomId)
{
	requestorId = RequestorId;
	requestTime = time(NULL);
	roomId = RoomId;
}