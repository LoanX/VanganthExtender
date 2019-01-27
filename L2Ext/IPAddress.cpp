#include "StdAfx.h"

CIPAddress::CIPAddress(void)
{
	memset(m_IP, 0, sizeof(m_IP));
}

CIPAddress::~CIPAddress(void)
{
}

CIPAddress::CIPAddress(string sIP)
{
	memset(m_IP, 0, sizeof(m_IP));
	sIP = Utils::ReplaceStringA(sIP, "*", "257", true);
	sIP = Parser::Replace(sIP, '.', ' ');
	stringstream str(sIP);
	str >> m_IP[0] >> m_IP[1] >> m_IP[2] >> m_IP[3];
}

CIPAddress::CIPAddress(wstring sIP)
{
	memset(m_IP, 0, sizeof(m_IP));
	sIP = Utils::ReplaceStringW(sIP, L"*", L"257", true);
	sIP = Parser::Replace(sIP, L'.', L' ');
	wstringstream str(sIP);
	str >> m_IP[0] >> m_IP[1] >> m_IP[2] >> m_IP[3];
}

CIPAddress::CIPAddress(sockaddr_in addr)
{
	m_IP[0] = addr.sin_addr.S_un.S_un_b.s_b1;
	m_IP[1] = addr.sin_addr.S_un.S_un_b.s_b2;
	m_IP[2] = addr.sin_addr.S_un.S_un_b.s_b3;
	m_IP[3] = addr.sin_addr.S_un.S_un_b.s_b4;
}

CIPAddress::CIPAddress(in_addr addr)
{
	m_IP[0] = addr.S_un.S_un_b.s_b1;
	m_IP[1] = addr.S_un.S_un_b.s_b2;
	m_IP[2] = addr.S_un.S_un_b.s_b3;
	m_IP[3] = addr.S_un.S_un_b.s_b4;
}

bool CIPAddress::operator==(const CIPAddress &other) const
{
	for(int n = 0; n < 4; n++)
	{
		if(m_IP[n] != other.m_IP[n] && m_IP[n] != 257 && other.m_IP[n] != 257)
		{
			return false;
		}
	}

	return true;
}

bool CIPAddress::operator!=(const CIPAddress &other) const
{
	return !( (*this)==other );
}

string CIPAddress::ToString() const
{
	stringstream ipStream;
	ipStream << m_IP[0] << "." << m_IP[1] << "." << m_IP[2] << "." << m_IP[3];
	return ipStream.str();
}

wstring CIPAddress::ToWString() const
{
	wstringstream ipStream;
	ipStream << m_IP[0] << "." << m_IP[1] << "." << m_IP[2] << "." << m_IP[3];
	return ipStream.str();
}