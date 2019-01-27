#include "StdAfx.h"
#include "BuildCmdHandler.h"
#include "HTMLCacheManager.h"
#include "CursedWeaponSystem.h"
#include "ItemEnchant.h"
#include "DB.h"
#include "SocketDB.h"
#include "AIO.h"
#include "PlayerAction.h"
#include "SocketLimiter.h"
#include "AccountDB.h"
#include "HeroSystem.h"
#include "Geo.h"
#include "HtmlFilter.h"
#include "ObjectDB.h"
#include "CliExt.h"

extern DOUBLE* g_lpExpRate;
extern DOUBLE* g_lpDropRate;
extern DOUBLE* g_lpSpoilRate;

map<wstring, CCommandInfo> CBuilderCommand::mData;
list<int> lBuilderAccount;

map<wstring, int> g_CommandPrivilege;	//<name, requiredLevel>

BuilderCmdInfo* pOrgHandler;	//(280)

void CBuilderCommand::SetBuilderCmdLevel(const WCHAR* wName, int newLevel)
{
	for(int n=0;n<281;n++)
	{
		if(!wcscmp(wName, pOrgHandler[n].pName))
		{
			pOrgHandler[n].requiredLevel = newLevel;
			return;
		}
	}
}

void CBuilderCommand::LoadData()
{
	wstringstream file(ReadFileW(L"..//Script//BuilderCmd.txt"));
	if(file)
	{
		int lineCount = 0;
		wstring line;
		while(getline(file, line))
		{
			lineCount++;
			if(line.size() > 4)
			{
				if( line[0] == L'/' || line[0] == L';' )
					continue;

				if( line.find(L"cmd_begin") == 0 )
				{
					//cmd_begin	name=stopsay	required_builder_level=9	cmd_end
					wstring wName = Parser::ParseOptionString(line, L"name");
					int requiredLevel = Parser::ParseOptionInt(line, L"required_builder_level", 1);
					
					if(wName.size() > 0 && requiredLevel > 0)
					{
						g_CommandPrivilege.insert(pair<wstring, int>(wName, requiredLevel));
					}else
					{
						g_Log.Add(CLog::Error, "[%S] Parser error line[%d]!", __FUNCTIONW__, lineCount);
					}
				}
			}
		}
	}else
	{
		g_Log.Add(CLog::Error, "[%s] Cannot find ..//Script//BuilderCmd.txt !", __FUNCTION__);
	}
}

void CBuilderCommand::Initialize()
{
	pOrgHandler = (BuilderCmdInfo*)0xBECFA0;

	LoadData();

	CCommandInfo Info;
	Info.Set(L"cursed_weapon_reset", 5, ResetCursedWeapon);
	mData.insert(pair<wstring, CCommandInfo>(L"cursed_weapon_reset", Info));
	Info.Set(L"cursed_weapon_info", 5, CursedWeaponInfo);
	mData.insert(pair<wstring, CCommandInfo>(L"cursed_weapon_info", Info));
	Info.Set(L"healthy", 5, Healthy);
	mData.insert(pair<wstring, CCommandInfo>(L"healthy", Info));
	Info.Set(L"redsky", 5, RedSky);
	mData.insert(pair<wstring, CCommandInfo>(L"redsky", Info));
	Info.Set(L"add_reputation", 1, AddReputation);
	mData.insert(pair<wstring, CCommandInfo>(L"add_reputation", Info));
	Info.Set(L"set_hero", 1, SetHero);
	mData.insert(pair<wstring, CCommandInfo>(L"set_hero", Info));
	Info.Set(L"del_hero", 1, DelHero);
	mData.insert(pair<wstring, CCommandInfo>(L"del_hero", Info));
	Info.Set(L"set_nobless", 1, SetNobless);
	mData.insert(pair<wstring, CCommandInfo>(L"set_nobless", Info));
	Info.Set(L"reset_skill_reuse", 5, ResetReuse);
	mData.insert(pair<wstring, CCommandInfo>(L"reset_skill_reuse", Info));
	Info.Set(L"announce2", 5, Announce2);
	mData.insert(pair<wstring, CCommandInfo>(L"announce2", Info));
	Info.Set(L"delete_clan_skills", 5, DelClanSkills);
	mData.insert(pair<wstring, CCommandInfo>(L"delete_clan_skills", Info));
	Info.Set(L"play_event_sound", 5, PlayEventSound);
	mData.insert(pair<wstring, CCommandInfo>(L"play_event_sound", Info));
	Info.Set(L"party_recall", 5, PartyRecall);
	mData.insert(pair<wstring, CCommandInfo>(L"party_recall", Info));
	Info.Set(L"who", 1, Who);
	mData.insert(pair<wstring, CCommandInfo>(L"who", Info));
	Info.Set(L"WHO", 1, Who);
	mData.insert(pair<wstring, CCommandInfo>(L"WHO", Info));
	Info.Set(L"who_is_online", 1, WhoIsOnline);
	mData.insert(pair<wstring, CCommandInfo>(L"who_is_online", Info));
	Info.Set(L"augment_item", 1, AugmentItem);
	mData.insert(pair<wstring, CCommandInfo>(L"augment_item", Info));
	Info.Set(L"test_cmd", 1, TestCMD);
	mData.insert(pair<wstring, CCommandInfo>(L"test_cmd", Info));

	//Team Container
	Info.Set(L"create_team", 5, CreateTeam);
	mData.insert(pair<wstring, CCommandInfo>(L"create_team", Info));
	Info.Set(L"delete_team", 5, DeleteTeam);
	mData.insert(pair<wstring, CCommandInfo>(L"delete_team", Info));
	Info.Set(L"clear_team", 5, ClearTeam);
	mData.insert(pair<wstring, CCommandInfo>(L"clear_team", Info));
	Info.Set(L"view_team", 5, TeamList);
	mData.insert(pair<wstring, CCommandInfo>(L"view_team", Info));
	Info.Set(L"add_to_team", 5, AddUserToTeam);
	mData.insert(pair<wstring, CCommandInfo>(L"add_to_team", Info));
	Info.Set(L"del_from_team", 5, DelUserFromTeam);
	mData.insert(pair<wstring, CCommandInfo>(L"del_from_team", Info));
	Info.Set(L"message_team", 5, MessageToTeam);
	mData.insert(pair<wstring, CCommandInfo>(L"message_team", Info));
	Info.Set(L"message_param_team", 5, MessageWithParamToTeam);
	mData.insert(pair<wstring, CCommandInfo>(L"message_param_team", Info));
	Info.Set(L"ht", 5, HTMessage);
	mData.insert(pair<wstring, CCommandInfo>(L"ht", Info));
	Info.Set(L"set_client_limit", 1, SetClientLimit);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));
	Info.Set(L"show_hwid_list", 5, ShowUsersByHwid);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));
	Info.Set(L"show_ip_list", 5, ShowUsersByIP);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));
	Info.Set(L"show_hwid_ip_list", 5, ShowUsersByIPnHwid);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));
	Info.Set(L"set_pending_write_limit", 1, SetPendingWriteLimit);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));
	Info.Set(L"setkarma", 1, SetKarma);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));
	Info.Set(L"set_client_version", 1, SetClientVersion);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));
	Info.Set(L"set_ssq_sky", 1, SetSSQSky);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));
	Info.Set(L"enchant_weapon", 1, EnchantWeapon);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));
	Info.Set(L"enchant_item", 1, EnchantItem);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));
	Info.Set(L"get_sd_value", 1, GetSDValue);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));
	Info.Set(L"giveitem", 1, GiveItem);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));
	Info.Set(L"delitem", 1, DelItem);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));

	Info.Set(L"set_aio", 1, SetAIO);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));
	Info.Set(L"del_aio", 1, DelAIO);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));
	Info.Set(L"add_to_npcpos", 1, AddToNpcPos);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));

	Info.Set(L"loc", 1, AddToLoc);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));

	Info.Set(L"coliseum_flush", 3, ColiseumFlush);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));
	Info.Set(L"reset_clan_name", 3, ResetClanName);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));
	Info.Set(L"tvt", 3, ShowTvTPanel);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));
	Info.Set(L"delete_vip_status", 1, DelVIPStatus);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));
	Info.Set(L"set_exp_rate", 1, SetExpRate);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));
	Info.Set(L"set_drop_rate", 1, SetDropRate);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));
	Info.Set(L"set_spoil_rate", 1, SetSpoilRate);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));

	Info.Set(L"block_item", 1, ShowBlockableItems);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));
	Info.Set(L"ban_account", 1, CBuilderCommand::BanAccount);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));
	Info.Set(L"ban_reset", 1, CBuilderCommand::ResetBan);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));
	Info.Set(L"ban_hwid", 1, CBuilderCommand::BanHwid);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));
	Info.Set(L"ban_hwid_ex", 1, CBuilderCommand::BanHwidEx);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));
	Info.Set(L"unban_hwid", 1, CBuilderCommand::UnbanHwid);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));
	Info.Set(L"perma_ban_hwid", 1, CBuilderCommand::PermaBanHwid);
	mData[Info.wsName] = Info;

	Info.Set(L"give_item_to_all", 1, CBuilderCommand::GiveItemToAll);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));

	Info.Set(L"killplayer", 1, CBuilderCommand::KillPlayer);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));
	Info.Set(L"killparty", 1, CBuilderCommand::KillParty);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));
	Info.Set(L"save_manor", 1, CBuilderCommand::SaveManor);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));
	Info.Set(L"give_item_in_range", 1, CBuilderCommand::GiveItemInRange);
	mData.insert(pair<wstring, CCommandInfo>(Info.wsName, Info));


	LoadBuilderList();

	for(map<wstring, int>::iterator Iter = g_CommandPrivilege.begin(); Iter!=g_CommandPrivilege.end();Iter++)
	{
		SetBuilderCmdLevel(Iter->first.c_str(), Iter->second);
	}
};

