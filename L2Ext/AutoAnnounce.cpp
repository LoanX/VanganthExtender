#include "stdafx.h"
#include "AutoAnnounce.h"
#include "NpcDb.h"

CAutoAnnounce g_AutoAnnounce;

CAutoAnnounce::CAutoAnnounce()
{
}

CAutoAnnounce::~CAutoAnnounce()
{
}

void CAutoAnnounce::Init()
{
	m_reload = FALSE;
	m_enabled = FALSE;
	m_data.clear();
	m_BossList.clear();
	LoadINI();
	if(m_enabled)
	{
		g_Log.Add(CLog::Blue, "Auto Announce feature is enabled. Loaded [%d] announces.", m_data.size());
	}
}

/*
Enabled=1
AnnounceCount=2
Announce1_Message=Welcome to ServerName Lineage II Interlude server, hope you enjoy.
Announce1_ShowOnEnterWorld=1
Announce1_Interval=0
Announce2_Message=Please remember to vote for us, more info at www.servername.com thx in advance.
Announce2_ShowOnEnterWorld=1
Announce2_Interval=300
*/

void CAutoAnnounce::LoadINI()
{
	const TCHAR* section = _T("AutoAnnounce");
	m_enabled = GetPrivateProfileInt(section, _T("Enabled"), 0, g_ConfigFile);
	UINT count = GetPrivateProfileInt(section, _T("AnnounceCount"), 0, g_ConfigFile);
	TCHAR temp[16384] = { 0 };
	m_BossAnnounce[0] = 0;
	if(GetPrivateProfileString(section, _T("AnnounceOnBossSpawn"), 0, temp, 16384, g_ConfigFile))
	{
		m_BossAnnounce = temp;
	}
	if(GetPrivateProfileString(section, _T("AnnounceBossList"), 0, temp, 16384, g_ConfigFile))
	{
		wstringstream npcStream(temp);
		UINT npcId = 0;
		while(npcStream >> npcId)
		{
			m_BossList.push_back(npcId);
		}
	}
	for(UINT n = 0;n<count;n++)
	{
		AutoAnnounceData aad;
		aad.nextAnnounceTime = 0;
		{
			wstringstream keyStream;
			keyStream << L"Announce" << (n+1) <<L"_Message";
			if(GetPrivateProfileString(section, keyStream.str().c_str(), 0, temp, 8192, g_ConfigFile))
			{
				aad.announce = temp;
			}
		}
		{
			wstringstream keyStream;
			keyStream << L"Announce" << (n+1) <<L"_ShowOnEnterWorld";
			aad.onEnterWorld = GetPrivateProfileInt(section, keyStream.str().c_str(), 0, g_ConfigFile);
		}
		{
			wstringstream keyStream;
			keyStream << L"Announce" << (n+1) <<L"_Interval";
			aad.interval = GetPrivateProfileInt(section, keyStream.str().c_str(), 0, g_ConfigFile);
		}
		if(temp[0] != '\0')
		{
			m_data.push_back(aad);
		}
	}
}

void CAutoAnnounce::TimerExpired()
{
	CTL;
	if(m_enabled || m_reload)
	{
		AUTO_LOCK(m_lock);
		if(m_reload)
		{
			Init();
			if(!m_enabled)
			{
				return;
			}
		}
		time_t currentTime = time(0);
		for(UINT n=0;n<m_data.size();n++)
		{
			AutoAnnounceData& data = m_data[n];
			if(data.interval > 0)
			{
				if(currentTime > data.nextAnnounceTime)
				{
					//announce and set new next announce time
					data.nextAnnounceTime = currentTime + data.interval;
					Utils::BroadcastToAllUser_Announce(data.announce.c_str());
				}
			}
		}
	}
}

void CAutoAnnounce::OnEnterWorld(User *pUser)
{
	CTL;
	if(m_enabled)
	{
		if(pUser->ValidUser())
		{
			AUTO_LOCK(m_lock);
			for(UINT n=0;n<m_data.size();n++)
			{
				AutoAnnounceData& data = m_data[n];
				if(data.onEnterWorld)
				{
					pUser->pSocket->Send("cddSS", 0x4A, 0, 0xA, L"", data.announce.c_str());
				}
			}		
		}
	}
}

void CAutoAnnounce::OnBossEnterWorld(UINT npcClassId)
{
	CTL;
	if(m_enabled)
	{
		bool affected = false;
		for(UINT n=0;n<m_BossList.size();n++)
		{
			if(m_BossList[n] == npcClassId)
			{
				affected = true;
				break;
			}
		}

		if(affected)
		{
			//make an announce
			wstring npcName = g_NpcDb.GetClientNameByClassId(npcClassId);
			wstring announce = m_BossAnnounce;
			size_t pos = announce.find(L"$bossName");
			while(pos != wstring::npos)
			{
				announce = announce.replace(pos, 9, npcName);
				pos = announce.find(L"$bossName");
			}
			//pUser->pSocket->Send("cddSS", 0x4A, 0, 0xA, L"", data.announce.c_str());
			CHAR buff[8190] = { 0 };
			int len = Assemble(buff, sizeof(buff), "cddSS", 0x4A, 0, 0xA, L"", announce.c_str());
			L2Server::BroadcastToAllUser(len, buff);
		}
	}
}