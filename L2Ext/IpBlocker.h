#pragma once

class CIpBlocker : private CThreadSafe
{
	vector<CIPAddress> lData;
public:
	void Initialize();
	void ReadData();
	bool IsBlocked(string sIp);
	bool IsBlocked(CIPAddress Ip);
};

extern CIpBlocker g_IpBlocker;