#include "StdAfx.h"
#include "DB.h"
#include "VoteReward.h"
#include "VIPSystem.h"
#include "AccountDB.h"
#include "AIO.h"
#include "PrivateStore.h"
#include "ItemDelivery.h"
#include "FractionSystem.h"
#include "Auction.h"
#include "SchemeBuffer.h"
#include "ClanSys.h"
#include "Wedding.h"
#include "RaidBossStatus.h"
#include "CliExt.h"
#include "PKPvPStatus.h"
#include "CharacterLock.h"
#include "DailyPvP.h"
#include "L2NetworkVote.h"
#include "VisualArmor.h"
#include "PlayerAction.h"
#include "Rebirth.h"
#include "DBNpcMaker.h"
#include "ObjectDB.h"
#include "PIN.h"
#include "ClanPvPStatus.h"

void CDB::Send(const char *format, ...)
{
	va_list va; 
	va_start(va, format);
	int nRand = (rand() % 2);
	if(nRand)
		pInstance->pDBSocket1->SendV(format, va);
	else
		pInstance->pDBSocket2->SendV(format, va);
	va_end(va);
}

void CDB::RequestSavePledgePower(User *pUser)
{
	typedef void (*_rspp)(CDB*, User*);
	_rspp rspp = (_rspp) 0x0052E0C0;
	rspp(pInstance, pUser);
}

void CDB::RequestSaveCharacterInfo(User *pUser, bool notActive, bool force)
{
	typedef void (*f)(CDB*, User*, bool, bool);
	f(0x0053A250L)(pInstance, pUser, notActive, force);
}

void CDB::RequestSaveCharacterForSubJob(User *pUser)
{
	typedef void (*_rsci)(CDB*, User*);
	_rsci rsci = (_rsci) 0x00534280;
	rsci(pInstance, pUser);
}

void CDB::RequestSetPledgeMemberRank(int nPledgeID, PWCHAR wszName, int nRank)
{
	if(nPledgeID)
	{
		Send("chSdd", 0xF9, CI_SET_PLEDGE_MEMBER_RANK, wszName, nPledgeID, nRank);
	}
}

void CDB::RequestSetPledgePrivilege(int nPledgeID, int nRank, INT32 nPrivilege)
{
	if(nPledgeID)
	{
		Send("chddd", 0xF9, CI_SET_PLEDGE_PRIVILEGE, nPledgeID, nRank, nPrivilege);
	}
}

void CDB::RequestReorganizeMember(PWCHAR wszName, int nPledgeType, int nPledgeID)
{
	Send("chSdd", 0xF9, CI_REORGANIZE_MEMBER, wszName, nPledgeType, nPledgeID);
}

void CDB::RequestCreateExtPledgeInfo(int nPledgeID)
{
	if(nPledgeID)
	{
		Send("chd", 0xF9, CI_CREATE_EXT_PLEDGE_INFO, nPledgeID);
	}
}

void CDB::RequestSetPledgeReputation(int nPledgeID, int nReputation)
{
	if(nPledgeID)
	{
		Send("chdd", 0xF9, CI_SET_PLEDGE_REPUTATION, nPledgeID, nReputation);
	}
}

void CDB::RequestSetSponsor(int nCharID, int nSponsorID, int nPledgeID)
{
	if(nPledgeID)
	{
		Send("chddd", 0xF9, CI_SET_SPONSOR, nCharID, nSponsorID, nPledgeID);
	}
}

void CDB::SendAcquiredSkill(int charId, int skillId, int level, bool mayDown)
{
	typedef void(*f)(CDB*, int, int, int, bool);
	f(0x520A60L)(pInstance, charId, skillId, level, mayDown);
}

void CDB::RequestUpdateUserPledge(User *pUser, CPledge *pPledge, int nUpdateType)
{
	typedef void (__fastcall *_Update)(CDB*, User*, CPledge*, int nUpdateType);
	_Update Update = (_Update) 0x0051E220;
	Update(pInstance, pUser, pPledge, nUpdateType);
}

void CDB::RequestSetUserPledgeTypeAndRank(int nDBID, int nRank, int nPledgeType)
{
	Send("chddd", 0xF9, CI_SET_USER_PLEDGE_TYPE_N_RANK, nDBID, nRank, nPledgeType);
}

void CDB::RequestSetSubCaptain(int nPledgeID, int nPledgeType, PWCHAR wszName, int nCaptainDBID)
{
	if(nPledgeID && nPledgeType!=0 && nPledgeType!=-1)
	{
		Send("chddSd", 0xF9, CI_SET_SUB_CAPTAIN, nPledgeID, nPledgeType, wszName, nCaptainDBID);
	}
}

void CDB::RequestCreateSubPledge(int nPledgeID, int nPledgeType, PWCHAR wszName)
{
	if(nPledgeID && nPledgeType)
	{
		Send("chddS", 0xF9, CI_CREATE_SUBPLEDGE, nPledgeID, nPledgeType, wszName);
	}
}

void CDB::RequestRenameSubPledge(int nPledgeID, int nPledgeType, PWCHAR wName)
{
	if(nPledgeID && nPledgeType)
	{
		Send("chddS", 0xF9, CI_RENAME_SUBPLEDGE, nPledgeID, nPledgeType, wName);
	}
}

void CDB::RequestAcquirePledgeSkill(int nPledgeID, int nSkillID, int nLevel)
{
	if(nPledgeID && nSkillID > 369 && nSkillID < 392 && nLevel < 4)
	{
		Send("chddd", 0xF9, CI_REQUEST_ACQUIRE_PLEDGE_SKILL, nPledgeID, nSkillID, nLevel);
	}
}

void CDB::RequestEnchantItem(CItem *pScroll, CItem *pItem, int nEnchant, User *pUser)
{
	typedef void (__fastcall *_Update)(CDB*, CItem*, CItem*, int, User*);
	_Update Update = (_Update) 0x005210E0;
	Update(pInstance, pScroll, pItem, nEnchant, pUser);
}

void CDB::RequestEnchantFail(CItem *pScroll, CItem *pItem, User *pUser)
{
	typedef void (__fastcall *_Update)(CDB*, CItem*, CItem*, User*);
	_Update Update = (_Update) 0x00521330;
	Update(pInstance, pScroll, pItem, pUser);
}

void CDB::SaveQuestInfo(User *pUser)
{
	typedef void (__fastcall *_Update)(CDB*, User*);
	_Update Update = (_Update) 0x00538850;
	Update(pInstance, pUser);
}

void CDB::RequestSaveJoinPledgeLevel(int nUserDBID, int nLevel)
{
	if(nUserDBID)
	{
		Send("chdd", 0xF9, CI_SAVE_JOIN_PLEDGE_LEVEL, nUserDBID, nLevel);
	}
}

void CDB::RequestSaveHeroType(User *pUser, int nHeroType)
{
	typedef void (*_rsht)(CDB*, User*, int);
	_rsht rsht = (_rsht) 0x005392C0;
	rsht(pInstance, pUser, nHeroType);
}

void CDB::RequestRemoveHero(int nDBID)
{
	Send("chddddd", 0xF8, 0x1C, nDBID, 0, 0, 0, 0);
}

void CDB::RequestSaveNoblessType(User *pUser, int nType)
{
	typedef void (*_rsnt)(CDB*, User*, int);
	_rsnt rsnt = (_rsnt) 0x00539130;
	rsnt(pInstance, pUser, nType);
}

void CDB::RequestDestroyItem(CItem *pItem, int nAmount, User *pUser)
{
	typedef void (*_rdi)(CDB*, CItem*, int, User*);
	_rdi rdi = (_rdi) 0x00520ED0;
	rdi(pInstance, pItem, nAmount, pUser);
}

void CDB::RequestPackageSendToTarget(User *pUser, int nTargetDBID, int nAmount, int nBuffSize, PUCHAR Buff, int nNpcClassID)
{
	typedef void(*__rps)(CDB*, User*, int, int, int, PUCHAR, int);
	__rps _rps = (__rps) 0x0052B410;
	_rps(pInstance, pUser, nTargetDBID, nAmount, nBuffSize, Buff, nNpcClassID);
}

void CDB::RequestLoadWareHouseItems(int nDBID, User *pUser, int nWhID, CWareHouseWithdraw *pWH, int nNpcClassID, int nMemoryID)
{
	typedef void (*_load)(CDB*, int, User*, int, CWareHouseWithdraw*, int, int);
	_load load = (_load) 0x0051E5A0;
	load(pInstance, nDBID, pUser, nWhID, pWH, nNpcClassID, nMemoryID);
}

