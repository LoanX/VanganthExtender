#pragma once

struct LockCharData
{
	UINT charId;
	INT32 hwid;
};

struct AccountData
{
	CUserSocket *pSocket;
	int vipLevel;
	DWORD vipTimestamp;
	DWORD banTimestamp;
	AccountData() : banTimestamp(0), pSocket(0), vipLevel(0), vipTimestamp(0) {};
};

class CAccountDB : private CThreadSafe
{
	map<UINT, AccountData> m_data;
	CSLock m_bindLock;
	map<UINT, HWID> m_lockedAccounts;
	map<UINT, HWID> m_lockedCharacters;
	CSLock m_additionalBoxLock;
	map<UINT, UINT> m_additionalBoxAccounts;
public:
	CAccountDB();
	~CAccountDB();
	void SetVIPLevel(UINT accountId, INT32 level, DWORD timestamp);
	INT32 GetVIPLevel(UINT accountId);
	void SetBan(UINT accountId, DWORD timestamp);
	UINT IsBanned(UINT accountId);
	void Add(UINT accountId, CUserSocket *pSocket);
	void Ban(UINT accountId, const WCHAR* accountName, DWORD timeout, const WCHAR* reason = L"");
	HWID GetLockedHwid(UINT accountId);
	bool LockHwid(UINT accountId, LPBYTE lpHwid, WCHAR* wName);
	bool UnlockHwid(UINT accountId, WCHAR* wName);
	HWID GetLockedCharHwid(UINT charId);
	bool LockHwidChar(UINT charId, LPBYTE lpHwid, WCHAR* wName);
	bool UnlockHwidChar(UINT charId, WCHAR* wName);
	void DBLockCharacter(UINT charId, LPBYTE lpHwid);
	void DBUnlockCharacter(UINT charId);
	void DBLockAccount(UINT accountId, LPBYTE lpHwid);
	void DBUnlockAccount(UINT accountId);
	bool SetAdditionalBox(UINT accountId, UINT timestamp);
	bool IsAdditionalBox(UINT accountId);
	void DBSetAdditionalBox(UINT accountId, UINT timestamp);
};

extern CAccountDB g_AccountDB;