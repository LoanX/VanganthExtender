#include "StdAfx.h"
#include "CastleSiegeManager.h"
#include "DB.h"

CCastleSiegeManager g_CastleSiegeManager;

void CCastleSiegeManager::Init()
{
	LoadINI();
	if(m_Enabled)
	{
		g_Log.Add(CLog::Blue, "[%s] Feature is enabled, loaded [%d] castle(s).", __FUNCTION__, m_Dates.size());
	}
}

void CCastleSiegeManager::LoadINI()
{
	const TCHAR* section = _T("CastleSiegeManager");
	m_Enabled = GetPrivateProfileInt(section, _T("Enabled"), 0, g_ConfigFile);
	m_CycleTime = GetPrivateProfileInt(section, _T("CycleTime"), 1209600, g_ConfigFile);
	//2013/07/06 18:00:00
	TCHAR temp[8190] = { 0 };
	if(GetPrivateProfileString(section, _T("Gludio"), 0, temp, 8190, g_ConfigFile))
	{
		wstring date = Utils::ReplaceString(temp, L"/", L" ", true);
		date = Utils::ReplaceString(date, L":", L" ", true);
		wstringstream dateStream(date);
		int year, month, day, hour, minute, second;
		if(dateStream >> year >> month >> day >> hour >> minute >> second)
		{
			time_t rawtime = 0;
			time ( &rawtime );
			tm tmTime;
			localtime_s(&tmTime, &rawtime);
			tmTime.tm_year = year - 1900;
			tmTime.tm_mon = month - 1;
			tmTime.tm_mday = day;
			tmTime.tm_hour = hour;
			tmTime.tm_min = minute;
			tmTime.tm_sec = second;

			UINT siegeTime = static_cast<UINT>(mktime ( &tmTime ));
			m_Dates[CastleGludio] = siegeTime;

		}
	}
	if(GetPrivateProfileString(section, _T("Dion"), 0, temp, 8190, g_ConfigFile))
	{
		wstring date = Utils::ReplaceString(temp, L"/", L" ", true);
		date = Utils::ReplaceString(date, L":", L" ", true);
		wstringstream dateStream(date);
		int year, month, day, hour, minute, second;
		if(dateStream >> year >> month >> day >> hour >> minute >> second)
		{
			time_t rawtime = 0;
			time ( &rawtime );
			tm tmTime;
			localtime_s(&tmTime, &rawtime);
			tmTime.tm_year = year - 1900;
			tmTime.tm_mon = month - 1;
			tmTime.tm_mday = day;
			tmTime.tm_hour = hour;
			tmTime.tm_min = minute;
			tmTime.tm_sec = second;

			UINT siegeTime = static_cast<UINT>(mktime ( &tmTime ));
			m_Dates[CastleDion] = siegeTime;

		}
	}
	if(GetPrivateProfileString(section, _T("Giran"), 0, temp, 8190, g_ConfigFile))
	{
		wstring date = Utils::ReplaceString(temp, L"/", L" ", true);
		date = Utils::ReplaceString(date, L":", L" ", true);
		wstringstream dateStream(date);
		int year, month, day, hour, minute, second;
		if(dateStream >> year >> month >> day >> hour >> minute >> second)
		{
			time_t rawtime = 0;
			time ( &rawtime );
			tm tmTime;
			localtime_s(&tmTime, &rawtime);
			tmTime.tm_year = year - 1900;
			tmTime.tm_mon = month - 1;
			tmTime.tm_mday = day;
			tmTime.tm_hour = hour;
			tmTime.tm_min = minute;
			tmTime.tm_sec = second;

			UINT siegeTime = static_cast<UINT>(mktime ( &tmTime ));
			m_Dates[CastleGiran] = siegeTime;

		}
	}
	if(GetPrivateProfileString(section, _T("Oren"), 0, temp, 8190, g_ConfigFile))
	{
		wstring date = Utils::ReplaceString(temp, L"/", L" ", true);
		date = Utils::ReplaceString(date, L":", L" ", true);
		wstringstream dateStream(date);
		int year, month, day, hour, minute, second;
		if(dateStream >> year >> month >> day >> hour >> minute >> second)
		{
			time_t rawtime = 0;
			time ( &rawtime );
			tm tmTime;
			localtime_s(&tmTime, &rawtime);
			tmTime.tm_year = year - 1900;
			tmTime.tm_mon = month - 1;
			tmTime.tm_mday = day;
			tmTime.tm_hour = hour;
			tmTime.tm_min = minute;
			tmTime.tm_sec = second;

			UINT siegeTime = static_cast<UINT>(mktime ( &tmTime ));
			m_Dates[CastleOren] = siegeTime;

		}
	}
	if(GetPrivateProfileString(section, _T("Aden"), 0, temp, 8190, g_ConfigFile))
	{
		wstring date = Utils::ReplaceString(temp, L"/", L" ", true);
		date = Utils::ReplaceString(date, L":", L" ", true);
		wstringstream dateStream(date);
		int year, month, day, hour, minute, second;
		if(dateStream >> year >> month >> day >> hour >> minute >> second)
		{
			time_t rawtime = 0;
			time ( &rawtime );
			tm tmTime;
			localtime_s(&tmTime, &rawtime);
			tmTime.tm_year = year - 1900;
			tmTime.tm_mon = month - 1;
			tmTime.tm_mday = day;
			tmTime.tm_hour = hour;
			tmTime.tm_min = minute;
			tmTime.tm_sec = second;

			UINT siegeTime = static_cast<UINT>(mktime ( &tmTime ));
			m_Dates[CastleAden] = siegeTime;

		}
	}
	if(GetPrivateProfileString(section, _T("Innadril"), 0, temp, 8190, g_ConfigFile))
	{
		wstring date = Utils::ReplaceString(temp, L"/", L" ", true);
		date = Utils::ReplaceString(date, L":", L" ", true);
		wstringstream dateStream(date);
		int year, month, day, hour, minute, second;
		if(dateStream >> year >> month >> day >> hour >> minute >> second)
		{
			time_t rawtime = 0;
			time ( &rawtime );
			tm tmTime;
			localtime_s(&tmTime, &rawtime);
			tmTime.tm_year = year - 1900;
			tmTime.tm_mon = month - 1;
			tmTime.tm_mday = day;
			tmTime.tm_hour = hour;
			tmTime.tm_min = minute;
			tmTime.tm_sec = second;

			UINT siegeTime = static_cast<UINT>(mktime ( &tmTime ));
			m_Dates[CastleInnadril] = siegeTime;

		}
	}
	if(GetPrivateProfileString(section, _T("Goddard"), 0, temp, 8190, g_ConfigFile))
	{
		wstring date = Utils::ReplaceString(temp, L"/", L" ", true);
		date = Utils::ReplaceString(date, L":", L" ", true);
		wstringstream dateStream(date);
		int year, month, day, hour, minute, second;
		if(dateStream >> year >> month >> day >> hour >> minute >> second)
		{
			time_t rawtime = 0;
			time ( &rawtime );
			tm tmTime;
			localtime_s(&tmTime, &rawtime);
			tmTime.tm_year = year - 1900;
			tmTime.tm_mon = month - 1;
			tmTime.tm_mday = day;
			tmTime.tm_hour = hour;
			tmTime.tm_min = minute;
			tmTime.tm_sec = second;

			UINT siegeTime = static_cast<UINT>(mktime ( &tmTime ));
			m_Dates[CastleGoddard] = siegeTime;

		}
	}
	if(GetPrivateProfileString(section, _T("Rune"), 0, temp, 8190, g_ConfigFile))
	{
		wstring date = Utils::ReplaceString(temp, L"/", L" ", true);
		date = Utils::ReplaceString(date, L":", L" ", true);
		wstringstream dateStream(date);
		int year, month, day, hour, minute, second;
		if(dateStream >> year >> month >> day >> hour >> minute >> second)
		{
			time_t rawtime = 0;
			time ( &rawtime );
			tm tmTime;
			localtime_s(&tmTime, &rawtime);
			tmTime.tm_year = year - 1900;
			tmTime.tm_mon = month - 1;
			tmTime.tm_mday = day;
			tmTime.tm_hour = hour;
			tmTime.tm_min = minute;
			tmTime.tm_sec = second;

			UINT siegeTime = static_cast<UINT>(mktime ( &tmTime ));
			m_Dates[CastleRune] = siegeTime;

		}
	}
	if(GetPrivateProfileString(section, _T("Schuttgard"), 0, temp, 8190, g_ConfigFile))
	{
		wstring date = Utils::ReplaceString(temp, L"/", L" ", true);
		date = Utils::ReplaceString(date, L":", L" ", true);
		wstringstream dateStream(date);
		int year, month, day, hour, minute, second;
		if(dateStream >> year >> month >> day >> hour >> minute >> second)
		{
			time_t rawtime = 0;
			time ( &rawtime );
			tm tmTime;
			localtime_s(&tmTime, &rawtime);
			tmTime.tm_year = year - 1900;
			tmTime.tm_mon = month - 1;
			tmTime.tm_mday = day;
			tmTime.tm_hour = hour;
			tmTime.tm_min = minute;
			tmTime.tm_sec = second;

			UINT siegeTime = static_cast<UINT>(mktime ( &tmTime ));
			m_Dates[CastleSchuttgard] = siegeTime;

		}
	}
}

