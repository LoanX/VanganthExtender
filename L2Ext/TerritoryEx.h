#pragma once


class CNickNameChangeTerritory;
class CBlockItemsTerritory;
class CBlockSkillsTerritory;
class CRaceChangeTerritory;
class CBlockChatTerritory;
class CPvPRewardTerritory;
class CDespawnSummonTerritory;
class CDispelSkillTerritory;
class CMiningTerritory;
class CLevelRestrictionTerritory;
class CPartyTerritory;
class CDispelAllTerritory;
class CNoRestartTerritory;
class CRequiredItemTerritory;
class CInstanceTerritory;
class CRespawnTerritory;
class CNoblessTerritory;

class CTerritoryEx
{
public:
	CNickNameChangeTerritory* SafeCastNickNameChange();
	CBlockItemsTerritory* SafeCastBlockItems();
	CBlockSkillsTerritory* SafeCastBlockSkills();
	CRaceChangeTerritory* SafeCastRaceChange();
	CBlockChatTerritory* SafeCastBlockChat();
	CPvPRewardTerritory* SafeCastPvPReward();
	CDespawnSummonTerritory* SafeCastDespawnSummon();
	CDispelSkillTerritory* SafeCastDispelSkill();
	CMiningTerritory* SafeCastMining();
	CLevelRestrictionTerritory* SafeCastLevelRestriction();
	CPartyTerritory *SafeCastParty();
	CDispelAllTerritory *SafeCastDispelAll();
	CNoRestartTerritory *SafeCastNoRestart();
	CRequiredItemTerritory *SafeCastRequiredItem();
	CInstanceTerritory *SafeCastInstance();
	CRespawnTerritory *SafeCastRespawn();
	CNoblessTerritory *SafeCastNobless();
private:
	FVector m_MinPos;
	FVector m_MaxPos;
	TerritoryType m_Type;
	bool m_Extend;
	vector<FVector> m_Points;

	bool CrossHalfLine(FVector point, FVector p1, FVector p2);
public:
	CTerritoryEx();
	virtual ~CTerritoryEx() {};
	void Set(double x, double X, double y, double Y, double z, double Z, TerritoryType type);
	bool IsInside(double x, double y, double z);
	bool IsInside(FVector point);
	void AddPoint(FVector point);
	void AddZRange(double z1, double z2);
	inline void SetType(TerritoryType value) { m_Type = value; };
	inline TerritoryType GetType() { return m_Type; };
	static TerritoryType CastType(int nType);
};

class CRespawnTerritory : public CTerritoryEx
{
	vector<IVector> m_Respawns;
	INT32 m_HpRatio;
	INT32 m_CpRatio;
	INT32 m_MpRatio;
	bool m_ProtectBuffs;
public:
	CRespawnTerritory() : m_HpRatio(0), m_CpRatio(0), m_MpRatio(0), m_ProtectBuffs(false) {};
	void AddRespawn(int x, int y, int z);
	IVector GetRandomRespawn();
	inline INT32 GetHpRatio() { return m_HpRatio; };
	inline void SetHpRatio(INT32 value) { m_HpRatio = value; };
	inline INT32 GetCpRatio() { return m_CpRatio; };
	inline void SetCpRatio(INT32 value) { m_CpRatio = value; };
	inline INT32 GetMpRatio() { return m_MpRatio; };
	inline void SetMpRatio(INT32 value) { m_MpRatio = value; };
	inline bool ProtectBuffs() { return m_ProtectBuffs; };
	inline void SetProtectBuffs(bool value) { m_ProtectBuffs = value; };
};

class CNoblessTerritory : public CTerritoryEx
{
	IVector m_KickOutPos;
public:
	CNoblessTerritory() { };
	inline void SetKickOutPos(int x, int y, int z) { m_KickOutPos.x = x; m_KickOutPos.y = y; m_KickOutPos.z = z; }
	inline IVector GetKickOutPos() { return m_KickOutPos; }
};

class CInstanceTerritory : public CTerritoryEx
{
	UINT m_InstanceId;
	UINT m_AllowedTime;
	UINT m_ReuseTime;
	IVector m_KickOutPos;
	BOOL m_ByHWID;
public:
	CInstanceTerritory() {};
	inline void SetByHWID(BOOL value) { m_ByHWID = value; };
	inline BOOL IsByHWID() { return m_ByHWID; };
	void SetKickOutPos(int x, int y, int z) { m_KickOutPos.x = x; m_KickOutPos.y = y; m_KickOutPos.z = z; };
	inline IVector GetKickOutPos() { return m_KickOutPos; };
	inline void SetInstanceId(UINT id) { m_InstanceId = id; };
	inline UINT GetInstanceId() { return m_InstanceId; };
	inline void SetAllowedTime(UINT value) { m_AllowedTime = value; };
	inline UINT GetAllowedTime() { return m_AllowedTime; };
	inline void SetReuseTime(UINT value) { m_ReuseTime = value; };
	inline UINT GetReuseTime() { return m_ReuseTime; };	
};


class CRequiredItemTerritory : public CTerritoryEx
{
	vector<int> m_Items;
	IVector m_KickOutPos;
public:
	CRequiredItemTerritory(){};
	void AddItem(int itemId) { m_Items.push_back(itemId); };
	void SetKickOutPos(int x, int y, int z) { m_KickOutPos.x = x; m_KickOutPos.y = y; m_KickOutPos.z = z; };
	bool HasItem(User *pUser);
	inline IVector GetKickOutPos() { return m_KickOutPos; };
};

class CNoRestartTerritory : public CTerritoryEx
{
	IVector m_KickOutLoc;
public:
	void SetKickOutLocation(int x, int y, int z) { m_KickOutLoc.x = x; m_KickOutLoc.y = y; m_KickOutLoc.z = z; };
	IVector GetKickOutLocation() { return m_KickOutLoc; };
};


