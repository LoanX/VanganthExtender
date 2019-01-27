#pragma once

struct MasterSkillInfo
{
	int skillId;
	int level;
	int maxLevel;
	ItemReq price;
	int priceSP;
	int requiredLevel;
	MasterSkillInfo() : skillId(0), level(0), maxLevel(0), requiredLevel(0), priceSP(0) {};
};

struct MasterSkillList
{
	UINT listId;
	map<int, vector<MasterSkillInfo>> m_Skills;
	vector<SkillPchId> m_PchId;
	MasterSkillList() : listId(0) {};
};

class CSkillMaster
{
	bool m_Enabled;
	vector<MasterSkillList*> m_Lists;
	INT32 m_SkillCount;
	void LoadData();
	bool IsAvailable(int skillId, int level, UINT listId);
	INT32 GetLowerLevel(int skillId, int baseLevel, UINT listId);
	MasterSkillInfo& GetSkillInfo(int skillId, int level, UINT listId);
	MasterSkillList* GetSkillList(UINT listId);
public:
	static const UINT m_AcquireSkillMode = 1;
	void Init();
	void ShowSkillList(User *pUser, UINT listId);
	void ShowSkillInfo(User *pUser, int skillId, int level, UINT listId);
	void AcquireSkill(User *pUser, int skillId, int level, UINT listId);
};

extern CSkillMaster g_SkillMaster;