void CCastleSiegeManager::TimerExpired()
{
	guard;
	if(m_Enabled)
	{
		time_t currentTime = time(0);
		if((currentTime % 1800) == 0)	//check it every 30 minutes
		{
			for(map<UINT, UINT>::iterator it = m_Dates.begin(); it!=m_Dates.end(); it++)
			{
				CCastle *pCastle = (CCastle*)g_ResidenceDB.GetResidence(it->first);
				if(pCastle->IsCastle())
				{
					if(!pCastle->IsSiegeTime())
					{
						if((pCastle->pRD->SiegeDate + 172800) < currentTime || currentTime < pCastle->pRD->SiegeDate)
						{
							UINT nextSiegeTime = it->second;
							while(nextSiegeTime < currentTime)
							{
								nextSiegeTime += m_CycleTime;
							}
							if(pCastle->pRD->SiegeDate != nextSiegeTime)
							{
								//set new castle siege date
								g_DB.RequestSetNextCastleSiege(0, pCastle, nextSiegeTime);
								typedef void (*reverseSiegeTime)(INT32, UINT);
								reverseSiegeTime(0x4A3BB0L)(pCastle->pRD->nResidenceID, nextSiegeTime);
								pCastle->pRD->SiegeDate = nextSiegeTime;
								pCastle->ChangeResidenceState(1);
								pCastle->BroadcastSetSiegeTime();
								g_Log.Add(CLog::Blue, "[%s] Setting castle[%d] siegeTime[%d]", __FUNCTION__, it->first, nextSiegeTime);
							}
						}
					}
				}
			}
		}
	}
	unguard;
}