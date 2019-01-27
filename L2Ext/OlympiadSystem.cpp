#include "StdAfx.h"
#include "SocketDB.h"
#include "OlympiadSystem.h"
#include "OlympiadField.h"
#include "ObjectDB.h"
#include "CategoryDataDB.h"

COlympiadDB g_OlympiadDB;

BOOL g_OlympiadSystemEnabled = FALSE;
OlympiadPeriod g_OlympiadPeriod[7][2];
BOOL g_OlympiadReloginFix = FALSE;
BOOL g_OlympiadOverweightFix = FALSE;
UINT g_OlympiadClassFreeEntryCount = 5;
UINT g_OlympiadClassEntryCount = 5;
list<int> g_OlympiadForbiddenItemList;
INT32 g_OlympiadMaxAllowedWeaponEnchant = 0;
INT32 g_OlympiadMaxAllowedArmorEnchant = 0;
UINT g_OlympiadMatchTime = 360;
UINT g_OlympiadPrepareToMatchTime = 60;
BOOL g_OlympiadCheckHWID = FALSE;
UINT g_HeroVoiceDelay = 0;
vector<UINT> g_OlympiadForbiddenItemGrade;

BOOL g_OlympiadClassFreeFight[7];
BOOL g_OlympiadClassFight[7];

INT32 g_OlympiadWinnerRewardId = 0;
INT32 g_OlympiadWinnerRewardAmount = 0;
INT32 g_OlympiadLooserRewardId = 0;
INT32 g_OlympiadLooserRewardAmount = 0;
BOOL g_OlympiadStopActionOnFightStart = FALSE;
BOOL g_SetFixedOlympiadFightPoints = FALSE;

