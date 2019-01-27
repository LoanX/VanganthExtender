#pragma once

enum LogType
{
	LogBlack = 0,
	LogBlue = 1,
	LogError = 2
};

enum StoreType
{
	StoreNone = 0,
	StoreSell = 1,
	StoreBuy = 3,
	StoreRecipe = 5
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

enum RESTART_POINT_TYPE
{
	RESTART_POINT_VILLAGE = 0,
	RESTART_POINT_CLAN_HALL = 1,
	RESTART_POINT_CASTLE = 2,
	RESTART_POINT_SIEGE_HQ = 3,
	RESTART_POINT_FIXED = 4
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

enum ConsumeType
{
	ConsumeTypeNormal = 0,
	ConsumeTypeCharge = 1,
	ConsumeTypeStackable = 2,
	ConsumeTypeAsset = 3
};