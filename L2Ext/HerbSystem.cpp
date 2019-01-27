#include "StdAfx.h"
#include "HerbSystem.h"
#include "Random.h"
#include "SkillFx.h"
#include "ObjectDB.h"

CHerbSystem g_HerbSystem;

CHerbSystem::CHerbSystem() : m_Enabled(FALSE)
{
}

CHerbSystem::~CHerbSystem()
{
}

void CHerbSystem::Init()
{
	m_Enabled = FALSE;
	m_Drop.clear();
	LoadData();
	if(m_Enabled)
	{
		g_Log.Add(CLog::Blue, "[Herb System] Feature enabled. Loaded [%d] npcs.", m_Drop.size());
	}
}

void CHerbSystem::LoadData()
{
	wstringstream file(ReadFileW(L"..//Script//HerbData.txt"));
	if(file)
	{
		wstring line;
		while(getline(file, line))
		{
			if(line.size() > 4)
			{
				if( line[0] == L'/' || line[0] == L';' )
					continue;

//setting_begin	enabled=1	setting_end
//npc_begin	id=12345	low={{8600;10};{8603;10}}	mid={{8601;7.5};{8604;7.5}}	high={{8601;5};{8604;5}}	buff={{8606;5};{8607;5};{8608;5};{8609;5};{8610;5};{8611;5}}	npc_end

				if( line.find(L"setting_begin") == 0 )
				{
					m_Enabled = Parser::ParseOptionInt(line, L"enabled", 0);
					m_AffectSummon = Parser::ParseOptionInt(line, L"affect_summon", 1);
					continue;
				}
				if( line.find(L"npc_begin") == 0 )
				{
					NpcHerbDropInfo* pHd = new NpcHerbDropInfo();
					pHd->classId = Parser::ParseOptionInt(line, L"npc_id", 0);
					wstring dropList = Parser::ParseOptionString(line, L"herb_drop_list");
					if(dropList.size() > 2)
					{
						ParseDropList(pHd->dropGroup, dropList, pHd->classId);
					}

					if(pHd->classId > 0)
					{
						m_Drop[pHd->classId] = pHd;
					}
				}
			}
		}
	}else
	{
		g_Log.Add(CLog::Error, "[%S] Cannot find ..//Script//HerbData.txt !", L"Herb System");
	}
}

bool CHerbSystem::ParseDropList(vector<HerbGroupDropInfo>& dropList, wstring data, UINT npcId)
{
//	CTL;
	//additional_make_multi_list=
	//{{{{[adena];30;42;100}};70};{{{[apprentice_s_earing];1;1;30.9858};{[magic_ring];1;1;46.0093};{[necklace_of_magic];1;1;23.0049}};25.7011}}
	if(data.size() > 3)
	{
		data = data.substr(1, data.size() - 2);

		//split data into groups
		size_t pos = data.find(L";{{{");
		while(pos != wstring::npos)
		{
			data = data.replace(pos, 1, L" ");
			pos = data.find(L";{{{");
		}

		wstringstream dataStream(data);
		wstring group;
		while(dataStream >> group)
		{
			if(group.size() > 3)
			{
				//{{{[magic_ring];1;1;46.0093};{[necklace_of_magic];1;1;23.0049}};25.7011}
				HerbGroupDropInfo gdi;
				gdi.chance = 0.0;
				gdi.itemsChanceSum = 0.0;

				group = group.substr(1, group.size() - 2);
				//{{[magic_ring];1;1;46.0093};{[necklace_of_magic];1;1;23.0049}};25.7011
				INT32 itemCount = 1;
				pos = group.find(L"};{");
				while(pos != wstring::npos)
				{
					itemCount++;
					pos = group.find(L"};{", pos + 1);
				}

				group = Utils::ReplaceStringW(group, L"{", L" ", true);
				group = Utils::ReplaceStringW(group, L"}", L" ", true);
				group = Utils::ReplaceStringW(group, L";", L" ", true);
				group = Utils::ReplaceStringW(group, L"]", L" ", true);
				group = Utils::ReplaceStringW(group, L"[", L" ", true);

				wstringstream itemStream(group);
				for(int n=0;n<itemCount;n++)
				{
					wstring itemName;
					HerbDropInfo idi = { 0, 0, 0, 0 };
					itemStream >> itemName >> idi.minCount >> idi.maxCount >> idi.chance;
					if(itemName.size() > 0)
					{
						idi.itemId = g_ObjectDB.GetClassIdFromName(itemName.c_str());
						if(idi.itemId > 0)
						{
							gdi.itemsChanceSum += idi.chance;
							gdi.items.push_back(idi);
						}else
						{
							g_Log.Add(CLog::Error, "[%s] Invalid item name[%S]!", __FUNCTION__, itemName.c_str());
						}
					}
				}
				itemStream >> gdi.chance;
				if(gdi.chance > 0.0)
				{
					if(gdi.itemsChanceSum > 100.0)
					{
						g_Log.Add(CLog::Error, "[%s] Sum of items chance in group should be less or equal 100! - npc[%d]!", __FUNCTION__, npcId);
					}
			//		g_Log.Add(CLog::Blue, "[%s] Adding group size[%d] chance[%f]", __FUNCTION__, gdi.items.size(), gdi.chance);
					dropList.push_back(gdi);
				}else
				{
					g_Log.Add(CLog::Error, "[%s] Invalid group chance[%f]!", __FUNCTION__, gdi.chance);
				
					return false;
				}
			}
		}
	}

	return true;
}

bool CHerbSystem::IsHerbSkill(int skillId)
{
	if(skillId >= LOWEST_HERB_SKILL_ID && skillId <= HIGHEST_HERB_SKILL_ID)
		return true;

	return false;
}

