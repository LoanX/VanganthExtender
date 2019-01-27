#include "StdAfx.h"
#include "DB.h"
#include "PlayerAction.h"

CUserDB g_UserDB;

void CUserDB::Initialize()
{
	g_Log.Add(CLog::Blue, "[%s] Initializing Data", __FUNCTION__); 
	INT32 nTmpExpTable[] = 
	{		 0,
			 0,
			68,
		   363,
		  1168,
		  2884,
		  6038,
		 11287,
		 19423,
		 31378,
		 48229,  //level 10
		 71201,
		101676,
		141192,
		191452,
		254327,
		331864,
		426284,
		539995,
		675590,
		835854,  //level 20
		1023775,
		1242536,
		1495531,
		1786365,
		2118860,
		2497059,
		2925229,
		3407873,
		3949727,
		4555766,  //level 30
		5231213,
		5981539,
		6812472,
		7729999,
		8740372,
		9850111,
		11066012,
		12395149,
		13844879,
		15422851,  //level 40
		17137002,
		18995573,
		21007103,
		23180442,
		25524751,
		28049509,
		30764519,
		33679907,
		36806133,
		40153995, //level 50
		45524865,
		51262204,
		57383682,
		63907585,
		70852742,
		80700339,
		91162131,
		102265326,
		114038008,
		126509030,  //level 60
		146307211,
		167243291,
		189363788,
		212716741,
		237351413,
		271973532,
		308441375,
		346825235,
		387197529,
		429632402,  //level 70
		474205751,
		532692055,
		606319094,
		696376867,
		804219972,
		931275828,
		1151275834,
		1511275834, //78 - 1511275834 
		1520463334, // 2099275834 - Last Level / 64 = (1511275834 + 9187500 = 1520463334)  [Level 79]
		1553287149,	// 4200000000 - Last Level / 64 = (1520463334 + 32823815 = 1553287149) [Level 80]
		1586099570  // 6299994999 - Last Level / 64 = (1553287149 + 32812421 = 1586099570) [Level 81]
	};
	memcpy(&nExpTable[0], &nTmpExpTable[0], sizeof(nExpTable));
	mClass.insert(pair<wstring, int>(L"fighter", 0));
	mClass.insert(pair<wstring, int>(L"warrior", 1));
	mClass.insert(pair<wstring, int>(L"gladiator", 2));
	mClass.insert(pair<wstring, int>(L"warlord", 3));
	mClass.insert(pair<wstring, int>(L"knight", 4));
	mClass.insert(pair<wstring, int>(L"paladin", 5));
	mClass.insert(pair<wstring, int>(L"dark_avenger", 6));
	mClass.insert(pair<wstring, int>(L"rogue", 7));
	mClass.insert(pair<wstring, int>(L"treasure_hunter", 8));
	mClass.insert(pair<wstring, int>(L"hawkeye", 9));
	mClass.insert(pair<wstring, int>(L"mage", 10));
	mClass.insert(pair<wstring, int>(L"wizard", 11));
	mClass.insert(pair<wstring, int>(L"sorcerer", 12));
	mClass.insert(pair<wstring, int>(L"necromancer", 13));
	mClass.insert(pair<wstring, int>(L"warlock", 14));
	mClass.insert(pair<wstring, int>(L"cleric", 15));
	mClass.insert(pair<wstring, int>(L"bishop", 16));
	mClass.insert(pair<wstring, int>(L"prophet", 17));
	mClass.insert(pair<wstring, int>(L"elven_fighter", 18));
	mClass.insert(pair<wstring, int>(L"elven_knight", 19));
	mClass.insert(pair<wstring, int>(L"temple_knight", 20));
	mClass.insert(pair<wstring, int>(L"swordsinger", 21));
	mClass.insert(pair<wstring, int>(L"elven_scout", 22));
	mClass.insert(pair<wstring, int>(L"plains_walker", 23));
	mClass.insert(pair<wstring, int>(L"plain_walker", 23));
	mClass.insert(pair<wstring, int>(L"silver_ranger", 24));
	mClass.insert(pair<wstring, int>(L"elven_mage", 25));
	mClass.insert(pair<wstring, int>(L"elven_wizard", 26));
	mClass.insert(pair<wstring, int>(L"spellsinger", 27));
	mClass.insert(pair<wstring, int>(L"elemental_summoner", 28));
	mClass.insert(pair<wstring, int>(L"oracle", 29));
	mClass.insert(pair<wstring, int>(L"elder", 30));
	mClass.insert(pair<wstring, int>(L"dark_fighter", 31));
	mClass.insert(pair<wstring, int>(L"palus_knight", 32));
	mClass.insert(pair<wstring, int>(L"shillien_knight", 33));
	mClass.insert(pair<wstring, int>(L"bladedancer", 34));
	mClass.insert(pair<wstring, int>(L"assassin", 35));
	mClass.insert(pair<wstring, int>(L"assasin", 35));
	mClass.insert(pair<wstring, int>(L"abyss_walker", 36));
	mClass.insert(pair<wstring, int>(L"phantom_ranger", 37));
	mClass.insert(pair<wstring, int>(L"dark_mage", 38));
	mClass.insert(pair<wstring, int>(L"dark_wizard", 39));
	mClass.insert(pair<wstring, int>(L"spellhowler", 40));
	mClass.insert(pair<wstring, int>(L"phantom_summoner", 41));
	mClass.insert(pair<wstring, int>(L"shillien_oracle", 42));
	mClass.insert(pair<wstring, int>(L"shillien_elder", 43));
	mClass.insert(pair<wstring, int>(L"orc_fighter", 44));
	mClass.insert(pair<wstring, int>(L"orc_raider", 45));
	mClass.insert(pair<wstring, int>(L"destroyer", 46));
	mClass.insert(pair<wstring, int>(L"orc_monk", 47));
	mClass.insert(pair<wstring, int>(L"tyrant", 48));
	mClass.insert(pair<wstring, int>(L"orc_mage", 49));
	mClass.insert(pair<wstring, int>(L"orc_shaman", 50));
	mClass.insert(pair<wstring, int>(L"overlord", 51));
	mClass.insert(pair<wstring, int>(L"warcryer", 52));
	mClass.insert(pair<wstring, int>(L"dwarven_fighter", 53));
	mClass.insert(pair<wstring, int>(L"scavenger", 54));
	mClass.insert(pair<wstring, int>(L"bounty_hunter", 55));
	mClass.insert(pair<wstring, int>(L"artisan", 56));
	mClass.insert(pair<wstring, int>(L"warsmith", 57));
	mClass.insert(pair<wstring, int>(L"duelist", 88));
	mClass.insert(pair<wstring, int>(L"dreadnought", 89));
	mClass.insert(pair<wstring, int>(L"phoenix_knight", 90));
	mClass.insert(pair<wstring, int>(L"hell_knight", 91));
	mClass.insert(pair<wstring, int>(L"sagittarius", 92));
	mClass.insert(pair<wstring, int>(L"adventurer", 93));
	mClass.insert(pair<wstring, int>(L"archmage", 94));
	mClass.insert(pair<wstring, int>(L"soultaker", 95));
	mClass.insert(pair<wstring, int>(L"arcana_lord", 96));
	mClass.insert(pair<wstring, int>(L"cardinal", 97));
	mClass.insert(pair<wstring, int>(L"hierophant", 98));
	mClass.insert(pair<wstring, int>(L"evas_templar", 99));
	mClass.insert(pair<wstring, int>(L"sword_muse", 100));
	mClass.insert(pair<wstring, int>(L"wind_rider", 101));
	mClass.insert(pair<wstring, int>(L"moonlight_sentinel", 102));
	mClass.insert(pair<wstring, int>(L"mystic_muse", 103));
	mClass.insert(pair<wstring, int>(L"elemental_master", 104));
	mClass.insert(pair<wstring, int>(L"evas_saint", 105));
	mClass.insert(pair<wstring, int>(L"shillien_templar", 106));
	mClass.insert(pair<wstring, int>(L"spectral_dancer", 107));
	mClass.insert(pair<wstring, int>(L"ghost_hunter", 108));
	mClass.insert(pair<wstring, int>(L"ghost_sentinel", 109));
	mClass.insert(pair<wstring, int>(L"storm_screamer", 110));
	mClass.insert(pair<wstring, int>(L"spectral_master", 111));
	mClass.insert(pair<wstring, int>(L"shillien_saint", 112));
	mClass.insert(pair<wstring, int>(L"titan", 113));
	mClass.insert(pair<wstring, int>(L"grand_khavatari", 114));
	mClass.insert(pair<wstring, int>(L"dominator", 115));
	mClass.insert(pair<wstring, int>(L"doomcryer", 116));
	mClass.insert(pair<wstring, int>(L"fortune_seeker", 117));
	mClass.insert(pair<wstring, int>(L"maestro", 118));
}

