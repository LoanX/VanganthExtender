#include "StdAfx.h"
#include "TerritoryChecker.h"

CTerritoryChecker::CTerritoryChecker()
{
}

CTerritoryChecker::~CTerritoryChecker()
{
	try
	{
		if(mData.size() > 0)
		{
			try
			{
				for(map<int, CTerritory*>::iterator Iter = mData.begin();Iter!=mData.end();Iter++)
				{
					if(Iter->second)
					{
						try
						{
							delete Iter->second;
							Iter->second = NULL;
						}catch(...)
						{
							EXCEPTION_LOG;
						}
					}
				}
				try
				{
					mData.clear();
				}catch(...)
				{
					EXCEPTION_LOG;
				}
			}catch(...)
			{
				EXCEPTION_LOG;
			}
		}
		
	}catch(...)
	{
		EXCEPTION_LOG;
	}
}

void CTerritoryChecker::Initialize()
{
	g_Log.Add(CLog::Blue, "[%s] Loading Data...", __FUNCTION__);
	ReadData("..\\Script\\TerritoryData.txt");
}

void CTerritoryChecker::ReadData(std::string sFile)
{
	try
	{
		ifstream file(sFile.c_str());
		if(file)
		{
			string sLine;
			try
			{
				if(mData.size() > 0)
				{
					for(map<int, CTerritory*>::iterator Iter = mData.begin();Iter!=mData.end();Iter++)
					{
						if(Iter->second)
							delete Iter->second;
						Iter->second = NULL;
					}
					mData.clear();
				}
			}catch(...)
			{
				EXCEPTION_LOG;
			}
			INT32 ID = 0;
			while(getline(file, sLine))
			{
				//ParseData
				if(sLine.find("begin") == 0)
				{
					//read base data
					ID++;
					double x = CParser::GetValueDouble(sLine, "MinX");
					double X = CParser::GetValueDouble(sLine, "MaxX");
					double y = CParser::GetValueDouble(sLine, "MinY");
					double Y = CParser::GetValueDouble(sLine, "MaxY");
					double z = CParser::GetValueDouble(sLine, "MinZ");
					double Z = CParser::GetValueDouble(sLine, "MaxZ");
					string sType = CParser::GetValueString(sLine, "Type");
					if(sType == "GET_OFF_WYVERN")
					{
						CTerritory *pTerritory = new CTerritory();
						pTerritory->Set(x,X, y, Y, z, Z, FORBIDDEN_FLY_ZONE);
						mData.insert(pair<int, CTerritory*>(ID, pTerritory));
					}else if(sType == "BLOCK_SUMMONING_PC")
					{
						CTerritory *pTerritory = new CTerritory();
						pTerritory->Set(x,X, y, Y, z, Z, FORBIDDEN_RECALL_ZONE);
						mData.insert(pair<int, CTerritory*>(ID, pTerritory));
					}else if(sType == "BLOCK_RIDE")
					{
						CTerritory *pTerritory = new CTerritory();
						pTerritory->Set(x,X, y, Y, z, Z, FORBIDDEN_RIDE_ZONE);
						mData.insert(pair<int, CTerritory*>(ID, pTerritory));
					}else if(sType == "GLUDIO_CASTLE")
					{
						CTerritory *pTerritory = new CTerritory();
						pTerritory->Set(x,X, y, Y, z, Z, GLUDIO_CASTLE);
						mData.insert(pair<int, CTerritory*>(ID, pTerritory));
					}else if(sType == "DION_CASTLE")
					{
						CTerritory *pTerritory = new CTerritory();
						pTerritory->Set(x,X, y, Y, z, Z, DION_CASTLE);
						mData.insert(pair<int, CTerritory*>(ID, pTerritory));
					}else if(sType == "GIRAN_CASTLE")
					{
						CTerritory *pTerritory = new CTerritory();
						pTerritory->Set(x,X, y, Y, z, Z, GIRAN_CASTLE);
						mData.insert(pair<int, CTerritory*>(ID, pTerritory));
					}else if(sType == "OREN_CASTLE")
					{
						CTerritory *pTerritory = new CTerritory();
						pTerritory->Set(x,X, y, Y, z, Z, OREN_CASTLE);
						mData.insert(pair<int, CTerritory*>(ID, pTerritory));
					}else if(sType == "ADEN_CASTLE")
					{
						CTerritory *pTerritory = new CTerritory();
						pTerritory->Set(x,X, y, Y, z, Z, ADEN_CASTLE);
						mData.insert(pair<int, CTerritory*>(ID, pTerritory));
					}else if(sType == "INNADRIL_CASTLE")
					{
						CTerritory *pTerritory = new CTerritory();
						pTerritory->Set(x,X, y, Y, z, Z, INNADRIL_CASTLE);
						mData.insert(pair<int, CTerritory*>(ID, pTerritory));
					}else if(sType == "GODDARD_CASTLE")
					{
						CTerritory *pTerritory = new CTerritory();
						pTerritory->Set(x,X, y, Y, z, Z, GODDARD_CASTLE);
						mData.insert(pair<int, CTerritory*>(ID, pTerritory));
					}else if(sType == "RUNE_CASTLE")
					{
						CTerritory *pTerritory = new CTerritory();
						pTerritory->Set(x,X, y, Y, z, Z, RUNE_CASTLE);
						mData.insert(pair<int, CTerritory*>(ID, pTerritory));
					}else if(sType == "SCHUTTGARD_CASTLE")
					{
						CTerritory *pTerritory = new CTerritory();
						pTerritory->Set(x,X, y, Y, z, Z, SCHUTTGARD_CASTLE);
						mData.insert(pair<int, CTerritory*>(ID, pTerritory));
					}else if(sType == "BLOCK_OFFLINE_SHOP")
					{
						CTerritory *pTerritory = new CTerritory();
						pTerritory->Set(x,X, y, Y, z, Z, FORBIDDEN_OFFLINE_SHOP_ZONE);
						mData.insert(pair<int, CTerritory*>(ID, pTerritory));
					}else if(sType == "BLOCK_TRADE")
					{
						CTerritory *pTerritory = new CTerritory();
						pTerritory->Set(x,X, y, Y, z, Z, FORBIDDEN_TRADE_ZONE);
						mData.insert(pair<int, CTerritory*>(ID, pTerritory));
					}else if(sType == "TRADE_GEODATA_CHECK")
					{
						CTerritory *pTerritory = new CTerritory();
						pTerritory->Set(x,X, y, Y, z, Z, TRADE_GEODATA_CHECK_ZONE);
						mData.insert(pair<int, CTerritory*>(ID, pTerritory));
					}else if(sType == "SSQ_ZONE")
					{
						CTerritory *pTerritory = new CTerritory();
						pTerritory->Set(x,X, y, Y, z, Z, SSQ_ZONE);
						mData.insert(pair<int, CTerritory*>(ID, pTerritory));
					}else if(sType == "PVP_ZONE")
					{
						CTerritory *pTerritory = new CTerritory();
						pTerritory->Set(x,X, y, Y, z, Z, PVP_ZONE);
						mData.insert(pair<int, CTerritory*>(ID, pTerritory));
					}else if(sType == "ANTI_RECALL_ZONE")
					{
						CTerritory *pTerritory = new CTerritory();
						pTerritory->Set(x,X, y, Y, z, Z, ANTI_RECALL_ZONE);
						mData.insert(pair<int, CTerritory*>(ID, pTerritory));
					}else if(sType == "NICKNAME_CHANGE_ZONE")
					{
						CNickNameChangeTerritory *pTerritory = new CNickNameChangeTerritory();
						pTerritory->Set(x,X, y, Y, z, Z, NICKNAME_CHANGE_ZONE);
						string sName = CParser::GetValueString(sLine, "NickName");
						DWORD Color = CParser::GetValueInt(sLine, "Color");
						DWORD HidePledge = CParser::GetValueInt(sLine, "HidePledge");
						wchar_t wName[512]; memset(wName, 0, sizeof(wName));
						_AnsiToUnicode(sName.c_str(), wName);
						pTerritory->SetNickName(wName);
						pTerritory->SetColor(Color);
						pTerritory->SetHidePledge(HidePledge);
						mData.insert(pair<int, CTerritory*>(ID, pTerritory));
					}else if(sType == "BLOCK_ITEMS_ZONE")
					{
						CBlockItemsTerritory *pTerritory = new CBlockItemsTerritory();
						pTerritory->Set(x,X, y, Y, z, Z, BLOCK_ITEMS_ZONE);
						string sItemList = CParser::GetValueString(sLine, "ItemList");
						sItemList = CParser::Replace(sItemList, ';', ' ');
						stringstream sstr;
						sstr << sItemList;
						int ItemID = 0;
						while(sstr >> ItemID)
						{
							pTerritory->AddItem(ItemID);
							ItemID = 0;
						}
						mData.insert(pair<int, CTerritory*>(ID, pTerritory));
					}else if(sType == "BLOCK_SKILLS_ZONE")
					{
						CBlockSkillsTerritory *pTerritory = new CBlockSkillsTerritory();
						pTerritory->Set(x,X, y, Y, z, Z, BLOCK_SKILLS_ZONE);
						string sSkillList = CParser::GetValueString(sLine, "SkillList");
						sSkillList = CParser::Replace(sSkillList, ';', ' ');
						stringstream sstr;
						sstr << sSkillList;
						int SkillID = 0;
						while(sstr >> SkillID)
						{
							pTerritory->AddSkill(SkillID);
							SkillID = 0;
						}
						mData.insert(pair<int, CTerritory*>(ID, pTerritory));
					}else if(sType == "RACE_CHANGE_ZONE")
					{
						CRaceChangeTerritory *pTerritory = new CRaceChangeTerritory();
						pTerritory->Set(x,X, y, Y, z, Z, RACE_CHANGE_ZONE);
						DWORD Race = CParser::GetValueInt(sLine, "Race");
						DWORD Sex = CParser::GetValueInt(sLine, "Sex");
						pTerritory->SetData(Race, Sex);
						mData.insert(pair<int, CTerritory*>(ID, pTerritory));
					}else if(sType == "NO_OL_BUFFS_ZONE")
					{
						CTerritory *pTerritory = new CTerritory();
						pTerritory->Set(x,X, y, Y, z, Z, NO_OL_BUFFS_ZONE);
						mData.insert(pair<int, CTerritory*>(ID, pTerritory));
					}else if(sType == "BLOCK_CHAT_ZONE")
					{
						CBlockChatTerritory *pTerritory = new CBlockChatTerritory();
						pTerritory->Set(x,X, y, Y, z, Z, BLOCK_CHAT_ZONE);
						pTerritory->SetData(CParser::GetValueInt(sLine, "All"), CBlockChatTerritory::ALL);
						pTerritory->SetData(CParser::GetValueInt(sLine, "Shout"), CBlockChatTerritory::SHOUT);
						pTerritory->SetData(CParser::GetValueInt(sLine, "Tell"), CBlockChatTerritory::TELL);
						pTerritory->SetData(CParser::GetValueInt(sLine, "Party"), CBlockChatTerritory::PARTY);
						pTerritory->SetData(CParser::GetValueInt(sLine, "Clan"), CBlockChatTerritory::CLAN);
						pTerritory->SetData(CParser::GetValueInt(sLine, "Trade"), CBlockChatTerritory::TRADE);
						pTerritory->SetData(CParser::GetValueInt(sLine, "Alliance"), CBlockChatTerritory::ALLIANCE);
						pTerritory->SetData(CParser::GetValueInt(sLine, "Hero"), CBlockChatTerritory::HERO);
						pTerritory->SetData(CParser::GetValueInt(sLine, "Msn"), CBlockChatTerritory::MSN);
						
						mData.insert(pair<int, CTerritory*>(ID, pTerritory));
					}else if(sType == "BLOCK_TVT_ZONE")
					{
						CTerritory *pTerritory = new CTerritory();
						pTerritory->Set(x, X, y, Y, z, Z, BLOCK_TVT_ZONE);
						mData.insert(pair<int, CTerritory*>(ID, pTerritory));
					}else if(sType == "PVP_REWARD_ZONE")
					{
						CPvPRewardTerritory *pTerritory = new CPvPRewardTerritory();
						pTerritory->Set(x, X, y, Y, z, Z, PVP_REWARD_ZONE);
						pTerritory->SetRewardId(CParser::GetValueInt(sLine, "RewardId"));
						pTerritory->SetRewardCount(CParser::GetValueInt(sLine, "RewardCount"));
						pTerritory->SetRequiredLevel(CParser::GetValueInt(sLine, "RequiredLevel"));
						pTerritory->SetStealFromVictim(CParser::GetValueInt(sLine, "StealFromVictim"));
						mData.insert(pair<int, CTerritory*>(ID, pTerritory));
					}else if(sType == "DESPAWN_SUMMON_ZONE")
					{
						CDespawnSummonTerritory *pTerritory = new CDespawnSummonTerritory();
						pTerritory->Set(x, X, y, Y, z, Z, DESPAWN_SUMMON_ZONE);
						string sSkillList = Parser::ParseOptionString(sLine, "SummonList");
						sSkillList = CParser::Replace(sSkillList, ';', ' ');
						stringstream sstr;
						sstr << sSkillList;
						INT32 classId = 0;
						while(sstr >> classId)
						{
							pTerritory->Block(classId);
							classId = 0;
						}
						mData.insert(pair<int, CTerritory*>(ID, pTerritory));
					}else if(sType == "DISPEL_SKILL_ZONE")
					{
						CDispelSkillTerritory *pTerritory = new CDispelSkillTerritory();
						pTerritory->Set(x, X, y, Y, z, Z, DISPEL_SKILL_ZONE);
						string sSkillList = Parser::ParseOptionString(sLine, "SkillList");
						sSkillList = CParser::Replace(sSkillList, ';', ' ');
						stringstream sstr;
						sstr << sSkillList;
						string skillName;
						while(sstr >> skillName)
						{
							wstring wName = StringToWString(skillName);
							INT32 skillId = 0;
							INT32 level = 0;
							g_SkillDB.GetSkillIdLevelByName(wName.c_str(), &skillId, &level);
							if(skillId > 0 && level > 0)
							{
								pTerritory->Block(skillId, level);
							}else
							{
								g_Log.Add(CLog::Error, "[%s] Invalid skill name[%s]", __FUNCTION__, skillName.c_str());
							}
						}
						mData.insert(pair<int, CTerritory*>(ID, pTerritory));
					}else if(sType == "MINING_ZONE")
					{
						CMiningTerritory *pTerritory = new CMiningTerritory();
						pTerritory->Set(x, X, y, Y, z, Z, MINING_ZONE);
						pTerritory->SetMiningId(Parser::ParseOptionInt(sLine, "MiningDataId"));
						mData.insert(pair<int, CTerritory*>(ID, pTerritory));
					}else
					{
						g_Log.Add(CLog::Error, "[%s] Unknown Type[%s] territory_id[%d]", __FUNCTION__, sType.c_str(), ID);
					}
				}
			}
			g_Log.Add(CLog::Blue, "[%s] Loaded[%d] Territories", __FUNCTION__, mData.size());
			file.close();
		}else
			g_Log.Add(CLog::Error, "[%s] Cannot find file[%s]", __FUNCTION__, sFile.c_str());
	}catch(...)
	{
		EXCEPTION_LOG;
	}
}

