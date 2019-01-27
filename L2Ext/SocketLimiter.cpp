/*
################SOCKET##LIMITER##################
## This module will protect l2server, blocking ##
## ugly bug with usersockets, opening xxk and  ##
## using all available IOBufferPool. It will   ##
## also auto ban ip, if there will be more than##
## x connection from this ip at once.          ##
#################################################
*/
#include "StdAfx.h"
#include "SocketLimiter.h"
#include "SocketDB.h"
#include "IpList.h"
#include "IpBlocker.h"

LPINT lpUsersOnline;
LPINT lpUserSockets;
LPINT lpPendingWriteLimit;
static int nSocketLimitPerIP;
static int nBanDuration; //in seconds
static UINT g_IOReadPerSecond = UINT_MAX;
UINT g_SocketReleaseTime = 25000;
UINT g_SocketPreReleaseTime = 5000;
int SocketLimiter::nLastCheck;

CUserSocket* g_ThreadUserSocket[32];

/*
[SocketLimiter]
Enabled=1
;ip separated by space ex. 127.0.0.1 192.168.0.1 192.168.0.2
SafeIpList=127.0.0.1
;Limit sockets per ip (default 10)
LimitPerIp=10
;Ban duration after reaching socket ip limit in seconds (default 300)
BanDuration=300
;Pending write limit (amount of data that will be sent to user in bytes Default l2server: 2097152)
PendingWriteLimit=1097152
PendingWriteDuringSiege=2097152
*/
void SocketLimiter::Initialize()
{
	g_Log.Add(CLog::Blue, "[Ext] Initializing Socket Limiter");
	nLastCheck = time(NULL);
	lpUsersOnline = (LPINT)0x1557E0C; //dVamp exe : 0x1558E10
	lpUserSockets = (LPINT)0x1557E08;	//dVamp exe : 0x1558E08
	lpPendingWriteLimit = (LPINT)0x0086C19C;
	
	const TCHAR* section = _T("SocketLimiter");

	if(!GetPrivateProfileInt(section, _T("Enabled"), 0, g_ConfigFile))
	{
		g_Log.Add(CLog::Blue, "[%s] Disabled", __FUNCTION__);
		return;
	}

	for(UINT n=0;n<32;n++)
	{
		g_ThreadUserSocket[n] = 0;
	}
	//Set Config Data
	nSocketLimitPerIP = GetPrivateProfileInt(section, _T("LimitPerIp"), 20, g_ConfigFile);
	nBanDuration = GetPrivateProfileInt(section, _T("BanDuration"), 300, g_ConfigFile);
	g_IOReadPerSecond = GetPrivateProfileInt(section, _T("MaxIOReadPerSecond"), UINT_MAX, g_ConfigFile);
	g_SocketReleaseTime = GetPrivateProfileInt(section, _T("SocketReleaseTime"), 25000, g_ConfigFile);
	g_SocketPreReleaseTime = GetPrivateProfileInt(section, _T("SocketPreReleaseTime"), 500, g_ConfigFile);
	//(*pPendingWriteLimit) = g_Config.SockLimiterInfo.GetPendingWriteLimit(); //2097152 org l2server limit
	g_HookManager.WriteMemoryDWORD(0x86c19c, GetPrivateProfileInt(section, _T("PendingWriteLimit"), 2097152, g_ConfigFile));
	g_HookManager.WriteMemoryDWORD(0x890B12, g_SocketReleaseTime);
	g_HookManager.WriteMemoryDWORD(0x890A5E, g_SocketPreReleaseTime);
	

	TCHAR temp[8190];
	if(GetPrivateProfileString(section, _T("SafeIpList"), 0, temp, sizeof(temp), g_ConfigFile))
	{
		wstringstream sstr;
		sstr << temp;
		wstring sVal;
		while(sstr >> sVal)
		{
			CIPAddress ip(sVal);
			g_Log.Add(CLog::Blue, "[%s] SafeIpList: %S [%d]", __FUNCTION__, ip.ToString().c_str(), g_SafeIpList.GetSize());
			g_SafeIpList.AddIp(ip);
		}
	}
	
	//Set Hooks
	g_HookManager.WriteCall(0x620EEF, AcceptConnection, 1);

	//CSocket::SendV hook
	g_HookManager.WriteCall(0x005AB493, ValidateSend, 0); //CEventMatch::Broadcast
	g_HookManager.WriteCall(0x005AB65D, ValidateSend, 0); //CEventMatch::Broadcast
	g_HookManager.WriteCall(0x005ABA6D, ValidateSend, 0); //CEventMatch::Broadcast2Observer
	g_HookManager.WriteCall(0x006FF983, ValidateSend, 0); //COlympiadField::SendVToPlayers
	g_HookManager.WriteCall(0x006FF9B5, ValidateSend, 0); //COlympiadField::SendVToPlayers
	g_HookManager.WriteCall(0x006FFE36, ValidateSend, 0); //COlympiadObserverEntry::Broadcast
	g_HookManager.WriteCall(0x0071D7C6, ValidateSend, 0); //CParty::BroadcastToAllParty
	g_HookManager.WriteCall(0x0071D9E1, ValidateSend, 0); //CParty::BroadcastToAllPartyV
	g_HookManager.WriteCall(0x007D5FCA, ValidateSend, 0); //CSocket::Send

	g_HookManager.WriteMemoryDWORD(0xA8EF38, (DWORD)SocketLimiter::OnRead);
	g_HookManager.WriteCall(0x8774D5, SocketLimiter::Read, 0);
	g_HookManager.WriteCall(0x87753A, SocketLimiter::Read, 0);
	g_HookManager.WriteCall(0x8777C6, SocketLimiter::Read, 0);
	g_HookManager.WriteCall(0x877516, SocketLimiter::UserSocketBadPacketSizeLog);
}

