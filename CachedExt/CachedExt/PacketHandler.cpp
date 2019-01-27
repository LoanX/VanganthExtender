#include "CachedExt.h"
#include "Query.h"
#include <algorithm>

DWORD _SendReplyToLoadPledgeExit = 0x004951BC;
DWORD _SendReplyToLoadPledgeExit2 = 0x00495356;

BOOL g_SubStack = FALSE;

void __declspec(naked) SendRepltyToLoadPledgeLanucher()
{
	__asm
	{
		mov edx, [esp+4]
		push edx
		push ecx
		call PacketHandler::SendReplyToLoadPledge
		add esp, 0Ch
		jmp _SendReplyToLoadPledgeExit
	}
}

void __declspec(naked) SendRepltyToLoadPledgeLanucher2()
{
	__asm
	{
		mov edx, [esp+4]
		push edx
		push ecx
		call PacketHandler::SendReplyToLoadPledge
		add esp, 0Ch
		jmp _SendReplyToLoadPledgeExit2
	}
}

QueryCallback g_CustomPacketHandler[256];

BOOL g_LastTaxUpdateDateFix = FALSE;
//EXEC lin_SetLastTaxUpdate '%d-%d-%d %d:%d:%d', '%d-%d-%d %d:%d:%d', '%d-%d-%d %d:%d:%d'

bool ExecuteLastTaxUpdate(DBConn* lpCon, const WCHAR* wProc, int year0, int month0, int day0, int hour0, int minute0, int second0, int year1, int month1, int day1, int hour1, int minute1, int second1, int year2, int month2, int day2, int hour2, int minute2, int second2)
{
	month0++;
	month1++;
	month2++;

	return lpCon->Execute(wProc, year0, month0, day0, hour0, minute0, second0, year1, month1, day1, hour1, minute1, second1, year2, month2, day2, hour2, minute2, second2);
}

void PacketHandler::Initialize()
{
	TCHAR File[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,File);
	lstrcat(File, L"\\l2cache.ini");
	g_LastTaxUpdateDateFix = GetPrivateProfileInt(_T("Setting"), _T("TaxUpdateDateFix"), 0, File);
	g_SubStack = GetPrivateProfileInt(_T("Setting"), _T("SubStack"), 0, File);

	UINT adminPort = GetPrivateProfileInt(_T("World"), _T("AdminPort"), 2012, File);
	CachedExt::WriteMemoryDWORD(0x424DFE, adminPort);
	UINT port1 = GetPrivateProfileInt(_T("World"), _T("Port1"), 2008, File);
	CachedExt::WriteMemoryDWORD(0x424DD0, port1);
	UINT port2 = GetPrivateProfileInt(_T("World"), _T("Port2"), 2006, File);
	CachedExt::WriteMemoryDWORD(0x424DE7, port2);

	//.text:00501D80                 push    offset RequestQueryPacket
	CachedExt::WriteMemoryDWORD(0x501D81, (DWORD)PacketHandler::Handler); //overwriting address, our function will be called instead of RequestQueryPacket from cached
	CachedExt::SetHook(0xE8, 0x491B84, (DWORD)DisassembleSaveItemPacket, 0);
	CachedExt::SetHook(0xE8, 0x4B8A8F, (DWORD)AssembleItemPacket, 0);
	CachedExt::SetHook(0xE8, 0x4951B7, (DWORD)SendRepltyToLoadPledgeLanucher, 0); //Reply load by pledge id
	CachedExt::SetHook(0xE8, 0x495351, (DWORD)SendRepltyToLoadPledgeLanucher2, 0);	//Reply load by pledge name
	CachedExt::SetHook(0xE8, 0x424D76, (DWORD)ExecuteProceduresOnStart, 0);
	CachedExt::WriteMemoryDWORD(0x4FFF61, (DWORD)SaveCharacterPacket);
	CachedExt::SetHook(0xE8, 0x498AE1, (DWORD)AssembleLoadCharacterPacket, 0);
	if(g_LastTaxUpdateDateFix)
	{
		CachedExt::SetHook(0xE8, 0x466100, (DWORD)ExecuteLastTaxUpdate, 0);
	}
	
	for(UINT n=0;n<256;n++)
	{
		g_CustomPacketHandler[n] = PacketHandler::DummyCustomPacket;
	}

	Bind(CI_SET_PLEDGE_PRIVILEGE, RequestSetPledgePrivilege);
	Bind(CI_SET_PLEDGE_MEMBER_RANK, RequestSetPledgeMemberRank);
	Bind(CI_REORGANIZE_MEMBER, RequestReorganizeMember);
	Bind(CI_SET_PLEDGE_REPUTATION, RequestSetPledgeReputation);
	Bind(CI_SET_USER_PLEDGE_TYPE_N_RANK, RequestSetUserPledgeTypeAndRank);
	Bind(CI_SET_SUB_CAPTAIN, RequestSetSubCaptain);
	Bind(CI_CREATE_SUBPLEDGE, RequestCreateSubPledge);
	Bind(CI_RENAME_SUBPLEDGE, RequestRenameSubPledge);
	Bind(CI_REQUEST_ACQUIRE_PLEDGE_SKILL, RequestAcquirePledgeSkill);
	Bind(CI_CREATE_EXT_PLEDGE_INFO, RequestAddPledge);
	Bind(CI_SET_SPONSOR, RequestSetSponsor);
	Bind(CI_SAVE_JOIN_PLEDGE_LEVEL, RequestSaveJoinPledgeLevel);
	Bind(CI_SAVE_ITEM_DATA_EX, RequestSaveItemDataEx);
	Bind(CI_REQUEST_DELEGATE_CLAN_MASTER, RequestDelegateClanMaster);
	Bind(CI_SAVE_PLEDGE_STAT, RequestSavePledgeStat);
	Bind(CI_SAVE_PRIVATE_STORE_INFO, RequestSetPrivateStore);
	Bind(CI_REQUEST_LOAD_SPIRIT_COUNT, RequestLoadSpiritCount);
	Bind(CI_SAVE_BUFF_LIST, RequestSaveBuffList);
	Bind(CI_REQUEST_LOAD_BUFF_LIST, RequestLoadBuffList);
	Bind(CI_SAVE_KILL_DEATH_STAT, RequestSaveKillDeathStat);
	Bind(CI_REQUEST_ENCHANT_ITEM, RequestEnchantItem);
	Bind(CI_REQUEST_SAVE_AIO, RequestSaveAIO);
	Bind(CI_REQUEST_LOAD_AIO, RequestLoadAIO);
	Bind(CI_REQUEST_DELETE_ITEMS, RequestDeleteItems);
	Bind(CI_REQUEST_RESTORE_CH, RequestRestoreCH);
	Bind(CI_REQUEST_LOAD_NPC_DEATH, RequestLoadNpcDeath);
	Bind(CI_REQUEST_SAVE_NPC_DEATH, RequestSaveNpcDeath);
	Bind(CI_REQUEST_LOAD_VIP_STATUS, RequestLoadVipStatus);
	Bind(CI_REQUEST_SAVE_VIP_STATUS, RequestSaveVipStatus);
	Bind(CI_REQUEST_LOAD_VIP_ACCOUNTS, RequestLoadVIPAccounts);
	Bind(CI_REQUEST_LOAD_VOTE_POINTS, RequestLoadVotePoints);
	Bind(CI_REQUEST_SAVE_VOTE_POINTS, RequestSaveVotePoints);
	Bind(CI_REQUEST_BAN_ACCOUNT, RequestBanAccount);
	Bind(CI_REQUEST_LOAD_BANNED_ACCOUNTS, RequestLoadBannedAccounts);
	Bind(CI_REQUEST_SAVE_MULTISELL_LOG, RequestSaveMultiSellLog);
	Bind(CI_REQUEST_LOCK_ACCOUNT, RequestLockAccount);
	Bind(CI_REQUEST_UNLOCK_ACCOUNT, RequestUnlockAccount);
	Bind(CI_REQUEST_LOCK_CHAR, RequestLockCharacter);
	Bind(CI_REQUEST_UNLOCK_CHAR, RequestUnlockCharacter);
	Bind(CI_REQUEST_LOAD_HWID_LOCK_DATA, RequestLoadHwidLockData);
	Bind(CI_REQUEST_SAVE_CHARACTER_HWID, RequestSaveCharacterHwid);
	Bind(CI_REQUEST_BAN_HWID, RequestBanHwid);
	Bind(CI_REQUEST_LOAD_BANNED_HWID, RequestLoadBannedHwid);
	Bind(CI_REQUEST_SET_ADDITIONAL_BOX, RequestSetAdditionalBox);
	Bind(CI_REQUEST_LOAD_ADDITIONAL_BOX_DATA, RequestLoadAdditionalBoxData);
	Bind(CI_REQUEST_SAVE_USER_FRACTION, RequestSaveUserFraction);
	Bind(CI_REQUEST_LOAD_USER_FRACTION, RequestLoadUserFraction);
	Bind(CI_REQUEST_ITEM_DELIVERY_ASK, RequestItemDeliveryAsk);
	Bind(CI_REQUEST_ITEM_DELIVERY_DONE, RequestItemDeliveryDone);
	Bind(CI_REQUEST_SAVE_FRACTION_DATA, RequestSaveFractionData);
	Bind(CI_REQUEST_LOAD_FRACTION_DATA, RequestLoadFractionData);
	Bind(CI_REQUEST_SAVE_SUBJOB, RequestSaveSubJob);
	Bind(CI_REQUEST_CREATE_AUCTION, RequestCreateAuction);
	Bind(CI_REQUEST_MODDIFY_AUCTION_AMOUNT, RequestModdifyAuctionAmount);
	Bind(CI_REQUEST_DELETE_AUCTION, RequestDeleteAuction);
	Bind(CI_REQUEST_LOAD_AUCTION_ITEMS, RequestLoadAuctionItems);
	Bind(CI_REQUEST_AUCTION_PAYMENT_ASK, RequestAuctionPaymentAsk);
	Bind(CI_REQUEST_AUCTION_PAYMENT_DONE, RequestAuctionPaymentDone);
	Bind(CI_REQUEST_AUCTION_ADD_PAYMENT_JOB, RequestAuctionAddPaymentJob);
	Bind(CI_REQUEST_ADD_PLEDGE_SKILL, RequestAddPledgeSkill);
	Bind(CI_REQUEST_LOAD_PLEDGE_SKILLS, RequestLoadPledgeSkills);
	Bind(CI_REQUEST_LOAD_WEDDING_STATUS, RequestLoadWeddingStatus);
	Bind(CI_REQUEST_SAVE_WEDDING_STATUS, RequestSaveWeddingStatus);
	Bind(CI_REQUEST_TEST_QUERY, RequestTestQuery);
	Bind(CI_REQUEST_LOAD_RB_STATUS, RequestLoadRaidBossStatus);
	Bind(CI_REQUEST_LOAD_PK_STATUS, RequestLoadPKStatus);
	Bind(CI_REQUEST_LOAD_PVP_STATUS, RequestLoadPvPStatus);
	Bind(CI_REQUEST_LOAD_BANNED_HARDWARE_ID, RequestLoadBannedHardwareId);
	Bind(CI_REQUEST_SAVE_HARDWARE_ID, RequestSaveHardwareId);
	Bind(CI_REQUEST_BAN_HARDWARE_ID, RequestBanHardwareId);
	Bind(CI_REQUEST_UNBAN_HARDWARE_ID, RequestUnBanHardwareId);
	Bind(CI_REQUEST_LOAD_NAME_TITLE_COLOR, RequestLoadNameTitleColor);
	Bind(CI_REQUEST_LOAD_CHARACTER_LOCK, RequestLoadCharacterLock);
	Bind(CI_REQUEST_SAVE_CHARACTER_LOCK, RequestSaveCharacterLock);
	Bind(CI_REQUEST_SAVE_DAILY_PVP, RequestSaveDailyPvP);
	Bind(CI_REQUEST_SAVE_DAILY_PVP_ENTRY, RequestSaveDailyPvPEntry);
	Bind(CI_REQUEST_LOAD_DAILY_PVP_ENTRY, RequestLoadDailyPvPEntry);
	Bind(CI_REQUEST_LOAD_DAILY_PVP, RequestLoadDailyPvP);
	Bind(CI_REQUEST_SAVE_SUBSTACK, RequestSaveSubStack);
	Bind(CI_REQUEST_LOAD_SUBSTACK, RequestLoadSubStack);
	Bind(CI_REQUEST_SAVE_INSTANCE_TIME, RequestSaveInstanceTime);
	Bind(CI_REQUEST_LOAD_INSTANCE_TIME, RequestLoadInstanceTime);
	Bind(CI_REQUEST_LOAD_L2NETWORK_VOTE, RequestLoadL2NetworkVote);
	Bind(CI_REQUEST_SAVE_L2NETWORK_VOTE, RequestSaveL2NetworkVote);
	Bind(CI_REQUEST_LOAD_VISUAL_ARMOR, RequestLoadVisualArmor);
	Bind(CI_REQUEST_SAVE_VISUAL_ARMOR, RequestSaveVisualArmor);
	Bind(CI_REQUEST_LOAD_REBIRTH, RequestLoadRebirth);
	Bind(CI_REQUEST_SAVE_REBIRTH, RequestSaveRebirth);
	Bind(CI_REQUEST_LOAD_DB_NPC_MAKER, RequestLoadDBNpcMaker);
	Bind(CI_REQUEST_SET_CHARACTER_PIN, RequestSetCharacterPIN);
	Bind(CI_REQUEST_LOAD_CHARACTER_PINS, RequestLoadCharacterPINS);
	Bind(CI_REQUEST_LOAD_CLAN_PVP_STATUS, RequestLoadPledgeTotalPvP);


	CachedExt::SetHook(0xE8, 0x4436C9, (DWORD)PacketHandler::LogOnItemSetAmount, 0);
	CachedExt::SetHook(0xE8, 0x4CA3CD, (DWORD)PacketHandler::OnItemSetAmount, 0);
	CachedExt::SetHook(0xE8, 0x4CA659, (DWORD)PacketHandler::OnItemSetAmount, 0);
	CachedExt::SetHook(0xE8, 0x4CA87F, (DWORD)PacketHandler::OnItemSetAmount, 0);
	CachedExt::SetHook(0xE8, 0x4CA8BC, (DWORD)PacketHandler::OnItemSetAmount, 0);
	CachedExt::SetHook(0xE8, 0x4CAB04, (DWORD)PacketHandler::OnItemSetAmount, 0);
}

void PacketHandler::LogOnItemSetAmount(PVOID pInstance, UINT logType, const WCHAR* format)
{
	g_Log.AddU(CLog::Error, format);
	g_Log.CallStack();
}

void PacketHandler::OnItemSetAmount(CItem *pItem, INT32 amount)
{
	guard;
	if(amount >= 0)
	{
		pItem->nAmount = amount;
	}else
	{
		UINT uAmount = (UINT)amount;
		uAmount -= pItem->nAmount;

		g_Log.Add(CLog::Error, "[%s] Trying to set invalid amount [%d] for item[%d][%d] baseAmount[%d] trying to add [%d]!", __FUNCTION__, amount, pItem->nItemClassID, pItem->nItemID, pItem->nAmount, uAmount);
	}
	unguard;
}

void PacketHandler::Bind(UINT id, QueryCallback cb)
{
	if(id < 256)
	{
		g_CustomPacketHandler[id] = cb;
	}
}

bool __cdecl PacketHandler::Handler(CQuerySocket *pSocket, const unsigned char* packet)
{
	guard;
	bool ret = false;
	WORD subId;
	const unsigned char* newPacket = Disassemble(packet, "h", &subId);
	if(subId > 1)
	{
		if(subId < 256)
		{
			ret = g_CustomPacketHandler[subId](pSocket, newPacket);
		}
	}else
	{
		typedef bool (__cdecl * f)(CQuerySocket*, const unsigned char*);
		ret = f(0x466880L)(pSocket, packet);
	}
	unguard;
	return ret;
}