int CUserDB::GetClassID(wstring wName)
{
	map<wstring, int>::iterator Iter = mClass.find(wName);
	if(Iter!=mClass.end())
		return Iter->second;
	else
		return NULL;
}

void CUserDB::AddUser(User *pUser)
{
	if(pUser->ValidUser())
	{
		Lock();
		try
		{
			m_byDatabaseId.insert(pair<UINT, User*>(pUser->nDBID, pUser));
			m_byServerId.insert(pair<UINT, User*>(pUser->nObjectID, pUser));
			wstring name = pUser->pSD->wszName;
			transform(name.begin(), name.end(), name.begin(), towlower);
			m_byName.insert(pair<wstring, User*>(name, pUser));
		}catch(...)
		{
			EXCEPTION_LOG;
		}
		Unlock();
	}
}

void CUserDB::DelUser(User *pUser)
{
	if(pUser->ValidUser())
	{
		Lock();
		try
		{
			map<UINT, User*>::iterator it = m_byDatabaseId.find(pUser->nDBID);
			if(it != m_byDatabaseId.end())
			{
				m_byDatabaseId.erase(it);
			}
			it = m_byServerId.find(pUser->nObjectID);
			if(it != m_byServerId.end())
			{
				m_byServerId.erase(it);
			}
			wstring name = pUser->pSD->wszName;
			transform(name.begin(), name.end(), name.begin(), towlower);
			map<wstring, User*>::iterator itName = m_byName.find(name);
			if(itName != m_byName.end())
			{
				m_byName.erase(itName);
			}
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception Detected!", __FUNCTION__);
		}
		Unlock();
	}
}