void COlympiadSystem::LoadINI()
{	
	TCHAR sTmp[8192];
	sTmp[0] = 0;
	const TCHAR* section = _T("OlympiadSystem");

	g_OlympiadSystemEnabled = GetPrivateProfileInt(section, _T("Enabled"), 0, g_ConfigFile);
	g_OlympiadClassFreeEntryCount = GetPrivateProfileInt(section, _T("ClassFreeEntry"), 5, g_ConfigFile);
	g_OlympiadClassEntryCount = GetPrivateProfileInt(section, _T("ClassEntry"), 5, g_ConfigFile);
	g_OlympiadReloginFix = GetPrivateProfileInt(section, _T("ReloginFix"), 0, g_ConfigFile);
	g_OlympiadOverweightFix = GetPrivateProfileInt(section, _T("OverweightFix"), 0, g_ConfigFile);
	g_OlympiadMaxAllowedWeaponEnchant = GetPrivateProfileInt(section, _T("MaxWeaponEnchant"), 0, g_ConfigFile);
	g_OlympiadMaxAllowedArmorEnchant = GetPrivateProfileInt(section, _T("MaxArmorEnchant"), 0, g_ConfigFile);
	g_OlympiadMatchTime = GetPrivateProfileInt(section, _T("MatchTime"), 360, g_ConfigFile);
	g_OlympiadPrepareToMatchTime = GetPrivateProfileInt(section, _T("PrepareToMatchTime"), 60, g_ConfigFile);
	g_HeroVoiceDelay = GetPrivateProfileInt(section, _T("HeroChatDelay"), 0, g_ConfigFile);
	g_OlympiadCheckHWID = GetPrivateProfileInt(section, _T("CheckHWID"), 0, g_ConfigFile);

	g_OlympiadWinnerRewardId = GetPrivateProfileInt(section, _T("WinnerRewardId"), 0, g_ConfigFile);
	g_OlympiadWinnerRewardAmount = GetPrivateProfileInt(section, _T("WinnerRewardAmount"), 0, g_ConfigFile);
	g_OlympiadLooserRewardId = GetPrivateProfileInt(section, _T("LooserRewardId"), 0, g_ConfigFile);
	g_OlympiadLooserRewardAmount = GetPrivateProfileInt(section, _T("LooserRewardAmount"), 0, g_ConfigFile);
	g_OlympiadStopActionOnFightStart = GetPrivateProfileInt(section, _T("StopActionOnFightStart"), 0, g_ConfigFile);
	g_SetFixedOlympiadFightPoints = GetPrivateProfileInt(section, _T("SetFixedOlympiadFightPoints"), 0, g_ConfigFile);

	if(GetPrivateProfileString(section, _T("ForbiddenItems"), 0, sTmp, 8192, g_ConfigFile))
	{
		wstringstream str;
		str << sTmp;
		int itemId = 0;
		while(str >> itemId)
		{
			g_OlympiadForbiddenItemList.push_back(itemId);
		}
		if(g_OlympiadForbiddenItemList.size() > 0)
		{
			g_OlympiadForbiddenItemList.sort();
			g_OlympiadForbiddenItemList.unique();
		}
	}

	if(GetPrivateProfileString(section, _T("ForbiddenItemGrade"), 0, sTmp, 8192, g_ConfigFile))
	{
		wstringstream str;
		str << sTmp;
		wstring grade;
		while(str >> grade)
		{
			if(grade == L"no")
			{
				g_OlympiadForbiddenItemGrade.push_back(CrystalNone);
			}else if(grade == L"d")
			{
				g_OlympiadForbiddenItemGrade.push_back(CrystalD);
			}else if(grade == L"c")
			{
				g_OlympiadForbiddenItemGrade.push_back(CrystalC);
			}else if(grade == L"b")
			{
				g_OlympiadForbiddenItemGrade.push_back(CrystalB);
			}else if(grade == L"a")
			{
				g_OlympiadForbiddenItemGrade.push_back(CrystalA);
			}else if(grade == L"s")
			{
				g_OlympiadForbiddenItemGrade.push_back(CrystalS);
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Invalid item grade[%S] - ForbiddenItemGrade", __FUNCTION__, grade.c_str());
			}
		}
	}

	UINT dayIndex = 0;
	
	g_OlympiadPeriod[dayIndex][0].startHour = GetPrivateProfileInt(section, _T("SundayStart"), 20, g_ConfigFile);
	g_OlympiadPeriod[dayIndex][0].endHour = GetPrivateProfileInt(section, _T("SundayEnd"), 0, g_ConfigFile);
	g_OlympiadPeriod[dayIndex][1].startHour = GetPrivateProfileInt(section, _T("SundayStartEx"), 20, g_ConfigFile);
	g_OlympiadPeriod[dayIndex][1].endHour = GetPrivateProfileInt(section, _T("SundayEndEx"), 0, g_ConfigFile);
	g_OlympiadClassFreeFight[dayIndex] = GetPrivateProfileInt(section, _T("SundayClassFree"), 1, g_ConfigFile);
	g_OlympiadClassFight[dayIndex] = GetPrivateProfileInt(section, _T("SundayClass"), 1, g_ConfigFile);
	dayIndex++;
	g_OlympiadPeriod[dayIndex][0].startHour = GetPrivateProfileInt(section, _T("MondayStart"), 20, g_ConfigFile);
	g_OlympiadPeriod[dayIndex][0].endHour = GetPrivateProfileInt(section, _T("MondayEnd"), 0, g_ConfigFile);
	g_OlympiadPeriod[dayIndex][1].startHour = GetPrivateProfileInt(section, _T("MondayStartEx"), 20, g_ConfigFile);
	g_OlympiadPeriod[dayIndex][1].endHour = GetPrivateProfileInt(section, _T("MondayEndEx"), 0, g_ConfigFile);
	g_OlympiadClassFreeFight[dayIndex] = GetPrivateProfileInt(section, _T("MondayClassFree"), 1, g_ConfigFile);
	g_OlympiadClassFight[dayIndex] = GetPrivateProfileInt(section, _T("MondayClass"), 1, g_ConfigFile);
	dayIndex++;
	g_OlympiadPeriod[dayIndex][0].startHour = GetPrivateProfileInt(section, _T("TuesdayStart"), 20, g_ConfigFile);
	g_OlympiadPeriod[dayIndex][0].endHour = GetPrivateProfileInt(section, _T("TuesdayEnd"), 0, g_ConfigFile);
	g_OlympiadPeriod[dayIndex][1].startHour = GetPrivateProfileInt(section, _T("TuesdayStartEx"), 20, g_ConfigFile);
	g_OlympiadPeriod[dayIndex][1].endHour = GetPrivateProfileInt(section, _T("TuesdayEndEx"), 0, g_ConfigFile);
	g_OlympiadClassFreeFight[dayIndex] = GetPrivateProfileInt(section, _T("TuesdayClassFree"), 1, g_ConfigFile);
	g_OlympiadClassFight[dayIndex] = GetPrivateProfileInt(section, _T("TuesdayClass"), 1, g_ConfigFile);
	dayIndex++;
	g_OlympiadPeriod[dayIndex][0].startHour = GetPrivateProfileInt(section, _T("WednesdayStart"), 20, g_ConfigFile);
	g_OlympiadPeriod[dayIndex][0].endHour = GetPrivateProfileInt(section, _T("WednesdayEnd"), 0, g_ConfigFile);
	g_OlympiadPeriod[dayIndex][1].startHour = GetPrivateProfileInt(section, _T("WednesdayStartEx"), 20, g_ConfigFile);
	g_OlympiadPeriod[dayIndex][1].endHour = GetPrivateProfileInt(section, _T("WednesdayEndEx"), 0, g_ConfigFile);
	g_OlympiadClassFreeFight[dayIndex] = GetPrivateProfileInt(section, _T("WednesdayClassFree"), 1, g_ConfigFile);
	g_OlympiadClassFight[dayIndex] = GetPrivateProfileInt(section, _T("WednesdayClass"), 1, g_ConfigFile);
	dayIndex++;
	g_OlympiadPeriod[dayIndex][0].startHour = GetPrivateProfileInt(section, _T("ThursdayStart"), 20, g_ConfigFile);
	g_OlympiadPeriod[dayIndex][0].endHour = GetPrivateProfileInt(section, _T("ThursdayEnd"), 0, g_ConfigFile);
	g_OlympiadPeriod[dayIndex][1].startHour = GetPrivateProfileInt(section, _T("ThursdayStartEx"), 20, g_ConfigFile);
	g_OlympiadPeriod[dayIndex][1].endHour = GetPrivateProfileInt(section, _T("ThursdayEndEx"), 0, g_ConfigFile);
	g_OlympiadClassFreeFight[dayIndex] = GetPrivateProfileInt(section, _T("ThursdayClassFree"), 1, g_ConfigFile);
	g_OlympiadClassFight[dayIndex] = GetPrivateProfileInt(section, _T("ThursdayClass"), 1, g_ConfigFile);
	dayIndex++;
	g_OlympiadPeriod[dayIndex][0].startHour = GetPrivateProfileInt(section, _T("FridayStart"), 20, g_ConfigFile);
	g_OlympiadPeriod[dayIndex][0].endHour = GetPrivateProfileInt(section, _T("FridayEnd"), 0, g_ConfigFile);
	g_OlympiadPeriod[dayIndex][1].startHour = GetPrivateProfileInt(section, _T("FridayStartEx"), 20, g_ConfigFile);
	g_OlympiadPeriod[dayIndex][1].endHour = GetPrivateProfileInt(section, _T("FridayEndEx"), 0, g_ConfigFile);
	g_OlympiadClassFreeFight[dayIndex] = GetPrivateProfileInt(section, _T("FridayClassFree"), 1, g_ConfigFile);
	g_OlympiadClassFight[dayIndex] = GetPrivateProfileInt(section, _T("FridayClass"), 1, g_ConfigFile);
	dayIndex++;
	g_OlympiadPeriod[dayIndex][0].startHour = GetPrivateProfileInt(section, _T("SaturdayStart"), 20, g_ConfigFile);
	g_OlympiadPeriod[dayIndex][0].endHour = GetPrivateProfileInt(section, _T("SaturdayEnd"), 0, g_ConfigFile);
	g_OlympiadPeriod[dayIndex][1].startHour = GetPrivateProfileInt(section, _T("SaturdayStartEx"), 20, g_ConfigFile);
	g_OlympiadPeriod[dayIndex][1].endHour = GetPrivateProfileInt(section, _T("SaturdayEndEx"), 0, g_ConfigFile);
	g_OlympiadClassFreeFight[dayIndex] = GetPrivateProfileInt(section, _T("SaturdayClassFree"), 1, g_ConfigFile);
	g_OlympiadClassFight[dayIndex] = GetPrivateProfileInt(section, _T("SaturdayClass"), 1, g_ConfigFile);
	dayIndex++;
}