void PacketHandler::ExecuteProceduresOnStart()
{
	typedef void (*f)();
	f CItem_Init = (f) 0x442800;
	CItem_Init();
	
	//our procedures
	g_Log.AddU(CLog::Black, L"[Ext] Delegating clan masters...");
	DBConn *pDBConn = new DBConn;
	pDBConn->Execute(L"EXEC lin_RunDelegateClanMaster");
	delete pDBConn;
	
	g_Log.AddU(CLog::Black, L"[Ext] Clan masters are delegated.");
	if(CConfig::GetInstance().IsSet(CConfig::PRIVATE_STORE_IN_DB))
	{
		g_Log.AddU(CLog::Black, L"[Ext] Clearing Private Store table...");
		pDBConn = new DBConn;
		int nCount = 0;
		pDBConn->Bind(&nCount);
		if(pDBConn->Execute(L"SELECT COUNT(*) FROM PrivateStore"))
		{
			if(pDBConn->Fetch())
			{
				if(nCount > 0)
				{
					DBConn *pDBConn2 = new DBConn();
					pDBConn2->Execute(L"DELETE FROM PrivateStore WHERE char_id > 0");
					delete pDBConn2;
				}
			}
		}
		delete pDBConn;
		
		g_Log.AddU(CLog::Black, L"[Ext] Private Store table is cleared.");
	}
}

bool __cdecl PacketHandler::DummyCustomPacket(CQuerySocket *pSocket, const unsigned char* packet)
{
	guard;
	g_Log.Add(CLog::Error, "[%s] called!", __FUNCTION__);
	unguard;
	return false;
};

bool PacketHandler::RequestSetCharacterPIN(CQuerySocket *pSocket, const unsigned char* packet)
{
	guard;

	UINT charId = 0, pin = 0, timestamp = 0;
	Disassemble(packet, "ddd", &charId, &pin, &timestamp);
	
	CQuery query(L"EXEC lin_SetPIN ?, ?, ?");
	query.AddParam(charId);
	query.AddParam(pin);
	query.AddParam(timestamp);
	DBConn *pCon = new DBConn();
	pCon->Execute(query.Get());
	delete pCon;

	unguard;
	return false;
}

bool PacketHandler::RequestLoadCharacterPINS(CQuerySocket *pSocket, const unsigned char* packet)
{
	guard;

	struct PinData
	{
		UINT charId;
		UINT pin;
		UINT timestamp;
	};

	vector<PinData> data;
	PinData pd;
	DBConn *pCon = new DBConn();
	pCon->Bind(&pd.charId);
	pCon->Bind(&pd.pin);
	pCon->Bind(&pd.timestamp);
	if(pCon->Execute(L"EXEC lin_LoadPINS"))
	{
		while(pCon->Fetch())
		{
			data.push_back(pd);
			pd.charId = pd.pin = pd.timestamp = 0;
		}
	}
	delete pCon;

	for(UINT n=0;n<data.size();n++)
	{
		pSocket->Send("cdddd", 0x22, CO_REPLY_LOAD_CHARACTER_PIN, data[n].charId, data[n].pin, data[n].timestamp);
	}
	unguard;
	return false;
}

bool PacketHandler::RequestLoadDBNpcMaker(CQuerySocket *pSocket, const unsigned char* packet)
{
	guard;

	CQuery query(L"EXEC lin_LoadDBNpcMaker");

	//[npc_id], [spawn_x], [spawn_y], [spawn_z], [heading], [start_time], [end_time]
	struct DBNpc
	{
		INT32 npcId, x, y, z, heading, startTime, endTime;
	};

	DBNpc npc;

	vector<DBNpc> npcs;

	DBConn *pCon = new DBConn();

	pCon->Bind(&npc.npcId);
	pCon->Bind(&npc.x);
	pCon->Bind(&npc.y);
	pCon->Bind(&npc.z);
	pCon->Bind(&npc.heading);
	pCon->Bind(&npc.startTime);
	pCon->Bind(&npc.endTime);

	if(pCon->Execute(query.Get()))
	{
		while(pCon->Fetch())
		{
			npcs.push_back(npc);
		}
	}

	delete pCon;

	for(UINT n=0;n<npcs.size();n++)
	{
		DBNpc& t = npcs[n];
		pSocket->Send("cdddddddd", 0x22, CO_REPLY_LOAD_DB_NPC_MAKER, t.npcId, t.x, t.y, t.z, t.heading, t.startTime, t.endTime);
	}

	unguard;
	return false;
}

bool PacketHandler::RequestSaveRebirth(CQuerySocket *pSocket, const unsigned char* packet)
{
	guard;

	UINT charId = 0, subjobId = 0;
	INT32 rebirthCount = 0, availablePoints = 0, bonusInt = 0, bonusStr = 0, bonusCon = 0, bonusMen = 0, bonusDex = 0, bonusWit = 0;

	Disassemble(packet, "dddddddddd", &charId, &subjobId, &rebirthCount, &availablePoints, &bonusInt, &bonusStr, &bonusCon, &bonusMen, &bonusDex, &bonusWit);

	CQuery query(L"EXEC lin_SaveRebirth ?, ?, ?, ?, ?, ?, ?, ?, ?, ?");
	query.AddParam(charId);
	query.AddParam(subjobId);
	query.AddParam(rebirthCount);
	query.AddParam(availablePoints);
	query.AddParam(bonusInt);
	query.AddParam(bonusStr);
	query.AddParam(bonusCon);
	query.AddParam(bonusMen);
	query.AddParam(bonusDex);
	query.AddParam(bonusWit);

	DBConn *pCon = new DBConn();

	pCon->Execute(query.Get());

	delete pCon;

	unguard;
	return false;
}


bool PacketHandler::RequestLoadRebirth(CQuerySocket *pSocket, const unsigned char* packet)
{
	guard;

	UINT charId = 0, subjobId = 0;
	Disassemble(packet, "dd", &charId, &subjobId);

	INT32 rebirthCount = 0, availablePoints = 0, bonusInt = 0, bonusStr = 0, bonusCon = 0, bonusMen = 0, bonusDex = 0, bonusWit = 0;

	CQuery query(L"SELECT [rebirth_count], [available_points], [bonus_int], [bonus_str], [bonus_con], [bonus_men], [bonus_dex], [bonus_wit] FROM [user_rebirth] WHERE [char_id] = ? AND [subjob_id] = ?");
	query.AddParam(charId);
	query.AddParam(subjobId);

	DBConn *pCon = new DBConn();

	pCon->Bind(&rebirthCount);
	pCon->Bind(&availablePoints);
	pCon->Bind(&bonusInt);
	pCon->Bind(&bonusStr);
	pCon->Bind(&bonusCon);
	pCon->Bind(&bonusMen);
	pCon->Bind(&bonusDex);
	pCon->Bind(&bonusWit);

	if(pCon->Execute(query.Get()))
	{
		pCon->Fetch();
	}

	delete pCon;

	if(rebirthCount > 0)
	{
		pSocket->Send("cddddddddddd", 0x22, CO_REPLY_LOAD_REBIRTH, charId, subjobId, rebirthCount, availablePoints, bonusInt, bonusStr, bonusCon, bonusMen, bonusDex, bonusWit);
	}

	unguard;
	return false;
}

bool PacketHandler::RequestLoadVisualArmor(CQuerySocket *pSocket, const unsigned char* packet)
{
	guard;

	UINT charId = 0;
	Disassemble(packet, "d", &charId);

	CQuery query(L"SELECT [visual_armor_id] FROM [user_data] WHERE [char_id] = ?");
	query.AddParam(charId);

	UINT armorId = 0;

	DBConn *pCon = new DBConn();

	pCon->Bind(&armorId);

	if(pCon->Execute(query.Get()))
	{
		pCon->Fetch();
	}

	if(armorId > 0)
	{
		pSocket->Send("cddd", 0x22, CO_REPLY_LOAD_VISUAL_ARMOR, charId, armorId);
	}

	delete pCon;

	unguard;
	return false;
}


bool PacketHandler::RequestSaveVisualArmor(CQuerySocket *pSocket, const unsigned char* packet)
{
	guard;
	UINT charId = 0, armorId = 0;
	Disassemble(packet, "dd", &charId, &armorId);

	CQuery query(L"UPDATE [user_data] SET [visual_armor_id] = ? WHERE [char_id] = ?");
	query.AddParam(armorId);
	query.AddParam(charId);

	DBConn *pCon = new DBConn;
	pCon->Execute(query.Get());
	delete pCon;

	unguard;
	return false;
}

bool PacketHandler::RequestSaveL2NetworkVote(CQuerySocket *pSocket, const unsigned char* packet)
{
	guard;

	WCHAR wIdentity[33] = { 0 };
	UINT voteTime = 0, rewardTime = 0;
	Disassemble(packet, "Sdd", sizeof(wIdentity), wIdentity, &voteTime, &rewardTime);

	CQuery query(L"EXEC lin_SaveL2NetworkVote '?', ?, ?");
	query.AddParam(wIdentity);
	query.AddParam(voteTime);
	query.AddParam(rewardTime);

	DBConn *pCon = new DBConn;
	pCon->Execute(query.Get());
	delete pCon;

	unguard;
	return false;
}


bool PacketHandler::RequestLoadL2NetworkVote(CQuerySocket *pSocket, const unsigned char* packet)
{
	guard;

	UINT charId = 0;
	WCHAR wIdentity[33] = { 0 };
	Disassemble(packet, "dS", &charId, sizeof(wIdentity), wIdentity);

	CQuery query(L"EXEC lin_LoadL2NetworkVote '?'");
	query.AddParam(wIdentity);

	UINT lastVoteTime = 0, lastRewardTime = 0;

	DBConn *pCon = new DBConn();

	pCon->Bind(&lastVoteTime);
	pCon->Bind(&lastRewardTime);

	if(pCon->Execute(query.Get()))
	{
		pCon->Fetch();
	}

	pSocket->Send("cddSdd", 0x22, CO_REPLY_LOAD_L2NETWORK_VOTE, charId, wIdentity, lastVoteTime, lastRewardTime);

	delete pCon;

	unguard;
	return false;
}

bool PacketHandler::RequestLoadInstanceTime(CQuerySocket *pSocket, const unsigned char* packet)
{
	guard;

	UINT charId = 0;
	Disassemble(packet, "d", &charId);

	CQuery query(L"SELECT [instance_id], [time_spent], [reuse_timeout] FROM [user_instance] WHERE [char_id] = ?");
	query.AddParam(charId);

	struct InstanceInfo
	{
		UINT instanceId;
		UINT spentTime;
		UINT reuseTimeout;
	};

	vector<InstanceInfo> instances;

	InstanceInfo ii;
	DBConn *pCon = new DBConn();

	pCon->Bind(&ii.instanceId);
	pCon->Bind(&ii.spentTime);
	pCon->Bind(&ii.reuseTimeout);

	if(pCon->Execute(query.Get()))
	{
		while(pCon->Fetch())
		{
			instances.push_back(ii);
		}
	}

	delete pCon;

	for(UINT n=0;n<instances.size();n++)
	{
		InstanceInfo& ii = instances[n];
		pSocket->Send("cddddd", 0x22, CO_REPLY_LOAD_INSTANCE_TIME, charId, ii.instanceId, ii.spentTime, ii.reuseTimeout);
	}

	unguard;
	return false;
}

bool PacketHandler::RequestSaveInstanceTime(CQuerySocket *pSocket, const unsigned char* packet)
{
	guard;

	UINT charId = 0, instanceId = 0, spentTime = 0, reuseTimeout = 0;
	Disassemble(packet, "dddd", &charId, &instanceId, &spentTime, &reuseTimeout);

	CQuery query(L"EXEC lin_SaveInstanceTime ?, ?, ?, ?");
	query.AddParam(charId);
	query.AddParam(instanceId);
	query.AddParam(spentTime);
	query.AddParam(reuseTimeout);

	DBConn *pCon = new DBConn();

	pCon->Execute(query.Get());

	delete pCon;

	unguard;
	return false;
}

bool PacketHandler::RequestLoadSubStack(CQuerySocket *pSocket, const unsigned char* packet)
{
	guard;

	UINT charId = 0;
	Disassemble(packet, "d", &charId);

	CQuery query(L"SELECT [baseClass0], [baseClass1], [baseClass2], [baseClass3], [stackClass0], [stackClass1], [stackClass2], [stackClass3] FROM [SubStack] WHERE [char_id] = ?");
	query.AddParam(charId);

	UINT classId[4][2] = { 0 };

	DBConn *pCon = new DBConn();

	for(UINT n=0;n<4;n++)
	{
		pCon->Bind(&classId[n][0]);
	}
	for(UINT n=0;n<4;n++)
	{
		pCon->Bind(&classId[n][1]);
	}

	bool send = false;
	if(pCon->Execute(query.Get()))
	{
		if(pCon->Fetch())
		{
			send = true;
		}
	}

	delete pCon;

	if(send)
	{	
		pSocket->Send("cdddddddddd", 0x22, CO_REPLY_LOAD_SUBSTACK, charId, classId[0][0], classId[1][0], classId[2][0], classId[3][0], classId[0][1], classId[1][1], classId[2][1], classId[3][1] );
	}

	unguard;
	return false;
}

bool PacketHandler::RequestSaveSubStack(CQuerySocket *pSocket, const unsigned char* packet)
{
	guard;

	UINT charId = 0, index = 0, baseClass = 0, stackClass = 0;
	Disassemble(packet, "dddd", &charId, &index, &baseClass, &stackClass);

	CQuery query(L"EXEC lin_SaveSubStack ?, ?, ?, ?");
	query.AddParam(charId);
	query.AddParam(index);
	if(baseClass == UINT_MAX)
	{
		query.AddParam(-1);
		query.AddParam(-1);
	}else
	{
		query.AddParam(baseClass);
		query.AddParam(stackClass);
	}

	DBConn *pCon = new DBConn();

//	g_Log.Add(CLog::Blue, "[%s] Saving for [%d] [%d][%d][%d]", __FUNCTION__, charId, index, baseClass, stackClass);

	pCon->Execute(query.Get());

	delete pCon;

	unguard;
	return false;
}

bool PacketHandler::RequestLoadDailyPvP(CQuerySocket* pSocket, const unsigned char* packet)
{
	guard;
	UINT charId = 0;
	Disassemble(packet, "d", &charId);

	CQuery query(L"SELECT [daily_pvp], [daily_pvp_timestamp] FROM [user_data] WHERE [char_id] = ?");
	query.AddParam(charId);
	UINT timestamp = 0;
	INT32 points = 0;

	DBConn *pCon = new DBConn();
	pCon->Bind(&points);
	pCon->Bind(&timestamp);
	if(pCon->Execute(query.Get()))
	{
		pCon->Fetch();
	}
	delete pCon;

	pSocket->Send("cdddd", 0x22, CO_REPLY_LOAD_DAILY_PVP, charId, points, timestamp);

	unguard;
	return false;
}

bool PacketHandler::RequestSaveDailyPvPEntry(CQuerySocket* pSocket, const unsigned char* packet)
{
	guard;
	UINT charId = 0, timestamp = 0;
	BYTE hwid[32];
	Disassemble(packet, "dbd", &charId, 32, hwid, &timestamp);

	WCHAR wHwid[33] = { 0 };
	wHwid[32] = 0;
	for(UINT n=0;n<32;n++)
	{
		wHwid[n] = hwid[n];
	}

	if(charId == 0)
	{
		//clean up
		CQuery query(L"DELETE FROM [DailyPvP]");
		DBConn *pCon = new DBConn();

		pCon->Execute(query.Get());

		delete pCon;
	}else
	{
		CQuery query(L"EXEC lin_AddDailyPvPEntry ?, '?', ?");
		query.AddParam(charId);
		query.AddParam(wHwid);
		query.AddParam(timestamp);

		DBConn *pCon = new DBConn();

		pCon->Execute(query.Get());

		delete pCon;
	}

	unguard;
	return false;
}

