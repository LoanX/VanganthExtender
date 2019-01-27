#pragma once

struct AuctionItem
{
	UINT sellerDatabaseId;
	WCHAR sellerName[25];
	int itemClassId;
	int itemEnchant;
	Augmentation itemAugmentation;
	int itemAmount;
	int itemPrice;
	UINT auctionTimeoutTime;
};

class CAuction
{
	BOOL m_Enabled;
	CSLock m_Lock;
	vector<int> m_ForbiddenItems;
	map<UINT, AuctionItem> m_Items;
	map<UINT, wstring> m_Categories;
	wstring m_CategoryList;
	INT32 m_AuctionTime;
	INT32 m_AuctionFee;
	INT32 m_AuctionLimit;
	vector<wstring> m_Pages;
	BOOL m_UpdatePageRequest;
	UINT m_ExpireTimestamp;

	void LoadINI();
	UINT GetCategoryByName(wstring name);
	void UpdatePages();
public:
	CAuction();
	void Init();
	void TimerExpired();
	~CAuction();
	bool RequestShowAuction(User *pUser, wstring wCategory, UINT page = 0);
	bool RequestCancelAuction(User *pUser);
	bool RequestSelectAuctionItem(User *pUser);
	bool HandleSelectedItem(User *pUser, UINT itemServerId);
	bool RequestSetAuction(User *pUser, INT32 price, INT32 amount);
	bool Create(UINT sellerId, const WCHAR* sellerName, UINT auctionId, INT32 itemId, INT32 amount, INT32 enchant, UINT augmentation, INT32 price, UINT expireTime, bool loadOnly = false);
	bool RequestSetBuyItem(User *pUser, UINT auctionId);
	bool RequestConfirmBuy(User *pUser, UINT auctionId, INT32 amount);
	bool RequestBuy(User *pUser, UINT auctionId, INT32 amount);
	void RequestShowAuctionConfirmation(User *pUser, AuctionItem& ai);
	void HandlePaymentRequest(User *pUser, UINT jobId, INT32 priceId, INT32 priceAmount, INT32 itemId, INT32 amount, const WCHAR* buyer);
	inline BOOL IsEnabled() { return m_Enabled; };
	void OnEnterWorld(User *pUser);
};

extern CAuction g_Auction;