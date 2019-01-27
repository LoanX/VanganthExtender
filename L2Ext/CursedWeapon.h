#pragma once


class CCursedWeapon
{
	enum Pos
	{
		X = 0,
		Y = 1,
		Z = 2
	};
	list<INT> l_PSkills;
	list<INT> l_ASkills;
	CRITICAL_SECTION lock;
	INT WeaponID;
	time_t DropTime; //Decreases each time owner kill pc
	time_t ExpireTime; //Const
	INT OwnerDBID;
	UINT OwnerSID;
	INT KillCount;
	double m_DropRate;
	INT Location[3];
public:
	enum SystemMessage
	{
		$S1_HAS_$S2_HOURS_OF_USAGE_TIME_REMAINING = 1813,
		$S1_HAS_$S2_MINUTES_OF_USAGE_TIME_REMAINING = 1814,
		$S2_WAS_DROPPED_IN_THE_$S1_REGION = 1815,
		THE_OWNER_OF_$S2_HAS_APPEARED_IN_THE_$S1_REGION = 1816,
		$S2_S_OWNER_HAS_LOGGED_INTO_THE_$S1_REGION = 1817,
		$S1_HAS_DISAPPEARED = 1818,
		AN_EVIL_IS_PULSATING_FROM_$S2_IN_$S1 = 1819,
		$S1_IS_CURRENTLY_ASLEEP = 1820,
		$S2_S_EVIL_PRESENCE_IS_FELT_IN_$S1 = 1821,
		$S1_HAS_BEEN_SEALED = 1822,
		SHOUT_AND_TRADE_CHATTING_CANNOT_BE_USED_WHILE_POSSESSING_A_CURSED_WEAPON = 2085,
		YOU_CANNOT_MOUNT_A_STEED_WHILE_A_CURSED_WEAPON_IS_EQUIPPED = 2255
	};
	CCursedWeapon();
	~CCursedWeapon();
	inline void Lock(){ EnterCriticalSection(&lock); };
	inline void Unlock() { LeaveCriticalSection(&lock); };
	int GetLevel();
	double GetDropRate();
	void Init(int nWeaponID);
	void SetLocation(int x, int y, int z);
	void GetXYZ(LPINT pX, LPINT pY, LPINT pZ);
	void OnAcquire(User *pUser, CItem *pItem);
	void OnDrop(User *pUser);
	void OnDelete(User *pUser);
	void OnEnterWorld(User *pUser);
	void OnLeaveWorld(User *pUser);
	void OnTeleport(User *pUser);
	void OnChangeSubjob(User *pUser);
	void UpdateKillCount(User *pUser, int nKillCount);
	void Delete();
	void TimerExpired();
	void Save();
	bool HasOwner();
	UINT GetOwnerSID();
	bool NotExistsInTheWorld();
	void SendTimeLeft(User *pUser);
	bool IsOwner(User *pUser);
	bool IsOnline();
	void BroadcastSpecialEffect();
	void NpcDrop(CNPC *pNPC);
	void Reset(User *pUser);
	void SendInfo(User *pUser);
};
