#pragma once
#pragma pack( push, 1 )

struct QuestData
{
	int		questId;
	int		questState;
	int		questJournal;
	int		unk4;
};

class CDuelInfo
{
public:
	UINT OpponentSID;
	int DuelID;
	short DuelTeam;
	short Fighting;
	void Clear()
	{
		OpponentSID = 0;
		DuelID = 0;
		DuelTeam = 0;
		Fighting = 0;
	}
	void Set(UINT Opponent, int nDuelID, int nDuelTeam)
	{
		OpponentSID = Opponent;
		DuelID = nDuelID;
		DuelTeam = nDuelTeam;
	}
	void SetFighting(short sFight)
	{
		Fighting = sFight;
	}
};

class CSharedCreatureData
{
public:
	/* 0000 */ int	_dw0000;
	/* 0004 */ int	_dw0004;
	/* 0008 */ FVector Pos;
	/* 0020 */ WORD	sHeading;
	/* 0022 */ WORD us0022;
	/* 0024 */ UINT	nObjectID;
	/* 0028 */ int	nServerIndex;
	/* 002C */ int	nContainerIndex;
	/* 0030 */ int	_dw0030;
	/* 0034 */ int	_dw0034;
	/* 0038 */ int	_dw0038;
	/* 003C */ int	_dw003C;
	/* 0040 */ int	_dw0040;
	/* 0044 */ int	_dw0044;
	/* 0048 */ int	_dw0048;
	/* 004C */ int	_dw004C;
	/* 0050 */ int	nActiveUser;
	/* 0054 */ int	nSharedIndex;	//-1 - last or  used by const creature, -2 used by normal creature
	/* 0058 */ int	nNextIndex;	// -1 when used, else next sd index
	/* 005C */ int	_dw005C;	//0
	/* 0060 */ int	_dw0060;	//0
	/* 0064 */ int	nIsPC;		//1 player 0 - npc
	/* 0068 */ int	nAlive;		//1 - live 0- dead
	/* 006C */ int	_dw006C;	
	/* 0070 */ int	_dw0070;
	/* 0074 */ int	_dw0074;
	/* 0078 */ int	_dw0078;
	/* 007C */ int	_dw007C;
	/* 0080 */ int	_dw0080;
	/* 0084 */ int	_dw0084;
	/* 0088 */ int	_dw0088;
	/* 008C */ int	_dw008C;
	
	/* 0090 */ int	_dw0090; //Damage?
	/* 0094 */ int	nPrivateID;	//summon, private npc
	/* 0098 */ unsigned int	nSubClassIndex;
	/* 009C */ int	nNobless;	//0- none; 1- nobless;
	
	/* 00A0 */ int	nHero;	//0- none; 1- waiting; 2- hero;		
	/* 00A4 */ int	_dw00A4;
	/* 00A8 */ int	nNoblessIndex;
	/* 00AC */ int	nSubJobClass[4];
	/* 00BC */ int	nRace; // 0- human, 1- elf, 2- dark elf 3- orc, 4- dwarf, 23- mercenary, 24- castle_guard
	
	/* 00C0 */ int	nClass;
	/* 00C4 */ int	nSex;
	/* 00C8 */ WCHAR	wszName[25];
	/* 00FA */ bool	bUndying;
	/* 00FB */ bool	bCanBeAttacked;
	/* 00FC */ bool	bHide;
	/* 00FD */ bool	_b00FD;
	/* 00FE */ short	_w00FE;
	/* 0100 */ double fCollisionRadius; // NPC default 16
	/* 0108 */ double fCollisionHeight; // NPC default 1000
	/* 0110 */ int	nHairShape;
	/* 0114 */ int	nHairColor;	
	/* 0118 */ int	nFaceIndex;	
	/* 011C */ int	nExp;
	/* 0120 */ int	nSp;

	/* 0124 */ int nLastItemIndex; //first item in inventory
	/* 0128 */ int nEquipment[18]; //Item Index

	/* 0170 */ int	nDefaultINT;
	/* 0174 */ int	nDefaultSTR;
	/* 0178 */ int	nDefaultCON;
	/* 017C */ int	nDefaultMEN;
	/* 0180 */ int	nDefaultDEX;
	/* 0184 */ int	nDefaultWIT;
	
