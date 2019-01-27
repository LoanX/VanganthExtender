#include "StdAfx.h"
#include "IpList.h"

CSafeIpList g_SafeIpList;
CBannedIpList g_BannedIpList;

size_t CSafeIpList::GetSize()
{
	return m_Data.size();
}

void CSafeIpList::AddIp(const CIPAddress& ip)
{
	for(size_t n=0;n<m_Data.size();n++)
	{
		CIPAddress& safeIp = m_Data[n];
		if(safeIp == ip)
		{
			return;
		}
	}
	m_Data.push_back(ip);
};

bool CSafeIpList::Contain(const CIPAddress& ip) const
{
	for(size_t n=0;n<m_Data.size();n++)
	{
		const CIPAddress& safeIp = m_Data[n];
		if(safeIp == ip)
		{
			return true;
		}
	}
	return false;
};

void CBannedIpList::AddIp(const CIPAddress& ip, int banDuration)
{
	time_t timeout = time(0) + banDuration;
	bool updated = false;
	{
		AUTO_LOCK(m_Lock);
		for(size_t n = 0; n < m_Data.size(); n++)
		{
			if(m_Data[n].ip == ip)
			{
				m_Data[n].banEndTime = timeout;
				updated = true;
			}
		}
		if(!updated)
		{
			BanIpData data;
			data.ip = ip;
			data.banEndTime = time(0) + banDuration;
			m_Data.push_back(data);
		}
	}
};

bool CBannedIpList::Contain(const CIPAddress& ip)
{
	bool result = false;
	time_t currentTime = time(0);
	{
		AUTO_LOCK(m_Lock);
		for(size_t n = 0; n < m_Data.size(); n++)
		{
			if(m_Data[n].ip == ip)
			{
				if(currentTime < m_Data[n].banEndTime)
				{
					result = true;
				}
				break;
			}
		}
	}
	return result;
}