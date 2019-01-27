#include "stdafx.h"
#include "DropList.h"
#include "AutoLoot.h"
#include "VipSystem.h"
#include "DnN.h"
#include "ObjectDB.h"

using namespace NpcData;

CDropList g_DropList;

CDropList::CDropList() : m_enabled(FALSE), m_debugMsg(FALSE), m_raidBossItemRate(1.0)
{
}

CDropList::~CDropList()
{
//	Release();
}

void CDropList::Release()
{
	for(map<INT32, NpcDropInfo*>::iterator it = m_data.begin(); it!= m_data.end(); it++)
	{
		if(it->second)
		{
			delete it->second;
			it->second = 0;
		}
	}
	m_data.clear();
}

void CDropList::Init()
{
	m_enabled = FALSE;
	m_raidBossItemRate = 1.0;
	LoadData();
	if(m_enabled)
	{
		g_Log.Add(CLog::Blue, "[Drop List] Feature is enabled. Loaded [%d] npcs.", m_data.size());
	}
}

void CDropList::LoadData()
{
	const TCHAR* section = _T("DropList");

	m_enabled = GetPrivateProfileInt(section, _T("Enabled"), 0, g_ConfigFile);
	m_debugMsg = GetPrivateProfileInt(section, _T("DebugMsg"), 0, g_ConfigFile);
	double temp = GetPrivateProfileDouble(section, _T("RaidBossItemRate"), 100.0, g_ConfigFile);
	temp /= 100.0;
	m_raidBossItemRate = temp;

	if(m_enabled)
	{
		wstringstream npcDataStream(ReadFileW(L"..\\Script\\NpcData.txt"));
		wstring line;
		while(getline(npcDataStream, line))
		{
			ParseNpcData(line);
		}
	}
}

