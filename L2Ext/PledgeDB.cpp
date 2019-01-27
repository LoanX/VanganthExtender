#include "StdAfx.h"

CPledgeDB::CPledgeDB()
{
}

CPledgeDB::~CPledgeDB()
{
}

void CPledgeDB::AddPledge(CPledge *pPledge)
{
	if(pPledge && pPledge->pSD)
	{
		Lock();
		try
		{
			mData.insert(pair<int, CPledge*>(pPledge->pSD->nDBID, pPledge));
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception detected.", __FUNCTION__);
		}
		Unlock();
	}else
	{
		g_Log.Add(CLog::Error, "[%s] Invalid Pledge!", __FUNCTION__);
	}
};

void CPledgeDB::DelPledge(CPledge *pPledge)
{
	if(pPledge && pPledge->pSD)
	{
		Lock();
		try
		{
			PledgeMap::iterator Iter = mData.find(pPledge->pSD->nDBID);
			if(Iter != mData.end())
			{
				mData.erase(Iter);
			}
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception detected.", __FUNCTION__);
		}
		Unlock();
	}else
	{
		g_Log.Add(CLog::Error, "[%s] Invalid Pledge!", __FUNCTION__);
	}
};

CPledge* CPledgeDB::GetPledge(int nPledgeID)
{
	CPledge *pPledge = NULL;
	Lock();
	try
	{
		PledgeMap::iterator Iter = mData.find(nPledgeID);
		if(Iter != mData.end())
		{
			pPledge = Iter->second;
		}
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception detected - pledge[%d].", __FUNCTION__, nPledgeID);
	}
	Unlock();
	return pPledge;
}

CPledge* CPledgeDB::GetPledgeByName(wstring wName)
{
	try
	{
		transform(wName.begin(), wName.end(), wName.begin(), towlower);
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception detected - pledge[%S] converting to lowercase.", __FUNCTION__, wName);
	}

	CPledge *pPledge = NULL;
	Lock();
	try
	{
		PledgeMap::iterator Iter = mData.begin();
		for(;Iter!=mData.end();Iter++)
		{
			CPledge *pTmp = Iter->second;
			if(pTmp)
			{
				WCHAR wClanName[24];
				transform(&pTmp->pSD->wszClanName[0], &pTmp->pSD->wszClanName[24], &wClanName[0], towlower);
				if(!wcscmp(wClanName, wName.c_str()))
				{
					pPledge = pTmp;
					break;
				}
			}
		}
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception detected - pledge[%S].", __FUNCTION__, wName.c_str());
	}
	Unlock();
	return pPledge;
}