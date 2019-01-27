#pragma once
#include "CCreature.h"
#include "MacAddress.h"
#include "ItemBonus.h"
#include "OlympiadUser.h"
#include "Friends.h"

#define VALIDATE_AUTO_HP_TIMER 10101
#define VALIDATE_AUTO_MP_TIMER 10102
#define VALIDATE_AUTO_CP_TIMER 10103
#define VALIDATE_AUTO_CP_ADV_TIMER 10104

struct BlockItemInfo
{
	UINT databaseId;
	INT32 itemId;
	INT32 enchant;
};

namespace TvT
{
	enum UserStatus
	{
		UserNone = 0,
		UserRegistered = 1,
		UserPreparing = 2,
		UserFighting = 3,
		UserFinishing = 4
	};
	enum TeamType
	{
		TeamNone = 0,
		TeamBlue = 1,
		TeamRed = 2
	};

	struct TvTUser
	{
		UserStatus status;
		TeamType team;
		IVector orgPos;
		IVector respawnPos;
		INT32 resetTimer;
		INT32 respawnTick;
		BOOL friendlyFire;
		TvTUser()
		{
			Clear();
		}
		inline void Clear()
		{
			status = UserNone;
			team = TeamNone;
			respawnTick = 0;
			resetTimer = 0;
			friendlyFire = 0;
			orgPos.x = 0;
			orgPos.y = 0;
			orgPos.z = 0;
		}
	};
};

struct FractionUser
{
	CSLock lock;
	INT32 fractionId;
	UINT points;
	time_t leaveTimestamp;
	map<UINT, UINT> killedPlayerTimestamps;
	FractionUser()
	{
		Clear();
	}
	inline void Clear()
	{
		fractionId = 0;
		points = 0;
		leaveTimestamp = 0;
	}
};

struct QueuedBuff
{
	CSkillInfo *pSI;
	INT32 abnormalTime;
	bool summon;
	QueuedBuff() : pSI(0), summon(false), abnormalTime(0) {}
};

struct SchemeBufferUser
{
	CSLock lock;
	vector<QueuedBuff> buffQueue;
	BOOL timerInUse;
	UINT useTick;
	vector<SkillPchId> buffList;
	vector<SkillPchId> summonBuffList;
	vector<SkillPchId> remoteBuffList;
	vector<SkillPchId> remoteSummonBuffList;
	bool saveList;
	bool inBlockZone;

	SchemeBufferUser(): useTick(0), timerInUse(0), saveList(false), inBlockZone(false) {};
	inline vector<SkillPchId>& GetList(bool summon, bool remote)
	{
		if(summon)
		{
			if(remote)
			{
				return remoteSummonBuffList;
			}else
			{
				return summonBuffList;
			}
		}else
		{
			if(remote)
			{
				return remoteBuffList;
			}else
			{
				return buffList;
			}
		}
	}
};

struct SentHtml
{
	UINT classId;
	WCHAR wHtml[16382];
	WCHAR wName[260];
};


struct SkillDmgMultipler
{
	int skillId;
	int level;
	double multipler;
	SkillDmgMultipler() : skillId(0), level(0), multipler(1.0) {};
};

struct AutoCastSkill
{
	enum Type
	{
		OnDamaged = 1,
		OnPhysicalAttack = 2,
		OnMagicalAttack = 3,
		OnCriticalAttack = 4
	};
	UINT type;
	int skillId;
	int level;
	double chance;
	AutoCastSkill() : skillId(0), level(0), chance(0.0) {};
};

struct AuctionUser
{
	UINT createTimeout;
	UINT itemServerId;
	UINT itemClassId;
	BOOL pendingAuction;
	UINT paymentAskTimestamp;
};

struct VisualArmorUser
{
	UINT floodTimestamp;
	UINT armorId;
	UINT armorType;
	int chestSlotId;
	int legsSlotId;
	int glovesSlotId;
	int feetSlotId;
	int minLevel;
	int maxLevel;
	VisualArmorUser() { Clear(); };
	void Clear()
	{
		memset(this, 0, sizeof(VisualArmorUser));
	}
};

