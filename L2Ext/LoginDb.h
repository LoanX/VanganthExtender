#pragma once


extern "C"
{
	void LoginFix();
};

struct LoginData
{
	wstring accountName;
	BOOL used;
	time_t timestamp;
};

class CLoginDb
{
	CSLock m_Lock;
	map<UINT, LoginData> m_Accounts;
public:
	void Init();
	void SetLoginData(UINT accountId, wstring accountName);
	UINT CanLogin(UINT accountId, wstring accountName);
	static bool OnAuthRequestAboutToPlay(PVOID pSocket, const unsigned char* packet);
};

extern CLoginDb g_LoginDb;