void CDB::RequestSaveItemDataEx(UINT charId, CItem *pItem)
{
	if(charId && pItem)
	{
		Send("chdddddd", 0xF9, CI_SAVE_ITEM_DATA_EX, charId, pItem->pSID->nDBID, pItem->nAugmentationID, pItem->nManaLeft, pItem->nLifeTime, pItem->nProtectionTimeout);
	}
}

void CDB::RequestDelegateClanMaster(int nPledgeID, int nCharID)
{
	if(nPledgeID)
	{
		Send("chdd", 0xF9, CI_REQUEST_DELEGATE_CLAN_MASTER, nPledgeID, nCharID);
	}
}

void CDB::RequestSavePledgeStat(PCHAR Buff, int Size, int MemberCount)
{
	if(Size > 0)
	{
		Send("chdb", 0xF9, CI_SAVE_PLEDGE_STAT, MemberCount, Size, Buff);
	}
}

void CDB::RequestSaveInstanceTime(UINT charId, UINT instanceId, UINT timeSpent, UINT reuseTimeout)
{
	Send("chdddd", 0xF9, CI_REQUEST_SAVE_INSTANCE_TIME, charId, instanceId, timeSpent, reuseTimeout);
}

void CDB::RequestLoadInstanceTime(UINT charId)
{
	Send("chd", 0xF9, CI_REQUEST_LOAD_INSTANCE_TIME, charId);
}

void CDB::RequestLoadL2NetworkVote(UINT charId, const WCHAR* wIdentity)
{
	Send("chdS", 0xF9, CI_REQUEST_LOAD_L2NETWORK_VOTE, charId, wIdentity);
}

void CDB::RequestSaveL2NetworkVote(const WCHAR* wIdentity, UINT lastVoteTime, UINT lastRewardTime)
{
	Send("chSdd", 0xF9, CI_REQUEST_SAVE_L2NETWORK_VOTE, wIdentity, lastVoteTime, lastRewardTime);
}

void CDB::RequestSaveVisualArmor(UINT charId, UINT armorId)
{
	Send("chdd", 0xF9, CI_REQUEST_SAVE_VISUAL_ARMOR, charId, armorId);
}

void CDB::RequestLoadVisualArmor(UINT charId)
{
	Send("chd", 0xF9, CI_REQUEST_LOAD_VISUAL_ARMOR, charId);
}

void CDB::RequestLoadDBNpcMaker()
{
	Send("ch", 0xF9, CI_REQUEST_LOAD_DB_NPC_MAKER);
}

void CDB::RequestLoadRebirth(UINT charId, UINT subjobId)
{
	Send("chdd", 0xF9, CI_REQUEST_LOAD_REBIRTH, charId, subjobId);
}

void CDB::RequestSaveRebirth(UINT charId, UINT subjobId, INT32 rebirthCount, INT32 availablePoints, INT32 bonusInt, INT32 bonusStr, INT32 bonusCon, INT32 bonusMen, INT32 bonusDex, INT32 bonusWit)
{
	Send("chdddddddddd", 0xF9, CI_REQUEST_SAVE_REBIRTH, charId, subjobId, rebirthCount, availablePoints, bonusInt, bonusStr, bonusCon, bonusMen, bonusDex, bonusWit);
}

void CDB::RequestSavePrivateStoreInfo(int nCharID, int nStoreType, StoreItemList& lItems, int nOffline, int nCreate)
{
	if(nCharID)
	{
		if(nStoreType == 0)
		{
			Send("chddddd", 0xF9, CI_SAVE_PRIVATE_STORE_INFO, nCharID, 0, 0, 0, 0);
		}else if(lItems.size() < 9)
		{
			CHAR Buff[8192];
			int nLen = 0;
			nLen += Assemble(Buff, (8192-nLen), "chddddd", 0xF9, CI_SAVE_PRIVATE_STORE_INFO, nCharID, nStoreType, nCreate, nOffline, (int)lItems.size());
			for(StoreItemList::iterator Iter = lItems.begin();Iter!=lItems.end();Iter++)
			{
				nLen += Assemble(&Buff[nLen], (8192-nLen), "dddd", Iter->nClassID, Iter->nPrice, Iter->nCount, Iter->nEnchant);
			}
			Send("b", nLen, Buff);
		}
	}
}

void CDB::RequestNewbieCheck(User *pUser)
{
	typedef void(*f)(CDB*, User*);
	f(0x00538F70)(pInstance, pUser);
}

void CDB::RequestLoadPledge(const WCHAR* wPledgeName)
{
	typedef void(*f)(CDB*, const WCHAR*);
	f(0x51F590L)(pInstance, wPledgeName);
}

void CDB::RequestModifyPledgeName(unsigned int pledgeId, const WCHAR* wPledgeName)
{
	typedef void (*f)(CDB*, unsigned int, const WCHAR*);
	f(0x524780L)(pInstance, pledgeId, wPledgeName);
}

void CDB::RequestLoadSpiritCount(int nCharID)
{
	if(nCharID)
	{
		Send("chd", 0xF9, CI_REQUEST_LOAD_SPIRIT_COUNT, nCharID);
	}
}

void CDB::RequestLoadVIPAccounts()
{
	Send("ch", 0xF9, CI_REQUEST_LOAD_VIP_ACCOUNTS);
}

void CDB::RequestLoadBannedAccounts()
{
	Send("ch", 0xF9, CI_REQUEST_LOAD_BANNED_ACCOUNTS);
}

void CDB::RequestBanAccount(UINT accountId, const WCHAR* wName, UINT timestamp, const WCHAR* wReason)
{
	Send("chdSdS", 0xF9, CI_REQUEST_BAN_ACCOUNT, accountId, wName, timestamp, wReason);
}

void CDB::RequestGetItemFromPet(User *pUser, CItem *pItem, INT32 itemCount)
{
	typedef void(*f)(CDB*, User*, CItem*, INT32);
	f(0x52A060L)(pInstance, pUser, pItem, itemCount);
}

void CDB::RequestSaveBuffList(User *pUser)
{
	SchemeBufferUser& sbu = pUser->pED->schemeBufferUser;
	if(sbu.saveList)
	{
		sbu.lock.Enter();
		CHAR buff[8190];
		int len = Assemble(buff, 8190, "ddd", pUser->nDBID, pUser->pSD->nSubClassIndex, sbu.buffList.size());
		for(UINT n=0;n<sbu.buffList.size();n++)
		{
			if(n < 48)
			{
				len += Assemble(&buff[len], (8190 - len), "d", sbu.buffList[n].pchId);
			}else
			{
				g_Log.Add(CLog::Error, "[%s] User[%S] 1 reached buff amount limit[%d]!", __FUNCTION__, pUser->pSD->wszName, sbu.buffList.size());
				break;
			}
		}
		len += Assemble(&buff[len], (8190 - len), "d", sbu.remoteBuffList.size());
		for(UINT n=0;n<sbu.remoteBuffList.size();n++)
		{
			if(n < 48)
			{
				len += Assemble(&buff[len], (8190 - len), "d", sbu.remoteBuffList[n].pchId);
			}else
			{
				g_Log.Add(CLog::Error, "[%s] User[%S] 2 reached buff amount limit[%d]!", __FUNCTION__, pUser->pSD->wszName, sbu.remoteBuffList.size());
				break;
			}
		}
		len += Assemble(&buff[len], (8190 - len), "d", sbu.summonBuffList.size());
		for(UINT n=0;n<sbu.summonBuffList.size();n++)
		{
			if(n < 48)
			{
				len += Assemble(&buff[len], (8190 - len), "d", sbu.summonBuffList[n].pchId);
			}else
			{
				g_Log.Add(CLog::Error, "[%s] User[%S] 3 reached buff amount limit[%d]!", __FUNCTION__, pUser->pSD->wszName, sbu.summonBuffList.size());
				break;
			}
		}
		len += Assemble(&buff[len], (8190 - len), "d", sbu.remoteSummonBuffList.size());
		for(UINT n=0;n<sbu.remoteSummonBuffList.size();n++)
		{
			if(n < 48)
			{
				len += Assemble(&buff[len], (8190 - len), "d", sbu.remoteSummonBuffList[n].pchId);
			}else
			{
				g_Log.Add(CLog::Error, "[%s] User[%S] 3 reached buff amount limit[%d]!", __FUNCTION__, pUser->pSD->wszName, sbu.remoteSummonBuffList.size());
				break;
			}
		}
		sbu.saveList = false;
		sbu.lock.Leave();
		Send("chb", 0xF9, CI_SAVE_BUFF_LIST, len, buff);
	}
};