void COlympiadSystem::Initialize()
{
	LoadINI();
	g_OlympiadDB.Init();
	if(g_OlympiadSystemEnabled)
	{
		g_Log.Add(CLog::Blue, "[%s] Initializing - ReloginFix[%d] OverweightFix[%d]", __FUNCTION__, g_OlympiadReloginFix, g_OlympiadOverweightFix);

		for(int n=0;n<7;n++)
		{
			g_Log.Add(CLog::Blue, "[%s] Start[%d][%d] End[%d][%d] Day[%d]", __FUNCTION__, g_OlympiadPeriod[n][0].startHour, g_OlympiadPeriod[n][1].startHour, g_OlympiadPeriod[n][0].endHour, g_OlympiadPeriod[n][1].endHour, n);
		}

		//L2Servr crash fix when user is null
		g_HookManager.WriteJump(0x7016F5, (PVOID)0x701744, 3);
		g_HookManager.WriteJump(0x701837, (PVOID)0x70185A, 3);

		//AntyRelogin
		if(g_OlympiadReloginFix)
		{
			g_HookManager.WriteCall(0x7038B6, AsyncAddEntry, 0);
			g_HookManager.WriteCall(0x7001E9, OlympiadFieldReloginFix2, 0);
			g_HookManager.WriteCall(0x700204, OlympiadFieldReloginFix2, 0);
		}
		//Anty Overweight
		if(g_OlympiadOverweightFix)
		{
			g_HookManager.WriteCall(0x6FE798, OlympiadFieldOverweightFix, 0);
			g_HookManager.WriteCall(0x6FE9C0, OlympiadFieldOverweightFix, 0);
		}
		//Olympiad Stat user command fix
		g_HookManager.WriteCall(0x88FD71, RequestOlympiadStat, 0);
		//Anty game ends in tie
		g_HookManager.WriteCall(0x701F65, OlympiadGameEndsInTie, 0);
		//Change waiting time for match begining 50s - > 60 s
		g_HookManager.WriteMemoryBYTE(0x7009E5, g_OlympiadPrepareToMatchTime);
		g_HookManager.WriteMemoryBYTE(0x700A02, g_OlympiadPrepareToMatchTime);
		g_HookManager.WriteMemoryBYTE(0x700A0A, g_OlympiadPrepareToMatchTime);
		//Change match time 180s->360s
		g_HookManager.WriteMemoryDWORD(0x6FF1A0, g_OlympiadMatchTime);
		//Change user requiements to start match
		g_HookManager.WriteMemoryBYTE(0x7092DB, g_OlympiadClassFreeEntryCount);	//Class Free Entry
		g_HookManager.WriteMemoryBYTE(0x709759, g_OlympiadClassEntryCount);	//Class Entry
		g_HookManager.WriteMemoryBYTE(0x709D9D, g_OlympiadClassEntryCount);	//Class-2 Entry
		//Fix for custom olympiad period - competition time
		g_HookManager.WriteCall(0x0070E190, GetWantedStatus, 0);

		g_HookManager.WriteJump(0x700C32, PointFix1, 4);
		g_HookManager.WriteJump(0x700C4D, PointFix2, 4);

		g_HookManager.WriteCall(0x7034DB, COlympiadSystem::COlympiadObserverEntry_AddEntry, 0);
		g_HookManager.WriteCall(0x7038D0, COlympiadSystem::COlympiadField_AsyncEscapeObserver, 0);
		g_HookManager.NopMemory(0x70D1F7, 5);	//already registered msg

		g_HookManager.WriteCall(0x7016B4, COlympiadSystem::OnSaveOlympiadRecord);
		if(g_SetFixedOlympiadFightPoints)
		{
			g_HookManager.WriteCall(0x700C3F, COlympiadSystem::ReturnDouble1);
			g_HookManager.WriteCall(0x700C5A, COlympiadSystem::ReturnDouble1);
			g_HookManager.WriteCall(0x700C6F, COlympiadSystem::ReturnDouble1);
			g_HookManager.WriteCall(0x700C87, COlympiadSystem::ReturnDouble1);
		}
	}
}

