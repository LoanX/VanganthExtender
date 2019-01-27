#pragma once

struct MultiSellInfo
{
	UINT listId;
	UINT entryId;
	INT32 count;
};

class CMultiSellStat
{
	BOOL m_enabled;
	void LoadINI();
public:
	CMultiSellStat();
	~CMultiSellStat();
	void Init();
	void SetInfo(UINT listId=0, UINT entryId=0, UINT count=0);
	inline BOOL IsEnabled() { return m_enabled; };
	static void OnCreateUpdateItems(LPVOID lpInstance, User *pUser, int n, int buffSize, PCHAR lpBuff, LPVOID lpAtomicJob);
};

extern CMultiSellStat g_MultiSellStat;