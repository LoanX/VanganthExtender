#pragma once


extern "C"
{
	void PrivateStoreBuyCheckAugmentation();
};

class AugmentStat
{
public:
	enum Stat
	{
		PDEF = 0,
		MDEF = 1,
		MAXHP = 2,
		MAXMP = 3,
		MAXCP = 4,
		PATK = 5,
		MATK = 6,
		HPREGEN = 7,
		MPREGEN = 8,
		CPREGEN = 9,
		EVASION = 10,
		ACCURACY = 11,
		CRITICAL = 12
	};
public:
	int nAugmentID;
	double daStat[13];
public:
	AugmentStat(){memset(this, 0, sizeof(AugmentStat));};
};

class AugmentSkill
{
public:
	int nSkillID;
	int nLevel;
	int nAugmentID;
	AugmentSkill(){memset(this, 0, sizeof(AugmentSkill));};
};
typedef map<UINT , AugmentSkill> AugmentSkillMap;
typedef map<UINT , AugmentStat> AugmentStatMap;
typedef map<UINT, wstring> AugmentNameMap;

class CAugmentation
{
private:
	//STAT
	static const int STAT_SIZE = 14560;
	static const int STAT_BLOCKSIZE = 3640; //4
	static const int STAT_SUBBLOCKSIZE = 91; //40
	//BASESTAT
	static const int BASESTAT_STR = 16341;
	static const int BASESTAT_CON = 16342;
	static const int BASESTAT_INT = 16343;
	static const int BASESTAT_MEN = 16344;
	//SKILL
	static const int SKILL_START = 14561;
	static const int SKILL_SIZE = 1780;
	static const int SKILL_BLOCKSIZE = 178;
	AugmentSkillMap mSkills;
	AugmentStatMap mStats;
	AugmentNameMap mName;
	int SkillChance[4];
	int BaseStatChance[4];
public:
	enum LifeStoneGrade
	{
		NOT_LS = -1,
		LS_LOW = 0,
		LS_MID = 1,
		LS_HIGH = 2,
		LS_TOP = 3
	};
	enum Gemstone
	{
		GemstoneD = 2130,
		GemstoneC = 2131,
		GemstoneB = 2132,
		GemstoneA = 2133,
		GemstoneS = 2134
	};
public:
	void Initialize();
	void InitRates();
	void ReadSkillData();
	void ReadStatData();
	void ReadNameData();
	bool CheckItemID(int nItemID);
	bool IsSkill(UINT nEffect);
	bool DeleteActiveSkill(CCreature *pCreature, UINT nAugmentation);
	bool IsBaseStat(UINT nEffect);
	AugmentSkill GetAugmentSkill(UINT nAugmentID);
	AugmentStat GetAugmentStat(UINT nAugmentID);
	static UINT GetFirstEffectID(UINT nAugmentID);
	static UINT GetSecondEffectID(UINT nAugmentID);
	static int GetGemstoneID(int nCrystalType);
	static int GetGemstoneCount(int nCrystalType);
	wstring GetName(UINT nEffect);
	UINT GenerateAugmentation(int nLifeStoneGrade, int nLifeStoneLevel);
	bool ShowAugmentMixWindow(CUserSocket *pSocket);
	bool ShowAugmentRemoveWindow(CUserSocket *pSocket);
	void SetAugmentationSkillMod(CCreature *pCreature, UINT nAugmentationID);
	bool AugumentWeapon(CUserSocket *pSocket, UINT nWeaponSID, UINT nLifeStoneSID, UINT nGemstoneSID, int nGemstoneCount);
	void ValidateAugmentation(CCreature *pCreature);
	bool RemoveAugmentation(CItem *pWeapon, User *pUser);
	bool BlockItemAction(UINT nObjectID);
	void ValidateBaseStat(User *pUser);
	bool RequestConfirmTargetItem(CUserSocket *pSocket, const unsigned char* packet);
	bool RequestConfirmRefinerItem(CUserSocket *pSocket, const unsigned char* packet);
	bool RequestConfirmGemStone(CUserSocket *pSocket, const unsigned char* packet);
	bool RequestAugmentItem(CUserSocket *pSocket, const unsigned char* packet);
	bool RequestConfirmCancelItem(CUserSocket *pSocket, const unsigned char* packet);
	bool RequestCancelItem(CUserSocket *pSocket, const unsigned char* packet);
	static bool MultiSellDeleteItem(CInventory *pInventory, CItem *pItem, int nType);
	void MultiSellAddItem(User *pUser, CItem *pItem);
	static const unsigned char* DisassembleWareHouseItem(const unsigned char* packet, const char* format, LPINT lpItemId, LPINT lpItemType, LPINT lpAmount, LPINT lpEnchant, LPINT lpEroded, LPINT lpBless, LPINT lpIdent, LPINT lpWished);
	static bool WareHouseWithdrawItem(CInventory* pInventory, CItem* pItem, int updateType);
};

extern CAugmentation g_Augmentation;