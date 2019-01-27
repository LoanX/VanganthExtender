#pragma once

struct AIOSkill
{
	CSkillKey skill;
	UINT abnormalTime;
	BOOL partyBuff;
};

struct AIOData
{
	UINT id;
	BOOL enabled;
	INT32 priceId;
	INT32 priceAmount;
	INT32 rewardId;
	INT32 minLevel;
	UINT nicknameColor;
	UINT expireTime;
	vector<AIOSkill> skills;
};

class CAIOSystem
{
	BOOL m_enabled;
	BOOL m_canUseOutsidePeaceZone;
	BOOL m_canMoveOutsidePeacezone;
	IVector m_peaceZoneLoc;
	map<UINT, AIOData> m_data;
	void AcquireSkills(User *pUser);
	void DeleteSkills(User *pUser);
	void LoadData();
public:
	CAIOSystem();
	virtual ~CAIOSystem();
	void Init();
	inline BOOL IsEnabled() { return m_enabled; };
	bool CanUse(User *pUser, int skillId);
	void BecomeAIO(User *pUser, UINT id, bool byGM = false);
	void RemoveAIO(User *pUser);
	void TimerExpired(User *pUser);
	DWORD GetNicknameColor(UINT id);
	void OnLoad(User *pUser, UINT id, UINT timestamp);
	INT32 GetAbnormalTime(User *pUser, int skillId, int level);
	bool IsPartyBuff(UINT id, int skillId, int level);
};

extern CAIOSystem g_AIOSystem;