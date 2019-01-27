#pragma once

typedef bool (*_BuildCmdFunc)(User*, wstring);

typedef bool (*BuilderCallback)(CSocket *pSocket, User* pUser, WCHAR* pBuf);

struct BuilderCmdInfo
{
	const WCHAR* pName;
	INT64 requiredLevel;
	BuilderCallback func;
};



class CCommandInfo
{
public:
	wstring wsName;
	int nRequiedLevel;
	_BuildCmdFunc Func;
	CCommandInfo() : nRequiedLevel(0){};
	void Set(wstring sName, int RequiedLevel, _BuildCmdFunc Function)
	{
		wsName = sName;
		nRequiedLevel = RequiedLevel;
		Func = Function;
	}
};

class CBuilderCommand
{
	static map<wstring, CCommandInfo> mData;
public:
	static void Initialize();
	static void ValidateBuilder(User *pUser);
	static void LoadData();
	static void SetBuilderCmdLevel(const WCHAR* wName, int newLevel);
	static void LoadBuilderList();
	static bool Handle(User *pUser, wstring wsCommand);
	static bool ResetReuse(User *pUser, wstring wsParams);
	static bool AddReputation(User *pUser, wstring wsParams);
	static bool RedSky(User *pUser, wstring wsParams);
	static bool Healthy(User *pUser, wstring wsParams);
	static bool SetHero(User *pUser, wstring wsParams);
	static bool DelHero(User *pUser, wstring wsParams);
	static bool SetKarma(User *pUser, wstring wsParams);
	static bool SetNobless(User *pUser, wstring wsParams);
	static bool DelClanSkills(User *pUser, wstring wsParams);
	static bool Announce2(User *pUser, wstring wsParams);
	static bool PlayEventSound(User *pUser, wstring wsParams);
	static bool ResetCursedWeapon(User *pUser, wstring wsParams);
	static bool CursedWeaponInfo(User *pUser, wstring wsParams);
	static bool PartyRecall(User *pUser, wstring wsParams);
	static bool Who(User *pUser, wstring wsParams);
	static bool WhoIsOnline(User *pUser, wstring wsParams);
	static bool AugmentItem(User *pUser, wstring wsParams);
	static bool EnchantItem(User *pUser, wstring wsParams);
	static bool SetPendingWriteLimit(User *pUser, wstring wsParams);
	static bool TestCMD(User *pUser, wstring wsParams);
	static bool SetClientVersion(User *pUser, wstring wsParams);
	static bool SetSSQSky(User *pUser, wstring wsParams);
	static bool GiveItemToAll(User *pUser, wstring wsParams);
	static bool EnchantWeapon(User *pUser, wstring wsParams);
	static bool GiveItem(User *pUser, wstring wsParams);
	static bool DelItem(User *pUser, wstring wsParams);
	static bool ColiseumFlush(User *pUser, wstring wsParams);
	static bool ResetClanName(User *pUser, wstring wsParams);
	static bool ShowTvTPanel(User *pUser, wstring wsParams);
	static bool DelVIPStatus(User *pUser, wstring wsParams);
	static bool SetExpRate(User *pUser, wstring wsParams);
	static bool SetDropRate(User *pUser, wstring wsParams);
	static bool SetSpoilRate(User *pUser, wstring wsParams);
	static bool SetAIO(User *pUser, wstring wsParams);
	static bool DelAIO(User *pUser, wstring wsParams);
	static bool AddToNpcPos(User* pUser, wstring wParams);
	static bool AddToLoc(User* pUser, wstring wParams);	

	//team container commands
	static bool CreateTeam(User *pUser, wstring wsParams);
	static bool DeleteTeam(User *pUser, wstring wsParams);
	static bool ClearTeam(User *pUser, wstring wsParams);
	static bool TeamList(User *pUser, wstring wsParams);
	static bool DelUserFromTeam(User *pUser, wstring wsParams);
	static bool AddUserToTeam(User *pUser, wstring wsParams);
	static bool MessageWithParamToTeam(User *pUser, wstring wsParams);
	static bool MessageToTeam(User *pUser, wstring wsParams);
	//Hero tournament
	static bool HTMessage(User *pUser, wstring wsParams);
	//IP&HWID addon
	static bool SetClientLimit(User *pUser, wstring wsParams);
	static bool ShowUsersByHwid(User *pUser, wstring wsParams);
	static bool ShowUsersByIP(User *pUser, wstring wsParams);
	static bool ShowUsersByIPnHwid(User *pUser, wstring wsParams);
	//dev tools
	static bool GetSDValue(User *pUser, wstring wsParams);
	//block item
	static bool ShowBlockableItems(User *pUser, wstring wsParams);
	static bool BanAccount(User *pUser, wstring wsParams);
	static bool ResetBan(User *pUser, wstring wsParams);
	static bool BanHwid(User* pUser, wstring wParams);
	static bool BanHwidEx(User* pUser, wstring wParams);
	static bool UnbanHwid(User* pUser, wstring wParams);

	static bool KillPlayer(User* pUser, wstring wParams);
	static bool KillParty(User* pUser, wstring wParams);
	static bool SaveManor(User *pUser, wstring wParams);
	static bool PermaBanHwid(User *pUser, wstring wParams);
	static bool GiveItemInRange(User *pUser, wstring wParams);
};