void CBuilderCommand::ValidateBuilder(User *pUser)
{
	if(pUser->ValidUser())
	{
		if(pUser->pSD->nBuilder)
		{
			int accountId = pUser->pSocket->accountId;
			for(list<int>::iterator Iter = lBuilderAccount.begin();Iter!=lBuilderAccount.end();Iter++)
			{
				if((*Iter) == accountId)
				{
					return;
				}
			}

			g_Log.Add(CLog::Error, "[%s] Not authorized user have builder rights[%d] - User[%S] AccountId[%d]", __FUNCTION__, pUser->pSD->nBuilder, pUser->pSD->wszName, accountId);
			pUser->pSocket->Close();
		}
	}
}

void CBuilderCommand::LoadBuilderList()
{
	lBuilderAccount.clear();
	TCHAR tmp[8192];
	memset(tmp, 0, 8192);
	if(GetPrivateProfileString(_T("L2Server"), _T("BuilderAccountIdList"), 0, tmp, sizeof(tmp), g_ConfigFile) > 0)
	{
		tstringstream sstr(tmp);
		int accountId = 0;
		while(sstr >> accountId)
		{
			lBuilderAccount.push_back(accountId);
		}
	}
}

bool CBuilderCommand::Handle(User *pUser, wstring wsBuff)
{
	if(pUser->ValidUser() && pUser->pSD->nBuilder)
	{
		try
		{
			wstringstream sstr;
			wstring sCommand;
			sstr << wsBuff;
			sstr >> sCommand;
			map<wstring, CCommandInfo>::iterator Iter = mData.find(sCommand);
			if(Iter!=mData.end())
			{
				if(pUser->pSD->nBuilder <= Iter->second.nRequiedLevel)
				{
					wstring sParams;
					getline(sstr, sParams);
					return Iter->second.Func(pUser, sParams);
				}
				else
					pUser->SendSystemMessage(L"Access Denied!");
			}
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception Detected - [%S]!", __FUNCTION__, wsBuff.c_str());
			return false;
		}
	}else
	{
		if(pUser->ValidUser())
		{
			g_Log.Add(CLog::Error, "[%s] Not GM User[%s] trying to use GM Command!", pUser->pSD->wszName);
		}
	}
	return true;
};

bool CBuilderCommand::GiveItemInRange(User *pUser, wstring wParams)
{
	guard;

	wstringstream paramStream(wParams);
	int itemId = 0, amount = 0, range = 0;
	if(paramStream >> itemId >> amount >> range)
	{
		UINT counter = 0;
		g_UserDB.Lock();
		map<UINT, User*>& mData = g_UserDB.GetUserMap();
		for(map<UINT, User*>::iterator Iter = mData.begin(); Iter!=mData.end(); Iter++)
		{
			if(User *pTarget = Iter->second->SafeCastUser())
			{
				if(pTarget->IsInRange(pUser, range))
				{
					pTarget->AddItemToInventory(itemId, amount);
					counter++;
					g_Logger.Add(L"User[%s] got item[%d][%d] from gm[%s] from give_item_in_range - range[%d]", pTarget->pSD->wszName, itemId, amount, pUser->pSD->wszName, range);
				}
			}
		}
		g_UserDB.Unlock();
		wstringstream msg;
		msg << L"Affected players: " << counter;
		pUser->SendSystemMessage(msg.str().c_str());
	}else
	{
		pUser->SendSystemMessage(L"//give_item_in_range [item_id] [amount] [range]");
	}

	unguard;
	return true;
}

bool CBuilderCommand::PermaBanHwid(User *pUser, wstring wParams)
{
	guard;

	User *pTarget = User::GetUserBySID(&pUser->targetId);
	if(pTarget->ValidUser())
	{
		g_Logger.Add(L"GM[%s] banned hwid[%s] permanently.", pUser->pSD->wszName, pTarget->pSocket->pED->longHWID.ToString().c_str());
		UINT timeout = INT_MAX;
		g_CliExt.Ban(pTarget->pSocket->pED->longHWID.hash, pTarget->pSocket->accountName, pUser->pSD->wszName, timeout);
		pTarget->Kick();
		pUser->SendSystemMessage(L"Command accepted!");
	}else
	{
		pUser->SendSystemMessage(L"Invalid target!");
	}

	unguard;
	return true;
}

bool CBuilderCommand::SaveManor(User *pUser, wstring wParams)
{
	guard;
	UINT manorId;
	wstringstream dataStream(wParams);
	if(dataStream >> manorId)
	{
		if(manorId < 10)
		{
			if(CManor *pManor = CManor::GetManor(manorId))
			{
				pManor->WriteLock(__LINE__, __FILEW__);
				pManor->SaveNowManorInfo();
				pManor->WriteUnlock(__LINE__, __FILEW__);
				pUser->SendSystemMessage(L"Command accepted!");
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Cannot find manor[%d]!", __FUNCTION__, manorId);
			}
		}
	}
	unguard;
	return true;
}

bool CBuilderCommand::KillParty(User* pUser, wstring wParams)
{
	wstring wName;
	wstringstream str(wParams);
	if( str >> wName )
	{	
		if(User *pTarget = g_UserDB.GetUserByName(wName))
		{
			if(CParty *pParty = pTarget->GetParty())
			{
				UserVector vUsers;
				pParty->GetAllMembersEx(vUsers);
				for(UINT n=0;n<vUsers.size();n++)
				{
					if(User *pMember = vUsers[n]->SafeCastUser())
					{
						if(pMember->pSD->nAlive)
						{
							pMember->killDamageType = 5;
							PlayerAction::OnDie(pMember, 0);
							g_Logger.Add(L"GM[%s] killed player[%s]!", pUser->pSD->wszName, pMember->pSD->wszName);
						}
					}
				}
			}
		}else
		{
			pUser->SendSystemMessage(L"Cannot find user!");
		}
	}else
	{
		if(pUser->targetId)
		{
			if(User *pTarget = User::GetUserBySID(&pUser->targetId))
			{
				if(CParty *pParty = pTarget->GetParty())
				{
					UserVector vUsers;
					pParty->GetAllMembersEx(vUsers);
					for(UINT n=0;n<vUsers.size();n++)
					{
						if(User *pMember = vUsers[n]->SafeCastUser())
						{
							if(pMember->pSD->nAlive)
							{
								pMember->killDamageType = 5;
								PlayerAction::OnDie(pMember, 0);
								g_Logger.Add(L"GM[%s] killed player[%s]!", pUser->pSD->wszName, pMember->pSD->wszName);
							}
						}
					}
				}
			}else
			{
				pUser->SendSystemMessage(L"Invalid target!");
			}
		}
	}
	return true;
}

