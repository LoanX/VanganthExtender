#include "StdAfx.h"
#include "AccountDB.h"
#include "DB.h"

CAccountDB g_AccountDB;

CAccountDB::CAccountDB()
{
}

CAccountDB::~CAccountDB()
{
}

INT32 CAccountDB::GetVIPLevel(UINT accountId)
{
	int vipLevel = 0;
	Lock();
	try
	{
		map<UINT, AccountData>::iterator Iter = m_data.find(accountId);
		if(Iter!=m_data.end())
		{
			if(time(NULL) < Iter->second.vipTimestamp)
			{
				vipLevel = Iter->second.vipLevel;
			}
		}else
		{
			//ToDO:
			//g_DB.RequestLoadVipStatus(accountId);
		}
	}catch(...)
	{
		EXCEPTION_LOG;
	}
	Unlock();
	return vipLevel;
}

void CAccountDB::SetVIPLevel(UINT accountId, INT32 level, DWORD timestamp)
{
	Lock();
	try
	{
		map<UINT, AccountData>::iterator Iter = m_data.find(accountId);
		if(Iter!=m_data.end())
		{
			Iter->second.vipLevel = level;
			Iter->second.vipTimestamp = timestamp;
		}else
		{
			AccountData ad;
			ad.pSocket = 0;
			ad.vipLevel = level;
			ad.vipTimestamp = timestamp;
			m_data.insert(pair<UINT, AccountData>(accountId, ad));
		}
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception!", __FUNCTION__);
	}
	Unlock();
}

void CAccountDB::Add(UINT accountId, CUserSocket *pSocket)
{
	Lock();
	try
	{
		map<UINT, AccountData>::iterator Iter = m_data.find(accountId);
		if(Iter!=m_data.end())
		{
			Iter->second.pSocket = pSocket;
		}else
		{
			AccountData ad;
			ad.pSocket = pSocket;
			ad.vipLevel = 0;
			ad.vipTimestamp = 0;
			ad.banTimestamp = 0;
			m_data.insert(pair<UINT, AccountData>(accountId, ad));
		}
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception!", __FUNCTION__);
	}
	Unlock();
}

void CAccountDB::SetBan(UINT accountId, DWORD timestamp)
{
	Lock();
	try
	{
		map<UINT, AccountData>::iterator Iter = m_data.find(accountId);
		if(Iter!=m_data.end())
		{
			Iter->second.banTimestamp = timestamp;
		}else
		{
			AccountData ad;
			ad.pSocket = 0;
			ad.vipLevel = 0;
			ad.vipTimestamp = 0;
			ad.banTimestamp = timestamp;
			m_data.insert(pair<UINT, AccountData>(accountId, ad));
		}
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception!", __FUNCTION__);
	}
	Unlock();
}

UINT CAccountDB::IsBanned(UINT accountId)
{
	UINT isBanned = 0;
	Lock();
	try
	{
		
		map<UINT, AccountData>::iterator Iter = m_data.find(accountId);
		if(Iter!=m_data.end())
		{
			if(Iter->second.banTimestamp > time(0))
			{
				isBanned = Iter->second.banTimestamp;
			}
		}
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception!", __FUNCTION__);
	}
	Unlock();

	return isBanned;
}

void CAccountDB::Ban(UINT accountId, const WCHAR* accountName, DWORD timeout, const WCHAR* reason)
{
	UINT timestamp = time(0) + timeout;
	if(timeout == 0)
	{
		timestamp = 0;
	}

	bool remove = false;

	Lock();
	map<UINT, AccountData>::iterator Iter = m_data.find(accountId);
	if(Iter!=m_data.end())
	{
		Iter->second.banTimestamp = timestamp;
		if(timestamp == 0)
		{
			remove = true;
		}
	}else
	{
		AccountData ad;
		ad.pSocket = 0;
		ad.vipLevel = 0;
		ad.vipTimestamp = 0;
		ad.banTimestamp = timestamp;
		m_data.insert(pair<UINT, AccountData>(accountId, ad));
	}
	Unlock();
	if(timestamp == 0)
	{
		if(remove)
		{
			g_DB.RequestBanAccount(accountId, L"", 0, L"Ban Removed");
		}
	}else
	{
		g_Log.Add(CLog::Blue, "Banned account[%d][%S] reason[%S] timeout[%d] second(s)", accountId, accountName, reason, timeout);
		g_DB.RequestBanAccount(accountId, accountName, timestamp, reason);
	}
}

HWID CAccountDB::GetLockedHwid(UINT accountId)
{
	guard;
	HWID hwid;
	m_bindLock.Enter();
	map<UINT, HWID>::iterator it = m_lockedAccounts.find(accountId);
	if(it!=m_lockedAccounts.end())
	{
		hwid = it->second;
	}
	m_bindLock.Leave();
	unguard;
	return hwid;
};

bool CAccountDB::LockHwid(UINT accountId, LPBYTE lpHwid, WCHAR* wName)
{
	guard;
	bool succeed = false;
	HWID hwid(lpHwid);
	m_bindLock.Enter();
	map<UINT, HWID>::iterator it = m_lockedAccounts.find(accountId);
	if(it==m_lockedAccounts.end())
	{
		m_lockedAccounts.insert(pair<UINT, HWID>(accountId, hwid));
		succeed = true;
	}
	m_bindLock.Leave();
	if(succeed)
	{
		g_DB.RequestLockAccount(accountId, lpHwid, wName);
		g_Logger.Add(L"User[%s] locked account[%d] hwid[%s]", wName, accountId, hwid.ToString().c_str());
	}
	unguard;
	return succeed;
}

