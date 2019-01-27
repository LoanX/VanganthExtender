#pragma once
#include "MultiPartyCommandChannel.h"

class CMultiPartyCommandChannelRegistry
{
public:
	static CMultiPartyCommandChannelSP* FindByMasterParty(CMultiPartyCommandChannelSP& channelSP, CParty *pParty);
};