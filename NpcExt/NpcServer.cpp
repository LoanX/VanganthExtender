#include "stdafx.h"
#include "NpcServer.h"

CNpcServer g_NpcServer;

CNpcServer::CNpcServer()
{ 
	Instance = (CNpcServer*)0x328BBB0;
}

void CNpcServer::Send(const char* format, ...)
{
	va_list va;
	va_start(va, format);
	if(CSocket *pSocket = GetSocket())
	{
		typedef void(*f)(CSocket*, const char*, va_list);
		f(0x5E9FF0L)(pSocket, format, va);
	}else
	{
		g_Log.Add(LogError, "[%s] Null socket!", __FUNCTION__);
	}
	va_end(va);
}