bool CAccountDB::UnlockHwid(UINT accountId, WCHAR* wName)
{
	guard;
	bool succeed = false;
	m_bindLock.Enter();
	map<UINT, HWID>::iterator Iter = m_lockedAccounts.find(accountId);
	if(Iter!=m_lockedAccounts.end())
	{
		m_lockedAccounts.erase(Iter);
		succeed = true;
	}
	m_bindLock.Leave();
	if(succeed)
	{
		g_DB.RequestUnlockAccount(accountId);
		g_Logger.Add(L"User[%s] unlocked account[%d]", wName, accountId);
	}
	unguard;
	return succeed;
}

HWID CAccountDB::GetLockedCharHwid(UINT charId)
{
	guard;
	HWID hwid;
	m_bindLock.Enter();
	map<UINT, HWID>::iterator Iter = m_lockedCharacters.find(charId);
	if(Iter!=m_lockedCharacters.end())
	{
		hwid = Iter->second;
	}
	m_bindLock.Leave();
	unguard;
	return hwid;
}

bool CAccountDB::LockHwidChar(UINT charId, LPBYTE lpHwid, WCHAR* wName)
{
	guard;
	bool succeed = false;
	HWID hwid(lpHwid);
	m_bindLock.Enter();
	map<UINT, HWID>::iterator Iter = m_lockedCharacters.find(charId);
	if(Iter==m_lockedCharacters.end())
	{
		m_lockedCharacters.insert(pair<UINT, HWID>(charId, hwid));
		succeed = true;
	}
	m_bindLock.Leave();
	if(succeed)
	{
		g_DB.RequestLockChar(charId, lpHwid, wName);
		g_Logger.Add(L"User[%s] locked char[%d] hwid[%s]", wName, charId, hwid.ToString().c_str());
	}
	unguard;
	return succeed;
}

bool CAccountDB::UnlockHwidChar(UINT charId, WCHAR* wName)
{
	guard;
	bool succeed = false;
	m_bindLock.Enter();
	map<UINT, HWID>::iterator Iter = m_lockedCharacters.find(charId);
	if(Iter!=m_lockedCharacters.end())
	{
		m_lockedCharacters.erase(Iter);
		succeed = true;
	}
	m_bindLock.Leave();
	if(succeed)
	{
		g_DB.RequestUnlockChar(charId);
		g_Logger.Add(L"User[%s] unlocked char[%d]", wName, charId);
	}
	unguard;
	return succeed;
}

void CAccountDB::DBLockCharacter(UINT charId, LPBYTE lpHwid)
{
	guard;
	HWID hwid(lpHwid);
	m_bindLock.Enter();
	m_lockedCharacters[charId] = hwid;
	m_bindLock.Leave();
	unguard;
}

void CAccountDB::DBUnlockCharacter(UINT charId)
{
	guard;
	m_bindLock.Enter();
	map<UINT, HWID>::iterator it = m_lockedCharacters.find(charId);
	if(it != m_lockedCharacters.end())
	{
		m_lockedCharacters.erase(it);
	}
	m_bindLock.Leave();
	unguard;
}

void CAccountDB::DBLockAccount(UINT accountId, LPBYTE lpHwid)
{
	guard;
	HWID hwid(lpHwid);
	Lock();
	m_lockedAccounts[accountId] = hwid;
	Unlock();
	unguard;
}

void CAccountDB::DBUnlockAccount(UINT accountId)
{
	guard;
	m_bindLock.Enter();
	map<UINT, HWID>::iterator it = m_lockedAccounts.find(accountId);
	if(it != m_lockedAccounts.end())
	{
		m_lockedAccounts.erase(it);
	}
	m_bindLock.Leave();
	unguard;
}

bool CAccountDB::SetAdditionalBox(UINT accountId, UINT timestamp)
{
	guard;
	bool ret = false;
	m_additionalBoxLock.Enter();
	if(timestamp > 0)
	{
		m_additionalBoxAccounts.insert(pair<UINT, UINT>(accountId, timestamp));
		ret = true;
	}else
	{
		map<UINT, UINT>::iterator it = m_additionalBoxAccounts.find(accountId);
		if(it != m_additionalBoxAccounts.end())
		{
			m_additionalBoxAccounts.erase(it);
			ret = true;
		}
	}
	m_additionalBoxLock.Leave();
	g_DB.RequestSetAdditionalBox(accountId, timestamp);
	g_Logger.Add(L"Setting additional box accountId[%d] timestamp[%d]", accountId, timestamp);
	unguard;
	return ret;
}

bool CAccountDB::IsAdditionalBox(UINT accountId)
{
	guard;
	bool ret = false;
	UINT toRemove = 0;
	m_additionalBoxLock.Enter();
	map<UINT, UINT>::iterator it = m_additionalBoxAccounts.find(accountId);
	if(it != m_additionalBoxAccounts.end())
	{
		if(it->second > time(0))
		{
			ret = true;
		}else
		{
			m_additionalBoxAccounts.erase(it);
			toRemove = accountId;
		}
	}
	m_additionalBoxLock.Leave();

	if(toRemove > 0)
	{
		g_DB.RequestSetAdditionalBox(toRemove, 0);
	}

	unguard;
	return ret;
}

void CAccountDB::DBSetAdditionalBox(UINT accountId, UINT timestamp)
{
	guard;
	m_additionalBoxLock.Enter();
	if(timestamp > 0)
	{
		m_additionalBoxAccounts.insert(pair<UINT, UINT>(accountId, timestamp));
	}else
	{
		map<UINT, UINT>::iterator it = m_additionalBoxAccounts.find(accountId);
		if(it != m_additionalBoxAccounts.end())
		{
			m_additionalBoxAccounts.erase(it);
		}
	}
	m_additionalBoxLock.Leave();
	unguard;
}