bool PacketHandler::RequestSaveDailyPvP(CQuerySocket* pSocket, const unsigned char* packet)
{
	guard;
	UINT charId = 0, timestamp = 0, points = 0;

	Disassemble(packet, "ddd", &charId, &points, &timestamp);

	CQuery query(L"UPDATE [user_data] SET [daily_pvp] = ?, [daily_pvp_timestamp] = ? WHERE [char_id] = ?");
	query.AddParam(points);
	query.AddParam(timestamp);
	query.AddParam(charId);

	DBConn *pCon = new DBConn();

	pCon->Execute(query.Get());

	delete pCon;

	unguard;
	return false;
}

struct Packet
{
	char buff[8190];
	int len;
	int amount;
	Packet() { len = 0; amount = 0; };
};

bool PacketHandler::RequestLoadDailyPvPEntry(CQuerySocket* pSocket, const unsigned char* packet)
{
	guard;

	vector<Packet> packets;

	UINT charId = 0, timestamp = 0;
	WCHAR wHwid[33];
	memset(wHwid, 0, sizeof(wHwid));

	CQuery query(L"SELECT [char_id], [hardware_id], [timestamp] FROM [DailyPvP]");
	DBConn *pCon = new DBConn();
	pCon->Bind(&charId);
	pCon->BindS(wHwid, sizeof(wHwid));
	pCon->Bind(&timestamp);

	Packet packetEx;
	if(pCon->Execute(query.Get()))
	{
		while(pCon->Fetch())
		{
			BYTE hwid[32];
			for(UINT n=0;n<32;n++)
			{
				hwid[n] = static_cast<BYTE>(wHwid[n]);
			}
			if(packetEx.len < 8000)
			{
				packetEx.len = Assemble(&packetEx.buff[packetEx.len], 8190 - packetEx.len, "dbd", charId, 32, hwid, timestamp);
				packetEx.amount++;
			}else
			{
				packets.push_back(packetEx);
				memset(&packetEx, 0, sizeof(packetEx));
				packetEx.len = Assemble(&packetEx.buff[packetEx.len], 8190 - packetEx.len, "dbd", charId, 32, hwid, timestamp);
				packetEx.amount++;
			}
		}
		if(packetEx.len > 0)
		{
			packets.push_back(packetEx);
		}
	}

	delete pCon;
	
	//sending packets to server
	for(UINT n=0;n<packets.size();n++)
	{
		Packet& pck = packets[n];
		pSocket->Send("cddb", 0x22, CO_REPLY_LOAD_DAILY_PVP_ENTRY, pck.amount, pck.len, pck.buff);
	}
	unguard;
	return false;
}

bool PacketHandler::RequestSaveCharacterLock(CQuerySocket* pSocket, const unsigned char* packet)
{
	guard;
	UINT charId = 0;
	packet = Disassemble(packet, "d", &charId);
	WCHAR wHash[33];
	memset(wHash, 0, sizeof(wHash));
	
	for(UINT n=0;n<32;n++)
	{
		wHash[n] = packet[n];
	}

	packet = &packet[32];
	WCHAR wHWID[33];
	memset(wHWID, 0, sizeof(wHWID));

	for(UINT n=0;n<32;n++)
	{
		wHWID[n] = packet[n];
	}

	DBConn *pCon = new DBConn();

	pCon->BindS(wHash, sizeof(wHash));
	CQuery query(L"INSERT INTO [CharacterLock] ([code_hash], [hardware_id], [char_id]) VALUES ('?', '?', ?)");
	query.AddParam(wHash);
	query.AddParam(wHWID);
	query.AddParam(charId);

	pCon->Execute(query.Get());

	delete pCon;

	unguard;
	return false;
}

bool PacketHandler::RequestLoadCharacterLock(CQuerySocket* pSocket, const unsigned char* packet)
{
	guard;
	UINT charId = 0;
	Disassemble(packet, "d", &charId);
	WCHAR wHash[33];
	memset(wHash, 0, sizeof(wHash));

	DBConn *pCon = new DBConn();

	pCon->BindS(wHash, sizeof(wHash));
	CQuery query(L"SELECT [code_hash] FROM [CharacterLock] WHERE [char_id] = ?");
	query.AddParam(charId);

	if(pCon->Execute(query.Get()))
	{
		pCon->Fetch();
	}

	BYTE hash[32];
	for(UINT n=0;n<32;n++)
	{
		hash[n] = static_cast<BYTE>(wHash[n]);
	}

	pSocket->Send("cddb", 0x22, CO_REPLY_LOAD_CHARACTER_LOCK, charId, 32, hash);

	delete pCon;

	unguard;
	return false;
}

bool PacketHandler::RequestLoadNameTitleColor(CQuerySocket* pSocket, const unsigned char* packet)
{
	guard;
	UINT charId = 0;
	Disassemble(packet, "d", &charId);

	UINT nameColor = 0;
	UINT titleColor = 0;

	CQuery query(L"SELECT [name_color], [title_color] FROM [user_data] WHERE [char_id] = ? ");
	query.AddParam(charId);

	DBConn *pCon = new DBConn();
	pCon->Bind(&nameColor);
	pCon->Bind(&titleColor);
	if(pCon->Execute(query.Get()))
	{
		pCon->Fetch();
	}

	delete pCon;

	if(nameColor != 0 || titleColor != 0)
	{
		pSocket->Send("cdddd", 0x22, CO_REPLY_LOAD_NAME_TITLE_COLOR, charId, nameColor, titleColor);
	}

	unguard;
	return false;
}

bool PacketHandler::RequestLoadPvPStatus(CQuerySocket* pSocket, const unsigned char* packet)
{
	guard;

	struct PvPInfo
	{
		UINT charId;
		WCHAR wName[25];
		UINT classId;
		INT32 level;
		INT32 pvpAmount;
	};

	vector<PvPInfo> stats;
	PvPInfo pki;
	memset(&pki, 0, sizeof(pki));

	CQuery query(L"SELECT TOP(100) [char_id], [char_name], [class], [Lev], [duel] FROM [user_data] order by [duel] desc");
	DBConn *pConn = new DBConn;
	pConn->Bind(&pki.charId);
	pConn->BindS(pki.wName, sizeof(pki.wName));
	pConn->Bind(&pki.classId);
	pConn->Bind(&pki.level);
	pConn->Bind(&pki.pvpAmount);

	if(pConn->Execute(query.Get()))
	{
		while(pConn->Fetch())
		{
			stats.push_back(pki);
			memset(&pki, 0, sizeof(pki));
		}
	}
	delete pConn;

	for(UINT n=0;n<stats.size();n++)
	{
		PvPInfo& pk = stats[n];
		pSocket->Send("cddSddd", 0x22, CO_REPLY_LOAD_PVP_STATUS, pk.charId, pk.wName, pk.classId, pk.level, pk.pvpAmount);
	}

	unguard;
	return false;
}

namespace
{
	struct PledgeInfo
	{
		UINT pledgeId;
		WCHAR pledgeName[25];
		UINT pvpCount;
	};

	//from highest to lowest
	bool PledgeInfoComp(const PledgeInfo& p1, const PledgeInfo& p2)
	{
		return p1.pvpCount > p2.pvpCount;
	}
}

bool PacketHandler::RequestLoadPledgeTotalPvP(CQuerySocket* pSocket, const unsigned char* packet)
{
	guard;

	UINT pledgeLevel = 0;
	UINT topLimit = 0;
	Disassemble(packet, "dd", &pledgeLevel, &topLimit);

	vector<PledgeInfo> stats;
	PledgeInfo pi;
	memset(&pi, 0, sizeof(PledgeInfo));

	CQuery query(L"EXEC lin_GetPledgeTotalPvP ?");
	query.AddParam(pledgeLevel);


	DBConn *pConn = new DBConn;
	pConn->Bind(&pi.pledgeId);
	pConn->BindS(pi.pledgeName, sizeof(pi.pledgeName));
	pConn->Bind(&pi.pvpCount);

	if(pConn->Execute(query.Get()))
	{
		while(pConn->Fetch())
		{
			stats.push_back(pi);
			memset(&pi, 0, sizeof(PledgeInfo));
		}
	}
	delete pConn;

	std::sort(stats.begin(), stats.end(), PledgeInfoComp);

	for(UINT n=0;n<stats.size();n++)
	{
		if(n < topLimit)
		{
			PledgeInfo& pi = stats[n];
//			g_Log.Add(CLog::Blue, "[%s] pvp[%d] pledge[%S] - pledgeLevel[%d] topLimit[%d]", __FUNCTION__, pi.pvpCount, pi.pledgeName, pledgeLevel, topLimit);
			pSocket->Send("cddSd", 0x22, CO_REPLY_LOAD_CLAN_PVP_STATUS, pi.pledgeId, pi.pledgeName, pi.pvpCount);
		}
	}

	unguard;
	return false;
}

bool PacketHandler::RequestLoadPKStatus(CQuerySocket* pSocket, const unsigned char* packet)
{
	guard;

	struct PKInfo
	{
		UINT charId;
		WCHAR wName[25];
		UINT classId;
		INT32 level;
		INT32 pkAmount;
	};

	vector<PKInfo> stats;
	PKInfo pki;
	memset(&pki, 0, sizeof(pki));

	CQuery query(L"SELECT TOP(100) [char_id], [char_name], [class], [Lev], [PK] FROM [user_data] order by [PK] desc");
	DBConn *pConn = new DBConn;
	pConn->Bind(&pki.charId);
	pConn->BindS(pki.wName, sizeof(pki.wName));
	pConn->Bind(&pki.classId);
	pConn->Bind(&pki.level);
	pConn->Bind(&pki.pkAmount);

	if(pConn->Execute(query.Get()))
	{
		while(pConn->Fetch())
		{
			stats.push_back(pki);
			memset(&pki, 0, sizeof(pki));
		}
	}
	delete pConn;

	for(UINT n=0;n<stats.size();n++)
	{
		PKInfo& pk = stats[n];
		pSocket->Send("cddSddd", 0x22, CO_REPLY_LOAD_PK_STATUS, pk.charId, pk.wName, pk.classId, pk.level, pk.pkAmount);
	}

	unguard;
	return false;
}

bool PacketHandler::RequestUnBanHardwareId(CQuerySocket* pSocket, const unsigned char* packet)
{
	guard;
	WCHAR wsHwid[33];
	memset(wsHwid, 0, sizeof(wsHwid));

	for(UINT n=0;n<32;n++)
	{
		wsHwid[n] = (*packet);
		packet++;
	}

	CQuery query(L"EXEC lin_UnBanHardwareId '?'");
	query.AddParam(wsHwid);
	DBConn *pCon = new DBConn();
	pCon->Execute(query.Get());
	delete pCon;

	unguard;
	return false;
}

bool PacketHandler::RequestBanHardwareId(CQuerySocket* pSocket, const unsigned char* packet)
{
	guard;
	WCHAR wsHwid[33];
	memset(wsHwid, 0, sizeof(wsHwid));
	WCHAR account[25] = { 0 };
	WCHAR gm[25] = { 0 };

	for(UINT n=0;n<32;n++)
	{
		wsHwid[n] = (*packet);
		packet++;
	}
	UINT timeout = 0;
	Disassemble(packet, "SSd", sizeof(account), account, sizeof(gm), gm, &timeout);

	CQuery query(L"EXEC lin_BanHardwareId  '?', '?', '?', ?");
	query.AddParam(wsHwid);
	query.AddParam(account);
	query.AddParam(gm);
	query.AddParam(timeout);

	DBConn *pCon = new DBConn();
	pCon->Execute(query.Get());
	delete pCon;

	unguard;
	return false;
}

bool PacketHandler::RequestSaveHardwareId(CQuerySocket* pSocket, const unsigned char* packet)
{
	guard;
	WCHAR wsHwid[33];
	memset(wsHwid, 0, sizeof(wsHwid));
	UINT charId = 0;
	packet = Disassemble(packet, "d", &charId);

	for(UINT n=0;n<32;n++)
	{
		wsHwid[n] = (*packet);
		packet++;
	}

	CQuery query(L"EXEC lin_SaveHardwareId ?, '?'");
	query.AddParam(charId);
	query.AddParam(wsHwid);
	DBConn *pCon = new DBConn();
	pCon->Execute(query.Get());
	delete pCon;

	unguard;
	return false;
}

bool PacketHandler::RequestLoadBannedHardwareId(CQuerySocket* pSocket, const unsigned char* packet)
{
	guard;

	CQuery query(L"EXEC lin_LoadBannedHardwareId");

	struct BHI
	{
		WCHAR wsHWID[33];
		WCHAR account[25];
		WCHAR gm[25];
		UINT timeout;
	};

	BHI bhi;
	memset(&bhi, 0, sizeof(bhi));

	vector<BHI> bans;

	DBConn *pCon = new DBConn();
	pCon->BindS(bhi.wsHWID, 66);
	pCon->BindS(bhi.account, 50);
	pCon->BindS(bhi.gm, 50);
	pCon->Bind(&bhi.timeout);
	if(pCon->Execute(query.Get()))
	{
		while(pCon->Fetch())
		{
			bans.push_back(bhi);
			memset(&bhi, 0, sizeof(bhi));
		}
	}
	delete pCon;

	for(UINT n=0;n<bans.size();n++)
	{
		BHI& bh = bans[n];
		BYTE hwid[32];
		for(UINT m=0;m<32;m++)
		{
			hwid[m] = static_cast<BYTE>(bh.wsHWID[m]);
		}
		pSocket->Send("cdbSSd", 0x22, CO_REPLY_LOAD_BANNED_HARDWARE_ID, 32, hwid, bh.account, bh.gm, bh.timeout);
	}

	unguard;
	return false;
}

bool PacketHandler::RequestLoadRaidBossStatus(CQuerySocket* pSocket, const unsigned char* packet)
{
	guard;

	struct RaidBossInfo
	{
		WCHAR wName[128];
		int x;
		int y;
		int z;
		BOOL alive;
		UINT deathTime;
	};

	vector<RaidBossInfo> bosses;
	RaidBossInfo rbi;
	memset(&rbi, 0, sizeof(rbi));

	CQuery query(L"SELECT [npc_db_name], [pos_x], [pos_y], [pos_z], [alive], [time_low] FROM [dbo].[npc_boss]");
	DBConn *pConn = new DBConn;
	pConn->BindS(rbi.wName, sizeof(rbi.wName));
	pConn->Bind(&rbi.x);
	pConn->Bind(&rbi.y);
	pConn->Bind(&rbi.z);
	pConn->Bind(&rbi.alive);
	pConn->Bind(&rbi.deathTime);

	if(pConn->Execute(query.Get()))
	{
		while(pConn->Fetch())
		{
			bosses.push_back(rbi);
			memset(&rbi, 0, sizeof(rbi));
		}
	}
	delete pConn;

	for(UINT n=0;n<bosses.size();n++)
	{
		RaidBossInfo& rb = bosses[n];
		pSocket->Send("cdSddddd", 0x22, CO_REPLY_LOAD_RB_STATUS, rb.wName, rb.x, rb.y, rb.z, rb.alive, rb.deathTime);
	}

	unguard;
	return false;
}

