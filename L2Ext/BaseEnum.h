#pragma once

enum ConsumeType
{
	ConsumeNormal = 0,
	ConsumeCharge = 1,
	ConsumeStackable = 2,
	ConsumeAsset = 3
};

enum StoreType
{
	StoreNone = 0,
	StoreSell = 1,
	StorePrepareSell = 2, //??
	StoreBuy = 3,
	StorePrepareBuy = 4, //??
	StoreRecipe = 5,
	StorePrepareRecipe = 6	//??
};

enum TerritoryType
{
	TERRITORY_NONE = -1,
	FORBIDDEN_FLY_ZONE = 0,
	FORBIDDEN_RECALL_ZONE = 1,
	FORBIDDEN_RIDE_ZONE = 2,
	//CastleZones
	GLUDIO_CASTLE = 3,
	DION_CASTLE = 4,
	GIRAN_CASTLE = 5,
	OREN_CASTLE = 6,
	ADEN_CASTLE = 7,
	INNADRIL_CASTLE = 8,
	GODDARD_CASTLE = 9,
	RUNE_CASTLE = 10,
	SCHUTTGARD_CASTLE = 11,
	//end of castle zones
	FORBIDDEN_OFFLINE_SHOP_ZONE = 12,
	TRADE_GEODATA_CHECK_ZONE = 13,
	FORBIDDEN_TRADE_ZONE = 14,
	SSQ_ZONE = 15,
	PVP_ZONE = 16,
	ANTI_RECALL_ZONE = 17,
	NICKNAME_CHANGE_ZONE = 18,
	BLOCK_ITEMS_ZONE = 19,
	BLOCK_SKILLS_ZONE = 20,
	RACE_CHANGE_ZONE = 21,
	NO_OL_BUFFS_ZONE = 22,
	BLOCK_CHAT_ZONE = 23,
	BLOCK_TVT_ZONE = 24,
	PVP_REWARD_ZONE = 25,
	DESPAWN_SUMMON_ZONE = 26,
	DISPEL_SKILL_ZONE = 27,
	MINING_ZONE = 28,
	LEVEL_RESTRICTION_ZONE = 29,
	BLOCK_SCHEME_BUFFER_ZONE = 30,
	DISPEL_ALL_ZONE = 31,
	NO_PARTY_ZONE = 32,
	PARTY_ZONE = 33,
	NO_RESTART_ZONE = 34,
	REQUIRED_ITEM_ZONE = 35,
	INSTANCE_ZONE = 36,
	RESPAWN_ZONE = 37,
	NOBLESS_ZONE = 38,
	PRESERVE_ABNORMAL_ZONE = 39,
	TERRITORY_COUNT = 40
};

enum StatType
{
	StatInt = 0,
	StatStr = 1,
	StatCon = 2,
	StatMen = 3,
	StatDex = 4,
	StatWit = 5
};

enum OperateType
{
	OperateA1 = 0,
	OperateA2 = 1,
	OperateA3 = 2,
	OperateA4 = 3,
	OperatePassive = 4,
	OperateToggle = 5,
	OperatePP = 6
};

enum EtcItemType
{
	EtcItemNone = 0,
	EtcItemScroll = 1,
	EtcItemArrow = 2,
	EtcItemPotion = 3,
	EtcItemSpellbook = 4,
	EtcItemRecipe = 5,
	EtcItemMaterial = 6,
	EtcItemPetCollar = 7,
	EtcItemCastleGuard = 8,
	EtcItemDye = 9,
	EtcItemSeed = 10,
	EtcItemSeed2 = 11,
	EtcItemHarvest = 12,
	EtcItemLotto = 13,
	EtcItemRaceTicket = 14,
	EtcItemTicketOfLord = 15,
	EtcItemLure = 16,
	EtcItemCrop = 17,
	EtcItemMatureCrop = 18,
	EtcItemScrollEnchantWeapon = 19,
	EtcItemScrollEnchantArmor = 20,
	EtcItemBlessedScrollEnchantWeapon = 21,
	EtcItemBlessedScrollEnchantArmor = 22,
	//Custom etc item types
	EtcItemInfinitySoulshot = 23,
	EtcItemInfinitySpiritshot = 24,
	EtcItemInfinityBlessedSpiritshot = 25
};

