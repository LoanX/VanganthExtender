#include "stdafx.h"
#include "DnN.h"

using namespace DnN;

CDnNSystem g_DnNSystem;

CDnNSystem::CDnNSystem() : m_Enabled(FALSE)
{
};

CDnNSystem::~CDnNSystem()
{
};

void CDnNSystem::Init()
{
	m_Enabled = FALSE;
	LoadINI();
	if(m_Enabled)
	{
		g_Log.Add(CLog::Blue, "Day and Night system is enabled.");
	}
}

void CDnNSystem::LoadINI()
{
	TCHAR file[260];
	GetCurrentDirectory(MAX_PATH, file);
	lstrcat(file, TEXT("\\DnN.ini"));
	const TCHAR* section = _T("Setting");
	m_Enabled = GetPrivateProfileInt(section, _T("Enabled"), FALSE, file);

	m_Rate[TimeDay][RateExp] = GetPrivateProfileDouble(section, _T("DayExp"), 1.0, file);
	m_Rate[TimeDay][RateSp] = GetPrivateProfileDouble(section, _T("DaySp"), 1.0, file);
	m_Rate[TimeDay][RateDrop] = GetPrivateProfileDouble(section, _T("DayDrop"), 1.0, file);
	m_Rate[TimeDay][RateSpoil] = GetPrivateProfileDouble(section, _T("DaySpoil"), 1.0, file);
	m_Rate[TimeDay][RateAdena] = GetPrivateProfileDouble(section, _T("DayAdena"), 1.0, file);
	
	m_Rate[TimeNight][RateExp] = GetPrivateProfileDouble(section, _T("NightExp"), 1.0, file);
	m_Rate[TimeNight][RateSp] = GetPrivateProfileDouble(section, _T("NightSp"), 1.0, file);
	m_Rate[TimeNight][RateDrop] = GetPrivateProfileDouble(section, _T("NightDrop"), 1.0, file);
	m_Rate[TimeNight][RateSpoil] = GetPrivateProfileDouble(section, _T("NightSpoil"), 1.0, file);
	m_Rate[TimeNight][RateAdena] = GetPrivateProfileDouble(section, _T("NightAdena"), 1.0, file);
}

double CDnNSystem::GetRate(RateType type)
{
	if(m_Enabled)
	{
		TimeType timeType = TimeDay;
		if(g_L2Time.IsNight())
		{
			timeType = TimeNight;
		}
		return m_Rate[timeType][type];
	}
	return 1.0;
}

void CDnNSystem::TimerExpired()
{
	guard;
	if(m_Enabled)
	{

	}
	unguard;
}