void CUserDB::DelUser(UINT serverId)
{
	Lock();
	try
	{
		User *pUser = 0;
		map<UINT, User*>::iterator it = m_byServerId.find(serverId);
		if(it != m_byServerId.end())
		{
			pUser = it->second->GetUser();
			m_byServerId.erase(it);
		}
		if(pUser)
		{
			it = m_byDatabaseId.find(pUser->nDBID);
			if(it != m_byDatabaseId.end())
			{
				m_byDatabaseId.erase(it);
			}
			
			wstring name = pUser->pSD->wszName;
			transform(name.begin(), name.end(), name.begin(), towlower);
			map<wstring, User*>::iterator itName = m_byName.find(name);
			if(itName != m_byName.end())
			{
				m_byName.erase(itName);
			}
		}
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception Detected!", __FUNCTION__);
	}
	Unlock();
}

User* CUserDB::GetUserByName(wstring wName)
{
	transform(wName.begin(), wName.end(), wName.begin(), towlower);
	User *pUser = NULL;
	if(wName.size() > 0)
	{
		Lock();
		map<wstring, User*>::iterator it = m_byName.find(wName);
		if(it != m_byName.end())
		{
			pUser = it->second;
		}
		Unlock();
	}
	return pUser;
}

User* CUserDB::GetUserByDBID(UINT databaseId)
{
	User *pUser = NULL;
	if(databaseId)
	{
		Lock();
		map<UINT, User*>::iterator Iter = m_byDatabaseId.find(databaseId);
		if(Iter!=m_byDatabaseId.end())
		{
			pUser = Iter->second;
		}
		Unlock();
	}
	return pUser;
}

User* CUserDB::GetUserByServerId(UINT serverId)
{
	User *pUser = NULL;
	if(serverId)
	{
		Lock();
		map<UINT, User*>::iterator Iter = m_byServerId.find(serverId);
		if(Iter!=m_byServerId.end())
		{
			pUser = Iter->second;
		}
		Unlock();
	}
	return pUser;
}

INT32 CUserDB::GetExp(int nLevel)
{
	if(nLevel > 0 && nLevel < 82)
	{
		return nExpTable[nLevel];
	}
	return NULL;
}


