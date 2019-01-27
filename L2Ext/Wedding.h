#pragma once

class CWedding
{
	BOOL m_Enabled;
	vector<ItemReq> m_Price;
	vector<ItemReq> m_Reward;
	UINT m_Penalty;
	UINT m_TeleportPenalty;
	INT32 m_MinLevel;
	INT32 m_MaxLevel;
	BOOL m_AllowHomo;
	BOOL m_RequireFormalWear;
	wstring m_NormalAnnounce;
	wstring m_HomoXAnnounce;
	wstring m_HomoYAnnounce;
	wstring m_WeddingSound;
	void LoadINI();
public:
	void Init();
	bool CanBeMarried(User *pUser, bool sendMessage = true);
	bool Engage(User *pUser, const WCHAR* wTarget);
	void EngageReply(User *pUser, UINT answer);
	bool Marry(User *pUser);
	void MarryReply(User *pUser, UINT answer);
	void OnLoad(User *pUser);
	bool Divorce(User *pUser);
	void DivorceReply(User *pUser, UINT answer);
	void GoToLove(User *pUser);
};

extern CWedding g_Wedding;