bool CBuilderCommand::KillPlayer(User* pUser, wstring wParams)
{
	wstring wName;
	wstringstream str(wParams);
	if( str >> wName )
	{	
		if(User *pTarget = g_UserDB.GetUserByName(wName))
		{
			if(pTarget->pSD->nAlive)
			{
				pTarget->killDamageType = 5;
				PlayerAction::OnDie(pTarget, 0);
				g_Logger.Add(L"GM[%s] killed player[%s]!", pUser->pSD->wszName, pTarget->pSD->wszName);
			}
		}else
		{
			pUser->SendSystemMessage(L"Cannot find user!");
		}
	}else
	{
		if(pUser->targetId)
		{
			if(User *pTarget = User::GetUserBySID(&pUser->targetId))
			{
				if(pTarget->pSD->nAlive)
				{
					pTarget->killDamageType = 5;
					PlayerAction::OnDie(pTarget, 0);
					g_Logger.Add(L"GM[%s] killed player[%s]!", pUser->pSD->wszName, pTarget->pSD->wszName);
				}
			}else
			{
				pUser->SendSystemMessage(L"Invalid target!");
			}
		}
	}
	return true;
}

bool CBuilderCommand::UnbanHwid(User* pUser, wstring wParams)
{
	wstring hwid;
	wstringstream str(wParams);
	if( str >> hwid )
	{
		HWID Hwid(hwid);
		g_Logger.Add(L"GM[%s] removed ban for hwid[%s]", pUser->pSD->wszName, hwid.c_str());
		g_CliExt.UnBan(Hwid.hash, pUser->pSD->wszName);

		pUser->SendSystemMessage(L"Command accepted!");
	}else
	{
		pUser->SendSystemMessage(L"Format: //unban_hwid [hardware_id]");
	}
	return true;
}

bool CBuilderCommand::BanHwidEx(User* pUser, wstring wParams)
{
	wstring hwid;
	UINT timeout = 0;
	wstring reason;
	wstringstream str(wParams);
	if( str >> hwid >> timeout >> reason )
	{	
		HWID Hwid(hwid);
		g_Logger.Add(L"GM[%s] banned hwid[%s] for [%d] second(s) reason[%S]", pUser->pSD->wszName, hwid.c_str(), timeout, reason.c_str());
		timeout += time(0);
		g_CliExt.Ban(Hwid.hash, L"none", pUser->pSD->wszName, timeout);
		pUser->SendSystemMessage(L"Command accepted!");
	}else
	{
		pUser->SendSystemMessage(L"Format: //ban_hwid_ex [hwid] [timeInSeconds] [reason_of_ban] (reason have to be without spaces - use _ as a space separator)");
	}
	
	return true;
}

bool CBuilderCommand::GiveItemToAll(User *pUser, std::wstring wsParams)
{
	wstringstream str(wsParams);
	INT32 itemId = 0;
	INT32 count = 0;
	if(str >> itemId >> count)
	{
		g_UserDB.GiveItemToAllUser(itemId, count);
		pUser->SendSystemMessage(L"Command Accepted!");
		g_Logger.Add(L"GM[%s] give item to all [%d][%d]", pUser->pSD->wszName, itemId, count);
	}
	return true;
}


bool CBuilderCommand::BanHwid(User* pUser, wstring wParams)
{
	User *pTarget = User::GetUserBySID(&pUser->targetId);
	if(pTarget->ValidUser())
	{
		UINT timeout = 0;
		wstring reason;
		wstringstream str(wParams);
		if( str >> timeout >> reason )
		{	
			g_Logger.Add(L"GM[%s] banned hwid[%s] for [%d] second(s) reason[%s]", pUser->pSD->wszName, pTarget->pSocket->pED->longHWID.ToString().c_str(), timeout, reason.c_str());
			timeout += time(0);
			g_CliExt.Ban(pTarget->pSocket->pED->longHWID.hash, pTarget->pSocket->accountName, pUser->pSD->wszName, timeout);
			pTarget->Kick();
			pUser->SendSystemMessage(L"Command accepted!");
		}else
		{
			pUser->SendSystemMessage(L"Format: //ban_hwid [timeInSeconds] [reason_of_ban] (reason have to be without spaces - use _ as a space separator)");
		}
	}else
	{
		pUser->SendSystemMessage(L"Invalid target!");
	}
	return true;
}



bool CBuilderCommand::ResetBan(User* pUser, wstring wParams)
{
	UINT accountId = 0;
	wstringstream str(wParams);
	if( str >> accountId )
	{	
		g_Logger.Add(L"GM[%s] removed ban for account[%d]", pUser->pSD->wszName, accountId);
		g_AccountDB.Ban(accountId, L"", 0);
		pUser->SendSystemMessage(L"Command accepted!");
	}else
	{
		pUser->SendSystemMessage(L"Format: //reset_ban [account_id]");
	}
	return true;
}

bool CBuilderCommand::BanAccount(User* pUser, wstring wParams)
{
	User *pTarget = User::GetUserBySID(&pUser->targetId);
	if(pTarget->ValidUser())
	{
		UINT timeout = 0;
		wstring reason;
		wstringstream str(wParams);
		if( str >> timeout >> reason )
		{	
			g_Logger.Add(L"GM[%s] banned account[%d][%s] for [%d] second(s)", pUser->pSD->wszName, pTarget->pSocket->accountId, pTarget->pSocket->accountName, timeout);
			g_AccountDB.Ban(pTarget->pSocket->accountId, pTarget->pSocket->accountName, timeout, reason.c_str());
			pUser->SendSystemMessage(L"Command accepted!");
		}else
		{
			pUser->SendSystemMessage(L"Format: //ban_account [timeInSeconds] [reason_of_ban] (reason have to be without spaces - use _ as a space separator)");
		}
	}else
	{
		pUser->SendSystemMessage(L"Invalid target!");
	}
	return true;
}

bool CBuilderCommand::ShowBlockableItems(User* pUser, wstring wParams)
{
	wstring playerName;

	wstringstream str(wParams);
	if( str >> playerName )
	{
		User *pTarget = g_UserDB.GetUserByName(playerName);
		if(pTarget->ValidUser())
		{
			pTarget->ShowBlockableItems(pUser);
		}else
		{
			pUser->pSocket->SendSystemMessage(L"Invalid user name!");
		}
	}else
	{
		pUser->pSocket->SendSystemMessage(L"Format: //block_item [player_name]");
	}
	return false;
}

bool CBuilderCommand::AddToLoc(User* pUser, wstring wParams)
{
	static vector<FVector> locations;

	wstring type;
	wstringstream str(wParams);
	if( str >> type)
	{
		if(type == L"save")
		{
			stringstream nameStream;
			nameStream << "Locations_" << time(0) << ".txt";
			ofstream fi(nameStream.str().c_str());
			for(INT32 n=0;n<locations.size();n++)
			{
				FVector& pos = locations[n];
				fi << (INT32)pos.x << ";" << (INT32)pos.y << ";" << (INT32)pos.z << endl;
			}
			fi.close();
			pUser->SendSystemMessage(L"Locations have been saved!");
		}else if(type == L"clear")
		{
			locations.clear();
			pUser->SendSystemMessage(L"Locations have been cleared!");
		}else
		{
			pUser->SendSystemMessage(L"Invalid command! (save, clear)");
		}
	}else
	{
		locations.push_back(pUser->pSD->Pos);
		wstringstream msgStream;
		msgStream << L"Added position to locations (" << locations.size() << L")";
		pUser->SendSystemMessage(msgStream.str().c_str());
	}

	return false;
}

bool CBuilderCommand::AddToNpcPos(User* pUser, wstring wParams)
{
	wstring npcName, territoryName;
	INT32 count = 0, range = 0;

	wstringstream str(wParams);
	if( str >> territoryName >> npcName >> count >> range )
	{
		int classId = g_ObjectDB.GetClassIdFromName(npcName.c_str());
		if(classId > 1000000)
		{
			IVector iPos(pUser->pSD->Pos);
			wstring wData = CreateSpawnEntry(territoryName, range, iPos, pUser->pSD->sHeading, npcName, count);
			AppendToFileW(L"..//Script//NpcPos.txt", wData);
			pUser->pSocket->SendSystemMessage(L"Npc has been added to npcpos.txt");
			g_Log.Add(CLog::Black, "AddToNpcPos: GM[%S] npcName[%S][%d] pos(%d, %d, %d) count(%d) range(%d)", pUser->pSD->wszName, npcName.c_str(), classId, iPos.x, iPos.y, iPos.z, count, range);
		}else
		{
			pUser->pSocket->SendSystemMessage(L"Invalid npc name!");
		}
	}else
	{
		pUser->pSocket->SendSystemMessage(L"Format: //add_to_npcpos [territory_name] [npc_name] [count] [range]");
	}
	return false;
}

