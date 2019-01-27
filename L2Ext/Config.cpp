#include "StdAfx.h"

BOOL g_BlockStatisticPacket = FALSE;
BOOL g_SafeListStatisticPacket = FALSE;

CConfig::CConfig()
{
	Instance = this;
}

void CConfig::ReadData()
{	
	Protocol.SetProtocolVersion( GetPrivateProfileInt(_T("Features"), _T("ProtocolVersion"), 746, g_ConfigFile) );
	g_BlockStatisticPacket = GetPrivateProfileInt(_T("Features"), _T("BlockStatisticPacket"), 0, g_ConfigFile);
	g_SafeListStatisticPacket = GetPrivateProfileInt(_T("Features"), _T("SafeListStatisticPacket"), 0, g_ConfigFile);
	LogSystem = GetPrivateProfileInt(_T("Features"), _T("LogSystem"), NULL, g_ConfigFile);
	SiegeReport = GetPrivateProfileInt(_T("Features"), _T("SiegeReport"), NULL, g_ConfigFile);
	CheckGeoInAnimation = GetPrivateProfileInt(_T("Features"), _T("CheckGeoInAnimation"), NULL, g_ConfigFile);
	ItemReuseManager = GetPrivateProfileInt(_T("Features"), _T("ItemReuseManager"), 0, g_ConfigFile);
	SkillReuseManager = GetPrivateProfileInt(_T("Features"), _T("SkillReuseManager"), 0, g_ConfigFile);
	SpiritSystem = GetPrivateProfileInt(_T("Features"), _T("SpiritSystem"), 0, g_ConfigFile);
	DisablePartyLevelGap = GetPrivateProfileInt(_T("Features"), _T("DisablePartyLevelGap"), 0, g_ConfigFile);
	KillDeathStat = GetPrivateProfileInt(_T("Features"), _T("KillDeathStat"), 0, g_ConfigFile);
	EraSystem = GetPrivateProfileInt(_T("Features"), _T("EraSystem"), 0, g_ConfigFile);
	L2ServerConfig.Load();
	AdminSocketInfo.Load();
	if(SpiritSystem)
	{
		SpiritSystemInfo.Load();
	}
	if(SiegeReport)
	{
		SiegeReportInfo.SetReportDamage( GetPrivateProfileInt(_T("SiegeReport"), _T("ReportDamage"), 0, g_ConfigFile) );
		SiegeReportInfo.SetDumpToDB( GetPrivateProfileInt(_T("SiegeReport"), _T("DumpToDB"), 0, g_ConfigFile) );
		SiegeReportInfo.SetDumpToFile( GetPrivateProfileInt(_T("SiegeReport"), _T("DumpToFile"), 0, g_ConfigFile) );
		SiegeReportInfo.SetGenerateHTML( GetPrivateProfileInt(_T("SiegeReport"), _T("GenerateHTML"), 0, g_ConfigFile) );
	}
	if(true)
	{
		TCHAR szString[8190];
		memset(szString, 0, sizeof(szString));
		ClanInfo.SetAllySize( GetPrivateProfileInt(_T("ClanSystem"), _T("MaxAllySize"), 3, g_ConfigFile) );
		ClanInfo.SetPrice(CClanInfo::OATH_SET, GetPrivateProfileInt(_T("ClanSystem"), _T("OathPrice"), 15000, g_ConfigFile) );
		ClanInfo.SetPrice(CClanInfo::APPELlA_SET, GetPrivateProfileInt(_T("ClanSystem"), _T("ApellaPrice"), 25000, g_ConfigFile) );
		
		ClanInfo.SetPrice(CClanInfo::ACADEMY_SUBPLEDGE, GetPrivateProfileInt(_T("ClanSystem"), _T("AcademyPrice"), 0, g_ConfigFile) );
		ClanInfo.SetPrice(CClanInfo::ROYAL_GUARD_SUBPLEDGE, GetPrivateProfileInt(_T("ClanSystem"), _T("RoyalGuardPrice"), 5000, g_ConfigFile) );
		ClanInfo.SetPrice(CClanInfo::ORDER_OF_KNIGHT_SUBPLEDGE, GetPrivateProfileInt(_T("ClanSystem"), _T("OrderOfKnightPrice"), 10000, g_ConfigFile) );
		ClanInfo.SetPenalty(PLEDGE_OUST_PENALTY_TIMEOUT, GetPrivateProfileInt(_T("ClanSystem"), _T("PLEDGE_OUST_PENALTY_TIMEOUT"), 86400, g_ConfigFile));
		ClanInfo.SetPenalty(PLEDGE_WITHDRAW_PENALTY_TIMEOUT, GetPrivateProfileInt(_T("ClanSystem"), _T("PLEDGE_WITHDRAW_PENALTY_TIMEOUT"), 86400, g_ConfigFile));
		ClanInfo.SetPenalty(PLEDGE_OUSTED_PENALTY_TIMEOUT, GetPrivateProfileInt(_T("ClanSystem"), _T("PLEDGE_OUSTED_PENALTY_TIMEOUT"), 86400, g_ConfigFile));
		ClanInfo.SetPenalty(PLEDGE_DISMISS_TIMEOUT, GetPrivateProfileInt(_T("ClanSystem"), _T("PLEDGE_DISMISS_TIMEOUT"), 86400, g_ConfigFile));
		ClanInfo.SetPenalty(PLEDGE_DISMISS_PENALTY_TIMEOUT, GetPrivateProfileInt(_T("ClanSystem"), _T("PLEDGE_DISMISS_PENALTY_TIMEOUT"), 86400, g_ConfigFile));
		ClanInfo.SetPenalty(ALLIANCE_OUST_PENALTY_TIMEOUT, GetPrivateProfileInt(_T("ClanSystem"), _T("ALLIANCE_OUST_PENALTY_TIMEOUT"), 86400, g_ConfigFile));
		ClanInfo.SetPenalty(ALLIANCE_WITHDRAW_PENALTY_TIMEOUT, GetPrivateProfileInt(_T("ClanSystem"), _T("ALLIANCE_WITHDRAW_PENALTY_TIMEOUT"), 86400, g_ConfigFile));
		ClanInfo.SetPenalty(ALLIANCE_OUSTED_PENALTY_TIMEOUT, GetPrivateProfileInt(_T("ClanSystem"), _T("ALLIANCE_OUSTED_PENALTY_TIMEOUT"), 86400, g_ConfigFile));
		ClanInfo.SetPenalty(ALLIANCE_DISMISS_PENALTY_TIMEOUT, GetPrivateProfileInt(_T("ClanSystem"), _T("ALLIANCE_DISMISS_PENALTY_TIMEOUT"), 86400, g_ConfigFile));
		ClanInfo.SetWarDelay( GetPrivateProfileInt(_T("ClanSystem"), _T("WarDelay"), 0, g_ConfigFile) );
	}
	if(LogSystem)
	{
		LogInfo.SetSaveSystem( GetPrivateProfileInt(_T("LogSystem"), _T("SystemLog"), 1, g_ConfigFile) );
		LogInfo.SetSaveError( GetPrivateProfileInt(_T("LogSystem"), _T("ErrorLog"), 1, g_ConfigFile) );
		LogInfo.SetSaveIn( GetPrivateProfileInt(_T("LogSystem"), _T("InLog"), 1, g_ConfigFile) );
		LogInfo.SetSaveDb( GetPrivateProfileInt(_T("LogSystem"), _T("DbLog"), 1, g_ConfigFile) );
		LogInfo.SetSaveAudit( GetPrivateProfileInt(_T("LogSystem"), _T("AuditLog"), 1, g_ConfigFile) );
		LogInfo.SetSaveChat( GetPrivateProfileInt(_T("LogSystem"), _T("ChatLog"), 1, g_ConfigFile) );
		LogInfo.SetBlockOrtReport( GetPrivateProfileInt(_T("LogSystem"), _T("BlockOrtReport"), 1, g_ConfigFile) );
	}
	
	AugmentationInfo.Load();
}

