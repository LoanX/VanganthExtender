#pragma once

class CSkillOperateCondition;

class CSkillKey
{
public:
	WORD skill_id;
	BYTE  level;
	CSkillKey(WORD id = 0, BYTE lvl = 1)
		: skill_id(id), level(lvl) {}
	bool operator< (const CSkillKey& other) const
	{
		if (skill_id < other.skill_id)
			return true;
		if (skill_id > other.skill_id)
			return false;
		return level < other.level;
	}
	bool operator == (const CSkillKey& other) const
	{
		if(skill_id == other.skill_id && level == other.level)
			return true;

		return false;
	}
};

class CSkillInfo
{
public:
	enum BUFF_TYPE
	{
		BUFF = 0,
		DEBUFF = 1,
		BUFF_TYPE_NONE = 2
	};
	enum BASE_PROPERTY
	{
			PROP_STR = 1,
			PROP_DEX = 2,
			PROP_CON = 3,
			PROP_INT = 4,
			PROP_MEN = 5,
			PROP_WIT = 6,
			PROP_NONE = 26
	};
	enum TargetType
	{
		TargetSelf = 0,
		TargetTarget = 1, 
		TargetEnemy = 3,
		TargetEnemyOnly = 4,
		TargetItem = 5,
		TargetSummon = 6,
		TargetHolything = 7,
		TargetDoorTreasure = 9,
		TargetPCBody = 10,
		TargetNPCBody = 11
	};
	enum AFFECT_SCOPE
	{
		AS_SINGLE = 0,
		AS_PARTY = 1,
		AS_RANGE = 2,
		AS_POINT_BLANK = 3,
		AS_FAN = 5,
		AS_PLEDGE = 6,
		AS_SQUARE = 7,
		AS_SQUARE_PB = 9,
		AS_DEAD_PLEDGE = 10,
		AS_WYVERN_SCOPE = 11,
		AS_BALAKAS_SCOPE = 12,
		AS_NONE = 13
	};
	enum ATTRIBUTE_TYPE
	{
		W_SWORD	=	1,
		W_BLUNT	=	2,
		W_DAGGER	=	3,
		W_POLE	=	4,
		W_FIST	=	5,
		W_BOW	=	6,
		W_ETC	=	7,
		EARTH	=	9,
		WATER	=	10,
		WIND	=	11,
		FIRE	=	12,
		POISON	=	13,
		HOLY	=	14,
		HOLD	=	15,
		BLEED	=	16,
		SLEEP	=	17,
		SHOCK	=	18,
		DERANGEMENT = 19,
		UNHOLY	= 20,
		BUG_WEAKNESS = 21,
		ANIMAL_WEAKNESS = 22,
		PLANT_WEAKNESS = 23,
		BEAST_WEAKNESS = 24,
		DRAGON_WEAKNESS = 25,
		PARALYZE = 26,
		W_DUAL = 27,
		W_DUALFIST = 28,
		BOSS = 29,
		DEATH = 32,
		VALAKAS = 33
	};

/* 0000 */ PWCHAR wszName;
/* 0008 */ int nSkillID;
/* 000C */ int nLevel;
/* 0010 */ int nMagicLevel;
/* 0014 */ int nIsMagic;
/* 0018 */ int nIsEnergyAttack;
/* 001C */ UINT operateType;
/* 0020 */ int nConsumeMP1;
/* 0024 */ int nConsumeMP2;
/* 0028 */ int nConsumeHP;
/* 002C */ int n2C;
/* 0030 */ std::wstring wItemConsumeName;
/* 0058 */ int n58;
/* 005C */ int nItemConsumeCount;
/* 0060 */ int nCastRange;
/* 0064 */ int nEffectiveRange;
/* 0068 */ int n68;
/* 006C */ int n6C;
/* 0070 */ int n70;
/* 0074 */ int n74;
/* 0078 */ double dSkillHitTime;
/* 0080 */ double dSkillCoolTime;
/* 0088 */ double dSkillHitCancelTime;
/* 0090 */ double dReuseDelay;
/* 0098 */ vector<CSkillOperateCondition *> vOperateCondition;
/* 00B8 */ double LevelBonusRate;
/* 00C0 */ int nActivateRate;
/* 00C4 */ BASE_PROPERTY BaseProperty; //str- 1 dex-2 con-3 int-0? men-5 wit-6 none- 26
/* 00C8 */ int nAbnormalTime;
/* 00CC */ int nAbnormalLevel;
/* 00D0 */ int nAbnormalType;
/* 00D4 */ int nAbnormalVisualEffect;
/* 00D8 */ ATTRIBUTE_TYPE attributeType;
/* 00DC */ int nEffectPoint;
/* 00E0 */ int nDebuff; // 2- for non sets, 1 - debuff, 0- buff
/* 00E4 */ int nNextAction; //stand- 1 attack- 4 none- 0 sit-2 fakeDeath- 2
/* 00E8 */ int nFakeDeath; // fake -1
/* 00EC */ int nRideState;
/* 00F0 */ int nF0;
/* 00F4 */ int nF4;
/* 00F8 */ int nF8;
/* 00FC */ int nFC;
/* 0100 */ int n100;
/* 0104 */ int n104;
/* 0108 */ int n108;
/* 010C */ int n10C;
/* 0110 */ int n110;
/* 0114 */ int n114;
/* 0118 */ int n118;
/* 011C */ int n11C;
/* 0120 */ int n120;
/* 0124 */ int n124;
/* 0128 */ int n128;
/* 012C */ int n12C;
/* 0130 */ int n130;
/* 0134 */ int n134;
/* 0138 */ int n138;
/* 013C */ int n13C;
/* 0140 */ TargetType targetType;
/* 0144 */ AFFECT_SCOPE AffectScope;
/* 0148 */ int nAffectType;
/* 014C */ int nAffectRaceType;
/* 0150 */ int nMinZ;
/* 0154 */ int nMaxZ;
/* 0158 */ int nAffectRange;
/* 015C */ int nAffectLimitIncreaser;
/* 0160 */ int nAffectLimitRandom;
/* 0164 */ int n164; //End of CSkillInfo 0x0FFFFFFF
/* Extended values */
/* 0168 */ int nAttachSkillId;
/* 016C */ int nInstantCastTime;
/* 0170 */ int nResistCancel;
/* 0174 */ BOOL nGeodataCheck;

	void ActivateSkill(CObject *pCreature, CObject *pTarget, double dUnkn = 0, INT64 nUnkn = 0);
	bool IsMeleeAtk();
	bool IsSongOrDance();
	bool IsSelfBuff();
	bool CanUsedBy(CCreature *pCreature);
};

class CSkillDB
{
	CSkillDB* pSkillDB;
public:
	CSkillDB();
	CSkillInfo *GetSkillInfo(int nSkillID, int nLevel);
	void GetSkillIdLevelByName(const WCHAR* wName, LPINT Id, LPINT Level);
};

extern CSkillDB g_SkillDB;