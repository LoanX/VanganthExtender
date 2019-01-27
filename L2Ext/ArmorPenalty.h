#pragma once

#include "Parser.h"
enum ArmorMasteryType
{
	LIGHT_MASTERY = 1,
	HEAVY_MASTERY = 2,
	ROBE_MASTERY = 3
};

class CArmorMasteryDB
{
	map<ArmorMasteryType, list<int>> mData;
public:
	CArmorMasteryDB();
	void ReadData();
	bool CanEquip(User *pUser, int Type);
};
extern CArmorMasteryDB g_ArmorMasteryDB;

class CArmorPenalty
{
	BOOL m_Enabled;
	int m_SkillID;
	BOOL m_PenaltyForMixedItems;
public:
	void Initialize();
	void Activate(User *pUser);
	void Deactivate(User *pUser);
	void Validate(User *pUser);
	inline BOOL IsEnabled() { return m_Enabled; };
};
extern CArmorPenalty g_ArmorPenalty;