double COlympiadSystem::ReturnDouble1()
{
	return 1.0;
}

bool COlympiadSystem::IsFightEnabled(bool classFree)
{
	tm ti;
	GetTimeInfo(ti);

	if(classFree)
	{
		if(g_OlympiadClassFreeFight[ti.tm_wday] == FALSE)
		{
			return false;
		}
	}else
	{
		if(g_OlympiadClassFight[ti.tm_wday] == FALSE)
		{
			return false;
		}
	}
	return true;
}

void COlympiadSystem::AsyncAddEntry(COlympiadField *pField, BYTE byGameRule, UINT playerId1, UINT playerId2)
{
	typedef void(*f)(COlympiadField*, BYTE, UINT, UINT);
	f(0x6FF240L)(pField, byGameRule, playerId1, playerId2);

	if(pField)
	{
		User *pUser = User::GetUserBySID(&playerId1);
		if(pUser->ValidUser())
		{
			pUser->pSD->nWaitingForMatch = 1;
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Cannot find user[%d]!", __FUNCTION__, playerId1);
		}
		pUser =  User::GetUserBySID(&playerId2);
		if(pUser->ValidUser())
		{
			pUser->pSD->nWaitingForMatch = 1;
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Cannot find user[%d]!", __FUNCTION__, playerId2);
		}
	}
}

bool COlympiadSystem::COlympiadObserverEntry_AddEntry(LPVOID pInstance, int nUserSID)
{
	typedef bool(*f)(LPVOID, int);
	if(f(0x700730L)(pInstance, nUserSID))
	{
		User *pUser = User::GetUserBySID((LPUINT)&nUserSID);
		if(pUser->ValidUser())
		{
			pUser->pSD->nIsObserver = 2;
		//	g_Log.Add(CLog::Blue, "[%s] Olympiad observer", __FUNCTION__);
		}else
			g_Log.Add(CLog::Error, "[%s] Invalid User[%d]", __FUNCTION__, nUserSID);

		return true;
	}
	return false;
}

void COlympiadSystem::COlympiadField_AsyncEscapeObserver(LPVOID pInstance, int nUserSID)
{
	typedef void (*f)(LPVOID, int);
	
	f(0x7020F0L)(pInstance, nUserSID);
	
	User *pUser = User::GetUserBySID((LPUINT)&nUserSID);
	if(pUser->ValidUser())
	{
		pUser->pSD->nIsObserver = 0;
	//	g_Log.Add(CLog::Blue, "[%s] Olympiad observer", __FUNCTION__);
	}
}

void COlympiadSystem::SaveOlympiadRecord(int nWinnerDBID, int nLoserDBID, int nWinnerPoints, int nLoserPoints)
{
	typedef void(*__SOR)(INT64, int, int, int, int);
	__SOR _SOR = (__SOR) 0x00711260;
	_SOR(0x0BC5DBD0, nWinnerDBID, nLoserDBID, nWinnerPoints, nLoserPoints);
}