void CDB::RequestLoadBuffList(int nCharID, int nSubjobIdx)
{
	if(nCharID)
	{
		Send("chdd", 0xF9, CI_REQUEST_LOAD_BUFF_LIST, nCharID, nSubjobIdx);
	}
};
void CDB::RequestSaveKillDeathStat(int nCharID, int nClassID1, int nLevel, int nCharID2, int nClassID2, int nLevel2, int KillerHP, int KillerMaxHP, int KillerCP, int KillerMaxCP, int x, int y, int z, int PvP, int SkillID, int SkillLevel)
{
	if(nCharID && nCharID2)
	{
		Send("chddddddddddddddddd", 0xF9, CI_SAVE_KILL_DEATH_STAT, nCharID, nClassID1, nLevel, nCharID2, nClassID2, nLevel2, KillerHP, KillerMaxHP, KillerCP, KillerMaxCP, x, y, z, PvP, SkillID, SkillLevel, time(NULL));
	}
}

void CDB::RequestSaveMultiSellLog(UINT charId, UINT accountId, PWCHAR wName, PWCHAR wAccount, INT32 price1Id, INT32 price1Count, INT32 price2Id, INT32 price2Count, INT32 price3Id, INT32 price3Count, INT32 price4Id, INT32 price4Count, INT32 price5Id, INT32 price5Count, INT32 reward1Id, INT32 reward1Count, INT32 reward2Id, INT32 reward2Count, INT32 reward3Id, INT32 reward3Count, INT32 reward4Id, INT32 reward4Count, INT32 reward5Id, INT32 reward5Count, UINT timestamp)
{
	Send("chddSSddddddddddddddddddddd", 0xF9, CI_REQUEST_SAVE_MULTISELL_LOG, charId, accountId, wName, wAccount, price1Id, price1Count, price2Id, price2Count, price3Id, price3Count, price4Id, price4Count, price5Id, price5Count, reward1Id, reward1Count, reward2Id, reward2Count, reward3Id, reward3Count, reward4Id, reward4Count, reward5Id, reward5Count, timestamp);
}

void CDB::RequestCustomEnchantItem(int nUserID, int nItemID, int nEnchant, int scrollId)
{
	if(nUserID > 0 && nItemID > 0 && nEnchant >= 0)
	{
		Send("chdddd", 0xF9, CI_REQUEST_ENCHANT_ITEM, nUserID, nItemID, nEnchant, scrollId);
	}
}

void CDB::RequestSaveAIO(int charId, UINT id, UINT timestamp)
{
	if(charId)
	{
		Send("chddd", 0xF9, CI_REQUEST_SAVE_AIO, charId, id, timestamp);
	}
}

void CDB::RequestLoadAIO(int charId)
{
	if(charId)
	{
		Send("chd", 0xF9, CI_REQUEST_LOAD_AIO, charId);
	}
}

void CDB::RequestLoadVipStatus(int charId)
{
	if(charId)
	{
		Send("chd", 0xF9, CI_REQUEST_LOAD_VIP_STATUS, charId);
	}
}

void CDB::RequestLoadVotePoints(int accountId, int charId)
{
	if(charId)
	{
		Send("chdd", 0xF9, CI_REQUEST_LOAD_VOTE_POINTS, accountId, charId);
	}
}

void CDB::RequestSaveVotePoints(int accountId, int charId, int points)
{
	Send("chddd", 0xF9, CI_REQUEST_SAVE_VOTE_POINTS, accountId, charId, points);
}

void CDB::RequestSaveVipStatus(int charId, int vipLevel, DWORD vipTimestamp)
{
	if(charId)
	{
		Send("chddd", 0xF9, CI_REQUEST_SAVE_VIP_STATUS, charId, vipLevel, vipTimestamp);
	}
}

void CDB::RequestDeleteItems(DWORD UniqueId, int RequestType, DWORD StartTick, list<int>& lItems)
{
	if(lItems.size() > 0)
	{
		CHAR Buff[8000];
		int nLen = 0;
		int ItemCount = 0;
		for(list<int>::iterator Iter=lItems.begin();Iter!=lItems.end();Iter++)
		{
			if(int nItemID = (*Iter))
			{
				nLen += Assemble(&Buff[nLen], (8190 - nLen), "d", nItemID);
				ItemCount++;
			}
		}
		if(ItemCount > 0)
		{
			Send("chddddb", 0xF9, CI_REQUEST_DELETE_ITEMS, UniqueId, RequestType, StartTick, ItemCount, nLen, Buff);
		}
	}
}

void CDB::RequestOwnAgit(int ResidenceId, int PledgeId)
{
	typedef void(*f)(CDB*, int, int);
	f(0x51FA80L)(pInstance, ResidenceId, PledgeId);
}

void CDB::RequestRestoreCH(int pledgeId, int lostChId)
{
	if(pledgeId)
	{
		Send("chdd", 0xF9, CI_REQUEST_RESTORE_CH, pledgeId, lostChId);
	}
}

void CDB::RequestLoadNpcDeath()
{
	Send("ch", 0xF9, CI_REQUEST_LOAD_NPC_DEATH);
}

void CDB::RequestSaveNpcDeath(int npc_class_id, DWORD death_time)
{
	Send("chdd", 0xF9, CI_REQUEST_SAVE_NPC_DEATH, npc_class_id, death_time);
}

void CDB::RequestLockAccount(UINT accountId, LPBYTE lpHwid, WCHAR *wName)
{
	Send("chdbS", 0xF9, CI_REQUEST_LOCK_ACCOUNT, accountId, 32, lpHwid, wName);
}

void CDB::RequestUnlockAccount(UINT accountId)
{
	Send("chd", 0xF9, CI_REQUEST_UNLOCK_ACCOUNT, accountId);
}

void CDB::RequestLockChar(UINT charId, LPBYTE lpHwid, WCHAR *wName)
{
	Send("chdbS", 0xF9, CI_REQUEST_LOCK_CHAR, charId, 32, lpHwid, wName);
}

void CDB::RequestUnlockChar(UINT charId)
{
	Send("chd", 0xF9, CI_REQUEST_UNLOCK_CHAR, charId);
}

void CDB::RequestLoadHwidLockData()
{
	Send("ch", 0xF9, CI_REQUEST_LOAD_HWID_LOCK_DATA);
}

void CDB::RequestSaveUserFraction(User *pUser)
{
	if(pUser->ValidUser())
	{
		Send("chdddd", 0xF9, CI_REQUEST_SAVE_USER_FRACTION, pUser->nDBID, pUser->pED->fractionUser.fractionId, pUser->pED->fractionUser.points, pUser->pED->fractionUser.leaveTimestamp);
	}
}

void CDB::RequestLoadUserFraction(UINT charId)
{
	Send("chd", 0xF9, CI_REQUEST_LOAD_USER_FRACTION, charId);
}

void CDB::RequestSetAdditionalBox(UINT accountId, UINT timestamp)
{
	Send("chdd", 0xF9, CI_REQUEST_SET_ADDITIONAL_BOX, accountId, timestamp);
}

void CDB::RequestLoadAdditionalBoxData()
{
	Send("ch", 0xF9, CI_REQUEST_LOAD_ADDITIONAL_BOX_DATA);
}

void CDB::RequestSaveFractionData(UINT fractionId, INT32 points, UINT onlineMembers)
{
	Send("chddd", 0xF9, CI_REQUEST_SAVE_FRACTION_DATA, fractionId, points, onlineMembers);
}

void CDB::RequestLoadFractionData(UINT fractionId)
{
	Send("chd", 0xF9, CI_REQUEST_LOAD_FRACTION_DATA, fractionId);
}

void CDB::RequestItemDeliveryAsk(UINT charId)
{
	Send("chd", 0xF9, CI_REQUEST_ITEM_DELIVERY_ASK, charId);
}

void CDB::RequestItemDeliveryDone(UINT charId, UINT jobId)
{
	Send("chdd", 0xF9, CI_REQUEST_ITEM_DELIVERY_DONE, charId, jobId);
}

void CDB::RequestChangePos(WCHAR *wName, UINT worldId, INT32 x, INT32 y, INT32 z)
{
	typedef void(*f)(CDB*, WCHAR*, UINT, INT32, INT32, INT32);
	f(0x522350L)(pInstance, wName, worldId, x, y, z);
}

void CDB::RequestClassChangedJob(User *pUser)
{
	Send("chddd", 0xF8, 0x22, pUser->nDBID, pUser->pSD->nSubClassIndex, pUser->pSD->nClass);
}