void CDropList::ParseNpcData(std::wstring line)
{
	guard;
	if(m_enabled)
	{
		//npc_begin	warrior	20446	[utuku_orc]	level=6	acquire_exp_rate=5.9167	acquire_sp=6	unsowing=0	clan={@orc_clan}	ignore_clan_list={}	clan_help_range=300	slot_chest=[]	slot_rhand=[hand_axe]	slot_lhand=[]	shield_defense_rate=0	shield_defense=0	skill_list={@s_race_humanoids}	npc_ai={[utuku_orc];{[MoveAroundSocial]=138};{[MoveAroundSocial1]=138};{[MoveAroundSocial2]=138}}	category={}	race=humanoid	sex=male	undying=0	can_be_attacked=1	corpse_time=7	no_sleep_mode=0	agro_range=1000	ground_high={110;0;0}	ground_low={45;0;0}	exp=6039	org_hp=93.54158	org_hp_regen=2	org_mp=77.2	org_mp_regen=0.9	collision_radius={10;10}	collision_height={21;21}	str=40	int=21	dex=30	wit=20	con=43	men=20	base_attack_type=blunt	base_attack_range=40	base_damage_range={0;0;80;120}	base_rand_dam=50	base_physical_attack=13.52471	base_critical=1	physical_hit_modify=9	base_attack_speed=253	base_reuse_delay=0	base_magic_attack=9.23562	base_defend=53.53373	base_magic_defend=35.6433609604933	physical_avoid_modify=0	soulshot_count=0	spiritshot_count=0	hit_time_factor=0.48	item_make_list={}	corpse_make_list={{[iron_ore];1;1;7.7741};{[coal];1;1;7.7741};{[bow_shaft];1;1;0.5553}}	additional_make_list={}	additional_make_multi_list={{{{[adena];41;58;100}};70};{{{[leather_shirt];1;1;38.4772};{[leather_pants];1;1;61.5228}};0.4912};{{{[rp_brandish];1;1;5.80429};{[stem];1;1;62.7971};{[iron_ore];1;1;31.3986}};4.8757}}	hp_increase=0	mp_increase=0	safe_height=100	npc_end
		if(line.find(L"npc_begin") == 0)
		{
			//normalize line - anti stupid/lazy ppl
			line = Utils::ReplaceStringW(line, L" ", L"\t", true);
			line = Utils::ReplaceStringW(line, L"\t\t", L"\t", true);
			line = Utils::ReplaceStringW(line, L"\t=", L"=", true);
			line = Utils::ReplaceStringW(line, L"=\t", L"=", true);
			
			vector<wstring> column;
			wstringstream lineStream(line);
			wstring col;
			while(getline(lineStream, col, L'\t'))
			{
				column.push_back(col);
			}
			if(column.size() > 5)
			{
				INT32 npcId = 1000000 + _wtoi(column[2].c_str());
				if(npcId > 1000000)
				{
				//	g_Log.Add(CLog::Blue, "[%s] Npc[%d] [%S]", __FUNCTION__, npcId, column[2].c_str());
					NpcDropInfo *pInfo = new NpcDropInfo;
					if(pInfo)
					{
						pInfo->classId = npcId;
						for(int n=0;n<column.size();n++)
						{
							wstring parseLine = column[n];
							if(parseLine.find(L"item_make_list=") == 0)
							{
								wstring data = parseLine.substr(15, (parseLine.size() - 15) );
								if(ParseItemMakeList(pInfo->itemMakeList, data))
								{
									continue;
								}else
								{
									g_Log.Add(CLog::Error, "[%d] Error while parsing item_make_list for npc[%d]!", __FUNCTION__, npcId); 
								}
							}
							else if(parseLine.find(L"additional_make_list=") == 0)
							{
								wstring data = parseLine.substr(21, (parseLine.size() - 21) );
								if(ParseAdditionalMakeList(pInfo->additionalMakeList, data))
								{
									continue;
								}else
								{
									g_Log.Add(CLog::Error, "[%d] Error while parsing additional_make_list for npc[%d]!", __FUNCTION__, npcId); 
								}
							}
							else if(parseLine.find(L"additional_make_multi_list=") == 0)
							{
								wstring data = parseLine.substr(27, (parseLine.size() - 27) );
								if(ParseAdditionalMakeMultiList(pInfo->additionalMakeMultiList, data))
								{
									continue;
								}else
								{
									g_Log.Add(CLog::Error, "[%d] Error while parsing additional_make_multi_list for npc[%d]!", __FUNCTION__, npcId); 
								}
							}
						}
						m_data.insert(pair<INT32, NpcDropInfo*>(npcId, pInfo));
					}else
					{
						AddToCallStack(L"Cannot allocate memory for NpcDropInfo!");
						LPDWORD lpCrash = 0;
						(*lpCrash) = 0x666;
					}
				}
			}
		}
	}
	unguard;
}

bool CDropList::ParseItemMakeList(std::vector<ItemDropInfo> &itemMakeList, std::wstring data)
{
	guard;
	if(data.size() > 3)
	{
		//additional_make_list=
		//{{[breaking_arrow];50;100;100}}
		data = Utils::ReplaceStringW(data, L"{", L" ", true);
		data = Utils::ReplaceStringW(data, L"}", L" ", true);
		data = Utils::ReplaceStringW(data, L";", L" ", true);
		data = Utils::ReplaceStringW(data, L"]", L" ", true);
		data = Utils::ReplaceStringW(data, L"[", L" ", true);
		//  [breaking_arrow] 50 100 100  
		wstringstream dataStream(data);
		ItemDropInfo idi = { 0, 0, 0, 0 };
		wstring itemName;
		while(dataStream >> itemName >> idi.minCount >> idi.maxCount >> idi.chance)
		{
			if(itemName.size() > 0)
			{
				idi.itemId = g_ObjectDB.GetClassIdFromName(itemName.c_str());
				if(idi.itemId)
				{
			//		g_Log.Add(CLog::Blue, "[%s] adding[%d][%d][%d][%f]", __FUNCTION__, idi.itemId, idi.minCount, idi.maxCount, idi.chance);
					itemMakeList.push_back(idi);
				}
			}
		}
	}
	unguard;
	return true;
}