void CHerbSystem::RestoreBuff(CCreature *pCreature, int nAbnormalType)
{
	//Restoring buff if any exists
	try
	{
		if(pCreature)
		{
			pCreature->pSD->pExData->HerbAbnormalMgr.RestoreAbnormal(pCreature, nAbnormalType);
		}
	}catch(...)
	{
		EXCEPTION_LOG;
	}
}

void CHerbSystem::StoreBuff(CCreature *pCreature, CSkillInfo *pInfo)
{
	if(pCreature->ValidUser() || pCreature->IsSummon())
	{
		pCreature->abnormalStatusLock.Lock(__FILEW__, __LINE__);
		try
		{
			AbnormalDataVector::iterator Iter = pCreature->vAbnormalStatus.begin();
			for(; Iter!= pCreature->vAbnormalStatus.end(); Iter++)
			{
				if(Iter->pSI->nSkillID != pInfo->nSkillID)
				{
					if(Iter->pSI->nAbnormalType == pInfo->nAbnormalType && !IsHerbSkill(Iter->pSI->nSkillID))
					{
						if(Iter->pSI->nAbnormalLevel <= pInfo->nAbnormalLevel)
						{
							pCreature->pSD->pExData->HerbAbnormalMgr.AddAbnormal((*Iter));
							break;
						}
					}
				}else
				{
					pCreature->pSD->pExData->HerbAbnormalMgr.UpdateExpireTime(pInfo->nAbnormalType);
					break;
				}
			}
		}catch(...)
		{
			EXCEPTION_LOG;
		}
		pCreature->abnormalStatusLock.Unlock();
	}
}

bool CHerbSystem::HandleHerb(CCreature *pCreature, CItem *pItem)
{
	if(pCreature->ValidCreature() && pItem->IsValidItem())
	{
		if(pItem->pSID->nItemID > 8599 && pItem->pSID->nItemID < 8615)
		{
			if(pItem->pSID->nCanBePickedUp == 1)
			{
				if(pItem->pII->itemSkillId > 0)
				{
					CSkillInfo *pSI = g_SkillDB.GetSkillInfo(pItem->pII->itemSkillId, pItem->pII->itemSkillLevel);
					if(pSI)	//Cast buff
					{
						pSI->ActivateSkill(pCreature, pCreature);
						CSkillFx::BroadcastAutoCastSkill(pCreature->nObjectID, pCreature->nObjectID, pSI->nSkillID, pSI->nLevel, &pCreature->pSD->Pos);
						if(m_AffectSummon)
						{
							if(User *pUser = pCreature->GetUser())
							{
								if(CSummon *pSummon = pUser->GetSummon())
								{
									pSI->ActivateSkill(pCreature, pSummon);
									CSkillFx::BroadcastAutoCastSkill(pSummon->nObjectID, pSummon->nObjectID, pSI->nSkillID, pSI->nLevel, &pSummon->pSD->Pos);
								}
							}
						}

					}
					//Delete item from world
					g_World.AddToDelete(pItem);
					g_World.DeleteItemFromWorld(pItem, true);
					return true;
				}
			}
		}
	}

	return false;
}

void CHerbSystem::DropHerb(CCreature *pCreature, CCreature *pEnemy)
{
	if(m_Enabled)
	{
		if(pCreature->IsNPC() && pEnemy->ValidCreature())
		{
			if((pCreature->pSD->nLevel + 9) < pEnemy->pSD->nLevel)
			{
				return;
			}

			int npcId = pCreature->pSD->nNpcClassID;
			map<UINT, NpcHerbDropInfo*>::iterator iter = m_Drop.find(npcId);
			if(iter!=m_Drop.end())
			{
				NpcHerbDropInfo* pHd = iter->second;
				try
				{
					if(pHd)
					{
						for(int groupIndex = 0;groupIndex < pHd->dropGroup.size(); groupIndex++)
						{
							HerbGroupDropInfo& gdi = pHd->dropGroup[groupIndex];
							if(gdi.items.size() > 0)
							{
								double groupChance = gdi.chance;
								
								double dice = g_Random.RandDouble(100.0);
								if(dice < groupChance)
								{
									double itemDice = g_Random.RandDouble(100.0);

									double itemChance = 0.0;

									//drop one of items
									for(int itemIndex = 0; itemIndex < gdi.items.size(); itemIndex++)
									{
										HerbDropInfo& idi = gdi.items[itemIndex];
										itemChance += idi.chance;
										if(false /* debug */)
										{
											g_Log.Add(CLog::Blue, "[%s] group[%d] rate[%f] dice[%f] itemChance[%f] itemDice[%f]", __FUNCTION__, groupIndex, groupChance, dice, itemChance, itemDice);
										}
										if(itemDice < itemChance)
										{
											INT32 minCount = idi.minCount;
											INT32 maxCount = idi.maxCount;
											INT32 max = maxCount - minCount + 1;
											INT32 amount = minCount;
											if(max > 1)
											{
												amount += g_Random.RandInt(max);
											}

											if(false /* debug */)
											{
											//	g_Log.Add(CLog::Blue, "Min[%d] Max[%d] ItemId[%d][%d][%d] Count[%d] isSown[%d][%d][%d]", minCount, maxCount, idi.itemId, idi.minCount, idi.maxCount, amount, pNpc->pSD->isSown, pNpc->pSD->nSeedType, pNpc->pSD->nSeederSID);
											}
											if(amount > 0)
											{
												pCreature->AddItemToInventory(idi.itemId, amount);
												break;
											}
										}
									}
								}
							}
						}
					}
				}catch(...)
				{
					EXCEPTION_LOG;
				}
			}
		}
	}
}