void CUserDB::DeleteItemInInventory(IntList& lItems)
{
	if(lItems.size() > 0)
	{
		Lock();
		//Have to lock db to affect all logged in members
		for(map<UINT, User*>::iterator mIter = m_byServerId.begin();mIter!=m_byServerId.end();mIter++)
		{
			if(User *pUser = mIter->second)
			{
				if(pUser->ValidUser())
				{
					if(pUser->IsNowTrade())
						pUser->TradeCancel();

					for(IntList::iterator lIter = lItems.begin();lIter!=lItems.end();lIter++)
					{
						if(int nItemID = *lIter)
						{
							if(pUser->HaveItemInPrivateStore(nItemID))
							{
								pUser->QuitPrivateStore();
							}
							ItemList lDelete;
							CItem *pItem = pUser->inventory.GetFirstItemByClassID(nItemID);
							while(pItem->IsValidItem())
							{
								lDelete.push_back(pItem);
								pItem = pUser->inventory.GetNextItemByClassID(pItem->pSID->nItemIndex);
							}
							for(ItemList::iterator dIter = lDelete.begin();dIter!=lDelete.end();dIter++)
							{
								pItem = (*dIter);
								g_DB.RequestDestroyItem(pItem, pItem->pSID->nItemAmount, pUser); 
							}
						}
					}
				}
			}
		}
		Unlock();
	}
}


void CUserDB::KickOutFromColiseum(IVector pos)
{
	Lock();
	try
	{
		for(map<UINT, User*>::iterator Iter = m_byServerId.begin();Iter!=m_byServerId.end();Iter++)
		{
			if(Iter->second->ValidUser())
			{
				if(User *pUser = Iter->second)
				{
					if(pUser->pSD->Pos.x > 147534 && pUser->pSD->Pos.x < 151470)
					{
						if(pUser->pSD->Pos.y > 45274 && pUser->pSD->Pos.y < 48164)
						{
							pUser->StopMove();
							PlayerAction::OnTeleportToLocation(pUser, pos.x, pos.y, pos.z, false);
						}
					}
				}
			}
		}
	}catch(...)
	{
		EXCEPTION_LOG;
	}
	Unlock();
}

void CUserDB::KickOutFromRange(IVector pos, int minX, int maxX, int minY, int maxY)
{
	double x1 = static_cast<double>(minX);
	double x2 = static_cast<double>(maxX);
	double y1 = static_cast<double>(minY);
	double y2 = static_cast<double>(maxY);
	
	Lock();
	try
	{
		for(map<UINT, User*>::iterator Iter = m_byServerId.begin();Iter!=m_byServerId.end();Iter++)
		{
			if(Iter->second->ValidUser())
			{
				if(User *pUser = Iter->second)
				{
					if(pUser->pSD->Pos.x >= x1 && pUser->pSD->Pos.x <= x2)
					{
						if(pUser->pSD->Pos.y >= y1 && pUser->pSD->Pos.y <= y2)
						{
							pUser->StopMove();
							PlayerAction::OnTeleportToLocation(pUser, pos.x, pos.y, pos.z, false);
						}
					}
				}
			}
		}
	}catch(...)
	{
		EXCEPTION_LOG;
	}
	Unlock();
}

bool CUserDB::AnyPrivateStoreInRange(FVector pos, double range)
{
	bool ret = false;
	guard;
	Lock();
	try
	{
		for(map<UINT, User*>::iterator Iter = m_byServerId.begin();Iter!=m_byServerId.end();Iter++)
		{
			if(Iter->second->ValidUser())
			{
				if(User *pUser = Iter->second->GetUser())
				{
					if(pUser->pSD->nStoreMode == 1 || pUser->pSD->nStoreMode == 3 || pUser->pSD->nStoreMode == 5 )
					{
						double x1 = pos.x;
						double y1 = pos.y;
						double x2 = pUser->pSD->Pos.x;
						double y2 = pUser->pSD->Pos.y;
						double dist = sqrt(pow((x2-x1), 2.0) + pow((y2-y1), 2.0));
						if(dist < range)
						{
							ret = true;
							break;
						}
					}
				}
			}
		}
	}catch(...)
	{
		EXCEPTION_LOG;
	}
	Unlock();
	unguard;
	return ret;
}