UINT g_LastInvalidPacketLog = 0;

void SocketLimiter::UserSocketBadPacketSizeLog(LPVOID lpInstance, UINT logType, const WCHAR* format, UINT64 sockFunc, int packetSize)
{
	UINT currentTick = GetTickCount();
	if(g_LastInvalidPacketLog < currentTick)
	{
		g_LastInvalidPacketLog = currentTick + 5000;
		UINT threadIndex = GetThreadIndex();
		if(threadIndex < 32)
		{
			CUserSocket *pSocket = g_ThreadUserSocket[threadIndex];
			if(pSocket->ValidUserSocket())
			{
				g_Log.Add(CLog::Error, "Invalid packet size[%d] in User Socket from ip[%d.%d.%d.%d]!", packetSize, (UINT)pSocket->addr.S_un.S_un_b.s_b1, (UINT)pSocket->addr.S_un.S_un_b.s_b2, (UINT)pSocket->addr.S_un.S_un_b.s_b3, (UINT)pSocket->addr.S_un.S_un_b.s_b4);
			}
		}
	}
}

void SocketLimiter::OnRead(CUserSocket *pSocket)
{
	guard;

	UINT threadIndex = GetThreadIndex();
	if(threadIndex < 32)
	{
		g_ThreadUserSocket[threadIndex] = pSocket;
	}

	if(pSocket && pSocket->pED)
	{
		DWORD currentTick = GetTickCount();
		DWORD timeoutTick = currentTick + 30000;
		pSocket->pED->onReadTimeout = timeoutTick;
		if(pSocket->pED->ioReadTimeout < currentTick || pSocket->pED->ioReadTimeout > (DWORD)(currentTick + 1000))
		{
			pSocket->pED->ioReadCount = 0;
			pSocket->pED->ioReadTimeout = GetTickCount() + 1000;
		}
	}
	typedef void(*f)(CUserSocket*);
	f(0x876F60L)(pSocket);

	if(threadIndex < 32)
	{
		g_ThreadUserSocket[threadIndex] = 0;
	}

	unguard;
}

void SocketLimiter::Read(CUserSocket *pSocket, DWORD dwLeft)
{
	guard;

	//check for deadlock
	UINT currentTick = GetTickCount();
	if(pSocket->pED->onReadTimeout > currentTick || ((DWORD)(pSocket->pED->onReadTimeout - 30000) > currentTick))
	{
		pSocket->pED->ioReadCount = pSocket->pED->ioReadCount + 1;

		if(pSocket->pED->ioReadCount < g_IOReadPerSecond)
		{
			typedef void(*f)(CUserSocket*, DWORD);
			f(0x6205F0L)(pSocket, dwLeft);
		}else
		{
			//close socket
			g_Log.Add(CLog::Error, "[%s] UserSocket from ip[%d.%d.%d.%d] reached IOReadPerSecond limit [%d][%d] - closed!", __FUNCTION__, pSocket->addr.S_un.S_un_b.s_b1, pSocket->addr.S_un.S_un_b.s_b2, pSocket->addr.S_un.S_un_b.s_b3, pSocket->addr.S_un.S_un_b.s_b4, pSocket->pED->ioReadCount, g_IOReadPerSecond);
			pSocket->Close();
		}
	}else
	{
		//close socket
		g_Log.Add(CLog::Error, "[%s] UserSocket from ip[%d.%d.%d.%d] reached execute limit for CUserSocket::OnRead [%u][%u] - closed!", __FUNCTION__, pSocket->addr.S_un.S_un_b.s_b1, pSocket->addr.S_un.S_un_b.s_b2, pSocket->addr.S_un.S_un_b.s_b3, pSocket->addr.S_un.S_un_b.s_b4, pSocket->pED->onReadTimeout, currentTick);
		pSocket->Close();
	}

	unguard;
}