bool PacketHandler::RequestTestQuery(CQuerySocket* pSocket, const unsigned char* packet)
{
	guard;
	UINT startTick = 0, startTime = 0;
	UINT receiveTick = GetTickCount(), receiveTime = static_cast<UINT>(time(0));
	Disassemble(packet, "dd", &startTick, &startTime);

	UINT userCount = 0;
	CQuery query(L"SELECT COUNT(*) FROM user_data");
	DBConn *pConn = new DBConn;
	pConn->Bind(&userCount);
	if(pConn->Execute(query.Get()))
	{
		pConn->Fetch();
	}
	delete pConn;
	

	pSocket->Send("cddddddd", 0x22, CO_REPLY_TEST_QUERY, startTick, startTime, receiveTick, receiveTime, GetTickCount(), time(0));
	unguard;
	return false;
}

bool PacketHandler::RequestSaveWeddingStatus(CQuerySocket *pSocket, const unsigned char* packet)
{
	guard;
	UINT charId, state, partnerId, penaltyTimeout, teleportTimeout, weddingTime = 0;
	WCHAR partnerName[25];
	Disassemble(packet, "dddSddd", &charId, &state, &partnerId, sizeof(partnerName), partnerName, &penaltyTimeout, &teleportTimeout, &weddingTime);
	if(charId > 0)
	{
		CQuery query(L"EXEC lin_SaveWeddingStatus ?, ?, ?, '?', ?, ?, ?");
		query.AddParam(charId);
		query.AddParam(state);
		query.AddParam(partnerId);
		query.AddParam(partnerName);
		query.AddParam(penaltyTimeout);
		query.AddParam(teleportTimeout);
		query.AddParam(weddingTime);

		DBConn *pConn = new DBConn;

		pConn->Execute(query.Get());

		delete pConn;

	}
	unguard;
	return false;
}

bool PacketHandler::RequestLoadWeddingStatus(CQuerySocket *pSocket, const unsigned char *packet)
{
	guard;

	UINT charId = 0;
	Disassemble(packet, "d", &charId);
	if(charId > 0)
	{
		UINT partnerId = 0;
		WCHAR partnerName[25] = { 0 };
		UINT state, penaltyTimeout, teleportTimeout, weddingTime = 0;
		CQuery query(L"EXEC lin_LoadWeddingStatus ?");
		query.AddParam(charId);
		DBConn *pConn = new DBConn();
		pConn->Bind(&state);
		pConn->Bind(&partnerId);
		pConn->BindS(partnerName, sizeof(partnerName));
		pConn->Bind(&penaltyTimeout);
		pConn->Bind(&teleportTimeout);
		pConn->Bind(&weddingTime);
		bool done = false;
		if(pConn->Execute(query.Get()))
		{
			if(pConn->Fetch())
			{
				done = true;
			}
		}
		delete pConn;
		if(done)
		{
			pSocket->Send("cddddSddd", 0x22, CO_REPLY_LOAD_WEDDING_STATUS, charId, state, partnerId, partnerName, penaltyTimeout, teleportTimeout, weddingTime);
		}
	}
	unguard;
	return false;
}



bool PacketHandler::RequestLoadPledgeSkills(CQuerySocket *pSocket, const unsigned char* packet)
{
	guard;

	UINT pledgeId = 0;
	Disassemble(packet, "d", &pledgeId);
	if(pledgeId > 0)
	{
		CHAR buff[8190];
		int len = 0;
		int skillCnt = 0;
		CQuery query(L"EXEC lin_LoadPledgeSkills ?");
		query.AddParam(pledgeId);
		DBConn* pConn = new DBConn;
		int skillId = 0, skillLevel = 0;
		pConn->Bind(&skillId);
		pConn->Bind(&skillLevel);
		if(pConn->Execute(query.Get()))
		{
			while(pConn->Fetch())
			{
				if(skillId > 0 && skillLevel > 0)
				{
					if(skillCnt < 200)
					{
						skillCnt++;
						len += Assemble(&buff[len], sizeof(buff) - len, "dd", skillId, skillLevel);
						skillId = skillLevel = 0;
					}else
					{
						g_Log.Add(CLog::Error, "[%s] Too many pledge skills - 200 is the limit - pledgeId[%d]", __FUNCTION__, pledgeId);
					}
				}
			}
		}
		if(len > 0)
		{
			pSocket->Send("cdddb", 0x22, CO_REPLY_LOAD_PLEDGE_SKILLS, pledgeId, skillCnt, len, buff);
		}
		delete pConn;
	}
	unguard;
	return false;
}

bool PacketHandler::RequestAddPledgeSkill(CQuerySocket *pSocket, const unsigned char* packet)
{
	guard;
	UINT pledgeId = 0;
	int skillId = 0, level = 0;
	Disassemble(packet, "ddd", &pledgeId, &skillId, &level);

	if(pledgeId > 0 && skillId > 0 && level > 0)
	{
		CQuery query(L"EXEC lin_AddPledgeSkill ?, ?, ?");
		query.AddParam(pledgeId);
		query.AddParam(skillId);
		query.AddParam(level);
		DBConn *pConn = new DBConn;
		pConn->Execute(query.Get());

		delete pConn;
	}
	unguard;
	return false;
}

bool PacketHandler::RequestAuctionAddPaymentJob(CQuerySocket *pSocket, const unsigned char* packet)
{
	guard;
	UINT charId = 0;
	INT32 priceId = 0, priceAmount = 0, itemId = 0, itemAmount = 0;
	WCHAR buyer[25];
	Disassemble(packet, "dddddS", &charId, &priceId, &priceAmount, &itemId, &itemAmount, sizeof(buyer), buyer);

	CQuery query(L"EXEC lin_AuctionAddPaymentJob ?, ?, ?, ?, ?, '?'");
	query.AddParam(charId);
	query.AddParam(priceId);
	query.AddParam(priceAmount);
	query.AddParam(itemId);
	query.AddParam(itemAmount);
	query.AddParam(buyer);

	DBConn *pCon = new DBConn();
	pCon->Execute(query.Get());
	delete pCon;

	g_Logger.Add(L"Adding payment job for char[%d][%d][%d][%d][%d][%s]", charId, priceId, priceAmount, itemId, itemAmount, buyer);
	unguard;
	return false;
}

bool PacketHandler::RequestAuctionPaymentDone(CQuerySocket *pSocket, const unsigned char* packet)
{
	guard;
	UINT charId = 0, jobId = 0;
	Disassemble(packet, "dd", &charId, &jobId);
	if(charId > 0 && jobId > 0)
	{
		CQuery query(L"EXEC lin_AuctionPaymentDone ?, ?");
		query.AddParam(charId);
		query.AddParam(jobId);

		DBConn *pCon = new DBConn;
		pCon->Execute(query.Get());
		delete pCon;
		g_Logger.Add(L"Auction Payment done - user[%d] job[%d]", charId, jobId);
	}
	unguard;
	return false;
}

bool PacketHandler::RequestAuctionPaymentAsk(CQuerySocket *pSocket,const unsigned char* packet)
{
	guard;
	UINT charId = 0;
	Disassemble(packet, "d", &charId);
	if(charId > 0)
	{
		CQuery query(L"EXEC lin_AuctionPaymentAsk ?");
		query.AddParam(charId);

		struct AuctionPaymentInfo
		{
			UINT jobId;
			UINT charId;
			INT32 priceId;
			INT32 priceAmount;
			INT32 itemId;
			INT32 itemAmount;
			WCHAR buyer[25];
		};

		AuctionPaymentInfo api;
		memset(&api, 0, sizeof(api));

		vector<AuctionPaymentInfo> payments;

		DBConn *pCon = new DBConn;
		pCon->Bind(&api.jobId);
		pCon->Bind(&api.charId);
		pCon->Bind(&api.priceId);
		pCon->Bind(&api.priceAmount);
		pCon->Bind(&api.itemId);
		pCon->Bind(&api.itemAmount);
		pCon->BindS(api.buyer, sizeof(api.buyer));

		if(pCon->Execute(query.Get()))
		{
			while(pCon->Fetch())
			{
				payments.push_back(api);
				memset(&api, 0, sizeof(api));
			}
		}
		delete pCon;

		for(UINT n=0;n<payments.size();n++)
		{
			pSocket->Send("cdddddddS", 0x22, CO_REPLY_AUCTION_PAYMENT_ASK, payments[n].jobId, payments[n].charId, payments[n].priceId, payments[n].priceAmount, payments[n].itemId, payments[n].itemAmount, payments[n].buyer);
		}
	}
	unguard;
	return false;
}

bool PacketHandler::RequestModdifyAuctionAmount(CQuerySocket *pSocket, const unsigned char* packet)
{
	guard;
	UINT auctionId = 0;
	INT32 newAmount = 0;
	Disassemble(packet, "dd", &auctionId, &newAmount);
	if(auctionId > 0)
	{
		if(newAmount > 0)
		{
			CQuery query(L"EXEC lin_AuctionModdifyAmount ?, ?");
			query.AddParam(auctionId);
			query.AddParam(newAmount);
			DBConn *pCon = new DBConn();
			pCon->Execute(query.Get());
			delete pCon;
		}
	}
	unguard;
	return false;
}

bool PacketHandler::RequestDeleteAuction(CQuerySocket *pSocket, const unsigned char* packet)
{
	guard;
	UINT auctionId = 0;
	Disassemble(packet, "d", &auctionId);
	if(auctionId > 0)
	{
		CQuery query(L"EXEC lin_AuctionDelete ?");
		query.AddParam(auctionId);
		DBConn *pCon = new DBConn();
		pCon->Execute(query.Get());
		delete pCon;
	}
	unguard;
	return false;
}

bool PacketHandler::RequestLoadAuctionItems(CQuerySocket *pSocket, const unsigned char* packet)
{
	guard;
	CQuery query(L"EXEC lin_AuctionLoadItems");
	struct AuctionItem
	{
		UINT sellerId;
		WCHAR sellerName[25];
		INT32 itemId;
		INT32 amount;
		INT32 enchant;
		UINT augmentation;
		INT32 price;
		UINT expireTime;
	};

	map<UINT, AuctionItem> items;

	UINT auctionId = 0;
	AuctionItem ai;
	DBConn *pCon = new DBConn;
	pCon->Bind(&auctionId);
	pCon->Bind(&ai.sellerId);
	pCon->BindS(ai.sellerName, sizeof(ai.sellerName));
	pCon->Bind(&ai.itemId);
	pCon->Bind(&ai.amount);
	pCon->Bind(&ai.enchant);
	pCon->Bind(&ai.augmentation);
	pCon->Bind(&ai.price);
	pCon->Bind(&ai.expireTime);

	if(pCon->Execute(query.Get()))
	{
		while(pCon->Fetch())
		{
			items.insert(pair<UINT, AuctionItem>(auctionId, ai));
		}
	}
	delete pCon;

	if(items.size() > 0)
	{
		for(map<UINT, AuctionItem>::iterator it = items.begin(); it != items.end(); it++)
		{
			pSocket->Send("cddSddddddd", 0x22, CO_REPLY_LOAD_AUCTION_ITEM, it->second.sellerId, it->second.sellerName, it->first, it->second.itemId, it->second.amount, it->second.enchant, it->second.augmentation, it->second.price, it->second.expireTime);
		}
	}
	unguard;
	return false;
}

bool PacketHandler::RequestCreateAuction(CQuerySocket *pSocket, const unsigned char* packet)
{
	guard;
	UINT sellerId = 0, augmentation = 0, expireTime = 0;
	WCHAR sellerName[25];
	INT32 itemId = 0, amount = 0, enchant = 0, price = 0;
	Disassemble(packet, "dSdddddd", &sellerId, sizeof(sellerName), sellerName, &itemId, &amount, &enchant, &augmentation, &price, &expireTime);
	UINT auctionId = 0;

	CQuery query(L"EXEC lin_AuctionCreate ?, '?', ?, ?, ?, ?, ?, ?");
	query.AddParam(sellerId);
	query.AddParam(sellerName);
	query.AddParam(itemId);
	query.AddParam(amount);
	query.AddParam(enchant);
	query.AddParam(augmentation);
	query.AddParam(price);
	query.AddParam(expireTime);

	DBConn* pConn = new DBConn();
	pConn->Bind(&auctionId);
	if(pConn->Execute(query.Get()))
	{
		pConn->Fetch();
	}
	delete pConn;

	pSocket->Send("cddSddddddd", 0x22, CO_REPLY_CREATE_AUCTION, sellerId, sellerName, auctionId, itemId, amount, enchant, augmentation, price, expireTime);
	unguard;
	return false;
}

bool PacketHandler::RequestSaveSubJob(CQuerySocket *pSocket, const unsigned char* packet)
{
	guard;
	UINT charId = 0;
	INT32 subjobId = 0, henna1 = 0, henna2 = 0, henna3 = 0, exp = 0, sp = 0, level = 0;
	double hp = 0, mp = 0;
	Disassemble(packet, "ddffdddddd", &charId, &subjobId, &hp, &mp, &exp, &sp, &level, &henna1, &henna2, &henna3);
	if(charId > 0)
	{
		DBConn *pConn = new DBConn();
		CQuery query(L"EXEC lin_SaveSubjob ?, ?, ?, ?, ?, ?, ?, ?, ?, ?");
		query.AddParam(charId);
		query.AddParam(subjobId);
		query.AddParam(hp);
		query.AddParam(mp);
		query.AddParam(sp);
		query.AddParam(exp);
		query.AddParam(level);
		query.AddParam(henna1);
		query.AddParam(henna2);
		query.AddParam(henna3);
		pConn->Execute(query.Get());
		delete pConn;
	}
//	Send("chdddffdddddd", 0xF9, 2, REQUEST_SAVE_SUBJOB, charId, subjobId, hp, mp, exp, sp, level, henna1, henna2, henna3);
	unguard;
	return false;
}

bool PacketHandler::RequestSaveFractionData(CQuerySocket *pSocket, const unsigned char* packet)
{
	guard;
	UINT fractionId = 0;
	INT32 points = 0, onlineMembers = 0;
	Disassemble(packet, "ddd", &fractionId, &points, &onlineMembers);
	if(fractionId > 0)
	{
		DBConn *pCon = new DBConn;
		pCon->Execute(L"EXEC lin_SafeFractionData %d, %d, %d", fractionId, points, onlineMembers);
		delete pCon;
	}
	unguard;
	return false;
}

bool PacketHandler::RequestLoadFractionData(CQuerySocket *pSocket, const unsigned char* packet)
{
	guard;
	UINT fractionId = 0;
	Disassemble(packet, "d", &fractionId);
	if(fractionId > 0)
	{
		DBConn *pCon = new DBConn();
		INT32 points = 0;
		pCon->Bind(&points);
		if(pCon->Execute(L"EXEC lin_LoadFractionData %d", fractionId))
		{
			pCon->Fetch();
		}
		delete pCon;

		pSocket->Send("cddd", 0x22, CO_REPLY_LOAD_FRACTION_DATA, fractionId, points);
	}
	unguard;
	return false;
}

bool PacketHandler::RequestItemDeliveryDone(CQuerySocket *pSocket, const unsigned char* packet)
{
	guard;
	UINT charId = 0, jobId = 0;
	Disassemble(packet, "dd", &charId, &jobId);
	if(charId > 0 && jobId > 0)
	{
		CQuery query(L"EXEC lin_ItemDeliveryDone ?, ?");
		query.AddParam(charId);
		query.AddParam(jobId);

		DBConn *pCon = new DBConn;
		pCon->Execute(query.Get());
		delete pCon;
		g_Logger.Add(L"Item Delivery done - user[%d] job[%d]", charId, jobId);
	}
	unguard;
	return false;
}

