#include "stdafx.h"
#include "RaidBossStatus.h"
#include "NpcDb.h"
#include "DB.h"
#include "HtmlCache.h"
#include "PlayerAction.h"

CRaidBossStatus g_RaidBossStatus;

void CRaidBossStatus::Init()
{
	LoadINI();
	if(m_Enabled)
	{
		g_Log.Add(CLog::Blue, "[%s] Feature is enabled, refresh timeout [%d] second(s), loaded [%d] DB name(s).", __FUNCTION__, m_RefreshTimeout, m_DBNames.size());
		m_RefreshTime = time(0) + m_RefreshTimeout;
		g_DB.RequestLoadRaidBossStatus();
	}
}

/*
[RaidBossStatus]
;Enables new bypass: "bypass -h rb_status?page=0" - will show raidboss status
Enabled=0
;Refresh time - how often it will refresh the status
RefreshTimeout=60

*/

void CRaidBossStatus::LoadINI()
{
	CTL;

	const TCHAR* section = _T("RaidBossStatus");
	m_Enabled = GetPrivateProfileInt(section, _T("Enabled"), 0, g_ConfigFile);
	m_RefreshTimeout = GetPrivateProfileInt(section, _T("RefreshTimeout"), 0, g_ConfigFile);
	TCHAR temp[0x10000] = { 0 };
	if(GetPrivateProfileString(section, _T("RaidbossDBNameList"), 0, temp, 0x10000, g_ConfigFile))
	{
		wstringstream nameStream(temp);
		wstring rbName;
		while(nameStream >> rbName)
		{
			m_DBNames.push_back(rbName);
		}
	}
	//StaticRespawnBosses={amber;86400} {antaras;406400}
	if(GetPrivateProfileString(section, _T("StaticRespawnBosses"), 0, temp, 0x10000, g_ConfigFile))
	{
		wstring params = Parser::Replace(temp, L'{', L' ');
		params = Parser::Replace(params, L'}', L' ');
		params = Parser::Replace(params, L';', L' ');
		wstringstream paramStream(params);
		wstring rbName;
		UINT respawnTime = 0;
		while(paramStream >> rbName >> respawnTime)
		{
		//	g_Log.Add(CLog::Blue, "[%s] name[%S] respawn[%d]", __FUNCTION__, rbName.c_str(), respawnTime);
			m_StaticDB[rbName] = respawnTime;
		}
	}
	if(GetPrivateProfileString(section, _T("RespawnAnnounce"), 0, temp, 0x10000, g_ConfigFile))
	{
		vector<wstring> params = Parser::Split(temp, L";{}");
		//{boss_name;time_in_seconds_before_respawn;Announce message}{boss_name2;time_in_seconds_before_respawn;Announce message2}
		vector<wstring> rparams;
		for(UINT n=0;n<params.size();n++)
		{
			wstring param = Parser::Trim(params[n]);
			if(param.size() > 0)
				rparams.push_back(params[n]);
		}

		RaidBossRespawnAnnounce ra;
		INT32 paramIdx = 1;
		for(UINT n=0;n<rparams.size();n++)
		{
			wstring param = rparams[n];
			switch(paramIdx)
			{
			case 4:
				{
					paramIdx = 1;
				}
			case 1:
				{
					ra.npcName = param;
					break;
				}
			case 2:
				{
					ra.announceTime = _wtoi(param.c_str());
					break;
				}
			case 3:
				{
					ra.message = param;
					m_Announce[ra.npcName] = ra;
					break;
				}

			}
			paramIdx++;
		}
	}

	if(GetPrivateProfileString(section, _T("AliveColor"), 0, temp, 16384, g_ConfigFile))
	{
		m_AliveColor = temp;
	}
	if(GetPrivateProfileString(section, _T("DeadColor"), 0, temp, 16384, g_ConfigFile))
	{
		m_DeadColor = temp;
	}
	if(m_AliveColor.size() == 0)
	{
		m_AliveColor = L"69ED74";
	}
	if(m_DeadColor.size() == 0)
	{
		m_DeadColor = L"854CA6";
	}
}

UINT CRaidBossStatus::GetStaticRespawn(wstring name)
{
	guard;
	UINT respawn = 0;

	map<wstring, UINT>::iterator it = m_StaticDB.find(name);
	if(it!=m_StaticDB.end())
	{
		respawn = it->second;
	}

	unguard;
	return respawn;
}

