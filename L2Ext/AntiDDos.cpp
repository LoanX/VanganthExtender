#include "StdAfx.h"
#include "AntiDDoS.h"
#include "UserDB.h"
#include <Ws2tcpip.h>

CAntiDDoS g_AntiDDoS;
BOOL loginEnabled = TRUE;

CAntiDDoS::CAntiDDoS() : announceToPlayers(TRUE), disableLogin(TRUE), kickPlayers(TRUE), useProtectionSkill(TRUE), underAttack(FALSE), performedAction(FALSE), startTick(0), checkTimeout(15000), skillId(10027), milisecondPingLimit(300), safeTickCount(0), enabled(FALSE)
{
	InitializeCriticalSection(&_lock);
}

CAntiDDoS::~CAntiDDoS()
{
	DeleteCriticalSection(&_lock);
}

void CAntiDDoS::LoadINI()
{
	lUrl.clear();
	const TCHAR* sectionName = _T("AntiDDoS");
	enabled = GetPrivateProfileInt(sectionName, _T("Enabled"), 0, g_ConfigFile);
	milisecondPingLimit = GetPrivateProfileInt(sectionName, _T("MaxAllowedPing"), 200, g_ConfigFile);
	TCHAR tmp[8192];
	GetPrivateProfileString(sectionName, _T("Url"), NULL, tmp, 8192, g_ConfigFile);
	tstringstream sstr;
	sstr << tmp;
	tstring url;
	while(sstr >> url)
	{
		string sUrl = WStringToString(url);
		lUrl.push_back(sUrl);
	}
	announceToPlayers = GetPrivateProfileInt(sectionName, _T("Announce"), 1, g_ConfigFile);
	skillId = GetPrivateProfileInt(sectionName, _T("SkillID"), 0, g_ConfigFile);
	useProtectionSkill = GetPrivateProfileInt(sectionName, _T("UseSkill"), 0, g_ConfigFile);
	kickPlayers = GetPrivateProfileInt(sectionName, _T("KickAll"), 1, g_ConfigFile);
	disableLogin = GetPrivateProfileInt(sectionName, _T("DisableLogin"), 1, g_ConfigFile);
	checkTimeout = GetPrivateProfileInt(sectionName, _T("ValidateTimestamp"), 15000, g_ConfigFile);
	GetPrivateProfileString(sectionName, _T("AnnounceMsgStart"), NULL, tmp, 8192, g_ConfigFile);
	msgStart = tmp;
	GetPrivateProfileString(sectionName, _T("AnnounceMsgEnd"), NULL, tmp, 8192, g_ConfigFile);
	msgEnd = tmp;
}

ULONG CAntiDDoS::Ping(const char* AddressIP, int timeout)
{
	unsigned long ipAddr = 0;
	struct addrinfo *result = NULL; 
	getaddrinfo(AddressIP, NULL, NULL, &result);

	if(result == NULL)
	{
		g_Log.Add(CLog::Error, "[%s] Cannot get ip address from %s, err(%d)!", __FUNCTION__, AddressIP, WSAGetLastError());
		return -1;
	}
	
	sockaddr_in *addr = (sockaddr_in *)result->ai_addr;

	ipAddr = addr->sin_addr.S_un.S_addr;

    HANDLE hIP = IcmpCreateFile();
    if( hIP == INVALID_HANDLE_VALUE )
	{
		return -1;
	}
 
    BYTE pingBuffer[32];
    memset( pingBuffer, '\xAA', sizeof( pingBuffer ) );

	BYTE replyBuffer[512];

	ULONG ping = -1;
	DWORD dwStatus = IcmpSendEcho( hIP, ipAddr , pingBuffer, sizeof(pingBuffer), NULL, replyBuffer, sizeof(replyBuffer), timeout );
	if( dwStatus != 0 )
	{
		PICMP_ECHO_REPLY32 pEchoReply = (PICMP_ECHO_REPLY32)replyBuffer;
		ping = pEchoReply->RoundTripTime;
	}
	else
	{
		int err = GetLastError();
		if(err == IP_REQ_TIMED_OUT)
		{
			//check again
			dwStatus = IcmpSendEcho( hIP, ipAddr , pingBuffer, sizeof(pingBuffer), NULL, replyBuffer, sizeof(replyBuffer), timeout );
			if(dwStatus != 0)
			{
				PICMP_ECHO_REPLY32 pEchoReply = (PICMP_ECHO_REPLY32)replyBuffer;
				ping = pEchoReply->RoundTripTime;
			}else
			{
				err = GetLastError();
				if(err == IP_REQ_TIMED_OUT)
				{
					g_Log.Add(CLog::Error, "[%s] ping > timeout(%d ms) for %s - %s!", __FUNCTION__, timeout, AddressIP, inet_ntoa(addr->sin_addr));
				}
			}
		}else
		{
			//show log
			g_Log.Add(CLog::Error, "[%s] IcmpSendEcho failure : %d for %s !", __FUNCTION__, err, AddressIP);
		}

	}

    IcmpCloseHandle(hIP);
    return ping;
}

