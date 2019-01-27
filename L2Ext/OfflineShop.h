#pragma once

class COfflineShop
{
	BOOL m_Enabled;
	BOOL m_AutoActivate;
	BOOL m_StayInGame;
	BOOL m_ConsumeRequiredItem;
	int m_RequiredItemId;
	int m_RequiredItemCount;
	UINT m_ShopTimeLimit;
	int m_IpMacLimit;
	int m_RequiredItemPriceId;
	int m_RequiredItemPriceCount;
public:
	static const int OFFLINE_SHOP_ABNORMAL = 180;	//not used
	static const int OFFLINE_SHOP_USER_COMMAND = 114;
	void Initialize();
	void RequestActivateOfflineShop(User *pUser);
	void RequestBuyRequiredItem(User *pUser);
	bool IsOfflineShop(CUserSocket *pSocket);
	void TimerExpired(User *pUser);
	void AutoActivate(User *pUser);
	static int AuthKick(User *pUser);
	static void BuilderCmdKick(CUserSocket *pSocket);
	static void KickCharPacket(CUserSocket *pSocket);
	static bool ManageCloseTimer(CIOObject *pIOObject, int nTimeSpan, int nTimerID);
	static void AddToStatusWindow(LPVOID pWindow, int nLine, PWCHAR format, int nCount);
	static bool PrivateMessageA(User *pUser, int nSenderDBID);
	static void PrivateMessageB(CUserSocket *pSocket, int nMsgID, PWCHAR wsName);
};

extern COfflineShop g_OfflineShop;