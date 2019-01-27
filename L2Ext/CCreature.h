#pragma once
#include "Object.h"
#include "CInventory.h"
#include "ContributeData.h"
#include "IOCriticalSection.h"

struct ToggleSkillData
{
	CSkillInfo *pSI;
	bool InUse;
};

typedef map<int, ToggleSkillData> ToggleSkillMap;

class CTimeData
{
	DWORD nSeconds;
	DWORD nMiliseconds;
public:
	CTimeData(): nSeconds(0), nMiliseconds(0){};
	__forceinline void Set(DWORD Miliseconds) 
	{ 
		nSeconds = Miliseconds / 1000;
		nMiliseconds = Miliseconds % 1000;
	};
	__forceinline DWORD GetSeconds() { return nSeconds; };
	__forceinline DWORD GetMiliseconds() { return nMiliseconds; };

};

struct CActiveCubicInfo
{
	/* 0000 */ void* pCubicInfo; // there, short nCubicId is at offset 0
	/* 0008 */ int   _unk08[10];
	/* 0030 sizeof */
};

typedef vector<CActiveCubicInfo> CubicVector;

typedef map<CSkillKey, DWORD> SkillReuseMap;
typedef map<int, int> AcquiredSkillMap;
typedef map<int, int> AttachedSkillMap;
typedef vector<CSkillKey> AttachedSkillVector;

class CCreature : public CObject
{
public:
	enum STATUS_UPDATE
	{
		UPDATE_HP = 9,
		UPDATE_MP = 11,
		UPDATE_CP = 26
	};

