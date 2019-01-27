#pragma once


/*
4D=RequestStartPledgeWar:s(PledgeName)
4E=RequestReplyStartPledgeWar:s(PledgeName)d(Answer) //not working in l2server (packet is just doing nothing)
4F=RequestStopPledgeWar:s(PledgeName)
50=RequestReplyStopPledgeWar:s(PledgeName)d(Answer) //not working in l2server (packet is just doing nothing)
51=RequestSurrenderPledgeWar:s(PledgeName) //not working in l2server (packet is just doing nothing)
52=RequestReplySurrenderPledgeWar:s(PledgeName)d(Answer) //not working in l2server (packet is just doing nothing)
*/

class CWarDelayInfo
{
public:
	CWarDelayInfo() : StartTime(0), nPledgeID(0) {};
	DWORD StartTime;
	int nPledgeID;
};

class CWarDelay
{
	CWarDelayInfo Info[30];
public:
	void StartDelay(int nPledgeID);
	bool HasDelay(int nPledgeID);
	void DelDelay(int nPledgeID);
};