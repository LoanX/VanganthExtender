#pragma once

class CPledgeDB : private CThreadSafe
{
	PledgeMap mData;
public:
	CPledgeDB();
	~CPledgeDB();
	void AddPledge(CPledge *pPledge);
	void DelPledge(CPledge *pPledge);
	CPledge* GetPledge(int nPledgeID);
	CPledge* GetPledgeByName(wstring wName);
};

extern CPledgeDB g_PledgeDB;