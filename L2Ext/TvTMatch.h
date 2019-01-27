#ifndef TVTMATCH_H
#define TVTMATCH_H

namespace TvT
{
	enum TvTType
	{
		TvTNone = 0,
		TvTLastTeamStanding = 1,
		TvTDeathMatch = 2
	};

	enum TvTState
	{
		StateNone = 0,
		StateRegistration = 1,
		StatePreparation = 2,
		StateCountDown = 3,
		StateFight = 4,
		StateFinish = 5,
		StateDelete = 6
	};

	struct TvTInfo
	{
		BOOL enabled;
		TvTType type;
		bool dayFlag[7];
		INT32 startHour;
		INT32 startMinute;
		INT32 registerTime;
		INT32 prepareTime;
		INT32 countDownTime;
		INT32 fightTime;
		INT32 finishTime;
		BOOL dispelAbnormal;
		IVector bluePos;
		IVector redPos;
		BOOL randomizeTeam;
		INT32 requiredItemId;
		INT32 requiredItemCount;
		INT32 minPlayers;
		INT32 maxPlayers;
		INT32 minLevel;
		INT32 maxLevel;
		INT32 respawnTime;
		INT32 skillPchId;
		INT32 rewardId;
		INT32 rewardCount;
		IVector kickOutPos;
		INT32 registerNpcClassId;
		IVector registerNpcPos;
		INT32 fightZoneX[2];
		INT32 fightZoneY[2];
		list<wstring> doorList;
		wstring registrationStartMsg1;
		wstring registrationStartMsg2;
		wstring registrationRemindMsg;
		wstring registrationEndMsg;
		wstring registrationNotEnoughParticipantsMsg;
		wstring preparationStartMsg;
		wstring preparationRemindMsg;
		wstring fightStartRemindMsg;
		wstring fightStartMessage;
		wstring fightEndRemindMsg;
		wstring endMsg[3];
		wstring teleportBackRemindMsg;
		wstring endEventMsg;
		vector<INT32> blockedSkill;
		vector<INT32> blockedItem;
		vector<SkillPchId> defaultMageBuffs;
		vector<SkillPchId> defaultFighterBuffs;
		bool blockedCrystalType[10];
		BOOL blockAugmentation;
		BOOL blockAttribute;
		INT32 maxArmorEnchant;
		INT32 maxWeaponEnchant;
		BOOL friendlyFire;
		BOOL checkHwid;
	};

	class CMatch
	{
		friend class CTvT;
	protected:
		CRITICAL_SECTION m_lock;
	private:
		inline void Lock() { EnterCriticalSection(&m_lock); }
		inline void Unlock() { LeaveCriticalSection(&m_lock); }

		TvTInfo* m_lpInfo;
		map<UINT, User*> m_users;
		TvTState m_state;
		time_t m_startTime;
		time_t m_stateTime;
		UINT m_managerServerId;
		TeamType m_winnerTeam;
		INT32 m_killCountBlue;
		INT32 m_killCountRed;

	public:
		CMatch(TvT::TvTInfo *pInfo);
		void Init();
		~CMatch();
		void OnExit(User *pUser);
		void OnFinish();
		void OnDelete();
		bool ValidateWinner(bool timeout = false);
		void Broadcast(const char* format, ...);
		void RandomizeTeams();
	};
};

#endif