	/* 0030 */ int _n0030;
	/* 0034 */ int _n0034;
	/* 0038 */ int _n0038;
	/* 003C */ int _n003C;
	/* 0040 */ int _n0040;
	/* 0044 */ int _n0044;
	/* 0048 */ int _n0048;
	/* 004C */ int _n004C;
	/* 0050 */ int _n50[44];
	/* 100 */ int _n100[64];
	/* 200 */ int _n200[64];
	/* 300 */ int _n300[64];
	/* 400 */ int _n400[64];
	/* 500 */ int _n500[34];
	/* 588 */ CreatureSharedData *pSD;
	/* 590 */ LPVOID _unkn590;
	/* 598 */ LPCIOCriticalSection lpSharedDataLock;
	/* 5A0 */ CInventory inventory;
    /* 608 */ int currentSetID;
	/* 60C */ int _unkn60C;
	/* 610 */ LPVOID _unkn610;
	/* 618 */ LPVOID _unkn618;
	/* 620 */ LPVOID _unkn620;
	/* 628 */ LPVOID _unkn628;
	/* 630 */ LPVOID _unkn630;
	/* 638 */ LPVOID _unkn638;
	/* 640 */ LPVOID _unkn640;
	/* 648 */ LPVOID _unkn648;
	/* 650 */ LPVOID _unkn650;
	/* 658 */ LPVOID _unkn658;
	/* 660 */ LPVOID _unkn660;
	/* 668 */ LPVOID _unkn668;
	/* 670 */ LPVOID _unkn670;
	/* 678 */ LPVOID _unkn678;
	/* 680 */ LPVOID _unkn680;
	/* 688 */ LPVOID _unkn688;
	/* 690 */ LPVOID _unkn690;
	/* 698 */ int npcKilledByGM;
	/* 69C */ int _unkn69C;
	/* 6A0 */ LPVOID _unkn6A0;
	/* 6A8 */ LPVOID _unkn6A8;
	/* 6B0 */ LPVOID _unkn6B0;
	/* 6B8 */ LPVOID _unkn6B8;
	/* 6C0 */ LPVOID _unkn6C0;
	/* 6C8 */ double physicalAttackPER;
	/* 6D0 */ double physicalAttackDIFF;
	/* 6D8 */ double physicalDefencePER;
	/* 6E0 */ double physicalDefenceDIFF;
	/* 6E8 */ double magicalAttackPER;
	/* 6F0 */ double magicalAttackDIFF;
	/* 6F8 */ double magicalDefencePER;
	/* 700 */ double magicalDefenceDIFF;
	/* 708 */ double hpRegenPER;
	/* 710 */ double hpRegenDIFF;
	/* 718 */ double mpRegenPER;
	/* 720 */ double mpRegenDIFF;
	/* 728 */ double speedPER;
	/* 730 */ double speedDIFF;
	/* 738 */ double attackSpeedDP;
	/* 740 */ double magicSpeedPER;
	/* 748 */ double magicSpeedDIFF;
	/* 750 */ double accuracyDIFF;
	/* 758 */ double evasionDIFF;
	/* 760 */ double criticalRatePER;
	/* 768 */ double criticalRateDIFF;
	/* 770 */ double magicCriticalRatePER;
	/* 778 */ double magicCriticalRateDIFF;
	/* 780 */ bool canMove;
	/* 781 */ bool canUseItem;
	/* 782 */ bool controlAbility;
	/* 783 */ bool canCastMagical;
	/* 784 */ bool canCastPhysical;
	/* 785 */ bool canCastOther;
	/* 786 */ bool preserveAbnormals;	//nobless blessing
	/* 787 */ bool canDamageHP;
	/* 788 */ bool canDamageMP;
	/* 789 */ bool reduceDropPenalty;
	/* 78A */ bool underFearEffect;
	/* 78B */ bool reflectPhysicalDamage;
	/* 78C */ bool _bUnkn78C;
	/* 78D */ bool _bUnkn78D;
	/* 78E */ bool _bUnkn78E;
	/* 78F */ bool _bUnkn78F;
	/* 790 */ double damageShieldPER;
	/* 798 */ double vampireAttackPER;
	/* 7A0 */ double accuracyPER;
	/* 7A8 */ double maxHpDIFF;
	/* 7B0 */ double maxHpPER;
	/* 7B8 */ double maxMpDIFF;
	/* 7C0 */ double maxMpPER;
	/* 7C8 */ double maxCpDIFF;
	/* 7D0 */ double maxCpPER;
	/* 7D8 */ double attackRangeDIFF;
	/* 7E0 */ double attackRangePER;
	/* 7E8 */ double criticalDamageDIFF;
	/* 7F0 */ double criticalDamagePER;
	/* 7F8 */ double skillReuseDelayPER;
	/* 800 */ double magicReuseDelayPER;
	/* 808 */ double otherReuseDelayPER;
	/* 810 */ double reduceCancelDIFF;
	/* 818 */ double reduceCancelPER;
	/* 820 */ double attackSpeedByWeaponPER;
	/* 828 */ double attackSpeedByWeaponDIFF;
	/* 830 */ double shieldDefenceRatePER;
	/* 838 */ int hennaINT;
	/* 83C */ int hennaSTR;
	/* 840 */ int hennaCON;
	/* 844 */ int hennaMEN;
	/* 848 */ int hennaDEX;
	/* 84C */ int hennaWIT;
	/* 850 */ double hpRegenByMoveModePER[4];
	/* 870 */ double hpRegenByMoveModeDIFF[4];
	/* 890 */ double mpRegenByMoveModePER[4];
	/* 8B0 */ double mpRegenByMoveModeDIFF[4];
	/* 8D0 */ double avoidByMoveModePER[4];
	/* 8F0 */ double avoidByMoveModeDIFF[4];
	/* 910 */ double safeFallHeightPER;
	/* 918 */ double safeFallHeightDIFF;
	/* 920 */ double breathPER;
	/* 928 */ double breathDIFF;
	/* 930 */ double weightLimitPER;
	/* 938 */ double _dUnkn938[8];
	/* 978 */ double healEffect;
	/* 980 */ double _dUnkn980[5];
	/* 9A8 */ int avoidAgro;
	/* 9AC */ int _nUnkn9AC;
	/* 9B0 */ double _dUnkn9B0;
	/* 9B8 */ double _dUnkn9B8;
	/* 9CO */ double _dUnkn9C0;
	/* 9C8 */ double _dUnkn9C8;
	/* 9D0 */ double _dUnkn9D0;
	/* 9D8 */ double _dUnkn9D8;
	/* 9E0 */ double _dUnkn9E0;
	/* 9E8 */ double _dUnkn9E8;
	/* 9F0 */ double criticalDamagePositionFrontPER;
	/* 9F8 */ double criticalDamagePositionFrontDIFF;
	/* A00 */ double criticalDamagePositionSidePER;
	/* A08 */ double criticalDamagePositionSideDIFF;
	/* A10 */ double criticalDamagePositionBackPER;
	/* A18 */ double criticalDamagePositionBackDIFF;
	/* A20 */ double shieldDefensePowerPER;
	/* A28 */ double shieldDefensePowerDIFF;
	/* A30 */ double skillMpCostPER;
	/* A38 */ double skillMpCostDIFF;
	/* A40 */ double magicMpCostPER;
	/* A48 */ double magicMpCostDIFF;
	/* A50 */ double otherMpCostPER;
	/* A58 */ double otherMpCostDIFF;
	/* A60 */ double _dA60;
	/* A68 */ double _dA68;
	/* A70 */ double _dA70;
	/* A78 */ bool shieldDefenseAngle;
	/* A79 */ bool _bUnknA79;
	/* A7A */ bool _bUnknA7A;
	/* A7B */ bool isBlockBuff;
	/* A7C */ bool isBlockDebuff;
	/* A7D */ bool _bUnknA7D;
	/* A7E */ bool _bUnknA7E;
	/* A7F */ bool _bUnknA7F;
	/* A80 */ int skillCriticalRate;	//base 26
	/* A84 */ int _nUnknA84;
	/* A88 */ double skillCriticalProbabilityPER;
	/* A90 */ double skillCriticalProbabilityDIFF;
	/* A98 */ int reflectPhysicalChance;
	/* A9C */ int reflectMagicalChance;
	/* AA0 */ int reflectOtherChance;
	/* AA4 */ int _nUnknAA4;
	/* AA8 */ double resistDispelByCategorySlotBuff;
	/* AB0 */ double resistDispelByCategorySlotDebuff;
	/* AB8 */ double resistAbnormalByCategorySlotBuff;
	/* AC0 */ double resistAbnormalByCategorySlotDebuff;
	/* AC8 */ int allowed_grade;
	/* ACC */ int enlarge_storage[7];  // CSkillEffect_p_enlarge_storage::Pump
				// 0 - inventory_normal, 1 - storage_private, 2 - ??, 3 - trade_sell, 4 - trade_buy
				// 5 - recipe_dwarven, 6 - recipe_common
	/* AE8 */ int hit_number;			// CSkillEffect_p_hit_number
	/* AEC */ int _nUnknAEC;
	/* AF0 */ int abnormal_type;  //-- set by.. abnormal_type [1-3]
	/* AF4 */ int _n0AF4;
	/* AF8 */ FVector _v0AF8;
	/* B10 */ int pendingAction; // (0..9)
	/* B14 */ DWORD tickDie;
	/* B18 */ DWORD tickStartFishing;
	/* B1C */ DWORD tickFishing;
	//----------- start of areamod ----
	/* B20 */	FVector	_v0B20;
	/* B38 */	bool    _b0B38;
	/* B39 */	bool    _b0B39;
	/* B3A */	bool    _b0B3A;
	/* B3B */	bool    _b0B3B;
	/* B3C */	bool    _b0B3C;
	/* B3D */	bool    _b0B3D;
	/* B3E */	bool    _b0B3E;
	/* B3F */	bool    _b0B3F;
	/* B40 */	bool    _b0B40;
	/* B41 */	bool    inPvpZone; // enter pvp area ? siege zone
	/* B42 */	bool    _b0B42;	//used for siege validation msg
	/* B43 */	bool    _b0B43;
	/* B44 */	bool    _b0B44;
	/* B45 */	bool    _b0B45; // CCreature__AreaCheckForEveryTick+223
	/* B46 */	bool    _b0B46;
	/* B47 */	bool    inPeaceZone;
	/* B48 */	bool    _b0B48;
	/* B49 */	bool    _b0B49;
	/* B4A */	bool    _b0B4A;
	/* B4B */	bool    _b0B4B; //not used
	/* B4C */	bool    _b0B4C; //not used
	/* B4D */	bool    _b0B4D; //not used
	/* B4E */	bool    _b0B4E; //not used
	/* B4F */	bool    _b0B4F; //not used
	/* B50 */	double  f_area_speed_coef; // 1.0
	/* B58 */	double  f_area_hp_regen_diff; // 0.0
	/* B60 */	double  f_area_mp_regen_diff; // 0.0
	/* B68 */	__int64 _i0B68; // 0 - id of system message for given area?
	/* B70 */	double  f_height_damage; // 0.0
	//----------- end of areamod ----
	/* B78 */	int     _n0B78; // 0,1,2,3,4
	/* B7C */	int     _n0B7C; //not used
	/* B80 */   double carry_weight_speed_coef;
	/* B88 */   double carryWeightHpRegenCoef;
	/* B90 */   double carryWeightMpRegenCoef;
	/* B98 */   double shieldDefencePower;
	/* BA0 */   double shieldDefenceRate;
	/* BA8 */   int expDrop;
	/* BAC */	int energyCount;
	/* BB0 */	int startCharningTime;
	/* BB4 */	int songSlotsUsed;
	/* BB8 */	int danceSlotsUsed;
	/* BBC */	DWORD fakeDeathTicks;
	/* BC0 */   double  _fDamage_BC0;
	/* BC8 */   int pump_cubic_mastery; // 0..3
	/* BCC */   bool    _b0BCC;
    /* BCD */   bool    _b0BCD; // p_hit_at_night condition
    /* BCE */   bool    _b0BCE;
    /* BCF */   bool    _b0BCF; 
	/* BD0 */ bool informAboutEffectByHp1;
	/* BD1 */ bool informAboutEffectByHp2;
	/* BD2 */ short _sUnkn1;
	/* BD4 */ int _nBD4[11];
	/* C00 */ int _nC00[37];
	
