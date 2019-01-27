#include "stdafx.h"
#include "EraSystem.h"
#include "NpcServer.h"

CEraSystem g_EraSystem;

CEraSystem::CEraSystem()
{
	m_enabled = FALSE;
	m_currentEra = 0;
	m_count = 0;
	m_duration = 0;
	m_lastChange = 0;
	m_startTime = 0;
	m_useMoon = 0;
	memset(m_moonState, 0, sizeof(m_moonState));
	memset(m_eraNames, 0, sizeof(m_eraNames));
}

void CEraSystem::LoadINI()
{
	TCHAR File[260];
	GetCurrentDirectory(MAX_PATH,File);
	lstrcat(File, _T("\\l2npc.ini"));
	const TCHAR* sectionName = _T("EraSystem");
	m_enabled = GetPrivateProfileInt(sectionName, _T("Enabled"), 0, File);
	m_currentEra = GetPrivateProfileInt(sectionName, _T("CurrentEra"), 0, File);
	m_count = GetPrivateProfileInt(sectionName, _T("Count"), 0, File);
	m_duration = GetPrivateProfileInt(sectionName, _T("Duration"), 0, File);
	m_lastChange = GetPrivateProfileInt(sectionName, _T("LastChange"), 0, File);
	m_useMoon = GetPrivateProfileInt(sectionName, _T("UseMoon"), 0, File);
	TCHAR temp[8190];
	memset(temp, 0, sizeof(temp));
	if(GetPrivateProfileString(sectionName, _T("EraNames"), 0, temp, 8190, File))
	{
		wstringstream sstr;
		sstr << temp;
		UINT count = 0;
		wstring eraName;
		while(sstr >> eraName)
		{
			if(eraName.size() > 0 && eraName.size() < 32)
			{
				wcscpy_s(m_eraNames[count], 25, eraName.c_str());
			}
			count++;
		}
	}
	memset(temp, 0, sizeof(temp));
	if(GetPrivateProfileString(sectionName, _T("MoonStates"), 0, temp, 8190, File))
	{
		wstringstream sstr;
		sstr << temp;
		UINT count = 0;
		UINT moonState = 0;
		while(sstr >> moonState)
		{
			m_moonState[count] = moonState;
			count++;
		}
	}

	memset(temp, 0, sizeof(temp));
	if(GetPrivateProfileString(sectionName, _T("Start"), 0, temp, 8190, File))
	{
		//01/03/2010 20:00
		wstring wTime = temp;
		wTime = Parser::Replace(wTime, L'/', L' ');
		wTime = Parser::Replace(wTime, L':', L' ');
		wstringstream sstr;
		sstr << wTime;
		int day = 0, month = 0, year = 0, hour = 0, minute = 0;
		if(sstr >> day >> month >> year >> hour >> minute)
		{
			time_t rawtime;
			tm timeinfo;
			memset(&timeinfo, 0, sizeof(timeinfo));
			time ( &rawtime );
			localtime_s(&timeinfo, &rawtime);
			timeinfo.tm_year = year - 1900;
			timeinfo.tm_mon = month - 1;
			timeinfo.tm_mday = day;
			timeinfo.tm_hour = hour;
			timeinfo.tm_min = minute;
			m_startTime = static_cast<UINT>(mktime ( &timeinfo ));
		}
	}

}

void CEraSystem::Init()
{
	LoadINI();
	if(m_enabled)
	{
		ValidateEra();
		g_Log.Add(LogBlue, "[%s] Era Count[%d] currentEra[%d] lastChagned[%d] startTime[%d] currentTime[%d]", __FUNCTION__, m_count, m_currentEra, m_lastChange, m_startTime, time(0));

		for(INT32 n=0;n<m_count;n++)
		{
			g_Log.Add(LogBlue, "[%s] Era[%S] Moon[%d] Index[%d]", __FUNCTION__, m_eraNames[n], m_moonState[n], (n+1));
		}
		//preparing init packet
		CHAR Buff[8190]; memset(Buff, 0, sizeof(Buff));
		int nLen = Assemble(Buff, 8192, "dddddd", m_currentEra, m_count, m_duration, m_lastChange, m_startTime, m_useMoon);
		for(INT32 n=0;n<m_count;n++)
			nLen += Assemble(&Buff[nLen], (8190-nLen), "dS", m_moonState[n], m_eraNames[n]);
		
		g_NpcServer.Send("cdb", 0x38, INITIALIZE_ERA, nLen, Buff);
	}
}

int CEraSystem::GetEraIndex(PWCHAR wName)
{
	for(INT32 n=0;n<m_count;n++)
	{
		if(!wcscmp(wName, m_eraNames[n]))
			return n;
	}
	return -1;
}

