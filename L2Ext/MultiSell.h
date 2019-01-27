#pragma once
#include "Utils.h"

class CMultiSellItem
{
	int ItemID;
	int Count;
	friend class CMultiSellEntry;
	friend class CMultiSell;
	friend class CMultiSellDB;
public:
	CMultiSellItem(int nItemID, int nCount) : ItemID(nItemID), Count(nCount) {};
	CMultiSellItem() : ItemID(0), Count(0) {};
	inline int GetID() { return ItemID; };
	inline int GetCount() { return Count; };
	inline void SetID(int ID) { ItemID = ID; };
	inline void SetCount(int nCount) { Count = nCount; };
};

class CMultiSellEntry
{
	vector<CMultiSellItem> vReward;
	vector<CMultiSellItem> vPrice;
	enum PARSE_STATE
	{
		MS_BEGIN,
		MS_REWARD,
		MS_REWARD_NAME,
		MS_REWARD_COUNT,
		MS_PRICE,
		MS_PRICE_NAME,
		MS_PRICE_COUNT,
		MS_END
	};
	friend class CMultiSell;
	friend class CMultiSellDB;
public:
	CMultiSellEntry(wstring wLine);
	size_t GetPriceItemCount();
	size_t GetRewardItemCount();
};

class CMultiSell
{
	int ID;
	bool is_dutyfree;
	bool is_show_all;
	bool keep_enchanted;
	wstring wName;
	vector<CMultiSellEntry> vEntry;
	friend class CMultiSellDB;
public:
	CMultiSell();
	void Begin(wstring wLine);
	int GetID();
	size_t GetEntryCount();
	void Clear();
	void SetOption(wstring wLine);
	void AddEntry(wstring wLine);
};

class CMultiSellDB
{
	map<int, CMultiSell> mData;
public:
	void Init();
	void ReadData();
	int ConvertEntry(UINT nEntryID, int nItemCount);
	bool Validate(User *pUser, int nListID, int nItemIndex, int nAmount);
	bool GetPriceAndReward(UINT listId, UINT entryId, INT32 count, LPINT lpPriceId, LPINT lpPriceCount, LPINT lpRewardId, LPINT lpRewardCount);
	static void MultiSellChoose(User *pUser, UINT groupId, int itemChooseId, int itemAmount);
	static void SendList(CUserSocket *pSocket, const char* format, BYTE opCode, UINT groupId, UINT page, UINT finished, UINT totalPages, UINT groupSize, DWORD buffLen, LPBYTE lpBuff);
};

extern CMultiSellDB g_MultiSellDB;