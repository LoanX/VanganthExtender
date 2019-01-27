#pragma once

struct VoteReuse
{
	HWID hardwareId;
	time_t timeout;
};

class CL2NetworkVote
{
	bool m_Enabled;
	bool m_EnableCommand;
	bool m_CheckOnEnterWorld;
	bool m_HardwareIdCheck;
	UINT m_IdentityType;
	ItemReq m_VoteReward;
	UINT m_VoteTimeout;
	CHAR m_ApiKey[33];
	CHAR m_Host[256];
	CHAR m_Path[256];
	WCHAR m_TemplateLink[260];
	vector<VoteReuse> m_HWIDReuse;
	CSLock m_Lock;
	void LoadINI();
public:
	void Init();
	inline bool Enabled() { return m_Enabled; };
	inline ItemReq GetReward() { return m_VoteReward; };
	inline bool EnableCommand() { return m_EnableCommand; };
	inline bool CheckOnEnterWorld() { return m_CheckOnEnterWorld; };
	inline UINT GetIdentityType() { return m_IdentityType; };
	inline UINT VoteTimeout() { return m_VoteTimeout; };
	void OnCheck(User *pUser);
	void OnShowVote(User *pUser);
	void OnClientReply(User *pUser, INT32 lastVoteTime);
};

extern CL2NetworkVote g_L2NetworkVote;