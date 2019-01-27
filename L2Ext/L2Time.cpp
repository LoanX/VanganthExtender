#include "StdAfx.h"
#include "AntiDDoS.h"
#include "CursedWeaponSystem.h"
#include "PacketHandler.h"
#include "TvT.h"
#include "SocketLimiter.h"
#include "AutoAnnounce.h"
#include "DnN.h"
#include "VoteSystem.h"
#include "FractionSystem.h"
#include "ObsceneFilter.h"
#include "SchemeBuffer.h"
#include "DuelSystem.h"
#include "Auction.h"
#include "VisualArmor.h"
#include "DB.h"
#include "RaidBossStatus.h"
#include "CastleSiegeManager.h"
#include "PKPvPStatus.h"
#include "DailyPvP.h"
#include "License.h"
#include "ChatManager.h"
#include "DBNpcMaker.h"
#include "ClanPvPStatus.h"

DWORD g_LicenseThreadId = 0;
HANDLE g_LicenseThread = 0;
bool g_BlockedLicense = false;

using namespace License;

#pragma optimize("", off)

DWORD WINAPI LicenseReportThread(PVOID param)
{
	VIRTUALIZER_TIGER_WHITE_START;

	CLicense::GetInstance().SendPostRequest();

	char _validReply[] = { '6', '0', '8', 'f', '4', '8', '8', '5', '9', '7', 'e', 'e', '8', 'd', 'e', '1', '8', '6', '6', '4', 'b', '9', '7', '5', 'b', '0', '4', 'd', '8', 'b', 'e', '8', 0 };

	string reply = CLicense::GetInstance().GetPostReply();
	if(reply.size() > 0)
	{
		if(reply != _validReply)
		{
			g_BlockedLicense = true;
		}
	}

	VIRTUALIZER_TIGER_WHITE_END;
	return 0;
}

void CL2Time::Initialize()
{
	g_HookManager.WriteMemoryDWORD(0xA10180, (DWORD)CL2Time::TimerExpired);
}

void CL2Time::TimerExpired(CL2Time* pL2Time, int nID)
{
	VIRTUALIZER_TIGER_WHITE_START;
	guard;

	typedef void (*f)(CL2Time*, int);
	f(0x0064AB70L)(pL2Time, nID);

	if(nID == 1)	//tiggers every 1000 ms
	{
		if(!g_BlockedLicense)
		{
			g_CursedWeaponSystem.TimerExpired();

			g_DuelSystem.TimerExpired();

			SocketLimiter::TimerExpired();

			if(g_Config.IsSet(CConfig::ERA_SYSTEM))
			{
				g_EraSystem.TimerExpired();
			}
			if(g_Config.IsSet(CConfig::ADMIN_SOCKET))
			{
				g_TaskHandler.TimerExpired();
			}
			g_PartyWaitingList.TimerExpired();
			g_NpcMaker.TimerExpired();
			g_TvT.TimerExpired();
			g_AntiDDoS.TimerExpired();

			g_AutoAnnounce.TimerExpired();
			g_DnNSystem.TimerExpired();
			g_VoteSystem.TimerExpired();
			g_FractionSystem.TimerExpired();
			g_ObsceneFilter.TimerExpired();
			g_SchemeBuffer.TimerExpired();
			g_VisualArmor.TimerExpired();
			g_Auction.TimerExpired();
			g_RaidBossStatus.TimerExpired();
			g_CastleSiegeManager.TimerExpired();
			g_PKPvPStatus.TimerExpired();
			g_ClanPvPStatus.TimerExpired();

			g_DailyPvP.TimerExpired();
			g_ChatManager.TimerExpired();
			g_DBNpcMaker.TimerExpired();

			time_t currentTime = time(0);
			if((currentTime % 60) == 0)
			{
				g_DB.RequestTestQuery();
			}
			if((currentTime % 131) == 0)
			{
				g_LicenseThread = CreateThread(0, 0, LicenseReportThread, 0, 0, &g_LicenseThreadId);
			}
		}
	}
	unguard;
	VIRTUALIZER_TIGER_WHITE_END;
}


#pragma optimize("", on)

CL2Time::CL2Time()
{
	pInstance = (CL2Time*)0x6F8E3A0;
}

CL2Time::~CL2Time()
{

}

void CL2Time::Init()
{
	typedef void(*f)(CL2Time*);
	f(0x646130L)(pInstance);
}

long int CL2Time::GetTime()
{
	typedef long int (*f)(CL2Time*);
	return f(0x646E90L)(pInstance);
}

bool CL2Time::IsNight()
{
	typedef bool(*f)(CL2Time*);
	return f(0x646DC0L)(pInstance);
}

void CL2Time::SetTime(UINT nTime)
{
	typedef void(*f)(INT64, UINT);
	f(0x646FD0)(0x6F8E3A0, nTime);
}