	/* 0188 */ double	fHP;
	/* 0190 */ double	fMP;
	/* 0198 */ double	fCP;
	
	/* 01A0 */ int _n1A0;
	/* 01A4 */ int nMoveType;
	/* 01A8 */ int nStopMode;
	/* 01AC */ int _n1AC;
	/* 01B0 */ int _n1B0; //Used by CSPointer FindObjectSP
	/* 01B4 */ int _n1B8;
	/* 01B8 */ int nTargetIndex;
	/* 01BC */ UINT nTargetObjId;
	/* 01C0 */ int _n1C0;
	/* 01C4 */ bool bGuilty;	//1C5 - in siege zone
	/* 01C5 */ bool bSiegeGround; //freezed ?
	/* 01C6 */ bool bUnkn1C6;
	/* 01C7 */ bool bUnkn1C7;
	/* 01C8 */ int nPvpCount;
	/* 01CC */ int nPkCount;
	/* 01D0 */ int nPkPardon;
	/* 01D4 */ int nKarma;
	/* 01D8 */ int nRecommendationsLeft;
	/* 01DC */ int nEvalScore;
	
	/* 01E0 */ long nPvPAttackTimeStamp;
	/* 01E4 */ int nQuestDataU[16][4]; //QuestData
	/* 02E4 */ BYTE OneTimeQuestBitMap[32];
	/* 0304 */ int nQuestLastRewardTime;
	/* 0308 */ int nLastAttackerID;
	/* 030C */ int _n30C;
	/* 0310 */ int _n310;
	/* 0314 */ int _n314;
	/* 0318 */ int nPledgeID;
	/* 031C */ int _n31C;
	/* 0320 */ int nIsPledgeLeader;
	/* 0324 */ int nAllianceID;
	/* 0328 */ int _n328;
	/* 032C */ int nPartyID;
	/* 0330 */ int _n330;
	/* 0334 */ int _n334;
	/* 0338 */ int nResidenceID;
	/* 033C */ int _n33C;
	/* 0340 */ int nBossResidenceID; //in siege agit/npc battle agit etc
	/* 0344 */ int nYongmaType;
	/* 0348 */ int nYongmaClass;
	/* 034C */ UINT nStoreMode;	//PrivateStoreBuy - 3, PrivateStoreSell - 1, Dwarven Recipe - 5
	/* 0350 */ int _n350;
	/* 0354 */ int _n354;
	/* 0358 */ int _n358;
	/* 035C */ int nSoulshotGrade; //6- S 5-A 4-B 3-C 2-D 1-NG
	/* 0360 */ double dSoulshotPower; 
	/* 0368 */ int bSpiritshot; //Spiritshot /Blessed spiritshot = 1
	/* 036C */ int bIsRiding;
	
	/* 0370 */ double dSpiritShotPower;
	/* 0378 */ double dSpiritShotSkillReuseReduce;
	
	/* 0380 */ double _d0380;
	/* 0388 */ bool _b388; 
	/* 0389 */ bool _b389;
	/* 038A */ short _w38A;
	/* 038C */ int _n38C;
	/* 0390 */ int _n390;
	/* 0394 */ int nDuringRessurection;
	/* 0398 */ int _n398;
	/* 039C */ int _n39C;
	/* 03A0 */ int _n3A0;
	/* 03A4 */ int _n3A4;
	/* 03A8 */ int _n3A8;
	/* 03AC */ int _n3AC;
	/* 03B0 */ int _n3B0;
	/* 03B4 */ int _n3B4;
	/* 03B8 */ int _n3B8;
	/* 03BC */ int _n3BC;
	/* 03C0 */ int _n3C0;
	/* 03C4 */ int _n3C4;
	/* 03C8 */ int _n3C8;
	/* 03CC */ int nRessurectExp;	//percent of resurrected exp
	/* 03D0 */ int nRessurectTickCount;
	/* 03D4 */ int _n3D4;
	/* 03D8 */ int nIsNewbie;
	/* 03DC */ int nDBID;
	
	/* 03E0 */ int nDefaultWeaponType;
	/* 03E4 */ int _n3E4;
	/* 03E8 */ int nDefaultAttackRange;
	/* 03EC */ int nDefaultDamageRange[4];
	/* 03FC */ int nDefaultRandomDamage;