	/* C94 */ UINT requestSaveCharacterInfoTick; //200 = save tick time
	/* C98 */ UINT requestSaveItemInfoTick; //1000 = save tick time

	/* C9C */ UINT _unknC9C[2];

	/* CA4 */ bool broadcastInfoPacket;
	/* CA5 */ bool inCombatMode;
	/* CA6 */ bool _bUnknCA6;
	/* CA7 */ bool _bUnknCA7;
	/* CA8 */ UINT combatModeTimeStamp;
	/* CB0 */ int _nUnknCB0[7];
	/* CC8 */ UINT talkTargetId;
	/* CCC */ int _nUnknCCC;
	/* CD0 */ UINT targetId;
	/* CD4 */ int _nUnknCD4;
	/* CD8 */ CCreatureController *pCreatureController;
	/* CE0 */ int _nCE8[2];
	/* CE8 */ CIOCriticalSection acquiredSkillLock;
	/* D18 */ AcquiredSkillMap mAcquiredSkill;
	/* D30 */ CIOCriticalSection attachedSkillLock;
	/* D60 */ AttachedSkillMap mAttachedSkill;
	/* D78 */ AttachedSkillVector vAttachedSkill;
	/* D98 */ CIOCriticalSection skillReuseMapLock;
	/* DC8 */ SkillReuseMap mSkillReuse;
	/* DE0 */ CIOCriticalSection abnormalStatusLock;
	/* E10 */ CIOCriticalSection cubicLock; //cubic lock
	/* E40 */ AbnormalDataVector vAbnormalStatus;
	/* E60 */ CubicVector vCubic;
	/* E80 */ AbnormalData lifeForce;
	/* E98 */ CIOCriticalSection lifeForceLock;
	/* EC8 */ CIOCriticalSection ToggleSkillLock;
	/* EF8 */ LPVOID _pUnknEF8;	//map<int, int> mUsableActiveSkills
	/* F00 */ LPVOID _pUnknF00;
	/* F08 */ LPVOID _pUnknF08;
	/* F10 */ LPVOID _pUnknF10;
	/* F18 */ LPVOID _pUnknF18;	//vector<CSkillInfo*> someskills
	/* F20 */ LPVOID _pUnknF20;
	/* F28 */ LPVOID _pUnknF28;
	/* F30 */ ToggleSkillMap mToggleSkill;
	/* F48 */ LPVOID _pUnknF48;
	/* F50 */ LPVOID _pUnknF50;
	/* F58 */ LPVOID _pUnknF58;
	/* F60 */ LPVOID _pUnknF60;
	/* F68 */ LPVOID _pUnknF68;
	/* F70 */ LPVOID _pUnknF70;
	/* F78 */ LPVOID _pUnknF78;
	/* F80 */ LPVOID _pUnknF80;
	/* F88 */ int currentAbnormalEffect;
	/* F8C */ int _unk0F8C;
	/* F90 */ CIOCriticalSection _lock0F90;
	/* FC0 */ LPVOID _unknFC0;
	/* FC8 */ LPVOID _unknFC8;
	/* FD0 */ LPVOID _unknFD0;
	/* FD8 */ CIOCriticalSection _lock0FD8;
	/* 1008 */ int _unk1008[126];
	/* 1200 */ bool attackAttribute[34];
	/* 1222 */ WORD _w1222;
	/* 1224 */ int _dw1224;
	/* 1228 */ double defenceAttribute[34];
	/* 1338 */ int _dw1338[14];
	/* 1370 */ CContributeData *pContributeData;
	/* 1378 */ CIOCriticalSection pledgeContributionLock;	//during clan hall battle etc
	/* 13A8 */ map<int, double> mPledgeContribution;
	/* 13C0 */ LPVOID _unkn13C0;
	/* 13C8 */ LPVOID _unkn13C8;
	/* 13D0 */ LPVOID _unkn13D0;
	/* 13D8 */ LPVOID _unkn13D8;
	/* 13E0 */ CIOCriticalSection _lock13E0;
	/* 1410 */ LPVOID _unkn1410;
	/* 1418 */ LPVOID _unkn1418;
	/* 1420 */ LPVOID _unkn1420;
	/* 1428 */ LPVOID _unkn1428;
	/* 1430 */ LPVOID _unkn1430;
	/* 1438 */ LPVOID _unkn1438;
	/* 1440 */ LPVOID _unkn1440;
	/* 1448 */ LPVOID _unkn1448;
	/* 1450 */ LPVOID _unkn1450;
	/* 1458 */ LPVOID _unkn1458;
	/* 1460 */ LPVOID _unkn1460;
	/* 1468 */ LPVOID _unkn1468;
	/* 1470 */ LPVOID _unkn1470;
	/* 1478 */ LPVOID _unkn1478;
	/* 1480 */ LPVOID _unkn1480;
	/* 1488 */ LPVOID _unkn1488;
	/* 1490 */ LPVOID _unkn1490;
	/* 1498 */ LPVOID _unkn1498;
	/* 14A0 */ LPVOID _unkn14A0;
	/* 14A8 */ LPVOID _unkn14A8;
	/* 14B0 */ LPVOID _unkn14B0;
	/* 14B8 */ LPVOID _unkn14B8;
	/* 14C0 */ LPVOID _unkn14C0;
	/* 14C8 */ LPVOID _unkn14C8;
	/* 14D0 */ LPVOID _unkn14D0;
	/* 14D8 */ LPVOID _unkn14D8;
	/* 14E0 */ LPVOID _unkn14E0;
	/* 14E8 */ LPVOID _unkn14E8;
	/* 14F0 */ LPVOID _unkn14F0;
	/* 14F8 */ LPVOID _unkn14F8;
public:
	bool ValidCreature();
	bool ValidUser();
	bool IsNPC();
	bool IsSummon();
	bool IsPet();
	bool IsBoss();
	bool IsZZoldagu();
	bool IsMob();
	CInventory * GetInventory();
	UINT GetBuffAmount();
	void GetBuffDebuffAmount(UINT& buffAmount, UINT& debuffAmount, UINT& selfBuffAmount);
	void GetBuffDebuffAmount(UINT& buffAmount, UINT& debuffAmount);
	double GetAttribute(UINT attributeType);
	double GetAttributeBonus(CCreature *pOpponent);
	int GetDebuffAmount();
	UINT GetAbnormalAmount();
	double Distance(CCreature *pCreature);
	int GetAcquiredSkillLevel(int nSkillID);
	bool HaveSkill(int nSkillID, int nLevel);
	void ValidateHP();
	void ValidateMP();
	void ValidateParameters();
	bool ValidateBaseCP();
	bool ValidateBaseData();
	bool ValidateAllOnWeapon();
	bool ValidateSkillMod();
	void ValidateLocation();
	void InitializeSkillMod();
	void AddTimer(int timeout, int id);
	void AddContribution(CCreature *pEnemy, double value);
	bool ValidateAllOnSIM();
	bool ValidateSetItem(int nSlotType);
	void BroadcastFlyToLocation(int x, int y, int z);
	void CheckAbnormalVisualEffect();
	void Action(CCreature *pCreature, DWORD dOption1, DWORD dOption2);
	void AcquireSkill(int nSkillID, int nLevel, int Replace=0, int UseLock=1);
	void AttachSkill(int ID, int Level, int OperateType);
	void DeleteSkill(int nSkillID);
	bool InfectedSkillBy(CCreature *pBuffer, CSkillInfo *pSkillInfo);
	void WriteLock(PWCHAR wszFile, int nLine);
	void WriteUnlock();
	bool DeleteItemInInventory(int nItemID, int nAmount);
	void AddItemToInventory(int itemId, int amount, bool isPick = false, int enchanted = 0, int blessed = 0, int eroded = 0);
	void ValidateSkillList();
	bool Die(CCreature *pEnemy);
	bool IsShieldDefend(CCreature *pAttacker, double dCriticalBonus);
	void SetMagicLevel();
	void DispelAllByGM();
	void UpdateStatus(int nType);
	CSharedItemData *GetEquipedWeapon();
	CCreature *GetTarget();
	bool IsValidTalkTarget();	//IsNpc()
	bool CheckDistance(CCreature *pTarget, int Distance);
	bool IsInRange(CCreature *pTarget, int range);
	static CCreature *GetCreatureBySID(UINT *pSID);
	static bool IsValidServerId(PUINT pServerId);
	static CCreature *GetTalkTarget(PUINT pServerId);
	void SetSkillUsableTime(int nSkillID, int nLevel, DWORD *stamp, DWORD ReuseDelay);
	DWORD* GetSkillUsedTime(int nSkillID, int nLevel);
	DWORD* GetItemUsedTime(int itemClassID);
	void SetItemUsableTime(int itemClassID, DWORD* stamp, DWORD reusedelay);
	void SkillAction(int nSkillID, int nControl, bool bShift);
	void BuffCreature(int nSkillClassID);
	void DispelByAbnormal(int nAbnormalType, bool useSDLock = false);
	void TurnOffSpiritshot();
	void TurnOffSoulshot();
	FVector* GetPos(FVector* Pos);
	User* GetResponsibleUser();
	int HasAbnormal(int nAbnormalType);
	bool UpdateAbnormalLevel(int skillId, int newLevel, bool mayDown = false);
	void ResetSkillReuse();
	void SocialAction(SocialActionType ActionID);
	DWORD GetSkillReuse(int nSkillID);
	bool IsInAlliance(CCreature *pCreature);
	bool IsMyParty(CCreature *pCreature);
	void BroadcastCombatModeStart();
	void DispelNpcBuff();
	double GetDefAttribute(CSkillInfo::ATTRIBUTE_TYPE AttributeType);
	CTimeData GetSkillReuseEx(int SkillID, int Level=0);
	bool TurnOffToggleSkills();
	bool ToggleSkillAction(int nSkillID, int nLevel);
	void InsertAbnormalData(AbnormalData abnormal);
	void MakeCombat();
	void StopMove();
	bool ResurrectedBy(CCreature *pCreature = 0, INT32 percent=100);
	bool IsSameAbnormalTeam(CCreature *pOther);
	bool ValidateAttackSpeed();
	bool ValidateSpeed();
	double EquipedArmorSpeedByCrystal();
	double WeaponHitCorrectionByCrystal();
};
