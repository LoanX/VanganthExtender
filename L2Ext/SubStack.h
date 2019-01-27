#pragma once

class CSubStack
{
	BOOL m_Enabled;
	ItemReq m_StackPrice;
	ItemReq m_StackPriceEx;
	ItemReq m_SwitchPrice;
	ItemReq m_RemovePrice;
	INT32 m_Delevel;
	INT32 m_RequiredLevel;
	BOOL m_Require3rdClassChange;

	vector<UINT> m_AvailableClasses;
	void LoadINI();
public:
	void Init();
	void ShowAvailable(User *pUser, bool ex = false);
	void RequestStack(User *pUser, UINT classId, bool ex = false);
	void RequestSwitch(User *pUser);
	void RequestRemove(User *pUser);
	inline BOOL IsEnabled() { return m_Enabled; };
};

extern CSubStack g_SubStack;