	/* 0400 */ int nDefaultBasePhysAttack;
	/* 0404 */ int _n404;
	/* 0408 */ int nDefaultBaseAttackSpeed;
	/* 040C */ int nDefaultBaseCritical;

	/* 0410 */ int _n410;
	/* 0414 */ int nBaseAttackSpeed;
	/* 0418 */ int nBaseCritical;

	/* 041C */ int nINT;
	/* 0420 */ int nSTR;
	/* 0424 */ int nCON;
	/* 0428 */ int nMEN;
	/* 042C */ int nDEX;
	/* 0430 */ int nWIT;
	
	/* 0434 */ int nHenna1;
	/* 0438 */ int nHenna2;
	/* 043C */ int nHenna3;
	/* 0440 */ int _n440;
	//=====================//
	/* 0444 */ int nLevel;
	/* 0448 */ int _n448;
	/* 044C */ int _n44C;
	/* 0450 */ int _n450;
	/* 0454 */ int _n454;
	/* 0458 */ int _n458;
	/* 045C */ int _n45C;
	/* 0460 */ int _n460;
	/* 0464 */ int _n464;
	/* 0468 */ int _n468;
	/* 046C */ int _n46C;
	/* 0470 */ int _n470;
	/* 0474 */ int _n474;
	/* 0478 */ int _n478;
	/* 047C */ int _n47C;
	/* 0480 */ int _n480;
	/* 0484 */ int _n484;
	/* 0488 */ int _n488;
	/* 048C */ int _n48C;
	/* 0490 */ int nMaxSlot;
	/* 0494 */ int nCurrSlot;
	/* 0498 */ int nCurrWeight;
	/* 049C */ int _n49C;
	/* 04A0 */ double fMaxLoad;
	/* 04A8 */ double fOrgHP;
	/* 04B0 */ double fOrgMP;
	/* 04B8 */ double fOrgCP; 
	/* 04C0 */ double MaxHP; 
	/* 04C8 */ double MaxMP;
	/* 04D0 */ double MaxCP; 
	/* 04D8 */ double _fHpRegen;
	/* 04E0 */ double _fMpRegen;
	/* 04E8 */ double _fCPRegen;// forgspeed ?
	/* 04F0 */ double _f4f0;	//not used in validate speed
	/* 04F8 */ double _f4F8;
	/* 0500 */ double _f500;
	/* 0508 */ double _f508;
	/* 0510 */ double fSpeed;
	/* 0518 */ double fHit;
	/* 0520 */ double fAvoid;
	/* 0528 */ double _f528;
	/* 0530 */ double fNoise;
	/* 0538 */ double fAccuracy;
	/* 0540 */ double fEvasion;
	/* 0548 */ double _f548;
	/* 0550 */ int _n550;
	/* 0554 */ int _n554;
	/* 0558 */ int _n558;
	/* 055C */ int _n55C;
	/* 0560 */ int _n560;
	/* 0564 */ int _n564;
	
	/* 0568 */ UINT nWeaponId; //WeaponID
	/* 056C */ UINT nWeaponType;		/* none=0 sword= 1; blunt= 2; dagger= 3; pole= 4; fist= 5; bow= 6; etc= 7; dual=8; dualfist=9; fishing_rod=10; */
	