bool PacketHandler::RequestItemDeliveryAsk(CQuerySocket *pSocket,const unsigned char* packet)
{
	guard;
	UINT charId = 0;
	Disassemble(packet, "d", &charId);
	if(charId > 0)
	{
		CQuery query(L"EXEC lin_ItemDeliveryAsk ?");
		query.AddParam(charId);

		struct ItemDeliveryInfo
		{
			UINT charId;
			UINT jobId;
			INT32 itemId;
			INT32 itemAmount;
			INT32 enchant;
		};

		ItemDeliveryInfo idf = { 0 };
		vector<ItemDeliveryInfo> items;

		DBConn *pCon = new DBConn;
		pCon->Bind(&idf.jobId);
		pCon->Bind(&idf.charId);
		pCon->Bind(&idf.itemId);
		pCon->Bind(&idf.itemAmount);
		pCon->Bind(&idf.enchant);

		if(pCon->Execute(query.Get()))
		{
			while(pCon->Fetch())
			{
				items.push_back(idf);
				idf.itemId = 0;
				idf.itemAmount = 0;
				idf.jobId = 0;
				idf.enchant = 0;
			}
		}
		delete pCon;

		for(UINT n=0;n<items.size();n++)
		{
			pSocket->Send("cdddddd", 0x22, CO_REPLY_ITEM_DELIVERY_ASK, items[n].charId, items[n].jobId, items[n].itemId, items[n].itemAmount, items[n].enchant);
		}
	}
	unguard;
	return false;
}

bool PacketHandler::RequestSaveUserFraction(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	guard;
	UINT charId = 0, fractionId = 0, fractionTimestamp = 0;
	INT32 fractionPoints = 0;
	Disassemble(packet, "dddd", &charId, &fractionId, &fractionPoints, &fractionTimestamp);

	CQuery query(L"EXEC lin_UpdateUserFraction ?, ?, ?, ?");
	query.AddParam(charId);
	query.AddParam(fractionId);
	query.AddParam(fractionPoints);
	query.AddParam(fractionTimestamp);

	DBConn *pConn = new DBConn;
	pConn->Execute(query.Get());
	delete pConn;
	unguard;
	return false;
}

bool PacketHandler::RequestLoadUserFraction(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	guard;
	UINT charId = 0;
	Disassemble(packet, "d", &charId);
	if(charId > 0)
	{
		UINT fractionId = 0, fractionTimestamp = 0;
		INT32 fractionPoints = 0;
		
		CQuery query(L"EXEC lin_LoadUserFraction ?");
		query.AddParam(charId);

		DBConn *pConn = new DBConn;
		pConn->Bind(&fractionId);
		pConn->Bind(&fractionPoints);
		pConn->Bind(&fractionTimestamp);
		if(pConn->Execute(query.Get()))
		{
			pConn->Fetch();
		}
		delete pConn;

		pQuerySocket->Send("cddddd", 0x22, CO_REPLY_LOAD_USER_FRACTION, charId, fractionId, fractionPoints, fractionTimestamp);
	}
	unguard;
	return false;
}

bool PacketHandler::RequestLoadAdditionalBoxData(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	guard;

	CQuery query(L"SELECT [account_id], [timestamp] FROM [AdditionalBox]");
	
	struct ABD
	{
		UINT accountId;
		UINT timestamp;
	};

	vector<ABD> boxes;

	ABD abd = {0, 0};
	DBConn *pDBConn = new DBConn;

	pDBConn->Bind(&abd.accountId);
	pDBConn->Bind(&abd.timestamp);
	if(pDBConn->Execute(query.Get()))
	{
		while(pDBConn->Fetch())
		{
			boxes.push_back(abd);
			abd.timestamp = 0;
			abd.accountId = 0;
		}
	}
	delete pDBConn;

	g_Log.AddU(CLog::Blue, L"Loaded [%d] additional box(es).", boxes.size());

	for(UINT n=0;n<boxes.size();n++)
	{
		pQuerySocket->Send("cddd", 0x22, CO_REPLY_LOAD_ADDITIONAL_BOX_DATA, boxes[n].accountId, boxes[n].timestamp);
	}
	unguard;
	return false;
}

bool PacketHandler::RequestSetAdditionalBox(CQuerySocket *pSocket, const unsigned char* packet)
{
	guard;
	UINT accountId = 0, timestamp = 0;
	Disassemble(packet, "dd", &accountId, &timestamp);
	if(accountId > 0)
	{
		CQuery query(L"EXEC lin_ProcessAdditionalBox ?, ?");
		query.AddParam(accountId);
		query.AddParam(timestamp);
		DBConn *pConn = new DBConn;
		pConn->Execute(query.Get());
		delete pConn;
	}
	unguard;
	return false;
}

bool PacketHandler::RequestLoadBannedHwid(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	guard;
	g_Log.Add(CLog::Error,"[%s] outdated packet called!", __FUNCTION__);
	return false;
}

bool PacketHandler::RequestBanHwid(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	guard;
	INT32 hwid = 0;
	UINT timestamp = 0;
	WCHAR wName[25];
	WCHAR wReason[255];
	memset(wName, 0, sizeof(wName));
	memset(wReason, 0, sizeof(wReason));

	Disassemble(packet, "ddSS", &hwid, &timestamp, sizeof(wName), wName, sizeof(wReason), wReason);

	if(hwid)
	{
		CQuery query(L"EXEC lin_ProcessHardwareIdBan ?, ?, '?', '?'");
		query.AddParam(hwid);
		query.AddParam(timestamp);
		query.AddParam(wName);
		query.AddParam(wReason);
		DBConn *pConn = new DBConn;
		pConn->Execute(query.Get());
		delete pConn;
	}
	unguard;
	return false;
}

bool PacketHandler::RequestSaveCharacterHwid(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	guard;
	g_Log.Add(CLog::Error, "[%s] outdated packet called!", __FUNCTION__);
	unguard;
	return false;
}

bool PacketHandler::RequestLockAccount(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	guard;
	UINT accountId = 0;
	WCHAR wName[25];
	memset(wName, 0, sizeof(wName));
	WCHAR hwid[33];
	memset(hwid, 0, sizeof(hwid));
	packet = Disassemble(packet, "d", &accountId);

	for(UINT n=0;n<32;n++)
	{
		hwid[n] = static_cast<WCHAR>(packet[n]);
	}
	packet+=32;

	packet = Disassemble(packet, "S", sizeof(wName), wName);
	if(accountId > 0)
	{
		CQuery query(L"EXEC lin_ProcessLockedHardwareId ?, ?, '?', '?'");
		query.AddParam(accountId);
		query.AddParam(0);	//charId
		query.AddParam(hwid);
		query.AddParam(wName);
		DBConn* pDBConn = new DBConn;
		pDBConn->Execute(query.Get());
		delete pDBConn;
	}
	unguard;
	return false;
}

bool PacketHandler::RequestUnlockAccount(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	guard;
	UINT accountId = 0;
	Disassemble(packet, "d", &accountId);
	
	CQuery query(L"EXEC lin_ProcessLockedHardwareId ?, ?, '?', '?'");
	query.AddParam(accountId);
	query.AddParam(0);	//charId
	query.AddParam(L"");	//hwid
	query.AddParam(L"L2Server");
	DBConn* pDBConn = new DBConn;
	pDBConn->Execute(query.Get());
	delete pDBConn;
	unguard;
	return false;
}

bool PacketHandler::RequestLockCharacter(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	guard;
	UINT charId = 0;
	WCHAR wName[25];
	memset(wName, 0, sizeof(wName));
	WCHAR hwid[33];
	memset(hwid, 0, sizeof(hwid));
	packet = Disassemble(packet, "d", &charId);
	for(UINT n=0;n<32;n++)
	{
		hwid[n] = static_cast<WCHAR>(packet[n]);
	}
	packet+=32;
	packet = Disassemble(packet, "S", sizeof(wName), wName);
	if(hwid)
	{
		CQuery query(L"EXEC lin_ProcessLockedHardwareId ?, ?, '?', '?'");
		query.AddParam(0);
		query.AddParam(charId);	//charId
		query.AddParam(hwid);
		query.AddParam(wName);
		DBConn* pDBConn = new DBConn;
		pDBConn->Execute(query.Get());
		delete pDBConn;
	}
	unguard;
	return false;
}

bool PacketHandler::RequestUnlockCharacter(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	guard;
	UINT charId = 0;
	Disassemble(packet, "d", &charId);
	
	CQuery query(L"EXEC lin_ProcessLockedHardwareId ?, ?, '?', '?'");
	query.AddParam(0);
	query.AddParam(charId);	//charId
	query.AddParam(L"");	//hwid
	query.AddParam(L"L2Server");
	DBConn* pDBConn = new DBConn;
	pDBConn->Execute(query.Get());
	delete pDBConn;
	unguard;
	return false;
}

bool PacketHandler::RequestLoadHwidLockData(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	guard;
	CQuery query(L"SELECT [account_id], [char_id], [hardware_id] FROM [LockedHardwareId]");
	
	struct HLD
	{
		UINT accountId;
		UINT charId;
		WCHAR hwid[33];
	};

	vector<HLD> locks;
	HLD hld = {0, 0, 0};
	DBConn *pDBConn = new DBConn;

	pDBConn->Bind(&hld.accountId);
	pDBConn->Bind(&hld.charId);
	pDBConn->BindS(&hld.hwid, 66);
	if(pDBConn->Execute(query.Get()))
	{
		while(pDBConn->Fetch())
		{
			locks.push_back(hld);
			hld.charId = 0;
			hld.accountId = 0;
			memset(hld.hwid, 0, 66);
		}
	}
	delete pDBConn;

	g_Log.AddU(CLog::Blue, L"Loaded [%d] locked with hardware id account(s)/character(s).", locks.size());
	for(UINT n=0;n<locks.size();n++)
	{
		BYTE hwid[32];
		for(UINT m=0;m<32;m++)
		{
			hwid[m] = static_cast<BYTE>(locks[n].hwid[m]);
		}
		pQuerySocket->Send("cdddb", 0x22, CO_REPLY_LOAD_HWID_LOCK_DATA, locks[n].accountId, locks[n].charId, 32, hwid);
	}
	unguard;
	return false;
}

bool PacketHandler::RequestLoadBannedAccounts(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	guard;
	UINT accountId = 0, timestamp = 0;
	map<UINT, UINT> mData;
	DBConn *pDBConn = new DBConn;
	pDBConn->Bind(&accountId);
	pDBConn->Bind(&timestamp);
	if(pDBConn->Execute(L"EXEC lin_LoadBannedAccounts"))
	{
		while(pDBConn->Fetch())
		{
			mData.insert(pair<UINT, UINT>(accountId, timestamp));
		}
	}
	delete pDBConn;

	CHAR buff[8190];
	int len = 0;
	UINT count = 0;
	UINT currentTime = (UINT)time(0);
	for(map<UINT, UINT>::iterator it = mData.begin(); it!=mData.end(); it++)
	{
		if(it->second > currentTime)
		{
			count++;
			len += Assemble(&buff[len], (8190 - len), "dd", it->first, it->second);
			if((count % 1000) == 0)
			{
				count = 0;
				pQuerySocket->Send("cddb", 0x22, CO_REPLY_LOAD_BANNED_ACCOUNTS, count, len, buff);
				memset(buff, 0, sizeof(buff));
				len = 0;
			}
		}
	}

	pQuerySocket->Send("cddb", 0x22, CO_REPLY_LOAD_BANNED_ACCOUNTS, count, len, buff);
	unguard;
	return false;
}

bool PacketHandler::RequestSaveMultiSellLog(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	guard;
	UINT charId = 0, accountId = 0;
	WCHAR wName[25];
	WCHAR wAccount[25];
	memset(wName, 0, sizeof(wName));
	memset(wAccount, 0, sizeof(wAccount));

	INT32 price[5][2];
	INT32 reward[5][2];
	memset(price, 0, sizeof(price));
	memset(reward, 0, sizeof(reward));

	UINT timestamp = 0;

	Disassemble(packet, "ddSSddddddddddddddddddddd", &charId, &accountId, sizeof(wName), wName, sizeof(wAccount), wAccount,
		&price[0][0], &price[0][1], &price[1][0], &price[1][1], &price[2][0], &price[2][1], &price[3][0], &price[3][1], &price[4][0], &price[4][1],
		&reward[0][0], &reward[0][1], &reward[1][0], &reward[1][1], &reward[2][0], &reward[2][1], &reward[3][0], &reward[3][1], &reward[4][0], &reward[4][1], &timestamp);

	CQuery query(L"EXEC lin_AddMultiSellLog ?, ?, '?', '?', ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?");
	query.AddParam(charId);
	query.AddParam(accountId);
	query.AddParam(wName);
	query.AddParam(wAccount);
	query.AddParam(price[0][0]);
	query.AddParam(price[0][1]);
	query.AddParam(price[1][0]);
	query.AddParam(price[1][1]);
	query.AddParam(price[2][0]);
	query.AddParam(price[2][1]);
	query.AddParam(price[3][0]);
	query.AddParam(price[3][1]);
	query.AddParam(price[4][0]);
	query.AddParam(price[4][1]);	
	query.AddParam(reward[0][0]);
	query.AddParam(reward[0][1]);
	query.AddParam(reward[1][0]);
	query.AddParam(reward[1][1]);
	query.AddParam(reward[2][0]);
	query.AddParam(reward[2][1]);
	query.AddParam(reward[3][0]);
	query.AddParam(reward[3][1]);
	query.AddParam(reward[4][0]);
	query.AddParam(reward[4][1]);
	query.AddParam(timestamp);

	DBConn *pDBConn = new DBConn;
	pDBConn->Execute(query.Get());
	delete pDBConn;

	unguard;
	return false;
}

bool PacketHandler::RequestBanAccount(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	guard;
	UINT accountId = 0;
	WCHAR wName[25];
	WCHAR wReason[255];
	memset(wName, 0, sizeof(wName));
	memset(wReason, 0, sizeof(wReason));
	UINT timestamp = 0;

	Disassemble(packet, "dSdS", &accountId, sizeof(wName), wName, &timestamp, sizeof(wReason), wReason);

	CQuery query(L"EXEC lin_BanAccount ?, '?', ?, '?'");
	query.AddParam(accountId);
	query.AddParam(wName);
	query.AddParam(timestamp);
	query.AddParam(wReason);

	DBConn *pDBConn = new DBConn;
	pDBConn->Execute(query.Get());
	delete pDBConn;
	unguard;
	return false;
}

bool PacketHandler::RequestSaveVotePoints(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	guard;
	int charId = 0, accountId = 0, points = 0;
	Disassemble(packet, "ddd", &accountId, &charId, &points);
	if(charId && accountId)
	{
		DBConn *pDBConn = new DBConn;
		if(!pDBConn->Execute(L"UPDATE user_vote SET vote_points = %d WHERE account_id = %d", points, accountId))
		{
			pDBConn->Execute(L"INSERT INTO user_vote (account_id, vote_points, vote_timestamp) VALUES (%d, %d, 0)", accountId, points);
		}
		delete pDBConn;

	}
	unguard;
	return false;
}

bool PacketHandler::RequestLoadVotePoints(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	guard;
	int charId = 0, accountId = 0, points = 0;
	DWORD voteTimestamp = 0;
	Disassemble(packet, "dd", &accountId, &charId);
	if(charId)
	{
		DBConn *pDBConn = new DBConn;
		pDBConn->Bind(&points);
		pDBConn->Bind(&voteTimestamp);
		if(pDBConn->Execute(L"SELECT vote_points, vote_timestamp FROM user_vote WHERE account_id = %d", accountId))
		{
			if(pDBConn->Fetch())
			{
				//done
			}
		}
		delete pDBConn;

		pQuerySocket->Send("cdddd", 0x22, CO_REPLY_LOAD_VOTE_POINTS, charId, points, voteTimestamp);
	}
	unguard;
	return false;
}

