#include "StdAfx.h"
#include "DB.h"
#include "ObjectDB.h"

CNpcSpawnDefine::CNpcSpawnDefine() : npcClassId(0), x(0), y(0), z(0), heading(0), startHour(-1), endHour(-1), respawnTime(0), saveInDB(0), spawnTime(0), deathTime(0), npcObjectId(0)
{
	memset(dayFlag, 0, sizeof(dayFlag));
}

CNpcMaker::CNpcMaker()
{
	InitializeCriticalSection(&lock);
	m_enabled = FALSE;
	loadedFromDB = false;
	requestedDB = false;
	requestReload = false;
}

CNpcMaker::~CNpcMaker()
{

	DeleteCriticalSection(&lock);
}

void CNpcMaker::Initialize()
{
	m_enabled = FALSE;
	ReadData();
	if(m_enabled)
	{
		g_Log.Add(CLog::Blue, "NpcMakerEx feature is enabled. Loaded [%d] npcs.",  lData.size());
	}
}

void CNpcMaker::ReadData()
{
	ifstream file("..\\Script\\NpcPosEx.txt");
	Lock();
	if(file)
	{
		DespawnAll();
		string sLine;
		while(getline(file, sLine))
		{
			//npc_pos_begin	npc_class_id=100100	start_date=2010-10-29-20-00	spawn_reuse=3600	spawn_x=12111	spawn_y=123123	spawn_z=2321	save_in_db=0	npc_pos_end;
			if(sLine.find("//") == 0)
				continue;
			if(sLine.find("setting_begin") == 0)
			{
				m_enabled = CParser::GetValueInt(sLine, "enabled");
			}else if(sLine.find("npc_pos_begin") == 0)
			{
				CNpcSpawnDefine data;
				data.spawnTime = 0;
				data.deathTime = 0;

				data.npcClassId = CParser::GetValueInt(sLine, "npc_class_id");
				string sStart = CParser::GetValueString(sLine, "start_date");
				if(sStart.size() > 1)
				{
					int year, month, day, hour, minute;
					sscanf(sStart.c_str(), "%d-%d-%d-%d-%d", &year, &month, &day, &hour, &minute);

					data.startDate = ConvertTime(year, month, day, hour, minute);
				}else
					data.startDate = 0;

				string sEnd = CParser::GetValueString(sLine, "end_date");
				if(sEnd.size() > 1)
				{
					int year, month, day, hour, minute;
					sscanf(sEnd.c_str(), "%d-%d-%d-%d-%d", &year, &month, &day, &hour, &minute);
					
					data.endDate = ConvertTime(year, month, day, hour, minute);
				}else
					data.endDate = INT_MAX;
			
				UINT dayFlag = CParser::GetValueInt(sLine, "day_flag", 1111111);
				for(UINT m = 0; m < 7; m++)
				{
					if((dayFlag % 10) == 1)
					{
						data.dayFlag[m] = true;
					}else
					{
						data.dayFlag[m] = false;
					}
					dayFlag /= 10;
				}
				data.startHour = CParser::GetValueInt(sLine, "start_hour", 0);
				data.startMinute = CParser::GetValueInt(sLine, "start_minute", 0);
				data.endHour = CParser::GetValueInt(sLine, "end_hour", INT_MAX);

				data.respawnTime = CParser::GetValueInt(sLine, "respawn_time", 0);
				data.saveInDB = CParser::GetValueInt(sLine, "save_in_db");
				
				string sPos = CParser::GetValueString(sLine, "position");
				//pos={148560;27104;-2208;16356}
				sPos = CParser::Replace(sPos, '{', ' ');
				sPos = CParser::Replace(sPos, '}', ' ');
				sPos = CParser::Replace(sPos, ';', ' ');
				stringstream sstrpos;
				sstrpos << sPos;
				sstrpos >> data.x >> data.y >> data.z >> data.heading;
							
				
				string sAiParams = CParser::GetValueString(sLine, "ai_parameters");
				//ai_parameters={[fnAgitMap]=[gludin];[fnAgitMap]=[gludin];[fnAgitMap]=[gludin]}
				sAiParams = CParser::Replace(sAiParams, '{', ' ');
				sAiParams = CParser::Replace(sAiParams, '}', ' ');
				sAiParams = CParser::Trim(sAiParams);
				sAiParams = CParser::Replace(sAiParams, ';', ' ');
				stringstream sstr;
				sstr << sAiParams;
				string sParam;
				while(sstr >> sParam)
				{
					//[fnAgitMap]=[gludin]
					sParam = CParser::Replace(sParam, '=', ' ');
					sParam = CParser::Replace(sParam, ']', ' ');
					sParam = CParser::Replace(sParam, '[', ' ');
					
					stringstream sstr2;
					sstr2 << sParam;
					string p1, p2;
					sstr2 >> p1 >> p2;
					wstring wParam1 = StringToWString(p1);
					wstring wParam2 = StringToWString(p2);
					if(wParam2.find(L"@") == 0)
					{
						wParam2 = wParam2.substr(1, (wParam2.size() - 1 ));
						int skillId, skillLevel;
						g_SkillDB.GetSkillIdLevelByName((PWCHAR)wParam2.c_str(), &skillId, &skillLevel);
						if(skillId > 0)
						{
							DWORD classId = (skillId << 8) + skillLevel;
							wstringstream wsstr2;
							wsstr2 << classId;
							wsstr2 >> wParam2;							
						}else
						{
							g_Log.Add(CLog::Error, "[%s] Invalid skill name[%S]!", __FUNCTION__, wParam2.c_str());
						}
					}
					data.ai_parameters.insert(pair<wstring, wstring>(wParam1, wParam2));
				}

				lData.push_back(data);
			}
		}
		file.close();
	}

	Unlock();
	g_Log.Add(CLog::Blue, "[%s] Loaded [%d] npcs.", __FUNCTION__, lData.size());
}