SOCKET SocketLimiter::AcceptConnection(SOCKET ListenSocket, sockaddr *addr, LPINT lpAddrLen)
{
	SOCKET s = accept(ListenSocket, addr, lpAddrLen);
	try
	{
		sockaddr_in* lpAddr = (struct sockaddr_in*)addr;
		char *szIP = inet_ntoa(lpAddr->sin_addr);
		CIPAddress ip((*lpAddr));
		
		if(!g_SafeIpList.Contain(ip)) 
		{
			
			if( g_IpBlocker.IsBlocked(szIP) )
			{
				closesocket(s);
				WSASetLastError(WSAEWOULDBLOCK);
				s = -1;
			}else if(g_BannedIpList.Contain(ip))
			{
				closesocket(s);
				WSASetLastError(WSAEWOULDBLOCK);
				s = -1;
			}else
			{
				int nSocketCount = g_SocketDB.GetIpCount(ip);
				if( nSocketCount >= nSocketLimitPerIP )
				{
					g_Log.Add(CLog::Error, "[%s] IP[%s] reached socket limit[%d]!", __FUNCTION__, szIP, nSocketLimitPerIP);
					g_Logger.Add(L"[%S] IP[%S] reached socket limit(%d) - Banned for [%d] seconds.", __FUNCTION__, szIP, nSocketLimitPerIP, nBanDuration);
					g_BannedIpList.AddIp(ip, nBanDuration);

					closesocket(s);
					WSASetLastError(WSAEWOULDBLOCK);
					s = -1;
				}
			}
		}
	}catch(...)
	{
		EXCEPTION_LOG;
	}
	return s;
}

void SocketLimiter::ValidateSend(CSocket *pSocket, const char *format, va_list va)
{
	try
	{
		if(CUserSocket *pUS = pSocket->CastUserSocket())
		{
			if( (INT64)pUS->s == -1 )
			{
				if(pUS->pED->invalidWriteTime == 0)
				{
					pUS->pED->invalidWriteTime = time(0) + 60;
				}
				if(pUS->pED->invalidWriteTime > 0 && pUS->pED->invalidWriteTime < time(0) && pUS->netPingTime < (time(0) + 30))
				{
					/*
					CHAR buff[16384];
					int len = Utils::VAssemble(buff, 16384, format, va);
					g_Log.Add(CLog::Error, "[%s] Trying to write to invalid user socket ip[%d.%d.%d.%d] - opCode[%x]!", __FUNCTION__, pUS->addr.S_un.S_un_b.s_b1, pUS->addr.S_un.S_un_b.s_b2, pUS->addr.S_un.S_un_b.s_b3, pUS->addr.S_un.S_un_b.s_b4, (BYTE)buff[0]);
					g_Log.StackTrace(CLog::Error); */
					pUS->netPingTime = time(0) + 120;
				}
				return;
			}
		}
		typedef void (*f)(CSocket*, const char *, va_list);
		f(0x7D5D40L)(pSocket, format, va);
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception format[%s]!", __FUNCTION__, format);
		g_Log.StackTrace(CLog::Error);
	}
}

void SocketLimiter::SetPendingWriteLimit(int nLimit)
{
	if(nLimit > 0)
	{
		(*lpPendingWriteLimit) = nLimit;
	}
}

bool SocketLimiter::ValidUserSocket(CSocket *pSocket)
{
	typedef bool (*f)(CSocket*);
		
	if(f(0x86C160L)(pSocket))
	{

		return true;
	}
	return false;
}

void SocketLimiter::TimerExpired()
{
	if(time(NULL) > (nLastCheck + 10))
	{
	//	FixUserSocketCount();
		nLastCheck = time(NULL);
	}
}

/*void SocketLimiter::FixUserSocketCount()
{
	size_t nSocketCount = g_SocketDB.GetSocketCount();
	if((*lpUserSockets) > (nSocketCount + 50))
	{
		(*lpUserSockets) = (int)nSocketCount;
		g_Log.Add(CLog::Blue, "[%s] Done.", __FUNCTION__);
	}
}*/