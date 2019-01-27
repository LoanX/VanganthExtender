#include "StdAfx.h"
#include "MiningSystem.h"
#include "TerritoryData.h"

CMiningSystem g_MiningSystem;

CMiningSystem::CMiningSystem()
{
}

CMiningSystem::~CMiningSystem()
{
	if(m_data.size() > 0)
	{
		ClearData();
	}
}

void CMiningSystem::ClearData()
{
	try
	{
		for(map<UINT, MiningAreaData*>::iterator Iter = m_data.begin();Iter!=m_data.end();Iter++)
		{
			MiningAreaData* pMAD = Iter->second;
			if(pMAD)
				delete pMAD;
		}
		m_data.clear();
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception", __FUNCTION__);
	}
}

void CMiningSystem::Init()
{
	LoadData();
}

void CMiningSystem::LoadData()
{
	wstring wFile = ReadFileW(L"..\\Script\\MiningData.txt");
	ClearData();
	if(wFile.size() > 0)
	{
		wstringstream file;
		file << wFile;

		wstring line;
		bool item_part = false;
		int item_count = 0;
		MiningAreaData* pData = NULL;
		INT32 counter = 0;

		while(getline(file, line))
		{
			counter++;
			if(line.find(L"//") == 0)
				continue;
			
			if(item_part)
			{
				if(line.find(L"dig_item_begin") == 0)
				{
					DigItemData did;
					//dig_item_begin item_id=1870	min_count=1	max_count=3	chance=25	dig_item_end
					did.item_id = Parser::ParseOptionInt(line, L"item_id");
					did.min_count = Parser::ParseOptionInt(line, L"min_count");
					did.max_count = Parser::ParseOptionInt(line, L"max_count");
					did.chance = Parser::ParseOptionDouble(line, L"chance");
					pData->items.push_back(did);
				}
			}

			if(line.find(L"mine_data_begin") == 0)
			{
				pData = new MiningAreaData;
				item_part = true;
				//mine_data_begin	id=1	required_weapon_id=0
				pData->id = Parser::ParseOptionInt(line, L"id");
				pData->requiredWeaponId = Parser::ParseOptionInt(line, L"required_weapon_id");
			}else if(line.find(L"mine_data_end") == 0)
			{
				item_part = false;
				if(pData)
				{
					if(m_data.find(pData->id) != m_data.end())
					{
						g_Log.Add(CLog::Error, "[%s] Duplicated id[%d]!", __FUNCTION__, pData->id);
					}else
					{
						m_data.insert(pair<UINT, MiningAreaData*>(pData->id, pData));
					}
					pData = NULL;
				}
			}
		}
		g_Log.Add(CLog::Blue, "[%s] Loaded [%d] area(s).", __FUNCTION__, m_data.size());
	}else
		g_Log.Add(CLog::Error, "[%s] Cannot find ..\\Script\\MiningData.txt !", __FUNCTION__);
}

bool CMiningSystem::CanDig(User* pUser)
{
	if(pUser->ValidUser())
	{
		if(pUser->pED->miningAreaId > 0)
		{
			return true;
		}
	}
	return false;
}

bool CMiningSystem::Dig(User *pUser, double baseChance)
{
	if(pUser->ValidUser() && pUser->pED->miningAreaId)
	{
		CMiningTerritory* pTerritory = g_TerritoryChecker.GetTerritory(pUser->pED->miningAreaId)->SafeCastMining();
		UINT miningId = 0;
		if(pTerritory)
		{
			miningId = pTerritory->GetMiningId();
		}
		if(miningId > 0)
		{
			map<UINT, MiningAreaData*>::iterator iter = m_data.find(miningId);
			if(iter != m_data.end())
			{
				MiningAreaData *pData = iter->second;
				if(pData)
				{
					for(int itemIdx = 0; itemIdx < pData->items.size(); itemIdx++)
					{
						DigItemData& did = pData->items[itemIdx];
						int itemId = did.item_id;
						double dice = g_Random.RandDouble(100.0);
						dice /= baseChance;
						double chance = did.chance * pUser->pSD->pExData->SkillMod.fDiggerMultipler;
					//	g_Log.Add(CLog::Blue, "[%s] dice[%f] chance[%f] itemId[%d]", __FUNCTION__, dice, chance, itemId);
						if(dice < chance)
						{
							int itemCount = did.min_count;
							if(did.max_count > did.min_count)
							{
								itemCount += g_Random.RandInt(did.max_count - did.min_count + 1);
							}
							if(itemCount > 0)
							{
								if(pUser->inventory.CheckAddable(itemId, itemCount))
								{
									pUser->AddItemToInventory(itemId, itemCount);
								}else
								{
									pUser->SendSystemMessage(L"Your inventory seems to be full!");
								}
							}
						}
					}
				}
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Invalid area id[%d] User[%S]", __FUNCTION__, miningId, pUser->pSD->wszName);
				pUser->SendSystemMessage(L"You cannot dig in current area.");
			}
		}else
		{
			g_Log.Add(CLog::Error, "[%s] MiningId NULL! User[%S]", __FUNCTION__, pUser->pSD->wszName);
			pUser->SendSystemMessage(L"You cannot dig in current area.");
		}
	}
	return false;
}