struct WeddingUser
{
	enum State
	{
		WeddingNone = 0,
		WeddingAskEngage = 1,
		WeddingEngaged = 2,
		WeddingAskMarry = 3,
		WeddingMarried = 4
	};
	UINT state;
	UINT partnerId;
	wstring partnerName;
	UINT penaltyTimeout;
	UINT teleportTimeout;
	UINT bigHeadTimeout;
	UINT weddingTime;
	WeddingUser() : state(WeddingNone), partnerId(0), penaltyTimeout(0), teleportTimeout(0), bigHeadTimeout(0), weddingTime(0) {};
	void clear() { state = WeddingNone; partnerId = 0; partnerName.clear(); };
};

struct AutoConsumeInfo;

struct ItemAutoConsumeData
{
	INT32 itemId;
	UINT lastUseTick;
	AutoConsumeInfo *pInfo;
	ItemAutoConsumeData() : itemId(0), lastUseTick(0), pInfo(0) {};
};

struct SubStackInfo
{
	UINT classId[4][2];	//subjobId, baseClass/stackClass
	UINT timestamp;
	SubStackInfo() : timestamp(GetTickCount())
	{
		for(UINT n=0;n<4;n++)
		{
			classId[n][0] = classId[n][1] = UINT_MAX;
		}
	}
};

struct InstanceData
{
	UINT instanceId;
	UINT timeSpent;
	UINT incTimeTick;
	UINT reuseTimeout;
	bool saved;
	InstanceData() : instanceId(0), timeSpent(0), incTimeTick(0), reuseTimeout(0), saved(false) {};
};

struct HiddenAbnormalData : AbnormalData
{
	time_t hideExpireTimestamp;
	bool restored;
	HiddenAbnormalData() { hideExpireTimestamp = 0; restored = false; };
	HiddenAbnormalData(const AbnormalData& other)
	{
		restored = false;
		hideExpireTimestamp = 0;
		pSI = other.pSI;
		expireTime = other.expireTime;
		selfBuff = other.selfBuff;
		casterSID = other.casterSID;
		partyBuff = other.partyBuff;
	};
};

struct L2NetworkUser
{
	WCHAR identity[33];
	UINT lastVoteTime;
	UINT lastRewardTime;
	bool loaded;
	UINT checkTimestamp;
	L2NetworkUser() : lastVoteTime(0), lastRewardTime(0), loaded(false), checkTimestamp(0) { memset(identity, 0, sizeof(identity)); };
};

struct RebirthUser
{
	INT32 rebirthCount;
	INT32 availablePoints;
	INT32 bonusInt;
	INT32 bonusStr;
	INT32 bonusCon;
	INT32 bonusMen;
	INT32 bonusDex;
	INT32 bonusWit;
	RebirthUser() : rebirthCount(0), availablePoints(0), bonusInt(0), bonusStr(0), bonusCon(0), bonusMen(0), bonusDex(0), bonusWit(0) {};
};

struct PvPAnnounceUser
{
	INT32 killCount;
	time_t lastKillTime;
	PvPAnnounceUser() : killCount(0), lastKillTime(0) {}
};

