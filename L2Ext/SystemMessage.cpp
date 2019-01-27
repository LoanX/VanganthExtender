#include "stdafx.h"

CSystemMessage::CSystemMessage(int msgId)
{
	memset(Buff, 0, 8192);
	size = 0;
	nVars = 0;
	Buff[0] = 0x64;
	size++;
	try
	{
		(*(LPDWORD)(&Buff[size])) = msgId;
		size+=4;
		//Amount of vars
		lpVars = (LPDWORD)&Buff[size];
		size+=4;
		(*lpVars) = nVars;
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception detected - msg[%d]", __FUNCTION__, msgId);
	}
};

void CSystemMessage::AddText(const wchar_t* wsText)
{
	try
	{
		(*(LPDWORD)(&Buff[size])) = TEXT;
		size+=4;
		UINT nLen = static_cast<UINT>(wcslen(wsText));
		nLen *= 2; nLen += 2;
		wcscpy_s((PWCHAR)(&Buff[size]), (8192 - size), wsText);
		size += nLen;
		nVars++;
		(*lpVars) = nVars;
	}
	catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception detected - wsText[%S]", __FUNCTION__, wsText);
	}
};
void CSystemMessage::AddNumber(DWORD nNumber)
{
	try
	{
		*(LPDWORD)(&Buff[size]) = NUMBER;
		size+=4;
		*(LPDWORD)(&Buff[size]) = nNumber;
		size+=4;
		nVars++;
		(*lpVars) = nVars;
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception detected.", __FUNCTION__);
	}
};
void CSystemMessage::AddNpc(int nClassID)
{
	try
	{
		*(LPDWORD)(&Buff[size]) = NPC_NAME;
		size+=4;
		*(LPDWORD)(&Buff[size]) = nClassID;
		size+=4;
		nVars++;
		(*lpVars) = nVars;
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception detected.", __FUNCTION__);
	}
};
void CSystemMessage::AddItem(int nClassID)
{
	try
	{
		*(LPDWORD)(&Buff[size]) = ITEM_NAME;
		size+=4;
		*(LPDWORD)(&Buff[size]) = nClassID;
		size+=4;
		nVars++;
		(*lpVars) = nVars;
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception detected.", __FUNCTION__);
	}
};
void CSystemMessage::AddSkill(int nSkillID, int nLevel)
{
	try
	{
		*(LPDWORD)(&Buff[size]) = SKILL_NAME;
		size+=4;
		*(LPDWORD)(&Buff[size]) = nSkillID;
		size+=4;
		*(LPDWORD)(&Buff[size]) = nLevel;
		size+=4;
		nVars++;
		(*lpVars) = nVars;
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception detected.", __FUNCTION__);
	}
};
void CSystemMessage::AddZone(int x, int y, int z)
{
	try
	{
		*(LPDWORD)(&Buff[size]) = ZONE_NAME;
		size+=4;
		*(LPDWORD)(&Buff[size]) = x;
		size+=4;
		*(LPDWORD)(&Buff[size]) = y;
		size+=4;
		*(LPDWORD)(&Buff[size]) = z;
		size+=4;
		nVars++;
		(*lpVars) = nVars;
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception detected.", __FUNCTION__);
	}
};
PCHAR CSystemMessage::GetBuff()
{
	return Buff;
};
UINT CSystemMessage::GetSize()
{
	return size;
};