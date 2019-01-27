#pragma once

//begin	skill_name=[s_triple_slash11]	skill_id=1	soulshot_bonus=1.33	shield_block=0	magic_shield_block=0	cancel_resistance=0	end

class CSkillInfoEx
{
public:
	string name;
	int id;
	int level;
	double soulshotBonus;
	bool avoidShieldBlock;
	bool magicShieldBlock;
	bool directToHp;
	bool geoDataTest;
	int staticReuseTime;
	CSkillInfoEx() : id(0), level(0), geoDataTest(0), soulshotBonus(0), avoidShieldBlock(false), magicShieldBlock(false), directToHp(false), staticReuseTime(0) { }
};

class CSkillDBEx
{
	void ReadData();
	void ReadData2();
	map<int, map<int, wstring>> mData2; //<SkillID, <Level, Name>>
	map<CSkillKey, CSkillInfoEx*> mData;
public:
	~CSkillDBEx();
	void Initialize();
	int GetMaxSkillLevel(int SkillID);
	int GetMaxNotEnchantedSkillLevel(int SkillID);
	CSkillInfoEx *GetSkillInfoEx(int nSkillID, int nLevel);
};

extern CSkillDBEx g_SkillDBEx;