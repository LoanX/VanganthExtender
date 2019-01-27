#pragma once

struct AutoStartData
{
	TCHAR dir[260];
	TCHAR exeName[260];
};

class CAutoStart
{
	BOOL m_enabled;
	list<AutoStartData> m_app;
	void LoadINI();
public:
	CAutoStart();
	~CAutoStart();
	void Init();
	void StartApps();
};

extern CAutoStart g_AutoStart;