#include "stdafx.h"
#include "DailyPvP.h"
#include "DB.h"

CDailyPvP g_DailyPvP;

void CDailyPvP::Init()
{
	m_Enabled = FALSE;
	m_RequiredPvP = 20;
	m_MinLevel = 70;
	m_CleanUpTimestamp = time(0);
	LoadINI();
	if(m_Enabled)
	{
		g_Log.Add(CLog::Blue, "[%s] Feature is enabled.", __FUNCTION__);
		g_DB.RequestLoadDailyPvPEntry();
	}
}

/* INI section
[DailyPvP]
Enabled=1
RequiredPvP=20
MinLevel=70
*/

void CDailyPvP::OnDBLoad(UINT charId, LPBYTE lpHWID, UINT timestamp)
{
	guard;
	
	//checking time
	tm currentTime = { 0 };
	tm deathTime = { 0 };
	GetTimeInfo(currentTime);
	GetTimeInfo(deathTime, timestamp);
	if(currentTime.tm_mday == deathTime.tm_mday && currentTime.tm_mon == deathTime.tm_mon)
	{
		//add to db
		DailyPvPInfo dpi;
		dpi.charId = charId;
		memcpy(dpi.hardwareId.hash, lpHWID, 32);
		dpi.timestmap = timestamp;
		m_Lock.Enter();
		m_DB.push_back(dpi);
		m_Lock.Leave();
	}
	unguard;
}

void CDailyPvP::LoadINI()
{
	const TCHAR * section = _T("DailyPvP");
	m_Enabled = GetPrivateProfileInt(section, _T("Enabled"), 0, g_ConfigFile);
	m_RequiredPvP = GetPrivateProfileInt(section, _T("RequiredPvP"), 20, g_ConfigFile);
	m_MinLevel = GetPrivateProfileInt(section, _T("MinLevel"), 70, g_ConfigFile);
}

bool CDailyPvP::GetPoint(User *pKiller, User *pEnemy)
{
	guard;
	bool ret = false;
	if(m_Enabled)
	{
		if(pKiller->ValidUser() && pEnemy->ValidUser())
		{
			//HWID check
			if(!pKiller->pSocket->pED->longHWID.IsEqual(pEnemy->pSocket->pED->longHWID))
			{
				//check for pledge/alliance/party
				if(!pKiller->IsInAlliance(pEnemy) && !pKiller->IsMyParty(pEnemy))
				{
					//check for min level
					if(pKiller->pSD->nLevel >= m_MinLevel && pEnemy->pSD->nLevel >= m_MinLevel)
					{
						//check for pvp amount - if player has less pvp points than required
						if(pKiller->pED->dailyPvPAmount < m_RequiredPvP)
						{
							bool canGet = true;
							bool updated = false;
							HWID& enemyHWID = pEnemy->pSocket->pED->longHWID;
							//checking if player with enemy hwid wasn't killed today
							//if he wasn't then add his hwid and timestamp to db (or update it)
							m_Lock.Enter();
							for(UINT n=0;n<m_DB.size();n++)
							{
								DailyPvPInfo& dpi = m_DB[n];
								//checking charId or hwid - so switching chars wont cause abusing the system
								if(dpi.charId == pEnemy->nDBID || dpi.hardwareId.IsEqual(enemyHWID))
								{
									//checking timestamp
									tm currentTime = { 0 };
									tm deathTime = { 0 };
									GetTimeInfo(currentTime);
									GetTimeInfo(deathTime, dpi.timestmap);
									if(currentTime.tm_mday == deathTime.tm_mday)
									{
										//player with this charId/hwid was already killed today
										canGet = false;
										break;
									}else
									{
										//player with this charId/hwid was killed but it was yesterday
										//updating old timestamp
										dpi.timestmap = time(0);
										updated = true;
										break;
									}
								}
							}
							if(canGet && !updated)
							{
								//add new entry to db - player with this charId/hwid wasn't killed before
								DailyPvPInfo dpi;
								dpi.charId = pEnemy->nDBID;
								dpi.timestmap = time(0);
								dpi.hardwareId = enemyHWID;
								m_DB.push_back(dpi);
							}
							m_Lock.Leave();

							if(canGet)
							{
								pKiller->pED->dailyPvPAmount++;
								pKiller->pED->dailyPvPTimestamp = time(0);
								//Send info to player 
								ShowPoints(pKiller);
								//save info in db
								g_DB.RequestSaveDailyPvP(pKiller->nDBID, pKiller->pED->dailyPvPAmount, pKiller->pED->dailyPvPTimestamp);
								g_DB.RequestSaveDailyPvPEntry(pEnemy->nDBID, enemyHWID.hash, time(0));

								if(pKiller->pED->dailyPvPAmount == m_RequiredPvP)
								{
									//give reward or whatever you'd like to do when they gather all required points here
								}
							}
						}
					}
				}
			}
		}
	}
	unguard;
	return ret;
}

//Timer expried for player
void CDailyPvP::TimerExpired(User *pUser)
{
	guard;
	if(m_Enabled)
	{
		//check when was the last time player get pvp point (checking if it was today if not then reset the amount)
		if(pUser->pED->dailyPvPTimestamp != 0)
		{
			tm currentTime = { 0 };
			tm lastPvPTime = { 0 };
			GetTimeInfo(currentTime);
			GetTimeInfo(lastPvPTime, pUser->pED->dailyPvPTimestamp);
			if(currentTime.tm_mday != lastPvPTime.tm_mday || currentTime.tm_mon != lastPvPTime.tm_mon)
			{
				//resetting the points and time
				pUser->pED->dailyPvPAmount = 0;
				pUser->pED->dailyPvPTimestamp = 0;
				g_DB.RequestSaveDailyPvP(pUser->nDBID, 0, 0);	//charId, points, timestamp
				//send info to user if neccessary
				ShowPoints(pUser);
			}
		}
	}
	unguard;
}

//timer expired for system
void CDailyPvP::TimerExpired()
{
	guard;
	if(m_Enabled)
	{
		//check if cleanup is needed
		tm currentTime = { 0 };
		tm cleanUpTime = { 0 };
		GetTimeInfo(currentTime);
		GetTimeInfo(cleanUpTime, m_CleanUpTimestamp);
		if(currentTime.tm_mday != cleanUpTime.tm_mday || currentTime.tm_mon != cleanUpTime.tm_mon)
		{
			//cleaning up db
			m_CleanUpTimestamp = time(0);
			BYTE hwid[32];
			memset(hwid, 0, 32);
			g_DB.RequestSaveDailyPvPEntry(0, hwid, 0);	//this will signal clean up in cached
			m_Lock.Enter();
			//cleaning local db
			m_DB.clear();
			m_Lock.Leave();
			//players will get updated automatically in first timer expired function
		}
	}
	unguard;
}

void CDailyPvP::ShowPoints(User *pUser)
{
	guard;
	if(m_Enabled)
	{
		tm currentTime = { 0 };
		GetTimeInfo(currentTime);
		pUser->ShowPCCafePoints(m_RequiredPvP - pUser->pED->dailyPvPAmount, 0, 23 - currentTime.tm_hour, false);
	}
	unguard;
}