void CDB::RequestSaveSubjob(UINT charId, INT32 subjobId, double hp, double mp, int exp, int sp, int level, int henna1, int henna2, int henna3)
{
	Send("chddffdddddd", 0xF9, CI_REQUEST_SAVE_SUBJOB, charId, subjobId, hp, mp, exp, sp, level, henna1, henna2, henna3);
}

void CDB::RequestCreateAuction(UINT sellerId, const WCHAR* wSeller, int itemId, int amount, int enchant, UINT augmentation, int price, UINT expireTime)
{
	Send("chdSdddddd", 0xF9, CI_REQUEST_CREATE_AUCTION, sellerId, wSeller, itemId, amount, enchant, augmentation, price, expireTime);
}

void CDB::RequestModdifyAuctionAmount(UINT auctionId, int newAmount)
{
	Send("chdd", 0xF9, CI_REQUEST_MODDIFY_AUCTION_AMOUNT, auctionId, newAmount);
}

void CDB::RequestDeleteAuction(UINT auctionId)
{
	Send("chd", 0xF9, CI_REQUEST_DELETE_AUCTION, auctionId);
}

void CDB::RequestLoadAuctionItems()
{
	Send("ch", 0xF9, CI_REQUEST_LOAD_AUCTION_ITEMS);
}

void CDB::RequestAuctionPaymentAsk(UINT charId)
{
	Send("chd", 0xF9, CI_REQUEST_AUCTION_PAYMENT_ASK, charId);
}

void CDB::RequestAuctionPaymentDone(UINT charId, UINT jobId)
{
	Send("chdd", 0xF9, CI_REQUEST_AUCTION_PAYMENT_DONE, charId, jobId);
}

void CDB::RequestSetCharacterPIN(UINT charId, UINT pin, UINT timestamp)
{
	Send("chddd", 0xF9, CI_REQUEST_SET_CHARACTER_PIN, charId, pin, timestamp);
}

void CDB::RequestLoadCharacterPINS()
{
	Send("ch", 0xF9, CI_REQUEST_LOAD_CHARACTER_PINS);
}

void CDB::RequestLoadClanPvPStatus(UINT pledgeLevel, UINT topLimit)
{
	Send("chdd", 0xF9, CI_REQUEST_LOAD_CLAN_PVP_STATUS, pledgeLevel, topLimit);
}

void CDB::RequestAuctionAddPaymentJob(UINT sellerId, int priceItemId, int priceAmount, int itemId, int amount, const WCHAR* wBuyer)
{
	Send("chdddddS", 0xF9, CI_REQUEST_AUCTION_ADD_PAYMENT_JOB, sellerId, priceItemId, priceAmount, itemId, amount, wBuyer);
}

void CDB::RequestLoadPledgeSkills(UINT pledgeId)
{
	Send("chd", 0xF9, CI_REQUEST_LOAD_PLEDGE_SKILLS, pledgeId);
}

void CDB::RequestAddPledgeSkill(UINT pledgeId, INT32 skillId, INT32 skillLevel)
{
	Send("chddd", 0xF9, CI_REQUEST_ADD_PLEDGE_SKILL, pledgeId, skillId, skillLevel);
}

void CDB::RequestSaveWeddingStatus(UINT userId, UINT state, UINT partnerId, const WCHAR *wPartnerName, UINT penaltyTimeout, UINT teleportTimeout, UINT weddingTime)
{
	Send("chdddSddd", 0xF9, CI_REQUEST_SAVE_WEDDING_STATUS, userId, state, partnerId, wPartnerName, penaltyTimeout, teleportTimeout, weddingTime);
}

void CDB::RequestLoadWeddingStatus(UINT userId)
{
	Send("chd", 0xF9, CI_REQUEST_LOAD_WEDDING_STATUS, userId);
}

void CDB::RequestTestQuery()
{
	Send("chdd", 0xF9, CI_REQUEST_TEST_QUERY, GetTickCount(), time(0));
}

void CDB::RequestLoadRaidBossStatus()
{
	Send("ch", 0xF9, CI_REQUEST_LOAD_RB_STATUS);
}

void CDB::RequestLoadPKStatus()
{
	Send("ch", 0xF9, CI_REQUEST_LOAD_PK_STATUS);
}

void CDB::RequestLoadPvPStatus()
{
	Send("ch", 0xF9, CI_REQUEST_LOAD_PVP_STATUS);
}

void CDB::RequestSetNextCastleSiege(User *pUser, CCastle *pCastle, UINT nextSiegeTime)
{
	typedef void(*f)(CDB*, User*, CCastle*, UINT);
	f(0x5260C0L)(pInstance, pUser, pCastle, nextSiegeTime);
}

void CDB::RequestSetNextAgitBattle(CResidence *pResidence)
{
	typedef void(*f)(CDB*, CResidence*);
	f(0x5270D0L)(pInstance, pResidence);
}

void CDB::RequestLoadBannedHardwareId()
{
	Send("ch", 0xF9, CI_REQUEST_LOAD_BANNED_HARDWARE_ID);
}

void CDB::RequestSaveHardwareId(UINT charId, LPBYTE lpHwid)
{
	Send("chdb", 0xF9, CI_REQUEST_SAVE_HARDWARE_ID, charId, 32, lpHwid);
}

void CDB::RequestBanHardwareId(LPBYTE lpHwid, const WCHAR* wAccount, const WCHAR* wGMName, UINT timeout)
{
	Send("chbSSd", 0xF9, CI_REQUEST_BAN_HARDWARE_ID, 32, lpHwid, wAccount, wGMName, timeout);
}

void CDB::RequestUnBanHardwareId(LPBYTE lpHwid)
{
	Send("chb", 0xF9, CI_REQUEST_UNBAN_HARDWARE_ID, 32, lpHwid);
}

void CDB::RequestLoadNameTitleColor(UINT charId)
{
	Send("chd", 0xF9, CI_REQUEST_LOAD_NAME_TITLE_COLOR, charId);
}

void CDB::RequestLoadCharacterLock(UINT charId)
{
	Send("chd", 0xF9, CI_REQUEST_LOAD_CHARACTER_LOCK, charId);
}

void CDB::RequestSaveCharacterLock(UINT charId, LPBYTE lpHash, LPBYTE lpHWID)
{
	Send("chdbb", 0xF9, CI_REQUEST_SAVE_CHARACTER_LOCK, charId, 32, lpHash, 32, lpHWID);
}

void CDB::RequestSaveDailyPvP(UINT charId, INT32 amount, UINT timestamp)
{
	Send("chddd", 0xF9, CI_REQUEST_SAVE_DAILY_PVP, charId, amount, timestamp);
}

void CDB::RequestSaveDailyPvPEntry(UINT charId, LPBYTE lpHWID, UINT timestamp)
{
	Send("chdbd", 0xF9, CI_REQUEST_SAVE_DAILY_PVP_ENTRY, charId, 32, lpHWID, timestamp);
}

void CDB::RequestLoadDailyPvPEntry()
{
	Send("ch", 0xF9, CI_REQUEST_LOAD_DAILY_PVP_ENTRY);
}

void CDB::RequestLoadDailyPvP(UINT charId)
{
	Send("chd", 0xF9, CI_REQUEST_LOAD_DAILY_PVP, charId);
}

void CDB::RequestSaveSubStack(UINT charId, UINT subjobIndex, UINT baseClass, UINT stackClass)
{
	Send("chdddd", 0xF9, CI_REQUEST_SAVE_SUBSTACK, charId, subjobIndex, baseClass, stackClass);
}

void CDB::RequestLoadSubStack(UINT charId)
{
	Send("chd", 0xF9, CI_REQUEST_LOAD_SUBSTACK, charId);
}

//Replies from cached

DBPacketCallback g_DBPacketTable[256];

