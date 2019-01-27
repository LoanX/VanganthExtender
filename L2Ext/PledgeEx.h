#pragma once

#include "WarDelay.h"

enum SubUnit
{
	MainClan = 0,
	Academy = -1,
	RoyalGuardI = 100,
	RoyalGuardII = 200,
	OrderOfKnightI = 1001,
	OrderOfKnightII = 1002,
	OrderOfKnightIII = 2001,
	OrderOfKnightIV = 2002
};

class CSubUnit
{
public:
	int nID; //-1/100/200/1001/1002/2001/2002
	int nLeaderID;
	wstring Name;
	wstring LeaderName;
	CSubUnit(){nID = Academy; nLeaderID = 0;};
	~CSubUnit(){};
};

typedef map<int, PledgeSkillInfo> PledgeSkillMap;
typedef map<int, CSubUnit> SubUnitMap;

class CPledgeEx
{
public:
	CPledgeEx(int PledgeID) : nPledgeID(PledgeID) {}
	int nPledgeID;
	CWarDelay WarDelay;
	PledgeSkillMap m_Skills;
	SubUnitMap mSubUnits;
	CSLock m_Lock;
};