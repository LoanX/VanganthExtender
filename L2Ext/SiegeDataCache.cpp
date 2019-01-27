#include "StdAfx.h"

bool CSiegeDataCache::IsValid(CCacheData::DataType Type, int nPledgeID)
{
	if( Type  == CCacheData::PledgeReport )
	{
		PledgeReportMap::iterator Iter = mPledgeReport.find(nPledgeID);
		if(Iter != mPledgeReport.end())
		{
			time_t CurrentTime = time(NULL);
			if( CurrentTime < (Iter->second.CreateTime + Timeout) )
				return true;
		}
	}
	else
	{
		CacheDataMap::iterator Iter = mCacheData.find(Type);
		if(Iter != mCacheData.end())
		{
			time_t CurrentTime = time(NULL);
			if( CurrentTime < (Iter->second.CreateTime + Timeout) )
				return true;
		}
	}
	return false;
}
void CSiegeDataCache::AddReport(CCacheData& Data, int nPledgeID)
{
	if( Data.Type == CCacheData::PledgeReport )
	{
		if( nPledgeID != 0 )
		{
			mPledgeReport.insert(pair<int, CCacheData>(nPledgeID, Data));
			g_Log.Add(CLog::Blue, "[%s] Type[%d] Pledge[%d] Time[%d]", __FUNCTION__, Data.Type, nPledgeID, Data.CreateTime);
		}else
			g_Log.Add(CLog::Error, "[%s] NULL PledgeID!", __FUNCTION__);
	}else
	{
		mCacheData.insert(pair<CCacheData::DataType, CCacheData>(Data.Type, Data));
		g_Log.Add(CLog::Blue, "[%s] Type[%d] Time[%d]", __FUNCTION__, Data.Type, Data.CreateTime);
	}
}
wstring CSiegeDataCache::GetReport(CCacheData::DataType Type, int nPledgeID)
{
	wstring wReport;
	if( Type == CCacheData::PledgeReport )
	{
		if( nPledgeID != 0 )
		{
			PledgeReportMap::iterator Iter = mPledgeReport.find(nPledgeID);
			if(Iter != mPledgeReport.end())
			{
				return Iter->second.wData;
			}
		}else
			g_Log.Add(CLog::Error, "[%s] NULL PledgeID!", __FUNCTION__);
	}else
	{
		CacheDataMap::iterator Iter = mCacheData.find(Type);
		if(Iter != mCacheData.end())
		{
			return Iter->second.wData;
		}
	}
	return wReport;
}