//0x22 base packet opcode
void CDBPacket::Init()
{
	g_HookManager.WriteMemoryDWORD(0xBEFD80, (DWORD)CDBPacket::Handler);
	for(UINT n=0;n<256;n++)
	{
		g_DBPacketTable[n] = CDBPacket::DummyPacket;
	}

	Bind( CO_REPLY_LOAD_SPIRIT_COUNT, CDBPacket::ReplyLoadSpiritCount);
	Bind( CO_REPLY_LOAD_BUFF_LIST, CDBPacket::ReplyLoadBuffList);
	Bind( CO_REPLY_ENCHANT_ITEM, CDBPacket::ReplyCustomEnchantItem);
	Bind( CO_REPLY_LOAD_AIO, CDBPacket::ReplyLoadAIO);
	Bind( CO_REPLY_RESTORE_CH, CDBPacket::ReplyRestoreCH);
	Bind( CO_REPLY_LOAD_NPC_DEATH, CDBPacket::ReplyLoadNpcDeath);
	Bind( CO_REPLY_LOAD_VIP_STATUS, CDBPacket::ReplyLoadVipStatus);
	Bind( CO_REPLY_LOAD_VIP_ACCOUNTS, CDBPacket::ReplyLoadVIPAccounts);
	Bind( CO_REPLY_LOAD_VOTE_POINTS, CDBPacket::ReplyLoadVotePoints);
	Bind( CO_REPLY_LOAD_BANNED_ACCOUNTS, CDBPacket::ReplyLoadBannedAccounts);
	Bind( CO_REPLY_LOAD_HWID_LOCK_DATA, CDBPacket::ReplyLoadHwidLockData);
	Bind( CO_REPLY_LOAD_ADDITIONAL_BOX_DATA, CDBPacket::ReplyLoadAdditionalBox);
	Bind( CO_REPLY_LOAD_USER_FRACTION, CDBPacket::ReplyLoadUserFraction);
	Bind( CO_REPLY_ITEM_DELIVERY_ASK, CDBPacket::ReplyItemDeliveryAsk);
	Bind( CO_REPLY_LOAD_FRACTION_DATA, CDBPacket::ReplyLoadFractionData);
	Bind( CO_REPLY_CREATE_AUCTION, CDBPacket::ReplyCreateAuction);
	Bind( CO_REPLY_LOAD_AUCTION_ITEM, CDBPacket::ReplyLoadAuctionItem);
	Bind( CO_REPLY_AUCTION_PAYMENT_ASK, CDBPacket::ReplyAuctionPaymentAsk);
	Bind( CO_REPLY_LOAD_PLEDGE_SKILLS, CDBPacket::ReplyLoadPledgeSkills);
	Bind( CO_REPLY_LOAD_WEDDING_STATUS, CDBPacket::ReplyLoadWeddingStatus);
	Bind( CO_REPLY_TEST_QUERY, CDBPacket::ReplyTestQuery);
	Bind( CO_REPLY_LOAD_RB_STATUS, CDBPacket::ReplyLoadRaidBossStatus);
	Bind( CO_REPLY_LOAD_BANNED_HARDWARE_ID, CDBPacket::ReplyLoadBannedHardwareId);
	Bind( CO_REPLY_LOAD_PK_STATUS, CDBPacket::ReplyLoadPKStatus);
	Bind( CO_REPLY_LOAD_PVP_STATUS, CDBPacket::ReplyLoadPvPStatus);
	Bind( CO_REPLY_LOAD_NAME_TITLE_COLOR, CDBPacket::ReplyLoadNameTitleColor);
	Bind( CO_REPLY_LOAD_CHARACTER_LOCK, CDBPacket::ReplyLoadCharacterLock);
	Bind( CO_REPLY_LOAD_DAILY_PVP_ENTRY, CDBPacket::ReplyLoadDailyPvPEntry);
	Bind( CO_REPLY_LOAD_DAILY_PVP, CDBPacket::ReplyLoadDailyPvP);
	Bind( CO_REPLY_LOAD_SUBSTACK, CDBPacket::ReplyLoadSubStack);
	Bind( CO_REPLY_LOAD_INSTANCE_TIME, CDBPacket::ReplyLoadInstanceTime);
	Bind( CO_REPLY_LOAD_L2NETWORK_VOTE, CDBPacket::ReplyLoadL2NetworkVote);
	Bind( CO_REPLY_LOAD_VISUAL_ARMOR, CDBPacket::ReplyLoadVisualArmor);
	Bind( CO_REPLY_LOAD_REBIRTH, CDBPacket::ReplyLoadRebirth);
	Bind( CO_REPLY_LOAD_DB_NPC_MAKER, CDBPacket::ReplyLoadDBNpcMaker);
	Bind( CO_REPLY_LOAD_CHARACTER_PIN, CDBPacket::ReplyLoadCharacterPIN);
	Bind( CO_REPLY_LOAD_CLAN_PVP_STATUS, CDBPacket::ReplyLoadPledgeTotalPvP);
}

void CDBPacket::Bind(UINT id, DBPacketCallback func)
{
	if(id < 256)
	{
		g_DBPacketTable[id] = func;
	}
}

bool CDBPacket::DummyPacket(CDBSocket *pSocket, const unsigned char* packet, UINT id)
{
	g_Log.Add(CLog::Error, "[%s] Dummy packet[%d] called in Custom DB Packet Handler!", __FUNCTION__, id);
	return false;
}

bool CDBPacket::Handler(CDBSocket *pSocket, const unsigned char* packet)
{
	guard;
	UINT packetId;
	packet = Disassemble(packet, "d", &packetId);
	//g_Log.Add(CLog::Blue, "[%s] called opcode[%d]", __FUNCTION__, nOpCode);

	if(packetId <256)
	{
		g_DBPacketTable[packetId](pSocket, packet, packetId);
	}

	unguard;
	return false;
}

bool CDBPacket::ReplyLoadPledgeTotalPvP(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	CTL;

	UINT pledgeId = 0, pvpCount = 0;
	WCHAR pledgeName[25] = { 0 };
	Disassemble(packet, "dSd", &pledgeId, sizeof(pledgeName), pledgeName, &pvpCount);


	if(g_ClanPvPStatus.Enabled())
	{
		g_ClanPvPStatus.OnDBLoadPvP(pledgeId, pledgeName, pvpCount);
	}

	return false;
}

bool CDBPacket::ReplyLoadCharacterPIN(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	UINT charId = 0, pin = 0, timestamp = 0;
	Disassemble(packet, "ddd", &charId, &pin, &timestamp);

	g_PINSystem.DBLoad(charId, pin, timestamp);

	return false;
}

bool CDBPacket::ReplyLoadDBNpcMaker(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	guard;

	UINT npcId = 0, startTime = 0, endTime = 0, heading = 0;
	INT32 x = 0, y = 0, z = 0;

	Disassemble(packet, "ddddddd", &npcId, &x, &y, &z, &heading, &startTime, &endTime);

	npcId += 1000000;
	const WCHAR *wName = g_ObjectDB.GetName(npcId);

	g_DBNpcMaker.OnDBLoad(npcId, wName != 0 ? wName : L"", startTime, endTime, x, y, z, heading);

	unguard;
	return false;
}

bool CDBPacket::ReplyLoadRebirth(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	guard;
	UINT charId = 0, subjobId = 0;
	INT32 rebirthCount = 0, availablePoints = 0, bonusInt = 0, bonusStr = 0, bonusCon = 0, bonusMen = 0, bonusDex = 0, bonusWit = 0;

	Disassemble(packet, "dddddddddd", &charId, &subjobId, &rebirthCount, &availablePoints, &bonusInt, &bonusStr, &bonusCon, &bonusMen, &bonusDex, &bonusWit);

	if(User *pUser = g_UserDB.GetUserByDBID(charId)->SafeCastUser())
	{
		if(g_RebirthSystem.Enabled())
		{
			RebirthUser& ru = pUser->pED->rebirthUser;
			if(pUser->pSD->nSubClassIndex == subjobId)
			{
				ru.availablePoints = availablePoints;
				ru.rebirthCount = rebirthCount;
				ru.bonusCon = bonusCon;
				ru.bonusDex = bonusDex;
				ru.bonusInt = bonusInt;
				ru.bonusMen = bonusMen;
				ru.bonusStr = bonusStr;
				ru.bonusWit = bonusWit;

				pUser->ValidateParameters();
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Invalid subjobId[%d] - User[%S] subjobId[%d]!", __FUNCTION__, subjobId, pUser->pSD->wszName, pUser->pSD->nSubClassIndex);
				g_DB.RequestLoadRebirth(pUser->nDBID, pUser->pSD->nSubClassIndex);
			}
		}
	}

	unguard;
	return false;
}

