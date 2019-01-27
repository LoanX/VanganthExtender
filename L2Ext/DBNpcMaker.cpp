#include "stdafx.h"
#include "DBNpcMaker.h"
#include "DB.h"
#include "NPCServer.h"
#include "ObjectDB.h"
#include "License.h"

using namespace License;

CDBNpcMaker g_DBNpcMaker;

void CNpcDbInfo::Set(UINT npcClassId, wstring name, time_t startTime, time_t endTime, int x, int y, int z, UINT heading)
{
	m_NpcClassId = npcClassId;
	m_NpcName = name;
	m_StartTime = startTime;
	m_EndTime = endTime;
	m_SpawnPos.x = x;
	m_SpawnPos.y = y;
	m_SpawnPos.z = z;
	m_Heading = heading;
}


#pragma optimize("", off)

void CDBNpcMaker::Init()
{
	m_Enabled = false;
	VIRTUALIZER_TIGER_BLACK_START;
	if(CLicense::GetInstance().CanUse(LicenseDBNpcMaker))
	{
		{
			//release data
			AUTO_LOCK(m_Lock);
			for(UINT n=0;n<m_Npc.size();n++)
			{
				DespawnNpc(m_Npc[n]);
				delete m_Npc[n];
			}
			m_Npc.clear();
		}
		LoadINI();
		if(m_Enabled)
		{
			g_Log.Add(CLog::Blue, "[%s] Feature is enabled.", __FUNCTION__);
			g_DB.RequestLoadDBNpcMaker();
		}
	}
	VIRTUALIZER_TIGER_BLACK_END;
}

void CDBNpcMaker::LoadINI()
{
	VIRTUALIZER_TIGER_BLACK_START;
	m_Enabled = GetPrivateProfileInt(_T("DBNpcMaker"), _T("Enabled"), 0, g_ConfigFile) ? true : false;
	VIRTUALIZER_TIGER_BLACK_END;
}


#pragma optimize("", on)

void CDBNpcMaker::OnDBLoad(UINT npcClassId, std::wstring name, time_t startTime, time_t endTime, int x, int y, int z, UINT heading)
{
	guard;

	CNpcDbInfo *pInfo = new CNpcDbInfo();

	pInfo->Set(npcClassId, name, startTime, endTime, x, y, z, heading);
	{
		AUTO_LOCK(m_Lock);
		m_Npc.push_back(pInfo);
	}

	unguard;
}

void CDBNpcMaker::TimerExpired()
{
	guard;
	if(m_Enabled)
	{
		time_t currentTime = time(0);
		if(NpcServer::IsConnected())
		{
			AUTO_LOCK(m_Lock);
			for(UINT n=0;n<m_Npc.size();n++)
			{
				CNpcDbInfo *pInfo = m_Npc[n];
				if(pInfo->GetStartTime() < currentTime)
				{
					if(pInfo->GetEndTime() > currentTime)
					{
						if(!pInfo->GetObjectId())
						{
							SpawnNpc(pInfo);
						}
					}else
					{
						if(pInfo->GetObjectId())
						{
							DespawnNpc(pInfo);
						}
					}
				}
			}
		}else
		{
			//disconnected
			AUTO_LOCK(m_Lock);
			//despawn npcs
			for(UINT n=0;n<m_Npc.size();n++)
			{
				CNpcDbInfo *pInfo = m_Npc[n];
				if(pInfo->GetObjectId())
				{
					DespawnNpc(pInfo);
				}
			}
		}
	}
	unguard;
}

void CDBNpcMaker::SpawnNpc(CNpcDbInfo *pInfo)
{
	const WCHAR* wszName = g_ObjectDB.GetName(pInfo->GetClassId());
	CNPC *pNPC = g_ObjectDB.CreateNPC(pInfo->GetClassId());
	if(pNPC)
	{
		const WCHAR* wszAI = g_ObjectDB.GetAI(pInfo->GetClassId());
		if(wszAI)
		{ 
			//ai params
			CHAR ai_params[4096];
			int ai_params_len = 0;
			int nAiParams = 0;
			memset(ai_params, 0, sizeof(ai_params));
			for(map<const wstring, const wstring>::iterator it = pNPC->pND->aiParams.begin(); it != pNPC->pND->aiParams.end(); it++)
			{
				ai_params_len += Assemble(&ai_params[ai_params_len], 4096 - ai_params_len, "SS", it->first.c_str(), it->second.c_str());
				nAiParams++;
			}

			//Fill data with correct info
			CHAR buff[8190];
			memset(buff, 0, 8190);
			IVector pos = pInfo->GetSpawnPos();
			int nBuffSize = Assemble(buff, 8190,"cddpddddddddSdb", 0x01, pNPC->pSD->nServerIndex, pNPC->nObjectID, 
				0, 0, pos.x, pos.y, pos.z, pInfo->GetHeading(), 0, 0, 0, wszAI, nAiParams, ai_params_len, ai_params);
			NpcServer::Send(nBuffSize, buff);
			pInfo->SetObjectId(pNPC->nObjectID);
		}else
			g_Log.Add(CLog::Error, "[%s] There is no AI specified for npc with class id[%d]", __FUNCTION__, pInfo->GetClassId());
	}else
		g_Log.Add(CLog::Error, "[%s] There is no npc with class id[%d]", __FUNCTION__, pInfo->GetClassId());

}

void CDBNpcMaker::DespawnNpc(CNpcDbInfo *pInfo)
{
	UINT objectId = pInfo->GetObjectId();
	if(CNPC* pNPC = CNPC::GetNPCByServerId(&objectId))
	{
		pNPC->KillNPC();
	}
	pInfo->SetObjectId(0);
}