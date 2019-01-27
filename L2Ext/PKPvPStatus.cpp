#include "StdAfx.h"
#include "PKPvPStatus.h"
#include "DB.h"
#include "HtmlCache.h"
#include "PlayerAction.h"

CPKPvPStatus g_PKPvPStatus;

void CPKPvPStatus::Init()
{
	LoadINI();
	if(m_Enabled)
	{
		g_Log.Add(CLog::Blue, "[%s] Feature is enabled, refresh timeout [%d] second(s).", __FUNCTION__, m_RefreshTimeout);
		m_RefreshTime = time(0) + m_RefreshTimeout;
		g_DB.RequestLoadPKStatus();
		g_DB.RequestLoadPvPStatus();
	}
}

/*
[PKPvPStatus]
;Enables new bypass: "bypass -h rb_status?page=0" - will show raidboss status
Enabled=0
;Refresh time - how often it will refresh the status
RefreshTimeout=60

*/

void CPKPvPStatus::LoadINI()
{
	const TCHAR* section = _T("PKPvPStatus");
	m_Enabled = GetPrivateProfileInt(section, _T("Enabled"), 0, g_ConfigFile);
	m_RefreshTimeout = GetPrivateProfileInt(section, _T("RefreshTimeout"), 0, g_ConfigFile);
}

void CPKPvPStatus::RequestPKStatus(User *pUser, UINT page)
{
	guard;
	if(m_Enabled)
	{
		wstring html = L"<html><head></head><body><br><br><br><center>Loading data...<br>Please try again later...</center></body></html>";
		m_Lock.Enter();
		if(m_PagesPK.size() > 0 && page < m_PagesPK.size())
		{
			html = m_PagesPK[page];
		}
		m_Lock.Leave();

		PlayerAction::ShowHTML(pUser, L"pk_status.htm", html.c_str(), 0);
	}
	unguard;
}

void CPKPvPStatus::RequestPvPStatus(User *pUser, UINT page)
{
	guard;
	if(m_Enabled)
	{
		wstring html = L"<html><head></head><body><br><br><br><center>Loading data...<br>Please try again later...</center></body></html>";
		m_Lock.Enter();
		if(m_PagesPvP.size() > 0 && page < m_PagesPvP.size())
		{
			html = m_PagesPvP[page];
		}
		m_Lock.Leave();

		PlayerAction::ShowHTML(pUser, L"pvp_status.htm", html.c_str(), 0);
	}
	unguard;
}

void CPKPvPStatus::OnDBLoadPK(UINT charId, wstring name, UINT classId, INT32 level, INT32 pvp, INT32 pk)
{
	guard;

	PKPvPUserInfo pui;
	pui.name = name;
	pui.classId = classId;
	pui.level = level;
	pui.pvpAmount = pvp;
	pui.pkAmount = pk;

	bool done = false;
	m_Lock.Enter();
	for(vector<PKPvPUserInfo>::iterator it = m_DBPK.begin(); it != m_DBPK.end();it++)
	{
		if(it->pkAmount < pk)
		{
			m_DBPK.insert(it, pui);
			done = true;
			break;
		}
	}
	if(!done)
	{
		m_DBPK.push_back(pui);
	}
	m_Lock.Leave();
	unguard;
}

void CPKPvPStatus::OnDBLoadPvP(UINT charId, wstring name, UINT classId, INT32 level, INT32 pvp, INT32 pk)
{
	guard;

	PKPvPUserInfo pui;
	pui.name = name;
	pui.classId = classId;
	pui.level = level;
	pui.pvpAmount = pvp;
	pui.pkAmount = pk;

	bool done = false;
	m_Lock.Enter();
	for(vector<PKPvPUserInfo>::iterator it = m_DBPvP.begin(); it != m_DBPvP.end();it++)
	{
		if(it->pvpAmount < pvp)
		{
			m_DBPvP.insert(it, pui);
			done = true;
			break;
		}
	}
	if(!done)
	{
		m_DBPvP.push_back(pui);
	}
	m_Lock.Leave();
	unguard;
}

