#pragma once

struct VoteRewardItem
{
	int itemId;
	int amount;
	VoteRewardItem() : itemId(0), amount(0) {}
};

struct VoteJob
{
	UINT userId;
	string accountName;
};

class CVoteReward
{
	BOOL m_Enabled;
	vector<VoteRewardItem> m_Rewards;
	int m_RequiredPoints;
	string m_HttpRequestUrl;
	int m_PendingJobLimit;
	string m_SecretKey;
	int m_ServerId;

	CSLock m_Lock;
	vector<VoteJob> m_PendingJobs;

	HANDLE m_ThreadHandle;
	bool m_TerminateThread;
public:
	CVoteReward();
	~CVoteReward();
	inline BOOL IsEnabled() { return m_Enabled; };
	inline string GetSecretKey() { return m_SecretKey; };
	inline int GetServerId() { return m_ServerId; };
	void Init();
	void LoadData();
	void OnLoad(User *pUser);
	void HandleCommand(User *pUser, wstring wParams);
	void Exchange(User *pUser);
	static void JobThread(LPVOID params);
};

extern CVoteReward g_VoteReward;