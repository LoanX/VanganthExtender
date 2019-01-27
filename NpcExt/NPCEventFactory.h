#pragma once

class CNPCEventFactory
{
public:
	static void Init();
	static LPVOID CreateDeletePreviousOlympiadPointReturned(CNPC *pNpc, User *pTalker, int nPoint, int nReturned);
};