void CTerritoryChecker::GetTerritories(CCreature *pCreature, TerritoryData &td)
{
	if(pCreature->ValidCreature())
	{
		map<int, CTerritory*>::iterator Iter = mData.begin();
		for(;Iter!=mData.end();Iter++)
		{
			if(Iter->second->IsInside(pCreature->pSD->Pos.x, pCreature->pSD->Pos.y, pCreature->pSD->Pos.z))
			{
				if(Iter->second->GetType() != TERRITORY_NONE)
				{
					td.territoryId[Iter->second->GetType()] = Iter->first;
				}
			}
		}
	}
}

int CTerritoryChecker::IsInTerritory(FVector& Pos, TERRITORY_TYPE TerritoryType)
{
	map<int, CTerritory*>::iterator Iter = mData.begin();
	for(;Iter!=mData.end();Iter++)
	{
		if(Iter->second->GetType() == TerritoryType)
		{
			if(Iter->second->IsInside(Pos.x, Pos.y, Pos.z))
			{
				return Iter->first;
			}
		}
	}
	return NULL;
}
int CTerritoryChecker::IsInTerritory(CCreature *pCreature, TERRITORY_TYPE TerritoryType)
{
	if(pCreature->ValidCreature())
	{
		map<int, CTerritory*>::iterator Iter = mData.begin();
		for(;Iter!=mData.end();Iter++)
		{
			if(Iter->second->GetType() == TerritoryType)
			{
				if(Iter->second->IsInside(pCreature->pSD->Pos.x, pCreature->pSD->Pos.y, pCreature->pSD->Pos.z))
				{
					return Iter->first;
				}
			}
		}
	}
	return NULL;
}