bool CDBPacket::ReplyLoadVisualArmor(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	guard;

	UINT charId = 0, armorId = 0;
	Disassemble(packet, "dd", &charId, &armorId);

	if(User *pUser = g_UserDB.GetUserByDBID(charId)->SafeCastUser())
	{
		if(VisualArmorInfo* pV = g_VisualArmor.Get(armorId))
		{
			VisualArmorUser& vau = pUser->pED->visualArmorUser;
			vau.armorId = armorId;
			vau.armorType = pV->armorType;
			vau.chestSlotId = pV->chestSlotId;
			vau.feetSlotId = pV->feetSlotId;
			vau.glovesSlotId = pV->glovesSlotId;
			vau.legsSlotId = pV->legsSlotId;
			vau.maxLevel = pV->maxLevel;
			vau.minLevel = pV->minLevel;
			PlayerAction::OnTeleportToLocation(pUser, static_cast<int>(pUser->pSD->Pos.x), static_cast<int>(pUser->pSD->Pos.y), static_cast<int>(pUser->pSD->Pos.z), 0);
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Invalid armor_id[%d] for user[%S]!", __FUNCTION__, armorId, pUser->pSD->wszName);
		}
	}


	unguard;
	return false;
}

bool CDBPacket::ReplyLoadL2NetworkVote(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	guard;

	UINT charId = 0, lastVoteTime = 0, lastRewardTime = 0;
	WCHAR wIdentity[33] = { 0 };

	Disassemble(packet, "dSdd", &charId, sizeof(wIdentity), wIdentity, &lastVoteTime, &lastRewardTime);

//	g_Log.Add(CLog::Blue, "[%s] charId[%d] identity[%S] lastVote[%d] lastReward[%d] current[%d]", __FUNCTION__, charId, wIdentity, lastVoteTime, lastRewardTime);

	if(User *pUser = g_UserDB.GetUserByDBID(charId)->SafeCastUser())
	{
		L2NetworkUser& lu = pUser->pED->l2networkUser;
		lu.lastRewardTime = lastRewardTime;
		lu.lastVoteTime = lastVoteTime;
		lu.loaded = true;
		wcscpy(lu.identity, wIdentity);

		if(g_L2NetworkVote.CheckOnEnterWorld())
		{
			g_L2NetworkVote.OnCheck(pUser);
		}
	}

	unguard;
	return false;
}

bool CDBPacket::ReplyLoadInstanceTime(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	guard;

	UINT charId = 0, instanceId = 0, spentTime = 0, reuseTimeout = 0;
	Disassemble(packet, "dddd", &charId, &instanceId, &spentTime, &reuseTimeout);

	if(User *pUser = g_UserDB.GetUserByDBID(charId)->SafeCastUser())
	{
		time_t currentTime = time(0);
	//	g_Log.Add(CLog::Blue, "[%s] reuse[%d] currentTime[%d]", __FUNCTION__, reuseTimeout, currentTime);
		if(reuseTimeout > 0 && currentTime < reuseTimeout)
		{
			//instance in reuse
			InstanceData id;
			id.incTimeTick = 0;
			id.instanceId = instanceId;
			id.reuseTimeout = reuseTimeout;
			id.saved = true;
			id.timeSpent = spentTime * 1000;

			//check if already exist if so then replace
			bool updated = false;
			for(UINT n=0;n<pUser->pED->instances.size();n++)
			{
				InstanceData& tmp = pUser->pED->instances[n];
				if(tmp.instanceId == id.instanceId)
				{
					tmp.reuseTimeout = id.reuseTimeout;
					tmp.timeSpent = id.timeSpent;
					tmp.incTimeTick = GetTickCount();
					updated = true;
					break;
				}
			}
			if(!updated)
			{
				pUser->pED->instances.push_back(id);
			}
		}
	}

	unguard;
	return false;
}

bool CDBPacket::ReplyLoadSubStack(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	guard;

	UINT charId = 0;
	UINT classId[4][2] = { 0 };
	Disassemble(packet, "ddddddddd", &charId, &classId[0][0], &classId[1][0], &classId[2][0], &classId[3][0], &classId[0][1], &classId[1][1], &classId[2][1], &classId[3][1]); 

	if(User *pUser = g_UserDB.GetUserByDBID(charId)->SafeCastUser())
	{
		memcpy(pUser->pED->subStackInfo.classId, classId, sizeof(classId));
	}
	unguard;
	return false;
}

bool CDBPacket::ReplyLoadDailyPvP(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	guard;
	UINT charId = 0, timestamp = 0;
	INT32 points = 0;

	Disassemble(packet, "ddd", &charId, &points, &timestamp);

	if(User *pUser = g_UserDB.GetUserByDBID(charId))
	{
		//date (timestamp) will get verified by CDailyPvP::TimerExpired function
		pUser->pED->dailyPvPAmount = points;
		pUser->pED->dailyPvPTimestamp = timestamp;
		g_DailyPvP.ShowPoints(pUser);
	}

	unguard;
	return false;
}

bool CDBPacket::ReplyLoadDailyPvPEntry(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	guard;
	UINT counter = 0;
	packet = Disassemble(packet, "d", &counter);
	for(UINT n=0;n<counter;n++)
	{
		UINT charId = 0, timestamp = 0;
		BYTE hwid[32];
		packet = Disassemble(packet, "dbd", &charId, 32, hwid, &timestamp);
		g_DailyPvP.OnDBLoad(charId, hwid, timestamp);
	}
	unguard;
	return false;
}

bool CDBPacket::ReplyLoadCharacterLock(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	guard;

	UINT charId = 0;
	packet = Disassemble(packet, "d", &charId);
	BYTE hash[32];
	memcpy(hash, packet, 32);
	g_CharacterLock.OnDBLoad(charId, hash);

	unguard;
	return false;
}

bool CDBPacket::ReplyLoadNameTitleColor(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	guard;
	UINT charId = 0, nameColor = 0, titleColor = 0;
	Disassemble(packet, "ddd", &charId, &nameColor, &titleColor);

	if(User *pUser = g_UserDB.GetUserByDBID(charId))
	{
		pUser->pED->nameColorEx = nameColor;
		pUser->pED->titleColorEx = titleColor;
		pUser->SetCharInfoChanged();
		pUser->SetUserInfoChanged();
	}

	unguard;
	return false;
}

bool CDBPacket::ReplyLoadPKStatus(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	guard;
	UINT charId = 0, classId = 0;
	INT32 level = 0, pkAmount = 0;
	WCHAR wName[25] = { 0 };
	Disassemble(packet, "dSddd", &charId, sizeof(wName), wName, &classId, &level, &pkAmount);
	if(charId > 0)
	{
		g_PKPvPStatus.OnDBLoadPK(charId, wName, classId, level, 0, pkAmount);
	}

	unguard;
	return false;
}

bool CDBPacket::ReplyLoadPvPStatus(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	guard;
	UINT charId = 0, classId = 0;
	INT32 level = 0, pvpAmount = 0;
	WCHAR wName[25] = { 0 };
	Disassemble(packet, "dSddd", &charId, sizeof(wName), wName, &classId, &level, &pvpAmount);
	if(charId > 0)
	{
		g_PKPvPStatus.OnDBLoadPvP(charId, wName, classId, level, pvpAmount, 0);
	}

	unguard;
	return false;
}

bool CDBPacket::ReplyLoadBannedHardwareId(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	guard;
	BYTE hwid[32];
	WCHAR account[25] = { 0 };
	WCHAR gm[25] = { 0 };
	UINT timeout = 0;
	for(UINT n=0;n<32;n++)
	{
		hwid[n] = (*packet);
		packet++;
	}

	Disassemble(packet, "SSd", sizeof(account), account, sizeof(gm), gm, &timeout);

	g_CliExt.OnBanDBLoad(hwid, account, gm, timeout);

	unguard;
	return false;
}

bool CDBPacket::ReplyLoadRaidBossStatus(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	guard;
	WCHAR wName[128] = { 0 };
	int x, y, z;
	BOOL alive = false;
	UINT deathTime = 0;
	Disassemble(packet, "Sddddd", sizeof(wName), wName, &x, &y, &z, &alive, &deathTime);

	g_RaidBossStatus.OnDBLoad(wName, x, y, z, alive, deathTime);
	unguard;
	return false;
}

bool CDBPacket::ReplyTestQuery(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	guard;
	UINT startTick = 0, startTime = 0;
	UINT receiveTick = 0, receiveTime = 0;
	UINT replyTick = 0, replyTime = 0;
	UINT currentTick = GetTickCount();
	UINT currentTime = time(0);
	Disassemble(packet, "dddddd", &startTick, &startTime, &receiveTick, &receiveTime, &replyTick, &replyTime);

	g_Logger.Add(L"CacheD connection test: startTick[%d] startTime[%d] receiveTick[%d] receiveTime[%d] replyTick[%d] replyTime[%d] currentTick[%d] currentTime[%d]", startTick, startTime, receiveTick, receiveTime, replyTick, replyTime, currentTick, currentTime);
	g_Logger.Add(L"Cached connection test: L2S->C diff[%d]ms [%d]s, CS->CE diff[%d]ms [%d]s, C->L2S diff[%d]ms [%d]s, Total diff[%d]ms [%d]s.", (receiveTick-startTick), (receiveTime-startTime), (replyTick-receiveTick), (replyTime-receiveTime), (currentTick-replyTick), (currentTime-replyTime), (currentTick-startTick), (currentTime-startTime));

	unguard;
	return false;
}

