#include "StdAfx.h"
#include "ObjectDB.h"

void NpcServer::Send(int nBuffSize, PCHAR buff)
{
	typedef void (*_Send)(INT64, const char *, int, PCHAR);
	_Send Send = (_Send) 0x006CCD40;
	Send(nServer, "b", nBuffSize, buff);
}

bool NpcServer::IsConnected()
{
	LPDWORD lpConnected = (LPDWORD)0x1557FF8;
	if((*lpConnected) > 0)
		return true;
	return false;
}

void NpcServer::SpawnNpc(UINT npcClassId, int x, int y, int z)
{
	const WCHAR* lpName = g_ObjectDB.GetName(npcClassId);
	if(lpName)
	{
		char buff[8190];
		int len = Assemble(buff, 8190, "cSdddS", 0xB, lpName, x, y, z, L"");
		NpcServer::Send(len, buff);
	}else
	{
		g_Log.Add(CLog::Error, "[%s] Cannot find npc [%d]", __FUNCTION__, npcClassId);
	}
}

UINT NpcServer::SpawnNpcEx(UINT npcClassId, int x, int y, int z, map<wstring, wstring>& aiParams)
{
	UINT nServerId = 0;
	const WCHAR* wszName = g_ObjectDB.GetName(npcClassId);
	CNPC *pNPC = g_ObjectDB.CreateNPC(npcClassId);
	if(pNPC)
	{
		const WCHAR* wszAI = g_ObjectDB.GetAI(npcClassId);
		if(wszAI)
		{
			//ai params
			CHAR ai_params[4096];
			int ai_params_len = 0;
			int nAiParams = 0;
			memset(ai_params, 0, sizeof(ai_params));
			for(map<wstring, wstring>::iterator Iter = aiParams.begin();Iter!=aiParams.end();Iter++)
			{
				ai_params_len += Assemble(&ai_params[ai_params_len], 4096 - ai_params_len, "SS", Iter->first.c_str(), Iter->second.c_str());
			//	g_Log.Add(CLog::Blue, "[%s] Npc[%S] param[%S] value[%S]", __FUNCTION__, wszName, Iter->first.c_str(), Iter->second.c_str());
				nAiParams++;
			}

			//Fill data with correct info
			CHAR buff[8190];
			memset(buff, 0, 8190);

			int nBuffSize = Assemble(buff, 8190,"cddpddddddddSdb", 0x01, pNPC->pSD->nServerIndex, pNPC->nObjectID, 
				0, 0, x, y, z,
				0, 0, 0, 0, wszAI, nAiParams, ai_params_len, ai_params);
			NpcServer::Send(nBuffSize, buff);
			nServerId = pNPC->nObjectID;
		}else
			g_Log.Add(CLog::Error, "[%s] There is no AI specified for npc with class id[%d]", __FUNCTION__, npcClassId);
	}else
		g_Log.Add(CLog::Error, "[%s] There is no npc with class id[%d]", __FUNCTION__, npcClassId);

	return nServerId;
}