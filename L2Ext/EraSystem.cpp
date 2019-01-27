#include "StdAfx.h"

CEraSystem::CEraSystem()
{
	Moon = NOT_USED;
	memset(&Data, 0, sizeof(EraInfo));
}

void CEraSystem::Initialize(const unsigned char* packet)
{
	Lock();
	packet = (PUCHAR)Disassemble(packet, "dddddd", &Data.nCurrentEra, &Data.nCount, &Data.nDuration, &Data.nLastChange, &Data.StartTime, &Data.UseMoon);
	if(Data.nCount < 32)
	{
		for(DWORD n=0;n<Data.nCount;n++)
		{
			packet = Disassemble(packet, "dS", &Data.MoonState[n], sizeof(Data.EraNames[0]), Data.EraNames[n]);
		}
	}
	Unlock();
	g_Log.Add(CLog::Blue, "[%s] Done.", __FUNCTION__);
	// start spawning npcs
	if(Data.nCurrentEra > 0)
	{
		if(Data.nCurrentEra <= Data.nCount)
		{
			ValidateMoon();
			g_Log.Add(CLog::Blue, "[%s] Spawning new era[%S] npcs. Moon[%d]", __FUNCTION__, Data.EraNames[Data.nCurrentEra-1], Moon);
			EnableNpcPosEvent(Data.EraNames[Data.nCurrentEra-1]);
			if(Data.UseMoon)
			{
				CSevenSignSky pck(Moon);
				L2Server::BroadcastToAllUser(pck.GetSize(), pck.GetBuff());
			}
		}else
			g_Log.Add(CLog::Error, "[%s] Invalid Era[%d] Max[%d]", __FUNCTION__, Data.nCurrentEra, Data.nCount);
	}
}

void CEraSystem::ProcessingFinished(int nEraIndex, PWCHAR wEra, bool Spawned)
{
	if(Spawned)
	{
		//Idle
		g_Log.Add(CLog::Blue, "[%s] Era[%S] npcs have been spawned.", __FUNCTION__, wEra);
	}else
	{
		//start spawning current era npc - real new era start
		g_Log.Add(CLog::Blue, "[%s] New Era[%S] has started.", __FUNCTION__, Data.EraNames[Data.nCurrentEra-1]);
		ValidateMoon();
		if(Data.UseMoon)
		{
			CSevenSignSky pck(Moon);
			L2Server::BroadcastToAllUser(pck.GetSize(), pck.GetBuff());
		}
		if(Data.nCurrentEra > 0)
		{
			g_Log.Add(CLog::Blue, "[%s] Spawning new era[%S] npcs. Moon[%d]", __FUNCTION__, Data.EraNames[Data.nCurrentEra-1], Moon);
			EnableNpcPosEvent(Data.EraNames[(Data.nCurrentEra-1)]);
		}
	}
}

void CEraSystem::ChangeEra(DWORD nEra, PWCHAR wEra, DWORD nLastChanged)
{
	if(nEra <= Data.nCount)
	{
		int nCurrentEra = Data.nCurrentEra;
		if(nCurrentEra > 0)
		{
			//start despawning npcs
			g_Log.Add(CLog::Blue, "[%s] Despawning old era[%S] npcs. Setting new era[%S]", __FUNCTION__, Data.EraNames[nCurrentEra-1], wEra);
			DisableNpcPosEvent(Data.EraNames[nCurrentEra - 1]);
			
			Data.nCurrentEra = nEra;
			Data.nLastChange = nLastChanged;
		}else
		{
			
			Data.nCurrentEra = nEra;
			Data.nLastChange = nLastChanged;
			if(nEra > 0)
			{
				ValidateMoon();
				g_Log.Add(CLog::Blue, "[%s] Spawning new era[%S] npcs. Moon[%d]", __FUNCTION__, Data.EraNames[nEra-1], Moon);
				EnableNpcPosEvent(Data.EraNames[nEra-1]);
				if(Data.UseMoon)
				{
					CSevenSignSky pck(Moon);
					L2Server::BroadcastToAllUser(pck.GetSize(), pck.GetBuff());
				}
			}
		}
	}else
		g_Log.Add(CLog::Error, "[%s] Invalid Era[%d]", __FUNCTION__, nEra);
}

void CEraSystem::TimerExpired()
{
}

void CEraSystem::ValidateMoon()
{
	if(Data.UseMoon)
	{
		int nEra = Data.nCurrentEra;
		if(nEra > 0)
		{
			int nMoon = Data.MoonState[nEra-1];
			switch(nMoon)
			{
			case NORMAL:
				Moon = NORMAL;
				break;
			case DUSK:
				Moon = DUSK;
				break;
			case DAWN:
				Moon = DAWN;
				break;
			}
		}else
			Moon = NOT_USED;
	}
}

void CEraSystem::OnEnterWorld(User *pUser)
{
	if(pUser->ValidUser())
	{
		if(Data.UseMoon)
		{
			CSevenSignSky pck(Moon);
			pUser->GetSocket()->Send(&pck);
		}
	}
}

void CEraSystem::ForceChangeEraGM(User *pUser, DWORD nEra)
{
	if(pUser->ValidUser())
	{
		if(nEra <= Data.nCount)
		{

		}else
			g_Log.Add(CLog::Error, "[%s] Invalid Era[%d]", __FUNCTION__, nEra);
	}
}