bool CDBPacket::ReplyLoadWeddingStatus(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	guard;
	UINT userId = 0, partnerId = 0;
	WCHAR wPartnerName[25] = { 0 };
	UINT state, penaltyTimeout, teleportTimeout, weddingTime;
	Disassemble(packet, "dddSddd", &userId, &state, &partnerId, sizeof(wPartnerName), wPartnerName, &penaltyTimeout, &teleportTimeout, &weddingTime);
//	g_Log.Add(CLog::Blue, "[%s] charId[%d] state[%d] partner[%d][%S]", __FUNCTION__, userId, state, partnerId, wPartnerName);
	if(userId > 0)
	{
		if(User *pUser = g_UserDB.GetUserByDBID(userId))
		{
			WeddingUser& wu = pUser->pED->weddingUser;
			wu.partnerId = partnerId;
			wu.state = state;
			wu.partnerName = wPartnerName;
			wu.penaltyTimeout = penaltyTimeout;
			wu.teleportTimeout = teleportTimeout;
			wu.weddingTime = weddingTime;
		}
	}
	unguard;
	return false;
}

bool CDBPacket::ReplyLoadPledgeSkills(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	guard;
	UINT pledgeId = 0;
	INT32 skillId = 0, skillLevel = 0;
	INT32 skillCnt = 0;
	packet = Disassemble(packet, "dd", &pledgeId, &skillCnt);
	if(pledgeId > 0 && skillCnt > 0)
	{
		if(CPledge *pPledge = g_PledgeDB.GetPledge(pledgeId))
		{
			pPledge->pSD->pPledgeEx->m_Lock.Enter();
			PledgeSkillMap& pledgeSkills = pPledge->pSD->pPledgeEx->m_Skills;
			for(int n=0;n<skillCnt;n++)
			{
				packet = Disassemble(packet, "dd", &skillId, &skillLevel);
				if(skillId > 0 && skillLevel > 0)
				{
					const PledgeSkillInfo& psi = g_PledgeSkillDb.GetSkillInfo(skillId, skillLevel);
					if(psi.skillId > 0)
					{
						pledgeSkills[skillId] = psi;
					}

				}
			}
			pPledge->pSD->pPledgeEx->m_Lock.Leave();
			UserVector vUsers;
			pPledge->GetAllMembers(vUsers);
			for(UINT n=0;n<vUsers.size();n++)
			{
				if(User *pMember = vUsers[n]->SafeCastUser())
				{
					pPledge->AcquirePledgeSkills(pMember);
				}
			}
		}
	}
	unguard;
	return false;
}

bool CDBPacket::ReplyAuctionPaymentAsk(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	UINT charId = 0, jobId = 0;
	INT32 itemId = 0, itemAmount = 0, priceId = 0, priceAmount = 0;
	WCHAR buyer[25];

	Disassemble(packet, "ddddddS", &jobId, &charId, &priceId, &priceAmount, &itemId, &itemAmount, sizeof(buyer), buyer);
	if(charId > 0 && itemId > 0)
	{
		User *pUser = g_UserDB.GetUserByDBID(charId);
		if(pUser->ValidUser())
		{
			g_Auction.HandlePaymentRequest(pUser, jobId, priceId, priceAmount, itemId, itemAmount, buyer);
		}
	}
	return false;
}

bool CDBPacket::ReplyLoadAuctionItem(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	UINT sellerId = 0, auctionId = 0, augmentation = 0, expireTime = 0;
	INT32 itemId = 0, amount = 0, enchant = 0, price = 0;
	WCHAR sellerName[25];
	Disassemble(packet, "dSddddddd", &sellerId, sizeof(sellerName), sellerName, &auctionId, &itemId, &amount, &enchant, &augmentation, &price, &expireTime);

	g_Auction.Create(sellerId, sellerName, auctionId, itemId, amount, enchant, augmentation, price, expireTime, true);

	return false;
}

bool CDBPacket::ReplyCreateAuction(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	UINT sellerId = 0, auctionId = 0, augmentation = 0, expireTime = 0;
	INT32 itemId = 0, amount = 0, enchant = 0, price = 0;
	WCHAR sellerName[25];
	Disassemble(packet, "dSddddddd", &sellerId, sizeof(sellerName), sellerName, &auctionId, &itemId, &amount, &enchant, &augmentation, &price, &expireTime);

	g_Auction.Create(sellerId, sellerName, auctionId, itemId, amount, enchant, augmentation, price, expireTime, false);

	return false;
}

bool CDBPacket::ReplyLoadFractionData(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	UINT fractionId = 0;
	INT32 points = 0;
	Disassemble(packet, "dd", &fractionId, &points);
	if(fractionId)
	{
	}

	return false;
}

bool CDBPacket::ReplyItemDeliveryAsk(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	UINT charId = 0, jobId = 0;
	INT32 itemId = 0, itemAmount = 0, enchant = 0;

	Disassemble(packet, "ddddd", &charId, &jobId, &itemId, &itemAmount, &enchant);
	if(charId > 0 && itemId > 0)
	{
		User *pUser = g_UserDB.GetUserByDBID(charId);
		if(pUser->ValidUser())
		{
			g_ItemDelivery.HandleRequest(pUser, jobId, itemId, itemAmount, enchant);
		}
	}
	return false;
}

bool CDBPacket::ReplyLoadUserFraction(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	UINT charId = 0, fractionId = 0, fractionTimestamp = 0;
	INT32 fractionPoints = 0;

	if(charId > 0)
	{
		if(User *pUser = g_UserDB.GetUserByDBID(charId))
		{
			if(pUser->ValidUser())
			{
				FractionUser& fu = pUser->pED->fractionUser;
				fu.lock.Enter();
				fu.fractionId = fractionId;
				fu.leaveTimestamp = fractionTimestamp;
				fu.points = fractionPoints;
				fu.lock.Leave();
				pUser->SetCharInfoChanged();
				pUser->SetUserInfoChanged();
			}
		}
	}
	return false;
}

bool CDBPacket::ReplyLoadAdditionalBox(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	UINT accountId = 0, timestamp = 0;
	Disassemble(packet, "dd", &accountId, &timestamp);

	if(accountId > 0)
	{
		g_AccountDB.DBSetAdditionalBox(accountId, timestamp);
	}

	return false;
}

bool CDBPacket::ReplyLoadHwidLockData(CDBSocket *pSocket, const unsigned char *packet, UINT packetId)
{
	UINT accountId = 0;
	UINT charId = 0;
	packet = Disassemble(packet, "dd", &accountId, &charId);
	HWID hwid(packet);
	if(hwid.IsNull() == false)
	{
		if(accountId)
		{
			g_AccountDB.DBLockAccount(accountId, hwid.hash);
		}
		if(charId)
		{
			g_AccountDB.DBLockCharacter(charId, hwid.hash);
		}
	}else
	{
		if(accountId)
		{
			g_AccountDB.DBUnlockAccount(accountId);
		}
		if(charId)
		{
			g_AccountDB.DBUnlockCharacter(charId);
		}
	}

	return false;
}

bool CDBPacket::ReplyLoadBannedAccounts(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	UINT count = 0;
	packet = Disassemble(packet, "d", &count);
	for(UINT n=0;n<count;n++)
	{
		UINT accountId = 0, timestamp = 0;
		packet = Disassemble(packet, "dd", &accountId, &timestamp);
		g_AccountDB.SetBan(accountId, timestamp);
	}
	g_Log.Add(CLog::Blue, "Loaded [%d] banned account(s).", count);
	return false;
}

bool CDBPacket::ReplyLoadVotePoints(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	int charId = 0, points = 0;
	DWORD voteTimestamp = 0;
	Disassemble(packet, "ddd", &charId, &points, &voteTimestamp);

	if(charId)
	{
		
		User *pUser = g_UserDB.GetUserByDBID(charId);
		if(pUser->ValidUser())
		{
			pUser->pED->voteRewardPoints = points;
		//	pUser->pSD->voteTimestamp = voteTimestamp;
			g_VoteReward.OnLoad(pUser);
		}
	}
	return false;
}

