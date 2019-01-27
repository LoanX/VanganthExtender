#include "stdafx.h"
#include "ChampionNpc.h"
#include "License.h"

using namespace License;

CChampionNpc g_ChampionNpc;


#pragma optimize("", off)


void CChampionNpc::Init()
{
	m_Enabled = false;
	VIRTUALIZER_TIGER_BLACK_START;
	if(CLicense::GetInstance().CanUse(LicenseChampionNpc))
	{
		LoadINI();
		if(m_Enabled)
		{
			g_Log.Add(CLog::Blue, "[%s] Feature is enabled, loaded [%d] npc(s).", __FUNCTION__, m_Npc.size());
		}
	}
	VIRTUALIZER_TIGER_BLACK_END;
}

void CChampionNpc::LoadINI()
{
	VIRTUALIZER_TIGER_BLACK_START;
	const TCHAR* section = _T("ChampionNpc");
	m_Enabled = GetPrivateProfileInt(section, _T("Enabled"), 0, g_ConfigFile) ? true : false;
	//ChampionNpcList={1018342;1020003;25.5}
	TCHAR temp[0x4000];
	if(GetPrivateProfileString(section, _T("ChampionNpcList"), 0, temp, 0x4000, g_ConfigFile))
	{
		wstring params = Parser::Replace(temp, L'{', L' ');
		params = Parser::Replace(params, L'}', L' ');
		params = Parser::Replace(params, L';', L' ');
		wstringstream paramStream(params);
		UINT baseId = 0, championId = 0;
		double chance = 0.0f;
		while(paramStream >> baseId >> championId >> chance)
		{
			ChampionNpcInfo* pInfo = new ChampionNpcInfo();
			pInfo->baseNpcId = baseId;
			pInfo->championNpcId = championId;
			pInfo->chance = chance;
			m_Npc.push_back(pInfo);
		}
	}
	VIRTUALIZER_TIGER_BLACK_END;
}

#pragma optimize("", on)

UINT CChampionNpc::GetNpcClassId(UINT baseNpc)
{
	guard;
	if(m_Enabled)
	{
		for(UINT n=0;n<m_Npc.size();n++)
		{
			ChampionNpcInfo *pInfo = m_Npc[n];
			if(pInfo->baseNpcId == baseNpc)
			{
				double dice = g_Random.RandDouble(100.0f);
				if(pInfo->chance > dice)
				{
					baseNpc = pInfo->championNpcId;
					g_Logger.Add(L"ChampionNpc: changing npc classId from[%d] to [%d]", pInfo->baseNpcId, pInfo->championNpcId);
				}
				break;
			}
		}
	}
	unguard;
	return baseNpc;
}