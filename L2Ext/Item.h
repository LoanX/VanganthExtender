#pragma once

/*
CItem+30h- ItemSharedData
ish+70h - itemID
ish+88h - enchant
ish+80h - 1 - stackable 0 not stackable
*/

#pragma pack( push, 8 )

//size 140h
class CItemInfo
{
public:
	static CItemInfo* FromIndex(int itemInfoIndex)
	{
		if (itemInfoIndex < 0 || itemInfoIndex >= *(int*)(0x0BF71C4))
			return 0;
		return *(CItemInfo**)0x0E412178 + itemInfoIndex;
	}
	/* 0000 */ LPVOID _unkn0000;
	/* 0008 */ UINT defaultAction;
	/* 000C */ INT32 recipeId;
	/* 0010 */ INT32 etcType;				// This value can be 0x15 or 0x16 meaning its an blessed enchant !
	/* 0014 */ INT32 weight;
	/* 0018 */ INT32 price;
	/* 001C */ INT32 defaultPrice;
	/* 0020 */	short	itemSkillId;
	/* 0022 */	short	itemSkillLevel;
	/* 0024 */	short	criticalAttackSkillId;
	/* 0026 */	short	criticalAttackSkillLevel;
	/* 0028 */	short	attackSkillId;
	/* 002A */	short	attackSkillLevel;
	/* 002C */	short	magicSkillId;
	/* 002E */	short	magicSkillLevel;
	/* 0030 */	short	itemSkillEnchantedFourId;
	/* 0032 */	short	itemSkillEnchantedFourLevel;
	/* 0034 */	int magicSkillProb;
	/* 0038 */ INT32 armorType;
	/* 003C */ INT32 materialType;
	/* 0040 */ INT32 soulshotCount;
	/* 0044 */ INT32 spiritshotCount;
	/* 0048 */ INT32 reduceSoulshotProb;
	/* 004C */ INT32 reduceSoulshotCount;
	/* 0050 */ INT32 reduceSpiritshotProb;
	/* 0054 */ INT32 reduceSpiritshotCount;
	/* 0058 */ INT32 _dw0058;
	/* 005C */ UINT nCrystalType;
	/* 0060 */ INT32 nCrystalCount;
	/* 0064 */ INT32 nImmediateEffect;
	/* 0068 */ INT32 _dw0068;
	/* 006C */ INT32 _dw006C;
	/* 0070 */ INT32 _dw0070;
	/* 0074 */ bool canTrade;
	/* 0075 */ bool canDrop;
	/* 0076 */ bool canDestroy;
	/* 0077 */ bool _align77;
	/* 0078 */ INT32 magicWeapon;
	/* 007C */ INT32 physicalDamage;
	/* 0080 */ INT32 randomDamage;
	/* 0084 */ INT32 weaponType;
	/* 0088 */ INT32 canPenetrate;
	/* 008C */ INT32 critical;
	/* 0090 */ INT32 attackRange;
	/* 0094 */ INT32 damageRange[4];
	/* 00A4 */ INT32 attackSpeed;
	/* 00A8 */ INT32 reuseDelay;
	/* 00AC */ INT32 mpConsume;
	/* 00B0 */ INT32 reduceMpProb;
	/* 00B4 */ INT32 reduceMpCount;
	/* 00B8 */ INT32 magicalDamage;
	/* 00BC */ INT32 durability;
	/* 00C0 */ INT32 physicalDefense;
	/* 00C4 */ INT32 magicalDefense;
	/* 00C8 */ INT32 _align[30];
};

class CSharedItemData
{
public:
	static CSharedItemData* FromIndex(int itemIndex)
	{
		if (itemIndex < 0 || itemIndex >= *(int*)(0x0BF71C0))
			return 0;
		return *(CSharedItemData**)0x0E412138 + itemIndex;
	}