	/* 0570 */ int _n570;		
	/* 0574 */ int nArmorType;
	/* 0578 */ int _n578;
	/* 057C */ int nWeaponRange;
	/* 0580 */ int nDamageRange1;
	/* 0584 */ int nDamageRange2;
	/* 0588 */ int nDamageRange3;
	/* 058C */ int nDamageRange4;
	/* 0590 */ int nWeaponRandomDmg;
	/* 0594 */ int nWeaponPhysDmg;
	/* 0598 */ int _n598;
	/* 059C */ int nWeaponAtkSpeed;
	/* 05A0 */ int _n5A0;
	/* 05A4 */ int nWeaponMagicDmg;
	/* 05A8 */ int _n5A8;
	/* 05AC */ int _n5AC;
	/* 05B0 */ double fPAtk;
	/* 05B8 */ double fPDef;
	/* 05C0 */ double fMAtk;
	/* 05C8 */ double fMDef;
	/* 05D0 */ double fAtkSpeed;
	/* 05D8 */ double fBaseCritical;
	/* 05E0 */ double fBaseAtkSpeed;
	/* 05E8 */ double fBaseCastSpeed; //AtkSpeed
	/* 05F0 */ double fCastSpeed;
	/* 05F8 */ int _n5F8;
	/* 05FC */ int _n5FC;
	/* 0600 */ int _n600;
	/* 0604 */ int _n604;
	/* 0608 */ int _n608;
	/* 060C */ int _n60C;
	/* 0610 */ int _n610;
	/* 0614 */ int _n614;
	/* 0618 */ int nBuilder;
	/* 061C */ int _n61C;
	/* 0620 */ int _n620;
	/* 0624 */ int _n624;
	/* 0628 */ int _n628;
	/* 062C */ int nLastBlowWeaponClassID;
	/* 0630 */ int nSSQRoundNumber;
	/* 0634 */ int nSSQPart;
	/* 0638 */ int nSSQPosition;;
	/* 063C */ int nSealSelectionNo;
	/* 0640 */ int nSSQPoints;
	/* 0644 */ int _n644;
	/* 0648 */ int _n648;
	/* 064C */ int _n64C;
	/* 0650 */ int _n650;
	/* 0654 */ int _n654;
	/* 0658 */ int _n658;
	/* 065C */ int _n65C;
	/* 0660 */ int nSSQDawnRound;
	/* 0664 */ int nInPeaceZone;
	/* 0668 */ int nInBattleField;
	/* 066C */ WCHAR wszAI[64];
	/* 06EC */ int _n6EC;
	/* 06F0 */ int _n6F0;
	/* 06F4 */ int _n6F4;
	/* 06F8 */ int _n6F8;
	/* 06FC */ int _n6FC;
	/* 0700 */ int _n0700;
	/* 0704 */ int _n0704;
	/* 0708 */ int _n0708;
	/* 070C */ int _n070C;
	/* 0710 */ int _n0710;
	/* 0714 */ int _n0714;
	/* 0718 */ int _n0718;
	/* 071C */ int _n071C;
	/* 0720 */ int _n0720;
	/* 0724 */ int _n0724;
	/* 0728 */ int _n0728;
	/* 072C */ int _n072C;
	/* 0730 */ int _n0730;
	/* 0734 */ int _n0734;
	/* 0738 */ int _n0738;
	/* 073C */ int _n073C;
	/* 0740 */ int _n0740;
	/* 0744 */ int _n0744;
	/* 0748 */ int _n0748;
	/* 074C */ int _n074C;
	/* 0750 */ int _n0750;
	/* 0754 */ int _n0754;
	/* 0758 */ int _n0758;
	/* 075C */ int _n075C;
	/* 0760 */ int _n0760;
	/* 0764 */ int _n0764; //Attacker SID ?
	/* 0768 */ int _n0768;
	/* 076C */ int _n076C;
	/* 0770 */ int _n0770;
	/* 0774 */ int _n0774;
	/* 0778 */ int nSummonType;
	/* 077C */ int _n077C;
	/* 0780 */ UINT nMasterID;
	/* 0784 */ int _n0784;
	/* 0788 */ int nNpcClassID;
	/* 078C */ int nWeightPoint;
	/* 0790 */ int nRespawnTime;
	/* 0794 */ int _n0794;
	/* 0798 */ int _n0798;
	/* 079C */ int _n079C;
	/* 07A0 */ int _n07A0;
	/* 07A4 */ int _n07A4;
	/* 07A8 */ int _n07A8;
	/* 07AC */ int _n07AC;
	/* 07B0 */ int nClanHelpRange;
	/* 07B4 */ int _n07B4;
	/* 07B8 */ double AcquireExpRate;
	/* 07C0 */ int nPState;
	/* 07C4 */ int _n07C4;
	/* 07C8 */ int _n07C8;
	/* 07CC */ int _n07CC;
	/* 07D0 */ int nAvoidAgro; // BYTE bPassive
	/* 07D4 */ int _n07D4;
	/* 07D8 */ CSharedCreatureData *pMasterSD;
	/* 07E0 */ int nAction;
	/* 07E4 */ int _n07E4;
	/* 07E8 */ int _n07E8;
	/* 07EC */ int _n07EC;
	/* 07F0 */ int _n07F0;
	/* 07F4 */ int nIsSpoiled; //byte
	/* 07F8 */ int _n07F8;
	/* 07FC */ int _n07FC;
	/* 0800 */ bool bCanBeSown;
	/* 0801 */ bool isSown;	//1 when seeded
	/* 0802 */ bool bUnkn802;
	/* 0803 */ bool bUnkn803;
	/* 0804 */ int nSeedType; // normal - 1 and alternative - 2
	/* 0808 */ UINT nSeederSID;
	/* 080C */ int _n080C;
	/* 0810 */ int _n0810;
	/* 0814 */ int nCurrentFeed;
	/* 0818 */ int nMaxFeed;
	/* 081C */ int nPetDBID;
	/* 0820 */ int nStriderLevel;
	/* 0824 */ int _n0824;
	/* 0828 */ int nMaxMagicLevel;
	/* 082C */ DWORD dHeroChatDelay; //Old AbnormalTable Start
	/* 0830 */ DWORD nSubJobManipulateDelay;
	/* 0834 */ int nIsOlympiadUser;
	/* 0838 */ int nPledgeType;
	/* 083C */ int nCursedVictim;
	/* 0840 */ int nCursedOwner;
	/* 0844 */ UINT nAugmentationID;
	/* 0848 */ UINT nAugmentationTmp;
	/* 084C */ CDuelInfo DuelInfo;
	/* 0858 */ UINT nSponsorSID;
	/* 085C */ UINT nApprenticeSID;
	/* 0860 */ UINT nGetOffTimer;
	/* 0864 */ UINT activateOfflineShopTime;
	/* 0868 */ UINT _notUsed868;
	/* 086C */ int nSecondResurrection;
	/* 0870 */ int nPrivMsgFlag;
	/* 0874 */ int nPrivateStoreTimer;
	/* 0878 */ int nWaitingForMatch;
	/* 087C */ int nSpiritCounter;
	/* 0880 */ int nBlockSystemMessage; // Blocks system messages
	/* 0884 */ int nInCustomPvpZone;
	/* 0888 */ int nInSSQ_ZONE;
	/* 088C */ UINT nIsCCOwner;
	/* 0890 */ INT32 nCCPartyAmount;
	/* 0894 */ int _notUsed894;
	/* 0898 */ int nIsObserver;
	/* 089C */ int nInNickNameChangeZone;
	/* 08A0 */ int nInBlockSkillsZone;
	/* 08A4 */ int nInBlockItemsZone;
	/* 08A8 */ int nInRaceChangeZone;
	/* 08AC */ int nInNoOlBuffsZone;
	/* 08B0 */ int nInBlockChatZone;
	/* 08B4 */ int buffedByBuffer;
	/* 08B8 */ int teamType;	/* 0 - None, 1 - Blue, 2 - Red */
	/* 08BC */ UINT nTimerTick;
	/* 08C0 */ UINT nProtectionTimeoutTmp;
	/* 08C4 */ int nNotUsed8C4;
	/* 08C8 */ int nNotUsed8C8;	//End of OldAbnormal Table
	/* 08CC */ int nDBValue; 
	/* 08D0 */ int nDualGara1;
	/* 08D4 */ int nDualGara2;
	/* 08D8 */ int nTradeSkillLevel;
	/* 08DC */ int nMakerParam1;
	/* 08E0 */ int nMakerParam2;
	/* 08E4 */ int nMakerParam3;
	/* 08E8 */ __int64 n64_PledgePower1;
	/* 08F0 */ __int64 n64_PledgePower2;
	/* 08F8 */ __int64 n64_PledgePower3;
	/* 0900 */ __int64 n64_PledgePower4;
	/* 0908 */ BYTE bLang; //Korea=0, USA=1, Japan=2, Taiwan=3, China=4, Thailand=5
	/* 0909 */ BYTE bUnkn909;
	/* 090A */ short w90A;
	/* 090C */ int n90C;
	/* 0910 */ int AbnormalTable[40][2]; //AbnormalType, Level
	/* 0A50 */ LPVOID *pExData;
	/* 0A58 */ QuestData Quests[26];
	/* 0BF8 */ int vipLevel;	/* vip level */
	/* 0BFC */ DWORD vipTimestamp;	/* timestamp for vip - expiration time */
	/* 0C00 */ int votePoints;
	/* 0C04 */ DWORD voteTimestamp;
	/* 0C08 */ DWORD aioTimestamp;
};

#pragma pack( pop )