class CDispelAllTerritory : public CTerritoryEx
{
	BOOL onEnterOnly;
public:
	CDispelAllTerritory() : onEnterOnly(FALSE) {};
	void SetOnEnterOnly(BOOL value) { onEnterOnly = value; };
	inline BOOL IsOnEnterOnly() { return onEnterOnly; };
};

class CPartyTerritory : public CTerritoryEx
{
	FVector kickOutPos;
	int minMembers;
	int maxMembers;
public:
	CPartyTerritory() : minMembers(2), maxMembers(9) {};
	void SetMinMembers(int value) { minMembers = value; };
	void SetMaxMembers(int value) { maxMembers = value; };
	void SetKickOutPos(FVector pos) { kickOutPos = pos; };
	inline FVector GetKickOutPos() { return kickOutPos; };
	bool CanEnter(int members) { if(members >= minMembers && members <= maxMembers) return true; return false; };
};

class CLevelRestrictionTerritory : public CTerritoryEx
{
	int minLevel;
	int maxLevel;
	FVector kickOutPos;
public:
	CLevelRestrictionTerritory() : minLevel(0), maxLevel(0) {};
	void SetMinLevel(int value) { minLevel = value; };
	void SetMaxLevel(int value) { maxLevel = value; };
	void SetKickOutPos(FVector pos) { kickOutPos = pos; };
	inline FVector GetKickOutPos() { return kickOutPos; };
	bool CanEnter(int level) { if(level >= minLevel && level <= maxLevel) return true; return false; };
};

class CNickNameChangeTerritory : public CTerritoryEx
{
	wstring wNickName;
	DWORD Color;
	bool HidePledge;
public:
	CNickNameChangeTerritory() : Color(0) {};
	void SetNickName(wstring wName) { wNickName = wName; };
	void SetColor(DWORD Value) { Color = Value; };
	void SetHidePledge(bool Val) { HidePledge = Val; };
	DWORD GetColor() { return Color; };
	bool IsHidePledge() { return HidePledge; };
	const wchar_t* GetNickName() { if(wNickName.size() > 0) return wNickName.c_str(); return NULL; };
};

class CBlockItemsTerritory : public CTerritoryEx
{
	vector<int> m_Items;
public:
	bool IsBlocked(int itemId)
	{
		for(UINT n=0;n<m_Items.size();n++)
		{
			if(m_Items[n] == itemId)
			{
				return true;
			}
		}
		return false;
	}
	void AddItem(int itemId)
	{
		m_Items.push_back(itemId);
	}
};

class CBlockSkillsTerritory : public CTerritoryEx
{
	list<int> lSkills;
public:
	bool IsBlocked(int skill_id)
	{
		for(list<int>::iterator Iter = lSkills.begin();Iter!=lSkills.end();Iter++)
		{
			if((*Iter) == skill_id)
				return true;
		}
		return false;
	}
	void AddSkill(int skill_id)
	{
		lSkills.push_back(skill_id);
	}
};

class CRaceChangeTerritory : public CTerritoryEx
{
	UINT m_Race;
	UINT m_Sex;
public:
	CRaceChangeTerritory() : m_Race(UINT_MAX), m_Sex(UINT_MAX){};
	void SetData(UINT race, UINT sex)
	{
		m_Race = race;
		m_Sex = sex;
	};
	UINT GetRace() { return m_Race; };
	UINT GetSex() { return m_Sex; };
};

class CBlockChatTerritory : public CTerritoryEx
{
	bool data[10];
public:
	enum TYPE
	{
		ALL,
		SHOUT,
		TELL,
		PARTY,
		CLAN,
		TRADE,
		ALLIANCE,
		HERO,
		MSN
	};
	CBlockChatTerritory();
	void SetData(bool value, TYPE type);
	bool IsSet(TYPE type);
};

class CPvPRewardTerritory : public CTerritoryEx
{
	INT32 requiredLevel;
	INT32 rewardId;
	INT32 rewardCount;
	BOOL stealFromVictim;
	BOOL checkHWID;
	INT32 killReuse;
public:
	CPvPRewardTerritory();
	inline void SetRewardId(INT32 value) { rewardId = value; };
	inline INT32 GetRewardId() { return rewardId; };
	inline void SetRewardCount(INT32 value) { rewardCount = value; };
	inline INT32 GetRewardCount() { return rewardCount; };
	inline void SetRequiredLevel(INT32 value) { requiredLevel = value; };
	inline INT32 GetRequiredLevel() { return requiredLevel; };
	inline void SetStealFromVictim(BOOL value) { stealFromVictim = value; };
	inline BOOL IsStealFromVictim() { return stealFromVictim; };
	inline BOOL IsCheckingHWID() { return checkHWID; };
	inline void SetCheckingHWID(BOOL value) { checkHWID = value; };
	inline void SetKillReuse(int value) { killReuse = value; };
	inline int GetKillReuse() { return killReuse; };
	void Handle(User *pUser, User *pAttacker);
};

class CDespawnSummonTerritory : public CTerritoryEx
{
	vector<INT32> npcs;
public:
	bool IsBlocked(INT32 classId);
	void Block(INT32 classId);
};

class CDispelSkillTerritory : public CTerritoryEx
{
	vector<CSkillKey> skills;
public:
	bool IsBlocked(INT32 skillId, INT32 level);
	void Block(INT32 skillId, INT32 level);
	void Validate(CCreature *pCreature);
};

class CMiningTerritory : public CTerritoryEx
{
	UINT miningId;
public:
	inline UINT GetMiningId() { return miningId; };
	inline void SetMiningId(UINT id) { miningId = id; };
};