bool PacketHandler::RequestLoadVIPAccounts(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	guard;
	int accountId = 0;
	int vipLevel = 0;
	DWORD timestamp = 0;

	struct VIPAcc
	{
		int id;
		int level;
		DWORD timestamp;
	};
	vector<VIPAcc> vips;

	DBConn* pDBConn = new DBConn;

	pDBConn->Bind(&accountId);
	pDBConn->Bind(&vipLevel);
	pDBConn->Bind(&timestamp);
	
	if(pDBConn->Execute(L"SELECT [account_id] ,[vip_level] ,[vip_timestamp] FROM [user_data] WHERE [vip_level] > 0"))
	{
		while(pDBConn->Fetch())
		{
			if(timestamp > time(NULL))
			{
				VIPAcc acc;
				acc.id = accountId;
				acc.level = vipLevel;
				acc.timestamp = timestamp;
				vips.push_back(acc);
			}
		}
	}
	delete pDBConn;

	int count = 0;
	CHAR buff[8190];
	memset(buff, 0, sizeof(buff));
	int len = 0;
	for(UINT n=0;n<vips.size();n++)
	{
		VIPAcc& va = vips[n];
		pQuerySocket->Send("cdddd", 0x22, CO_REPLY_LOAD_VIP_ACCOUNTS, va.id, va.level, va.timestamp);
	}

	unguard;
	return false;
}

bool PacketHandler::RequestLoadVipStatus(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	guard;
	int charId = 0, vipLevel = 0;
	DWORD vipTimestamp = 0;
	Disassemble(packet, "d", &charId);
	if(charId)
	{
		DBConn *pDBConn = new DBConn;
		pDBConn->Bind(&vipLevel);
		pDBConn->Bind(&vipTimestamp);
		if(pDBConn->Execute(L"SELECT vip_level, vip_timestamp FROM user_data WHERE char_id = %d", charId))
		{
			if(pDBConn->Fetch())
			{
				//done
			}
		}
		delete pDBConn;

		pQuerySocket->Send("cdddd", 0x22, CO_REPLY_LOAD_VIP_STATUS, charId, vipLevel, vipTimestamp);
	}
	unguard;
	return false;
}

bool PacketHandler::RequestSaveVipStatus(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	guard;
	int charId = 0, vipLevel = 0;
	DWORD vipTimestamp = 0;
	Disassemble(packet, "ddd", &charId, &vipLevel, &vipTimestamp);
	if(charId)
	{
		DBConn *pDBConn = new DBConn;
		pDBConn->Execute(L"UPDATE user_data SET vip_level = %d, vip_timestamp = %d WHERE char_id = %d", vipLevel, vipTimestamp, charId);
		delete pDBConn;
	}
	unguard;
	return false;
}

bool __cdecl PacketHandler::RequestSaveNpcDeath(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	guard;
	int npcClassId = 0;
	DWORD deathTime = 0;
	Disassemble(packet, "dd", &npcClassId, &deathTime);
	if(npcClassId > 0)
	{
		DBConn *pDBConn = new DBConn;
		DWORD count = 0;
		pDBConn->Bind(&count);
		if(pDBConn->Execute(L"SELECT COUNT(*) FROM NpcDeath WHERE npc_class_id = %d", npcClassId))
		{
			if(pDBConn->Fetch())
			{
				if(count > 0)
				{
					delete pDBConn;
					pDBConn = new DBConn;
					pDBConn->Execute(L"UPDATE NpcDeath SET death_time = %d WHERE npc_class_id = %d", deathTime, npcClassId);
				}else
				{
					delete pDBConn;
					pDBConn = new DBConn;
					pDBConn->Execute(L"INSERT INTO NpcDeath (npc_class_id, death_time) VALUES (%d, %d)", npcClassId, deathTime);
				}
			}
		}
		delete pDBConn;
	}
	unguard;
	return false;
}

bool __cdecl PacketHandler::RequestLoadNpcDeath(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	guard;
	DBConn *pDBConn = new DBConn;
	int npcClassId;
	DWORD deathTime;
	char buffer[8190];
	int len = 0;
	int counter = 0;
	pDBConn->Bind(&npcClassId);
	pDBConn->Bind(&deathTime);
	if(pDBConn->Execute(L"SELECT npc_class_id, death_time FROM NpcDeath"))
	{
		while(pDBConn->Fetch())
		{
			len += Assemble(&buffer[len], (8190-len), "dd", npcClassId, deathTime);
			npcClassId = 0;
			deathTime = 0;
			counter++;
		}
	}
	delete pDBConn;
	pQuerySocket->Send("cddb", 0x22, CO_REPLY_LOAD_NPC_DEATH, counter, len, buffer);
	unguard;
	return false;
}

bool __cdecl PacketHandler::RequestRestoreCH(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	int pledgeId = 0, lostAgitId = 0;
	Disassemble(packet, "dd", &pledgeId, &lostAgitId);
	if(pledgeId > 0)
	{
		int restoredAgitId = 0;
		DBConn* pDBConn = new DBConn();
		pDBConn->Bind(&restoredAgitId);
		if(pDBConn->Execute(L"SELECT id FROM agit WHERE id != %d AND pledge_id = %d", lostAgitId, pledgeId))
		{
			if(pDBConn->Fetch())
			{
				pQuerySocket->Send("cddd", 0x22, CO_REPLY_RESTORE_CH, pledgeId, restoredAgitId);
			}
		}
		delete pDBConn;
	}
	return false;
}

bool __cdecl PacketHandler::RequestDeleteItems(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	list<int> lItems;
	int ListSize = 0;
	DWORD GStart = 0, Start = GetTickCount(), RequestType = 0, UniqueId = 0;

	packet = Disassemble(packet, "dddd", &UniqueId, &RequestType, &GStart, &ListSize);
	if(ListSize > 0)
	{
		for(int n=0;n<ListSize;n++)
		{
			int nItemID = 0;
			packet = Disassemble(packet, "d", &nItemID);
			if(nItemID)
			{
				lItems.push_back(nItemID);
			}
		}
	//	g_Log.AddU(CLog::Black, L"[Ext] ListSize[%d].", ListSize);
		int MaxCharID = 0;
		{
			DBConn* pDBConn = new DBConn();
			pDBConn->Bind(&MaxCharID);
			if(pDBConn->Execute(L"SELECT TOP 1 [char_id] FROM [user_data] Order by [char_id] DESC"))
			{
				pDBConn->Fetch();
			}
			delete pDBConn;
		}
	//	g_Log.AddU(CLog::Black, L"[Ext] MaxCharId[%d].", MaxCharID);
		if(MaxCharID > 0)
		{
			for(int n=1;n<=MaxCharID;n++)
			{
				CUserSP user_sp = g_UserDB.GetUser(n, false);
				if(user_sp.pObject != NULL)
				{
					if(user_sp.pObject->is_online == false)
					{
						if(RequestType < 2)
						{
							CWareHouseSP WareHouseSP = user_sp->GetWareHouse(RequestType, false);
							if(WareHouseSP.pObject != NULL)
							{
								for(list<int>::iterator lIter = lItems.begin();lIter!=lItems.end();lIter++)
								{
									int nItemID = (*lIter);
									int nItemDBID = 0;
									int nItemCount = 0;
								//	g_Log.AddU(CLog::Black, L"[Ext] UserID[%d] warehouse[%d] trying to find item[%d].", n, RequestType, nItemID );
									if(nItemID > 0)
									{
										CItemSP ItemSP = WareHouseSP->GetItemByType(nItemID, 0);
										if(ItemSP.pObject != NULL)
										{
											nItemCount = ItemSP->nAmount;
											nItemDBID = ItemSP->nItemID;
										}
									}
									if(nItemCount > 0)
									{
										g_Log.AddU(CLog::Black, L"[Ext] UserID[%d] GotItem[%d]c[%d]wh[%d] deleting.", n, nItemID, nItemCount, RequestType );
										//Delete Item
										WareHouseSP->DeleteItem(nItemDBID, nItemCount);
									}
								}
							}
						}
					}
					else if(RequestType == 1) //WareHouse
					{
					//	g_Log.AddU(CLog::Black, L"[Ext] UserID is online[%d].", n);
						CWareHouseSP WareHouseSP = user_sp->GetWareHouse(1, true); //warehouse
						if(WareHouseSP.pObject != NULL)
						{
							for(list<int>::iterator lIter = lItems.begin();lIter!=lItems.end();lIter++)
							{
								int nItemID = (*lIter);
								int nItemCount = 0;
					//			g_Log.AddU(CLog::Black, L"[Ext] UserID[%d] warehouse[%d] trying to find item[%d].", n, 1, nItemID );
								if(nItemID > 0)
								{
									CItemSP ItemSP = WareHouseSP->GetItemByType(nItemID, 0);
									if(ItemSP.pObject != NULL)
									{
										nItemCount = ItemSP->nAmount;
									}
								}
								if(nItemCount > 0)
								{
									g_Log.AddU(CLog::Black, L"[Ext] UserID[%d] GotItem[%d][%d][%d] deleting.", n, nItemID, nItemCount, 1 );
									//Delete Item
									WareHouseSP->DeleteItem(nItemID, nItemCount);
								}
							}
						}
					}
				}
			}
			//delete from db for offline not loaded users
			for(list<int>::iterator lIter = lItems.begin();lIter!=lItems.end();lIter++)
			{
				if(int ItemID = *lIter)
				{
					DBConn* pDBConn = new DBConn();
					pDBConn->Execute(L"UPDATE user_item set char_id=0, item_type=0 WHERE item_type=%d AND warehouse=%d", ItemID, RequestType);
					delete pDBConn;
				}
			}
		}
	}
	
	pQuerySocket->Send("cdddddd", 0x22, CO_REPLY_DELETE_ITEMS, UniqueId, RequestType, GStart, Start, GetTickCount());
	return false;
}

bool __cdecl PacketHandler::RequestSaveAIO(CQuerySocket* pQuerySocket, const unsigned char* packet)
{
	UINT charId = 0;
	UINT timestamp = 0, id = 0;
	Disassemble(packet, "ddd", &charId, &id, &timestamp);
	if(charId)
	{
		DBConn* pDBConn = new DBConn;
		pDBConn->Execute(L"UPDATE [user_data] SET aio_id = %d, aio_timestamp = %d WHERE char_id = %d", id, timestamp, charId);
		delete pDBConn;
	}
	return false;
}

bool __cdecl PacketHandler::RequestLoadAIO(CQuerySocket *pSocket, const unsigned char* packet)
{
	UINT charId = 0;
	UINT id = 0, timestamp = 0;
	Disassemble(packet, "d", &charId);
	if(charId)
	{
		DBConn *pDBConn = new DBConn();
		pDBConn->Bind(&id);
		pDBConn->Bind(&timestamp);
		if(pDBConn->Execute(L"SELECT aio_id, aio_timestamp FROM user_data WHERE char_id = %d", charId))
		{
			pDBConn->Fetch();
		}
		if(id > 0)
		{
			pSocket->Send("cdddd", 0x22, CO_REPLY_LOAD_AIO, charId, id, timestamp);
		}
		delete pDBConn;
	}

	return false;
}

bool __cdecl PacketHandler::RequestSetPledgePrivilege(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	DWORD dPledgeID, dRank, dPrivilege;
	Disassemble(packet, "ddd", &dPledgeID, &dRank, &dPrivilege);
	DBConn *pDBConn = new DBConn;
	if(pDBConn->Execute(L"UPDATE pledge_ext SET [Rank%d] = %d WHERE pledge_id = %d", dRank, dPrivilege, dPledgeID))
		;
	else 
	{
		if(pDBConn->Execute(L"EXEC lin_AddPledgeExt %d", dPledgeID))
			g_Log.AddU(CLog::Blue, L"[PacketHandler] Adding PledgeID[%d] to database.", dPledgeID);
	}
	delete pDBConn;
	return false;
};

bool __cdecl PacketHandler::RequestSetPledgeMemberRank(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	INT32 nPledgeID, nRank;
	wchar_t wszName[25];
	wszName[0] = 0;
	Disassemble(packet, "Sdd", sizeof(wszName), wszName, &nPledgeID, &nRank);

	CQuery query(L"UPDATE [user_data] SET [Rank] = ? WHERE [char_name] = '?' AND [pledge_id] = ?");
	query.AddParam(nRank);
	query.AddParam(wszName);
	query.AddParam(nPledgeID);

	DBConn *pDBConn = new DBConn;
	pDBConn->Execute(query.Get());
	delete pDBConn;

	return false;
};

bool __cdecl PacketHandler::RequestReorganizeMember(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	wchar_t wszName[25];
	wszName[0] = 0;
	int nPledgeType, nPledgeID;
	int nDBID = 0;
	Disassemble(packet, "Sdd", sizeof(wszName), wszName, &nPledgeType, &nPledgeID );
	
	CQuery query(L"EXEC lin_ReorganizePledgeMember '?', ?, ?");
	query.AddParam(wszName);
	query.AddParam(nPledgeType);
	query.AddParam(nPledgeID);

	DBConn *pDBConn = new DBConn;
	pDBConn->Bind(&nDBID);
	if(pDBConn->Execute(query.Get()))
	{
		if(pDBConn->Fetch())
		{
		//	pQuerySocket->Send("chddSdd", 0xD8, 0x03, 5, nDBID, wszName, nPledgeType, nPledgeID);
		}
	}
	delete pDBConn;
	return false;
}

bool __cdecl PacketHandler::RequestSetPledgeReputation(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	int nPledgeID, nPoints;
	Disassemble(packet, "dd", &nPledgeID, &nPoints);

	DBConn *pDBConn = new DBConn;
	if(pDBConn->Execute(L"UPDATE pledge_ext SET reputation_points = %d WHERE pledge_id = %d", nPoints, nPledgeID))
	{
		g_Log.AddU(CLog::Blue, L"[PacketHandler] Updated reputation for pledge[%d] points[%d]", nPledgeID, nPoints);
	}else
		g_Log.AddU(CLog::Error, L"[PacketHandler] Error - RequestUpdateReputationPoints pledge[%d] points[%d]", nPledgeID, nPoints);
	delete pDBConn;
	return false;
}

bool __cdecl PacketHandler::RequestSetSponsor(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	int nCharID, nSponsorID, nPledgeID;
	Disassemble(packet, "ddd", &nCharID, &nSponsorID, &nPledgeID);
	DBConn *pDBConn = new DBConn;
	if(pDBConn->Execute(L"UPDATE user_data SET sponsor_id = %d WHERE char_id = %d", nSponsorID, nCharID))
	{
		//g_Log.AddU(CLog::Blue, L"[PacketHandler] RequestSetSponsor for[%d] pledge[%d]", nCharID, nPledgeID);
	}else
		g_Log.AddU(CLog::Error, L"[PacketHandler] Error - RequestSetSponsor pledge[%d] char_id[%d]", nPledgeID, nCharID);
	delete pDBConn;
	return false;
}

bool __cdecl PacketHandler::RequestSetUserPledgeTypeAndRank(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	int nRank, nPledgeType, nDBID;

	Disassemble(packet, "ddd", &nDBID, &nRank, &nPledgeType);

	DBConn *pDBConn = new DBConn;
	if(pDBConn->Execute(L"UPDATE [user_data] SET [Rank] = %d, [PledgeType] = %d WHERE [char_id] = %d", nRank, nPledgeType, nDBID))
	;
	else 
	{
		g_Log.AddU(CLog::Error, L"[PacketHandler] Error in executing- RequestSetUserPledgeTypeAndRank() DBID[%d] Rank[%d]", nDBID, nRank);
	}
	delete pDBConn;
	return false;
};

bool __cdecl PacketHandler::RequestCreateSubPledge(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	int nPledgeID, nPledgeType;
	wchar_t wszName[25];
	wszName[0] = 0;

	Disassemble(packet, "ddS", &nPledgeID, &nPledgeType, sizeof(wszName), wszName);

	CQuery query(L"EXEC lin_CreateSubPledge ?, ?, '?'");
	query.AddParam(nPledgeID);
	query.AddParam(nPledgeType);
	query.AddParam(wszName);

	DBConn *pDBConn = new DBConn;
	if(pDBConn->Execute(query.Get()))
	{
	}
	delete pDBConn;
	return false;
}

