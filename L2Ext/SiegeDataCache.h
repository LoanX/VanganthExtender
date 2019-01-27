#pragma once


class CCacheData
{
public:
	enum DataType
	{
		None = 0, //Not set
		TopDamageDealersReport = 1, //10 top damage dealres
		TopDamageTakersReport = 2, //10 top damage takers
		TopKillersReport = 3, //10 top killers
		TopOwnedReport = 4,  //10 top owned characters
		TopPledgeReport = 5, //10 top Pledge with highest kill count
		PledgeReport = 6 //Stats for members in pledge (top kill count with death count, damage taken/dealt)
	};
	static DataType GetType(int nType)
	{
		switch(nType)
		{
		case 1:
			return TopDamageDealersReport;
			break;
		case 2:
			return TopDamageTakersReport;
			break;
		case 3:
			return TopKillersReport;
			break;
		case 4:
			return TopOwnedReport;
			break;
		case 5:
			return TopPledgeReport;
			break;
		case 6:
			return PledgeReport;
			break;
		default:
			return None;
			break;
		}
		return None;
	}
public:
	CCacheData() : CreateTime(0), Type(None) {}
	CCacheData(time_t time, DataType DType, wstring Data) : CreateTime(time), Type(DType), wData(Data) {}
	time_t CreateTime;
	wstring wData;
	DataType Type;
};
typedef map<CCacheData::DataType, CCacheData> CacheDataMap;
typedef map<int, CCacheData> PledgeReportMap;
//Lock can be removed couse it's already locked in vector

class CSiegeDataCache
{
private:
	static const int Timeout = 300; // 300 seconds
	CacheDataMap mCacheData;
	PledgeReportMap mPledgeReport;
public:
	bool IsValid(CCacheData::DataType Type, int nPledgeID = 0);
	void AddReport(CCacheData& Data, int nPledgeID = 0);
	wstring GetReport(CCacheData::DataType Type, int nPledgeID = 0);
};