struct UserExtData
{
	CSLock lock;
	bool autoLoot;
	volatile LONG userInfoChanged;
	volatile LONG charInfoChanged;
	volatile LONG itemListChanged;
	volatile LONG abnormalStatusChanged;
	volatile LONG validateParametersRequest;
	UINT spawnProtection;
	UINT spawnProtectionTimeout;
	UINT spawnProtectionTimeSent;
	INT32 charmOfCourageLevel;
	CItemBonus itemBonus;
	INT32 cursedWeaponId;
	INT32 cursedWeaponLevel;
	TvT::TvTUser tvtUser;
	UINT miningAreaId;
	FractionUser fractionUser;
	map<INT32, INT32> itemSkills;
	DWORD nicknameColor;
	DWORD titleColor;
	INT32 antiPkLevelRange;
	UINT itemDeliveryTimestamp;
	UINT invalidTick;
	UINT aioId;
	UINT aioTimestamp;
	UINT aioNicknameColor;
	UINT autoInfinitySoulshot[6];
	UINT autoInfinitySpiritshot[6];
	AuctionUser auctionUser;
	SchemeBufferUser schemeBufferUser;
	VisualArmorUser visualArmorUser;
	CSLock sentHtmlLock;
	vector<SentHtml> sentHtml;
	vector<SkillDmgMultipler> skillDmgMultiplers;
	vector<AutoCastSkill> autoCastSkills;
	INT32 bonusInt;
	INT32 bonusStr;
	INT32 bonusCon;
	INT32 bonusMen;
	INT32 bonusDex;
	INT32 bonusWit;
	UINT multiSellGroupId;
	INT32 voteRewardPoints;
	UINT voteRewardTick;
	BOOL voteRewardMsgRequest;
	WeddingUser weddingUser;
	BOOL leftWorld;
	UINT nameColorEx;
	UINT titleColorEx;
	UINT vipChatTimestamp;
	UINT broadcastCombatModeTimestamp;
	BYTE lockHash[32];
	BYTE tempLockHash[32];
	bool isLocked;
	UINT lockAttempts;
	INT32 dailyPvPAmount;	//amount of daily pvp
	UINT dailyPvPTimestamp;	//this will tell us when player get last pvp if it wasn't in this day we'll reset pvp amount
	
	ItemAutoConsumeData autoConsumeHp;
	ItemAutoConsumeData autoConsumeMp;
	ItemAutoConsumeData autoConsumeCp;
	ItemAutoConsumeData autoConsumeCpAdv;

	SubStackInfo subStackInfo;
	vector<HiddenAbnormalData> hiddenAbnormals;
	bool requestSaveAllSkills;
	UINT currentInstanceId;
	vector<InstanceData> instances;
	UINT skillMasterFlag;
	
	DWORD chatTimestamp[18];

	L2NetworkUser l2networkUser;
	RebirthUser rebirthUser;
	bool preserveAbnormalTerritory;
	UINT raceChangeZoneId;
	UINT fakeRace;
	UINT fakeSex;
	UINT blockChatZoneId;
	bool gradePenaltySet;
	PvPAnnounceUser pvpAnnUser;

	UserExtData() : gradePenaltySet(false), skillMasterFlag(0), preserveAbnormalTerritory(false), fakeRace(UINT_MAX), fakeSex(UINT_MAX), raceChangeZoneId(0), blockChatZoneId(0) {};
};

class CPrivateStoreItemData
{
public:
	CPrivateStoreItemData() { memset(this, 0, sizeof(CPrivateStoreItemData)); }
/*00*/	DWORD ObjectID; //in sell
/*04*/	DWORD n2;
/*08*/	WORD Enchant;
/*0A*/	WORD w4;
/*0C*/	DWORD n5;
/*10*/	int ItemIndex;	//in sell
/*14*/	int Count;
/*18*/	int ClassID;
/*1C*/	int Price;
};

class CRecipeInfo
{
public:
	CRecipeInfo() : RecipeID(0), Level(0), MpConsume(0), SuccessRate(0), ItemID(0) { };
/*00*/	int RecipeID;
/*04*/	int Level;
/*08*/	int MpConsume;
/*0C*/	int SuccessRate;
/*10*/	int ItemID;
};

class CRecipeInfoData
{
public:
	CRecipeInfoData() : pInfo(NULL), Price(0) {  };
/*00*/ CRecipeInfo *pInfo;
/*08*/ int Price;
};

typedef map<int, CRecipeInfo*> RecipeInfoMap;
typedef map<int, CRecipeInfoData*> PrivateStoreRecipeMap;
typedef map<int, CPrivateStoreItemData*> PrivateStoreItemMap;

