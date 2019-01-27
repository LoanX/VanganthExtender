#include "stdafx.h"
#include "AutoStart.h"

CAutoStart g_AutoStart;

CAutoStart::CAutoStart() : m_enabled(FALSE)
{

}

CAutoStart::~CAutoStart()
{
}

void CAutoStart::Init()
{
	m_enabled = FALSE;
	m_app.clear();
	LoadINI();
	if(m_enabled)
	{
		g_Log.Add(CLog::Blue, "[%S] Feature is enabled - loaded [%d] instance(s).", L"AutoStart", m_app.size());
	}
}

void CAutoStart::LoadINI()
{
	const TCHAR* section = _T("AutoStart");
	m_enabled = GetPrivateProfileInt(section, _T("Enabled"), 0, g_ConfigFile);
	INT32 amount = GetPrivateProfileInt(section, _T("Count"), 0, g_ConfigFile);
	for(INT32 n = 0;n<amount;n++)
	{
		tstringstream dirStream;
		dirStream << _T("Directory_") << n ;
		tstringstream exeStream;
		exeStream << _T("ExeName_") << n ;
		AutoStartData asd;
		memset(&asd, 0, sizeof(asd));
		GetPrivateProfileString(section, dirStream.str().c_str(), 0, asd.dir, 260, g_ConfigFile);
		GetPrivateProfileString(section, exeStream.str().c_str(), 0, asd.exeName, 260, g_ConfigFile);
		m_app.push_back(asd);
	}
}


void CAutoStart::StartApps()
{
	guard;
	if(m_enabled)
	{
		for(list<AutoStartData>::iterator Iter = m_app.begin();Iter!=m_app.end(); Iter++)
		{
			STARTUPINFO si;
			PROCESS_INFORMATION pi;
			ZeroMemory( &si, sizeof(si) );
			si.cb = sizeof(si);
			ZeroMemory( &pi, sizeof(pi) );

			WCHAR cmdLine[260];
			wcscpy_s(cmdLine, 260, Iter->dir);
			lstrcat(cmdLine, Iter->exeName);
			// Start the child process. 
			if( CreateProcess( NULL,   // No module name (use command line)
				cmdLine,        // Command line
				NULL,           // Process handle not inheritable
				NULL,           // Thread handle not inheritable
				FALSE,          // Set handle inheritance to FALSE
				0,              // No creation flags
				NULL,           // Use parent's environment block
				Iter->dir,           // Use parent's starting directory 
				&si,            // Pointer to STARTUPINFO structure
				&pi )           // Pointer to PROCESS_INFORMATION structure
			) 
			{
				g_Log.Add(CLog::Blue, "[%S] Start: [%S]", L"AutoStart", cmdLine);
				// Close process and thread handles. 
				CloseHandle( pi.hProcess );
				CloseHandle( pi.hThread );
			}
		}
	}

	unguard;
}