UINT CRaidBossStatus::GetDeathTime(wstring name)
{
	CTL;
	{
		AUTO_LOCK(m_Lock);
		map<wstring, RaidBossInfo>::iterator it = m_DB.find(name);
		if(it != m_DB.end())
		{
			if(it->second.alive == false)
			{
				return it->second.deathTime;
			}
		}
	}

	return 0;
}

void CRaidBossStatus::OnDBLoad(wstring name, int x, int y, int z, bool alive, UINT deathTime)
{
	guard;

	bool canBeAdded = false;
	for(UINT n=0;n<m_DBNames.size();n++)
	{
		if(name == m_DBNames[n])
		{
			canBeAdded = true;
			break;
		}
	}
	if(canBeAdded)
	{
		RaidBossInfo rbi;
		rbi.alive = alive;
		rbi.deathTime = deathTime;
		rbi.dbName = name;
		rbi.name = g_NpcDb.GetClientNameByName(name);
		if(rbi.name.size() == 0)
		{
			rbi.name = name;
		}
		rbi.pos.x = x;
		rbi.pos.y = y;
		rbi.pos.z = z;

		m_Lock.Enter();
		m_DB[rbi.dbName] = rbi;
		m_Lock.Leave();
	}
	unguard;
}

void CRaidBossStatus::TimerExpired()
{
	guard;
	if(m_Enabled)
	{
		time_t currentTime = time(0);
		if(m_RefreshTime < currentTime)
		{
			UINT pages = 0;
			{
				AUTO_LOCK(m_Lock);
				//Generate pages
				GeneratePages();
				m_DB.clear();
				pages = static_cast<UINT>(m_Pages.size());
			}

			g_DB.RequestLoadRaidBossStatus();
			m_RefreshTime = time(0) + m_RefreshTimeout;
			
			g_Log.Add(CLog::Blue, "[%s] Refreshing status - generated [%d] page(s).", __FUNCTION__, pages);
		}

		//handle announces
		for(map<wstring, RaidBossRespawnAnnounce>::iterator it = m_Announce.begin(); it != m_Announce.end(); it++)
		{
			if(UINT deathTime = GetDeathTime(it->first))
			{
				if(UINT staticRespawn = GetStaticRespawn(it->first))
				{
					UINT spawnTime = deathTime + staticRespawn;
					spawnTime -= it->second.announceTime;
			//		g_Log.Add(CLog::Blue, "[%s] spawn[%d] vs current[%d] - npc[%S]", __FUNCTION__, spawnTime, currentTime, it->first.c_str());
					if(spawnTime == currentTime)
					{
						Utils::BroadcastToAllUser_Announce(it->second.message.c_str());
					}
				}
			}
		}
	}
	unguard;
}

