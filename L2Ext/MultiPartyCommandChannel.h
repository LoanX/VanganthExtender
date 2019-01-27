#pragma once
#include "MemoryObject.h"
#include "SPointer.h"

//78h size
class CMultiPartyCommandChannel : public MemoryObject
{
public:
	/* 18 */ UINT ownerPartyId;
	/* 1C */ UINT _unkn1C;
	/* 20 */ RWLock lock;
	/* 58 */ vector<UINT> parties;

	void TeleportMPCCToLocation(UINT requestorId, INT32 x, INT32 y, INT32 z);
	void TeleportMPCCToLocationByIndex(UINT requestorId, UINT index, INT32 x, INT32 y, INT32 z);
};

typedef CSPointer<CMultiPartyCommandChannel> CMultiPartyCommandChannelSP;