bool __cdecl PacketHandler::RequestRenameSubPledge(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	int nPledgeID, nPledgeType;
	wchar_t wszName[25];
	wszName[0] = 0;
	Disassemble(packet, "ddS", &nPledgeID, &nPledgeType, sizeof(wszName), wszName);
	CQuery query(L"EXEC lin_RenameSubPledge ?, ?, '?'");
	query.AddParam(nPledgeID);
	query.AddParam(nPledgeType);
	query.AddParam(wszName);

	DBConn *pDBConn = new DBConn;
	pDBConn->Execute(query.Get());
	delete pDBConn;

	return false;
}

bool __cdecl PacketHandler::RequestSetSubCaptain(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	wchar_t wszName[25];
	wszName[0] = 0;
	int nPledgeID, nPledgeType, nCaptainDBID;
	Disassemble(packet, "ddSd", &nPledgeID, &nPledgeType, sizeof(wszName), wszName, &nCaptainDBID);
	CQuery query(L"EXEC lin_SetSubCaptain ?, ?, '?', ?");
	query.AddParam(nPledgeID);
	query.AddParam(nPledgeType);
	query.AddParam(wszName);
	query.AddParam(nCaptainDBID);

	DBConn *pDBConn = new DBConn;
	if(pDBConn->Execute(query.Get()))
	{
	//	pQuerySocket->Send("chdddSd", 0xD8, 3, 6, nPledgeID, nPledgeType, wszName, nCaptainDBID);
	}
	delete pDBConn;
	return false;
}

bool __cdecl PacketHandler::RequestLoadSpiritCount(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	int nCharID = 0;
	int nSpiritCount = 0;
	Disassemble(packet, "d", &nCharID);
	DBConn *pDBConn = new DBConn();
	pDBConn->Bind(&nSpiritCount);
	if(pDBConn->Execute(L"SELECT spirit_count FROM user_data where char_id = %d", nCharID))
	{
		if(pDBConn->Fetch())
		{
			pQuerySocket->Send("cddd", 0x22, CO_REPLY_LOAD_SPIRIT_COUNT, nCharID, nSpiritCount);
		}
	}
	delete pDBConn;
	return false;
}

bool __cdecl PacketHandler::RequestSaveBuffList(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	UINT buffList[4][48];
	memset(buffList, 0, sizeof(buffList));

	UINT charId = 0, subjobIndex = 0;
	packet = Disassemble(packet, "dd", &charId, &subjobIndex);

	for(UINT n=0;n<4;n++)
	{
		UINT amount = 0;
		packet = Disassemble(packet, "d", &amount);
		for(UINT m=0;m<amount;m++)
		{
			if(m < 48)
			{
				packet = Disassemble(packet, "d", &buffList[n][m]);
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Buff amount overflow[%d] charId[%d]", __FUNCTION__, amount, charId);
				break;
			}
		}
	}

	bool insert = false;
	
	DBConn *pConn = new DBConn();
	//update
	for(UINT m=0;m<4;m++)
	{
		wstringstream query;
		query << L"UPDATE user_scheme_buffer SET ";
		for(UINT n=0;n<48;n++)
		{
			if(n==0)
			{
				query << L"[b" << n << L"] = " << buffList[m][n];
			}else
			{
				query << L", [b" << n << L"] = " << buffList[m][n];
			}
		}
		query << L" WHERE [char_id] = " << charId << L" AND subjob_index = " << subjobIndex << L" AND type = " << m ;
		if(!pConn->Execute(query.str().c_str()))
		{
			insert = true;
			break;
		}
	}
	
	if(insert)
	{
		//insert
		for(UINT m=0;m<4;m++)
		{
			wstringstream query;
			query << L"INSERT INTO user_scheme_buffer (char_id, subjob_index, type";
			for(UINT n=0;n<48;n++)
			{
				query << L", b" << n;
			}
			query << L") VALUES (" << charId << L", " << subjobIndex << L", " << m;
			for(UINT n=0;n<48;n++)
			{
				query << L", " << buffList[m][n];
			}
			query << L")";
			pConn->Execute(query.str().c_str());
		}
	}

	delete pConn;

	return false;
}

bool __cdecl PacketHandler::RequestLoadBuffList(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	UINT charId = 0;
	UINT subjobIndex = 0;
	Disassemble(packet, "dd", &charId, &subjobIndex);

	char mainBuff[8190];
	memset(mainBuff, 0, sizeof(mainBuff));
	int mainLen = Assemble(mainBuff, sizeof(mainBuff), "cdd", 0x22, CO_REPLY_LOAD_BUFF_LIST, charId);

	for(UINT m=0;m<4;m++)
	{
		int buffAmount = 0;
		char buff[8190];
		int len = 0;
		
		UINT buffList[48];
		memset(buffList, 0, sizeof(buffList));

		DBConn *pDBConn = new DBConn();
		for(int n=0;n<48;n++)
		{
			pDBConn->Bind(&buffList[n]);
		}

		if(pDBConn->Execute(L"SELECT [b0], [b1], [b2] ,[b3], [b4], [b5], [b6], [b7], [b8], [b9], [b10], [b11], [b12], [b13], [b14], [b15], [b16], [b17], [b18], [b19], [b20], [b21], [b22], [b23], [b24], [b25], [b26], [b27], [b28], [b29], [b30], [b31], [b32], [b33], [b34], [b35], [b36], [b37], [b38], [b39], [b40], [b41], [b42], [b43], [b44], [b45], [b46], [b47] FROM [user_scheme_buffer] WHERE char_id=%d AND subjob_index=%d AND type=%d", charId, subjobIndex, m))
		{
			if(pDBConn->Fetch())
			{
				for(UINT n=0;n<48;n++)
				{
					if(buffList[n] > 0)
					{
						buffAmount++;
						len += Assemble(&buff[len], (8190 - len), "d", buffList[n]);
					}
				}
			}
		}
		delete pDBConn;

		mainLen += Assemble(&mainBuff[mainLen], (8190 - mainLen), "db", buffAmount, len, buff);
	}

	pQuerySocket->Send(mainLen, mainBuff);

	return false;
}

bool __cdecl PacketHandler::RequestAddPledge(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	DWORD dPledgeID;
	Disassemble(packet, "d", &dPledgeID);

	DBConn *pDBConn = new DBConn;
	if(pDBConn->Execute(L"EXEC lin_AddPledgeExt %d", dPledgeID))
		;
	else g_Log.AddU(CLog::Error, L"[PacketHandler] Error in executing- RequestAddPledge() PledgeID[%d]", dPledgeID);
	delete pDBConn;
	return false;
}

bool __cdecl PacketHandler::RequestAcquirePledgeSkill(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	int nPledgeID, nSkillID, nLevel;
	Disassemble(packet, "ddd", &nPledgeID, &nSkillID, &nLevel);

	DBConn *pDBConn = new DBConn;
	if(pDBConn->Execute(L"EXEC lin_LearnClanSkill %d, %d, %d", nPledgeID, nSkillID, nLevel))
	{
		g_Log.AddU(CLog::Blue, L"[PacketHandler] Pledge[%d] bought clan skill[%d] lvl [%d]", nPledgeID, nSkillID, nLevel);
	}else
		g_Log.AddU(CLog::Error, L"[RequestAcquirePledgeSkill] pledge[%d] Skill[%d] Level[%d] - error", nPledgeID, nSkillID, nLevel);
	delete pDBConn;
	return false;
}

//Pledge Packet fix
void PacketHandler::SendReplyToLoadPledge(CPledge *pPledge, CSocket *pSocket)
{
	guard;
	//g_Log.AddU(CLog::Blue, L"[LoadPledge] PledgeID[%d]", pPledge->nPledgeID);
	char Buffer[16384];
	memset(Buffer, 0, 16384);
	int nMemberPos = 0;
	int nLen = 0;
	nLen = Assemble(Buffer, sizeof(Buffer), "cSddS", 0x15, pPledge->wszClanName, pPledge->nPledgeID, pPledge->nMasterID, pPledge->wszMasterName);
	nMemberPos = nLen;
	nLen += 4; //Space for member amount;

	//Pledge Ext Info part
	int nReputation = 0;
	DWORD PowerData[10];
	DWORD SkillData[22];
	memset(SkillData, 0, sizeof(SkillData));
	memset(PowerData, 0, sizeof(PowerData));
	DBConn *pDBConn = new DBConn;
	pDBConn->Bind(&nReputation);
	for(int n=1;n<10;n++)
		pDBConn->Bind(&PowerData[n]);
	for(int n=0;n<22;n++)
	{
		pDBConn->Bind(&SkillData[n]);
	}
	if(pDBConn->Execute(L"EXEC lin_LoadPledgeExtendInfo %d", pPledge->nPledgeID))
	{
		pDBConn->Fetch();
	}
	delete pDBConn;
	nLen += Assemble(&Buffer[nLen], (sizeof(Buffer)-nLen), "dddddddddd", nReputation, PowerData[1], PowerData[2], PowerData[3], PowerData[4], PowerData[5], PowerData[6], PowerData[7], PowerData[8], PowerData[9]);
	nLen += Assemble(&Buffer[nLen], (sizeof(Buffer)-nLen), "d", 22); //Amount of skills
	for(int n=370;n<392;n++)
	{
		nLen += Assemble(&Buffer[nLen], (sizeof(Buffer)-nLen), "dd", n, SkillData[(n-370)]);
	}

	//SubPledge part

	//Getting subpledges
	char cSubBuff[8192];
	memset(cSubBuff, 0, 8192);
	int nSubLen = 0;
	int nSubAmount = 0;
	int nPledgeType = 0;
	int nSubLeaderID = NULL;
	wchar_t wszSubName[25];
	wchar_t wszSubLeader[25];
	pDBConn = new DBConn;
	pDBConn->Bind(&nPledgeType);
	pDBConn->BindS(&wszSubName, sizeof(wszSubName));
	pDBConn->BindS(&wszSubLeader, sizeof(wszSubLeader));
	pDBConn->Bind(&nSubLeaderID);
	if(pDBConn->Execute(L"EXEC lin_LoadSubPledges %d", pPledge->nPledgeID))
	{
		while(pDBConn->Fetch())
		{
			nSubLen += Assemble((char*)(cSubBuff+nSubLen), (8192-nSubLen), "dSSd", nPledgeType, wszSubName, wszSubLeader, nSubLeaderID);
			nSubAmount++;
			memset(wszSubName, 0, sizeof(wszSubName));
			memset(wszSubLeader, 0, sizeof(wszSubLeader));
		}
	}
	delete pDBConn;
	nLen += Assemble(&Buffer[nLen], (sizeof(Buffer)-nLen), "d", nSubAmount);
	if(nSubAmount > 0)
	{
		memcpy((char*)(Buffer+nLen), cSubBuff, nSubLen);
		nLen+=nSubLen;
	}

	//Members Part 
	int nMemberAmount = 0;
	int nDBID = 0;
	wchar_t wszName[50];
	int nLevel = 0;
	int nClass = 0;
	int nGender = 0;
	int nRace = 0;
	nPledgeType = 0;
	int nRank = 0;
	int nSponsorID = 0;
	int nJoinPledgeLevel = 0;
	pDBConn = new DBConn;
	pDBConn->BindS(wszName, 50);
	pDBConn->Bind(&nDBID);
	pDBConn->Bind(&nGender);
	pDBConn->Bind(&nRace);
	pDBConn->Bind(&nClass);
	pDBConn->Bind(&nLevel);
	pDBConn->Bind(&nPledgeType);
	pDBConn->Bind(&nRank);
	pDBConn->Bind(&nSponsorID);
	pDBConn->Bind(&nJoinPledgeLevel);
	if(pDBConn->Execute(L"SELECT TOP 160 [char_name],[char_id],[gender],[race],[class],[Lev],[PledgeType],[Rank],[sponsor_id],[JoinPledgeLevel] FROM [user_data] WHERE [pledge_id] = %d AND [account_id] > 0", pPledge->nPledgeID))
	{
		while(pDBConn->Fetch())
		{
			nLen += Assemble(&Buffer[nLen], (sizeof(Buffer)-nLen), "dSdddddddd", nDBID, wszName, nLevel, nClass, nGender, nRace, nPledgeType, nRank, nSponsorID, nJoinPledgeLevel);
			//nLen += Assemble(&Buffer[nLen], (sizeof(Buffer)-nLen), "dSdddddddd", nDBID, wszName, nLevel, nClass, nGender, nRace, nPledgeType, nRank, nSponsorID, nJoinPledgeLevel);
			nMemberAmount++;
		}
	}
	delete pDBConn;

	Assemble((char*)((DWORD)Buffer+nMemberPos), 4, "d", nMemberAmount);
	
	//Pledge Data part 
	nLen += Assemble(&Buffer[nLen], (sizeof(Buffer)-nLen), "ddddddddddddddddddd", pPledge->now_war_id, pPledge->root_name_value, pPledge->challenge_time, pPledge->oust_time, pPledge->nVar5, pPledge->nVar6, pPledge->castle_id, pPledge->agit_id, pPledge->rank, pPledge->status, pPledge->private_flag, pPledge->crest_id, pPledge->is_guilty, pPledge->dismiss_reserved_time, pPledge->skill_level, pPledge->alliance_ousted_time, pPledge->alliance_withdraw_time, pPledge->alliance_dismiss_time, pPledge->emblem_id);

	//Send(pSocket, "b", nLen, Buffer);
	pSocket->Lock(__WFILE__, __LINE__);
	pSocket->DirectSend(Buffer, nLen);
	pSocket->Unlock();
	unguard;
}

bool __cdecl PacketHandler::RequestSaveItemDataEx(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	UINT charId = 0;
	INT32 itemId = 0, manaLeft = 0, lifeTime = 0, protectionTimeout = 0;
	UINT augmentation = 0;
	//Send("chdddddd", 0xF9, CI_SAVE_ITEM_DATA_EX, charId, pItem->pSID->nDBID, pItem->nAugmentationID, pItem->nManaLeft, pItem->nLifeTime, pItem->nProtectionTimeout);
	packet = Disassemble(packet, "dddddd", &charId, &itemId, &augmentation, &manaLeft, &lifeTime, &protectionTimeout );
	if(charId && itemId)
	{
		CQuery query(L"UPDATE [user_item] SET [augmentation] = ?, [mana_left] = ?, [life_time] = ?, [protection_timeout] = ? WHERE [item_id] = ? AND [char_id] = ?");
		query.AddParam(augmentation);
		query.AddParam(manaLeft);
		query.AddParam(lifeTime);
		query.AddParam(protectionTimeout);
		query.AddParam(itemId);
		query.AddParam(charId);

		DBConn *pDBConn = new DBConn;
		pDBConn->Execute(query.Get());
		delete pDBConn;
	}
	return false;
}
bool __cdecl PacketHandler::RequestSaveJoinPledgeLevel(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	int nUserDBID, nLevel;

	Disassemble(packet, "dd", &nUserDBID, &nLevel);

	DBConn *pDBConn = new DBConn;
	if(pDBConn->Execute(L"UPDATE [user_data] SET [JoinPledgeLevel] = %d WHERE [char_id] = %d", nLevel, nUserDBID))
	{
	}
	delete pDBConn;
	return false;
};

bool __cdecl PacketHandler::RequestDelegateClanMaster(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	int nCharID, nPledgeID;
	Disassemble(packet, "dd", &nPledgeID, &nCharID);
	if(nPledgeID)
	{
		DBConn *pDBConn = new DBConn;
		if(pDBConn->Execute(L"UPDATE [Pledge] SET [delegate_clid] = %d WHERE [pledge_id]=%d", nCharID, nPledgeID))
		{
		}
		delete pDBConn;
	}
	return false;
}

