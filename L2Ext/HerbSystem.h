#pragma once

struct HerbDropInfo
{
	INT32 itemId;
	INT32 minCount;
	INT32 maxCount;
	double chance;
};

struct HerbGroupDropInfo
{
	vector<HerbDropInfo> items;
	double chance;
	double itemsChanceSum;
};

struct NpcHerbDropInfo
{
	UINT classId;	// npcId + 1000000
	vector<HerbGroupDropInfo> dropGroup;
};

class CHerbSystem
{
	static const int LOWEST_HERB_SKILL_ID = 2280;
	static const int HIGHEST_HERB_SKILL_ID = 2285;
	BOOL m_Enabled;
	BOOL m_AffectSummon;
	map<UINT, NpcHerbDropInfo*> m_Drop;
	void LoadData();
	bool ParseDropList(vector<HerbGroupDropInfo>& dropList, wstring data, UINT npcId);
public:
	CHerbSystem();
	~CHerbSystem();
	void Init();
	bool IsHerbSkill(int skillId);
	void RestoreBuff(CCreature *pCreature, int nAbnormalType);
	void StoreBuff(CCreature *pCreature, CSkillInfo *pInfo);
	bool HandleHerb(CCreature *pCreature, CItem *pItem);
	void DropHerb(CCreature *pCreature, CCreature *pEnemy);
	inline BOOL IsAffectSummon() { return m_AffectSummon; };
};

extern CHerbSystem g_HerbSystem;