class User: public CCreature
{
public:
	/* 1500 */ LPVOID _unkn1500[32];
	/* 1600 */ LPVOID _unkn1600[32];	
	/* 1700 */ LPVOID _unkn1700[32];
	/* 1800 */ LPVOID _unkn1800[32];
	/* 1900 */ LPVOID _unkn1900[32];
	/* 1A00 */ LPVOID _unkn1A00[8];
	/* 1A40 */ bool soulShotInUse;
	/* 1A41 */ bool spiritShotInUse;
	/* 1A42 */ bool summonSoulShotInUse;
	/* 1A43 */ bool summonSpiritShotInUse;
	/* 1A44 */ INT32 _unkn1A44;
	/* 1A48 */ LPVOID _unkn1A48[3];

	/* 1A60 */ bool _unkn1A60;
	/* 1A61 */ bool _unkn1A61;
	/* 1A62 */ bool equippedFormalWear;
	/* 1A63 */ bool _unkn1a63;
	/* 1A64 */ UINT _unkn1a64;
	/* 1A68 */ LPVOID _unkn1A68[7];

	/* 1AA0 */ UINT _unkn1AA0;
	/* 1AA4 */ UINT soulshotUseTick;
	/* 1AA8 */ UINT autoSoulshotFlag;
	/* 1AAC */ bool isEnchantingItem;
	/* 1AAD */ bool _unkn1AAD[3];
	/* 1AB0 */ LPVOID _unkn1AB0[10];

	/* 1B00 */ LPVOID _unkn1B00[32];
	/* 1D00 */ LPVOID _unkn1D00[32];
	/* 1C00 */ LPVOID _unkn1C00[32];
	/* 1E00 */ LPVOID _unkn1E00[32];
	/* 1F00 */ LPVOID _unkn1F00[32];
	/* 2000 */ LPVOID _unkn2000[32];
	/* 2100 */ LPVOID _unkn2100[32];
	/* 2200 */ LPVOID _unkn2200[32];
	/* 2300 */ LPVOID _unkn2300[32];
	/* 2400 */ LPVOID _unkn2400[32];
	/* 2500 */ LPVOID _unkn2500[32];
	/* 2600 */ LPVOID _unkn2600[14];
	/* 2670 */ CUserSocket *pSocket;
	/* 2678 */ UINT tradeId;
	/* 267C */ UINT _unkn267C;
	/* 2680 */ LPVOID _unkn2680[16];
	/* 2700 */ LPVOID _unkn2700[32];
	
	/* 2800 */ LPVOID _unkn2800[12];

	/* 2860 */ UINT _unkn2860;
	/* 2864 */ UINT pledgeServerId;
	/* 2868 */ LPVOID _unkn2868;
	/* 2870 */ CFriends *pFriends;
	/* 2878 */ WCHAR wTitle[25];

	/* 28AA */ USHORT _align28AA;
	/* 28AC */ UINT _align28AC;
	/* 28B0 */ LPVOID _unkn28B0[3];

	/* 28C8 */ BYTE _unkn28C8;
	/* 28C9 */ BYTE _unkn28C9;
	/* 28CA */ bool blockAllChat;
	/* 28CB */ BYTE _aling28CB[5];

	/* 28D0 */ LPVOID _unkn28D0[6];

	/* 2900 */ LPVOID _unkn2900[1];
	/* 2908 */ UINT multisellNpcId;
	/* 290C */ UINT _align200C;
	/* 2910 */ LPVOID _unkn2190[6];
	
	/* 2940 */ UINT _unkn2940;
	/* 2944 */ UINT stopSayTick;
	/* 2948 */ LPVOID _unkn2948[2];

	/* 2958 */ UINT killDamageType;	//set to 5 in killme, set as damage type 
	/* 295C */ UINT _unkn295C;

	/* 2960 */ LPVOID _unkn2960[20];

	/* 2A00 */ LPVOID _unkn2A00[32];
	/* 2B00 */ LPVOID _unkn2B00[14];
	/* 2B70 */ long relationChangedRequest;
	/* 2B74 */ UINT _unkn2B74;
	/* 2B78 */ LPVOID _unkn2B78[16];