time_t CNpcMaker::GetFixedSpawnTime(tmEx startTime, time_t spawnTime, DWORD spawnReuse)
{
	time_t currentTime = time(NULL);
	time_t fixedSpawnTime = 0;
	if(startTime.tm_year > 0)
	{
		time_t start = mktime(&startTime);
		if(start < spawnTime)
		{
			time_t diffTime = spawnTime - start;
			diffTime /= spawnReuse;
			fixedSpawnTime = start + (diffTime*spawnReuse);
		}
	}else
	{
		fixedSpawnTime = spawnTime;
	}

	return fixedSpawnTime;
}

void CNpcMaker::SpawnTick(CNpcSpawnDefine& spawnDefine)
{
	if(spawnDefine.npcClassId)
	{
		time_t currentTime = time(0);
		tm timeInfo;
		GetTimeInfo(timeInfo);
		if(currentTime >= spawnDefine.startDate && currentTime < spawnDefine.endDate)
		{
			//check for day flag
			if(spawnDefine.dayFlag[timeInfo.tm_wday])
			{
				if(spawnDefine.respawnTime == 0)
				{
					if(spawnDefine.startHour == timeInfo.tm_hour && spawnDefine.startMinute == timeInfo.tm_min)
					{
						if(spawnDefine.spawnTime + 90 < currentTime)
						{
							SpawnNpc(spawnDefine);
						}
					}
				}else if(spawnDefine.startHour <= timeInfo.tm_hour && spawnDefine.endHour > timeInfo.tm_hour)
				{
					//check for minute
					if(spawnDefine.startMinute <= timeInfo.tm_min || spawnDefine.startHour < timeInfo.tm_hour)
					{
						//check for respawn time
						if( (spawnDefine.deathTime + spawnDefine.respawnTime) < currentTime )
						{
							SpawnNpc(spawnDefine);
						}
					}
				}
			}
		}
	}
}

void CNpcMaker::DespawnTick(CNpcSpawnDefine& spawnDefine)
{
	
	if(spawnDefine.npcObjectId)
	{
		time_t currentTime = time(0);
		tm timeInfo;
		GetTimeInfo(timeInfo);
		if(currentTime >= spawnDefine.startDate && currentTime < spawnDefine.endDate)
		{
			//check end hour
			if(spawnDefine.endHour <= timeInfo.tm_hour)
			{
				DespawnNpc(spawnDefine);
			}else if(spawnDefine.endHour == 24 && timeInfo.tm_hour == 23 && timeInfo.tm_min == 59 && timeInfo.tm_sec > 50)
			{
				DespawnNpc(spawnDefine);
			}
		}else
		{
			DespawnNpc(spawnDefine);
		}
	}
}

void CNpcMaker::TimerExpired()
{
	guard;

	if(requestReload)
	{
		Initialize();
		requestReload = false;
	}

	if(m_enabled)
	{
		Lock();
		
		if(NpcServer::IsConnected())
		{
			if(loadedFromDB)
			{
				time_t currentTime = time(NULL);
				for(SpawnDefineList::iterator Iter = lData.begin();Iter!=lData.end();Iter++)
				{
					if(Iter->npcObjectId)
					{
						DespawnTick(*Iter);
					}else
					{
						SpawnTick(*Iter);
					}
				}
			}else
			{
				if(!requestedDB)
				{
					g_DB.RequestLoadNpcDeath();
					requestedDB = true;
				}
			}
		}
		Unlock();
	}
	unguard;
}

