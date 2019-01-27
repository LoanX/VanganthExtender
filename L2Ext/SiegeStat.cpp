#include "StdAfx.h"
#include "DB.h"
#include "TerritoryData.h"

void CSiegeStat::InitVectors()
{
	PledgeStatMap mData;
	vData.push_back(mData); //Index 0 not used
	vData.push_back(mData); //Gludio
	vData.push_back(mData); //Dion
	vData.push_back(mData); //Giran
	vData.push_back(mData); //Oren
	vData.push_back(mData); //Aden
	vData.push_back(mData); //Innadril
	vData.push_back(mData); //Goddard
	vData.push_back(mData); //Rune
	vData.push_back(mData); //Schuttgard
	CSiegeDataCache Cache;
	vCache.push_back(Cache); //Index 0 not used
	vCache.push_back(Cache); //Gludio
	vCache.push_back(Cache); //Dion
	vCache.push_back(Cache); //Giran
	vCache.push_back(Cache); //Oren
	vCache.push_back(Cache); //Aden
	vCache.push_back(Cache); //Innadril
	vCache.push_back(Cache); //Goddard
	vCache.push_back(Cache); //Rune
	vCache.push_back(Cache); //Schuttgard
}

CSiegeStat& CSiegeStat::GetInstance()
{
	static CSiegeStat SiegeStat;
	return SiegeStat;
}
void CSiegeStat::Initialize()
{
	//Do Nothing just initialize object
}
CSiegeStat::CastleType CSiegeStat::GetCastle(int nCastle)
{
	switch(nCastle)
	{
	case 1:
		return Gludio;
		break;
	case 2:
		return Dion;
		break;
	case 3:
		return Giran;
		break;
	case 4:
		return Oren;
		break;
	case 5:
		return Aden;
		break;
	case 6:
		return Innadril;
		break;
	case 7:
		return Goddard;
		break;
	case 8:
		return Rune;
		break;
	case 9:
		return Schuttgard;
		break;
	default:
		return NotCastle;
		break;
	}
	return NotCastle;
}
CSiegeStat::CastleType CSiegeStat::GetCastleType(FVector& Pos)
{
	if(g_TerritoryChecker.IsInTerritory(Pos, GLUDIO_CASTLE))
		return Gludio;
	else if(g_TerritoryChecker.IsInTerritory(Pos, DION_CASTLE))
		return Dion;
	else if(g_TerritoryChecker.IsInTerritory(Pos, GIRAN_CASTLE))
		return Giran;
	else if(g_TerritoryChecker.IsInTerritory(Pos, OREN_CASTLE))
		return Oren;
	else if(g_TerritoryChecker.IsInTerritory(Pos, ADEN_CASTLE))
		return Aden;
	else if(g_TerritoryChecker.IsInTerritory(Pos, INNADRIL_CASTLE))
		return Innadril;
	else if(g_TerritoryChecker.IsInTerritory(Pos, GODDARD_CASTLE))
		return Goddard;
	else if(g_TerritoryChecker.IsInTerritory(Pos, RUNE_CASTLE))
		return Rune;
	else if(g_TerritoryChecker.IsInTerritory(Pos, SCHUTTGARD_CASTLE))
		return Schuttgard;
	return NotCastle;
}
void CSiegeStat::ReportKillDeathCount(FVector& Pos, int KillerDBID, int Pledge1ID, int OwnedDBID, int Pledge2ID)
{
	CastleType Castle = GetCastleType(Pos);
	if(Castle != NotCastle)
	{
		CCastle *pCastle = (CCastle*)g_ResidenceDB.GetResidence(Castle);
		if(pCastle)
		{
			if(pCastle->IsSiegeTime())
			{
				LockData();
				vData[Castle][Pledge1ID][KillerDBID].KillCount++;
				vData[Castle][Pledge2ID][OwnedDBID].DeathCount++;
				UnlockData();
			}else
				g_Log.Add(CLog::Blue, "[%s] Died not in siege time.", __FUNCTION__);
		}else
			g_Log.Add(CLog::Error, "[%s] Invalid Castle[%d]!", __FUNCTION__);
	}
}
void CSiegeStat::ReportDamage(FVector& Pos, int DealerDBID, int Pledge1ID, int TakerDBID, int Pledge2ID, double Damage)
{
	CastleType Castle = GetCastleType(Pos);
	if(Castle != NotCastle)
	{
		CCastle *pCastle = (CCastle*)g_ResidenceDB.GetResidence(Castle);
		if(pCastle)
		{
			if(pCastle->IsSiegeTime())
			{
				LockData();
				vData[Castle][Pledge1ID][DealerDBID].DamageDealt += Damage;
				vData[Castle][Pledge2ID][TakerDBID].DamageTaken += Damage;
				UnlockData();
			}
		}else
			g_Log.Add(CLog::Error, "[%s] Invalid Castle[%d]!", __FUNCTION__);
	}
}
CCacheData CSiegeStat::GenerateReport(CCacheData::DataType Type, CastleType Castle, int PledgeID)
{
	CCacheData Data(time(NULL), Type, L"");
	if(Type == CCacheData::PledgeReport)
	{
		if(PledgeID)
		{
			Data = GeneratePledgeReport(Castle, PledgeID);
			vCache[Castle].AddReport(Data, PledgeID);
		}
	}else
	{
		switch(Type)
		{
		case CCacheData::TopDamageDealersReport:
			{
				Data = GenerateTopDamageDealersReport(Castle);
				vCache[Castle].AddReport(Data);
				break;
			}
		case CCacheData::TopDamageTakersReport:
			{
				Data = GenerateTopDamageTakersReport(Castle);
				vCache[Castle].AddReport(Data);
				break;
			}
		case CCacheData::TopKillersReport:
			{
				Data = GenerateTopKillersReport(Castle);
				vCache[Castle].AddReport(Data);
				break;
			}
		case CCacheData::TopOwnedReport:
			{
				Data = GenerateTopOwnedReport(Castle);
				vCache[Castle].AddReport(Data);
				break;
			}
		case CCacheData::TopPledgeReport:
			{
				Data = GenerateTopPledgeReport(Castle);
				vCache[Castle].AddReport(Data);
				break;
			}
		}
	}
	g_Log.Add(CLog::Blue, "[%s] Type[%d] Castle[%d] PledgeID[%d] Time[%d]", __FUNCTION__, Type, Castle, PledgeID, Data.CreateTime);
	return Data;
}
CCacheData CSiegeStat::GenerateTopPledgeReport(CastleType Castle)
{
	CCacheData Data(time(NULL), CCacheData::TopPledgeReport, L"");
	wstring wReport;
	wstring wTitle = L"Top Pledge - Siege Stats";
	CReportGenerator Report(wTitle);
	Report.StartLine();
	Report.AddVariable(L"Top");
	Report.AddVariable(L"Clan");
	Report.AddVariable(L"Kills");
	Report.AddVariable(L"Deaths");
	Report.EndLine();
	Report.SaveLine();
	struct PledgeStatInfo
	{
		int PledgeID;
		int KillCount;
		int DeathCount;
	};
	multimap<int, PledgeStatInfo> mPledges; //<killCount, Stat>
	LockData();
	PledgeStatMap::iterator PledgeIter = vData[Castle].begin();
	for(;PledgeIter!=vData[Castle].end();PledgeIter++)
	{
		PledgeStatInfo pi; pi.PledgeID = PledgeIter->first; pi.KillCount = 0; pi.DeathCount = 0;
		UserStatMap::iterator UserIter = PledgeIter->second.begin();
		for(;UserIter!=PledgeIter->second.end();UserIter++)
		{
			pi.KillCount += UserIter->second.KillCount;
			pi.DeathCount += UserIter->second.DeathCount;
		}
		mPledges.insert(pair<int, PledgeStatInfo>(pi.KillCount, pi));
	}
	UnlockData();
	int nTop = 1;
	for(multimap<int, PledgeStatInfo>::reverse_iterator RIter = mPledges.rbegin(); RIter!= mPledges.rend(); RIter++)
	{
		if(nTop == 11)
			break;
		CPledge *pPledge = g_PledgeDB.GetPledge(RIter->second.PledgeID);
		if(pPledge && pPledge->pSD)
		{
			Report.StartLine();
			Report.AddVariable(nTop);
			Report.AddVariable(pPledge->pSD->wszClanName);
			Report.AddVariable(RIter->second.KillCount);
			Report.AddVariable(RIter->second.DeathCount);
			Report.EndLine();
			Report.SaveLine();
		}
	}
	wReport = Report.Generate();
	Data.wData = wReport;
	return Data;
}
CCacheData CSiegeStat::GenerateTopDamageDealersReport(CastleType Castle)
{
	CCacheData Data(time(NULL), CCacheData::TopDamageDealersReport, L"");
	wstring wReport;
	wstring wTitle = L"Top Damage Dealers - Siege Stats";
	CReportGenerator Report(wTitle);
	Report.StartLine();
	Report.AddVariable(L"Top");
	Report.AddVariable(L"Nick");
	Report.AddVariable(L"Clan");
	Report.AddVariable(L"Kills");
	Report.AddVariable(L"Deaths");
	Report.AddVariable(L"Dmg");
	Report.EndLine();
	Report.SaveLine();
	multimap<double, CUserStatData> mUsers; //<damage, Stat>
	LockData();
	PledgeStatMap::iterator PledgeIter = vData[Castle].begin();
	for(;PledgeIter!=vData[Castle].end();PledgeIter++)
	{
		UserStatMap::iterator UserIter = PledgeIter->second.begin();
		for(;UserIter!=PledgeIter->second.end();UserIter++)
		{
			CUserStatData data = UserIter->second;
			data.CharID = UserIter->first;
			data.PledgeID = PledgeIter->first;
			mUsers.insert(pair<double, CUserStatData>(data.DamageDealt, data));
		}
	}
	UnlockData();
	int nTop = 1;
	for(multimap<double, CUserStatData>::reverse_iterator RIter = mUsers.rbegin(); RIter!= mUsers.rend(); RIter++)
	{
		if(nTop == 11)
			break;
		CPledge *pPledge = g_PledgeDB.GetPledge(RIter->second.PledgeID);
		if(pPledge && pPledge->pSD)
		{
			MemberData *pMember = pPledge->GetMemberDataByDBID(RIter->second.CharID);
			try
			{
				if(pMember)
				{
					Report.StartLine();
					Report.AddVariable(nTop);
					Report.AddVariable(pMember->wszName);
					Report.AddVariable(pPledge->pSD->wszClanName);
					Report.AddVariable(RIter->second.KillCount);
					Report.AddVariable(RIter->second.DeathCount);
					Report.AddVariable(RIter->second.DamageDealt);
					Report.EndLine();
					Report.SaveLine();
					nTop++;
				}
			}catch(...)
			{
				g_Log.Add(CLog::Error, "[%s][%s][%d] Exception Detected!", __FILE__, __FUNCTION__, __LINE__);
			}
		}
	}
	wReport = Report.Generate();
	Data.wData = wReport;
	return Data;
}
CCacheData CSiegeStat::GenerateTopDamageTakersReport(CastleType Castle)
{
	CCacheData Data(time(NULL), CCacheData::TopDamageTakersReport, L"");
	wstring wReport;
	wstring wTitle = L"Top Damage Takers - Siege Stats";
	CReportGenerator Report(wTitle);
	Report.StartLine();
	Report.AddVariable(L"Top");
	Report.AddVariable(L"Nick");
	Report.AddVariable(L"Clan");
	Report.AddVariable(L"Kills");
	Report.AddVariable(L"Deaths");
	Report.AddVariable(L"Dmg");
	Report.EndLine();
	Report.SaveLine();
	multimap<double, CUserStatData> mUsers; //<damage, Stat>
	LockData();
	PledgeStatMap::iterator PledgeIter = vData[Castle].begin();
	for(;PledgeIter!=vData[Castle].end();PledgeIter++)
	{
		UserStatMap::iterator UserIter = PledgeIter->second.begin();
		for(;UserIter!=PledgeIter->second.end();UserIter++)
		{
			CUserStatData data = UserIter->second;
			data.CharID = UserIter->first;
			data.PledgeID = PledgeIter->first;
			mUsers.insert(pair<double, CUserStatData>(data.DamageTaken, data));
		}
	}
	UnlockData();
	int nTop = 1;
	for(multimap<double, CUserStatData>::reverse_iterator RIter = mUsers.rbegin(); RIter!= mUsers.rend(); RIter++)
	{
		if(nTop == 11)
			break;
		CPledge *pPledge = g_PledgeDB.GetPledge(RIter->second.PledgeID);
		if(pPledge && pPledge->pSD)
		{
			MemberData *pMember = pPledge->GetMemberDataByDBID(RIter->second.CharID);
			try
			{
				if(pMember)
				{
					Report.StartLine();
					Report.AddVariable(nTop);
					Report.AddVariable(pMember->wszName);
					Report.AddVariable(pPledge->pSD->wszClanName);
					Report.AddVariable(RIter->second.KillCount);
					Report.AddVariable(RIter->second.DeathCount);
					Report.AddVariable(RIter->second.DamageTaken);
					Report.EndLine();
					Report.SaveLine();
					nTop++;
				}
			}catch(...)
			{
				g_Log.Add(CLog::Error, "[%s][%s][%d] Exception Detected!", __FILE__, __FUNCTION__, __LINE__);
			}
		}
	}
	wReport = Report.Generate();
	Data.wData = wReport;
	return Data;
}
CCacheData CSiegeStat::GenerateTopKillersReport(CastleType Castle)
{
	CCacheData Data(time(NULL), CCacheData::TopKillersReport, L"");
	wstring wReport;
	wstring wTitle = L"Top Killers - Siege Stats";
	CReportGenerator Report(wTitle);
	Report.StartLine();
	Report.AddVariable(L"Top");
	Report.AddVariable(L"Nick");
	Report.AddVariable(L"Clan");
	Report.AddVariable(L"Kills");
	Report.AddVariable(L"Deaths");
	Report.EndLine();
	Report.SaveLine();
	multimap<int, CUserStatData> mUsers; //<killcount, Stat>
	LockData();
	PledgeStatMap::iterator PledgeIter = vData[Castle].begin();
	for(;PledgeIter!=vData[Castle].end();PledgeIter++)
	{
		UserStatMap::iterator UserIter = PledgeIter->second.begin();
		for(;UserIter!=PledgeIter->second.end();UserIter++)
		{
			CUserStatData data = UserIter->second;
			data.CharID = UserIter->first;
			data.PledgeID = PledgeIter->first;
			mUsers.insert(pair<int, CUserStatData>(data.KillCount, data));
		}
	}
	UnlockData();
	int nTop = 1;
	for(multimap<int, CUserStatData>::reverse_iterator RIter = mUsers.rbegin(); RIter!= mUsers.rend(); RIter++)
	{
		if(nTop == 11)
			break;
		CPledge *pPledge = g_PledgeDB.GetPledge(RIter->second.PledgeID);
		if(pPledge && pPledge->pSD)
		{
			MemberData *pMember = pPledge->GetMemberDataByDBID(RIter->second.CharID);
			try
			{
				if(pMember)
				{
					Report.StartLine();
					Report.AddVariable(nTop);
					Report.AddVariable(pMember->wszName);
					Report.AddVariable(pPledge->pSD->wszClanName);
					Report.AddVariable(RIter->second.KillCount);
					Report.AddVariable(RIter->second.DeathCount);
					Report.EndLine();
					Report.SaveLine();
					nTop++;
				}
			}catch(...)
			{
				g_Log.Add(CLog::Error, "[%s][%s][%d] Exception Detected!", __FILE__, __FUNCTION__, __LINE__);
			}
		}
	}
	wReport = Report.Generate();
	Data.wData = wReport;
	return Data;
}
CCacheData CSiegeStat::GenerateTopOwnedReport(CastleType Castle)
{
	CCacheData Data(time(NULL), CCacheData::TopOwnedReport, L"");
	wstring wReport;
	wstring wTitle = L"Top Owned - Siege Stats";
	CReportGenerator Report(wTitle);
	Report.StartLine();
	Report.AddVariable(L"Top");
	Report.AddVariable(L"Nick");
	Report.AddVariable(L"Clan");
	Report.AddVariable(L"Kills");
	Report.AddVariable(L"Deaths");
	Report.EndLine();
	Report.SaveLine();
	multimap<int, CUserStatData> mUsers; //<deathcount, Stat>
	LockData();
	PledgeStatMap::iterator PledgeIter = vData[Castle].begin();
	for(;PledgeIter!=vData[Castle].end();PledgeIter++)
	{
		UserStatMap::iterator UserIter = PledgeIter->second.begin();
		for(;UserIter!=PledgeIter->second.end();UserIter++)
		{
			CUserStatData data = UserIter->second;
			data.CharID = UserIter->first;
			data.PledgeID = PledgeIter->first;
			mUsers.insert(pair<int, CUserStatData>(data.DeathCount, data));
		}
	}
	UnlockData();
	int nTop = 1;
	for(multimap<int, CUserStatData>::reverse_iterator RIter = mUsers.rbegin(); RIter!= mUsers.rend(); RIter++)
	{
		if(nTop == 11)
			break;
		CPledge *pPledge = g_PledgeDB.GetPledge(RIter->second.PledgeID);
		if(pPledge && pPledge->pSD)
		{
			MemberData *pMember = pPledge->GetMemberDataByDBID(RIter->second.CharID);
			try
			{
				if(pMember)
				{
					Report.StartLine();
					Report.AddVariable(nTop);
					Report.AddVariable(pMember->wszName);
					Report.AddVariable(pPledge->pSD->wszClanName);
					Report.AddVariable(RIter->second.KillCount);
					Report.AddVariable(RIter->second.DeathCount);
					Report.EndLine();
					Report.SaveLine();
					nTop++;
				}
			}catch(...)
			{
				g_Log.Add(CLog::Error, "[%s][%s][%d] Exception Detected!", __FILE__, __FUNCTION__, __LINE__);
			}
		}
	}
	wReport = Report.Generate();
	Data.wData = wReport;
	return Data;
}
CCacheData CSiegeStat::GeneratePledgeReport(CastleType Castle, int nPledgeID)
{
	CCacheData Data(time(NULL), CCacheData::PledgeReport, L"");
	if(nPledgeID)
	{
		CPledge *pPledge = g_PledgeDB.GetPledge(nPledgeID);
		if(pPledge && pPledge->pSD)
		{
			wstring wReport;
			wstring wTitle = pPledge->pSD->wszClanName;
			wTitle += L" Siege Stats";
			CReportGenerator Report(wTitle);
			Report.StartLine();
			Report.AddVariable(L"Top");
			Report.AddVariable(L"Nick");
			Report.AddVariable(L"Kills");
			Report.AddVariable(L"Deaths");
			Report.EndLine();
			Report.SaveLine();
			
			multimap<int, CUserStatData> mUsers; //<killCount, Stat>
			//Getting data
			LockData();
			UserStatMap::iterator UserIter = vData[Castle][nPledgeID].begin();
			for(;UserIter!=vData[Castle][nPledgeID].end();UserIter++)
			{
				CUserStatData stat = UserIter->second;
				stat.CharID = UserIter->first;
				mUsers.insert(pair<int, CUserStatData>(stat.KillCount, stat));
			}
			UnlockData();
			//Filling report with data
			int nTop = 1;
			for(multimap<int, CUserStatData>::reverse_iterator RIter = mUsers.rbegin(); RIter!= mUsers.rend(); RIter++)
			{
				MemberData *pMember = pPledge->GetMemberDataByDBID(RIter->second.CharID);
				try
				{
					if(pMember)
					{
						Report.StartLine();
						Report.AddVariable(nTop);
						Report.AddVariable(pMember->wszName);
						Report.AddVariable(RIter->second.KillCount);
						Report.AddVariable(RIter->second.DeathCount);
						Report.EndLine();
						Report.SaveLine();
						nTop++;
					}
				}catch(...)
				{
					g_Log.Add(CLog::Error, "[%s][%s][%d] Exception Detected!", __FILE__, __FUNCTION__, __LINE__);
				}
				if(nTop > 10)
					break;
			}
			
			Data.wData = Report.Generate();
		}
	}
	return Data;
}
bool CSiegeStat::RequestSendReport(CSocket *pSocket, CCacheData::DataType Type, CastleType Castle, int PledgeID)
{
	if(CUserSocket *pUS = pSocket->CastUserSocket())
	{
		
		User *pUser = pUS->GetUser();
		if(pUser->ValidUser())
		{
			wstring wReport;
			LockCache();
			if(vCache[Castle].IsValid(Type, PledgeID)) //Check if exists and didnt timeout
			{
				wReport = vCache[Castle].GetReport(Type, PledgeID);
			}else
			{
				CCacheData Data;
				LockData();
				Data = GenerateReport(Type, Castle, PledgeID);
				UnlockData();
				wReport = Data.wData;
			}
			UnlockCache();
			pUser->ShowHTML(L"pledge_report.htm", wReport.c_str(), 0);
		}
	}
	return false;
}
void CSiegeStat::DumpToDB(CastleType Castle)
{
	if(Castle != NotCastle)
	{
		g_Log.Add(CLog::Blue, "[%s] Start dumping data - castle[%d]", __FUNCTION__, Castle);
		LockData();
		PledgeStatMap& mPledgeData = vData[Castle];
		PledgeStatMap::iterator PledgeIter = mPledgeData.begin();
		for(;PledgeIter!=mPledgeData.end();PledgeIter++)
		{
			CHAR buff[8192];
			memset(buff, 0, 8192);
			int Size = 0;
			int nCastleID = Castle;
			int nPledgeID = PledgeIter->first;
	//		g_Log.Add(CLog::Blue, "[%s] PledgeID[%d]", __FUNCTION__, nPledgeID);
			Size += Assemble(buff, (8192 - Size), "dd", nCastleID, nPledgeID);
			UserStatMap& mUserData = PledgeIter->second;
			UserStatMap::iterator UserIter = mUserData.begin();
			for(;UserIter!=mUserData.end();UserIter++)
			{
				Size += Assemble(&buff[Size], (8192 - Size), "ddddd", UserIter->first, UserIter->second.KillCount, UserIter->second.DeathCount, (int)UserIter->second.DamageDealt, (int)UserIter->second.DamageTaken);
			}
			g_DB.RequestSavePledgeStat(buff, Size, (int)mUserData.size());
		}
		UnlockData();
	}
}

