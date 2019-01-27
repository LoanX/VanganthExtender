#include "stdafx.h"
#include "MultiPartyCommandChannelRegistry.h"

CMultiPartyCommandChannelSP* CMultiPartyCommandChannelRegistry::FindByMasterParty(CMultiPartyCommandChannelSP &channelSP, CParty *pParty)
{
	typedef CMultiPartyCommandChannelSP* (*f)(CMultiPartyCommandChannelSP&, CParty*);
	return f(0x68EA50L)(channelSP, pParty);
}