bool CBuilderCommand::DelAIO(User *pUser, wstring wsParams)
{
	if(pUser->ValidUser())
	{
		if(pUser->targetId)
		{
			User *pTarget = User::GetUserBySID(&pUser->targetId);
			if(pTarget->ValidUser())
			{
				if(pTarget->pSD->nSubClassIndex == 0)
				{
					if(pTarget->pED->aioTimestamp)
					{
						pTarget->pED->aioTimestamp = 1;
						g_Logger.Add(L"DelAIO: GM[%s] changed aio status for user[%s]", pUser->pSD->wszName, pTarget->pSD->wszName);
					}
				}else
				{
					pUser->SendSystemMessage(L"Player have to be in main class to delete AIO status!");
				}
			}else
			{
				pUser->SendSystemMessage(L"Invalid target!");
			}
		}else
		{
			pUser->SendSystemMessage(L"Invalid target!");
		}
	}
	return false;
}

bool CBuilderCommand::SetAIO(User *pUser, wstring wsParams)
{
	if(pUser->ValidUser())
	{
		wstringstream sstr;
		sstr << wsParams;
		UINT id = 0;
		sstr >> id;

		if( id > 0)
		{
			if(pUser->targetId)
			{
				User *pTarget = User::GetUserBySID(&pUser->targetId);
				if(pTarget->ValidUser())
				{
					if(pTarget->pSD->nSubClassIndex == 0)
					{
						g_AIOSystem.BecomeAIO(pTarget, id, true);
						g_Logger.Add(L"SetAIO: GM[%s] set AIO id[%d] for user[%s]", pUser->pSD->wszName, id, pTarget->pSD->wszName);
						pUser->SendSystemMessage(L"Command accepted!");
					}else
					{
						pUser->SendSystemMessage(L"Player have to be in main class to obtain AIO status.");
					}
				}else
				{
					pUser->SendSystemMessage(L"Invalid target!");
				}
			}else
			{
				pUser->SendSystemMessage(L"Invalid target!");
			}
		}else
		{
			pUser->SendSystemMessage(L"usage: //set_aio [id]   - id from AIOData.txt");
		}
	}
	return false;
}

bool CBuilderCommand::SetExpRate(User *pUser, wstring wsParams)
{
	if(pUser->ValidUser())
	{
		wstringstream sstr;
		sstr << wsParams;
		int rate;
		sstr >> rate;

		if(rate > 0)
		{
			(*g_lpExpRate) = (double)rate;
			g_Log.Add(CLog::Blue, "[%s] new rate[%f] chagned by ]%S]", __FUNCTION__, (*g_lpExpRate), pUser->pSD->wszName);
		}else
		{
			pUser->SendSystemMessage(L"Usage: //set_exp_rate value   (where value for 1x = 100)");
		}
	}
	return false;
}

bool CBuilderCommand::SetDropRate(User *pUser, wstring wsParams)
{
	if(pUser->ValidUser())
	{
		wstringstream sstr;
		sstr << wsParams;
		int rate;
		sstr >> rate;

		if(rate > 0)
		{
			(*g_lpDropRate) = (double)rate;
			g_Log.Add(CLog::Blue, "[%s] new rate[%f] chagned by ]%S]", __FUNCTION__, (*g_lpDropRate), pUser->pSD->wszName);
		}else
		{
			pUser->SendSystemMessage(L"Usage: //set_drop_rate value   (where value for 1x = 1)");
		}
	}
	return false;
}

bool CBuilderCommand::SetSpoilRate(User *pUser, wstring wsParams)
{
	if(pUser->ValidUser())
	{
		wstringstream sstr;
		sstr << wsParams;
		int rate;
		sstr >> rate;

		if(rate > 0)
		{
			(*g_lpSpoilRate) = (double)rate;
			g_Log.Add(CLog::Blue, "[%s] new rate[%f] chagned by ]%S]", __FUNCTION__, (*g_lpSpoilRate), pUser->pSD->wszName);
		}else
		{
			pUser->SendSystemMessage(L"Usage: //set_spoil_rate value   (where value for 1x = 100)");
		}
	}
	return false;
}

bool CBuilderCommand::DelVIPStatus(User *pUser, wstring wsParams)
{
	if(pUser->ValidUser())
	{
		User *pTarget = User::GetUserBySID(&pUser->targetId);
		if(pTarget->ValidUser())
		{
			if(pTarget->pSD->vipLevel)
			{
				pTarget->pSD->vipLevel = 0;
				pTarget->pSD->vipTimestamp = 0;
				g_DB.RequestSaveVipStatus(pTarget->nDBID, 0, 0);
				g_Logger.Add(L"GM[%s] removed VIP status from user [%s]", pUser->pSD->wszName, pTarget->pSD->wszName);
				pUser->SendSystemMessage(L"VIP status has been removed.");
			}else
			{
				pUser->SendSystemMessage(L"Selected user doesnt have VIP status.");
			}
		}else
		{
			pUser->SendSystemMessage(L"Invalid target!");
		}
	}
	return false;
}

bool CBuilderCommand::ShowTvTPanel(User *pUser, wstring wsParams)
{
	if(pUser->ValidUser())
	{
		const WCHAR* pFile = g_HTMLCacheManager.GetHTMLFile( L"tvt_panel.htm", pUser->pSD->bLang);
		if(pFile)
		{
			PlayerAction::ShowHTML(pUser, L"tvt_panel.htm", pFile, 0);
		}
	}
	return false;
}

bool CBuilderCommand::ResetClanName(User *pUser, wstring wsParams)
{
	if(pUser->ValidUser())
	{
		wstring orgName;
		wstring newName;
		wstringstream sstr;
		sstr << wsParams;
		sstr >> orgName >> newName;

		newName = CParser::Trim(newName);

		if(orgName.size() > 0 && newName.size() > 0)
		{
			CPledge *pPledge = g_PledgeDB.GetPledgeByName(orgName.c_str());
			if(pPledge && pPledge->pSD)
			{
				g_DB.RequestModifyPledgeName(pPledge->pSD->nDBID, newName.c_str());
				pUser->SendSystemMessage(L"Builder command[reset_clan_name] accepted.");
			}else
			{
				pUser->SendSystemMessage(L"System", L"Pledge name is not valid or not loaded yet");
				g_DB.RequestLoadPledge(orgName.c_str());
			}
		}else
			pUser->SendSystemMessage(L"//reset_clan_name original_clan_name new_clan_name");
	}
	return false;
}

void RandomizePosInColiseum(User *pUser)
{
	if(pUser->ValidUser())
	{
		//MinX=147534	MaxX=151470	MinY=45274	MaxY=48164	MinZ=-19935	MaxZ=19935
		if(pUser->pSD->Pos.x > 147534 && pUser->pSD->Pos.x < 151470)
		{
			if(pUser->pSD->Pos.y > 45274 && pUser->pSD->Pos.y < 48164)
			{
				//user in coliseum - randomize pos
				int newX = 148534;	//+1000
				int newY = 46274;	//+1000
				int newZ = pUser->pSD->Pos.z;
				newX += g_MTRandom.RandInt(1936);	//difference - 2000
				newY += g_MTRandom.RandInt(890);	//difference - 2000
				pUser->StopMove();
				PlayerAction::OnTeleportToLocation(pUser, newX, newY, CGeo::GetInstance()->GetZLoc(newX, newY, newZ), false);
			}
		}
	}
}

bool CBuilderCommand::ColiseumFlush(User *pUser, wstring wsParams)
{
	if(pUser->ValidUser())
	{
		FOR_ALL_USERS_IN_MAP(RandomizePosInColiseum);
		pUser->SendSystemMessage(L"Players position have been randomized.");
	}
	return false;
}