void CUserDB::GiveItemToAllUser(INT32 itemId, INT32 count, BOOL checkIp)
{
	guard;
	Lock();
	try
	{
		vector<HWID> usedHWID;

		for(map<UINT, User*>::iterator Iter = m_byServerId.begin();Iter!=m_byServerId.end();Iter++)
		{
			User *pUser = Iter->second;
			if(pUser->ValidUser())
			{
				CUserSocket *pSocket = pUser->pSocket;
				if(pSocket)
				{
					if(pUser->pSD->activateOfflineShopTime == 0)
					{
						if(pUser->IsNowTrade())
						{
							pUser->TradeCancel();
						}

						bool canReceiveReward = true;

						//check for hwid
						if(checkIp)
						{
							if(CUserSocket *pSocket = pUser->GetSocket())
							{
								HWID& hwid = pSocket->pED->longHWID;
								for(UINT n=0;n<usedHWID.size();n++)
								{
									if(hwid.IsEqual(usedHWID[n]))
									{
										canReceiveReward = false;
										break;
									}
								}
								if(canReceiveReward)
								{
									usedHWID.push_back(hwid);
								}
							}
							
						}
						if(canReceiveReward)
						{
							g_Logger.Add(L"User[%s] get vote reward[%d][%d]", pUser->pSD->wszName, itemId, count);
							pUser->AddItemToInventory(itemId, count);
						}else
						{
							g_Logger.Add(L"User[%s] cannot receive vote reward[%d][%d]", pUser->pSD->wszName, itemId, count);
						}
					}
				}
			}
		}
	}catch(...)
	{
		EXCEPTION_LOG;
	}
	Unlock();
	unguard;
}

void CUserDB::GetUsersByIp(UINT ip, vector<User*>& vUsers)
{
	guard;
	Lock();
	for(map<UINT, User*>::iterator Iter = m_byServerId.begin();Iter!=m_byServerId.end();Iter++)
	{
		if(User *pUser = Iter->second->GetUser())
		{
			if(pUser->pSocket->addr.S_un.S_addr == ip)
			{
				vUsers.push_back(pUser);
			}
		}
	}
	Unlock();
	unguard;
}

void CUserDB::GetUsersByHwid(LPBYTE lpHwid, vector<User*>& vUsers)
{
	guard;
	Lock();
	for(map<UINT, User*>::iterator Iter = m_byServerId.begin();Iter!=m_byServerId.end();Iter++)
	{
		if(User *pUser = Iter->second->GetUser())
		{
			if(!memcmp(lpHwid, pUser->pSocket->pED->longHWID.hash, 32))
			{
				vUsers.push_back(pUser);
			}
		}
	}
	Unlock();
	unguard;
}

void CUserDB::GetUsersByIpHwid(UINT ip, LPBYTE lpHwid, vector<User*>& vUsers)
{
	guard;
	Lock();
	for(map<UINT, User*>::iterator Iter = m_byServerId.begin();Iter!=m_byServerId.end();Iter++)
	{
		if(User *pUser = Iter->second->GetUser())
		{
			if(!memcmp(lpHwid, pUser->pSocket->pED->longHWID.hash, 32) && pUser->pSocket->addr.S_un.S_addr == ip)
			{
				vUsers.push_back(pUser);
			}
		}
	}
	Unlock();
	unguard;
}

INT32 CUserDB::GetOlympiadUserAmountByIPnHWID(UINT ip, LPBYTE lpHwid)
{
	guard;
	INT32 amount = 0;
	Lock();
	for(map<UINT, User*>::iterator Iter = m_byServerId.begin();Iter!=m_byServerId.end();Iter++)
	{
		if(User *pUser = Iter->second->SafeCastUser())
		{
			if(pUser->olympiadUser.GetState() != 0 && pUser->olympiadUser.GetState() != 3)
			{
				if(!memcmp(lpHwid, pUser->pSocket->pED->longHWID.hash, 32) && pUser->pSocket->addr.S_un.S_addr == ip)
				{
					amount++;
				}
			}
		}
	}
	Unlock();
	unguard;
	return amount;
}

void CUserDB::SetInstanceTimeout(HWID hwid, UINT instanceId, UINT timeout)
{
	CTL;
	{
		AUTO_LOCK(m_InstanceLock);
		m_InstanceByHWID[instanceId][hwid] = timeout;
	}
}

UINT CUserDB::GetInstanceTimeout(HWID hwid, UINT instanceId)
{
	CTL;
	{
		AUTO_LOCK(m_InstanceLock);
		map<UINT, map<HWID, UINT>>::iterator it = m_InstanceByHWID.find(instanceId);
		if(it != m_InstanceByHWID.end())
		{
			map<HWID, UINT>::iterator itEx = it->second.find(hwid);
			if(itEx != it->second.end())
			{
				return itEx->second;
			}
		}
	}

	return 0;
}