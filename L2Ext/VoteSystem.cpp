#include "stdafx.h"
#include "VoteSystem.h"

CVoteSystem g_VoteSystem;

CVoteSystem::CVoteSystem() : m_enabled(FALSE)
{
	InitializeCriticalSection(&m_lock);
}

CVoteSystem::~CVoteSystem()
{
	DeleteCriticalSection(&m_lock);
}

void CVoteSystem::Init()
{
	guard;
	Lock();
	m_votePages.clear();
	m_enabled = FALSE;
	LoadINI();
	if(m_enabled)
	{
		g_Log.Add(CLog::Blue, "Vote System is enabled - loaded [%d] vote pages", m_votePages.size());
	}
	Unlock();
	unguard;
}

void CVoteSystem::LoadINI()
{
	const TCHAR* section = _T("VoteSystem");
	TCHAR file[260];
	GetCurrentDirectory(MAX_PATH,file);
	lstrcat(file, TEXT("\\Vote.ini"));
	m_enabled = GetPrivateProfileInt(section, _T("Enabled"), 0, file);
	TCHAR temp[8192];
	if(GetPrivateProfileString(section, _T("SectionList"), 0, temp, 8192, file))
	{
		tstringstream sstr;
		sstr << temp;
		TCHAR section[8192];
		while( sstr >> section)
		{
			CVotePage *lpVotePage = new CVotePage();
			if(lpVotePage)
			{
				lpVotePage->Init(section);
				m_votePages.push_back(lpVotePage);
			}else
			{
				Utils::ShowMessage(_T("Vote System"), _T("Cannot allocate memory for Vote Page!"));
			}
		}
	}
}

void CVoteSystem::TimerExpired()
{
	guard;
	if(m_enabled)
	{
		Lock();
		for(list<CVotePage*>::iterator Iter = m_votePages.begin(); Iter != m_votePages.end(); Iter++)
		{
			CVotePage *pPage = (*Iter);
			if(pPage)
			{
				pPage->TimerExpired();
			}
		}
		Unlock();
	}
	unguard;
}