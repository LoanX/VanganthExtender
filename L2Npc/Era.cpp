#include "L2NPC.h"

static void FinishedProcessing(CLog *pLog, CLog::LogType Type, const wchar_t *format, PWCHAR wEventName, DWORD Enabled, int Count, int MaxCount, int ElapsedSec)
{
	typedef void (*f)(CLog*, const wchar_t*, PWCHAR, DWORD, int, int, int);
	f(0x4AAAE0)(pLog, format, wEventName, Enabled, Count, MaxCount, ElapsedSec);
	
	if(gConfig.IsSet(Config::ERA))
	{
		int nEraIndex = g_EraSystem.GetEraIndex(wEventName);
		if(nEraIndex != -1)
		{
			g_Log.Add(CLog::Blue, "[%s] EventName[%S] Enabled[%d] EraIndex[%d]", __FUNCTION__, wEventName, Enabled, nEraIndex);
			g_EraSystem.ProcessingDone(nEraIndex, Enabled);
		}
	}
}

CEraSystem::CEraSystem()
{
	memset(&Data, 0, sizeof(EraInfo));
	WriteHook(CALL, 0x557D6E, (DWORD)FinishedProcessing, 0);
}

void CEraSystem::Initialize()
{
	if(gConfig.IsSet(Config::ERA))
	{
		Data.nDuration = gConfig.EraInfo.GetDuration();
		Data.nCount = gConfig.EraInfo.GeCount();
		Data.nCurrentEra = gConfig.EraInfo.GetCurrentEra();
		Data.nLastChange = gConfig.EraInfo.GetLastChange();
		Data.StartTime = gConfig.EraInfo.GetStartTime();
		Data.UseMoon = gConfig.EraInfo.GetUseMoon();

		g_Era.SetVariable(Data.nCurrentEra);

		wstringstream sstr;
		sstr << gConfig.EraInfo.GetEraNames();
		wstring sName;
		int n = 0;
		while(sstr >> sName)
		{
			if(sName.size() > 0 && n < 32)
			{
				wcscpy_s(Data.EraNames[n], 25, sName.c_str());
				//MultiByteToWideChar(CP_ACP, 0, sName.c_str(), -1, Data.EraNames[n], 25);
				n++;
			}
		}
		sstr.clear(); n = 0;
		sstr << gConfig.EraInfo.GetMoonStates();
		int nMoonState = 0;
		while(sstr >> nMoonState)
		{
			if(nMoonState >= 0 && n < 32)
			{
				Data.MoonState[n] = nMoonState;
				n++;
			}
		}
		
		ValidateEra();
		g_Log.Add(CLog::Blue, "[%s] Era Count[%d]", __FUNCTION__, Data.nCount);

		for(int n=0;n<Data.nCount;n++)
		{
			g_Log.Add(CLog::Blue, "[%s] Era[%S] Moon[%d] Index[%d]", __FUNCTION__, Data.EraNames[n], Data.MoonState[n], (n+1));
		}
		//preparing init packet
		UCHAR Buff[8192]; memset(Buff, 0, sizeof(Buff));
		int nLen = Assemble(Buff, 8192, "dddddd", Data.nCurrentEra, Data.nCount, Data.nDuration, Data.nLastChange, Data.StartTime, Data.UseMoon);
		for(int n=0;n<Data.nCount;n++)
			nLen += Assemble((PUCHAR)((INT64)Buff + nLen), (8192-nLen), "dS", Data.MoonState[n], Data.EraNames[n]);
		
		g_NpcServer.Send("cdb", 0x38, INITIALIZE_ERA, nLen, Buff);
	}
}

int CEraSystem::GetEraIndex(PWCHAR wName)
{
	for(int n=0;n<Data.nCount;n++)
	{
		if(!wcscmp(wName, Data.EraNames[n]))
			return n;
	}
	return -1;
}

void CEraSystem::TimerExpired()
{
	if(gConfig.IsSet(Config::ERA))
	{
		int nEra = Data.nCurrentEra;
		switch(ValidateEra())
		{
		case NOTHING:
		//	g_Log.Add(CLog::Blue, "[%s] Debug: LastChange[%d] Duration[%d] Time[%d] Era[%d]", __FUNCTION__, Data.nLastChange, Data.nDuration, time(NULL), Data.nCurrentEra);
			break;
		case ERA_CHANGE:
			g_Log.Add(CLog::Blue, "[%s] Era has been changed: OldEra[%d] NewEra[%d]", __FUNCTION__, nEra, Data.nCurrentEra);
			//send packet requesting despawn npc and set new era id
			UCHAR Buff[8192]; memset(Buff, 0, sizeof(Buff));
			int nLen = Assemble(Buff, 8192, "dSd", Data.nCurrentEra, Data.EraNames[Data.nCurrentEra-1], Data.nLastChange);
			g_NpcServer.Send("cdb", 0x38, CHANGE_ERA, nLen, Buff);
			break;
		}
	}
}

CEraSystem::STATE CEraSystem::ValidateEra()
{
	if(gConfig.IsSet(Config::ERA))
	{
		if(Data.nCount > 0)
		{
			if(Data.nCurrentEra == 0)
			{
				DWORD TimeDiff = time(NULL) - Data.StartTime;
				if(TimeDiff > 0)
				{
					if(TimeDiff > Data.nDuration)
					{
						//calculates cycle
						int nEra = ((TimeDiff / Data.nDuration) % Data.nCount) + 1;
						SetCurrentEra(nEra);
					}else
					{
						SetCurrentEra(1);
					}
					return CEraSystem::START;
				}
			}else
			{
				DWORD TimeDiff = time(NULL) - Data.nLastChange;
				if(TimeDiff > Data.nDuration)
				{
					if(Data.nCurrentEra < Data.nCount)
						SetCurrentEra(Data.nCurrentEra + 1);
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
	g_Era.SetVariable(nEra);
	if(Data.nCurrentEra == 0 || Data.nLastChange == 0)
	{
		Data.nLastChange = Data.StartTime;
		time_t TimeDiff = time(NULL) - Data.StartTime;
		if(TimeDiff > Data.nDuration)
		{
			int nCycle = TimeDiff / Data.nDuration;
			Data.nLastChange += (Data.nDuration * nCycle);
		}
	}else
	{
		time_t TimeDiff = time(NULL) - Data.nLastChange;
		if(TimeDiff > Data.nDuration)
		{
			int nCycle = TimeDiff / Data.nDuration;
			Data.nLastChange += (Data.nDuration * nCycle);
		}else
			g_Log.Add(CLog::Error, "[%s] TimeDiff[%d] lower than Duration[%d]", __FUNCTION__, TimeDiff, Data.nDuration);
	}


	Data.nCurrentEra = nEra;
	gConfig.EraInfo.SetLastChange(Data.nLastChange);
	gConfig.EraInfo.SetCurrentEra(nEra);
	gConfig.EraInfo.Save();
}

void CEraSystem::ProcessingDone(int nEraIndex, bool Spawned)
{
	if(nEraIndex < Data.nCount && nEraIndex > -1)
	{
		UCHAR Buff[8192]; memset(Buff, 0, sizeof(Buff));
		int nLen = 0;
		nLen = Assemble(Buff, 8192, "dSd", nEraIndex, Data.EraNames[nEraIndex], (DWORD)Spawned);
		g_NpcServer.Send("cdb", 0x38, PROCESSING_ERA_NPC_DONE, nLen, Buff);
	}else
		g_Log.Add(CLog::Error, "[%s] Invalid Era Index[%d]", __FUNCTION__, nEraIndex);
}