void CSiegeStat::DumpToFile(CastleType Castle)
{
	if(Castle != NotCastle)
	{
		g_Log.Add(CLog::Blue, "[%s] Start dumping data to file- castle[%d]", __FUNCTION__, Castle);
		
		tstring sDirectory = _T("SiegeStat");
		if(FILE_ATTRIBUTE_DIRECTORY == GetFileAttributes(sDirectory.c_str())) { /*directory exists*/ }
		else { CreateDirectory(sDirectory.c_str(), NULL); }
		sDirectory += _T("/");
		TCHAR sSubDir[64]; _itot_s(Castle, sSubDir, 8, 10);
		sDirectory += sSubDir;
		if(FILE_ATTRIBUTE_DIRECTORY == GetFileAttributes(sDirectory.c_str())) {	/*directory exists*/ }
		else { CreateDirectory(sDirectory.c_str(), NULL); }
		sDirectory += _T("/");
		tm timeinfo;
		GetTimeInfo(timeinfo);
		_stprintf_s(sSubDir, sizeof(sSubDir), _T("%d-%d-%d-%d-%d"), (timeinfo.tm_year+1900), (timeinfo.tm_mon + 1), timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min);
		sDirectory += sSubDir;
		if(FILE_ATTRIBUTE_DIRECTORY == GetFileAttributes(sDirectory.c_str())) {	/*directory exists*/ }
		else { CreateDirectory(sDirectory.c_str(), NULL); }
		sDirectory += _T("/");
		tstring sName = sDirectory;
		sName += _T("DataDump.txt");

		ofstream file(sName.c_str());
		
		if(file)
		{
			//file format:
			//castle_id	pledge_id	user_id	kills	deaths	damage_dealt	damage_taken
			LockData();
			PledgeStatMap& mPledgeData = vData[Castle];
			PledgeStatMap::iterator PledgeIter = mPledgeData.begin();
			for(;PledgeIter!=mPledgeData.end();PledgeIter++)
			{
				UserStatMap& mUserData = PledgeIter->second;
				UserStatMap::iterator UserIter = mUserData.begin();
				for(;UserIter!=mUserData.end();UserIter++)
				{
					file << Castle << "\t" << PledgeIter->first << "\t" << UserIter->first << "\t" << UserIter->second.KillCount << "\t" << UserIter->second.DeathCount << "\t" << (int)UserIter->second.DamageDealt << "\t" << (int)UserIter->second.DamageTaken << endl;
				}
			}
			UnlockData();
			file.close();
		}
	}
}