void CTerritoryChecker::ValidateRideTerritory(CCreature *pCreature)
{
	if(pCreature->ValidCreature())
	{
		int nYongmaType = pCreature->pSD->nYongmaType;
		if( nYongmaType == 2) //Check for Fly Zone
		{
			if(IsInTerritory(pCreature, FORBIDDEN_FLY_ZONE))
			{
				User *pUser = pCreature->GetUser();
				if(pUser->ValidUser())
				{
					if(pUser->pSD->nGetOffTimer == 0)
					{
						pUser->GetSocket()->SendSystemMessage(THIS_AREA_CANNOT_BE_ENTERED_WHILE_MOUNTED_ATOP_OF_A_WYVERN___YOU_WILL_BE_DISMOUNTED_FROM_YOUR_WYVERN_IF_YOU_DO_NOT_LEAVE_);
						pUser->pSD->pExData->TimerHandler.AddTimer(CCreatureTimerHandler::USER_DISMOUNT_WYVERN, 5);
						pUser->pSD->nGetOffTimer = 1;
					}
				}
			}
		}else if( nYongmaType == 1 ) //Check for Ride Zone
		{
			if(IsInTerritory(pCreature, FORBIDDEN_RIDE_ZONE))
			{
				User *pUser = pCreature->GetUser();
				if(pUser->ValidUser())
				{
					pUser->GetSocket()->SendSystemMessage(L"You're not allowed to ride strider here!");
					pUser->GetOff(true);
				}
			}
		}
	}
}

CTerritory *CTerritoryChecker::GetTerritory(int ID)
{
	map<int, CTerritory*>::iterator Iter = mData.find(ID);
	if(Iter!=mData.end())
		return Iter->second;
	return NULL;
}