void COlympiadSystem::OnSaveOlympiadRecord(LPVOID lpInstance, UINT winnerId, UINT looserId, int winnerPoint, int looserPoint)
{
	guard;

	try
	{
		if(g_OlympiadWinnerRewardId > 0 && g_OlympiadWinnerRewardAmount > 0)
		{
			if(User *pUser = g_UserDB.GetUserByDBID(winnerId))
			{
				if(pUser->IsNowTrade())
				{
					pUser->TradeCancel();
				}
				pUser->AddItemToInventory(g_OlympiadWinnerRewardId, g_OlympiadWinnerRewardAmount);
			}
		}
		if(g_OlympiadLooserRewardId > 0 && g_OlympiadLooserRewardAmount > 0)
		{
			if(User *pUser = g_UserDB.GetUserByDBID(looserId))
			{
				if(pUser->IsNowTrade())
				{
					pUser->TradeCancel();
				}
				pUser->AddItemToInventory(g_OlympiadLooserRewardId, g_OlympiadLooserRewardAmount);
			}
		}
	}catch(...)
	{
		EXCEPTION_LOG;
	}

	typedef void(*f)(LPVOID, UINT, UINT, int, int);
	f(0x00711260L)(lpInstance, winnerId, looserId, winnerPoint, looserPoint);
	unguard;
}

void COlympiadSystem::OlympiadGameEndsInTie(COlympiadField *pOlympiadField, const char *format, BYTE bOpCode, int nMsgID, int nParam)
{
	typedef void (*f)(COlympiadField*, const char*, BYTE, int, int);
	f(0x7005E0L)(pOlympiadField, format, bOpCode, nMsgID, nParam);
	
	try
	{
		User *pUser = g_UserDB.GetUserByDBID(pOlympiadField->nPlayer1DBID);
		User *pUser2 = g_UserDB.GetUserByDBID(pOlympiadField->nPlayer2DBID);
		if(pUser->ValidUser() && pUser2->ValidUser())
		{
			CSharedNoblessInfo *pNoblesInfo = pUser->GetNoblessInfo();
			CSharedNoblessInfo *pNoblesInfo2 = pUser2->GetNoblessInfo();
			if(pNoblesInfo && pNoblesInfo2)
			{
				int nPoints = pNoblesInfo->nOlympiadPoints;
				int nPoints2 = pNoblesInfo2->nOlympiadPoints;
				nPoints /= 3;
				nPoints2 /= 3;
				if(nPoints < 1) nPoints = 1;
				if(nPoints2 < 1) nPoints2 = 1;
				pNoblesInfo->nOlympiadPoints = pNoblesInfo->nOlympiadPoints - nPoints;
				pNoblesInfo2->nOlympiadPoints = pNoblesInfo2->nOlympiadPoints - nPoints2;
				SaveOlympiadRecord(0, pUser->nDBID, 0, -nPoints);
				SaveOlympiadRecord(0, pUser2->nDBID, 0, -nPoints2);
				pUser->GetSocket()->Send("cdddSdd", 0x64, 1658, 2, 0, pUser->pSD->wszName, 1, nPoints);
				pUser2->GetSocket()->Send("cdddSdd", 0x64, 1658, 2, 0, pUser2->pSD->wszName, 1, nPoints2);
			}
		}
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception Detected", __FUNCTION__);
	}
}

void COlympiadSystem::OlympiadFieldOverweightFix(CUserSocket *pSocket, int nMsgID)
{
	pSocket->SendSystemMessage(nMsgID);
	User *pUser = pSocket->GetUser();
	if(pUser->ValidUser())
	{
		try
		{
			CSharedNoblessInfo *pNoblesInfo = pUser->GetNoblessInfo();
			if(pNoblesInfo)
			{
				int nPoints = pNoblesInfo->nOlympiadPoints;
				nPoints /= 3;
				if(nPoints < 1)
					nPoints = 1;
				pNoblesInfo->nOlympiadPoints = pNoblesInfo->nOlympiadPoints - nPoints;
				SaveOlympiadRecord(0, pUser->nDBID, 0, -nPoints);
				pSocket->Send("cdddSdd", 0x64, 1658, 2, 0, pUser->pSD->wszName, 1, nPoints);
			}
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception Detected", __FUNCTION__);
		}
	}
}

void COlympiadSystem::OlympiadFieldOnGameExit(User *pUser)
{
	if(pUser->pSD->nWaitingForMatch == 1)
	{
		//do the job
		int nUserDBID = pUser->nDBID;
		if(COlympiadField *pOF = pUser->olympiadUser.GetField())
		{
			LPCSharedNoblessInfo pNI = CNoblessSystem::GetInfoByCharId(pUser->nDBID);
			if(pNI)
			{
				int nPoints = pNI->nOlympiadPoints;
				nPoints /= 5;
				if(nPoints < 1)
					nPoints = 1;
				pNI->nOlympiadPoints -= nPoints;
				pNI->nOlympiadLoseCount += 1;
				pNI->nOlympiadMatchCount += 1;
				SaveOlympiadRecord(0, pUser->nDBID, 0, (-nPoints));
				g_Log.Add(CLog::Error, "[%s] User[%S] DBID[%d] left game during countdown - lost[%d] points.", __FUNCTION__, pUser->pSD->wszName, pUser->nDBID, nPoints);
			}
		}
	}
}