bool CDropList::ParseAdditionalMakeList(vector<ItemDropInfo>& additionalMakeList, wstring data)
{
	guard;
	//additional_make_list=
	//{{[breaking_arrow];50;100;100}}
	if(data.size() > 3)
	{
		data = Utils::ReplaceStringW(data, L"{", L" ", true);
		data = Utils::ReplaceStringW(data, L"}", L" ", true);
		data = Utils::ReplaceStringW(data, L";", L" ", true);
		data = Utils::ReplaceStringW(data, L"]", L" ", true);
		data = Utils::ReplaceStringW(data, L"[", L" ", true);
		//  [breaking_arrow] 50 100 100  
		wstringstream dataStream(data);
		ItemDropInfo idi = { 0, 0, 0, 0 };
		wstring itemName;
		while(dataStream >> itemName >> idi.minCount >> idi.maxCount >> idi.chance)
		{
			if(itemName.size() > 0)
			{
				idi.itemId = g_ObjectDB.GetClassIdFromName(itemName.c_str());
				if(idi.itemId)
				{
			//		g_Log.Add(CLog::Blue, "[%s] adding[%d][%d][%d][%f]", __FUNCTION__, idi.itemId, idi.minCount, idi.maxCount, idi.chance);
					additionalMakeList.push_back(idi);
				}
			}
		}
	}

	unguard;
	return true;
}

bool CDropList::ParseAdditionalMakeMultiList(vector<GroupDropInfo>& additionalMakeMultiList, wstring data)
{
	guard;
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
				GroupDropInfo gdi;
				gdi.chance = 0.0;
				gdi.itemsChanceSum = 0.0;
				gdi.adenaGroup = FALSE;

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
					ItemDropInfo idi = { 0, 0, 0, 0 };
					itemStream >> itemName >> idi.minCount >> idi.maxCount >> idi.chance;
					if(itemName.size() > 0)
					{
						idi.itemId = g_ObjectDB.GetClassIdFromName(itemName.c_str());
						if(idi.itemId > 0)
						{
							if(idi.itemId == 57 || idi.itemId == 6360 || idi.itemId == 6361 || idi.itemId == 6362)
							{
								gdi.adenaGroup = TRUE;
							}
				//			g_Log.Add(CLog::Blue, "[%s] adding[%d][%d][%d][%f]", __FUNCTION__, idi.itemId, idi.minCount, idi.maxCount, idi.chance);
							gdi.itemsChanceSum += idi.chance;
							gdi.items.push_back(idi);
						}
					}
				}
				itemStream >> gdi.chance;
				if(gdi.chance > 0.0)
				{
			//		g_Log.Add(CLog::Blue, "[%s] Adding group size[%d] chance[%f]", __FUNCTION__, gdi.items.size(), gdi.chance);
					additionalMakeMultiList.push_back(gdi);
				}else
				{
					g_Log.Add(CLog::Error, "[%s] Invalid group chance[%f]!", __FUNCTION__, gdi.chance);
					unguard;
					return false;
				}
			}
		}
	}

	unguard;
	return true;
}

