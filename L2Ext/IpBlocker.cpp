#include "StdAfx.h"
#include "IpBlocker.h"

CIpBlocker g_IpBlocker;

void CIpBlocker::Initialize()
{
	ReadData();
}

void CIpBlocker::ReadData()
{
	ifstream file("BlockedIp.txt");
	int nIpCount = 0;
	if(file)
	{
		string sLine;
		lData.clear();
		while(getline(file, sLine))
		{
			sLine = CParser::Trim(sLine);

			if(sLine.find("//") == 0)
				continue;

			CIPAddress Ip(sLine);
			lData.push_back(Ip);
			nIpCount++;
		}
		file.close();
	}
	g_Log.Add(CLog::Blue, "[%s] Loaded [%d] blocked ip.", __FUNCTION__, nIpCount);
}

bool CIpBlocker::IsBlocked(string sIp)
{
	CIPAddress Ip(sIp);
	for(vector<CIPAddress>::iterator Iter = lData.begin(); Iter!=lData.end();Iter++)
	{
		if(*Iter == Ip)
			return true;
	}
	return false;
}

bool CIpBlocker::IsBlocked(CIPAddress Ip)
{
	for(vector<CIPAddress>::iterator Iter = lData.begin(); Iter!=lData.end();Iter++)
	{
		if(*Iter == Ip)
			return true;
	}
	return false;
}