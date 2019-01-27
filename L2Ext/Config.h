#pragma once

enum PENALTY
{
	PLEDGE_OUST_PENALTY_TIMEOUT,
	PLEDGE_WITHDRAW_PENALTY_TIMEOUT,
	PLEDGE_OUSTED_PENALTY_TIMEOUT,
	PLEDGE_DISMISS_TIMEOUT,
	PLEDGE_DISMISS_PENALTY_TIMEOUT,
	ALLIANCE_OUST_PENALTY_TIMEOUT,
	ALLIANCE_WITHDRAW_PENALTY_TIMEOUT,
	ALLIANCE_OUSTED_PENALTY_TIMEOUT,
	ALLIANCE_DISMISS_PENALTY_TIMEOUT
};

class CConfig
{
private:
	static CConfig* Instance;
public:
	enum SYSTEM
	{
		LOG,
		SIEGE_REPORT,
		ITEM_REUSE_MANAGER,
		SPIRIT_SYSTEM,
		SKILL_REUSE_MANAGER,
		DISABLE_PARTY_LEVEL_GAP,
		KILL_DEATH_STAT,
		ERA_SYSTEM,
		ADMIN_SOCKET
	};
	CConfig();
	void ReadData();
	void SetSystem(SYSTEM System, bool Value);
	bool IsSet(SYSTEM System);
private:
	bool LogSystem;
	bool SiegeReport;
	bool CheckGeoInAnimation;
	bool ItemReuseManager;
	bool SpiritSystem;
	bool SkillReuseManager;
	bool DisablePartyLevelGap;
	bool KillDeathStat;
	bool EraSystem;
	bool AdminSocket;
public:
	class CL2ServerConfig
	{
		int nPvpFlagTimeShort;
		int nPvpFlagTimeLong;
		int nUserLimit;
		bool bPlayerBuffAsSelfBuff;
		bool bTrueBoostFix;
		bool bProtectHeadquarter;
		bool bExtendClanHallSiege;
		bool bRestoreOldCH;
		int SpoilFix;
	public:
		CL2ServerConfig();
		void SetPvpFlagTimeShort(int Val) { nPvpFlagTimeShort = Val; };
		void SetPvpFlagTimeLong(int Val) { nPvpFlagTimeLong = Val; };
		int GetPvpFlagTimeShort() { return nPvpFlagTimeShort; };
		int GetPvpFlagTimeLong() { return nPvpFlagTimeLong; };
		int GetUserLimit() { return nUserLimit; };
		int GetSpoilFix() { return SpoilFix; };
		bool IsPlayerBuffAsSelfBuff() { return bPlayerBuffAsSelfBuff; };
		bool IsTrueBoostFix() { return bTrueBoostFix; };
		bool IsProtectHeadquarter() { return bProtectHeadquarter; };
		bool IsExtendClanHallSiege() { return bExtendClanHallSiege; };
		bool IsRestoreOldCH() { return bRestoreOldCH; };
		void SetPlayerBuffAsSelfBuff(bool Val) { bPlayerBuffAsSelfBuff = Val; };
		void SetUserLimit(int Value) { nUserLimit = Value; };
		void Load();
	} L2ServerConfig;
	class CAdminSocketInfo
	{
		int MaxActiveConnections;
		int Port;
		int Password;
	public:
		CAdminSocketInfo();
		void SetMaxActiveConnections(int Value) { MaxActiveConnections = Value; };
		int GetMaxActiveConnections() { return MaxActiveConnections; };
		void SetPort(int Value) { Port = Value; };
		int GetPort() { return Port; };
		void SetPassword(int Value) { Password = Value; };
		int GetPassword() { return Password; };
		void Load();
	} AdminSocketInfo;
	class CProtocol
	{
		char sProtocol[8];
		int nProtocol;
	public:
		CProtocol() : nProtocol(746) { strcpy_s(sProtocol, sizeof(sProtocol), "746"); };
		void SetProtocolVersion(int Protocol) { nProtocol = Protocol; itoa(Protocol, sProtocol, 10); };
		int GetProtocolVersion() { return nProtocol; };
		PCHAR GetProtocolVersionS() { return sProtocol; };
	} Protocol;
	class CClanInfo
	{
	public:
		enum PRICE
		{
			ACADEMY_SUBPLEDGE,
			ROYAL_GUARD_SUBPLEDGE,
			ORDER_OF_KNIGHT_SUBPLEDGE,
			APPELlA_SET,
			OATH_SET
		};
		void SetPenalty(PENALTY Type, int Time) { mPenalty[Type] = Time; };
		int GetPenalty(PENALTY Type) { map<PENALTY, int>::iterator Iter = mPenalty.find(Type); if(Iter!=mPenalty.end()) return Iter->second; else return 86400; };
		void SetAllySize(int Size) { AllySize = (BYTE)Size; };
		BYTE GetAllySize() { return AllySize; };
		void SetWarDelay(int Delay) { nWarDelay = Delay; };
		int GetWarDelay() { return nWarDelay; };
		void SetPrice(PRICE Type, int Price) { mPrice[Type] = Price; };
		int GetPrice(PRICE Type) { map<PRICE, int>::iterator Iter = mPrice.find(Type); if(Iter!=mPrice.end()) return Iter->second; else return NULL; };
	private:
		map<PENALTY, int> mPenalty;
		map<PRICE, int> mPrice;
		BYTE AllySize;
		int nWarDelay;
	} ClanInfo;
	class CAugmentationInfo
	{
	public:
		enum LIFE_STONE
		{
			LOW,
			MID,
			HIGH,
			TOP
		};
		CAugmentationInfo() { memset(BaseStatChance, 0, sizeof(BaseStatChance)); memset(SkillChance, 0, sizeof(SkillChance)); }
		void SetBaseStatChance(LIFE_STONE Type, int Value) { BaseStatChance[Type] = Value; };
		int GetBaseStatChance(LIFE_STONE Type) { return BaseStatChance[Type]; };
		void SetSkillChance(LIFE_STONE Type, int Value) { SkillChance[Type] = Value; };
		int GetSkillChance(LIFE_STONE Type) { return SkillChance[Type]; };
		void AddBlockedItem(int ItemID) { lBlockedItem.push_back(ItemID); };
		bool IsBlockedItem(int ItemID) { for(list<int>::iterator Iter = lBlockedItem.begin();Iter!=lBlockedItem.end();Iter++) { if(*Iter == ItemID) return true; }return false; };
		int GetBlockedItemCount() { return (int)lBlockedItem.size(); };
		void AddBlockedGlow(int ItemID) { lBlockedGlow.push_back(ItemID); };
		bool IsBlockedGlow(int ItemID) { for(list<int>::iterator Iter = lBlockedGlow.begin();Iter!=lBlockedGlow.end();Iter++) { if(*Iter == ItemID) return true; }return false; };
		int GetBlockedGlowCount() { return (int)lBlockedGlow.size(); };
		void SetDispelOnPassive(bool Value) { DispelOnPassive = Value; };
		bool IsDispelOnPassive() { return DispelOnPassive; };
		void SetDispelOnAttribute(bool Value) { DispelOnAttribute = Value; };
		bool IsDispelOnAttribute() { return DispelOnAttribute; };
		void Load();
	private:
		list<int> lBlockedItem;
		list<int> lBlockedGlow;
		int BaseStatChance[4];
		int SkillChance[4];
		bool DispelOnPassive;
		bool DispelOnAttribute;
	} AugmentationInfo;
	class CLogInfo
	{
	public:
		CLogInfo() { memset(this, 0, sizeof(CLogInfo)); }
		void SetSaveSystem(bool Value) { bSaveSystemLog = Value; };
		bool IsSaveSystem() { return bSaveSystemLog; };
		void SetSaveError(bool Value) { bSaveErrorLog = Value; };
		bool IsSaveError() { return bSaveErrorLog; };
		void SetSaveIn(bool Value) { bSaveInLog = Value; };
		bool IsSaveIn() { return bSaveInLog; };
		void SetSaveDb(bool Value) { bSaveDbLog = Value; };
		bool IsSaveDb() { return bSaveDbLog; };
		void SetSaveAudit(bool Value) { bSaveAuditLog = Value; };
		bool IsSaveAudit() { return bSaveAuditLog; };
		void SetSaveChat(bool Value) { bSaveChatLog = Value; };
		bool IsSaveChat() { return bSaveChatLog; };
		bool IsBlockOrtReport() { return bBlockOrtReport; };
		void SetBlockOrtReport(bool Value) { bBlockOrtReport = Value; };
	private:
		bool bSaveSystemLog;
		bool bSaveErrorLog;
		bool bSaveInLog;
		bool bSaveDbLog;
		bool bSaveAuditLog;
		bool bSaveChatLog;
		bool bBlockOrtReport;
	} LogInfo;
	class CSocketLimiterInfo
	{
	public:
		CSocketLimiterInfo() : nLimitPerIp(0), nBanDuration(0), nPendingWriteLimit(0), nPendingWriteDuringSiege(0) {};
		void SetIpList(tstring sStr) { sIpList = sStr; };
		tstring GetIpList() { return sIpList; };
		void SetLimitPerIp(int Value) { nLimitPerIp = Value; };
		int GetLimitPerIp() { return nLimitPerIp; };
		void SetBanDuration(int Value) { nBanDuration = Value; };
		int GetBanDuration() { return nBanDuration; };
		void SetPendingWriteLimit(int Value) { nPendingWriteLimit = Value; };
		int GetPendingWriteLimit() { return nPendingWriteLimit; };
		void SetPendingWriteLimitDuringSiege(int Value) { nPendingWriteDuringSiege = Value; };
		int GetPendingWriteLimitDuringSiege() { return nPendingWriteDuringSiege; };
	private:
		tstring sIpList;
		int nLimitPerIp;
		int nBanDuration;
		int nPendingWriteLimit;
		int nPendingWriteDuringSiege;
	} SockLimiterInfo;
	class CSiegeReportInfo
	{
	public:
		CSiegeReportInfo() : bReportDamage(0), bDumpToDB(0), bGenerateHTML(0) {};
		void SetReportDamage(bool Value) { bReportDamage = Value; };
		bool IsReportDamage() { return bReportDamage; };
		void SetDumpToDB(bool Value) { bDumpToDB = Value; };
		bool IsDumpToDB() { return bDumpToDB; };
		bool IsDumpToFile() { return bDumpToFile; };
		void SetDumpToFile(bool Value) { bDumpToFile = Value; };
		void SetGenerateHTML(bool Value) { bGenerateHTML = Value; };
		bool IsGenerateHTML() { return bGenerateHTML; };
	private:
		bool bReportDamage;
		bool bDumpToDB;
		bool bDumpToFile;
		bool bGenerateHTML;
	} SiegeReportInfo;
	class CSpiritSystemInfo
	{
	private:
		int MinReleaseSpiritLimit;
		int MaxReleaseSpiritLimit;
		int LevelRangeLimit;
		int MinRequiredLevel;
		int StaticBonus;
		int PercentBonus;
		int SkillID;
	public:
		void SetMinReleaseSpiritLimit(int Value) { MinReleaseSpiritLimit = Value; };
		int GetMinReleaseSpiritLimit() { return MinReleaseSpiritLimit; };
		void SetMaxReleaseSpiritLimit(int Value) { MaxReleaseSpiritLimit = Value; };
		int GetMaxReleaseSpiritLimit() { return MaxReleaseSpiritLimit; };
		void SetLevelRangeLimit(int Value) { LevelRangeLimit = Value; };
		int GetLevelRangeLimit() { return LevelRangeLimit; };
		void SetMinRequiredLevel( int Value ) { MinRequiredLevel = Value; };
		int GetMinRequiredLevel() { return MinRequiredLevel; };
		void SetStaticBonus(int Value) { StaticBonus = Value; };
		int GetStaticBonus() { return StaticBonus; };
		void SetPercentBonus( int Value ) { PercentBonus = Value; };
		int GetPercentBonus() { return PercentBonus; };
		int GetSkillID() { return SkillID; };
		CSpiritSystemInfo() { memset(this, 0, sizeof(CSpiritSystemInfo)); }
		void Load()
		{
			CConfig::Instance->SetSystem(CConfig::SPIRIT_SYSTEM, GetPrivateProfileInt(_T("Features"), _T("SpiritSystem"), NULL, g_ConfigFile) );
			SetLevelRangeLimit( GetPrivateProfileInt(_T("SpiritSystem"), _T("LevelRangeLimit"), 10, g_ConfigFile) );
			SetMaxReleaseSpiritLimit( GetPrivateProfileInt(_T("SpiritSystem"), _T("MaxReleaseSpiritLimit"), 10, g_ConfigFile) );
			SetMinReleaseSpiritLimit( GetPrivateProfileInt(_T("SpiritSystem"), _T("MinReleaseSpiritLimit"), 5, g_ConfigFile) );
			SetMinRequiredLevel( GetPrivateProfileInt(_T("SpiritSystem"), _T("MinRequierdLevel"), 55, g_ConfigFile) );
			SetPercentBonus( GetPrivateProfileInt(_T("SpiritSystem"), _T("PercentBonus"), 10, g_ConfigFile) );
			SetStaticBonus( GetPrivateProfileInt(_T("SpiritSystem"), _T("StaticBonus"), 200, g_ConfigFile) );
			SkillID = GetPrivateProfileInt(_T("SpiritSystem"), _T("SkillID"), 10013, g_ConfigFile);
		}
	} SpiritSystemInfo;
};

extern CConfig g_Config;