void COlympiadSystem::OlympiadFieldReloginFix2(CUserSocket *pSocket, int nMsgID)
{
	pSocket->SendSystemMessage(nMsgID);
	User *pUser = pSocket->GetUser();
	if(pUser->ValidUser())
	{
		pUser->pSD->nWaitingForMatch = 0;
	}
}
bool COlympiadSystem::GetWantedStatus(LPVOID pOlympiadSys, DWORD nSeasonStartTime, COlympiadStatus *pOS)
{
	typedef bool (*_gws)(void*, DWORD, COlympiadStatus*);
	_gws gws = (_gws) 0x008CA3D0;
	
	bool bRet = gws(pOlympiadSys, nSeasonStartTime, pOS);
	tm ti;
	GetTimeInfo(ti);


	//check if it's seal validation period
	int StartHour = 20;
	int EndHour = 0;

	if(SSQPart::none == SSQSystem::GetInstance()->GetWinner())
	{
		//event period or none
		StartHour = g_OlympiadPeriod[ti.tm_wday][0].startHour;
		EndHour = g_OlympiadPeriod[ti.tm_wday][0].endHour;
	}else
	{
		StartHour = g_OlympiadPeriod[ti.tm_wday][1].startHour;
		EndHour = g_OlympiadPeriod[ti.tm_wday][1].endHour;
	}

	pOS->EnableMatches = true;

	if(EndHour < StartHour)
	{
		if(ti.tm_hour >= StartHour)
		{
			pOS->EnableMatches = true;
			return bRet;
		}
		if(ti.tm_hour >= EndHour)
		{
			pOS->EnableMatches = false;
			return bRet;
		}
	}

	if(ti.tm_hour < StartHour)
	{
		//Disable oly
		pOS->EnableMatches = false;
		return bRet;
	}

	if(ti.tm_hour >= EndHour)
	{
		//Disable oly
		pOS->EnableMatches = false;
		return bRet;
	}

	return bRet;
}

void COlympiadSystem::RequestOlympiadStat(CUserSocket *pSocket, const char *format, ...)
{
	User *pUser = pSocket->GetUser();
	if(pUser->ValidUser())
	{
		CSharedNoblessInfo *pInfo = pUser->GetNoblessInfo();
		if(pInfo)
		{
			CSystemMessage msg(SystemMessage::YOUR_CURRENT_RECORD_FOR_THIS_GRAND_OLYMPIAD_IS_$S1_MATCHXESX__$S2_WINXSX_AND_$S3_DEFEATXSX__YOU_HAVE_EARNED_$S4_OLYMPIAD_POINTXSX_);
			msg.AddNumber(pInfo->nOlympiadMatchCount);
			msg.AddNumber(pInfo->nOlympiadWinCount);
			msg.AddNumber(pInfo->nOlympiadLoseCount);
			msg.AddNumber(pInfo->nOlympiadPoints);
			pSocket->Send("b", msg.GetSize(), msg.GetBuff());
		}
	}
}

void COlympiadSystem::ValidateEquippedItems(User *pUser)
{
	guard;
	if(pUser->ValidUser())
	{
		try
		{
			for(int n=0;n<18;n++)
			{
				int nItemIndex = pUser->pSD->nEquipment[n];
				if(nItemIndex > 0)
				{
					CSharedItemData *pSID = CSharedItemData::FromIndex(nItemIndex);
					if(pSID)
					{
						bool cont = true;
						for(list<int>::iterator Iter = g_OlympiadForbiddenItemList.begin(); Iter!=g_OlympiadForbiddenItemList.end();Iter++)
						{
							if((*Iter) == pSID->nItemID)
							{
								pUser->UnEquipItem(n);
								cont = false;
								break;
							}
						}
						if(CItemInfo *pII = pSID->GetItemInfo())
						{
							for(UINT m=0;m<g_OlympiadForbiddenItemGrade.size();m++)
							{
								if(pII->nCrystalType == g_OlympiadForbiddenItemGrade[m])
								{
									pUser->UnEquipItem(n);
									cont = false;
									break;
								}
							}
						}
						if(cont)
						{
							if(g_OlympiadMaxAllowedWeaponEnchant > 0 && pSID->nItemType == 0)
							{
								if(g_OlympiadMaxAllowedWeaponEnchant < (int)pSID->nEnchantLevel)
								{
									pUser->UnEquipItem(n);
									cont = false;
								}
							}
						}
						if(cont)
						{
							if(g_OlympiadMaxAllowedArmorEnchant > 0 && (pSID->nItemType == 1 || pSID->nItemType == 2))
							{
								if(g_OlympiadMaxAllowedArmorEnchant < (int)pSID->nEnchantLevel)
								{
									pUser->UnEquipItem(n);
									cont = false;
								}
							}
						}
					}
				}
			}
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception!", __FUNCTION__);
		}
	}
	unguard;
}

