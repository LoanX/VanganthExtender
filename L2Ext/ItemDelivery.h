#pragma once

class CItemDelivery
{
	BOOL m_enabled;
	UINT m_checkInterval; //inverval in seconds

	void LoadINI();
public:
	CItemDelivery();
	~CItemDelivery();
	void Init();
	void OnTimerExpired(User *pUser);
	void HandleRequest(User *pUser, UINT jobId, INT32 itemId, INT32 amount, INT32 enchant);
};

extern CItemDelivery g_ItemDelivery;