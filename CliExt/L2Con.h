#pragma once

struct ConAddr
{
	string addrStr1;
	UINT addr1;
	bool valid1;
	string addrStr2;
	UINT addr2;
	bool valid2;
	ConAddr() : addr1(0), addr2(0), valid1(false), valid2(false) {};
};

struct ConUrl
{
	string host;
	string path;
};

class CL2Con
{
	CSLock m_Lock;
	vector<ConAddr> m_Template;
	vector<ConUrl> m_TemplateUrl;
	UINT m_Address;
	string m_AddressStr;
	UINT m_Dice;
	BOOL m_Loaded;
	UINT m_TestCount;
	UINT m_AuthPort;
	BOOL m_Initialized;

	void LoadINI(wstring path);
	void GetLoginIpPort();
	void ResolveAddr();
	UINT ResolveAddrHelper(string addrStr, bool& valid);

public:
	CL2Con() : m_Initialized(FALSE) {};
	inline BOOL IsInitialized() { return m_Initialized; };
	inline BOOL IsLoaded() { return m_Loaded; };
	void Init(wstring path, UINT authPort);
	UINT GetWorkingAddress();
	UINT GetWorkingPort();
};

extern CL2Con g_L2Con;