void CRaidBossStatus::GeneratePages()
{
	guard;
	const WCHAR* wHTML = g_HtmlCache.Get(L"rb_status.htm");
	if(wHTML)
	{
		if(m_DB.size() > 0)
		{
			wstring html(wHTML);
			m_Pages.clear();
			UINT page = 0;
			UINT totalPages = static_cast<UINT>(m_DB.size() / 15);
			if((m_DB.size() % 15) != 0)
			{
				totalPages++;
			}
			WCHAR temp[8190] = { 0 };
			size_t startPos = html.find(L"<template_line>");
			wstring templateLine, templateLineEx;
			if(startPos != wstring::npos)
			{
				size_t endPos = html.find(L"</template_line>", startPos);
				if(endPos != wstring::npos)
				{
					templateLineEx = html.substr(startPos, (endPos + 16 - startPos));
					templateLine = html.substr(startPos + 15, (endPos - startPos - 15));

					//<template_line><tr><td width="150"><?boss_name?></td><td width="50"><?boss_status?></td><td width="50"><?boss_death?></td></tr></template_line>	
					wstringstream linesStream;
					bool pageFlag = true;
					UINT counter = 0;
					for(map<wstring, RaidBossInfo>::iterator it = m_DB.begin(); it != m_DB.end(); it++)
					{
						pageFlag = true;
						wstring line = templateLine;
						line = Utils::ReplaceString(line, L"<?boss_name?>", it->second.name, true);
						if(it->second.alive)
						{
							wstring color;
							color = L"<font color=\"";
							color += m_AliveColor;
							color += L"\">alive</font>";
							line = Utils::ReplaceString(line, L"<?boss_status?>", color, true);
							line = Utils::ReplaceString(line, L"<?boss_death?>", L" - ", true);
						}else
						{
							wstringstream deathTimeStream;
							UINT staticRespawn = GetStaticRespawn(it->second.dbName);
							if(staticRespawn > 0)
							{
								UINT currentTime = time(0);
								UINT respawnTimeout = it->second.deathTime + staticRespawn;
								int spawnTime = respawnTimeout - currentTime;
								if(spawnTime > 0)
								{
									int seconds = spawnTime % 60;
									spawnTime /= 60;
									int minutes = spawnTime % 60;
									spawnTime /= 60;
									int hours = spawnTime % 24;
									spawnTime /= 24;
									int days = spawnTime;
									
									if(days > 0)
										deathTimeStream << days << L":";
									if(hours > 9)
										deathTimeStream << hours << L":";
									else
										deathTimeStream << L"0" << hours << L":";
									if(minutes > 9)
										deathTimeStream << minutes << L":";
									else
										deathTimeStream << L"0" << minutes << L":";
									if(seconds > 9)
										deathTimeStream << seconds;
									else
										deathTimeStream << L"0" << seconds;

								}else
								{
									deathTimeStream << L"00:00:00";
								}
							}else
							{
								tm ti;
								GetTimeInfo(ti, it->second.deathTime);
								deathTimeStream << 1900 + ti.tm_year << L"/" << (ti.tm_mon+1) << L"/" << ti.tm_mday << L" " << ti.tm_hour << L":" << ti.tm_min << L":" << ti.tm_sec;
							}
							wstring color;
							color = L"<font color=\"";
							color += m_DeadColor;
							color += L"\">dead</font>";
							line = Utils::ReplaceString(line, L"<?boss_status?>", color, true);
							line = Utils::ReplaceString(line, L"<?boss_death?>", deathTimeStream.str(), true);
						}
						linesStream << line << endl;
						counter++;
						if((counter%15) == 0)
						{
							wstring templ = html;
							_itow(page+1, temp, 10);
							templ = Utils::ReplaceString(templ, L"<?page?>", temp, true);
							_itow(totalPages, temp, 10);
							templ = Utils::ReplaceString(templ, L"<?total_pages?>", temp, true);
							UINT prevPage = 0;
							if(page > 0)
							{
								prevPage = page - 1;
							}
							_itow(prevPage, temp, 10);
							templ = Utils::ReplaceString(templ, L"<?previous_page?>", temp, true);
							UINT nextPage = 0;
							if(totalPages > 1)
							{
								if((page+1) < totalPages)
								{
									nextPage = page + 1;
								}
							}
							_itow(nextPage, temp, 10);
							templ = Utils::ReplaceString(templ, L"<?next_page?>", temp, true);
							templ = Utils::ReplaceString(templ, templateLineEx, linesStream.str(), true);
							m_Pages.push_back(templ);
							linesStream.str(L"");
							pageFlag = false;
							page++;
						}
					}
					if(pageFlag)
					{
						//add last page
						wstring templ = html;
						_itow(page+1, temp, 10);
						templ = Utils::ReplaceString(templ, L"<?page?>", temp, true);
						_itow(totalPages, temp, 10);
						templ = Utils::ReplaceString(templ, L"<?total_pages?>", temp, true);
						UINT prevPage = 0;
						if(page > 0)
						{
							prevPage = page - 1;
						}
						_itow(prevPage, temp, 10);
						templ = Utils::ReplaceString(templ, L"<?previous_page?>", temp, true);
						UINT nextPage = 0;
						if(totalPages > 1)
						{
							if((page+1) < totalPages)
							{
								nextPage = page + 1;
							}
						}
						_itow(nextPage, temp, 10);
						templ = Utils::ReplaceString(templ, L"<?next_page?>", temp, true);
						templ = Utils::ReplaceString(templ, templateLineEx, linesStream.str(), true);
						m_Pages.push_back(templ);
						linesStream.str(L"");
						pageFlag = false;
						page++;
					}
				}
			}
		}
	}else
	{
		g_Log.Add(CLog::Error, "[%s] Cannot find html - rb_status.htm !", __FUNCTION__);
	}
	unguard;
}

void CRaidBossStatus::RequestRaidBossStatus(User *pUser, UINT page)
{
	guard;
	if(m_Enabled)
	{
		wstring html = L"<html><head></head><body><br><br><br><center>Loading data...<br>Please try again later...</center></body></html>";
		m_Lock.Enter();
		if(m_Pages.size() > 0 && page < m_Pages.size())
		{
			html = m_Pages[page];
		}
		m_Lock.Leave();

		PlayerAction::ShowHTML(pUser, L"rb_status.htm", html.c_str(), 0);
	}
	unguard;
}