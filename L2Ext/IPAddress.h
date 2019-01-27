#pragma once


class CIPAddress
{
	unsigned short m_IP[4];
public:
	CIPAddress(void);
	~CIPAddress(void);
	CIPAddress(string sIP);
	CIPAddress(wstring sIP);
	CIPAddress(sockaddr_in addr);
	CIPAddress(in_addr addr);
	string ToString() const;
	wstring ToWString() const;
	bool operator==(const CIPAddress &other) const;
	bool operator!=(const CIPAddress &other) const;
};