#pragma once

class CLog
{
	LPVOID m_lpOrgInstance;
public:
	CLog();
	~CLog();
	void Add(LogType type, const CHAR* format, ...);
	void Add(LogType type, const WCHAR* format, ...);
};

extern CLog g_Log;