void CEraSystem::TimerExpired()
{
	if(m_enabled)
	{
		int nEra = m_currentEra;
		switch(ValidateEra())
		{
		case NOTHING:
		//	g_Log.Add(CLog::Blue, "[%s] Debug: LastChange[%d] Duration[%d] Time[%d] Era[%d]", __FUNCTION__, Data.nLastChange, Data.nDuration, time(NULL), Data.nCurrentEra);
			break;
		case ERA_CHANGE:
			g_Log.Add(LogBlue, "[%s] Era has been changed: OldEra[%d] NewEra[%d]", __FUNCTION__, nEra, m_currentEra);
			//send packet requesting despawn npc and set new era id
			CHAR Buff[8192]; memset(Buff, 0, sizeof(Buff));
			if(m_currentEra > 0)
			{
				int nLen = Assemble(Buff, 8192, "dSd", m_currentEra, m_eraNames[m_currentEra-1], m_lastChange);
				g_NpcServer.Send("cdb", 0x38, CHANGE_ERA, nLen, Buff);
			}else
			{
				int nLen = Assemble(Buff, 8192, "dSd", m_currentEra, m_eraNames[0], m_lastChange);
				g_NpcServer.Send("cdb", 0x38, CHANGE_ERA, nLen, Buff);
			}
			break;
		}
	}
}

CEraSystem::STATE CEraSystem::ValidateEra()
{
	if(m_enabled)
	{
		if(m_count > 0)
		{
			if(m_currentEra == 0)
			{
				INT32 TimeDiff = static_cast<INT32>(time(NULL) - m_startTime);
				if(TimeDiff > 0)
				{
					if(TimeDiff > m_duration)
					{
						//calculates cycle
						int nEra = ((TimeDiff / m_duration) % m_count) + 1;
						SetCurrentEra(nEra);
					}else
					{
						SetCurrentEra(1);
					}
					return CEraSystem::ERA_CHANGE;
				}
			}else
			{
				INT32 TimeDiff = static_cast<INT32>(time(NULL) - m_lastChange);
				if(TimeDiff > m_duration)
				{
					if(m_currentEra < m_count)
						SetCurrentEra(m_currentEra + 1);
					else
						SetCurrentEra(1);
					
					return CEraSystem::ERA_CHANGE;
				}
			}
		}
	}
	return CEraSystem::NOTHING;
}

void CEraSystem::SetCurrentEra(int nEra)
{
//	g_Era.SetVariable(nEra);
	if(m_currentEra == 0 || m_lastChange == 0)
	{
		m_lastChange = m_startTime;
		time_t TimeDiff = time(NULL) - m_startTime;
		if(TimeDiff > m_duration)
		{
			int nCycle = static_cast<INT32>(TimeDiff / m_duration);
			m_lastChange += (m_duration * nCycle);
		}
	}else
	{
		time_t TimeDiff = time(NULL) - m_lastChange;
		if(TimeDiff > m_duration)
		{
			int nCycle = static_cast<INT32>(TimeDiff / m_duration);
			m_lastChange += (m_duration * nCycle);
		}else
			g_Log.Add(LogError, "[%s] TimeDiff[%d] lower than Duration[%d]", __FUNCTION__, TimeDiff, m_duration);
	}


	m_currentEra = nEra;
	SaveINI();
}

void CEraSystem::ProcessingDone(int nEraIndex, BOOL Spawned)
{
	if(nEraIndex < m_count && nEraIndex > -1)
	{
		CHAR Buff[8192]; memset(Buff, 0, sizeof(Buff));
		int nLen = 0;
		nLen = Assemble(Buff, 8192, "dSd", nEraIndex, m_eraNames[nEraIndex], Spawned);
		g_NpcServer.Send("cdb", 0x38, PROCESSING_ERA_NPC_DONE, nLen, Buff);
	}else
		g_Log.Add(LogError, "[%s] Invalid Era Index[%d]", __FUNCTION__, nEraIndex);
}

void CEraSystem::SaveINI()
{
	TCHAR File[260];
	GetCurrentDirectory(MAX_PATH,File);
	lstrcat(File, _T("\\l2npc.ini"));
	const TCHAR* sectionName = _T("EraSystem");
	
	wstringstream wCurrentEra;
	wCurrentEra << m_currentEra;
	WritePrivateProfileString(sectionName, _T("CurrentEra"), wCurrentEra.str().c_str(), File);
	wstringstream wLastChange;
	wLastChange << m_lastChange;
	WritePrivateProfileString(sectionName, _T("LastChange"), wLastChange.str().c_str(), File);
}