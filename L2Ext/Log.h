#pragma once

class CLog
{
	CLog* pInstance;
public:
	enum LogType
	{
		Black = 0,	//SYSTEM
		Blue = 1,	//SYSTEM
		Error = 2,	//ERR
		In = 6,
		Db = 7,
		Audit = 8,
		Chat = 9
	};
	CLog();
	void Add(LogType Type, const char *format, ...);
	void Add(LogType Type, const wchar_t *format, ...);
	void StackTrace(LogType type);
};
extern CLog g_Log;

#define EXCEPTION_LOG g_Log.Add(CLog::Error, "[%s][%s][%d] Exception!", __FILE__, __FUNCTION__, __LINE__);
#define EXCEPTION_LOG_PARAM(paramString) g_Log.Add(CLog::Error, "[%s] exception occurred! - (%s)", __FUNCTION__, paramString)