	CItemInfo* GetItemInfo()
	{
		typedef CItemInfo* (*f)(CSharedItemData*);
		return f(0x78DE20L)(this);
	}
	/* 0000 */ int _dw0000;
	/* 0004 */ int _dw0001;
	/* 0008 */ FVector position;
	/* 0020 */ INT32 _dw0020;
	/* 0024 */ INT32 nObjectID;
	/* 0028 */ int nItemIndex;
	/* 002C */ int nContainerIndex;
	/* 0030 */ int itemInfoIndex;
	/* 0034 */ INT32 _dw0034;
	/* 0038 */ INT32 _dw0038;
	/* 003C */ INT32 _dw003C;
	/* 0040 */ INT32 _dw0040;
	/* 0044 */ INT32 _dw0044;
	/* 0048 */ INT32 _dw0048;
	/* 004C */ INT32 _dw004C;
	/* 0050 */ UINT nCanBePickedUp;
	/* 0054 */ UINT nPreviousIndex;
	/* 0058 */ UINT nNextIndex;
	/* 005C */ INT32 _dw005C;
	/* 0060 */ INT32 _dw0060;
	/* 0064 */ UINT nItemState;
	/* 0068 */ INT32 nItemAmount;
	/* 006C */ UINT nDBID;
	/* 0070 */ UINT nItemID;
	/* 0074 */ UINT nItemType; //Weapon - 0, Armor - 1, Accessary - 2, Questitem - 3, Asset - 4
	/* 0078 */ UINT nSlotType;
	/* 007C */ UINT nBless;
	/* 0080 */ UINT nConsumeType; //normal = 0, charge = 1, stackable = 2, asset = 3
	/* 0084 */ UINT nDamaged;
	/* 0088 */ INT32 nEnchantLevel;
	/* 008C */ UINT _dw008C;
};
/*
2^nSlotID = client side
LEarring - 1
REarring - 2
Neclkace - 3
LRing - 4
RRing - 5
Helmet - 6
LHand - 7
RHand - 8
Gloves - 9
Shirt - 10
Pants - 11
Boots - 12
NewHair slot - 13 ??
Double Hand - 14
OnePieceArmor - 15
Left Party Item - 16
*/
//OrgSize : 0x58

class CItem: public CObject
{
public:
	enum ConsumeType
	{
		consume_type_normal = 0,
		consume_type_charge = 1,
		consume_type_stackable = 2,
		consume_type_asset = 3
	};
	enum SlotType //2^value = value in CSharedItemData
	{
		SlotType_None = -1,
		Underware = 0,
		REar = 1,
		LEar = 2,
		Neck = 3,
		RFinger = 4,
		LFinger = 5,
		Head = 6,
		RHand = 7,
		LHand = 8,
		Gloves = 9,
		Chest = 10,
		Legs = 11,
		Feet = 12,
		Back = 13,
		LRHand = 14,
		OnePiece = 15,
		Hair = 16,
		Dress = 17,
		//new slots
		HairDeco = 18,
		HairAll = 19
	};
public:
	/* 0030 */ CSharedItemData *pSID;
	/* 0038 */ CItemInfo *pII;
	/* 0040 */ LPCIOCriticalSection lpLock;
	/* 0048 */ INT64 nUnkn2;
	/* 0050 */ INT64 nUnkn3;
	/* 0058 */ INT64 nUnknEtcItem; //Extending CItem;
	/* 0060 */ UINT nAugmentationID;
	/* 0064 */ int nManaLeft; //ShadowItem
	/* 0068 */ int nLifeTime; //time when item will disappear
	/* 006C */ UINT nProtectionTimeout;	//item protection timeout timestamp
public:
	void Clear(){ this->nManaLeft = 0; this->nAugmentationID = 0; this->nProtectionTimeout = 0; };
	bool IsValidItem();
	CSharedItemData *GetSharedData();
	CWeapon *GetWeapon();
	bool IsLifeStone();
	int GetLifeStoneGrade();
	int GetLifeStoneLevel();
	void Delete();
	void Lock()
	{
		this->lpLock->Lock(NULL, NULL);
	}
	void Unlock()
	{
		this->lpLock->Unlock();
	}
	static void FindItemSp(CItemSP& itemSp, int itemIndex);
	static CSPointer<CItem> FindItemSP(int nItemIndex);
};

#pragma pack( pop )