bool CBuilderCommand::GetSDValue(User *pUser, wstring wsParams)
{
	if(pUser->ValidUser())
	{
		if(wsParams.size() > 0)
		{
			int nValue = 0;
			wstringstream sstr;
			sstr << wsParams;
			sstr >> nValue;
			if(nValue > 0)
			{
				if(nValue > sizeof(CreatureSharedData))
				{
					pUser->SendSystemMessage(L"GetSDValue: Invalid sd value! (Value has to be lower than SD size)");
					return false;
				}
				try
				{
					int Value = (*(LPDWORD)(pUser->pSD + nValue));
					wchar_t wsTmp[32]; memset(wsTmp, 0, sizeof(wsTmp));
					_itow_s(Value, wsTmp, 31, 10);
					pUser->SendSystemMessage(L"SDValue:", (const wchar_t*)wsTmp);
				}catch(...)
				{
					g_Log.Add(CLog::Error, "[%s] Exception!", __FUNCTION__);
				}
			}else
				pUser->SendSystemMessage(L"Usage: //enchant_weapon [enchant_level]");
		}else
			pUser->SendSystemMessage(L"Usage: //enchant_weapon [enchant_level]");
	}
	return false;
}

bool CBuilderCommand::DelItem(User *pUser, std::wstring wsParams)
{
	if(pUser->ValidUser())
	{
		User *pTarget = User::GetUserBySID(&pUser->targetId);
		if(pTarget->ValidUser())
		{
			int ItemID = 0;
			int ItemCount = 0;

			wstringstream sstr;
			sstr << wsParams;
			sstr >> ItemID;
			sstr >> ItemCount;

			if(ItemID > 0 && ItemCount > 0)
			{
				CItem *pItem = pTarget->inventory.GetFirstItemByClassID(ItemID);
				if(pItem->IsValidItem())
				{
					if(pItem->pSID->nItemAmount >= ItemCount)
					{
						pTarget->DeleteItemInInventory(ItemID, ItemCount);
					}else
					{
						pTarget->DeleteItemInInventory(ItemID, pItem->pSID->nItemAmount);
					}
					pUser->SendSystemMessage(L"Done");
				}else
				{
					pUser->SendSystemMessage(L"Selected user haven't got that item!");
				}
			}else
			{
				pUser->SendSystemMessage(L"Invalid item_id or item_count, usage://delitem item_id item_count");
			}
		}else
			pUser->SendSystemMessage(L"Invalid target!");
	}
	return false;
}

bool CBuilderCommand::GiveItem(User *pUser, std::wstring wsParams)
{
	if(pUser->ValidUser())
	{
		User *pTarget = User::GetUserBySID(&pUser->targetId);
		if(pTarget->ValidUser())
		{
			int ItemID = 0;
			int ItemCount = 0;

			wstringstream sstr;
			sstr << wsParams;
			sstr >> ItemID;
			sstr >> ItemCount;
			if(ItemID > 0 && ItemCount > 0)
			{
				if(pTarget->inventory.CheckAddable(ItemID, ItemCount))
				{
					pTarget->AddItemToInventory(ItemID, ItemCount);
				}else
				{
					pUser->SendSystemMessage(L"Cannot add items to selected character");
				}
			}else
			{
				pUser->SendSystemMessage(L"Invalid item id or count. usage: //giveitem item_id item_count");
			}
			return false;
		}
	}
	return true;
}

bool CBuilderCommand::EnchantWeapon(User *pUser, wstring wsParams)
{
	if(pUser->ValidUser())
	{
		if(wsParams.size() > 0)
		{
			User *pTarget = User::GetUserBySID(&pUser->targetId);
			if(!pTarget->ValidUser())
			{
				pUser->SendSystemMessage(L"Invalid target!");
				return false;
			}
			int nEnchant = 0;
			wstringstream sstr;
			sstr << wsParams;
			sstr >> nEnchant;
			if(nEnchant > 0)
			{
				CSharedItemData *pData = pTarget->GetEquipedWeapon();
				if(pData)
				{
					CItem *pItem = pTarget->inventory.GetItemBySID(pData->nObjectID);
					CItemEnchant::CustomEnchantItem(pTarget, pItem, nEnchant);
					pUser->SendSystemMessage(L"Command accepted.");
				}else
					pUser->SendSystemMessage(L"Target have to equip weapon first.");
			}else
				pUser->SendSystemMessage(L"Usage: //enchant_weapon [enchant_level]");
		}else
			pUser->SendSystemMessage(L"Usage: //enchant_weapon [enchant_level]");
	}
	return false;
}

bool CBuilderCommand::EnchantItem(User *pUser, wstring wsParams)
{
	if(pUser->ValidUser())
	{
		if(wsParams.size() > 0)
		{
			User *pTarget = User::GetUserBySID(&pUser->targetId);
			if(!pTarget->ValidUser())
			{
				pUser->SendSystemMessage(L"Invalid target!");
				return false;
			}
			int nItemId = 0, nOldEnchant = 0, nNewEnchant = 0;
			wstringstream sstr;
			sstr << wsParams;
			sstr >> nItemId >> nOldEnchant >> nNewEnchant;
			if(nNewEnchant > 0 && nItemId > 0)
			{
				CItem *pItem = pTarget->GetInventory()->GetFirstItemByClassID(nItemId);
				if(pItem->IsValidItem())
				{
					if(pItem->pSID->nEnchantLevel == nOldEnchant)
					{
						CItemEnchant::CustomEnchantItem(pTarget, pItem, nNewEnchant);
						pUser->SendSystemMessage(L"Command accepted.");
					}else
					{
						//find our item
						int nSafeCheck = 0;
						while((pItem->pSID->nEnchantLevel != nOldEnchant) && nSafeCheck < 120)
						{
							pItem = pTarget->GetInventory()->GetNextItemByClassID(pItem->pSID->nItemIndex);
							if(!pItem->IsValidItem())
								break;
							nSafeCheck++;
						}
						if(pItem->IsValidItem() && pItem->pSID->nEnchantLevel == nOldEnchant)
						{
							CItemEnchant::CustomEnchantItem(pTarget, pItem, nNewEnchant);
							pUser->SendSystemMessage(L"Command accepted.");
						}else
							pUser->SendSystemMessage(L"Cannot find valid item!");
					}	
				}else
				{
					pUser->SendSystemMessage(L"Cannot find valid item!");
				}
			}else
				pUser->SendSystemMessage(L"Usage: //enchant_item [item_id] [old_enchant] [new_enchant]");
		}else
			pUser->SendSystemMessage(L"Usage: //enchant_item [item_id] [old_enchant] [new_enchant]");
	}
	return false;
}


bool CBuilderCommand::SetSSQSky(User *pUser, wstring wsParams)
{
	if(pUser->ValidUser())
	{
		if(wsParams.size() > 0)
		{
			int nVersion = 0;
			wstringstream sstr;
			sstr << wsParams;
			sstr >> nVersion;
			if(nVersion > -1 && nVersion < 3)
			{
				CSevenSignSky pck(nVersion);
				L2Server::BroadcastToAllUser(pck.GetSize(), pck.GetBuff());
				pUser->SendSystemMessage(L"Command accepted.");
			}else
				pUser->SendSystemMessage(L"Usage: //set_ssq_sky [0 Normal - 1 Dusk - 2 Dawn]");
		}else
			pUser->SendSystemMessage(L"Usage: //set_ssq_sky [0 Normal - 1 Dusk - 2 Dawn]");
	}
	return false;
}

bool CBuilderCommand::SetClientVersion(User *pUser, wstring wsParams)
{
	if(pUser->ValidUser())
	{
		if(wsParams.size() > 0)
		{
/*
			int nVersion = 0;
			wstringstream sstr;
			sstr << wsParams;
			sstr >> nVersion;
			if(nVersion > -1 && nVersion < INT_MAX)
			{
				g_Config.HashInfo.SetClientVersion(nVersion);
				g_Config.HashInfo.Save();
				g_Log.Add(CLog::Blue, "[%s] new client version[%d]", __FUNCTION__, nVersion);
				pUser->SendSystemMessage(L"Command accepted.");
			}else
				pUser->SendSystemMessage(L"Usage: //set_client_version [0 - 2147483647]");
*/
		}else
			pUser->SendSystemMessage(L"Usage: //set_client_version [0 - 2147483647]");
	}
	return false;
}

