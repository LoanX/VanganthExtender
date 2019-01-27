#include "stdafx.h"
#include "NPCEventFactory.h"

void CNPCEventFactory::Init()
{
	WriteCall(0x5DD420, CNPCEventFactory::CreateDeletePreviousOlympiadPointReturned, 0);
	WriteJump(0x5DD464, 0x5DD48C, 3);
}

LPVOID CNPCEventFactory::CreateDeletePreviousOlympiadPointReturned(CNPC *pNpc, User *pTalker, int nPoint, int nReturned)
{
	if(nPoint > 0)
	{
		typedef LPVOID (*f)(CNPC*, User*, int, int);
		return f(0x5428E0L)(pNpc, pTalker, nPoint, nReturned);
	}else
	{
		g_Log.Add(LogError, "[%s] Point[%d] - CHEATER[%S]!", __FUNCTION__, nPoint, pTalker->pSD->wszName);
	}
	return 0;
}