enum SexType
{
	SexMale = 0,
	SexFemale = 1
};

enum RaceType
{
	RaceHuman = 0,
	RaceElf = 1,
	RaceDarkElf = 2,
	RaceOrc = 3,
	RaceDwarf = 4,
	RaceMercenary = 23,
	RaceCastleGuard = 24
};

enum ShortCutType
{
	ShortCutItem = 1,
	ShortCutSkill = 2,
	ShortCutAction = 3,
	ShortCutMacro = 4,
	ShortCutRecipe = 5
};

enum SocialActionType
{
	SocialActionGreet = 2,
	SocialActionVictory = 3,
	SocialActionAdvance = 4,
	SocialActionNo = 5,
	SocialActionYes = 6,
	SocialActionBow = 7,
	SocialActionUnaware = 8,
	SocialActionWaiting = 9,
	SocialActionLaugh = 10,
	SocialActionApplaus = 11,
	SocialActionDance = 12,
	SocialActionSad = 13,
	SocialActionLevelUp = 15,
	SocialActionHero = 16,
	SocialActionCursedWeaponLevelUp = 17
};

enum SAY_TYPE
{
	SAY_ALL = 0,
	SAY_SHOUT = 1, //!
	SAY_TELL = 2,
	SAY_PARTY = 3, //#
	SAY_CLAN = 4,  //@
	SAY_GM = 5,
	SAY_PETITION_PLAYER = 6, // used for petition
	SAY_PETITION_GM = 7, //* used for petition
	SAY_TRADE = 8, //+
	SAY_ALLIANCE = 9, //$
	SAY_ANNOUNCEMENT = 10,
	SAY_PARTYROOM_ALL = 16, //(Red)
	SAY_PARTYROOM_COMMANDER = 15, //(Yellow)
	SAY_HERO_VOICE = 17
};

enum RestartPointType
{
	RestartPointVillage = 0,
	RestartPointClanHall = 1,
	RestartPointCastle = 2,
	RestartPointSiegeHQ = 3,
	RestartPointFixed = 4
};

enum SlotType
{
	SlotNone = 0,
	SlotUnderwear = 1,
	SlotRightEar = 2,
	SlotLeftEar = 4,
	SlotNeck = 8,
	SlotRightFinger = 16,
	SlotLeftFinger = 32,
	SlotHead = 64,
	SlotRightHand = 128,
	SlotLeftHand = 256,
	SlotGloves = 512,
	SlotChest = 1024,
	SlotLegs = 2048,
	SlotFeet = 4096,
	SlotBack = 8192,
	SlotRightLeftHand = 16384,
	SlotLeftRightHand = 16384,
	SlotOnePiece = 32768,
	SlotHair = 65536,
	SlotAllDress = 131072,
	SlotHairEx = 262144,
	SlotHairAll = 524288
};

enum ArmorType
{
	ArmorNone = 0,
	ArmorLight = 1,
	ArmorHeavy = 2,
	ArmorMagic = 3
};

enum WeaponType
{
	WeaponNone = 0,
	WeaponSword = 1,
	WeaponBlunt = 2,
	WeaponDagger = 3,
	WeaponPole = 4,
	WeaponFist = 5,
	WeaponBow = 6,
	WeaponEtc = 7,
	WeaponDual = 8,
	WeaponDualFist = 9,
	WeaponFishingRod = 10
};

enum CrystalType
{
	CrystalNone = 0,
	CrystalD = 1,
	CrystalC = 2,
	CrystalB = 3,
	CrystalA = 4,
	CrystalS = 5
};

enum CastleType
{
	CastleGludio = 1,
	CastleDion = 2,
	CastleGiran = 3,
	CastleOren = 4,
	CastleAden = 5,
	CastleInnadril = 6,
	CastleGoddard = 7,
	CastleRune = 8,
	CastleSchuttgard = 9
};