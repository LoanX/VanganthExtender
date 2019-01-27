#pragma once

#include "class.h"
#include "..//..//L2Ext//CacheD.h"


typedef bool (__cdecl *QueryCallback)(CQuerySocket *, const unsigned char*);


class PacketHandler
{
public:
	static void Initialize();
	static bool __cdecl Handler(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl DummyCustomPacket(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static void Bind(UINT id, QueryCallback cb);
	static bool __cdecl RequestSetPledgePrivilege(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestSetPledgeMemberRank(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestReorganizeMember(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestSetPledgeReputation(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestSetUserPledgeTypeAndRank(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestCreateSubPledge(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestRenameSubPledge(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestSetSubCaptain(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestAcquirePledgeSkill(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestAddPledge(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestSaveItemDataEx(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestSaveJoinPledgeLevel(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestSetSponsor(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestDelegateClanMaster(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestSavePledgeStat(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestSetPrivateStore(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestLoadSpiritCount(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestSaveBuffList(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestLoadBuffList(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestSaveKillDeathStat(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestEnchantItem(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestSaveAIO(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestLoadAIO(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestDeleteItems(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestRestoreCH(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestLoadNpcDeath(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestSaveNpcDeath(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestLoadVipStatus(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestSaveVipStatus(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestLoadVIPAccounts(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestLoadVotePoints(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestSaveVotePoints(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestBanAccount(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestLoadBannedAccounts(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestSaveMultiSellLog(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestLockAccount(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestUnlockAccount(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestLockCharacter(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestUnlockCharacter(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestLoadHwidLockData(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestSaveCharacterHwid(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestBanHwid(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestLoadBannedHwid(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestSetAdditionalBox(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestLoadAdditionalBoxData(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestSaveUserFraction(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestLoadUserFraction(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestItemDeliveryAsk(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestItemDeliveryDone(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestLoadFractionData(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestSaveFractionData(CQuerySocket *pQuerySocket, const unsigned char* packet);
	static bool __cdecl RequestSaveSubJob(CQuerySocket *pSocket, const unsigned char* packet);
	static bool __cdecl RequestCreateAuction(CQuerySocket *pSocket, const unsigned char* packet);
	static bool __cdecl RequestModdifyAuctionAmount(CQuerySocket *pSocket, const unsigned char* packet);
	static bool __cdecl RequestDeleteAuction(CQuerySocket *pSocket, const unsigned char* packet);
	static bool __cdecl RequestLoadAuctionItems(CQuerySocket *pSocket, const unsigned char* packet);
	static bool __cdecl RequestAuctionPaymentAsk(CQuerySocket *pSocket, const unsigned char* packet);
	static bool __cdecl RequestAuctionPaymentDone(CQuerySocket *pSocket, const unsigned char* packet);
	static bool __cdecl RequestAuctionAddPaymentJob(CQuerySocket *pSocket, const unsigned char* packet);
	static bool __cdecl RequestLoadPledgeSkills(CQuerySocket *pSocket, const unsigned char* packet);
	static bool __cdecl RequestAddPledgeSkill(CQuerySocket *pSocket, const unsigned char* packet);
	static bool __cdecl RequestLoadWeddingStatus(CQuerySocket *pSocket, const unsigned char* packet);
	static bool __cdecl RequestSaveWeddingStatus(CQuerySocket *pSocket, const unsigned char* packet);
	static bool __cdecl RequestTestQuery(CQuerySocket *pSocket, const unsigned char* packet);
	static bool __cdecl RequestLoadRaidBossStatus(CQuerySocket *pSocket, const unsigned char* packet);
	static bool __cdecl RequestLoadPKStatus(CQuerySocket *pSocket, const unsigned char* packet);
	static bool __cdecl RequestLoadPvPStatus(CQuerySocket *pSocket, const unsigned char* packet);
	static bool __cdecl RequestLoadPledgeTotalPvP(CQuerySocket *pSocket, const unsigned char* packet);
	
	static bool __cdecl RequestLoadBannedHardwareId(CQuerySocket *pSocket, const unsigned char* packet);
	static bool __cdecl RequestSaveHardwareId(CQuerySocket *pSocket, const unsigned char* packet);
	static bool __cdecl RequestBanHardwareId(CQuerySocket *pSocket, const unsigned char* packet);
	static bool __cdecl RequestUnBanHardwareId(CQuerySocket *pSocket, const unsigned char* packet);
	static bool __cdecl RequestLoadNameTitleColor(CQuerySocket *pSocket, const unsigned char* packet);
	static bool __cdecl RequestLoadCharacterLock(CQuerySocket *pSocket, const unsigned char* packet);
	static bool __cdecl RequestSaveCharacterLock(CQuerySocket *pSocket, const unsigned char* packet);

	static bool __cdecl RequestSaveDailyPvP(CQuerySocket *pSocket, const unsigned char* packet);
	static bool __cdecl RequestSaveDailyPvPEntry(CQuerySocket *pSocket, const unsigned char* packet);
	static bool __cdecl RequestLoadDailyPvPEntry(CQuerySocket *pSocket, const unsigned char* packet);
	static bool __cdecl RequestLoadDailyPvP(CQuerySocket *pSocket, const unsigned char* packet);
	static bool __cdecl RequestSaveSubStack(CQuerySocket *pSocket, const unsigned char* packet);
	static bool __cdecl RequestLoadSubStack(CQuerySocket *pSocket, const unsigned char* packet);
	static bool __cdecl RequestSaveInstanceTime(CQuerySocket *pSocket, const unsigned char* packet);
	static bool __cdecl RequestLoadInstanceTime(CQuerySocket *pSocket, const unsigned char* packet);
	static bool RequestLoadL2NetworkVote(CQuerySocket *pSocket, const unsigned char* packet);
	static bool RequestSaveL2NetworkVote(CQuerySocket *pSocket, const unsigned char* packet);
	static bool RequestSaveVisualArmor(CQuerySocket *pSocket, const unsigned char* packet);
	static bool RequestLoadVisualArmor(CQuerySocket *pSocket, const unsigned char* packet);
	static bool RequestLoadRebirth(CQuerySocket *pSocket, const unsigned char* packet);
	static bool RequestSaveRebirth(CQuerySocket *pSocket, const unsigned char* packet);
	static bool RequestLoadDBNpcMaker(CQuerySocket *pSocket, const unsigned char* packet);
	static bool RequestSetCharacterPIN(CQuerySocket *pSocket, const unsigned char* packet);
	static bool RequestLoadCharacterPINS(CQuerySocket *pSocket, const unsigned char* packet);

	static const unsigned char* DisassembleSaveItemPacket(const unsigned char* packet, const char *format, LPINT pItemDBID, LPINT pCharID, LPINT pItemType, LPINT pAumount, LPINT pEnchant, LPINT pEroded, LPINT pBless, LPINT pIdent, LPINT pWished, LPINT pWarehouse);
	static int AssembleItemPacket(PCHAR Buff, int Len, const char* format, int ItemDBID, int ItemType, int Amount, int Enchant, int Eroded, int Bless, int Ident, int Wished);
	static void SendReplyToLoadPledge(CPledge *pPledge, CSocket *pSocket);
	static void ExecuteProceduresOnStart();
	static bool __cdecl SaveCharacterPacket(CQuerySocket *pSocket, const unsigned char* packet);
	static int AssembleLoadCharacterPacket(PCHAR buff, int len, const char* format, PWCHAR wsName,	int nCharID, PWCHAR wsAccount, DWORD Val1, DWORD Val2, DWORD Val3, DWORD Val4, DWORD Val5, DWORD Val6, DWORD Val7, DWORD Val8, DWORD Val9, DWORD Val10, double Val11, double Val12, DWORD Val13, DWORD Val14, DWORD Val15, DWORD Val16, DWORD Val17,
								 DWORD Val18 , DWORD Val19 , DWORD Val20 , DWORD Val21 , DWORD Val22 , DWORD Val23 , DWORD Val24 , DWORD Val25 , DWORD Val26 , DWORD Val27 , DWORD Val28 , DWORD Val29 , DWORD Val30 , DWORD Val31 , DWORD Val32 , DWORD Val33 , DWORD Val34 , DWORD Val35 , DWORD Val36 , DWORD Val37 , DWORD Val38 ,
								 DWORD Val39 , DWORD Val40 , DWORD Val41 , DWORD Val42 , DWORD Val43 , DWORD Val44 , DWORD Val45 , DWORD Val46 , DWORD Val47 , DWORD Val48 , DWORD Val49 , DWORD Val50 , DWORD Val51 , DWORD Val52 , DWORD Val53 , DWORD Val54 , DWORD Val55 , DWORD Val56 , DWORD Val57 , DWORD Val58 , DWORD Val59 ,
								 DWORD Val60 , double Val61 , double Val62 , DWORD Val63 , DWORD Val64 , DWORD Val65 , BYTE Val66);
	static void LogOnItemSetAmount(PVOID pInstance, UINT logType, const WCHAR* format);
	static void __thiscall OnItemSetAmount(CItem *pItem, INT32 amount);
};