COlympiadDB::COlympiadDB()
{
}

COlympiadDB::~COlympiadDB()
{

}

void COlympiadDB::Init()
{
	LoadForbiddenSkill();
	g_Log.Add(CLog::Blue, "[%s] Loaded [%d] forbidden skill(s).", __FUNCTION__, m_ForbiddenSkill.size());
	LoadForbiddenItem();
	g_Log.Add(CLog::Blue, "[%s] Loaded [%d] forbidden item(s).", __FUNCTION__, m_ForbiddenItem.size());
	LoadBuffSupport();
	if(!m_BuffSupport.empty())
	{
		g_Log.Add(CLog::Blue, "[%s] Loaded [%d] olympiad support buff definition(s).", __FUNCTION__, m_BuffSupport.size());
	}
}

void COlympiadDB::LoadForbiddenSkill()
{
	m_ForbiddenSkill.clear();
	wstring file = ReadFileW(L"..\\Script\\Setting.txt");
	if(file.size() > 0)
	{
		file = CParser::Trim(file);
		size_t pos = file.find(L"forbidden_skill=");
		if(pos != wstring::npos)
		{
			size_t start = file.find(L"{", pos);
			if(start!=wstring::npos)
			{
				start++;
				size_t endPos = file.find(L"}", start);
				if(endPos != wstring::npos)
				{
					wstring skills = file.substr(start, (endPos - start));
					vector<wstring> cols = Parser::Split(skills, L";[]");
					for(UINT n=0;n<cols.size();n++)
					{
						if(cols[n].size() > 0)
						{
							INT32 skillId = 0, level = 0;
							g_SkillDB.GetSkillIdLevelByName(cols[n].c_str(), &skillId, &level);
							if(skillId > 0 && level > 0)
							{
								SkillPchId pchId(skillId, level);
								m_ForbiddenSkill.push_back(pchId);
							}else
							{
								g_Log.Add(CLog::Error, "[%s] Invalid skill[%S]!", __FUNCTION__, cols[n].c_str());
							}
						}
					}
				}
			}
		}
	}
}

void COlympiadDB::LoadForbiddenItem()
{
	m_ForbiddenItem.clear();
	wstring file = ReadFileW(L"..\\Script\\Setting.txt");
	if(file.size() > 0)
	{
		file = CParser::Trim(file);
		size_t pos = file.find(L"forbidden_item=");
		if(pos != wstring::npos)
		{
			size_t start = file.find(L"{", pos);
			if(start!=wstring::npos)
			{
				start++;
				size_t endPos = file.find(L"}", start);
				if(endPos != wstring::npos)
				{
					wstring itemStr = file.substr(start, (endPos - start));
					vector<wstring> cols = Parser::Split(itemStr, L";[]");
					for(UINT n=0;n<cols.size();n++)
					{
						if(cols[n].size() > 0)
						{
							int itemId = g_ObjectDB.GetClassIdFromName(cols[n].c_str());
							if(itemId > 0)
							{
								m_ForbiddenItem.push_back(itemId);
							}else
							{
								g_Log.Add(CLog::Error, "[%s] Invalid item[%S]!", __FUNCTION__, cols[n].c_str());
							}
						}
					}
				}else
				{
					g_Log.Add(CLog::Error, "[%s] Cannot find end of forbidden_item list!", __FUNCTION__);
				}
			}
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Cannot find forbidden_item list in ..\\Script\\Settting.txt !", __FUNCTION__);
		}
	}else
	{
		g_Log.Add(CLog::Error, "[%s] Cannot load ..\\Script\\Settting.txt !", __FUNCTION__);
	}
}

