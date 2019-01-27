#pragma once

class CSafeIpList
{
	CSLock m_Lock;
	vector<CIPAddress> m_Data;
public:
	size_t GetSize();
	void AddIp(const CIPAddress& ip);
	bool Contain(const CIPAddress& ip) const;
};

class CBannedIpList
{
	typedef struct
	{
		CIPAddress ip;
		time_t banEndTime;
	} BanIpData;
	typedef vector<BanIpData> BanVector;
	CSLock m_Lock;
	BanVector m_Data;
public:
	void AddIp(const CIPAddress& ip, int banDuration);
	bool Contain(const CIPAddress& ip);
};

extern CSafeIpList g_SafeIpList;
extern CBannedIpList g_BannedIpList;