bool CDBPacket::ReplyLoadVIPAccounts(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	guard;

	UINT accountId = 0, timestamp = 0;
	int vipLevel = 0;
	Disassemble(packet, "ddd", &accountId, &vipLevel, &timestamp);

	if(accountId > 0)
	{
		g_AccountDB.SetVIPLevel(accountId, vipLevel, timestamp);
	}
	unguard;
	return false;
}

bool CDBPacket::ReplyLoadVipStatus(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	int vipLevel = 0, charId = 0;
	DWORD vipTimestamp = 0;
	Disassemble(packet, "ddd", &charId, &vipLevel, &vipTimestamp);
	if(charId)
	{
		User *pUser = g_UserDB.GetUserByDBID(charId);
		if(pUser->ValidUser())
		{
			pUser->pSD->vipLevel = vipLevel;
			pUser->pSD->vipTimestamp = vipTimestamp;
			g_VIPSystem.OnLoad(pUser);
			g_AccountDB.SetVIPLevel(pUser->pSocket->accountId, vipLevel, vipTimestamp);
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Invalid user[%d].", __FUNCTION__, charId);
		}
	}

	return false;
}

bool CDBPacket::ReplyLoadNpcDeath(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	DWORD npc_count = 0;
	packet = Disassemble(packet, "d", &npc_count);
	if(npc_count > 0)
	{
		for(DWORD n=0;n<npc_count;n++)
		{
			int npcClassId;
			DWORD deathTime;
			packet = Disassemble(packet, "dd", &npcClassId, &deathTime);
			g_NpcMaker.LoadedFromDB(npcClassId, deathTime);
		}
	};
	g_NpcMaker.SetLoadedFromDB(true);
	g_Log.Add(CLog::Blue, "[NpcMaker] Loaded %d npcs from db.", npc_count);
	return false;
}

bool CDBPacket::ReplyRestoreCH(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	int pledgeId, agitId;
	Disassemble(packet, "dd", &pledgeId, &agitId);
	if(pledgeId > 0 && agitId > 0)
	{
		CPledge *pPledge = g_PledgeDB.GetPledge(pledgeId);
		if(pPledge && pPledge->pSD)
		{
			CResidence* pAgit = g_ResidenceDB.GetResidence(agitId);
			if(pAgit->IsAgit())
			{
				pPledge->pAgit = (CAgit*)pAgit;
				pPledge->pSD->nAgitID = agitId;
				pPledge->Save(7);
				pPledge->UpdateInfo();
				g_Log.Add(CLog::Blue, "[%s] Restored old CH[%d] for pledge[%d]", __FUNCTION__, agitId, pledgeId);
			}else
			{
				g_Log.Add(CLog::Error,"[%s] Invalid agit[%d] pledge[%d]", __FUNCTION__, agitId, pledgeId);
			}
		}
	}
	return false;
}

bool CDBPacket::ReplyLoadAIO(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	UINT charId = 0, id = 0, timestamp = 0;
	
	Disassemble(packet, "ddd", &charId, &id, &timestamp);
	if(charId > 0)
	{
		User *pUser = g_UserDB.GetUserByDBID(charId);
		if(pUser->ValidUser())
		{
			g_AIOSystem.OnLoad(pUser, id, timestamp);
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Cannot find user[%d]", __FUNCTION__, charId);
		}

	}

	return false;
}

bool CDBPacket::ReplyCustomEnchantItem(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	int nUserID = 0, nItemID = 0, nEnchant = 0, scrollId = 0;
	Disassemble(packet, "dddd", &nUserID, &nItemID, &nEnchant, &scrollId);
	if(nUserID > 0 && nItemID > 0 && nEnchant >= 0)
	{
		User *pUser = g_UserDB.GetUserByDBID(nUserID);
		if(pUser->ValidUser())
		{
			CItem *pItem = pUser->inventory.GetItemByDBID(nItemID);
			if(pItem->IsValidItem())
			{
				pItem->Lock();
				pItem->pSID->nEnchantLevel = nEnchant;
				pItem->Unlock();
				pUser->inventory.SetInventoryChanged(pItem, CInventory::UPDATE);
				pUser->ValidateParameters();
				if(scrollId)
				{
					pUser->DeleteItemInInventory(scrollId, 1);
					pUser->pSocket->Send("cd", 0x81, 0);
					pUser->isEnchantingItem = 0;
					typedef void (*f)(UINT64, UINT);
					UINT64 addr = (UINT64)pUser + 0x2930;
					f(0x7F0210)(addr, 0);
					pUser->SendItemList(true);
				}
			//	pUser->SendItemList();
			}else
				g_Log.Add(CLog::Error, "[%s] Invalid Item[%d] - User[%d] Enchant[%d]", __FUNCTION__, nItemID, nUserID, nEnchant);
		}else
			g_Log.Add(CLog::Error, "[%s] Invalid User[%d] - ItemID[%d] Enchant[%d]", __FUNCTION__, nUserID, nItemID, nEnchant);
	}else
		g_Log.Add(CLog::Error, "[%s] Invalid data: UserID[%d] ItemID[%d] Enchant[%d]", __FUNCTION__, nUserID, nItemID, nEnchant);
	return false;
}

bool CDBPacket::ReplyLoadSpiritCount(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	int nSpiritCount = 0, nCharID = 0;
	Disassemble(packet, "dd", &nCharID, &nSpiritCount);
//	g_Log.Add(CLog::Blue, "[%s] Loading spirits [%d] for char[%d]", __FUNCTION__, nSpiritCount, nCharID);
	if(nCharID > 0 && nSpiritCount > 0)
	{
		User *pUser = g_UserDB.GetUserByDBID(nCharID);
		if(pUser->ValidUser())
		{
	//		g_Log.Add(CLog::Blue, "[%s] Loading spirits start timer", __FUNCTION__);
			pUser->pSD->nSpiritCounter = nSpiritCount;
			pUser->pSD->pExData->TimerHandler.AddTimer(CCreatureTimerHandler::USER_SEND_SPIRIT_STATUS, 5);
		}else
			g_Log.Add(CLog::Error, "[%s] Invalid User", __FUNCTION__);
	}
	return false;
}

bool CDBPacket::ReplyLoadBuffList(CDBSocket *pSocket, const unsigned char* packet, UINT packetId)
{
	guard;

	UINT charId = 0;
	packet = Disassemble(packet, "d", &charId);

	UINT buffList[4][48];
	memset(buffList, 0, sizeof(buffList));

	for(UINT m=0;m<4;m++)
	{
		UINT amount = 0;
		packet = Disassemble(packet, "d", &amount);
		for(UINT n=0;n<amount;n++)
		{
			if(n < 48)
			{
				packet = Disassemble(packet, "d", &buffList[m][n]);
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Buff amount overflow[%d] limit 48 - char_id[%d]!", __FUNCTION__, amount, charId);
				break;
			}
		}
	}

	if(User *pUser = g_UserDB.GetUserByDBID(charId))
	{
		SchemeBufferUser& sbu = pUser->pED->schemeBufferUser;
		sbu.lock.Enter();
		sbu.buffList.clear();
		for(UINT n=0;n<48;n++)
		{
			if(buffList[0][n] > 0)
			{
				SkillPchId skill(buffList[0][n]);
				sbu.buffList.push_back(skill);
			}
		}
		sbu.remoteBuffList.clear();
		for(UINT n=0;n<48;n++)
		{
			if(buffList[1][n] > 0)
			{
				SkillPchId skill(buffList[1][n]);
				sbu.remoteBuffList.push_back(skill);
			}
		}
		sbu.summonBuffList.clear();
		for(UINT n=0;n<48;n++)
		{
			if(buffList[2][n] > 0)
			{
				SkillPchId skill(buffList[2][n]);
				sbu.summonBuffList.push_back(skill);
			}
		}
		sbu.remoteSummonBuffList.clear();
		for(UINT n=0;n<48;n++)
		{
			if(buffList[3][n] > 0)
			{
				SkillPchId skill(buffList[3][n]);
				sbu.remoteSummonBuffList.push_back(skill);
			}
		}
		sbu.lock.Leave();
		g_SchemeBuffer.ValidateBuffList(pUser, false, false);
		g_SchemeBuffer.ValidateBuffList(pUser, false, true);
		g_SchemeBuffer.ValidateBuffList(pUser, true, false);
		g_SchemeBuffer.ValidateBuffList(pUser, true, true);
	}

	unguard;
	return false;
}