void CSiegeStat::GenerateHTML(CastleType Castle)
{
	tstring sDirectory = _T("SiegeStat");
	if(FILE_ATTRIBUTE_DIRECTORY == GetFileAttributes(sDirectory.c_str())) { /*directory exists*/ }
	else { CreateDirectory(sDirectory.c_str(), NULL); }
	sDirectory += _T("/");
	TCHAR sSubDir[64]; _itot_s(Castle, sSubDir, 8, 10);
	sDirectory += sSubDir;
	if(FILE_ATTRIBUTE_DIRECTORY == GetFileAttributes(sDirectory.c_str())) {	/*directory exists*/ }
	else { CreateDirectory(sDirectory.c_str(), NULL); }
	sDirectory += _T("/");
	tm timeinfo;
	GetTimeInfo(timeinfo);
	_stprintf_s(sSubDir, 64, _T("%d-%d-%d-%d-%d"), (timeinfo.tm_year+1900), (timeinfo.tm_mon + 1), timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min);
	sDirectory += sSubDir;
	if(FILE_ATTRIBUTE_DIRECTORY == GetFileAttributes(sDirectory.c_str())) {	/*directory exists*/ }
	else { CreateDirectory(sDirectory.c_str(), NULL); }
	sDirectory += _T("/");
	wstring wReport = vCache[Castle].GetReport(CCacheData::TopDamageDealersReport, 0);
	tstring sName = sDirectory;
	sName += _T("TopDamageDealers.htm");
	wofstream file(sName.c_str());
	file.write(wReport.c_str(), wReport.size());
	file.close();
	sName = sDirectory;
	sName += _T("TopDamageTakers.htm");
	wReport = vCache[Castle].GetReport(CCacheData::TopDamageTakersReport, 0);
	file.open(sName.c_str());
	file.write(wReport.c_str(), wReport.size());
	file.close();
	sName = sDirectory;
	sName += _T("TopKillers.htm");
	wReport = vCache[Castle].GetReport(CCacheData::TopKillersReport, 0);
	file.open(sName.c_str());
	file.write(wReport.c_str(), wReport.size());
	file.close();
	sName = sDirectory;
	sName += _T("TopOwned.htm");
	wReport = vCache[Castle].GetReport(CCacheData::TopOwnedReport, 0);
	file.open(sName.c_str());
	file.write(wReport.c_str(), wReport.size());
	file.close();
	sName = sDirectory;
	sName += _T("TopPledge.htm");
	wReport = vCache[Castle].GetReport(CCacheData::TopPledgeReport, 0);
	file.open(sName.c_str());
	file.write(wReport.c_str(), wReport.size());
	file.close();
}
void CSiegeStat::OnSiegeEnd(int nCastleID)
{
	CastleType Castle = CSiegeStat::GetCastle(nCastleID);

	GenerateReport(CCacheData::TopPledgeReport, Castle, 0);
	GenerateReport(CCacheData::TopOwnedReport, Castle, 0);
	GenerateReport(CCacheData::TopKillersReport, Castle, 0);
	GenerateReport(CCacheData::TopDamageTakersReport, Castle, 0);
	GenerateReport(CCacheData::TopDamageDealersReport, Castle, 0);

	if(g_Config.SiegeReportInfo.IsDumpToDB())
	{
		DumpToDB(Castle);
	}
	if(g_Config.SiegeReportInfo.IsGenerateHTML())
	{
		GenerateHTML(Castle);
	}
	if(g_Config.SiegeReportInfo.IsDumpToFile())
	{
		DumpToFile(Castle);
	}
}