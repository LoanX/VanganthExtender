#pragma once
#include "ReportGenerator.h"
#include "UserStatData.h"
#include "SiegeDataCache.h"
typedef map<int, CUserStatData> UserStatMap; //<user_dbid, stat>
typedef map<int, UserStatMap> PledgeStatMap; //<pledge_id, UserStatMap>
typedef vector<PledgeStatMap> StatVector;   // castle_id = index
typedef vector<CSiegeDataCache> CacheVector;  // castle_id = index

class CSiegeStat
{
	CRITICAL_SECTION DataLock;
	CRITICAL_SECTION CacheLock;
	CacheVector vCache;
	StatVector vData;
	CSiegeStat()
	{ 
		InitializeCriticalSection(&DataLock);
		InitializeCriticalSection(&CacheLock);
		InitVectors();
	}
	~CSiegeStat() 
	{ 
		DeleteCriticalSection(&DataLock);
		DeleteCriticalSection(&CacheLock);
	}
	inline void LockData()
	{ 
		EnterCriticalSection(&DataLock); 
	}
	inline void UnlockData()
	{
		LeaveCriticalSection(&DataLock); 
	}
	inline void LockCache()
	{ 
		EnterCriticalSection(&CacheLock); 
	}
	inline void UnlockCache()
	{
		LeaveCriticalSection(&CacheLock); 
	}
	CSiegeStat(const CSiegeStat &);
	CSiegeStat& operator=(const CSiegeStat&);
	void InitVectors();
public:
	enum CastleType
	{
		NotCastle = 0,
		Gludio = 1,
		Dion = 2,
		Giran = 3,
		Oren = 4,
		Aden = 5,
		Innadril = 6,
		Goddard = 7,
		Rune = 8,
		Schuttgard = 9
	};
	static CSiegeStat& GetInstance();
	void Initialize();
	static CastleType GetCastle(int nCastle);
	static CastleType GetCastleType(FVector& Pos);
	void ReportKillDeathCount(FVector& Pos, int KillerDBID, int Pledge1ID, int OwnedDBID, int Pledge2ID);
	void ReportDamage(FVector& Pos, int DealerDBID, int Pledge1ID, int TakerDBID, int Pledge2ID, double Damage);
	CCacheData GenerateReport(CCacheData::DataType Type, CastleType Castle, int PledgeID = 0);
	CCacheData GenerateTopPledgeReport(CastleType Castle);
	CCacheData GenerateTopDamageDealersReport(CastleType Castle);
	CCacheData GenerateTopDamageTakersReport(CastleType Castle);
	CCacheData GenerateTopKillersReport(CastleType Castle);
	CCacheData GenerateTopOwnedReport(CastleType Castle);
	CCacheData GeneratePledgeReport(CastleType Castle, int nPledgeID);
	bool RequestSendReport(CSocket *pSocket, CCacheData::DataType Type, CastleType Castle, int PledgeID = 0);
	void DumpToDB(CastleType Castle);
	void DumpToFile(CastleType Castle);
	void GenerateHTML(CastleType Castle);
	void OnSiegeEnd(int nCastleID);
};