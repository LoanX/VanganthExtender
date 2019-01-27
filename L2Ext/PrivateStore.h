#pragma once


struct StoreItemDef
{
	int itemId;
	int minPrice;
	int maxPrice;
	int maxAmount;
};

struct StoreRecipeDef
{
	int recipeId;
	int minPrice;
	int maxPrice;
};

class CStoreItemData
{
public:
	CStoreItemData() : nClassID(0), nObjectSID(0), nPrice(0), nCount(0), nEnchant(0) {};
	~CStoreItemData() {};
	int nClassID;
	int nObjectSID;
	int nPrice;
	int nCount;
	int nEnchant;
};

class CPrivateStoreSystem
{
	static const int m_CheckTime = 30; // 0,5 minute
	INT32 m_MinRequiredLevel;
	double m_MinDistance;
	map<int, StoreItemDef> m_StoreItems;
	map<int, StoreRecipeDef> m_StoreRecipes;
	bool m_StoreInDB;

	void LoadINI();
public:
	void Init();
	inline bool StoreInDB() { return m_StoreInDB; };
	inline INT32 GetMinRequiredLevel() { return m_MinRequiredLevel; };
	inline double GetMinDistance() { return m_MinDistance; };
	bool IsValidStoreItem(int itemId, int price, int amount, UINT& error);
	bool IsValidStoreRecipe(int recipeId, int price);
	void OnCreateStore(User *pUser);
	void TimerExpired(User *pUser);
	void OnDeleteStore(User *pUser);
	bool GotEnoughAdenaToBuy(User *pUser);
};

extern CPrivateStoreSystem g_PrivateStoreSystem;