void CDropList::OnDie(CNPC *pNpc, CCreature *pKiller)
{
	guard;
	if(m_enabled && pNpc->ValidCreature())
	{
		map<INT32, NpcDropInfo*>::iterator it = m_data.find(pNpc->pSD->nNpcClassID);
		if(it!=m_data.end())
		{
			NpcDropInfo* pInfo = it->second;
			if(pInfo && pNpc->pContributeData)
			{
				double levelPenalty = 1.0;

				INT32 mostContributeLevel = pNpc->pContributeData->contributorLevel;
				INT32 npcLevel = pNpc->pSD->nLevel;
				INT32 diffLevel = 0;
				if(mostContributeLevel > 0)
				{					
					diffLevel = mostContributeLevel - npcLevel;
				}else if(pKiller->ValidCreature())
				{
					diffLevel = pKiller->pSD->nLevel - npcLevel;
				}
				if(diffLevel > 0)
				{
					if(diffLevel > 9)
					{
						levelPenalty = 0.125 / (double)diffLevel;
					}else if(diffLevel == 9)
					{
						levelPenalty = 0.125;
					}else if(diffLevel > 5)
					{
						levelPenalty = 0.5;
					}
				}

			//	g_Log.Add(CLog::Blue, "[%s] levelPenalty[%f] diffLevel[%d]", __FUNCTION__, levelPenalty, diffLevel);


				DOUBLE* lpAdenaRate = (DOUBLE*)0xBEF260;
				DOUBLE* lpDropRate = (DOUBLE*)0xBEF268;

				double itemRate = (*lpDropRate);
				double adenaRate = (*lpAdenaRate);

				itemRate *= levelPenalty;
				adenaRate *= levelPenalty;

				if(g_DnNSystem.IsEnabled())
				{
					itemRate *= g_DnNSystem.GetRate(DnN::RateDrop);
					adenaRate *= g_DnNSystem.GetRate(DnN::RateAdena);
				}

				User *pUser = User::GetUserBySID(&pNpc->pContributeData->contributorId);
				if(pUser->ValidUser())
				{
					if(!pNpc->IsInRange(pUser, 2000))
					{
						pUser = 0;
					}
				}

				if(!pUser->ValidUser())
				{
					pUser = pKiller->GetUser();
					if(pKiller->IsSummon())
					{
						pUser = pKiller->CastSummon()->GetMaster();
					}
					if(pUser->ValidUser())
					{
						if(!pNpc->IsInRange(pUser, 2000))
						{
							pUser = 0;
						}
					}
				}

				double itemRateOnlyChance = 1.0;

				double itemRateBonus = 1.0;
				double adenaBonus = 1.0;
				double raidBossBonus = 1.0;
				double raidBossOnlyChanceBonus = 1.0;
				double epicBossBonus = 1.0;
				double onlyChanceBonus = 1.0;

				
				if(pUser->ValidUser())
				{
					pUser->GetDropBonuses(itemRateBonus, adenaBonus, raidBossBonus, raidBossOnlyChanceBonus, epicBossBonus, onlyChanceBonus);

					if(m_debugMsg)
					{
						g_Log.Add(CLog::Blue, "[%s] User[%S] Npc[%S][%d] itemRate[%f] adenaRate[%f] itemBonus[%f] adenaBonus[%f] rbBonus[%f] rbOnlyChanceBonus[%f], epicBonus[%f] onlyChanceBonus[%f]", __FUNCTION__,
							pUser->pSD->wszName, pNpc->pSD->wszName, pNpc->pSD->nNpcClassID, itemRate, adenaRate, itemRateBonus, adenaBonus, raidBossBonus, raidBossOnlyChanceBonus,
							epicBossBonus, onlyChanceBonus);
					}

					if(!pNpc->IsZZoldagu() && !pNpc->IsBoss())
					{
						if(pUser->pSD->vipLevel > 0)
						{
							/* VipSystem */
							itemRate *= g_VIPSystem.GetDropRate(pUser);
							adenaRate *= g_VIPSystem.GetAdenaRate(pUser);
						}

						/* Skill Effects */
						itemRate *= itemRateBonus;
						itemRateOnlyChance *= onlyChanceBonus;
						adenaRate *= adenaBonus;
					}else
					{
						INT32 npcClassId = pNpc->pSD->nNpcClassID;
						itemRate = 1.0;
						itemRate *= levelPenalty;
						itemRateOnlyChance *= m_raidBossItemRate;
						//raid boss rates
						if(npcClassId == 1029001 || npcClassId == 1029006 || npcClassId == 1029014 || npcClassId == 1029019 || npcClassId == 1029020 || npcClassId == 1029022 || npcClassId == 1029028 || npcClassId == 1029047 || npcClassId == 1029066 || npcClassId == 1029067 || npcClassId == 1029068)
						{
							itemRateOnlyChance *= epicBossBonus;
							//epic rb
						}else
						{
							//normal rb
							itemRateOnlyChance *= raidBossOnlyChanceBonus;
							itemRate *= raidBossBonus;
						}
					}
				}
				map<int, int> droppedItems;
				map<int, int>::iterator droppedIter;

				//Item Make List
				if(pInfo->itemMakeList.size() > 0)
				{
					for(int n=0;n<pInfo->itemMakeList.size();n++)
					{
						ItemDropInfo& idi = pInfo->itemMakeList[n];
						double itemChance = idi.chance * itemRate;
						if(itemChance < 100.0)
						{
							itemChance *= itemRateOnlyChance;
							if(itemChance > 100.0)
							{
								itemChance = 100.0;
							}
						}
						INT32 minCount = idi.minCount;
						INT32 maxCount = idi.maxCount;
						if(idi.itemId == 57 || idi.itemId == 6360 || idi.itemId == 6361 || idi.itemId == 6362)
						{
							itemChance = idi.chance * levelPenalty;
							double mc = static_cast<double>(minCount) * adenaRate;
							minCount = static_cast<int>(mc);
							mc = static_cast<double>(maxCount) * adenaRate;
							maxCount = static_cast<int>(mc);
						}

						for(;itemChance > 0.0; itemChance -= 100.0)
						{
							double dice = g_Random.RandDouble(100.0);
							if(dice < itemChance)
							{
								INT32 max = maxCount - minCount + 1;
								INT32 amount = minCount;
								if(max > 1)
								{
									amount += g_Random.RandInt(max);
								}

								if(pUser->ValidUser())
								{
									double fCount = static_cast<INT32>(amount);
									fCount *= pUser->pED->itemBonus.Get(idi.itemId);
									amount = static_cast<INT32>(fCount);
								}

								if(!pNpc->pSD->nSeederSID || idi.itemId == 57 || idi.itemId == 6360 || idi.itemId == 6361 || idi.itemId == 6362)
								{
									if(amount > 0)
									{
										droppedIter = droppedItems.find(idi.itemId);
										if(droppedIter != droppedItems.end())
										{
											droppedIter->second += amount;
										}else
										{
											droppedItems.insert(pair<int, int>(idi.itemId, amount));
										}
									}
								}
							}
						}
					}
				}
				//Additional Make List
				if(pInfo->additionalMakeList.size() > 0)
				{
					for(int n=0;n<pInfo->additionalMakeList.size();n++)
					{
						ItemDropInfo& idi = pInfo->additionalMakeList[n];
						double itemChance = idi.chance * itemRate;
						INT32 minCount = idi.minCount;
						INT32 maxCount = idi.maxCount;

						if(itemChance < 100.0)
						{
							itemChance *= itemRateOnlyChance;
							if(itemChance > 100.0)
							{
								itemChance = 100.0;
							}
						}

						if(idi.itemId == 57 || idi.itemId == 6360 || idi.itemId == 6361 || idi.itemId == 6362)
						{
							itemChance = idi.chance * levelPenalty;
							double mc = static_cast<double>(minCount) * adenaRate;
							minCount = static_cast<int>(mc);
							mc = static_cast<double>(maxCount) * adenaRate;
							maxCount = static_cast<int>(mc);
						}

						for(;itemChance > 0.0; itemChance -= 100.0)
						{
							double dice = g_Random.RandDouble(100.0);
							if(dice < itemChance)
							{
								INT32 max = maxCount - minCount + 1;
								INT32 amount = minCount;
								if(max > 1)
								{
									amount += g_Random.RandInt(max);
								}
								if(pUser->ValidUser())
								{
									double fCount = static_cast<INT32>(amount);
									fCount *= pUser->pED->itemBonus.Get(idi.itemId);
									amount = static_cast<INT32>(fCount);
								}
								if(!pNpc->pSD->nSeederSID || idi.itemId == 57 || idi.itemId == 6360 || idi.itemId == 6361 || idi.itemId == 6362)
								{
									if(amount > 0)
									{
										droppedIter = droppedItems.find(idi.itemId);
										if(droppedIter != droppedItems.end())
										{
											droppedIter->second += amount;
										}else
										{
											droppedItems.insert(pair<int, int>(idi.itemId, amount));
										}
									}
								}
							}
						}
					}
				}
				//Additional Make Multi List
				if(pInfo->additionalMakeMultiList.size() > 0)
				{
					for(int groupIndex = 0;groupIndex < pInfo->additionalMakeMultiList.size(); groupIndex++)
					{
						GroupDropInfo& gdi = pInfo->additionalMakeMultiList[groupIndex];
						if(gdi.items.size() > 0)
						{
							double groupChance = gdi.chance * itemRate;
							if(gdi.adenaGroup)
							{
								if(diffLevel < 10)
								{
									groupChance = gdi.chance;
								}
							}else if(groupChance < 100.0)
							{
								groupChance *= itemRateOnlyChance;
								if(groupChance > 100.0)
								{
									groupChance = 100.0;
								}
							}
							if(m_debugMsg)
							{
								g_Log.Add(CLog::Blue, "[%s] group[%d] rate[%f]", __FUNCTION__, groupIndex, groupChance);
							}
							for(;groupChance > 0; groupChance -= 100.0)
							{
								double dice = g_Random.RandDouble(100.0);
						//		g_Log.Add(CLog::Blue, "[%s] group[%d] rate[%f] dice[%f]", __FUNCTION__, groupIndex, groupChance, dice);
								if(dice < groupChance)
								{
									double itemDice = 0.0;
									if(gdi.itemsChanceSum > 100.0)
									{
										itemDice = g_Random.RandDouble(gdi.itemsChanceSum);
									}else
									{
										itemDice = g_Random.RandDouble(100.0);
									}

									double itemChance = 0.0;

									//drop one of items
									for(int itemIndex = 0; itemIndex < gdi.items.size(); itemIndex++)
									{
										ItemDropInfo& idi = gdi.items[itemIndex];
										itemChance += idi.chance;
										if(m_debugMsg)
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
											if(pUser->ValidUser())
											{
												double fCount = static_cast<INT32>(amount);
												fCount *= pUser->pED->itemBonus.Get(idi.itemId);
												amount = static_cast<INT32>(fCount);
											}
											if(idi.itemId == 57 || idi.itemId == 6360 || idi.itemId == 6361 || idi.itemId == 6362)
											{
												itemChance = idi.chance * levelPenalty;
												double newCount = static_cast<double>(amount);
												newCount *= adenaRate;
										//		g_Log.Add(CLog::Blue, "[%s] adena [%d] vs [%f] adenaRate[%f] chance[%f] levelPenalty[%f]", __FUNCTION__, amount, newCount, adenaRate, itemChance, levelPenalty);
												amount = static_cast<INT32>(newCount);
											}
											if(m_debugMsg)
											{
												g_Log.Add(CLog::Blue, "Min[%d] Max[%d] ItemId[%d][%d][%d] Count[%d] isSown[%d][%d][%d]", minCount, maxCount, idi.itemId, idi.minCount, idi.maxCount, amount, pNpc->pSD->isSown, pNpc->pSD->nSeedType, pNpc->pSD->nSeederSID);
											}
											if(!pNpc->pSD->nSeederSID || idi.itemId == 57 || idi.itemId == 6360 || idi.itemId == 6361 || idi.itemId == 6362)
											{
												if(amount > 0)
												{
													droppedIter = droppedItems.find(idi.itemId);
													if(droppedIter != droppedItems.end())
													{
														droppedIter->second += amount;
													}else
													{
														droppedItems.insert(pair<int, int>(idi.itemId, amount));
													}
												}
											}
											break;
										}
									}
								}
							}
						}
					}
				}
				for(droppedIter = droppedItems.begin();droppedIter != droppedItems.end();droppedIter++)
				{
					if( droppedIter->second >=0 )
					{
						if(g_AutoLoot.IsEnabled())
						{
							if(!g_AutoLoot.Handle(pNpc, pKiller, droppedIter->first, droppedIter->second))
							{
								pNpc->AddItemToInventory(droppedIter->first, droppedIter->second);
							}
						}else
						{
							pNpc->AddItemToInventory(droppedIter->first, droppedIter->second);
						}
					}else
					{
						g_Log.Add(CLog::Error, "[%s] Invalid amount - item[%d] amount[%d] npc[%S] user[%S]", __FUNCTION__, droppedIter->first, droppedIter->second, pNpc->pSD->wszName, pKiller->pSD->wszName);
					}
				}
			}
		}
	}
	unguard;
}