bool CBuilderCommand::SetClientLimit(User *pUser, wstring wsParams)
{
	if(pUser->ValidUser())
	{
		if(wsParams.size() > 0)
		{
			int nLimit = 0;
			wstringstream sstr;
			sstr << wsParams;
			sstr >> nLimit;
			if(nLimit > -1 && nLimit < 500)
			{
				g_CliExt.SetBoxLimit(nLimit);
				g_Log.Add(CLog::Blue, "[%s] GM[%S] sets max allowed clients limit[%d].", __FUNCTION__, pUser->pSD->wszName, nLimit);
				pUser->SendSystemMessage(L"Command accepted.");
			}else
				pUser->SendSystemMessage(L"Usage: //set_client_limit [limit(0-500)]");
		}else
			pUser->SendSystemMessage(L"Usage: //set_client_limit [limit(0-500)]");
	}
	return false;
}

bool CBuilderCommand::ShowUsersByHwid(User *pUser, wstring wsParams)
{
	if(pUser->ValidUser())
	{
		User *pTarget = User::GetUserBySID(&pUser->targetId);
		if(pTarget->ValidUser())
		{
			vector<User*> vUsers;

			g_UserDB.GetUsersByHwid(pTarget->pSocket->pED->longHWID.hash, vUsers);

			wstringstream msgStream;
			msgStream << L"Users by HWID [" << pTarget->pSocket->pED->longHWID.ToString() << L"]:" << endl;
			for(INT32 n=0;n<vUsers.size();n++)
			{
				msgStream << vUsers[n]->pSD->wszName << endl;
			}
			pUser->SendSystemMessage(msgStream.str().c_str());
		}else
			pUser->SendSystemMessage(L"Invalid target!");
	}
	return false;
}

bool CBuilderCommand::ShowUsersByIP(User *pUser, wstring wsParams)
{
	if(pUser->ValidUser())
	{
		User *pTarget = User::GetUserBySID(&pUser->targetId);
		if(pTarget->ValidUser())
		{
			vector<User*> vUsers;
			IP ip;
			ip.full = pTarget->pSocket->addr.S_un.S_addr;

			g_UserDB.GetUsersByIp(ip.full, vUsers);

			wstringstream msgStream;
			msgStream << L"Users by IP [" << (UINT)ip.part[0] << L"." << (UINT)ip.part[1] << L"." << (UINT)ip.part[2] << L"." << (UINT)ip.part[3] << L"]:" << endl;
			for(INT32 n=0;n<vUsers.size();n++)
			{
				msgStream << vUsers[n]->pSD->wszName << endl;
			}
			pUser->SendSystemMessage(msgStream.str().c_str());
		}else
			pUser->SendSystemMessage(L"Invalid target!");
	}
	return false;
}

bool CBuilderCommand::ShowUsersByIPnHwid(User *pUser, wstring wsParams)
{
	if(pUser->ValidUser())
	{
		User *pTarget = User::GetUserBySID(&pUser->targetId);
		if(pTarget->ValidUser())
		{
			vector<User*> vUsers;
			IP ip;
			ip.full = pTarget->pSocket->addr.S_un.S_addr;

			g_UserDB.GetUsersByIpHwid(ip.full, pTarget->pSocket->pED->longHWID.hash, vUsers);

			wstringstream msgStream;
			msgStream << L"Users by IP [" << (UINT)ip.part[0] << L"." << (UINT)ip.part[1] << L"." << (UINT)ip.part[2] << L"." << (UINT)ip.part[3] << L"] and HWID [" << pTarget->pSocket->pED->longHWID.ToString() << L"] :" << endl;
			for(INT32 n=0;n<vUsers.size();n++)
			{
				msgStream << vUsers[n]->pSD->wszName << endl;
			}
			pUser->SendSystemMessage(msgStream.str().c_str());
		}else
			pUser->SendSystemMessage(L"Invalid target!");
	}
	return false;
}
bool CBuilderCommand::Who(User *pUser, wstring wsParams)
{
	if(pUser->ValidUser())
	{
		_SockFunc _who = (_SockFunc) 0x00447FF0;
		return _who(pUser->GetSocket(), NULL);
	}
	return false;
}
bool CBuilderCommand::WhoIsOnline(User *pUser, wstring wsParams)
{
	if(pUser->ValidUser())
	{
		_SockFunc _who = (_SockFunc) 0x00447FF0;
		return _who(pUser->GetSocket(), NULL);
	}
	return false;
}
bool CBuilderCommand::PlayEventSound(User *pUser, wstring wsParams)
{
	if(pUser->ValidUser())
	{
		User *pTarget = User::GetUserBySID(&pUser->targetId);
		if(pTarget->ValidUser())
		{
			pTarget->PlaySound(L"systemmsg_e.l2s_event", 1, pTarget->nObjectID, pTarget->pSD->Pos.x, pTarget->pSD->Pos.y, pTarget->pSD->Pos.z);
			pUser->PlaySound(L"systemmsg_e.l2s_event", 1, pUser->nObjectID, pUser->pSD->Pos.x, pUser->pSD->Pos.y, pUser->pSD->Pos.z);
		}else
			pUser->GetSocket()->SendSystemMessage(L"Invalid target");
	}
	return false;
}
bool CBuilderCommand::Announce2(User *pUser, wstring wsParams)
{
	if(pUser->ValidUser() && wsParams.size() > 1)
	{
		wstring wTxt = wsParams.substr(1, (wsParams.size() - 1));
		CHAR pck[8190];
		int nSize = Assemble(pck, 8190, "cddSS", 0x4A, 0, 10, L"", wTxt.c_str());
		L2Server::BroadcastToAllUser(nSize, pck);
	}
	return false;
}
bool CBuilderCommand::SetHero(User *pUser, wstring wsParams)
{
	if(pUser->ValidUser())
	{
		int nType = 0;
		wstringstream sstr;
		sstr << wsParams;
		sstr >> nType;

		User *pTarget = User::GetUserBySID(&pUser->targetId);
		if(pTarget->ValidUser())
		{
			g_DB.RequestSaveHeroType(pTarget, nType);
			
			if(nType == 2)
			{
				g_HeroSystem.AcquireHeroSkills(pUser);
			}
			pUser->GetSocket()->SendSystemMessage(L"Command Accepted.");
		}else
			pUser->GetSocket()->SendSystemMessage(SystemMessage::INVALID_TARGET_);	
	}
	return false;
}
bool CBuilderCommand::DelHero(User *pUser, wstring wsParams)
{
	if(pUser->ValidUser())
	{
		User *pTarget = User::GetUserBySID(&pUser->targetId);
		if(pTarget->ValidUser())
		{
			g_DB.RequestRemoveHero(pTarget->nDBID);
			pUser->GetSocket()->SendSystemMessage(L"Command Accepted.");
		}else
			pUser->GetSocket()->SendSystemMessage(SystemMessage::INVALID_TARGET_);
	}
	return false;
}
bool CBuilderCommand::SetNobless(User *pUser, wstring wsParams)
{
	if(pUser->ValidUser())
	{
		User *pTarget = User::GetUserBySID(&pUser->targetId);
		if(pTarget->ValidUser())
		{
			g_DB.RequestSaveNoblessType(pTarget, 1);
		}else
			pUser->GetSocket()->SendSystemMessage(SystemMessage::INVALID_TARGET_);
	}
	return false;
}
bool CBuilderCommand::DelClanSkills(User *pUser, wstring wsParams)
{
	if(pUser->ValidUser())
	{
		User *pTarget = User::GetUserBySID(&pUser->targetId);
		if(pTarget->ValidUser())
		{
			for(UINT n=370; n<392; n++)
			{
				pTarget->DeleteSkill(n);
			}
			pTarget->ValidateParameters();
		}else
			pUser->GetSocket()->SendSystemMessage(SystemMessage::INVALID_TARGET_);
	}
	return false;
}
bool CBuilderCommand::ResetReuse(User *pUser, std::wstring wsParams)
{
	if(pUser->ValidUser())
	{
		User *pTarget = User::GetUserBySID(&pUser->targetId);
		if(pTarget->ValidUser())
		{
			try
			{
				pTarget->ResetSkillReuse();
			}catch(...)
			{
				g_Log.Add(CLog::Error, "[%s] Exception Detected.", __FUNCTION__);
			}
		}else
			pUser->GetSocket()->SendSystemMessage(SystemMessage::INVALID_TARGET_);
	}
	return false;
}
bool CBuilderCommand::AddReputation(User *pUser, std::wstring wsParams)
{
	if(pUser->ValidUser())
	{
		if(wsParams.size() > 2)
		{
			wstring wsPledge;
			int nPoints = 0;
			wstringstream sstr;
			sstr << wsParams;
			sstr >> wsPledge;
			sstr >> nPoints;
			if(nPoints)
			{
				CObjectSP ObjectSP;
				CObjectSP::GetPledgeByName(&ObjectSP, (PWCHAR)wsPledge.c_str());
				if(ObjectSP.pObject != 0)
				{
					CPledge *pPledge = ObjectSP.pObject->CastPledge();
					if(pPledge && pPledge->pSD)
					{
						pPledge->UpdatePledgeNameValue(nPoints);
					}
					pUser->SendSystemMessage(L"Command Accepted.");
				}else
					pUser->SendSystemMessage(L"AddReputation: Pledge cannot be found.");
			}
		}else
			pUser->SendSystemMessage(L"Usage: //add_reputation [pledge_name] [points]");
	}
	return false;
}
bool CBuilderCommand::RedSky(User *pUser, std::wstring wsParams)
{
	if(pUser->ValidUser())
	{
		if(wsParams.size() > 0)
		{
			wstringstream sstr;
			sstr << wsParams;
			int nSecond;
			sstr >> nSecond;
			if(nSecond > 0)
			{
				char buff[8190];
				int len = Assemble(buff, 8190, "chd", 0xFE, 0x40, nSecond);
				L2Server::BroadcastToAllUser(len, buff);
				pUser->SendSystemMessage(L"Command Accepted.");
			}else
				pUser->SendSystemMessage(L"RedSky: Invalid Time (seconds)");
		}else
			pUser->SendSystemMessage(L"Usage: //redsky [time_in_seconds]");
	}
	return false;
};
bool CBuilderCommand::SetKarma(User *pUser, std::wstring wsParams)
{
	if(pUser->ValidUser())
	{
		if(wsParams.size() > 0)
		{
			wstringstream sstr;
			sstr << wsParams;
			int nKarma;
			sstr >> nKarma;
			if(nKarma >= 0)
			{
				User *pTarget = User::GetUserBySID(&pUser->targetId);
				if(pTarget->ValidUser())
				{
					pTarget->SetKarma(nKarma);
					pTarget->SendCharInfo(false);
					pTarget->SendUserInfo(false);
					pUser->SendSystemMessage(L"Command Accepted.");
				}else
					pUser->SendSystemMessage(L"Invalid target!");
			}else
				pUser->SendSystemMessage(L"SetKarma: Invalid Karma ( karma < 0)");
		}else
			pUser->SendSystemMessage(L"Usage: //setkarma [karma]");
	}
	return false;
};
bool CBuilderCommand::Healthy(User *pUser, std::wstring wsParams)
{
	if(pUser->ValidUser())
	{
		User *pTarget = User::GetUserBySID(&pUser->targetId);
		if(pTarget->ValidUser())
		{
			try
			{
				CreatureSharedData *pSD = pTarget->pSD;
				if(pSD)
				{
					pSD->fCP = pSD->MaxCP;
					pSD->fHP = pSD->MaxHP;
					pSD->fMP = pSD->MaxMP;
				}
			}catch(...)
			{
				g_Log.Add(CLog::Error, "[%s] Exception Detected.", __FUNCTION__);
			}
			pTarget->SendHpInfo();
			pTarget->SendMpInfo();
		}else
			pUser->GetSocket()->SendSystemMessage(SystemMessage::INVALID_TARGET_);
	}
	return false;
}
bool CBuilderCommand::ResetCursedWeapon(User *pUser, std::wstring wsParams)
{
	if(pUser->ValidUser())
	{
		if(wsParams.size() > 1)
		{
			wstringstream sstr;
			sstr << wsParams;
			int nWeaponID;
			sstr >> nWeaponID;
			if(g_CursedWeaponSystem.IsCursedWeapon(nWeaponID))
			{
				g_CursedWeaponSystem.RequestReset(pUser, nWeaponID);
				pUser->SendSystemMessage(L"Command Accepted.");
			}
		}else
			pUser->SendSystemMessage(L"Usage: //cursed_weapon_reset [weapon_id]");
	}
	return false;
}
bool CBuilderCommand::CursedWeaponInfo(User *pUser, std::wstring wsParams)
{
	if(pUser->ValidUser())
	{
		if(wsParams.size() > 1)
		{
			wstringstream sstr;
			sstr << wsParams;
			int nWeaponID;
			sstr >> nWeaponID;
			if(g_CursedWeaponSystem.IsCursedWeapon(nWeaponID))
			{
				g_CursedWeaponSystem.RequestInfo(pUser, nWeaponID);
				pUser->SendSystemMessage(L"Command Accepted.");
			}
		}else
			pUser->SendSystemMessage(L"Usage: //cursed_weapon_info [weapon_id]");
	}
	return false;
}