bool CAntiDDoS::IsWorldReachable()
{
	bool reachable = false;
	for(list<string>::iterator Iter = lUrl.begin();Iter!=lUrl.end();Iter++)
	{
		UINT ms = Ping(Iter->c_str(), milisecondPingLimit);
		if(ms != -1)
		{
			return true;
		}
	}

	return reachable;
}

BOOL CAntiDDoS::IsUnderAttack()
{
	BOOL ret = FALSE;
	Lock();
	ret = underAttack;
	Unlock();
	return ret;
}

void CAntiDDoS::SetUnderAttack(BOOL value)
{
	Lock();
	underAttack = value;
	Unlock();
}

void CAntiDDoS::ValidateState()
{
	threadHandle = CreateThread(NULL, 0, CAntiDDoS::ValidateNetworkThread, NULL, 0, &threadId);
	if(threadHandle)
	{
		startTick = GetTickCount();
	}
}

DWORD WINAPI CAntiDDoS::ValidateNetworkThread(LPVOID param)
{
	if(!g_AntiDDoS.IsWorldReachable())
	{
		g_AntiDDoS.safeTickCount = 0;
		g_AntiDDoS.SetUnderAttack(TRUE);
	}else
	{
		g_AntiDDoS.safeTickCount++;
		g_AntiDDoS.SetUnderAttack(FALSE);
	}
	return 0;
}

void CAntiDDoS::TimerExpired()
{
	guard;
	if(enabled)
	{
		Lock();
		if((startTick + checkTimeout) < GetTickCount())
		{
			ValidateState();
		}

		if(performedAction && safeTickCount == 5)
		{
			//ddos ended
			PerformAction(FALSE);
		}

		if(underAttack)
		{
			if(!performedAction)
			{
				PerformAction(TRUE);
			}
		}
		Unlock();
	}
	unguard;
}

void ApplyProtectionSkill(User *pUser)
{
	if(pUser->ValidUser())
	{
		pUser->AcquireSkill(g_AntiDDoS.GetSkillId(), 1);
		pUser->ValidateParameters();
	}
}

void RemoveProtectionSkill(User *pUser)
{
	if(pUser->ValidUser())
	{
		pUser->DeleteSkill(g_AntiDDoS.GetSkillId());
		pUser->ValidateParameters();
	}
}

void KickFromGame(User *pUser)
{
	if(pUser->ValidUser())
	{
		pUser->pSocket->Close();
	}
}

void CAntiDDoS::PerformAction(BOOL start)
{
	if(start)
	{
		if(disableLogin)
			loginEnabled = FALSE;

		performedAction = TRUE;

		if(announceToPlayers)
		{
			CHAR pck[8190];
			int nSize = Assemble(pck, 8190, "cddSS", 0x4A, 0, 18, L"Administration", msgStart.c_str());
			L2Server::BroadcastToAllUser(nSize, pck);
		}

		if(useProtectionSkill)
		{
			FOR_ALL_USERS_IN_MAP(ApplyProtectionSkill);
		}

		if(kickPlayers)
		{
			FOR_ALL_USERS_IN_MAP(KickFromGame);
		}
	}else
	{
		if(disableLogin)
			loginEnabled = TRUE;

		performedAction = FALSE;

		if(announceToPlayers)
		{
			CHAR pck[8190];
			int nSize = Assemble(pck, 8190, "cddSS", 0x4A, 0, 18, L"Administration", msgEnd.c_str());
			L2Server::BroadcastToAllUser(nSize, pck);
		}

		if(useProtectionSkill)
		{
			FOR_ALL_USERS_IN_MAP(RemoveProtectionSkill);
		}
	}
}