void COlympiadDB::LoadBuffSupport()
{
	LPBYTE lpFile = 0;
	m_BuffSupport.clear();
	UINT len = ReadFileBinary(L"..\\Script\\OlympiadSupport.txt", lpFile);
	if(lpFile)
	{
		if(len > 2)
		{
			bool enabled = false;
			wstringstream fileStream((PWCHAR)&lpFile[2]);
			wstring line;
			while(getline(fileStream, line))
			{
				if(line[0] == L'/' || line[0] == L';')
					continue;

				if(line.find(L"setting_begin") == 0)
				{
					enabled = Parser::ParseOptionInt(line , L"enabled", 0) ? true : false;
				}else if(line.find(L"support_begin") == 0)
				{
					if(enabled)
					{
						OlympiadBuffSupport osb;
						//support_begin class_id_1={95;96;97}	class_id_2={88;89;90} buff_class_1={s_wind_walk2;s_shield3;s_resist_shock4} buff_class_2={s_wind_walk2;s_mental_shield4;s_agit_magic_barrier2} support_end
						wstring class1 = Parser::ParseOptionString(line, L"class_id_1");
						{
							class1 = Parser::Replace(class1, L'{', L' ');
							class1 = Parser::Replace(class1, L';', L' ');
							class1 = Parser::Replace(class1, L'}', L' ');
							wstringstream classStream(class1);
							UINT classId = 0;
							while(classStream >> classId)
							{
								osb.classI.push_back(classId);
							}
						}
						wstring class2 = Parser::ParseOptionString(line, L"class_id_2");
						{
							class2 = Parser::Replace(class2, L'{', L' ');
							class2 = Parser::Replace(class2, L';', L' ');
							class2 = Parser::Replace(class2, L'}', L' ');
							wstringstream classStream(class2);
							UINT classId = 0;
							while(classStream >> classId)
							{
								osb.classII.push_back(classId);
							}
						}
						wstring buff1 = Parser::ParseOptionString(line, L"buff_class_1");
						{
							buff1 = Parser::Replace(buff1, L'{', L' ');
							buff1 = Parser::Replace(buff1, L';', L' ');
							buff1 = Parser::Replace(buff1, L'}', L' ');
							wstringstream buffStream(buff1);
							wstring skillName;
							while(buffStream >> skillName)
							{
								INT32 skillId = 0, skillLevel = 0;
								g_SkillDB.GetSkillIdLevelByName(skillName.c_str(), &skillId, &skillLevel);
								if(skillId > 0 && skillLevel > 0)
								{
									osb.buffsI.push_back(SkillPchId(skillId, skillLevel));
								}else
								{
									g_Log.Add(CLog::Error, "[%s] Invalid skillName[%S]!", __FUNCTION__, skillName.c_str());
								}
							}
						}
						wstring buff2 = Parser::ParseOptionString(line, L"buff_class_2");
						{
							buff2 = Parser::Replace(buff2, L'{', L' ');
							buff2 = Parser::Replace(buff2, L';', L' ');
							buff2 = Parser::Replace(buff2, L'}', L' ');
							wstringstream buffStream(buff2);
							wstring skillName;
							while(buffStream >> skillName)
							{
								INT32 skillId = 0, skillLevel = 0;
								g_SkillDB.GetSkillIdLevelByName(skillName.c_str(), &skillId, &skillLevel);
								if(skillId > 0 && skillLevel > 0)
								{
									osb.buffsII.push_back(SkillPchId(skillId, skillLevel));
								}else
								{
									g_Log.Add(CLog::Error, "[%s] Invalid skillName[%S]!", __FUNCTION__, skillName.c_str());
								}
							}
						}
						if(!osb.classI.empty() && !osb.classII.empty())
						{
							m_BuffSupport.push_back(osb);
						}
					}
				}
			}
		}
		delete [] lpFile;
	}

}

bool COlympiadDB::IsForbiddenSkill(int skillId, int level)
{
	SkillPchId id(skillId, level);
	for(UINT n=0;n<m_ForbiddenSkill.size();n++)
	{
		if(m_ForbiddenSkill[n] == id)
		{
			return true;
		}
	}
	return false;
}

bool COlympiadDB::IsForbiddenItem(int itemId)
{
	for(UINT n=0;n<m_ForbiddenItem.size();n++)
	{
		if(m_ForbiddenItem[n] == itemId)
		{
			return true;
		}
	}
	return false;
}

vector<SkillPchId>& COlympiadDB::GetBuffSupport(UINT baseClass, UINT enemyClass)
{
	static vector<SkillPchId> m_EmptyBuffs;
	
	if(!m_BuffSupport.empty())
	{
		bool isMage = false;
		bool isFighter = false;
		if(CCategoryDataDB::IsInCategory(baseClass, CCategoryDataDB::MAGE_GROUP))
		{
			isMage = true;
		}else
		{
			isFighter = true;
		}

		bool isEnemyMage = false;
		bool isEnemyFighter = false;
		if(CCategoryDataDB::IsInCategory(enemyClass, CCategoryDataDB::MAGE_GROUP))
		{
			isEnemyMage = true;
		}else
		{
			isEnemyFighter = true;
		}

		
		for(UINT n=0;n<m_BuffSupport.size();n++)
		{
			OlympiadBuffSupport& obs = m_BuffSupport[n];
			for(UINT m=0;m<obs.classI.size();m++)
			{
				if( obs.classI[m] == baseClass || (isFighter && obs.classI[m] == 0) || (isMage && obs.classI[m] == 1) )
				{
					//check second class
					for(UINT o = 0; o < obs.classII.size(); o++)
					{
						if( obs.classII[o] == enemyClass || (isEnemyFighter && obs.classII[o] == 0) || (isEnemyMage && obs.classII[o] == 1) )
						{
							return obs.buffsI;
						}
					}
				}
			}
			for(UINT m=0;m<obs.classII.size();m++)
			{
				if( obs.classII[m] == baseClass || (isFighter && obs.classII[m] == 0) || (isMage && obs.classII[m] == 1) )
				{
					//check second class
					for(UINT o = 0; o < obs.classI.size(); o++)
					{
						if( obs.classI[o] == enemyClass || (isEnemyFighter && obs.classI[o] == 0) || (isEnemyMage && obs.classI[o] == 1) )
						{
							return obs.buffsII;
						}
					}
				}
			}
		}
	}
	return m_EmptyBuffs;
}