bool CBuilderCommand::TestCMD(User *pUser, std::wstring wsParams)
{
	if(pUser->ValidUser())
	{
		pUser->pSocket->Send("cddS", 0x87, 11, 1, L"CF3614CF075AA0CB");
	}
	return false;
}

bool CBuilderCommand::PartyRecall(User *pUser, std::wstring wsParams)
{
	if(pUser->ValidUser())
	{
		if(wsParams.size() > 1)
		{
			wstringstream sstr;
			sstr << wsParams;
			wstring wsName;
			sstr >> wsName;
			if(wsName.size() > 0)
			{
				User *pTarget = g_UserDB.GetUserByName(wsName);
				if(pTarget->ValidUser())
				{
					CParty *pParty = pUser->GetParty();
					if(pParty)
					{
						UserVector vUsers;
						pParty->GetAllMembersEx(vUsers);
						UserVector::iterator Iter = vUsers.begin();
						for(;Iter!=vUsers.end();Iter++)
						{
							User *pMember = *Iter;
							if(pMember->ValidUser())
							{
								pMember->TeleportToLocation((int)pUser->pSD->Pos.x, (int)pUser->pSD->Pos.y, (int)pUser->pSD->Pos.z, true);
							}
						}
						pUser->SendSystemMessage(L"Command Accepted.");
					}else
					{
						pUser->SendSystemMessage(L"User has no party!");
						pTarget->TeleportToLocation((int)pUser->pSD->Pos.x, (int)pUser->pSD->Pos.y, (int)pUser->pSD->Pos.z, true);
					}
				}else
					pUser->SendSystemMessage(L"User cannot be found!");
			}
		}else
			pUser->SendSystemMessage(L"Usage: //party_recall [user_name]");
	}
	return false;
}