	/* 2BF8 */ UINT summonId;
	/* 2BFC */ int _unkn2BF4;
	/* 2C00 */ LPVOID _unkn2C00[21];
	/* 2CA8 */ UINT _unkn2CA8;
	/* 2CAC */ BYTE teamCircleId;
	/* 2CAD */ bool _align2CAD[3];
	/* 2CB0 */ COlympiadUser olympiadUser;
	
	/* 2D10 */ LPVOID _unkn2D00[9];
	/* 2D58 */ const WCHAR* wLastHtmlFileName;
	/* 2D60 */ LPVOID _unkn2D60;
	/* 2D68 */ LPVOID _unkn2D68;
	/* 2D70 */ UINT language;
	/* 2D74 */ UINT _unkn2D74;

	//Extended variables
	UserExtData* pED;

	enum RELATION_TYPE
	{
		INSIDE_PVP_ZONE = 1,
		PVP_FLAG = 2,
		KARMA = 4,
		IN_MY_PARTY = 0x20
	};
	static void ExtInit();
	static void OnCreate(User *pUser);
	static User* OnDestruct(User *pUser, DWORD param);
	static void OnDelete(LPVOID lpInstance, UINT userId, const char* file, int line);
	CPledge *GetPledge();
	CUserSocket *GetSocket();
	bool IsNowTrade();
	CParty * User::GetParty();
	void DespawnSummon();
	void DeleteItemInInventoryBeforeCommit(int ItemID, int Count)
	{
		typedef void (*_f)(User*, int, int);
		_f f = (_f) 0x00827150;
		f(this, ItemID, Count);
	}
	CWareHouseDeposit *GetWareHouseDeposit();
	CWareHouseWithdraw *GetWareHouseWithdraw();
	void ActionFailed();
	bool PacketAcceptable(int nPacketNo);
	void SendSystemMessage(PWCHAR Talker, PWCHAR Message);
	void SendSystemMessage(const wchar_t* Talker, const wchar_t* Message);
	void SendSystemMessage(PWCHAR wszMessage);
	void SendSystemMessage(const WCHAR* wszMessage);
	void SendSystemMessage(int msgId);
	void SendSystemMessage(CSystemMessage *pMsg);
	void SendUserInfo(CUserSocket* bGM = NULL);
	void SendCharInfo(CUserSocket* bGM = NULL);
	void SendHpInfo();
	void SendMpInfo();
	void SendQuestList();
	bool SendCoolTime();
	void SendItemList(bool show = true);
	void SendRelationChanged(CUserSocket *pSocket = 0);
	void SendAcquireSkillToDB(int nSkillID, int nLevel, bool bMayDown = false);
	void SendDeleteSkillToDB(int nSkillID);
	void SendETCBuffStatus();
	void SendAbnormalStatusInfo();
	void SendSpelledInfoChanged();
	void SaveInfectedSkills();
	void SendSkillList(bool bNothing, CSocket *pGMSocket);
	bool TeleportToLocation(int x, int y, int z, bool bNotFadeScreen);
	void SetKarma(int nKarma);
	void TradeCancel();
	void ChangeTarget(CCreature *pObject, bool ByUser);
	CSummon *GetSummonOrPet();
	CSummon *GetSummon();
	bool InCombat();
	DWORD CombatTime();
	bool CanEquip(CItem *pItem);
	bool CanGetPartyBonus(CCreature *pCreature);
	bool InCombatZone();
	void IncreaseDuelCount();
	bool UseItem(CItem *pItem, int nForce);
	bool IsItemUsable();
	bool DropItem(CItem *pItem, int nAmount);
	bool UnEquipItem(int nSlotType);
	int ExpDown(int nExpDec);
	void SetExpDown(int nExpDec);
	void IncreaseKarma(bool bWithPk);
	CSharedNoblessInfo *GetNoblessInfo();
	void ResetJoinPledgePenalty();
	bool ChangeClass(int nClass);
	bool CanJoinPledge();
	void Ride(CCreature *pCreature);
	void RideWyvern(int wyvernClassId, int level);
	UINT GetPledgeClass();
	int GetAugmentationID();
	int GetAugmentationID(int nItemIndex);
	void DespawnPet(bool byLeaveWorld);
	void PlaySound(PWCHAR wszName, int nCount, int nObjectID, int x, int y, int z);
	void OnDieDropItem(bool bDieOnBattleField);
	PWCHAR GetTitle();
	static User *GetUserBySID(UINT *pSID);
	int GetEquipedItemSlot(int nItemIndex);
	void QuitPrivateStore();
	void QuitPrivateStoreBuy();
	void QuitRecipeStore();
	bool IsAcademyMember();
	void GetOff(bool bForce);
	void ShowHTML(const WCHAR* filename, const WCHAR* buffer, int classID);
	void SetSpelledInfoChanged();
	void SetAbnormalStatusChanged();
	void SetUserInfoChanged();
	void SetCharInfoChanged();
	void SetItemListChanged();
	void ShowBoard(const WCHAR* wHtml);
	void HideBoard();
	void SetLevelByExp(INT32 nExp);
	bool DoExtraValidation();
	bool HaveMemo(int nQuestID);
	int GetMemoState(int nQuestID);
	void SetMemoState(int nQuestID, int nState);
	void SetLevel(int nLevel);
	PrivateStoreItemMap* GetPrivateStoreSell();
	PrivateStoreItemMap* GetPrivateStoreBuy();
	PrivateStoreRecipeMap* GetPrivateStoreRecipe();
	RecipeInfoMap* GetRecipeInfo();
	CRecipeInfo* GetRecipeInfo(int recipeId);
	CPet *GetPet();
	void DeleteItem(CItem *pItem);
	bool IsInCombatMode();
	void ValidateAcquiredSkills(bool log = true);
	void SaveAcquiredSkills();
	int GetTradeSID();
	int GetAccountID();
	void SetCombatMode();
	void SetGuilty(bool c);
	void SetPvPFlag();
	void Kick();
	void SetStopSay(DWORD timeInMs, bool saveInDB = 1);
	void KillMyself();
	void ValidateLifeTimeItems();
	void SendLifeTimeItemsInfo();
	void SendRuneInfo();
	void ValidateItemExSkill();
	bool HaveItemInPrivateStore(int ItemID);
	bool Revive();
	int GetPledgeType();
	int GetPledgeReputation();
	void ValidateTerritory();
	void RequestUpdateSpelledInfo();
	bool ValidateStackableItems();
	bool GetDropBonuses(double& itemDrop, double& adena, double& raidBoss, double& raidBossOnlyChance, double& epicBoss, double &itemOnlyChance);
	double GetAdenaBonus();
	double GetSpoilBonus();
	void ShowBlockableItems(User *pGM = 0);
	LPBYTE GetHWID();
	INT32 GetMPCCPartyAmount();
	INT32 GetMPCCMemberDistance();
	bool QueueBuff(CSkillInfo *pSI, bool summon, int abnormalTime);
	bool QueueBuffList(bool summon, bool remote);
	void ActivateQueuedBuff(QueuedBuff& qb);
	void DispatchQueuedBuff();
	void ClearQueuedBuffs();
	double GetSkillDmgMultipler(int skillId, int level);
	bool TiggerAutoCastSkill(AutoCastSkill::Type type, CCreature *pOther);
	inline void SetRelationChangeRequest(int state)
	{
		InterlockedCompareExchange(&relationChangedRequest, state, 0);
	}
	void StopSummon();
	inline void SetValidateParametersRequest()
	{
		InterlockedExchange(&pED->validateParametersRequest, TRUE);
	}
	bool IsFriend(User *pUser);
	void ShowPCCafePoints(INT32 totalPoints, INT32 newPoints, INT32 timeInHours, bool adding);
	void ValidateSubjobParameters();
	void HideAbnormal(AbnormalData& abnormal, UINT expireTime);
	void TimerExpiredForHiddenAbnormals(bool allExpired = false);
	void ClearHiddenAbnormals();
	void SaveInstanceTime(UINT instanceId, UINT previousInstanceId, UINT allowedTime, UINT reuseTime, bool& kick, bool byHwid = false);
	void SaveInstances();
	void ValidateEquippedItems();
};