void CNpcMaker::SetRequestReload()
{
	Lock();
	requestReload = true;
	Unlock();
}

void CNpcMaker::SpawnNpc(CNpcSpawnDefine &data)
{
	int nClassID = data.npcClassId;
	const WCHAR* wszName = g_ObjectDB.GetName(nClassID);
	CNPC *pNPC = g_ObjectDB.CreateNPC(nClassID);
	if(pNPC)
	{
		const WCHAR* wszAI = g_ObjectDB.GetAI(nClassID);
		if(wszAI)
		{
			//ai params
			CHAR ai_params[4096];
			int ai_params_len = 0;
			int nAiParams = 0;
			memset(ai_params, 0, sizeof(ai_params));
			for(map<wstring, wstring>::iterator Iter = data.ai_parameters.begin();Iter!=data.ai_parameters.end();Iter++)
			{
				ai_params_len += Assemble(&ai_params[ai_params_len], 4096 - ai_params_len, "SS", Iter->first.c_str(), Iter->second.c_str());
			//	g_Log.Add(CLog::Blue, "[%s] Npc[%S] param[%S] value[%S]", __FUNCTION__, wszName, Iter->first.c_str(), Iter->second.c_str());
				nAiParams++;
			}

			//Fill data with correct info
			CHAR buff[8190];
			memset(buff, 0, 8190);

			int nBuffSize = Assemble(buff, 8190,"cddpddddddddSdb", 0x01, pNPC->pSD->nServerIndex, pNPC->nObjectID, 
				0, 0, data.x, data.y, data.z,
				data.heading, 0, 0, 0, wszAI, nAiParams, ai_params_len, ai_params);
			NpcServer::Send(nBuffSize, buff);
			data.npcObjectId = pNPC->nObjectID;
			data.spawnTime = time(NULL);
		}else
			g_Log.Add(CLog::Error, "[%s] There is no AI specified for npc with class id[%d]", __FUNCTION__, nClassID);
	}else
		g_Log.Add(CLog::Error, "[%s] There is no npc with class id[%d]", __FUNCTION__, nClassID);

}

void CNpcMaker::DespawnNpc(CNpcSpawnDefine& spawnDefine)
{
	
	CNPC* pNPC = CNPC::GetNPCByServerId(&spawnDefine.npcObjectId);
	if(pNPC->ValidCreature())
	{
		pNPC->KillNPC();
	}

}

void CNpcMaker::LoadedFromDB(int npcClassId, DWORD spawnTime)
{
	Lock();
	for(SpawnDefineList::iterator Iter = lData.begin();Iter!=lData.end();Iter++)
	{
		if(Iter->npcClassId == npcClassId)
		{
		//	Iter->spawnTime = GetFixedSpawnTime(Iter->startDate, spawnTime, Iter->respawnTime);
			break;
		}
	}
	Unlock();
}

void CNpcMaker::SetLoadedFromDB(bool Value)
{ 
	Lock();
	loadedFromDB = Value; 
	Unlock(); 
};

void CNpcMaker::OnNpcDie(CNPC *pNPC, bool locked)
{
	if(m_enabled)
	{
		if(pNPC->ValidCreature())
		{
			if(!locked)
			{
				Lock();
			}
			for(SpawnDefineList::iterator Iter = lData.begin();Iter!=lData.end();Iter++)
			{
				if(Iter->npcObjectId == pNPC->nObjectID)
				{
					Iter->npcObjectId = 0;
					Iter->deathTime = time(NULL);

					if(Iter->saveInDB)
					{
						g_DB.RequestSaveNpcDeath(Iter->npcClassId, Iter->deathTime);	//should be spawn time depends on how should it work
					}
					break;
				}
			}
			if(!locked)
			{
				Unlock();
			}
		}
	}
}

void CNpcMaker::DespawnAll()
{
	for(SpawnDefineList::iterator Iter = lData.begin();Iter!=lData.end();Iter++)
	{
		if(Iter->npcObjectId)
		{
			//despawn
			CNPC* pNPC = CNPC::GetNPCByServerId(&Iter->npcObjectId);
			if(pNPC->ValidCreature())
			{
				pNPC->KillNPC();
			}
		}
	}
	lData.clear();
}