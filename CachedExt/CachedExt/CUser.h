#pragma once
#include "MemoryObject.h"

#pragma pack( push, 1 )
struct QuestData
{
	__int32		questId;
	__int32		questState;
	__int32		questJournal;
	__int32		unk4;
};

// original size - 5A8
class CUser : public MemoryObject
{
public:
/* 004 */	__int8	unk000[0x34];
/* 038 */	__int32	unk038;
/* 03C */	__int8	unk03C[0x88];
/* 0C4 */	__int32 charId;
/* 0C8 */	__int8	unk0C8[0x94];
/* 15C */	__int32	experience;
/* 160 */	__int8	unk160[0x2D0];
/* 430 */	QuestData originalQuests[16];	// 16 * 16= 0x100
/* 530 */	bool	is_online;
/* 531 */	__int8	unkn531[0x2F];
/* 560 */	__int8	unk560;
/* 561 */	__int8	unk561[0x47];
/* 5A8 */	// end of original CUser
/* 5A8 */	QuestData	quests[26];
/* 748 */

	void SaveMemo();
	void SetQuest(__int32 c1);
	void WriteLock(const wchar_t* file, int line);
	void WriteUnlock();
	wchar_t* GetCharName();
	int GetId();
	wchar_t* GetAccountName();
	int GetAccountID();
	int GetPledgeID();
	int GetBuilder();
	int GetGender();
	int GetRace();
	int GetClass();
	int GetWorld();
	int GetX();
	int GetY();
	int GetZ();
	double GetHP();
	double GetMP();
	double GetCP();
	int GetSP();
	int GetExp();
	int GetLevel();
	int GetAlign();
	int GetPK();
	int GetDuel();
	int GetPKPardon();
	int GetUnderware();
	int GetRight_ear();
	int GetLeft_ear();
	int GetNeck();
	int GetRight_finger();
	int GetLeft_finger();
	int GetHead();
	int GetRight_hand();
	int GetLeft_hand();
	int GetGloves();
	int GetChest();
	int GetLegs();
	int GetFeet();
	int GetBack();
	int GetBoth_hand();
	int GetHair();
	int GetExpDiff();
	int GetLevelByExp(int exp);

	CWareHouseSP GetWareHouse(int nType, bool bLoadIfNotExist) //0 - inventory, 1- warehouse, 2- clan warehouse
	{
		typedef CWareHouseSP* (__thiscall *_gwh)(CUser*, CWareHouseSP*, int, bool);
		_gwh gwh = (_gwh) 0x004A9D30;
		CWareHouseSP tmp;
		gwh(this, &tmp, nType, bLoadIfNotExist);
		return tmp;
	};
};

#pragma pack( pop )
