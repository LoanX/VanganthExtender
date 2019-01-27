#pragma once

class CItemDBEx;

class CItemDataEx
{
	int item_id;
	int shadow_time;
	int reuse_group;
	friend class CItemDBEx;
public:
	CItemDataEx() : item_id(0), shadow_time(0), reuse_group(0) { }
	__forceinline void Set(int ItemID, int ShadowTime, int ReuseGroup) { item_id = ItemID; shadow_time = ShadowTime; reuse_group = ReuseGroup; }
	__forceinline int GetItemID() { return item_id; };
	__forceinline int GetShadowTime() { return shadow_time;}
	__forceinline int GetReuseGroup() { return reuse_group; }
};

struct ItemExSkillList
{
	vector<CSkillKey> m_Skills;
	INT32 m_maxStackableLevel;
	INT32 m_stackType;
	CItemDataEx* *pExData;
};

class CItemDBEx : private CThreadSafe
{
	map<int, CItemDataEx*> mData;
	map<INT32, wstring> m_ItemName;
	map<INT32, wstring> m_ItemSA;
	map<INT32, wstring> m_ItemIcon;
	map<INT32, UINT> m_LifeTime;
public:
	CItemDBEx();
	~CItemDBEx();
	void Init();
	void LoadItemName();
	void LoadItemGrp();
	wstring GetItemName(int itemId);
	wstring GetItemSA(int itemId);
	wstring GetItemIcon(int itemId);
	void ReadData();
	void Clear();
	CItemDataEx* GetItemDataEx(int item_id);
	UINT GetLifeTime(int itemId);
};

extern CItemDBEx g_ItemDBEx;