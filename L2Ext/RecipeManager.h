#pragma once

class CRecipeManager;

class CRecipeBonusInfo
{
	typedef struct ItemRate	{ ItemReq item; INT32 rate;	};
	INT32 recipeId;
	INT32 baseItemRate;
	vector<ItemRate> items;
	INT32 systemMessage;
	friend class CRecipeManager;
public:
	CRecipeBonusInfo() : recipeId(0), baseItemRate(0), systemMessage(0) {}
	CRecipeBonusInfo(INT32 _recipeId, INT32 _rate, INT32 _multipler, INT32 _systemMessage)
		:recipeId(_recipeId),
		baseItemRate(_rate),
		systemMessage(_systemMessage) {}
};

class CRecipeManager
{
	bool m_Enabled;
	typedef map<INT32, CRecipeBonusInfo> BonusMap;
	BonusMap m_Bonus;
	CRecipeManager() : m_Enabled(false) {}
	CRecipeManager(const CRecipeManager& other) { CRecipeManager(); }
	void LoadData();
public:
	static CRecipeManager& Get();
	inline bool Enabled() { return m_Enabled; }
	void Init();
	bool OnCraft(User *pUser, INT32 recipeId, ItemReq& items, INT32& systemMessage) const;
};

