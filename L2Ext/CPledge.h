#pragma once

struct MembersLimit
{
	int MainClan0;
	int MainClan1;
	int MainClan2;
	int MainClan3;
	int MainClan4;

	int Academy;
	int RoyalGuard;
	int OrderOfKnight;

	MembersLimit() : MainClan0(10), MainClan1(15), MainClan2(20), MainClan3(30), MainClan4(40), Academy(20), RoyalGuard(20), OrderOfKnight(10) {}
};

struct MemberData
{
	WCHAR wszName[26];
	int nDBID; //34
	int nLevel; //38
	int nClass; //3C
	int nGender; //40
	int nRace; //44
	UINT nObjectID; //48
	int nPledgeType; //4C
	WORD nRank; // 50
	WORD nJoinLevel; // 52
	int nSponsorID; // 54
};

typedef map<int , MemberData*> MemberDataMap;

//178h size
class CPledge : public CObject
{
public:
	enum PledgeType
	{
		MAIN_CLAN = 0,
		ACADEMY = -1,
		ROYAL_GUARD_I = 100,
		ROYAL_GUARD_II = 200,
		ORDER_OF_KNIGHTS_I = 1001,
		ORDER_OF_KNIGHTS_II = 1002,
		ORDER_OF_KNIGHTS_III = 2001,
		ORDER_OF_KNIGHTS_IV = 2002
	};
public:
	/* 0030 */ LPCIOCriticalSection PledgeLock;
	/* 0038 */ CPledgeSharedData *pSD;
	/* 0040 */ int _n0040;
	/* 0044 */ int _n0044;
	/* 0048 */ int _n0048;
	/* 004C */ int _n004C;
	/* 0050 */ MemberDataMap m_Members;
	/* 0068 */ int _unk0068;
	/* 006C */ int _unk006C;
	/* 0070 */ int _unk0070;
	/* 0074 */ int _unk0074;
	/* 0078 */ CCastle* pCastle; //QWORD it tells if clan has castle, perhaps pointer to castle
	/* 0080 */ CAgit* pAgit; //QWORD it tells if clan has agit so perhaps it's pointer to agit
	/* 0088 */ int _unk0088;
	/* 008C */ int _unk008C;
	/* 0090 */ int _unk0090;
	/* 0094 */ int _unk0094;
	/* 0098 */ int _unk0098;
	/* 009C */ int _unk019C;
	/* 00A0 */ int _unk00A0;
	/* 00A4 */ int _unk00A4;
	/* 00A8 */ int _unk00A8;
	/* 00AC */ int _unk00AC;
	/* 00B0 */ int _unk00B0;
	/* 00B4 */ int _unk00B4;
	/* 00B8 */ int _unk00B8;
	/* 00BC */ int _unk00BC;
	/* 00C0 */ int _unk00C0;
	/* 00C4 */ int _unk00C4;
	/* 00C8 */ int _unk00C8;
	/* 00CC */ int _unk00CC;
	/* 00D0 */ int _unk00D0;
	/* 00D4 */ int _unk00D4;
	/* 00D8 */ int _unk00D8;
	/* 00DC */ int _unk00DC;
	/* 00E0 */ int _unk00E0;
	/* 00E4 */ int nClanLevel;
	/* 00E8 */ int _unk00E8;
	/* 00EC */ int _unk00EC;
	/* 00F0 */ int _unk00F0;
	/* 00F4 */ int _unk00F4;
	/* 00F8 */ int _unk00F8;
	/* 00FC */ int _unk00FC;
	/* 0100 */ int _unk0100;
	/* 0104 */ int _unk0104;
	/* 0108 */ int _unk0108;
	/* 010C */ int _unk010C;
	/* 0110 */ int _unk0110;
	/* 0114 */ int _unk0114;
	/* 0118 */ int _unk0118;
	/* 011C */ int _unk011C;
	/* 0120 */ int _unk0120;
	/* 0124 */ int nIsInWar;
	/* 0128 */ map<UINT, UINT> m_WarList;
	/* 0140 */ set<UINT> m_AttackerList;
	/* 0158 */ int _unk0158;
	/* 015C */ int nWaitingForReply;
	/* 0160 */ set<UINT> m_OnlineMembers;
public:
	CPledge *Constructor(INT32 nPledgeID, INT32 nMasterID, PWCHAR wszPledgeName, PWCHAR wszMasterName);
	LPVOID Destructor(DWORD dData);
	PWCHAR GetName();
	CPledgeSharedData *GetSharedData();
	INT64 *GetAlliance();
	int GetAllianceID();
	PWCHAR GetAllianceName();
	int GetAllianceCrestID();
	void UpdatePledgeInfo();
	PledgeVector GetEnemyList();
	CPledge *GetEnemyPledgeByName(PWCHAR wName);
	void GetAttackerList(PledgeVector& attackers);
	void GetWarList(PledgeVector& pledges);
	void GetAllMembers(UserVector& members);
	bool IsInWarWith(int nPledgeID);
	bool IsInWarWithEx(UINT sid);
	void Join(User *pUser);
	bool CanJoin(int nPledgeType);
	int GetDefaultRank(int nPledgeType);
	int IsCaptainOfSubpledge(PWCHAR wszName);
	void Withdraw(User *pUser);
	void Oust(int nUserDBID, PWCHAR wszName);
	void Realase(int nOption);
	void NotifyMemberKilledDuringWar(const WCHAR* wName);
	void NotifyMemberKilledEnemyDuringWar();
	void NotifyMemberNamedHero(PWCHAR wszName);
	void NotifyReputationDecreased(int nPoints);
	void NotifyReputationIncreased(int nPoints);
	void NotifyMemberGradeChanged(PWCHAR wszName, int nRank);
	void NotifySubPledgeCreated(int nPledgeType);
	void NotifyMemberGraduateFromAcademy(User *pUser, int nReputation);
	void UpdateMembersPrivilege(int nRank);
	void OpenAllWindowPacket(User *pUser, bool bGM, PWCHAR wszActiveUser);
	void UpdateWindowPacket(User *pUser, bool bIsOnline);
	void AddWindowPacket(User *pUser);
	void AddDBMemberByUser(User *pUser);
	void AddDBMemberByDBID(int nDBID, PWCHAR wszName, int nLevel, int nClass, int nGander, int nRace, int nServerID);
	void DeleteDBMeberByDBID(int nDBID);
	void Save(int nField);
	void SetMaster(User *pUser);
	void IncreaseLevel678(int nnLevel);
	User *GetMember(PWCHAR wszName);
	void Broadcast(size_t nLen, PCHAR packet);
	int GetSubPledgeMembers(int nPledgeType);
	bool CreateSubPledge(User *pUser, int nPledgeType, PWCHAR wszName);
	bool SetSubCaptain(User *pUser, int nPledgeType,  PWCHAR wszName);
	void SendEnemyList(User *pUser);
	void SendAttackerList(User *pUser);
	bool CanAcceptNewMember();
	bool IsWorkingWithAnotherClan(int nTimeout);
	void ValidateTotalLevel();
	void UpdateMembers();
	User *GetMaster();
	bool HasSubUnit(int nPledgeType);
	void DismissApprentice(MemberData *pData);
	MemberData *GetMemberDataByDBID(int nDBID);
	int GetApprenticeDBID(int nSponsorID);
	MemberData *GetApprenticeData(int nSponsorID);
	MemberData *GetMemberDataByName(const PWCHAR wsName);
	int GetReputation();
	void NotifyPledgeSkillAcquired(int nSkillID, int nLevel);
	void AcquirePledgeSkills(User *pUser);
	void DeletePledgeSkills(User *pUser);
	void SendSkillList(User *pUser);
	void UpdateInfo(CUserSocket *pSocket = 0);
	void SubUnitCreatedPacket(User *pUser, int nPledgeType);
	void UpdateMemberList(MemberData *pData);
	bool IsValidSubPledgeName(wstring wName);
	void UpdatePledgeNameValue(int value);
	static CPledge* GetPledge(UINT sid);
};
//NcSoft's GetPledgeByDBID - that func sucks
static CPledge *GetPledgeByID(int nPledgeID)
{
	CPledgeSP pledge_sp;
	typedef void(*f)(INT64 , CPledgeSP*, int);
	f(0x405600L)(0x0E285350, &pledge_sp, nPledgeID);
	return pledge_sp.get();
};