bool __cdecl PacketHandler::RequestSavePledgeStat(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	int nPledgeID = 0;
	int nCastleID = 0;
	int nMemberCount = 0;
	
	packet = Disassemble(packet, "ddd", &nMemberCount, &nCastleID, &nPledgeID);
	g_Log.AddU(CLog::Blue, L"[PacketHandler] Pledge[%d] Castle[%d] Count[%d]", nPledgeID, nCastleID, nMemberCount);

	if(nPledgeID && nCastleID && nMemberCount > 0)
	{
		DBConn *pDBConn = new DBConn();
		pDBConn->Execute(L"lin_ClearPledgeStat %d, %d", nCastleID, nPledgeID);
		delete pDBConn;
		for(int n=0;n<nMemberCount;n++)
		{
			CMemberStatInfo Member;
			packet = Disassemble(packet, "ddddd", &Member.CharID, &Member.KillCount, &Member.DeathCount, &Member.DamageDealt, &Member.DamageTaken);
			if(Member.CharID)
			{
				pDBConn = new DBConn();
				pDBConn->Execute(L"lin_AddMemberStat %d, %d, %d, %d, %d, %d, %d", nCastleID, nPledgeID, Member.CharID, Member.KillCount, Member.DeathCount, Member.DamageDealt, Member.DamageTaken);
				delete pDBConn;
			}
		}
		
	}
	return false;
}

bool __cdecl PacketHandler::RequestSaveKillDeathStat(CQuerySocket *pQuerySocket, const unsigned char* packet)
{

	int nCharID = 0, nClassID = 0, nLevel = 0, nKillerLevel = 0, nKillerID = 0, nKillerClassID = 0, nKillerHP = 0, nKillerMaxHP = 0, nKillerCP = 0, nKillerMaxCP = 0, x = 0, y = 0, z = 0, PvP = 0, SkillID = 0, SkillLevel = 0;
	DWORD nTime = 0;
	Disassemble(packet, "ddddddddddddddddd", &nCharID, &nClassID, &nLevel, &nKillerID, &nKillerClassID, &nKillerLevel, &nKillerHP, &nKillerMaxHP, &nKillerCP, &nKillerMaxCP, &x, &y, &z, &PvP, &SkillID, &SkillLevel, &nTime);
	DBConn *pDBConn = new DBConn();
	if(nCharID && nKillerID)
	{
		pDBConn->Execute(L"lin_AddKillDeathStat %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", nCharID, nClassID, nLevel, nKillerID, nKillerClassID, nKillerLevel, nKillerHP, nKillerMaxHP, nKillerCP, nKillerMaxCP, x, y, z, PvP, SkillID, SkillLevel, nTime);
	}
	delete pDBConn;
	return false;
}

bool __cdecl PacketHandler::RequestSetPrivateStore(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	int nCharID, nOffline, nStoreType, nCreate, nItemCount;
	int nItems[8][4]; memset(nItems, 0, sizeof(nItems));
	packet = Disassemble(packet, "ddddd", &nCharID, &nStoreType, &nCreate, &nOffline, &nItemCount);
	if(nStoreType > 0)
	{
		for(int n=0;n<nItemCount;n++)
		{
			int nPrice, nCount, nEnchant, nClassID;
			packet = Disassemble(packet, "dddd", &nClassID, &nPrice, &nCount, &nEnchant);
			nItems[n][0] = nClassID;
			nItems[n][1] = nCount;
			nItems[n][2] = nPrice;
			nItems[n][3] = nEnchant;
			//g_Log.AddU(CLog::Blue, L"ClassID[%d] Price[%d] Count[%d] Enchant[%d]", nClassID, nPrice, nCount, nEnchant);
		}

		if(nCreate)
		{
			DBConn *pDBConn = new DBConn();
			wstring wCmd = L"EXEC lin_CreatePrivateStore ";
			WCHAR wLine[512];
			swprintf(wLine, 512, L"%d, %d, %d, %d, %d, %d", nCharID, nStoreType, 0, 0, 0, nOffline);
			wCmd += wLine;
			for(int n=0;n<8;n++)
			{
				swprintf(wLine, 512, L", %d , %d, %d, %d", nItems[n][0], nItems[n][1], nItems[n][2], nItems[n][3]);
				wCmd += wLine;
			}
			pDBConn->Execute(wCmd.c_str());
			delete pDBConn;
		}else
		{
			DBConn *pDBConn = new DBConn();
			wstring wCmd = L"EXEC lin_UpdatePrivateStore ";
			WCHAR wLine[512];
			swprintf(wLine, 512, L"%d, %d, %d, %d, %d, %d", nCharID, nStoreType, 0, 0, 0, nOffline);
			wCmd += wLine;
			for(int n=0;n<8;n++)
			{
				swprintf(wLine, 512, L", %d , %d, %d, %d", nItems[n][0], nItems[n][1], nItems[n][2], nItems[n][3]);
				wCmd += wLine;
			}
			pDBConn->Execute(wCmd.c_str());
			delete pDBConn;
		}
	}else
	{
		//Delete
		DBConn *pDBConn = new DBConn();
		pDBConn->Execute(L"EXEC lin_DeletePrivateStore %d", nCharID);
		delete pDBConn;
	}
	return false;
}

bool __cdecl PacketHandler::RequestEnchantItem(CQuerySocket *pQuerySocket, const unsigned char* packet)
{
	int nUserID = 0, nItemID = 0, nEnchant = 0, scrollId = 0;
	Disassemble(packet, "dddd", &nUserID, &nItemID, &nEnchant, &scrollId);
	CUserSP UserSP = g_UserDB.GetUser(nUserID, true);
	if(UserSP.pObject != 0)
	{
		CWareHouseSP WareHouseSP = UserSP->GetWareHouse(0, true);
		if(WareHouseSP.pObject != 0)
		{
			CItemSP ItemSP = WareHouseSP->GetItem(nItemID);
			if(ItemSP.pObject != 0)
			{
				if(nEnchant >= 0)
				{
					ItemSP->nEnchant = nEnchant;
					ItemSP->Save(true);
					//for testing
			//		g_Log.Add(CLog::Blue, "[%s] Item enchanted successful - enchat[%d] Item[%d] User[%d]", __FUNCTION__, nEnchant, nItemID, nUserID);
					char Buff[8192]; memset(Buff, 0, sizeof(Buff)); int nLen = 0;
					nLen = Assemble(Buff, 8192, "cddddd", 0x22, CO_REPLY_ENCHANT_ITEM, nUserID, nItemID, nEnchant, scrollId);
					pQuerySocket->Send(nLen, Buff);
				}else
					g_Log.Add(CLog::Error, "[%s] Invalid enchant[%d] for item[%d] user[%d]", __FUNCTION__, nEnchant, nItemID, nUserID);
			}else
				g_Log.Add(CLog::Error, "[%s] Cannot find item[%d] for user[%d]", __FUNCTION__, nItemID, nUserID);
		}else
			g_Log.Add(CLog::Error, "[%s] Cannot find WareHouse for user[%d]", __FUNCTION__, nUserID); 
	}else
		g_Log.Add(CLog::Error, "[%s] Cannot find user[%d].", __FUNCTION__, nUserID);

	return false;
}


int PacketHandler::AssembleItemPacket(PCHAR Buff, int Len, const char* format, int ItemDBID, int ItemType, int Amount, int Enchant, int Eroded, int Bless, int Ident, int Wished)
{
	int ManaLeft = 0;
	int Augmentation = 0;
	int LifeTime = 0;
	int ProtectionTimeout = 0;
	DBConn *pDBConn = new DBConn;
	pDBConn->Bind(&Augmentation);
	pDBConn->Bind(&ManaLeft);
	pDBConn->Bind(&LifeTime);
	pDBConn->Bind(&ProtectionTimeout);
	if(pDBConn->Execute(L"SELECT [augmentation], [mana_left], [life_time], [protection_timeout] from [user_item] where [item_id] = %d", ItemDBID))
	{
		pDBConn->Fetch();
	};
	delete pDBConn;
//	g_Log.AddU(CLog::Blue, L"[LoadItems] AugmentationID[%d] manaLeft[%d] ItemID[%d]", Augmentation, ManaLeft, ItemDBID);
	return Assemble(Buff, Len, "dddddddddddd", ItemDBID, ItemType, Amount, Enchant, Eroded, Bless, Ident, Wished, Augmentation, ManaLeft, LifeTime, ProtectionTimeout );
}

const unsigned char* PacketHandler::DisassembleSaveItemPacket(const unsigned char* packet, const char *format, LPINT pItemDBID, LPINT pCharID, LPINT pItemType, LPINT pAumount, LPINT pEnchant, LPINT pEroded, LPINT pBless, LPINT pIdent, LPINT pWished, LPINT pWarehouse)
{
	int nAugmentationID;
	int nManaLeft;
	int nLifeTime;
	int nProtectionTimeout;

	const unsigned char* Ret = Disassemble(packet, "dddddddddddddd", pItemDBID, pCharID, pItemType, pAumount, pEnchant, pEroded, pBless, pIdent, pWished, pWarehouse, &nAugmentationID, &nManaLeft, &nLifeTime, &nProtectionTimeout);

	CQuery query(L"UPDATE [user_item] SET [augmentation] = ?, [mana_left] = ?, [life_time] = ?, [protection_timeout] = ? WHERE [item_id] = ? AND [char_id] = ?");
	query.AddParam(nAugmentationID);
	query.AddParam(nManaLeft);
	query.AddParam(nLifeTime);
	query.AddParam(nProtectionTimeout);
	query.AddParam(*pItemDBID);
	query.AddParam(*pCharID);

	DBConn *pDBConn = new DBConn;
	pDBConn->Execute(query.Get());
	delete pDBConn;

	return Ret;
}

bool __cdecl PacketHandler::SaveCharacterPacket(CQuerySocket *pSocket, const unsigned char* packet)
{
	typedef bool (__cdecl *_f)(CQuerySocket*, const unsigned char*);
	_f f = (_f) 0x00469830;

	int nCharID = 0, nAugmentation = 0, nSpiritCount = 0, nHairDeco = 0, nHairAll = 0;
	int nEffectiveMana[4]; memset(nEffectiveMana, 0, sizeof(nEffectiveMana));
	PUCHAR pck = (PUCHAR)(packet + 212);
	
	Disassemble(packet, "d", &nCharID);
	Disassemble(pck, "dddddddd", &nAugmentation, &nSpiritCount, &nHairDeco, &nHairAll, &nEffectiveMana[0], &nEffectiveMana[1], &nEffectiveMana[2], &nEffectiveMana[3]);
	DBConn *pDBConn = new DBConn();
	pDBConn->Execute(L"UPDATE user_data set augmentation=%d, spirit_count=%d, ST_hair_deco=%d, ST_hair_all=%d, effective_mana0=%d, effective_mana1=%d, effective_mana2=%d, effective_mana3=%d WHERE char_id = %d", nAugmentation, nSpiritCount, nHairDeco, nHairAll, nEffectiveMana[0], nEffectiveMana[1], nEffectiveMana[2], nEffectiveMana[3], nCharID);
	delete pDBConn;
//	g_Log.AddU(CLog::Blue, L"Save Character[%d] with augmentation[%d]", nCharID, nAugmentation);


	return f(pSocket, packet);
}

int PacketHandler::AssembleLoadCharacterPacket(PCHAR buff, int len, const char* format, PWCHAR wsName,	int nCharID, PWCHAR wsAccount, DWORD Val1, DWORD Val2, DWORD Val3, DWORD Val4, DWORD Val5, DWORD Val6, DWORD Val7, DWORD Val8, DWORD Val9, DWORD Val10, double Val11, double Val12, DWORD Val13, DWORD Val14, DWORD Val15, DWORD Val16, DWORD Val17,
								 DWORD Val18 , DWORD Val19 , DWORD Val20 , DWORD Val21 , DWORD Val22 , DWORD Val23 , DWORD Val24 , DWORD Val25 , DWORD Val26 , DWORD Val27 , DWORD Val28 , DWORD Val29 , DWORD Val30 , DWORD Val31 , DWORD Val32 , DWORD Val33 , DWORD Val34 , DWORD Val35 , DWORD Val36 , DWORD Val37 , DWORD Val38 ,
								 DWORD Val39 , DWORD Val40 , DWORD Val41 , DWORD Val42 , DWORD Val43 , DWORD Val44 , DWORD Val45 , DWORD Val46 , DWORD Val47 , DWORD Val48 , DWORD Val49 , DWORD Val50 , DWORD Val51 , DWORD Val52 , DWORD Val53 , DWORD Val54 , DWORD Val55 , DWORD Val56 , DWORD Val57 , DWORD Val58 , DWORD Val59 ,
								 DWORD Val60 , double Val61 , double Val62 , DWORD Val63 , DWORD Val64 , DWORD Val65 , BYTE Val66)
{
	int nAugmentation = 0, nHairDecoType = 0, nHairDecoID = 0;
	int nEffectiveMana[4]; memset(nEffectiveMana, 0, sizeof(nEffectiveMana));
	DBConn *pDBConn = new DBConn();
	pDBConn->Bind(&nAugmentation);
	pDBConn->Bind(&nHairDecoID);
	pDBConn->Bind(&nEffectiveMana[0]);
	pDBConn->Bind(&nEffectiveMana[1]);
	pDBConn->Bind(&nEffectiveMana[2]);
	pDBConn->Bind(&nEffectiveMana[3]);
	if(pDBConn->Execute(L"SELECT augmentation, ST_hair_deco, effective_mana0, effective_mana1, effective_mana2, effective_mana3 FROM user_data where char_id = %d", nCharID))
	{
		pDBConn->Fetch();
	}
	delete pDBConn;
	if(nHairDecoID)
	{
		pDBConn = new DBConn();
		pDBConn->Bind(&nHairDecoType);
		if(pDBConn->Execute(L"SELECT item_type FROM user_item WHERE item_id = %d and char_id = %d", nHairDecoID, nCharID))
		{
			pDBConn->Fetch();
		}
		delete pDBConn;
	}

	if(g_SubStack)
	{
		UINT baseClass = UINT_MAX;
		CQuery query(L"SELECT [baseClass0] FROM [SubStack] WHERE [char_id] = ?");
		DBConn *pCon = new DBConn();
		query.AddParam(nCharID);
		pCon->Bind(&baseClass);
		if(pCon->Execute(query.Get()))
		{
			pCon->Fetch();
		}
		delete pCon;
	//	g_Log.Add(CLog::Blue, "[%s] baseClass[%d]", __FUNCTION__, baseClass);
		if(baseClass != UINT_MAX)
		{
	//		g_Log.Add(CLog::Blue, "[%s] baseClass[%d] vs [%d]", __FUNCTION__, baseClass, Val6);
			Val6 = baseClass;
		}
	}

	const PCHAR newFormat = "SdSddddddddddffddddddddddddddddddddddddddddddddddddddddddddddddddffdddcddddd";

	return Assemble(buff, len, newFormat, wsName, nCharID, wsAccount, Val1, Val2, Val3, Val4, Val5, Val6, Val7, Val8, Val9, Val10, Val11, Val12, Val13, Val14, Val15, Val16, Val17,
								 Val18 , Val19 , Val20 , Val21 , Val22 , Val23 , Val24 , Val25 , Val26 , Val27 , Val28 , Val29 , Val30 , Val31 , Val32 , Val33 , Val34 , Val35 , Val36 , Val37 , Val38 ,
								 Val39 , Val40 , Val41 , nHairDecoID, Val42 , Val43 , Val44 , Val45 , Val46 , Val47 , Val48 , Val49 , Val50 , Val51 , Val52 , Val53 , Val54 , Val55 , Val56 , Val57 , nHairDecoType, Val58 , Val59 ,
								 Val60 , Val61 , Val62 , Val63 , Val64 , Val65 , Val66, nAugmentation, nEffectiveMana[0], nEffectiveMana[1], nEffectiveMana[2], nEffectiveMana[3]);
}