bool CBuilderCommand::AugmentItem(User *pUser, std::wstring wsParams)
{
	//augment_item [item_id] [enchant_level] [aug1_id] [aug2_id]
	if(pUser->ValidUser())
	{
		if(wsParams.size() > 6)
		{
			User *pTarget = User::GetUserBySID(&pUser->targetId);
			if(!pTarget->ValidUser())
			{
				pUser->SendSystemMessage(L"Invalid target!");
				return false;
			}

			int nItemID, nAug1ID, nAug2ID, nEnchant;
			wstringstream sstr;
			sstr << wsParams;
			sstr >> nItemID >> nEnchant >> nAug1ID >> nAug2ID;
			if(nItemID > 0)
			{
				if(nAug1ID > -1 && nAug1ID < 16345 && nAug2ID > -1 && nAug2ID < 16345)
				{
					int nAugmentationID = (nAug2ID << 16) + nAug1ID;
					CItem *pItem = pTarget->inventory.GetFirstItemByClassID(nItemID);
					if(pItem->IsValid(VT_ITEMWEAPON))
					{
						if(pItem->nAugmentationID == 0 && pItem->pSID->nEnchantLevel == nEnchant)
						{
							pItem->nAugmentationID = nAugmentationID;
							g_DB.RequestSaveItemDataEx(pTarget->nDBID, pItem);
							pTarget->inventory.SetInventoryChanged(pItem, CInventory::UPDATE);
						}else
						{
							//find our item
							int nSafeCheck = 0;
							while((pItem->nAugmentationID != 0 || pItem->pSID->nEnchantLevel != nEnchant) && nSafeCheck < 120)
							{
								pItem = pTarget->inventory.GetNextItemByClassID(pItem->pSID->nItemIndex);
								if(!pItem->IsValidItem())
									break;
								nSafeCheck++;
							}
							
							if(pItem->IsValid(VT_ITEMWEAPON))
							{
								pItem->nAugmentationID = nAugmentationID;
								g_DB.RequestSaveItemDataEx(pTarget->nDBID, pItem);
								pTarget->inventory.SetInventoryChanged(pItem, CInventory::UPDATE);
							}else
								pUser->SendSystemMessage(L"Cannot find valid weapon!");
						}
					}else
						pUser->SendSystemMessage(L"Cannot find valid weapon!");
				}else
					pUser->SendSystemMessage(L"Invalid augmentation id!");
			}else
				pUser->SendSystemMessage(L"Invalid item id!");
		}else
			pUser->SendSystemMessage(L"Usage: //augment_item [item_id] [enchant] [aug1_id] [aug2_id]");
	}
	return false;
}

bool CBuilderCommand::SetPendingWriteLimit(User *pUser, std::wstring wsParams)
{
	if(pUser->ValidUser())
	{
		if(wsParams.size() > 1)
		{
			int nLimit = 0;
			wstringstream sstr;
			sstr << wsParams;
			sstr >> nLimit;
			SocketLimiter::SetPendingWriteLimit(nLimit);
			pUser->SendSystemMessage(L"Command Accepted!");
		}else
		{
			pUser->SendSystemMessage(L"Invalid Param!");
		}
	}
	return false;
}
bool CBuilderCommand::CreateTeam(User *pUser, std::wstring wsParams)
{
	//create_team - generates new team id
	if(pUser->ValidUser())
	{
		int nTeamID = g_TeamContainer.GenerateNewTeam();
		if(nTeamID != -1)
		{
			wstring wMsg = L"New empty team has been generated. ID: ";
			WCHAR wNr[10];
			wMsg += _itow(nTeamID, wNr, 10);
			pUser->GetSocket()->SendSystemMessage((PWCHAR)wMsg.c_str());
		}else
			pUser->GetSocket()->SendSystemMessage(L"Error: team_id = -1");
	}
	return false;
}
bool CBuilderCommand::DeleteTeam(User *pUser, std::wstring wsParams)
{
	if(pUser->ValidUser())
	{
		if(wsParams.size() > 1)
		{
			int nTeamID = 0;
			wstringstream sstr;
			sstr << wsParams;
			sstr >> nTeamID;
			g_TeamContainer.DelTeam(pUser->GetSocket(), nTeamID);
		}else
			pUser->GetSocket()->SendSystemMessage(L"Usage: //delete_team [team_id]");
	}
	return false;
}
bool CBuilderCommand::ClearTeam(User *pUser, std::wstring wsParams)
{
	if(pUser->ValidUser())
	{
		if(wsParams.size() > 1)
		{
			int nTeamID = 0;
			wstringstream sstr;
			sstr << wsParams;
			sstr >> nTeamID;
			g_TeamContainer.ClearTeam(pUser->GetSocket(), nTeamID);
		}else
			pUser->GetSocket()->SendSystemMessage(L"Usage: //delete_team [team_id]");
	}
	return false;
}
bool CBuilderCommand::TeamList(User *pUser, std::wstring wsParams)
{
	if(pUser->ValidUser())
	{
		if(wsParams.size() > 1)
		{
			int nTeamID = 0;
			wstringstream sstr;
			sstr << wsParams;
			sstr >> nTeamID;
			g_TeamContainer.ViewUserList(pUser->GetSocket(), nTeamID);
		}else
		{
			g_TeamContainer.ViewTeamList(pUser->GetSocket());
		}
	}
	return false;
}
bool CBuilderCommand::DelUserFromTeam(User *pUser, std::wstring wsParams)
{
	if(pUser->ValidUser())
	{
		if(wsParams.size() > 1)
		{
			int nTeamID = 0;
			wstringstream sstr;
			sstr << wsParams;
			sstr >> nTeamID;
			User *pTarget = pUser->GetTarget()->GetUser();
			if(pTarget->ValidUser())
			{
				g_TeamContainer.DelUser(pUser->GetSocket(), nTeamID, pTarget);
			}else
				pUser->GetSocket()->SendSystemMessage(L"Invalid Target!");
		}else
		{
			User *pTarget = pUser->GetTarget()->GetUser();
			if(pTarget->ValidUser())
			{
				g_TeamContainer.DelUser(pUser->GetSocket(), pTarget);
			}else
				pUser->GetSocket()->SendSystemMessage(L"Invalid Target!");
		}
	}
	return false;
}
bool CBuilderCommand::AddUserToTeam(User *pUser, std::wstring wsParams)
{
	if(pUser->ValidUser())
	{
		if(wsParams.size() > 1)
		{
			int nTeamID = 0;
			wstringstream sstr;
			sstr << wsParams;
			sstr >> nTeamID;
			User *pTarget = pUser->GetTarget()->GetUser();
			if(pTarget->ValidUser())
			{
				g_TeamContainer.AddUser(pUser->GetSocket(), nTeamID, pTarget);
			}else
				pUser->GetSocket()->SendSystemMessage(L"Invalid Target!");
		}else
			pUser->GetSocket()->SendSystemMessage(L"Usage: //add_to_team [team_id]");
	}
	return false;
}
bool CBuilderCommand::MessageWithParamToTeam(User *pUser, std::wstring wsParams)
{
	if(pUser->ValidUser())
	{
		if(wsParams.size() > 4)
		{
			int nTeamID = 0, nMsgID, nParam;
			wstringstream sstr;
			sstr << wsParams;
			sstr >> nTeamID >> nMsgID >> nParam;
			g_TeamContainer.SendSystemMessage(nTeamID, nMsgID, nParam);
		}else
			pUser->GetSocket()->SendSystemMessage(L"Usage: //message_param_team [team_id] [message_id] [param_int]");
	}
	return false;
}
bool CBuilderCommand::MessageToTeam(User *pUser, std::wstring wsParams)
{
	if(pUser->ValidUser())
	{
		if(wsParams.size() > 2)
		{
			int nTeamID = 0, nMsgID;
			wstringstream sstr;
			sstr << wsParams;
			sstr >> nTeamID >> nMsgID;
			g_TeamContainer.SendSystemMessage(nTeamID, nMsgID);
		}else
			pUser->GetSocket()->SendSystemMessage(L"Usage: //message_team [team_id] [message_id]");
	}
	return false;
}
bool CBuilderCommand::HTMessage(User *pUser, std::wstring wsParams)
{
/*
2373    1    The Hero Tournament will begin in $s1 hour(s) and $s2 minute(s).    0    B2    B0    5A    FF            0    0    0    0    0        none
2374    1    The Hero Tournament will begin in $s1 hour(s) and $s2 minute(s). We advice you to read the Hero Tournament Rules once more and to prepare your inventory for battle.    0    B2    B0    5A    FF            0    0    0    0    0        none
*/
	if(pUser->ValidUser())
	{
		if(wsParams.size() > 2)
		{
			int nHours = 0, nMinutes = 0;
			wstringstream sstr;
			sstr << wsParams;
			sstr >> nHours >> nMinutes;
			if(nHours > 2)
			{
				CSystemMessage msg(2373);
				msg.AddNumber(nHours);
				msg.AddNumber(nMinutes);
				L2Server::BroadcastToAllUser(msg.GetSize(), msg.GetBuff());
			}else
			{
				CSystemMessage msg(2374);
				msg.AddNumber(nHours);
				msg.AddNumber(nMinutes);
				L2Server::BroadcastToAllUser(msg.GetSize(), msg.GetBuff());
			}
		}else
			pUser->GetSocket()->SendSystemMessage(L"Usage: //ht [hours] [minutes]");
	}
	return false;
}