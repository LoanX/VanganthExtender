#include "StdAfx.h"
#include "PlayerCustomizer.h"

CPlayerCustomizer g_PlayerCustomizer;

UINT g_GMNicknameColor = 0;
UINT g_GMTitleColor = 0;
UINT g_DefaultTitleColor = 15200924;

void CPlayerCustomizer::Init()
{
	const TCHAR* section = _T("PlayerCustomizer");
	//reading data
	m_Enabled = GetPrivateProfileInt(section, _T("Enabled"), 0, g_ConfigFile);
	if(m_Enabled)
	{
		TCHAR sString[8192];	memset(sString, 0, sizeof(sString));
		//PvPTitleColor=0:15597568 5:3937500
		//format: pvp_count:access_colour_code
		if(GetPrivateProfileString(section, _T("PvPTitleColor"), 0, sString, sizeof(sString), g_ConfigFile))
		{
			tstringstream sstr; sstr << sString;
			tstring sValue;
			while(sstr >> sValue)
			{
				size_t pos = sValue.find(_T(":"));
				tstring sCount = sValue.substr(0, pos);
				int nCount = _tstoi(sCount.c_str());
				DWORD nColor = _tstoi(sValue.substr(pos+1, (sValue.size() - pos - 1)).c_str());
				SetPvpTitleColor(nCount, nColor);
			}
		}
		g_GMNicknameColor = GetPrivateProfileInt(section, _T("GMNicknameColor"), 15200924, g_ConfigFile);
		g_GMTitleColor = GetPrivateProfileInt(section, _T("GMTitleColor"), 15200924, g_ConfigFile);
		g_DefaultTitleColor = GetPrivateProfileInt(section, _T("DefaultTitleColor"), 15200924, g_ConfigFile);

		if(m_PvpTitleColor.size() > 0)
		{
			g_Log.Add(CLog::Blue, "[%s] Custom PvP Title [%d] - Done.", __FUNCTION__, m_PvpTitleColor.size());
		}
	}
}

void CPlayerCustomizer::SetPvpTitleColor(int nPvpCount, DWORD nColor)
{
	m_PvpTitleColor[nPvpCount] = nColor;
}

DWORD CPlayerCustomizer::GetPvpTitleColor(int nPvpCount)
{
	DWORD nColor = g_DefaultTitleColor; //default color
	for(map<int, DWORD>::iterator Iter=m_PvpTitleColor.begin();Iter!=m_PvpTitleColor.end();Iter++)
	{
		if(Iter->first <= nPvpCount)
		{
			nColor = Iter->second;
		}else
		{
			break;
		}
	}
	return nColor;
}