void CPKPvPStatus::TimerExpired()
{
	guard;
	if(m_Enabled)
	{
		time_t currentTime = time(0);
		if(m_RefreshTime < currentTime)
		{
			UINT pagesPK = 0, pagesPvP = 0;
			m_Lock.Enter();
			//Generate pages
			GeneratePagesPK();
			GeneratePagesPvP();
			m_DBPK.clear();
			m_DBPvP.clear();
			pagesPK = static_cast<UINT>(m_PagesPK.size());
			pagesPvP = static_cast<UINT>(m_PagesPvP.size());
			m_Lock.Leave();

			g_DB.RequestLoadPKStatus();
			g_DB.RequestLoadPvPStatus();
			m_RefreshTime = time(0) + m_RefreshTimeout;
			
			g_Log.Add(CLog::Blue, "[%s] Refreshing status - generated PK[%d] PvP[%d] page(s).", __FUNCTION__, pagesPK, pagesPvP);
		}
	}
	unguard;
}

void CPKPvPStatus::GeneratePagesPK()
{
	guard;
	const WCHAR* wHTML = g_HtmlCache.Get(L"pk_status.htm");
	if(wHTML)
	{
		if(m_DBPK.size() > 0)
		{
			wstring html(wHTML);
			m_PagesPK.clear();
			UINT page = 0;
			UINT totalPages = static_cast<UINT>(m_DBPK.size() / 15);
			if((m_DBPK.size() % 15) != 0)
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

					//<template_line><tr><td width="160"><?boss_name?></td><td width="50"><?boss_status?></td></tr></template_line>	
					wstringstream linesStream;
					bool pageFlag = true;
					UINT counter = 0;
					UINT place = 0;
					for(UINT n=0;n<m_DBPK.size();n++)
					{
						PKPvPUserInfo& pui = m_DBPK[n];
						pageFlag = true;
						wstring line = templateLine;
						place++;
						line = Utils::ReplaceString(line, L"<?p_place?>", place, true);
						line = Utils::ReplaceString(line, L"<?p_name?>", pui.name, true);
						line = Utils::ReplaceString(line, L"<?p_level?>", pui.level, true);
						line = Utils::ReplaceString(line, L"<?p_pk?>", pui.pkAmount, true);
						
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
							m_PagesPK.push_back(templ);
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
						m_PagesPK.push_back(templ);
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

void CPKPvPStatus::GeneratePagesPvP()
{
	guard;
	const WCHAR* wHTML = g_HtmlCache.Get(L"pvp_status.htm");
	if(wHTML)
	{
		if(m_DBPvP.size() > 0)
		{
			wstring html(wHTML);
			m_PagesPvP.clear();
			UINT page = 0;
			UINT totalPages = static_cast<UINT>(m_DBPvP.size() / 15);
			if((m_DBPvP.size() % 15) != 0)
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

					//<template_line><tr><td width="160"><?boss_name?></td><td width="50"><?boss_status?></td></tr></template_line>	
					wstringstream linesStream;
					bool pageFlag = true;
					UINT counter = 0;
					UINT place = 0;
					for(UINT n=0;n<m_DBPvP.size();n++)
					{
						PKPvPUserInfo& pui = m_DBPvP[n];
						pageFlag = true;
						wstring line = templateLine;
						place++;
						line = Utils::ReplaceString(line, L"<?p_place?>", place, true);
						line = Utils::ReplaceString(line, L"<?p_name?>", pui.name, true);
						line = Utils::ReplaceString(line, L"<?p_level?>", pui.level, true);
						line = Utils::ReplaceString(line, L"<?p_pvp?>", pui.pvpAmount, true);
						
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
							m_PagesPvP.push_back(templ);
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
						m_PagesPvP.push_back(templ);
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