bool CConfig::IsSet(CConfig::SYSTEM System)
{
	switch(System)
	{
	case LOG:
		return LogSystem;
		break;
	case SIEGE_REPORT:
		return SiegeReport;
		break;
	case ITEM_REUSE_MANAGER:
		return ItemReuseManager;
		break;
	case SPIRIT_SYSTEM:
		return SpiritSystem;
		break;
	case SKILL_REUSE_MANAGER:
		return SkillReuseManager;
	case DISABLE_PARTY_LEVEL_GAP:
		return DisablePartyLevelGap;
		break;
	case KILL_DEATH_STAT:
		return KillDeathStat;
		break;
	case ERA_SYSTEM:
		return EraSystem;
		break;
	case ADMIN_SOCKET:
		return AdminSocket;
		break;
	}
	return false;
};

void CConfig::SetSystem(CConfig::SYSTEM System, bool Value)
{
	switch(System)
	{
	case LOG:
		LogSystem = Value;
		break;
	case SIEGE_REPORT:
		SiegeReport = Value;
		break;
	case ITEM_REUSE_MANAGER:
		ItemReuseManager = Value;
		break;
	case SPIRIT_SYSTEM:
		SpiritSystem = Value;
		break;
	case SKILL_REUSE_MANAGER:
		SkillReuseManager = Value;
		break;
	case DISABLE_PARTY_LEVEL_GAP:
		DisablePartyLevelGap = Value;
		break;
	case KILL_DEATH_STAT:
		KillDeathStat = Value;
		break;
	case ERA_SYSTEM:
		EraSystem = Value;
		break;
	case ADMIN_SOCKET:
		AdminSocket = Value;
		break;
	}
};

