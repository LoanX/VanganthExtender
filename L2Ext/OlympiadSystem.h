#pragma once

extern "C"
{
	void PointFix1();
	void PointFix2();
};

struct OlympiadPeriod
{
	UINT startHour;
	UINT endHour;
};

class COlympiadStatus
{
public:
	INT32 nVar1; //1 - End Competition
	bool EnableMatches; // 0 - End Competition
};

class COlympiadEntry;

class COlympiadSystem
{
public:
	static void Initialize();
	static void LoadINI();
	static void OlympiadGameEndsInTie(COlympiadField *pOlympiadField, const char *format, BYTE bOpCode, int nMsgID, int nParam);
	static void OlympiadFieldOverweightFix(CUserSocket *pSocket, int nMsgID);
	static void OlympiadFieldOnGameExit(User *pUser);
	static void OlympiadFieldReloginFix2(CUserSocket *pSocket, int nMsgID);
	static void SaveOlympiadRecord(int nWinnerDBID, int nLoserDBID, int nWinnerPoints, int nLoserPoints);
	static bool GetWantedStatus(LPVOID pOlympiadSys, DWORD nSeasonStartTime, COlympiadStatus *pOS);
	static void RequestOlympiadStat(CUserSocket *pSocket, const char *format, ...);
	static void ValidateEquippedItems(User *pUser);
	static bool COlympiadObserverEntry_AddEntry(LPVOID pInstance, int nUserSID);
	static void COlympiadField_AsyncEscapeObserver(LPVOID pInstance, int nUserSID);
	static void AsyncAddEntry(COlympiadField *pField, BYTE byGameRule, UINT playerId1, UINT playerId2);
	static bool IsFightEnabled(bool classFree);
	static double ReturnDouble1();
	static void OnSaveOlympiadRecord(LPVOID lpInstance, UINT winnerId, UINT looserId, int winnerPoint, int looserPoint);
};

struct OlympiadBuffSupport
{
	vector<UINT> classI;
	vector<UINT> classII;
	vector<SkillPchId> buffsI;
	vector<SkillPchId> buffsII;
};

class COlympiadDB
{
	CSLock m_Lock;
	vector<OlympiadBuffSupport> m_BuffSupport;
	map<UINT, int> m_RegisteredUser; //UserId, ClassType
	vector<SkillPchId> m_ForbiddenSkill;
	vector<int> m_ForbiddenItem;
	void LoadForbiddenSkill();
	void LoadForbiddenItem();
	void LoadBuffSupport();
public:
	COlympiadDB();
	~COlympiadDB();
	void Init();
	bool IsForbiddenSkill(int skillId, int level);
	bool IsForbiddenItem(int itemId);
	vector<SkillPchId>& GetBuffSupport(UINT baseClass, UINT enemyClass);
};

extern COlympiadDB g_OlympiadDB;