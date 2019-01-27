#pragma once

enum PRIVILEGE
{
	JOIN_CLAN = 0,
	GIVE_TITLE = 1,
	VIEW_WAREHOUSE = 2,
	CLAN_CREST_MANAGMENT = 3,
	CH_ENTRACE_AUTHORITY = 4,
	CH_OTHER_RIGHTS = 5,
	CH_DISMISS = 6,
	CT_OPEN_DOOR = 7,
	CT_OTHER_RIGHTS = 8,
	CT_DISMISS = 9,
	CLAN_WAR = 10,
	MANAGE_RANKS = 11,
	DISMISS_FROM_CLAN = 12,
	MASTER_RIGHTS = 13
};
enum PLEDGE_CLASS
{
	VAGABOND = 0,
	VASSAL = 1,
	HEIR = 2,
	KNIGHT = 3,
	ELDER = 4,
	BARON = 5,
	VISCOUNT = 6,
	COUNT = 7,
	MARQUIS = 8
};

class CPledgeSkillDb
{
	PledgeSkillInfo m_EmptySkill;
	map<int, vector<PledgeSkillInfo>> m_Skills;	//<pledge_level, skills>
	map<int, int> m_MaxSkillLevel;
	void LoadData();
public:
	CPledgeSkillDb();
	~CPledgeSkillDb();
	void Init();
	const PledgeSkillInfo& GetSkillInfo(int skillId, int level, bool showLog = true);
	void GetAcquirableSkills(int pledgeLevel, vector<PledgeSkillInfo>& acquirableSkills);
	void GetAcquirableSkills(int pledgeLevel, vector<PledgeSkillInfo>& acquirableSkills, map<int, PledgeSkillInfo>& acquiredSkills);
	void DeleteSkills(User *pUser);
	void AcqiureSkills(User *pUser);
	int GetMaxLevel(int skillId);

};

extern CPledgeSkillDb g_PledgeSkillDb;

//Used when reallocating info after receiving data from cached
struct ExtPledgeInfo
{
	WCHAR wszMasterName[50];
	int nReputation;
	DWORD PowerData[10];
	map<int, int> *m_Skills;
	SubUnitMap *m_SubUnits;
};

struct ExtMemberInfo
{
	WCHAR wszMemberName[50];
	int nRank;
	int nPledgeType;
	int nSponsorID;
	int nJoinLevel;
};
struct AtomicPledgeLevelUp
{
	/* 000 */ INT64 nUnkn;
	/* 008 */ INT64 nUnkn2;
	/* 010 */ INT64 nUnkn3;
	/* 018 */ UINT nUserSID;
	/* 01C */ int nPledgeID;
	/* 020 */ int nLevel;
};
extern "C"
{
	void AddMemberFix();
	void DelMemberFix();
}