CConfig::CL2ServerConfig::CL2ServerConfig() : nPvpFlagTimeShort(20), nPvpFlagTimeLong(40), nUserLimit(0), bPlayerBuffAsSelfBuff(false), SpoilFix(0)
{
};

void CConfig::CL2ServerConfig::Load()
{
	nPvpFlagTimeShort = GetPrivateProfileInt(_T("L2Server"), _T("PvpFlagTimeShort"), 20, g_ConfigFile);
	nPvpFlagTimeLong = GetPrivateProfileInt(_T("L2Server"), _T("PvpFlagTimeLong"), 40, g_ConfigFile);
	nUserLimit = GetPrivateProfileInt(_T("L2Server"), _T("UserLimit"), 0, g_ConfigFile);
	bPlayerBuffAsSelfBuff = GetPrivateProfileInt(_T("L2Server"), _T("PlayerBuffAsSelfBuff"), 0, g_ConfigFile);
	bTrueBoostFix = GetPrivateProfileInt(_T("L2Server"), _T("TrueBoostFix"), 0, g_ConfigFile);
	bProtectHeadquarter = GetPrivateProfileInt(_T("L2Server"), _T("ProtectHeadquarter"), 0, g_ConfigFile);
	bExtendClanHallSiege = GetPrivateProfileInt(_T("L2Server"), _T("ExtendClanHallSiege"), 0, g_ConfigFile);
	bRestoreOldCH = GetPrivateProfileInt(_T("L2Server"), _T("RestoreOldCH"), 0, g_ConfigFile);
	SpoilFix = GetPrivateProfileInt(_T("L2Server"), _T("SpoilFix"), 0, g_ConfigFile);
};

CConfig::CAdminSocketInfo::CAdminSocketInfo() : Port(6891), MaxActiveConnections(40)
{
}

void CConfig::CAdminSocketInfo::Load()
{
	CConfig::Instance->SetSystem(CConfig::ADMIN_SOCKET, GetPrivateProfileInt(_T("Features"), _T("AdminSocket"), NULL, g_ConfigFile) );
	Port = GetPrivateProfileInt(_T("AdminSocket"), _T("Port"), 6891, g_ConfigFile);
	MaxActiveConnections = GetPrivateProfileInt(_T("AdminSocket"), _T("MaxActiveConnections"), 40, g_ConfigFile);
	Password = GetPrivateProfileInt(_T("AdminSocket"), _T("Password"), 1234509876, g_ConfigFile);
}

void CConfig::CAugmentationInfo::Load()
{
	lBlockedItem.clear();
	lBlockedGlow.clear();

	SetBaseStatChance( LOW, GetPrivateProfileInt(_T("Augmentation"), _T("BaseStatLOW"), 0, g_ConfigFile) );
	SetBaseStatChance( MID, GetPrivateProfileInt(_T("Augmentation"), _T("BaseStatMID"), 1, g_ConfigFile) );
	SetBaseStatChance( HIGH, GetPrivateProfileInt(_T("Augmentation"), _T("BaseStatHIGH"), 2, g_ConfigFile) );
	SetBaseStatChance( TOP, GetPrivateProfileInt(_T("Augmentation"), _T("BaseStatTOP"), 2, g_ConfigFile) );
	SetSkillChance( LOW, GetPrivateProfileInt(_T("Augmentation"), _T("SkillLOW"), 2, g_ConfigFile) );
	SetSkillChance( MID, GetPrivateProfileInt(_T("Augmentation"), _T("SkillMID"), 3, g_ConfigFile) );
	SetSkillChance( HIGH, GetPrivateProfileInt(_T("Augmentation"), _T("SkillHIGH"), 5, g_ConfigFile) );
	SetSkillChance( TOP, GetPrivateProfileInt(_T("Augmentation"), _T("SkillTOP"), 7, g_ConfigFile) );
	SetDispelOnPassive( GetPrivateProfileInt(_T("Augmentation"), _T("DispelOnPassive"), 0, g_ConfigFile) );
	SetDispelOnAttribute( GetPrivateProfileInt(_T("Augmentation"), _T("DispelOnAttribute"), 0, g_ConfigFile) );

	TCHAR sString[8192];
	memset(sString, 0, sizeof(sString));
	if(GetPrivateProfileString(_T("Augmentation"), _T("BlockItem"), 0, sString, sizeof(sString), g_ConfigFile))
	{
		
		tstringstream sstr;
		sstr << sString;
		int nItemID = 0;
		while(sstr >> nItemID)
		{
			AddBlockedItem(nItemID);
		}
	}
	if(GetPrivateProfileString(_T("Augmentation"), _T("BlockGlow"), 0, sString, sizeof(sString), g_ConfigFile))
	{
		tstringstream sstr;
		sstr << sString;
		int nItemID = 0;
		while(sstr >> nItemID)
		{
			AddBlockedGlow(nItemID);
		}			
	}
}