class ClanSys
{
public:
	static void Initialize();
	static void LoadReputationData();
	static LPVOID CreateMemberData(int nSize);
	static const unsigned char* HandleExtPledgeInfo(const unsigned char* packet, const char *format, int nSize1, PWCHAR wszPledgeName, int *pPledgeID, int *pMasterID, int nSize2, ExtPledgeInfo **Info, int *pMemberAmount);
	static CPledge *HandleLoadedPledge(CPledge *pPledge, INT32 nPledgeID, INT32 nMasterID, PWCHAR wszPledgeName, ExtPledgeInfo** Info);
	static const unsigned char* HandleExtMemberInfo(const unsigned char* packet, const char *format, int *pDBID, int nSize, ExtMemberInfo **Info, int *pLevel, int *pClass, int *pGender, int *pRace);
	static void HandleLoadedMember(CPledge *pPledge, int nDBID, ExtMemberInfo** Info, int nLevel , int nClass, int nGander, int nRace, int nObjectID);
	static LPVOID HandleCreatedPledge(CPledge *pPledge, INT32 nPledgeID, INT32 nMasterID, PWCHAR wszPledgeName, PWCHAR wszMasterName);
	static void ReplyCreatePledge(CPledge *pPledge, User *pUser);
	static void UpdateMembers(CPledge *pPledge);
	static void OnNpcDie(CNPC* pNpc, CCreature *pEnemy);
	static LPVOID RemovePledge(CPledge *pPledge, DWORD dData);
	static INT64 C5ToC4Privilege(INT64 nPrivilege);
	static INT64 C4ToC5Privilege(DWORD dPrivilege);
	static bool CheckPrivilege(INT64 nPledgePower, int nPrivilege);
	static bool ManagePledgePower(CUserSocket *pSocket, const unsigned char* packet);
	static bool RequestSetMemberRank(CUserSocket *pSocket, const unsigned char* packet);
	static bool RequestMemberPowerInfo(CUserSocket *pSocket, const unsigned char* packet);
	static bool RequestMemberInfo(CUserSocket *pSocket, const unsigned char* packet);
	static bool RequestReorganizeMember(CUserSocket *pSocket, const unsigned char* packet);
	static bool RequestSetSponsor(CUserSocket *pSocket, const unsigned char* packet);
	static bool RequestWarInfo(CUserSocket *pSocket, const unsigned char* packet);
	static bool RequestPledgePowerGradeList(CUserSocket *pSocket, const unsigned char* packet);
	static bool RequestDelegateClanMaster(User *pUser, wstring wMasterName);
	static bool RequestDelegateClanMasterEx(User *pUser, wstring wMasterName);
	static bool RequestCancelDelegateClanMaster(User *pUser);
	static void EnterWorld(User *pUser);
	static void LeaveWorld(User *pUser);
	static CPledge *CPledge_Constructor(CPledge *pPledge, INT32 nPledgeID, INT32 nVal2, INT64 nVal3, INT64 nVal4);
	static LPVOID CPledge_Destructor(CPledge *pPledge, DWORD dData);
	static void CPledge_UpdateWindowPacket(CPledge *pPledge, User *pUser, bool bIsOnline);
	static void CPledge_OpenAllWindowPacket(CPledge *pPledge, User *pUser, bool bGM, PWCHAR wszActiveChar);
	static void CPledge_AddWindowPacket(CPledge *pPledge, User *pUser);
	static void CPledge_UpdateInfoWindowPacket(CPledge *pPledge);
	static void PledgeLevelUpByNPC(CNPC *pNPC, User *pUser, int nLevel);
	static bool AtomicPledgeLevelUp_Do(AtomicPledgeLevelUp *pJob); 
	static bool OnChangeClass(User *pUser, int nClass);
	static bool AskJoinPledgePacket(CUserSocket *pSocket, const unsigned char* packet);
	static bool AnswerJoinPledgePacket(CUserSocket *pSocket, const unsigned char* packet);
	static void CPledge_Join(CPledge *pPledge, User *pUser);
	static void CPledge_Withdraw(CPledge *pPledge, User *pUser);
	static void CPledge_Oust(CPledge *pPledge, int nUserDBID, PWCHAR wszName);
	static void CPledge_TakeOustPenalty(CPledge *pPledge);
	static void User_TakeOustedPenalty(User *pUser);
	static void CDB_RequestSeUserPledgeInfo(LPVOID lpInstance, UINT charId, INT32 field, INT32 value);
	static bool RequestShowPledgeSkillList(User *pUser);
	static void ApplyNewPenalty();
	static bool CreateSubUnit(User *pUser, int nPledgeType, wstring wPledgeName, wstring wMasterName);
	static bool RequestRenameSubPledge(User *pUser, int nPledgeType, wstring wPledgeName);
	static bool RequestSetSubUnitCaptain(User *pUser, int nPledgeType, wstring wCaptain);
	static bool RequestExchangeReputationForSetItems(User *pUser, int nSetType);
	static bool AcquireSkillInfo(CUserSocket *pSocket, int nSkillID, int nLevel);
	static bool RequestAcquireSkill(CUserSocket *pSocket, int nSkillID, int nLevel);
	static bool Die(User *pUser, CCreature *pEnemy);
	static void AC_ChangeOwner(CPledge *pPledge, int nLastOwnerID, int nResidenceID);
	static void CAgit_ChangeOwner(CResidence *pAgit, CPledge *pPledge, bool bCheck);
	static void CCastle_OnChangeOwner(CResidence *pCastle);
	static void EndSiege(CCastle *pCastle);
	static void CCastle_StartSiege(CCastle *pCastle);
	static void CPledge_DeleteDBMember(void *pPointer);
	static CPledge* GetRelationTo_GetPledge(User *pUser);
	static bool GotEnoughReputationForSubpledge(CPledge *pPledge, int nPledgeType);
	static void RequestLevelUp(User *pUser);
	static void DeleteExpelLock(LPVOID lpRWLock);
	static DWORD EnterExpelLock(LPVOID lpRWLock, const WCHAR* wFunction, int line);
	static void LeaveExpelLock(LPVOID lpRWLock);
};